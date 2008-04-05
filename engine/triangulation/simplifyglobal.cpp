
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include <cstdlib>
#include "triangulation/ntriangulation.h"

// Affects the number of random 4-4 moves attempted during simplification.
#define COEFF_4_4 3

namespace regina {

bool NTriangulation::intelligentSimplify() {
    bool changed;

    // Don't automatically fire a change event - we don't know in
    // advance if changes will be made or not.

    { // Begin scope for change event block.
        ChangeEventBlock block(this, false);

        // Reduce to a local minimum.
        changed = simplifyToLocalMinimum(true);

        // Clone to work with when we might want to roll back changes.
        NTriangulation* use;

        // Variables used for selecting random 4-4 moves.
        std::vector<std::pair<NEdge*, int> > fourFourAvailable;
        std::pair<NEdge*, int> fourFourChoice;

        unsigned long fourFourAttempts;
        unsigned long fourFourCap;

        NEdge* edge;
        EdgeIterator eit;
        int axis;

        while (true) {
            // --- Random 4-4 moves ---

            // Clone the triangulation and start making changes that might or
            // might not lead to a simplification.
            // If we've already simplified then there's no need to use a
            // separate clone since we won't need to undo further changes.
            use = (changed ? this : new NTriangulation(*this));

            // Make random 4-4 moves.
            fourFourAttempts = fourFourCap = 0;
            while (true) {
                // Calculate the list of available 4-4 moves.
                fourFourAvailable.clear();
                // Use getEdges() to ensure the skeleton has been calculated.
                for (eit = use->getEdges().begin();
                        eit != use->getEdges().end(); eit++) {
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
                if (use->getNumberOfTetrahedra() < getNumberOfTetrahedra()) {
                    // The 4-4 moves were successful; accept them.
                    cloneFrom(*use);
                    changed = true;
                }
                delete use;
            }

            // At this point we have decided that 4-4 moves will help us
            // no more.

            // --- TODO: Open book moves ---

            // If we did any book opening stuff, move back to the beginning
            // of the loop.  Otherwise exit.
            break;
        }
    } // End scope for change event block.

    if (changed)
        fireChangedEvent();
    return changed;
}

bool NTriangulation::simplifyToLocalMinimum(bool perform) {
    EdgeIterator eit;
    VertexIterator vit;
    BoundaryComponentIterator bit;
    NEdge* edge;
    NBoundaryComponent* bc;
    unsigned long nFaces;
    unsigned long iFace;
    // unsigned long nEdges;
    // unsigned long iEdge;
    // std::deque<NEdgeEmbedding>::const_iterator embit, embbeginit, embendit;

    bool changed = false;   // Has anything changed ever (for return value)?
    bool changedNow = true; // Did we just change something (for loop control)?

    // Don't automatically fire a change event - we don't know in
    // advance if changes will be made or not.

    { // Begin scope for change event block.
        ChangeEventBlock block(this, false);

        while (changedNow) {
            changedNow = false;
            if (! calculatedSkeleton) {
                calculateSkeleton();
            }

            // Crush a maximal skeleton.
            /* Don't crush a maximal skeleton until we know what the
             * routine does!
            if (vertices.size() > components.size()) {
                if (crushMaximalForest()) {
                    if (! calculatedSkeleton)
                        calculateSkeleton();
                    // Keep trying to simplify in this iteration of the loop.
                    // Thus changedNow will remain false.
                    changed = true;
                }
            }
            */

            // Look for internal simplifications.
            for (eit = edges.begin(); eit != edges.end(); eit++) {
                edge = *eit;
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
            for (vit = vertices.begin(); vit != vertices.end(); vit++) {
                if (twoZeroMove(*vit, true, perform)) {
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
            if (hasBoundaryFaces()) {
                for (bit = boundaryComponents.begin();
                        bit != boundaryComponents.end(); bit++) {
                    bc = *bit;

                    // Run through faces of this boundary component looking
                    // for shell boundary moves.
                    nFaces = (*bit)->getNumberOfFaces();
                    for (iFace = 0; iFace < nFaces; iFace++) {
                        if (shellBoundary((*bit)->getFace(iFace)->
                                getEmbedding(0).getTetrahedron(),
                                true, perform)) {
                            changedNow = changed = true;
                            break;
                        }
                    }
                    if (changedNow)
                        break;

                    /**
                     * Do NOT do open book moves here since they don't reduce
                     * the triangulation per se.  We'll do this in
                     * intelligentSimplify() instead.
                     */
                    /*
                    // Run through edges of this boundary component looking
                    // for open book moves.
                    nEdges = (*bit)->getNumberOfEdges();
                    for (iEdge = 0; iEdge < nEdges; iEdge++) {
                        embbeginit = (*bit)->getEdge(iEdge)->
                            getEmbeddings().begin();
                        embendit = (*bit)->getEdge(iEdge)->
                            getEmbeddings().end();
                        for (embit = embbeginit; embit != embendit; embit++)
                            if (openBook((*embit).getTetrahedron()->getFace(
                                    ((*embit).getVertices())[2]),
                                    true, perform)) {
                                changedNow = true;
                                changed = true;
                                break;
                            }
                        if (changedNow)
                            break;
                    }
                    if (changedNow)
                        break;
                    */
                }
                if (changedNow) {
                    if (perform)
                        continue;
                    else
                        return true;
                }
            }
        }
    } // End scope for change event block.

    if (changed)
        fireChangedEvent();
    return changed;
}

} // namespace regina

