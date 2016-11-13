
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
#include "triangulation/dim4.h"

// Affects the number of random 3-3 moves attempted during simplification.
#define COEFF_3_3 10

namespace regina {

bool Triangulation<4>::intelligentSimplify() {
    bool changed;

    { // Begin scope for change event span.
        ChangeEventSpan span(this);

        // Reduce to a local minimum.
        changed = simplifyToLocalMinimum(true);

        // Clone to work with when we might want to roll back changes.
        Triangulation<4>* use;

        // Variables for selecting random 3-3 moves.
        std::vector<Triangle<4>*> threeThreeAvailable;
        Triangle<4>* threeThreeChoice;

        unsigned long threeThreeAttempts;
        unsigned long threeThreeCap;

        Triangle<4>* triangle;
        TriangleIterator fit;

        while (true) {
            // --- Random 3-3 moves ---

            // Clone the triangulation and start making changes that might or
            // might not lead to a simplification.
            // If we've already simplified then there's no need to use a
            // separate clone since we won't need to undo further changes.
            use = (changed ? this : new Triangulation<4>(*this, false));

            // Make random 3-3 moves.
            threeThreeAttempts = threeThreeCap = 0;
            while (true) {
                // Calculate the list of available 3-3 moves.
                threeThreeAvailable.clear();
                // Use triangles() to ensure the skeleton has been calculated.
                for (fit = use->triangles().begin();
                        fit != use->triangles().end(); ++fit) {
                    triangle = *fit;
                    if (use->threeThreeMove(triangle, true, false))
                        threeThreeAvailable.push_back(triangle);
                }

                // Increment threeThreeCap if needed.
                if (threeThreeCap < COEFF_3_3 * threeThreeAvailable.size())
                    threeThreeCap = COEFF_3_3 * threeThreeAvailable.size();

                // Have we tried enough 3-3 moves?
                if (threeThreeAttempts >= threeThreeCap)
                    break;

                // Perform a random 3-3 move on the clone.
                threeThreeChoice = threeThreeAvailable[
                    static_cast<unsigned>(rand()) % threeThreeAvailable.size()];
                use->threeThreeMove(threeThreeChoice, false, true);

                // See if we can simplify now.
                if (use->simplifyToLocalMinimum(true)) {
                    // We have successfully simplified!
                    // Start all over again.
                    threeThreeAttempts = threeThreeCap = 0;
                } else
                    threeThreeAttempts++;
            }

            // Sync the real triangulation with the clone if appropriate.
            if (use != this) {
                // At this point, changed == false.
                if (use->size() < size()) {
                    // The 3-3 moves were successful; accept them.
                    swapContents(*use);
                    changed = true;
                }
                delete use;
            }

            // At this point we have decided that 3-3 moves will help us
            // no more.

            // --- Open book moves ---

            if (hasBoundaryTetrahedra()) {
                // Clone again, always -- we don't want to create gratuitous
                // boundary facets if they won't be of any help.
                use = new Triangulation<4>(*this, false);

                // Perform every book opening move we can find.
                TetrahedronIterator tit;

                bool opened = false;
                bool openedNow = true;
                while (openedNow) {
                    openedNow = false;

                    for (tit = use->tetrahedra().begin();
                            tit != use->tetrahedra().end(); ++tit)
                        if (use->openBook(*tit, true, true)) {
                            opened = openedNow = true;
                            break;
                        }
                }

                // If we're lucky, we can now simplify further.
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
            }

            // Nothing more we can do here.
            break;
        }
    } // End scope for change event span.

    return changed;
}

bool Triangulation<4>::simplifyToLocalMinimum(bool perform) {
    unsigned long nTetrahedra;
    unsigned long iTet;

    bool changed = false;   // Has anything changed ever (for return value)?
    bool changedNow = true; // Did we just change something (for loop control)?

    { // Begin scope for change event span.
        ChangeEventSpan span(this);

        while (changedNow) {
            changedNow = false;
            ensureSkeleton();

            // Crush edges if we can.
            if (countVertices() > countComponents() &&
                    countVertices() > countBoundaryComponents()) {
                for (Edge<4>* e : edges()) {
                    if (collapseEdge(e, true, perform)) {
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
            }

            // Look for internal simplifications.

            // Experience suggests that 2-0 moves are more important to
            // "unblock" other moves, and we should leave the simpler
            // 4-2 moves until last.
            for (Triangle<4>* t : triangles()) {
                if (twoZeroMove(t, true, perform)) {
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

            for (Edge<4>* e : edges()) {
                if (twoZeroMove(e, true, perform)) {
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

            for (Edge<4>* e : edges()) {
                if (fourTwoMove(e, true, perform)) {
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

            // Look for boundary simplifications.
            if (hasBoundaryTetrahedra()) {
                for (BoundaryComponent<4>* bc : boundaryComponents()) {
                    // Run through facets of this boundary component looking
                    // for shell boundary moves.
                    nTetrahedra = bc->countTetrahedra();
                    for (iTet = 0; iTet < nTetrahedra; ++iTet) {
                        if (shellBoundary(bc->tetrahedron(iTet)->
                                front().pentachoron(),
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

