
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
#include "maths/vector.h"
#include "maths/polynomial.h"
#include "maths/rational.h"

#include "testhelper.h"

using regina::AdjugateAlgorithm;
using regina::Integer;
using regina::Laurent;
using regina::Laurent2;
using regina::Matrix;
using regina::Polynomial;
using regina::Rational;
using regina::Vector;

template <regina::CommutativeRing T>
void validateEmpty(const Matrix<T>& m) {
    EXPECT_NO_THROW({ m.validate(); });
    EXPECT_EQ(m.rows(), 0);
    EXPECT_EQ(m.columns(), 0);
    EXPECT_EQ(m, Matrix<T>());
}

template <regina::CommutativeRing T>
void validateNoRows(const Matrix<T>& m, size_t columns) {
    EXPECT_NO_THROW({ m.validate(); });
    EXPECT_EQ(m.rows(), 0);
    EXPECT_EQ(m.columns(), columns);
    EXPECT_EQ(m, Matrix<T>(0, columns));
}

template <regina::CommutativeRing T>
void validateNoColumns(const Matrix<T>& m, size_t rows) {
    EXPECT_NO_THROW({ m.validate(); });
    EXPECT_EQ(m.rows(), rows);
    EXPECT_EQ(m.columns(), 0);
    EXPECT_EQ(m, Matrix<T>(rows, 0));
}

