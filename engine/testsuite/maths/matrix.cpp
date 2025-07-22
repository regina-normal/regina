
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

TEST(MatrixTest, determinant) {
    // Some simple determinant tests, to verify that Matrix is working
    // correctly with non-native types.

    using L = Laurent<Integer>;
    using L2 = Laurent2<Integer>;
    using P = Polynomial<Integer>;

    EXPECT_EQ(Matrix<Integer>(2).det(), 0);
    EXPECT_EQ(Matrix<Rational>(2).det(), 0);
    EXPECT_EQ(Matrix<Laurent<Integer>>(2).det(), Laurent<Integer>());
    EXPECT_EQ(Matrix<Laurent2<Integer>>(2).det(), Laurent2<Integer>());
    EXPECT_EQ(Matrix<Polynomial<Integer>>(2).det(), Polynomial<Integer>());

    // [ 1, 2, -3, 4 ] -> 10
    EXPECT_EQ(Matrix<Integer>({ { 1, 2 }, { -3, 4 } }).det(), 10);

    // [ 1, 1/4 | 2, -1 ] -> -3/2
    EXPECT_EQ(Matrix<Rational>({ { 1, {1,4} }, { 2, -1 } }).det(),
        Rational(-3, 2));

    // [ 1, x | x^-1, 1 ] -> 0
    EXPECT_EQ(Matrix<L>(
        { { {0, {1}}, {1, {1}} }, { {-1, {1}}, {0, {1}} } }).det(), L());

    // [ 1, x + x^-1 | x - x^-1, -1 ] -> x^-2 - 1 - x^2
    EXPECT_EQ(Matrix<L>(
        { { {0, {1}}, {-1, {1,0,1}} }, { {-1, {-1,0,1}}, {0, {-1}} } }).det(),
        L(-2, {1,0,-1,0,-1}));

    // [ xy, y^-1, -y^2x, x^-1 ] -> y + xy
    EXPECT_EQ(Matrix<L2>(
        { { {{1,1,1}}, {{0,-1,1}} }, { {{1,2,-1}}, {{-1,0,1}} } }).det(),
        L2({ {0, 1, 1}, {1, 1, 1} }));

    // [ 1, x | -x, 1 ] -> x^2 + 1
    EXPECT_EQ(Matrix<P>({ { {1}, {0,1} }, { {0,-1}, {1} } }).det(), P({1,0,1}));
}

