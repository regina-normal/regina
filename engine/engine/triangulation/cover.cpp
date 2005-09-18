
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include <queue>
#include "triangulation/ntriangulation.h"

namespace regina {

void NTriangulation::makeDoubleCover() {
    unsigned long sheetSize = tetrahedra.size();
    if (sheetSize == 0)
        return;

    ChangeEventBlock block(this);

    // Create a second sheet of tetrahedra.
    NTetrahedron** upper = new NTetrahedron*[sheetSize];
    unsigned long i;
    TetrahedronIterator tit = tetrahedra.begin();
    for (i = 0; i < sheetSize; i++) {
        upper[i] = new NTetrahedron((*tit)->getDescription());
        tit++;
    }

    // Reset each tetrahedron orientation.
    tit = tetrahedra.begin();
    for (i = 0; i < sheetSize; i++) {
        (*tit)->orientation = 0;
        upper[i]->orientation = 0;
        tit++;
    }

    // Run through the upper sheet and recreate the gluings as we
    // propagate tetrahedron orientations through components.
    std::queue<unsigned long> tetQueue;
        /**< Tetrahedra whose orientation must be propagated. */
    int face;
    unsigned long upperTet;
    NTetrahedron* lowerTet;
    unsigned long upperAdj;
    NTetrahedron* lowerAdj;
    int lowerAdjOrientation;
    NPerm gluing;
    for (i = 0; i < sheetSize; i++)
        if (upper[i]->orientation == 0) {
            // We've found a new component.
            // Completely recreate the gluings for this component.
            upper[i]->orientation = 1;
            tetrahedra[i]->orientation = -1;
            tetQueue.push(i);

            while (! tetQueue.empty()) {
                upperTet = tetQueue.front();
                tetQueue.pop();
                lowerTet = tetrahedra[upperTet];

                for (face = 0; face < 4; face++) {
                    lowerAdj = lowerTet->getAdjacentTetrahedron(face);

                    // See if this tetrahedron is glued to something in the
                    // lower sheet.
                    if (! lowerAdj)
                        continue;

                    // Make sure we haven't already fixed this gluing in
                    // the upper sheet.
                    if (upper[upperTet]->getAdjacentTetrahedron(face))
                        continue;

                    // Determine the expected orientation of the
                    // adjacent tetrahedron in the lower sheet.
                    gluing = lowerTet->getAdjacentTetrahedronGluing(face);
                    lowerAdjOrientation = (gluing.sign() == 1 ?
                        -lowerTet->orientation : lowerTet->orientation);

                    upperAdj = getTetrahedronIndex(lowerAdj);
                    if (lowerAdj->orientation == 0) {
                        // We haven't seen the adjacent tetrahedron yet.
                        lowerAdj->orientation = lowerAdjOrientation;
                        upper[upperAdj]->orientation = -lowerAdjOrientation;
                        upper[upperTet]->joinTo(face, upper[upperAdj], gluing);
                        tetQueue.push(upperAdj);
                    } else if (lowerAdj->orientation == lowerAdjOrientation) {
                        // The adjacent tetrahedron already has the
                        // correct orientation.
                        upper[upperTet]->joinTo(face, upper[upperAdj], gluing);
                    } else {
                        // The adjacent tetrahedron already has the
                        // incorrect orientation.  Make a cross between
                        // the two sheets.
                        lowerTet->unjoin(face);
                        lowerTet->joinTo(face, upper[upperAdj], gluing);
                        upper[upperTet]->joinTo(face, lowerAdj, gluing);
                    }
                }
            }
        }

    // Add the new tetrahedra to the triangulation.
    for (i = 0; i < sheetSize; i++)
        tetrahedra.push_back(upper[i]);

    // Tidy up.
    delete[] upper;
    gluingsHaveChanged();
}

} // namespace regina

