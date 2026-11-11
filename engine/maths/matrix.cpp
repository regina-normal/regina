
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "maths/matrix-impl.h"
#include "maths/numbertheory.h"

namespace regina {

// Instantiate routines in matrix-impl.h for the common case Matrix<Integer>:

template size_t Matrix<Integer>::makeRowHermite();
template size_t Matrix<Integer>::makeColumnHermite();
template std::tuple<Matrix<Integer>, Matrix<Integer>, size_t>
    Matrix<Integer>::makeColumnHermiteCoB();
template void Matrix<Integer>::makeRowBasis();
template std::pair<Matrix<Integer>, Matrix<Integer>>
    Matrix<Integer>::rowBasisAndOrthComp() const;
template Matrix<Integer> Matrix<Integer>::inverse() &&;
template void Matrix<Integer>::makeSmith();
template std::array<Matrix<Integer>, 4>
    Matrix<Integer>::makeSmithHafnerMcCurley();
template std::array<Matrix<Integer>, 4> Matrix<Integer>::makeSmithMetrical();
template Matrix<Integer> Matrix<Integer>::preimageOfLattice(
    const std::vector<Integer>&) const;

} // namespace regina

