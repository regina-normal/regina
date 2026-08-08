
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
void verifyAdjugate(const Matrix<T>& m, const T& determinant,
        regina::AdjugateAlgorithm alg) {
    // We have already verified that the matrix is square through the variant
    // of verifyAdjugate() that does not take an algorithm as input.

    EXPECT_EQ(m.det(alg), determinant);

    if (m.rows() > 1 && alg == regina::AdjugateAlgorithm::MahajanVinay) {
        EXPECT_THROW({ m.adjugate(alg); }, regina::InvalidArgument);
    } else {
        auto adj = m.adjugate(alg);
        EXPECT_EQ(adj.first.rows(), m.rows());
        EXPECT_EQ(adj.first.columns(), m.columns());
        EXPECT_EQ(adj.second, determinant);
        if (m.rows() != 0)
            EXPECT_EQ(m * adj.first,
                Matrix<T>::identity(m.rows()) * determinant);
    }
}

template <regina::CommutativeRing T>
void verifyAdjugate(const Matrix<T>& m, const T& determinant) {
    // We use this if we already have an expcted value for the determinant.
    ASSERT_EQ(m.rows(), m.columns());

    verifyAdjugate(m, determinant, regina::AdjugateAlgorithm::FaddeevLeverrier);
    verifyAdjugate(m, determinant, regina::AdjugateAlgorithm::PreparataSarwate);
    verifyAdjugate(m, determinant, regina::AdjugateAlgorithm::MahajanVinay);
}

template <regina::CommutativeRing T>
void verifyAdjugate(const Matrix<T>& m) {
    // We use this if we do not know the determinant in advance.
    ASSERT_EQ(m.rows(), m.columns());

    auto det = m.det(regina::AdjugateAlgorithm::MahajanVinay);
    verifyAdjugate(m, det, regina::AdjugateAlgorithm::FaddeevLeverrier);
    verifyAdjugate(m, det, regina::AdjugateAlgorithm::PreparataSarwate);
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

    // Some slightly larger cases, now just using integers:
    for (size_t size = 3; size <= 10; ++size) {
        Matrix<Integer> diagonal(size);
        Matrix<Integer> antidiagonal(size);
        Integer factorial = 1;
        for (size_t i = 0; i < size; ++i) {
            diagonal.entry(i, i) = i + 1;
            antidiagonal.entry(size - i - 1, i) = i + 1;
            factorial *= (i + 1);
        }
        verifyAdjugate<Integer>(diagonal, factorial);
        switch (size & 3) {
            case 0:
            case 1:
                // size == 0 or 1 mod 4
                verifyAdjugate<Integer>(antidiagonal, factorial);
                break;
            default:
                // size == 2 or 3 mod 4
                verifyAdjugate<Integer>(antidiagonal, -factorial);
                break;
        }

        for (int attempt = 0; attempt < 5; ++attempt) {
            Matrix<Integer> random(size);
            for (size_t i = 0; i < size; ++i)
                for (size_t j = 0; j < size; ++j)
                    random.entry(i, j) = Integer::randomBinary(8);
            verifyAdjugate<Integer>(random);
        }
    }
}

