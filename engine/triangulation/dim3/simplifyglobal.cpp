
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <cstdlib>
#include "triangulation/dim3.h"

// Affects the number of random 4-4 moves attempted during simplification.
#define COEFF_4_4 5

namespace regina {

bool Triangulation<3>::intelligentSimplify() {
    bool changed;

    { // Begin scope for change event block.
        ChangeEventSpan span(this);

        // Reduce to a local minimum.
        changed = simplifyToLocalMinimum(true);

        // Clone to work with when we might want to roll back changes.
        Triangulation<3>* use;

        // Variables used for selecting random 4-4 moves.
        std::vector<std::pair<Edge<3>*, int> > fourFourAvailable;
        std::pair<Edge<3>*, int> fourFourChoice;

        unsigned long fourFourAttempts;
        unsigned long fourFourCap;

        Edge<3>* edge;
        EdgeIterator eit;
        int axis;

        while (true) {
            // --- Random 4-4 moves ---

            // Clone the triangulation and start making changes that might or
            // might not lead to a simplification.
            // If we've already simplified then there's no need to use a
            // separate clone since we won't need to undo further changes.
            use = (changed ? this : new Triangulation<3>(*this, false));

            // Make random 4-4 moves.
            fourFourAttempts = fourFourCap = 0;
            while (true) {
                // Calculate the list of available 4-4 moves.
                fourFourAvailable.clear();
                // Use edges() to ensure the skeleton has been calculated.
                for (eit = use->edges().begin();
                        eit != use->edges().end(); eit++) {
                    edge = *eit;
                    for (axis = 0; axis < 2; axis++)
                        if (use->fourFourMove(edge, axis, true, false))
                            fourFourAvailable.push_back(
                                std::make_pair(edge, axis));
                }

                // Increment fourFourCap if needed.
                if (fourFourCap < COEFF_4_4 * fourFourAvailable.size())
                    fourFourCap = COEFF_4_4 * fourFourAvailable.size();

                // Have we tried enough 4-4 moves?
                if (fourFourAttempts >= fourFourCap)
                    break;

                // Perform a random 4-4 move on the clone.
                fourFourChoice = fourFourAvailable[
                    static_cast<unsigned>(rand()) % fourFourAvailable.size()];
                use->fourFourMove(fourFourChoice.first, fourFourChoice.second,
                    false, true);

                // See if we can simplify now.
                if (use->simplifyToLocalMinimum(true)) {
                    // We have successfully simplified!
                    // Start all over again.
                    fourFourAttempts = fourFourCap = 0;
                } else
                    fourFourAttempts++;
            }

            // Sync the real triangulation with the clone if appropriate.
            if (use != this) {
                // At this point, changed == false.
                if (use->size() < size()) {
                    // The 4-4 moves were successful; accept them.
                    swapContents(*use);
                    changed = true;
                }
                delete use;
            }

            // At this point we have decided that 4-4 moves will help us
            // no more.

            // --- Open book and close book moves ---

            if (hasBoundaryTriangles()) {
                // Clone again, always -- we don't want to create gratuitous
                // boundary triangles if they won't be of any help.
                use = new Triangulation<3>(*this, false);

                // Perform every book opening move we can find.
                TriangleIterator fit;

                bool opened = false;
                bool openedNow = true;
                while (openedNow) {
                    openedNow = false;

                    for (fit = use->triangles().begin();
                            fit != use->triangles().end(); ++fit)
                        if (use->openBook(*fit, true, true)) {
                            opened = openedNow = true;
                            break;
                        }
                }

                // If we're lucky, we now have an edge that we can collapse.
                if (opened) {
                    if (use->simplifyToLocalMinimum(true)) {
                        // Yay!
                        swapContents(*use);
                        changed = true;
                    } else {
                        // No good.
                        // Ditch use and don't open anything.
                        opened = false;
                    }
                }

                delete use;

                // If we did any book opening stuff, start all over again.
                if (opened)
                    continue;

                // If we've made it this far then there seems to be
                // nothing left to do.
                //
                // Perform book *closing* moves to simplify the boundary
                // of the triangulation, even if this does not actually
                // reduce the number of tetrahedra.
                //
                // Since we always want to simplify the boundary, make
                // the changes directly to this triangulation.
                bool closed = false;

                EdgeIterator eit;
                for (eit = edges().begin(); eit != edges().end(); ++eit)
                    if (closeBook(*eit, true, true)) {
                        closed = true;
                        changed = true;

                        // We don't actually care whether we reduce the
                        // number of tetrahedra or not.  Ignore the
                        // return value from simplifyToLocalMinimum().
                        simplifyToLocalMinimum(true);

                        break;
                    }

                // If we *did* manage to close a book, there might be
                // further internal simplifications that we can now do.
                // Back to the top.
                if (closed)
                    continue;
            }

            // Nothing more we can do here.
            break;
        }
    } // End scope for change event span.

    return changed;
}

bool Triangulation<3>::simplifyToLocalMinimum(bool perform) {
    unsigned long nTriangles;
    unsigned long iTriangle;
    // unsigned long nEdges;
    // unsigned long iEdge;
    // std::deque<EdgeEmbedding<3>>::const_iterator embit, embbeginit, embendit;

    bool changed = false;   // Has anything changed ever (for return value)?
    bool changedNow = true; // Did we just change something (for loop control)?

    { // Begin scope for change event span.
        ChangeEventSpan span(this);

        while (changedNow) {
            changedNow = false;
            ensureSkeleton();

            // Crush edges if we can.
            if (countVertices() > components().size() &&
                    countVertices() > countBoundaryComponents()) {
                for (Edge<3>* edge : edges())
                    if (collapseEdge(edge, true, perform)) {
                        changedNow = changed = true;
                        break;
                    }
                if (changedNow) {
                    if (perform)
                        continue;
                    else
                        return true;
                }
            }

            // Look for internal simplifications.
            for (Edge<3>* edge : edges()) {
                if (threeTwoMove(edge, true, perform)) {
                    changedNow = changed = true;
                    break;
                }
                if (twoZeroMove(edge, true, perform)) {
                    changedNow = changed = true;
                    break;
                }
                if (twoOneMove(edge, 0, true, perform)) {
                    changedNow = changed = true;
                    break;
                }
                if (twoOneMove(edge, 1, true, perform)) {
                    changedNow = changed = true;
                    break;
                }
            }
            if (changedNow) {
                if (perform)
                    continue;
                else
                    return true;
            }
            for (Vertex<3>* vertex : vertices())
                if (twoZeroMove(vertex, true, perform)) {
                    changedNow = changed = true;
                    break;
                }
            if (changedNow) {
                if (perform)
                    continue;
                else
                    return true;
            }

            // Look for boundary simplifications.
            if (hasBoundaryTriangles()) {
                for (BoundaryComponent<3>* bc : boundaryComponents()) {
                    // Run through triangles of this boundary component looking
                    // for shell boundary moves.
                    nTriangles = bc->countTriangles();
                    for (iTriangle = 0; iTriangle < nTriangles; iTriangle++) {
                        if (shellBoundary(bc->triangle(iTriangle)->
                                front().tetrahedron(),
                                true, perform)) {
                            changedNow = changed = true;
                            break;
                        }
                    }
                    if (changedNow)
                        break;
                }
                if (changedNow) {
                    if (perform)
                        continue;
                    else
                        return true;
                }
            }
        }
    } // End scope for change event span.

    return changed;
}

} // namespace regina

