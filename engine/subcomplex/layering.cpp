
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

#include "subcomplex/layering.h"
#include "triangulation/dim3.h"

namespace regina {

Layering::Layering(Tetrahedron<3>* bdry0, Perm<4> roles0, Tetrahedron<3>* bdry1,
        Perm<4> roles1) : size_(0), reln(1, 0, 0, 1) {
    oldBdryTet_[0] = newBdryTet_[0] = bdry0;
    oldBdryTet_[1] = newBdryTet_[1] = bdry1;

    oldBdryRoles_[0] = newBdryRoles_[0] = roles0;
    oldBdryRoles_[1] = newBdryRoles_[1] = roles1;
}

bool Layering::extendOne() {
    // See if we move to a common new tetrahedron.
    // Also make sure this really is a new tetrahedron, so we don't get
    // stuck in a loop.
    Tetrahedron<3>* next = newBdryTet_[0]->adjacentTetrahedron(
        newBdryRoles_[0][3]);

    if (next == 0 || next == newBdryTet_[0] || next == newBdryTet_[1] ||
            next == oldBdryTet_[0] || next == oldBdryTet_[1])
        return false;
    if (next != newBdryTet_[1]->adjacentTetrahedron(newBdryRoles_[1][3]))
        return false;

    // Get the mappings from the boundary vertex roles to the new tetrahedron
    // vertices.
    Perm<4> cross0 = newBdryTet_[0]->adjacentGluing(
        newBdryRoles_[0][3]) * newBdryRoles_[0];
    Perm<4> cross1 = newBdryTet_[1]->adjacentGluing(
        newBdryRoles_[1][3]) * newBdryRoles_[1];

    // Is it actually a layering?
    if (cross1 == cross0 * Perm<4>(3, 2, 1, 0)) {
        // We're layering over the edge joining vertex roles 1 and 2.
        size_++;

        newBdryRoles_[0] = cross0 * Perm<4>(0, 1, 3, 2);
        newBdryRoles_[1] = cross0 * Perm<4>(3, 2, 0, 1);

        newBdryTet_[0] = newBdryTet_[1] = next;

        // new a = old a         = reln00 p + reln01 q
        // new b = old a + old b = (reln00 + reln10) p + (reln01 + reln11) q
        reln[1][0] += reln[0][0];
        reln[1][1] += reln[0][1];

        return true;
    } else if (cross1 == cross0 * Perm<4>(2, 3, 0, 1)) {
        // We're layering over the edge joining vertex roles 0 and 2.
        size_++;

        newBdryRoles_[0] = cross0 * Perm<4>(0, 1, 3, 2);
        newBdryRoles_[1] = cross0 * Perm<4>(2, 3, 1, 0);

        newBdryTet_[0] = newBdryTet_[1] = next;

        // new a = old a         = reln00 p + reln01 q
        // new b = old b - old a = (reln10 - reln00) p + (reln11 - reln01) q
        reln[1][0] -= reln[0][0];
        reln[1][1] -= reln[0][1];

        return true;
    } else if (cross1 == cross0 * Perm<4>(1, 0, 3, 2)) {
        // We're layering over the edge joining vertex roles 0 and 1.
        size_++;

        newBdryRoles_[0] = cross0 * Perm<4>(0, 3, 2, 1);
        newBdryRoles_[1] = cross0 * Perm<4>(1, 2, 3, 0);

        newBdryTet_[0] = newBdryTet_[1] = next;

        // new a = old a - old b = (reln00 - reln10) p + (reln01 - reln11) q
        // new b = old b         = reln10 p + reln11 q
        reln[0][0] -= reln[1][0];
        reln[0][1] -= reln[1][1];

        return true;
    }

    // It's not a layering at all.
    return false;
}

unsigned long Layering::extend() {
    unsigned long added = 0;

    while (extendOne())
        added++;

    return added;
}

bool Layering::matchesTop(Tetrahedron<3>* upperBdry0, Perm<4> upperRoles0,
        Tetrahedron<3>* upperBdry1, Perm<4> upperRoles1, Matrix2& upperReln)
        const {
    // We can cut half our cases by assuming that upperBdry0 meets with
    // newBdryTet[0] and that upperBdry1 meets with newBdryTet[1].
    bool rot180;
    if (upperBdry0->adjacentTetrahedron(upperRoles0[3]) == newBdryTet_[1] &&
            upperBdry0->adjacentFace(upperRoles0[3]) == newBdryRoles_[1][3]) {
        // If it does match, it's the opposite matching (upperBdry0 with
        // newBdryTet[1] and vice versa).  Switch them and remember what
        // we did.
        Tetrahedron<3>* tmpTet = upperBdry0;
        upperBdry0 = upperBdry1;
        upperBdry1 = tmpTet;

        Perm<4> tmpPerm = upperRoles0;
        upperRoles0 = upperRoles1;
        upperRoles1 = tmpPerm;

        rot180 = true;
    } else {
        // If it does match, it's what we'd like.
        rot180 = false;
    }

    // Do we meet the right tetrahedra and faces?
    if (upperBdry0->adjacentTetrahedron(upperRoles0[3]) != newBdryTet_[0])
        return false;
    if (upperBdry0->adjacentFace(upperRoles0[3]) != newBdryRoles_[0][3])
        return false;
    if (upperBdry1->adjacentTetrahedron(upperRoles1[3]) != newBdryTet_[1])
        return false;
    if (upperBdry1->adjacentFace(upperRoles1[3]) != newBdryRoles_[1][3])
        return false;

    // Find the mapping from the upper vertex roles to the boundary
    // vertex roles.  Verify that this mapping is consistent for both faces.
    Perm<4> cross = newBdryRoles_[0].inverse() * upperBdry0->
        adjacentGluing(upperRoles0[3]) * upperRoles0;
    if (cross != newBdryRoles_[1].inverse() * upperBdry1->
            adjacentGluing(upperRoles1[3]) * upperRoles1)
        return false;

    // It's a match!  Run through the six possible mappings to get the
    // relationship matrix correct.
    if (cross == Perm<4>(0, 1, 2, 3)) {
        // It's the identity.
        upperReln = reln;
    } else if (cross == Perm<4>(0, 2, 1, 3)) {
        // new a = + old b
        // new b = + old a
        upperReln = Matrix2(0, 1, 1, 0) * reln;
    } else if (cross == Perm<4>(1, 0, 2, 3)) {
        // new a = - old a
        // new b = - old a + old b
        upperReln = Matrix2(-1, 0, -1, 1) * reln;
    } else if (cross == Perm<4>(1, 2, 0, 3)) {
        // new a = - old a + old b
        // new b = - old a
        upperReln = Matrix2(-1, 1, -1, 0) * reln;
    } else if (cross == Perm<4>(2, 0, 1, 3)) {
        // new a = - old b
        // new b = + old a - old b
        upperReln = Matrix2(0, -1, 1, -1) * reln;
    } else if (cross == Perm<4>(2, 1, 0, 3)) {
        // new a = + old a - old b
        // new b = - old b
        upperReln = Matrix2(1, -1, 0, -1) * reln;
    }

    // Don't forget to account for the 180 degree rotation if it
    // happened.
    if (rot180)
        upperReln.negate();

    return true;
}

} // namespace regina