TEST(MatrixTest, empty) {
    // Test everything that we are allowed to do with an empty matrix.
    validateEmpty(Matrix<Integer>());
    validateEmpty(Matrix<Integer>(0));
    validateEmpty(Matrix<Integer>(0, 0));
    validateEmpty(Matrix<Integer>({}));

    Matrix<Integer> empty;
    auto m = Matrix<Integer>::identity(3);
    EXPECT_EQ(m.rows(), 3);
    EXPECT_EQ(m.columns(), 3);

    m = empty;
    validateEmpty(m);
    validateEmpty(Matrix<Integer>(m));
    validateEmpty(Matrix<Integer>(std::move(m)));

    m = Matrix<Integer>::identity(4); // assignment after move-out
    EXPECT_NO_THROW({ m.validate(); });
    EXPECT_EQ(m.rows(), 4);
    EXPECT_EQ(m.columns(), 4);
    EXPECT_TRUE(m.isIdentity());

    m = std::move(empty);
    validateEmpty(m);

    {
        auto tmp = Matrix<Integer>::identity(3);
        EXPECT_NO_THROW({ tmp.validate(); });
        EXPECT_EQ(tmp.rows(), 3);
        EXPECT_EQ(tmp.columns(), 3);

        m.swap(tmp);
        validateEmpty(tmp);
        EXPECT_NO_THROW({ m.validate(); });
        EXPECT_EQ(m.rows(), 3);
        EXPECT_EQ(m.columns(), 3);

        swap(m, tmp);
        validateEmpty(m);
        EXPECT_NO_THROW({ tmp.validate(); });
        EXPECT_EQ(tmp.rows(), 3);
        EXPECT_EQ(tmp.columns(), 3);
    }

    m.fill(3);
    validateEmpty(m);

    EXPECT_EQ(m, Matrix<Integer>());
    EXPECT_EQ(m, m);

    validateEmpty(m.transpose());
    EXPECT_EQ(m.str(), "(empty 0x0 matrix)");
    EXPECT_EQ(m.detail(), "(empty 0x0 matrix)\n");

    validateEmpty(Matrix<Integer>::identity(0));
    m.makeIdentity();
    validateEmpty(m);
    EXPECT_TRUE(m.isIdentity());
    EXPECT_TRUE(m.isZero());

    m += Matrix<Integer>();
    validateEmpty(m);
    m += m;
    validateEmpty(m);
    m -= Matrix<Integer>();
    validateEmpty(m);
    m -= m;
    validateEmpty(m);
    m *= 3;
    validateEmpty(m);
    m *= Integer(3);
    validateEmpty(m);
    validateEmpty(m * 3);
    validateEmpty(Matrix<Integer>(m) * 3);
    EXPECT_NO_THROW({
        validateEmpty(m * Matrix<Integer>());
        validateEmpty(m * m);
        validateNoRows(m * Matrix<Integer>(0, 4), 4);
        validateNoColumns(Matrix<Integer>(3, 0) * m, 3);
    });
    EXPECT_THROW({ m * Matrix<Integer>(3, 4); }, regina::InvalidArgument);
    EXPECT_THROW({ Matrix<Integer>(3, 4) * m; }, regina::InvalidArgument);
    EXPECT_THROW({ m * Matrix<Integer>(3, 0); }, regina::InvalidArgument);
    EXPECT_THROW({ Matrix<Integer>(0, 4) * m; }, regina::InvalidArgument);
    {
        auto ans = m * Vector<Integer>(0);
        EXPECT_EQ(ans.size(), 0);
    }
    EXPECT_THROW({ m * Vector<Integer>(3); }, regina::InvalidArgument);
    EXPECT_EQ(m.trace(), 0);
    EXPECT_EQ(m.det(), 1);
    EXPECT_EQ(m.det(AdjugateAlgorithm::Default), 1);
    EXPECT_EQ(m.det(AdjugateAlgorithm::FaddeevLeverrier), 1);
    EXPECT_EQ(m.det(AdjugateAlgorithm::PreparataSarwate), 1);
    EXPECT_EQ(m.det(AdjugateAlgorithm::MahajanVinay), 1);
    {
        auto [adj, det] = m.adjugate(AdjugateAlgorithm::Default);
        validateEmpty(adj);
        EXPECT_EQ(det, 1);
    }
    {
        auto [adj, det] = m.adjugate(AdjugateAlgorithm::FaddeevLeverrier);
        validateEmpty(adj);
        EXPECT_EQ(det, 1);
    }
    {
        auto [adj, det] = m.adjugate(AdjugateAlgorithm::PreparataSarwate);
        validateEmpty(adj);
        EXPECT_EQ(det, 1);
    }
    {
        // The empty matrix is special-cased here, and works even though
        // Mahajan-Vinay cannot be used to compute adjugates in general.
        auto [adj, det] = m.adjugate(AdjugateAlgorithm::MahajanVinay);
        validateEmpty(adj);
        EXPECT_EQ(det, 1);
    }

    m.negate();
    validateEmpty(m);

    EXPECT_EQ(m.rowEchelonForm(), 0);
    validateEmpty(m);

    EXPECT_EQ(m.columnEchelonForm(), 0);
    validateEmpty(m);

    EXPECT_EQ(m.rank(), 0);
    EXPECT_EQ(Matrix<Integer>(m).rank(), 0);

}

template <regina::CommutativeRing T>
void verifyAdjugate(const Matrix<T>& m, const T& determinant,
        AdjugateAlgorithm alg) {
    // We have already verified that the matrix is square through the variant
    // of verifyAdjugate() that does not take an algorithm as input.

    EXPECT_EQ(m.det(alg), determinant);

    if (m.rows() > 1 && alg == AdjugateAlgorithm::MahajanVinay) {
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

    verifyAdjugate(m, determinant, AdjugateAlgorithm::FaddeevLeverrier);
    verifyAdjugate(m, determinant, AdjugateAlgorithm::PreparataSarwate);
    verifyAdjugate(m, determinant, AdjugateAlgorithm::MahajanVinay);
}

template <regina::CommutativeRing T>
void verifyAdjugate(const Matrix<T>& m) {
    // We use this if we do not know the determinant in advance.
    ASSERT_EQ(m.rows(), m.columns());

    auto det = m.det(AdjugateAlgorithm::MahajanVinay);
    verifyAdjugate(m, det, AdjugateAlgorithm::FaddeevLeverrier);
    verifyAdjugate(m, det, AdjugateAlgorithm::PreparataSarwate);
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

