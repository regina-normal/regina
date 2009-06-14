
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
#include "dim4/dim4triangulation.h"

namespace regina {

bool Dim4Triangulation::intelligentSimplify() {
    bool changed;

    // Don't automatically fire a change event - we don't know in
    // advance if changes will be made or not.

    { // Begin scope for change event block.
        ChangeEventBlock block(this, false);

        // Reduce to a local minimum.
        changed = simplifyToLocalMinimum(true);

        // Clone to work with when we might want to roll back changes.
        Dim4Triangulation* use;

        while (true) {
            // --- Open book moves ---

            if (hasBoundaryTetrahedra()) {
                // Clone again, always -- we don't want to create gratuitous
                // boundary faces if they won't be of any help.
                use = new Dim4Triangulation(*this);

                // Perform every book opening move we can find.
                TetrahedronIterator tit;

                bool opened = false;
                bool openedNow = true;
                while (openedNow) {
                    openedNow = false;

                    for (tit = use->getTetrahedra().begin();
                            tit != use->getTetrahedra().end(); ++tit)
                        // TODO
                        // if (use->openBook(*tit, true, true)) {
                        //     opened = openedNow = true;
                        //     break;
                        // }
                        ; // TODO
                }

                // If we're lucky, we can now simplify further.
                if (opened) {
                    if (use->simplifyToLocalMinimum(true)) {
                        // Yay!
                        cloneFrom(*use);
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
    } // End scope for change event block.

    if (changed)
        fireChangedEvent();
    return changed;
}

bool Dim4Triangulation::simplifyToLocalMinimum(bool perform) {
    BoundaryComponentIterator bit;
    Dim4BoundaryComponent* bc;
    unsigned long nTetrahedra;
    unsigned long iTet;

    bool changed = false;   // Has anything changed ever (for return value)?
    bool changedNow = true; // Did we just change something (for loop control)?

    // Don't automatically fire a change event - we don't know in
    // advance if changes will be made or not.

    { // Begin scope for change event block.
        ChangeEventBlock block(this, false);

        while (changedNow) {
            changedNow = false;
            if (! calculatedSkeleton_) {
                calculateSkeleton();
            }

            // Look for boundary simplifications.
            if (hasBoundaryTetrahedra()) {
                for (bit = boundaryComponents_.begin();
                        bit != boundaryComponents_.end(); bit++) {
                    bc = *bit;

                    // Run through facets of this boundary component looking
                    // for shell boundary moves.
                    nTetrahedra = (*bit)->getNumberOfTetrahedra();
                    for (iTet = 0; iTet < nTetrahedra; ++iTet) {
                        if (shellBoundary((*bit)->getTetrahedron(iTet)->
                                getEmbedding(0).getPentachoron(),
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
    } // End scope for change event block.

    if (changed)
        fireChangedEvent();
    return changed;
}

} // namespace regina

