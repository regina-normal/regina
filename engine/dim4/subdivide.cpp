
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <vector>

#include "dim4/dim4triangulation.h"

namespace regina {

void Dim4Triangulation::barycentricSubdivision() {
    unsigned long nOldPent = pentachora_.size();
    if (nOldPent == 0)
        return;

    Dim4Triangulation staging;
    ChangeEventSpan span1(&staging);

    Dim4Pentachoron** newPent = new Dim4Pentachoron*[nOldPent * 120];
    Dim4Pentachoron* oldPent;

    // A pentachoron in the subdivision is uniquely defined by the
    // permutation (tet, face, edge, vtx, corner) of (0, 1, 2, 3, 4).
    // This is the pentachoron that:
    // - meets the boundary in the tetrahedron opposite vertex "tet";
    // - meets that tetrahedron in the face opposite veretx "face";
    // - meets that face in the edge opposite vertex "edge";
    // - meets that edge in the vertex opposite vertex "vtx";
    // - directly touches vertex "corner".

    unsigned long pent;
    for (pent=0; pent < 120 * nOldPent; ++pent)
        newPent[pent] = staging.newPentachoron();

    // Do all of the internal gluings
    int permIdx;
    NPerm5 perm, glue;
    for (pent=0; pent < nOldPent; ++pent)
        for (permIdx = 0; permIdx < 120; ++permIdx) {
            perm = NPerm5::S5[permIdx];
            // (0, 1, 2, 3, 4) -> (tet, face, edge, vtx, corner)

            // Internal gluings within the old pentachoron:
            newPent[120 * pent + permIdx]->joinTo(perm[4],
                newPent[120 * pent + (perm * NPerm5(4, 3)).S5Index()],
                NPerm5(perm[4], perm[3]));

            newPent[120 * pent + permIdx]->joinTo(perm[3],
                newPent[120 * pent + (perm * NPerm5(3, 2)).S5Index()],
                NPerm5(perm[3], perm[2]));

            newPent[120 * pent + permIdx]->joinTo(perm[2],
                newPent[120 * pent + (perm * NPerm5(2, 1)).S5Index()],
                NPerm5(perm[2], perm[1]));

            newPent[120 * pent + permIdx]->joinTo(perm[1],
                newPent[120 * pent + (perm * NPerm5(1, 0)).S5Index()],
                NPerm5(perm[1], perm[0]));

            // Adjacent gluings to the adjacent pentachoron:
            oldPent = getPentachoron(pent);
            if (! oldPent->adjacentPentachoron(perm[0]))
                continue; // This hits a boundary facet.
            if (newPent[120 * pent + permIdx]->adjacentPentachoron(perm[0]))
                continue; // We've already done this gluing from the other side.

            glue = oldPent->adjacentGluing(perm[0]);
            newPent[120 * pent + permIdx]->joinTo(perm[0],
                newPent[120 * pentachoronIndex(
                    oldPent->adjacentPentachoron(perm[0])) +
                    (glue * perm).S5Index()],
                glue);
        }


    // Delete the existing pentachora and put in the new ones.
    ChangeEventSpan span2(this);
    removeAllPentachora();
    swapContents(staging);
    delete[] newPent;
}

} // namespace regina
