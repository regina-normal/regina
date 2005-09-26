
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

#include "subcomplex/ntxicore.h"

namespace regina {

NTxIDiagonalCore::NTxIDiagonalCore(unsigned long newSize, unsigned long newK) :
        size_(newSize), k_(newK) {
    // We'll build the actual triangulation last.  Meanwhile, fill in
    // the remaining bits and pieces.
    bdryTet_[0][0] = 0;
    bdryTet_[0][1] = 1;
    bdryTet_[0][2] = size_ - 2;
    bdryTet_[0][3] = size_ - 1;

    // All bdryRoles permutations are identities.
    // No need to change them here.

    bdryReln_[0] = NMatrix2(1, 0, 0, 1);
    bdryReln_[1] = NMatrix2(-1, 0, 0, 1);
    parallelReln_ = NMatrix2(1, size_ - 6, 0, 1);

    // Off we go!
    unsigned i;
    NTetrahedron** t = new NTetrahedron*[size_];
    for (i = 0; i < size_; i++)
        t[i] = new NTetrahedron;

    // Glue together the pairs of triangles in the central surface.
    t[0]->joinTo(0, t[1], NPerm(0, 2, 1, 3));
    t[size_ - 2]->joinTo(0, t[size_ - 1], NPerm(0, 2, 1, 3));

    // Glue together the long diagonal line of quads, and hook the ends
    // together using the first pair of triangles.
    t[0]->joinTo(1, t[3], NPerm(2, 3, 1, 0));
    for (i = 3; i < size_ - 3; i++)
        t[i]->joinTo(0, t[i + 1], NPerm(0, 3));
    t[size_ - 3]->joinTo(0, t[1], NPerm(1, 0, 2, 3));

    // Glue the quadrilateral and double-triangular bulges to their
    // horizontal neighbours.
    t[1]->joinTo(2, t[2], NPerm());
    t[2]->joinTo(3, t[0], NPerm(1, 0, 3, 2));
    t[size_ - 1]->joinTo(2, t[size_ - 2 - k_], NPerm(3, 0, 1, 2));
    t[size_ - 2]->joinTo(2, t[size_ - 2 - k_], NPerm(0, 3, 2, 1));

    // Glue in the lower edge of each bulges.
    if (k_ == size_ - 5)
        t[2]->joinTo(0, t[size_ - 2], NPerm(1, 3, 2, 0));
    else
        t[2]->joinTo(0, t[3], NPerm(2, 1, 3, 0));
    if (k_ == 1)
        t[size_ - 1]->joinTo(1, t[2], NPerm(2, 1, 3, 0));
    else
        t[size_ - 1]->joinTo(1, t[size_ - 1 - k_], NPerm(3, 2, 0, 1));

    // Glue in the lower edge of each quadrilateral.
    for (i = 3; i <= size_ - 3; i++) {
        if (i == size_ - 2 - k_)
            continue;

        if (i == size_ - 3)
            t[i]->joinTo(1, t[2], NPerm(3, 1, 0, 2));
        else if (i == size_ - 3 - k_)
            t[i]->joinTo(1, t[size_ - 2], NPerm(0, 1, 3, 2));
        else
            t[i]->joinTo(1, t[i + 1], NPerm(1, 2));
    }

    for (i = 0; i < size_; i++)
        core_.addTetrahedron(t[i]);
    delete[] t;
}

NTxIParallelCore::NTxIParallelCore() {
    // We'll build the actual triangulation last.  Meanwhile, fill in
    // the remaining bits and pieces.
    bdryTet_[0][0] = 0;
    bdryTet_[0][1] = 1;
    bdryTet_[0][2] = 4;
    bdryTet_[0][3] = 5;

    // All bdryRoles permutations are identities.
    // No need to change them here.

    bdryReln_[0] = bdryReln_[1] = parallelReln_ = NMatrix2(1, 0, 0, 1);

    // Off we go!
    // Just hard-code it.  It's only one triangulation, and it's highly
    // symmetric.
    unsigned i;
    NTetrahedron** t = new NTetrahedron*[6];
    for (i = 0; i < 6; i++)
        t[i] = new NTetrahedron;

    t[0]->joinTo(0, t[1], NPerm(1, 2));
    t[4]->joinTo(0, t[5], NPerm(1, 2));
    t[1]->joinTo(2, t[2], NPerm());
    t[5]->joinTo(2, t[3], NPerm());
    t[0]->joinTo(2, t[2], NPerm(1, 0, 3, 2));
    t[4]->joinTo(2, t[3], NPerm(1, 0, 3, 2));
    t[1]->joinTo(1, t[3], NPerm(2, 0, 3, 1));
    t[5]->joinTo(1, t[2], NPerm(2, 0, 3, 1));
    t[0]->joinTo(1, t[3], NPerm(0, 3));
    t[4]->joinTo(1, t[2], NPerm(0, 3));

    for (i = 0; i < 6; i++)
        core_.addTetrahedron(t[i]);
    delete[] t;
}

} // namespace regina
