
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

#include "subcomplex/txicore.h"
#include <sstream>

namespace regina {

std::string TxICore::name() const {
    std::ostringstream out;
    writeName(out);
    return out.str();
}

std::string TxICore::TeXName() const {
    std::ostringstream out;
    writeTeXName(out);
    return out.str();
}

TxIDiagonalCore::TxIDiagonalCore(unsigned long newSize, unsigned long newK) :
        size_(newSize), k_(newK) {
    // We'll build the actual triangulation last.  Meanwhile, fill in
    // the remaining bits and pieces.
    bdryTet_[0][0] = 0;
    bdryTet_[0][1] = 1;
    bdryTet_[1][0] = size_ - 2;
    bdryTet_[1][1] = size_ - 1;

    // All bdryRoles permutations are identities.
    // No need to change them here.

    bdryReln_[0] = Matrix2(1, 0, 0, 1);
    bdryReln_[1] = Matrix2(-1, 0, 0, 1);
    parallelReln_ = Matrix2(1, size_ - 6, 0, 1);

    // Off we go!
    unsigned i;
    Tetrahedron<3>** t = new Tetrahedron<3>*[size_];
    for (i = 0; i < size_; i++)
        t[i] = core_.newTetrahedron();

    // Glue together the pairs of triangles in the central surface.
    t[0]->join(0, t[1], Perm<4>(0, 2, 1, 3));
    t[size_ - 2]->join(0, t[size_ - 1], Perm<4>(0, 2, 1, 3));

    // Glue together the long diagonal line of quads, and hook the ends
    // together using the first pair of triangles.
    t[0]->join(1, t[3], Perm<4>(2, 3, 1, 0));
    for (i = 3; i < size_ - 3; i++)
        t[i]->join(0, t[i + 1], Perm<4>(0, 3));
    t[size_ - 3]->join(0, t[1], Perm<4>(1, 0, 2, 3));

    // Glue the quadrilateral and double-triangular bulges to their
    // horizontal neighbours.
    t[1]->join(2, t[2], Perm<4>());
    t[2]->join(3, t[0], Perm<4>(1, 0, 3, 2));
    t[size_ - 1]->join(2, t[size_ - 2 - k_], Perm<4>(3, 0, 1, 2));
    t[size_ - 2]->join(2, t[size_ - 2 - k_], Perm<4>(0, 3, 2, 1));

    // Glue in the lower edge of each bulges.
    if (k_ == size_ - 5)
        t[2]->join(0, t[size_ - 2], Perm<4>(1, 3, 2, 0));
    else
        t[2]->join(0, t[3], Perm<4>(2, 1, 3, 0));
    if (k_ == 1)
        t[size_ - 1]->join(1, t[2], Perm<4>(2, 1, 3, 0));
    else
        t[size_ - 1]->join(1, t[size_ - 1 - k_], Perm<4>(3, 2, 0, 1));

    // Glue in the lower edge of each quadrilateral.
    for (i = 3; i <= size_ - 3; i++) {
        if (i == size_ - 2 - k_)
            continue;

        if (i == size_ - 3)
            t[i]->join(1, t[2], Perm<4>(3, 1, 0, 2));
        else if (i == size_ - 3 - k_)
            t[i]->join(1, t[size_ - 2], Perm<4>(0, 1, 3, 2));
        else
            t[i]->join(1, t[i + 1], Perm<4>(1, 2));
    }

    delete[] t;
}

TxIParallelCore::TxIParallelCore() {
    // We'll build the actual triangulation last.  Meanwhile, fill in
    // the remaining bits and pieces.
    bdryTet_[0][0] = 0;
    bdryTet_[0][1] = 1;
    bdryTet_[1][0] = 4;
    bdryTet_[1][1] = 5;

    // All bdryRoles permutations are identities.
    // No need to change them here.

    bdryReln_[0] = bdryReln_[1] = parallelReln_ = Matrix2(1, 0, 0, 1);

    // Off we go!
    // Just hard-code it.  It's only one triangulation, and it's highly
    // symmetric.
    unsigned i;
    Tetrahedron<3>** t = new Tetrahedron<3>*[6];
    for (i = 0; i < 6; i++)
        t[i] = core_.newTetrahedron();

    t[0]->join(0, t[1], Perm<4>(1, 2));
    t[4]->join(0, t[5], Perm<4>(1, 2));
    t[1]->join(2, t[2], Perm<4>());
    t[5]->join(2, t[3], Perm<4>());
    t[0]->join(2, t[2], Perm<4>(1, 0, 3, 2));
    t[4]->join(2, t[3], Perm<4>(1, 0, 3, 2));
    t[1]->join(1, t[3], Perm<4>(2, 0, 3, 1));
    t[5]->join(1, t[2], Perm<4>(2, 0, 3, 1));
    t[0]->join(1, t[3], Perm<4>(0, 3));
    t[4]->join(1, t[2], Perm<4>(0, 3));

    delete[] t;
}

} // namespace regina
