
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

#include "triangulation/ntetrahedron.h"
#include "subcomplex/nlayering.h"

namespace regina {

NLayering::NLayering(NTetrahedron* bdry0, NPerm roles0, NTetrahedron* bdry1,
        NPerm roles1) : size(0), reln(1, 0, 0, 1) {
    oldBdryTet[0] = newBdryTet[0] = bdry0;
    oldBdryTet[1] = newBdryTet[1] = bdry1;

    oldBdryRoles[0] = newBdryRoles[0] = roles0;
    oldBdryRoles[1] = newBdryRoles[1] = roles1;
}

bool NLayering::extendOne() {
    // See if we move to a common new tetrahedron.
    // Also make sure this really is a new tetrahedron, so we don't get
    // stuck in a loop.
    NTetrahedron* next = newBdryTet[0]->adjacentTetrahedron(
        newBdryRoles[0][3]);

    if (next == 0 || next == newBdryTet[0] || next == newBdryTet[1] ||
            next == oldBdryTet[0] || next == oldBdryTet[1])
        return false;
    if (next != newBdryTet[1]->adjacentTetrahedron(newBdryRoles[1][3]))
        return false;

    // Get the mappings from the boundary vertex roles to the new tetrahedron
    // vertices.
    NPerm cross0 = newBdryTet[0]->adjacentGluing(
        newBdryRoles[0][3]) * newBdryRoles[0];
    NPerm cross1 = newBdryTet[1]->adjacentGluing(
        newBdryRoles[1][3]) * newBdryRoles[1];

    // Is it actually a layering?
    if (cross1 == cross0 * NPerm(3, 2, 1, 0)) {
        // We're layering over the edge joining vertex roles 1 and 2.
        size++;

        newBdryRoles[0] = cross0 * NPerm(0, 1, 3, 2);
        newBdryRoles[1] = cross0 * NPerm(3, 2, 0, 1);

        newBdryTet[0] = newBdryTet[1] = next;

        // new a = old a         = reln00 p + reln01 q
        // new b = old a + old b = (reln00 + reln10) p + (reln01 + reln11) q
        reln[1][0] += reln[0][0];
        reln[1][1] += reln[0][1];

        return true;
    } else if (cross1 == cross0 * NPerm(2, 3, 0, 1)) {
        // We're layering over the edge joining vertex roles 0 and 2.
        size++;

        newBdryRoles[0] = cross0 * NPerm(0, 1, 3, 2);
        newBdryRoles[1] = cross0 * NPerm(2, 3, 1, 0);

        newBdryTet[0] = newBdryTet[1] = next;

        // new a = old a         = reln00 p + reln01 q
        // new b = old b - old a = (reln10 - reln00) p + (reln11 - reln01) q
        reln[1][0] -= reln[0][0];
        reln[1][1] -= reln[0][1];

        return true;
    } else if (cross1 == cross0 * NPerm(1, 0, 3, 2)) {
        // We're layering over the edge joining vertex roles 0 and 1.
        size++;

        newBdryRoles[0] = cross0 * NPerm(0, 3, 2, 1);
        newBdryRoles[1] = cross0 * NPerm(1, 2, 3, 0);

        newBdryTet[0] = newBdryTet[1] = next;

        // new a = old a - old b = (reln00 - reln10) p + (reln01 - reln11) q
        // new b = old b         = reln10 p + reln11 q
        reln[0][0] -= reln[1][0];
        reln[0][1] -= reln[1][1];

        return true;
    }

    // It's not a layering at all.
    return false;
}

unsigned long NLayering::extend() {
    unsigned long added = 0;

    while (extendOne())
        added++;

    return added;
}

bool NLayering::matchesTop(NTetrahedron* upperBdry0, NPerm upperRoles0,
        NTetrahedron* upperBdry1, NPerm upperRoles1, NMatrix2& upperReln)
        const {
    // We can cut half our cases by assuming that upperBdry0 meets with
    // newBdryTet[0] and that upperBdry1 meets with newBdryTet[1].
    bool rot180;
    if (upperBdry0->adjacentTetrahedron(upperRoles0[3]) == newBdryTet[1] &&
            upperBdry0->adjacentFace(upperRoles0[3]) == newBdryRoles[1][3]) {
        // If it does match, it's the opposite matching (upperBdry0 with
        // newBdryTet[1] and vice versa).  Switch them and remember what
        // we did.
        NTetrahedron* tmpTet = upperBdry0;
        upperBdry0 = upperBdry1;
        upperBdry1 = tmpTet;

        NPerm tmpPerm = upperRoles0;
        upperRoles0 = upperRoles1;
        upperRoles1 = tmpPerm;

        rot180 = true;
    } else {
        // If it does match, it's what we'd like.
        rot180 = false;
    }

    // Do we meet the right tetrahedra and faces?
    if (upperBdry0->adjacentTetrahedron(upperRoles0[3]) != newBdryTet[0])
        return false;
    if (upperBdry0->adjacentFace(upperRoles0[3]) != newBdryRoles[0][3])
        return false;
    if (upperBdry1->adjacentTetrahedron(upperRoles1[3]) != newBdryTet[1])
        return false;
    if (upperBdry1->adjacentFace(upperRoles1[3]) != newBdryRoles[1][3])
        return false;

    // Find the mapping from the upper vertex roles to the boundary
    // vertex roles.  Verify that this mapping is consistent for both faces.
    NPerm cross = newBdryRoles[0].inverse() * upperBdry0->
        adjacentGluing(upperRoles0[3]) * upperRoles0;
    if (cross != newBdryRoles[1].inverse() * upperBdry1->
            adjacentGluing(upperRoles1[3]) * upperRoles1)
        return false;

    // It's a match!  Run through the six possible mappings to get the
    // relationship matrix correct.
    if (cross == NPerm(0, 1, 2, 3)) {
        // It's the identity.
        upperReln = reln;
    } else if (cross == NPerm(0, 2, 1, 3)) {
        // new a = + old b
        // new b = + old a
        upperReln = NMatrix2(0, 1, 1, 0) * reln;
    } else if (cross == NPerm(1, 0, 2, 3)) {
        // new a = - old a
        // new b = - old a + old b
        upperReln = NMatrix2(-1, 0, -1, 1) * reln;
    } else if (cross == NPerm(1, 2, 0, 3)) {
        // new a = - old a + old b
        // new b = - old a
        upperReln = NMatrix2(-1, 1, -1, 0) * reln;
    } else if (cross == NPerm(2, 0, 1, 3)) {
        // new a = - old b
        // new b = + old a - old b
        upperReln = NMatrix2(0, -1, 1, -1) * reln;
    } else if (cross == NPerm(2, 1, 0, 3)) {
        // new a = + old a - old b
        // new b = - old b
        upperReln = NMatrix2(1, -1, 0, -1) * reln;
    }

    // Don't forget to account for the 180 degree rotation if it
    // happened.
    if (rot180)
        upperReln.negate();

    return true;
}

} // namespace regina
