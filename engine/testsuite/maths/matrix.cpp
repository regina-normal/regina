
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include "maths/integer.h"
#include "maths/laurent.h"
#include "maths/laurent2.h"
#include "maths/matrix.h"
#include "maths/polynomial.h"
#include "maths/rational.h"

#include "testhelper.h"

using regina::Integer;
using regina::Laurent;
using regina::Laurent2;
using regina::Matrix;
using regina::Polynomial;
using regina::Rational;

template <regina::CommutativeRing T>
void verifyAdjugate(const Matrix<T>& m, const T& determinant) {
    ASSERT_EQ(m.rows(), m.columns());

    EXPECT_EQ(m.det(), determinant);

    auto adj = m.adjugate();
    EXPECT_EQ(adj.first.rows(), m.rows());
    EXPECT_EQ(adj.first.columns(), m.columns());
    EXPECT_EQ(adj.second, determinant);
    if (m.rows() != 0)
        EXPECT_EQ(m * adj.first, Matrix<T>::identity(m.rows()) * determinant);
}

TEST(MatrixTest, determinantAdjugate) {
    // Some simple determinant tests, to verify that Matrix is working
    // correctly with non-native types.

    using P = Polynomial<Integer>;
    using L = Laurent<Integer>;
    using L2 = Laurent2<Integer>;

    // Empty matrices (which are technically not supported by Regina):
    verifyAdjugate<Integer>({0}, 1);
    verifyAdjugate<Rational>({0}, 1);

    // Some matrices filled with zeroes:
    for (size_t size = 1; size < 5; ++size) {
        verifyAdjugate<Integer>({size}, 0);
        verifyAdjugate<Rational>({size}, 0);
        verifyAdjugate<Polynomial<Integer>>({size}, {});
        verifyAdjugate<Laurent<Integer>>({size}, {});
        verifyAdjugate<Laurent2<Integer>>({size}, {});
    }

    // Some ad-hoc 1x1 cases:
    verifyAdjugate<Integer>({ { -3 } }, {-3});
    verifyAdjugate<Rational>({ { {2,5} } }, {2,5});
    verifyAdjugate<Polynomial<Integer>>({ { {3,-1} } }, {3,-1});
    verifyAdjugate<Laurent<Integer>>({ { {-2, {-1,0,3,2}} } },
        {-2, {-1,0,3,2}});
    verifyAdjugate<Laurent2<Integer>>({ { {{-3,2,5}} } }, {{-3,2,5}});

    // Some ad-hoc 2x2 cases:

    // [ 1, 2, -3, 4 ] -> 10
    verifyAdjugate<Integer>({ { 1, 2 }, { -3, 4 } }, 10);

    // [ 1, 1/4 | 2, -1 ] -> -3/2
    verifyAdjugate<Rational>({ { 1, {1,4} }, { 2, -1 } }, {-3, 2});

    // [ 1, x | -x, 1 ] -> x^2 + 1
    verifyAdjugate<Polynomial<Integer>>(
        { { {1}, {0,1} }, { {0,-1}, {1} } },
        {1,0,1});

    // [ 1, x | x^-1, 1 ] -> 0
    verifyAdjugate<Laurent<Integer>>(
        { { {0, {1}}, {1, {1}} }, { {-1, {1}}, {0, {1}} } },
        {});

    // [ 1, x + x^-1 | x - x^-1, -1 ] -> x^-2 - 1 - x^2
    verifyAdjugate<Laurent<Integer>>(
        { { {0, {1}}, {-1, {1,0,1}} }, { {-1, {-1,0,1}}, {0, {-1}} } },
        {-2, {1,0,-1,0,-1}});

    // [ xy, y^-1 | -y^2x, x^-1 ] -> y + xy
    verifyAdjugate<Laurent2<Integer>>(
        { { {{1,1,1}}, {{0,-1,1}} }, { {{1,2,-1}}, {{-1,0,1}} } },
        {{ {0, 1, 1}, {1, 1, 1} }});
}

