
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
using regina::MatrixInt;
using regina::Polynomial;
using regina::Rational;
using regina::SNFAlgorithm;
using regina::Vector;

class MatrixTest : public testing::Test {
    protected:
        // Several test cases for working with integers:
        const MatrixInt zero34 { 3, 4 };
        const MatrixInt identity3 { MatrixInt::identity(3) };
        const MatrixInt one {{ 1 }};
        const MatrixInt minusOne {{ -1 }};
        const MatrixInt two {{ 2 }};
        const MatrixInt vector {{ 12, -8, 18 }};
        const MatrixInt shear2 {{ 1, 5 },
                                { 0, 1 }};
        const MatrixInt square3 {{ 2, 4, 4 },
                                 { -6, 6, 12 },
                                 { 10, -4, -16 }};
        const MatrixInt rect34 {{ 4, -17, 0, 6 },
                                { -2, 4, 9, 0 },
                                { 6, -3, -2, 10 }};
        const MatrixInt rect43 {{ 4, -17, 0 },
                                { 6, -2, 4 },
                                { 9, 0, 6 },
                                { -3, -2, 10 }};
        const MatrixInt redundant34 {{ 3, 8, 11, -5 },
                                     { 1, 4, 5, -3 },
                                     { 2, 8, 10, -6 }};
        const MatrixInt duplicate34 {{ 1, 1, 1, 1 },
                                     { 0, 0, 2, 3 },
                                     { 0, 0, 3, 5 }};
        const MatrixInt perm4 {{ 0, 1, 0, 0 },
                               { 0, 0, 0, 1 },
                               { 1, 0, 0, 0 },
                               { 0, 0, 1, 0 }};

        // Some cases with determinant 1 from problem F28 in Richard K. Guy,
        // "Unsolved Problems in Number Theory", 3rd ed., Springer, 2004.
        const MatrixInt fib1 {{ 1167, 2, 5 },
                              { 1698, 3, 8 },
                              { 2866, 5, 13 }};
        const MatrixInt fib2 {{ 610, 5, 13 },
                              { 1054, 8, 21 },
                              { 1665, 13, 34 }};

        const std::array<std::reference_wrapper<const MatrixInt>, 15> cases {
            std::cref(zero34), std::cref(identity3),
            std::cref(one), std::cref(minusOne), std::cref(two),
            std::cref(vector), std::cref(shear2), std::cref(square3),
            std::cref(rect34), std::cref(rect43),
            std::cref(redundant34), std::cref(duplicate34),
            std::cref(perm4), std::cref(fib1), std::cref(fib2)
        };
};

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

template <regina::CommutativeRing T>
void validateIdentity(const Matrix<T>& m, size_t size) {
    EXPECT_NO_THROW({ m.validate(); });
    ASSERT_EQ(m.rows(), size);
    ASSERT_EQ(m.columns(), size);
    for (size_t i = 0; i < size; ++i)
        for (size_t j = 0; j < size; ++j)
            EXPECT_EQ(m.entry(i, j), i == j ? 1 : 0);
}

template <regina::CommutativeRing T>
void validateZero(const Matrix<T>& m, size_t rows, size_t columns) {
    EXPECT_NO_THROW({ m.validate(); });
    ASSERT_EQ(m.rows(), rows);
    ASSERT_EQ(m.columns(), columns);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < columns; ++j)
            EXPECT_EQ(m.entry(i, j), 0);
}

TEST_F(MatrixTest, empty) {
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

    EXPECT_EQ(m.makeRowEchelon(), 0);
    validateEmpty(m);

    EXPECT_EQ(m.makeColumnEchelon(), 0);
    validateEmpty(m);
    {
        auto res = m.columnEchelonCoB();
        EXPECT_EQ(Matrix<Integer>(m).columnEchelonCoB(), res);

        auto [e, r, ri, rank] = std::move(res);
        validateEmpty(e);
        validateEmpty(r);
        validateEmpty(ri);
        EXPECT_EQ(rank, 0);
    }

    EXPECT_EQ(m.rank(), 0);
    EXPECT_EQ(Matrix<Integer>(m).rank(), 0);

    EXPECT_NO_THROW({ validateEmpty(m.inverse()); });
    EXPECT_NO_THROW({ validateEmpty(Matrix<Integer>(m).inverse()); });

    validateEmpty(m.smith());
    validateEmpty(Matrix<Integer>(m).smith());
    {
        auto res = m.smithCoB();
        EXPECT_EQ(Matrix<Integer>(m).smithCoB(), res);
        EXPECT_EQ(m.smithCoB(SNFAlgorithm::Metrical), res);
        EXPECT_EQ(Matrix<Integer>(m).smithCoB(SNFAlgorithm::Metrical), res);

        for (const auto& bit : res)
            validateEmpty(bit);
    }

    validateEmpty(m.preimageOfLattice({}));
}

TEST_F(MatrixTest, noRows) {
    // Test everything that we are allowed to do with a 0 x k matrix.
    validateNoRows(Matrix<Integer>(0, 3), 3);

    Matrix<Integer> noRows(0, 3);
    auto m = Matrix<Integer>::identity(4);
    EXPECT_EQ(m.rows(), 4);
    EXPECT_EQ(m.columns(), 4);

    m = noRows;
    validateNoRows(m, 3);
    validateNoRows(Matrix<Integer>(m), 3);
    validateNoRows(Matrix<Integer>(std::move(m)), 3);

    m = Matrix<Integer>::identity(4); // assignment after move-out
    EXPECT_NO_THROW({ m.validate(); });
    EXPECT_EQ(m.rows(), 4);
    EXPECT_EQ(m.columns(), 4);
    EXPECT_TRUE(m.isIdentity());

    m = std::move(noRows);
    validateNoRows(m, 3);

    {
        auto tmp = Matrix<Integer>::identity(4);
        EXPECT_NO_THROW({ tmp.validate(); });
        EXPECT_EQ(tmp.rows(), 4);
        EXPECT_EQ(tmp.columns(), 4);

        m.swap(tmp);
        validateNoRows(tmp, 3);
        EXPECT_NO_THROW({ m.validate(); });
        EXPECT_EQ(m.rows(), 4);
        EXPECT_EQ(m.columns(), 4);

        swap(m, tmp);
        validateNoRows(m, 3);
        EXPECT_NO_THROW({ tmp.validate(); });
        EXPECT_EQ(tmp.rows(), 4);
        EXPECT_EQ(tmp.columns(), 4);
    }

    m.fill(2);
    validateNoRows(m, 3);

    EXPECT_EQ(m, Matrix<Integer>(0, 3));
    EXPECT_EQ(m, m);

    validateNoColumns(m.transpose(), 3);

    m.swapCols(0, 1);
    validateNoRows(m, 3);
    m.swapCols(m.columns() - 2, m.columns() - 1);
    validateNoRows(m, 3);

    EXPECT_EQ(m.str(), "(empty 0x3 matrix)");
    EXPECT_EQ(m.detail(), "(empty 0x3 matrix)\n");

    m.makeIdentity();
    validateNoRows(m, 3);
    EXPECT_FALSE(m.isIdentity());
    EXPECT_TRUE(m.isZero());

    m.addCol(0, 1);
    validateNoRows(m, 3);
    m.addCol(1, 0);
    validateNoRows(m, 3);
    m.addCol(m.columns() - 2, m.columns() - 1);
    validateNoRows(m, 3);
    m.addCol(m.columns() - 1, m.columns() - 2);
    validateNoRows(m, 3);
    m.addColFrom(0, 1, 0);
    validateNoRows(m, 3);
    m.addColFrom(1, 0, 0);
    validateNoRows(m, 3);
    m.addColFrom(m.columns() - 2, m.columns() - 1, 0);
    validateNoRows(m, 3);
    m.addColFrom(m.columns() - 1, m.columns() - 2, 0);
    validateNoRows(m, 3);
    m.addCol(0, 1, 5);
    validateNoRows(m, 3);
    m.addCol(1, 0, 5);
    validateNoRows(m, 3);
    m.addCol(m.columns() - 2, m.columns() - 1, 5);
    validateNoRows(m, 3);
    m.addCol(m.columns() - 1, m.columns() - 2, 5);
    validateNoRows(m, 3);
    m.multCol(0, 5);
    validateNoRows(m, 3);
    m.multCol(m.columns() - 1, 5);
    validateNoRows(m, 3);
    m.combCols(0, 1, 4, 5, 6, 7);
    validateNoRows(m, 3);
    m.combCols(1, 0, 4, 5, 6, 7);
    validateNoRows(m, 3);
    m.combCols(m.columns() - 2, m.columns() - 1, 4, 5, 6, 7);
    validateNoRows(m, 3);
    m.combCols(m.columns() - 1, m.columns() - 2, 4, 5, 6, 7);
    validateNoRows(m, 3);

    m += Matrix<Integer>(0, 3);
    validateNoRows(m, 3);
    m += m;
    validateNoRows(m, 3);
    m -= Matrix<Integer>(0, 3);
    validateNoRows(m, 3);
    m -= m;
    validateNoRows(m, 3);
    m *= 2;
    validateNoRows(m, 3);
    m *= Integer(2);
    validateNoRows(m, 3);
    validateNoRows(m * 2, 3);
    validateNoRows(Matrix<Integer>(m) * 2, 3);
    EXPECT_NO_THROW({
        validateNoRows(m * rect34, 4);
        validateEmpty(m * Matrix<Integer>(3, 0));
        validateNoRows(Matrix<Integer>() * m, 3);
        validateZero(Matrix<Integer>(4, 0) * m, 4, 3);
    });
    EXPECT_THROW({ m * Matrix<Integer>(4, 3); }, regina::InvalidArgument);
    EXPECT_THROW({ m * Matrix<Integer>(0, 3); }, regina::InvalidArgument);
    EXPECT_THROW({ m * Matrix<Integer>(); }, regina::InvalidArgument);
    EXPECT_THROW({ Matrix<Integer>(4, 3) * m; }, regina::InvalidArgument);
    EXPECT_THROW({ Matrix<Integer>(0, 3) * m; }, regina::InvalidArgument);
    {
        auto ans = m * Vector<Integer>(3);
        EXPECT_EQ(ans.size(), 0);
    }
    EXPECT_THROW({ m * Vector<Integer>(0); }, regina::InvalidArgument);

    EXPECT_THROW({ m.trace(); }, regina::FailedPrecondition);
    EXPECT_THROW({ m.det(AdjugateAlgorithm::Default); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.det(AdjugateAlgorithm::FaddeevLeverrier); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.det(AdjugateAlgorithm::PreparataSarwate); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.det(AdjugateAlgorithm::MahajanVinay); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.adjugate(AdjugateAlgorithm::Default); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.adjugate(AdjugateAlgorithm::FaddeevLeverrier); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.adjugate(AdjugateAlgorithm::PreparataSarwate); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.adjugate(AdjugateAlgorithm::MahajanVinay); },
        regina::FailedPrecondition);

    m.negate();
    validateNoRows(m, 3);
    m.negateCol(0);
    validateNoRows(m, 3);
    m.negateCol(m.columns() - 1);
    validateNoRows(m, 3);
    m.divColExact(0, 17);
    validateNoRows(m, 3);
    m.divColExact(m.columns() - 1, 17);
    validateNoRows(m, 3);

    EXPECT_EQ(m.gcdCol(0), 0);
    EXPECT_EQ(m.gcdCol(m.columns() - 1), 0);
    m.reduceCol(0);
    validateNoRows(m, 3);
    m.reduceCol(m.columns() - 1);
    validateNoRows(m, 3);

    EXPECT_EQ(m.makeRowEchelon(), 0);
    validateNoRows(m, 3);
    EXPECT_EQ(m.makeColumnEchelon(), 0);
    validateNoRows(m, 3);
    EXPECT_EQ(m.rank(), 0);
    EXPECT_EQ(Matrix<Integer>(m).rank(), 0);
    {
        auto res = m.columnEchelonCoB();
        EXPECT_EQ(Matrix<Integer>(m).columnEchelonCoB(), res);

        auto [e, r, ri, rank] = std::move(res);
        validateNoRows(e, 3);
        validateIdentity(r, 3);
        validateIdentity(ri, 3);
        EXPECT_EQ(rank, 0);
    }

    EXPECT_THROW({ m.inverse(); }, regina::FailedPrecondition);
    EXPECT_THROW({ Matrix<Integer>(m).inverse(); }, regina::FailedPrecondition);

    validateNoRows(m.smith(), 3);
    validateNoRows(Matrix<Integer>(m).smith(), 3);
    {
        auto res = m.smithCoB();
        EXPECT_EQ(Matrix<Integer>(m).smithCoB(), res);
        EXPECT_EQ(m.smithCoB(SNFAlgorithm::Metrical), res);
        EXPECT_EQ(Matrix<Integer>(m).smithCoB(SNFAlgorithm::Metrical), res);

        validateNoRows(res[0], 3);
        validateIdentity(res[1], 3);
        validateIdentity(res[2], 3);
        validateEmpty(res[3]);
        validateEmpty(res[4]);
    }

    validateIdentity(m.preimageOfLattice({}), 3);
}

TEST_F(MatrixTest, noColumns) {
    // Test everything that we are allowed to do with a k x 0 matrix.
    validateNoColumns(Matrix<Integer>(3, 0), 3);
    validateNoColumns(Matrix<Integer>({ {}, {}, {} }), 3);

    Matrix<Integer> noColumns(3, 0);
    auto m = Matrix<Integer>::identity(4);
    EXPECT_EQ(m.rows(), 4);
    EXPECT_EQ(m.columns(), 4);

    m = noColumns;
    validateNoColumns(m, 3);
    validateNoColumns(Matrix<Integer>(m), 3);
    validateNoColumns(Matrix<Integer>(std::move(m)), 3);

    m = Matrix<Integer>::identity(4); // assignment after move-out
    EXPECT_NO_THROW({ m.validate(); });
    EXPECT_EQ(m.rows(), 4);
    EXPECT_EQ(m.columns(), 4);
    EXPECT_TRUE(m.isIdentity());

    m = std::move(noColumns);
    validateNoColumns(m, 3);

    {
        auto tmp = Matrix<Integer>::identity(4);
        EXPECT_NO_THROW({ tmp.validate(); });
        EXPECT_EQ(tmp.rows(), 4);
        EXPECT_EQ(tmp.columns(), 4);

        m.swap(tmp);
        validateNoColumns(tmp, 3);
        EXPECT_NO_THROW({ m.validate(); });
        EXPECT_EQ(m.rows(), 4);
        EXPECT_EQ(m.columns(), 4);

        swap(m, tmp);
        validateNoColumns(m, 3);
        EXPECT_NO_THROW({ tmp.validate(); });
        EXPECT_EQ(tmp.rows(), 4);
        EXPECT_EQ(tmp.columns(), 4);
    }

    m.fill(2);
    validateNoColumns(m, 3);

    EXPECT_EQ(m, Matrix<Integer>(3, 0));
    EXPECT_EQ(m, m);

    validateNoRows(m.transpose(), 3);

    m.swapRows(0, 1);
    validateNoColumns(m, 3);
    m.swapRows(m.rows() - 2, m.rows() - 1);
    validateNoColumns(m, 3);

    EXPECT_EQ(m.str(), "(empty 3x0 matrix)");
    EXPECT_EQ(m.detail(), "(empty 3x0 matrix)\n");

    m.makeIdentity();
    validateNoColumns(m, 3);
    EXPECT_FALSE(m.isIdentity());
    EXPECT_TRUE(m.isZero());

    m.addRow(0, 1);
    validateNoColumns(m, 3);
    m.addRow(1, 0);
    validateNoColumns(m, 3);
    m.addRow(m.rows() - 2, m.rows() - 1);
    validateNoColumns(m, 3);
    m.addRow(m.rows() - 1, m.rows() - 2);
    validateNoColumns(m, 3);
    m.addRowFrom(0, 1, 0);
    validateNoColumns(m, 3);
    m.addRowFrom(1, 0, 0);
    validateNoColumns(m, 3);
    m.addRowFrom(m.rows() - 2, m.rows() - 1, 0);
    validateNoColumns(m, 3);
    m.addRowFrom(m.rows() - 1, m.rows() - 2, 0);
    validateNoColumns(m, 3);
    m.addRow(0, 1, 5);
    validateNoColumns(m, 3);
    m.addRow(1, 0, 5);
    validateNoColumns(m, 3);
    m.addRow(m.rows() - 2, m.rows() - 1, 5);
    validateNoColumns(m, 3);
    m.addRow(m.rows() - 1, m.rows() - 2, 5);
    validateNoColumns(m, 3);
    m.multRow(0, 5);
    validateNoColumns(m, 3);
    m.multRow(m.rows() - 1, 5);
    validateNoColumns(m, 3);
    m.combRows(0, 1, 4, 5, 6, 7);
    validateNoColumns(m, 3);
    m.combRows(1, 0, 4, 5, 6, 7);
    validateNoColumns(m, 3);
    m.combRows(m.rows() - 2, m.rows() - 1, 4, 5, 6, 7);
    validateNoColumns(m, 3);
    m.combRows(m.rows() - 1, m.rows() - 2, 4, 5, 6, 7);
    validateNoColumns(m, 3);

    m += Matrix<Integer>(3, 0);
    validateNoColumns(m, 3);
    m += m;
    validateNoColumns(m, 3);
    m -= Matrix<Integer>(3, 0);
    validateNoColumns(m, 3);
    m -= m;
    validateNoColumns(m, 3);
    m *= 2;
    validateNoColumns(m, 3);
    m *= Integer(2);
    validateNoColumns(m, 3);
    validateNoColumns(m * 2, 3);
    validateNoColumns(Matrix<Integer>(m) * 2, 3);
    EXPECT_NO_THROW({
        validateNoColumns(rect43 * m, 4);
        validateEmpty(Matrix<Integer>(0, 3) * m);
        validateNoColumns(m * Matrix<Integer>(), 3);
        validateZero(m * Matrix<Integer>(0, 4), 3, 4);
    });
    EXPECT_THROW({ m * Matrix<Integer>(3, 4); }, regina::InvalidArgument);
    EXPECT_THROW({ m * Matrix<Integer>(3, 0); }, regina::InvalidArgument);
    EXPECT_THROW({ Matrix<Integer>() * m; }, regina::InvalidArgument);
    EXPECT_THROW({ Matrix<Integer>(3, 4) * m; }, regina::InvalidArgument);
    EXPECT_THROW({ Matrix<Integer>(3, 0) * m; }, regina::InvalidArgument);
    {
        auto ans = m * Vector<Integer>(0);
        EXPECT_EQ(ans.size(), 3);
        EXPECT_TRUE(ans.isZero());
    }
    EXPECT_THROW({ m * Vector<Integer>(3); }, regina::InvalidArgument);

    EXPECT_THROW({ m.trace(); }, regina::FailedPrecondition);
    EXPECT_THROW({ m.det(AdjugateAlgorithm::Default); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.det(AdjugateAlgorithm::FaddeevLeverrier); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.det(AdjugateAlgorithm::PreparataSarwate); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.det(AdjugateAlgorithm::MahajanVinay); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.adjugate(AdjugateAlgorithm::Default); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.adjugate(AdjugateAlgorithm::FaddeevLeverrier); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.adjugate(AdjugateAlgorithm::PreparataSarwate); },
        regina::FailedPrecondition);
    EXPECT_THROW({ m.adjugate(AdjugateAlgorithm::MahajanVinay); },
        regina::FailedPrecondition);

    m.negate();
    validateNoColumns(m, 3);
    m.negateRow(0);
    validateNoColumns(m, 3);
    m.negateRow(m.rows() - 1);
    validateNoColumns(m, 3);
    m.divRowExact(0, 17);
    validateNoColumns(m, 3);
    m.divRowExact(m.rows() - 1, 17);
    validateNoColumns(m, 3);

    EXPECT_EQ(m.gcdRow(0), 0);
    EXPECT_EQ(m.gcdRow(m.rows() - 1), 0);
    m.reduceRow(0);
    validateNoColumns(m, 3);
    m.reduceRow(m.rows() - 1);
    validateNoColumns(m, 3);

    EXPECT_EQ(m.makeRowEchelon(), 0);
    validateNoColumns(m, 3);
    EXPECT_EQ(m.makeColumnEchelon(), 0);
    validateNoColumns(m, 3);
    EXPECT_EQ(m.rank(), 0);
    EXPECT_EQ(Matrix<Integer>(m).rank(), 0);
    {
        auto res = m.columnEchelonCoB();
        EXPECT_EQ(Matrix<Integer>(m).columnEchelonCoB(), res);

        auto [e, r, ri, rank] = std::move(res);
        validateNoColumns(e, 3);
        validateEmpty(r);
        validateEmpty(ri);
        EXPECT_EQ(rank, 0);
    }

    EXPECT_THROW({ m.inverse(); }, regina::FailedPrecondition);
    EXPECT_THROW({ Matrix<Integer>(m).inverse(); }, regina::FailedPrecondition);

    validateNoColumns(m.smith(), 3);
    validateNoColumns(Matrix<Integer>(m).smith(), 3);
    {
        auto res = m.smithCoB();
        EXPECT_EQ(Matrix<Integer>(m).smithCoB(), res);
        EXPECT_EQ(m.smithCoB(SNFAlgorithm::Metrical), res);
        EXPECT_EQ(Matrix<Integer>(m).smithCoB(SNFAlgorithm::Metrical), res);

        validateNoColumns(res[0], 3);
        validateEmpty(res[1]);
        validateEmpty(res[2]);
        validateIdentity(res[3], 3);
        validateIdentity(res[4], 3);
    }

    validateEmpty(m.preimageOfLattice({ 2, 3, 5 }));
    validateEmpty(m.preimageOfLattice({ 0, 0, 0 }));
}

static void verifyEchelonForm(const MatrixInt& m) {
    SCOPED_TRACE_REGINA(m);

    MatrixInt trans = m.transpose();

    // We test row echelon form of m, and column echelon form of trans.
    // The assumption is that verifyEchelonForm() will be called on both m and
    // m.transpose(), thereby leading to the testing of all four variants.

    auto res = m.rowEchelon();
    auto resTrans = trans.columnEchelon();
    EXPECT_EQ(MatrixInt(m).rowEchelon(), res); // rvalue variant
    EXPECT_EQ(MatrixInt(trans).columnEchelon(), resTrans); // rvalue variant

    EXPECT_EQ(res.first, resTrans.first.transpose()); // compare matrices
    EXPECT_EQ(res.second, resTrans.second); // compare ranks

    EXPECT_EQ(m.rank(), res.second);
    EXPECT_EQ(MatrixInt(m).rank(), res.second); // rvalue variant

    EXPECT_EQ(res.first.rows(), m.rows());
    EXPECT_EQ(res.first.columns(), m.columns());

    // Verify that res.first is actually in row echelon form.
    {
        size_t fromCol = 0;
        for (size_t r = 0; r < m.rows(); ++r) {
            // The initial non-zero entry in this row must appear in
            // column ≥ fromCol.
            do {
                // Whether or not res.first[r, fromCol] is zero, the entire
                // column beneath this position must be zero.
                for (size_t i = r + 1; i < m.rows(); ++i)
                    EXPECT_EQ(res.first.entry(i, fromCol), 0);

                if (res.first.entry(r, fromCol) != 0)
                    break;
                ++fromCol;
            } while (fromCol < m.columns());

            if (fromCol == m.columns())
                break;

            // The first non-zero entry in this row is res.first[r, fromCol].
            auto corner = res.first.entry(r, fromCol);
            EXPECT_GT(corner, 0);
            for (size_t i = 0; i < r; ++i) {
                EXPECT_GE(res.first.entry(i, fromCol), 0);
                EXPECT_LT(res.first.entry(i, fromCol), corner);
            }

            ++fromCol;
            if (fromCol == m.columns())
                break;
        }
    }

    if (m.rows() == m.columns())
        EXPECT_EQ(res.first.det(), m.det().abs());

    // Ensure that res.first spans the same row space as m.
    {
        MatrixInt comb(2 * m.rows(), m.columns());
        for (size_t i = 0; i < m.rows(); ++i)
            for (size_t j = 0; j < m.columns(); ++j)
                comb.entry(i, j) = m.entry(i, j);
        for (size_t i = 0; i < m.rows(); ++i)
            for (size_t j = 0; j < m.columns(); ++j)
                comb.entry(m.rows() + i, j) = res.first.entry(i, j);
        EXPECT_EQ(comb.rank(), res.second);
    }

    // Compare results with the change-of-basis variant for column echelon form.
    {
        auto cob = trans.columnEchelonCoB();
        EXPECT_EQ(cob, MatrixInt(trans).columnEchelonCoB()); // rvalue variant

        auto [e, r, ri, rank] = std::move(cob);
        EXPECT_EQ(e, resTrans.first);
        EXPECT_EQ(rank, resTrans.second);

        ASSERT_EQ(r.rows(), trans.columns());
        ASSERT_EQ(r.columns(), trans.columns());
        ASSERT_EQ(ri.rows(), trans.columns());
        ASSERT_EQ(ri.columns(), trans.columns());
        EXPECT_EQ(trans * r, e);
        EXPECT_EQ(e * ri, trans);
        EXPECT_TRUE((r * ri).isIdentity());
        EXPECT_TRUE((ri * r).isIdentity());
    }
}

TEST_F(MatrixTest, echelonForm) {
    for (const MatrixInt& m : cases) {
        verifyEchelonForm(m);
        verifyEchelonForm(m.transpose());
    }
}

static void verifyRowBasis(const MatrixInt& m) {
    SCOPED_TRACE_REGINA(m);

    MatrixInt copy(m);
    copy.makeRowBasis();
    EXPECT_EQ(m.rowBasis(), copy);
    EXPECT_EQ(MatrixInt(m).rowBasis(), copy); // rvalue variant

    size_t rank = m.rank();
    ASSERT_EQ(copy.rows(), rank);
    ASSERT_EQ(copy.columns(), m.columns());
    EXPECT_EQ(copy.rank(), rank);

    // Ensure that copy spans the same row space as m.
    {
        MatrixInt comb(m.rows() + copy.rows(), m.columns());
        for (size_t i = 0; i < m.rows(); ++i)
            for (size_t j = 0; j < m.columns(); ++j)
                comb.entry(i, j) = m.entry(i, j);
        for (size_t i = 0; i < copy.rows(); ++i)
            for (size_t j = 0; j < m.columns(); ++j)
                comb.entry(m.rows() + i, j) = copy.entry(i, j);
        EXPECT_EQ(comb.rank(), rank);
    }

    // Ensure that every row of copy matches some row of m.
    // (There are faster ways of doing this, but our tests are not large.)
    for (size_t i = 0; i < copy.rows(); ++i) {
        bool found = false;
        for (size_t j = 0; j < m.rows(); ++j) {
            bool equal = true;
            for (size_t c = 0; c < m.columns(); ++c)
                if (copy.entry(i, c) != m.entry(j, c)) {
                    equal = false;
                    break;
                }
            if (equal) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    // Now verify the incremental basis for the orthogonal complement.
    auto [basis, comp] = m.rowBasisAndOrthComp();
    EXPECT_EQ(basis, copy);
    ASSERT_EQ(comp.rows(), m.columns());
    ASSERT_EQ(comp.columns(), m.columns());
    EXPECT_EQ(comp.rank(), m.columns());
    for (size_t i = 0; i < basis.rows(); ++i) {
        // Row i of basis should be orthogonal to all rows > i of comp.
        for (size_t j = i + 1; j < comp.rows(); ++j) {
            Integer sum;
            for (size_t k = 0; k < m.columns(); ++k)
                sum.addProduct(basis.entry(i, k), comp.entry(j, k));
            EXPECT_EQ(sum, 0);
        }
    }
}

TEST_F(MatrixTest, rowBasis) {
    for (const MatrixInt& m : cases) {
        verifyRowBasis(m);
        verifyRowBasis(m.transpose());
    }

    // Some more hand-crafted tests:
    verifyRowBasis({
        { 1, 2, 3 },
        { 5, 0, 2 },
        { 6, 2, 5 }, // redundant via rows 0,1
        { 8, 3, 1 }
    });
    verifyRowBasis({
        { 1, 2, 3 },
        { 5, 0, 2 },
        { 6, 2, 5 }, // redundant via rows 0,1
        { 11, 2, 7 } // redundant via rows 0,1
    });
    verifyRowBasis({
        { 1, 2, 3 },
        { 2, 4, 6 }, // redundant via row 0
        { 3, 6, 9 }, // redundant via row 0
        { 5, 0, 2 }
    });
    verifyRowBasis({
        { 1, 2, 3 },
        { 2, 4, 6 }, // redundant via row 0
        { 5, 0, 2 },
        { 3, 6, 9 }  // redundant via row 0
    });
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

TEST_F(MatrixTest, determinantAdjugate) {
    // We include here several simple tests with different (non-integer)
    // element types.
    using P = Polynomial<Integer>;
    using L = Laurent<Integer>;
    using L2 = Laurent2<Integer>;

    // Empty matrices:
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

    // Work through all of our usual square Matrix cases, and just a couple of
    // our usual non-square cases.
    using Alg = AdjugateAlgorithm;
    for (auto alg : { Alg::Default, Alg::FaddeevLeverrier,
            Alg::PreparataSarwate, Alg::MahajanVinay }) {
        EXPECT_THROW({ rect34.det(alg); }, regina::FailedPrecondition);
        EXPECT_THROW({ rect43.det(alg); }, regina::FailedPrecondition);
        EXPECT_THROW({ rect34.adjugate(alg); }, regina::FailedPrecondition);
        EXPECT_THROW({ rect43.adjugate(alg); }, regina::FailedPrecondition);
    }
    verifyAdjugate<Integer>(identity3, 1); // self-transpose
    verifyAdjugate<Integer>(one, 1); // self-transpose
    verifyAdjugate<Integer>(minusOne, -1); // self-transpose
    verifyAdjugate<Integer>(two, 2); // self-transpose
    verifyAdjugate<Integer>(shear2, 1);
    verifyAdjugate<Integer>(shear2.transpose(), 1);
    verifyAdjugate<Integer>(square3, -144);
    verifyAdjugate<Integer>(square3.transpose(), -144);
    verifyAdjugate<Integer>(perm4, -1);
    verifyAdjugate<Integer>(perm4.transpose(), -1);
    verifyAdjugate<Integer>(fib1, 1);
    verifyAdjugate<Integer>(fib1.transpose(), 1);
    verifyAdjugate<Integer>(fib2, 1);
    verifyAdjugate<Integer>(fib2.transpose(), 1);
}

static void verifyInverse(const MatrixInt& m) {
    SCOPED_TRACE_REGINA(m);

    if (m.rows() != m.columns()) {
        EXPECT_THROW({ m.inverse(); }, regina::FailedPrecondition);
        EXPECT_THROW({ MatrixInt(m).inverse(); }, regina::FailedPrecondition);
        return;
    }

    auto det = m.det();
    if (det != 1 && det != -1) {
        EXPECT_THROW({ m.inverse(); }, regina::NoSolution);
        EXPECT_THROW({ MatrixInt(m).inverse(); }, regina::NoSolution);
        return;
    }

    MatrixInt inv;
    EXPECT_NO_THROW({ inv = m.inverse(); });
    EXPECT_NO_THROW({ EXPECT_EQ(MatrixInt(m).inverse(), inv); }); // rvalue
    EXPECT_EQ(inv.rows(), m.rows());
    EXPECT_EQ(inv.columns(), m.rows());
    EXPECT_TRUE((m * inv).isIdentity());
    EXPECT_TRUE((inv * m).isIdentity());
}

TEST_F(MatrixTest, inverse) {
    for (const MatrixInt& m : cases) {
        verifyInverse(m);
        verifyInverse(m.transpose());
    }
}

static void verifySmith(const MatrixInt& m, std::initializer_list<long> diag) {
    SCOPED_TRACE_REGINA(m);

    MatrixInt snf = m.smith();
    ASSERT_EQ(MatrixInt(m).smith(), snf); // rvalue variant

    ASSERT_EQ(snf.rows(), m.rows());
    ASSERT_EQ(snf.columns(), m.columns());

    for (size_t r = 0; r < snf.rows(); ++r)
        for (size_t c = 0; c < snf.columns(); ++c) {
            if (r != c || r >= diag.size())
                EXPECT_EQ(snf.entry(r, c), 0);
        }

    size_t i = 0;
    for (auto d : diag) {
        EXPECT_EQ(snf.entry(i, i), d);
        ++i;
    }

    for (auto alg : { SNFAlgorithm::HafnerMcCurley, SNFAlgorithm::Metrical }) {
        auto cobResult = m.smithCoB(alg);
        EXPECT_EQ(MatrixInt(m).smithCoB(alg), cobResult); // rvalue variant

        auto [snfAlt, R, invR, C, invC] = std::move(cobResult);

        // Note: different algorithms could use different row/column moves and
        // thereby give different change-of-basis matrices.
        EXPECT_EQ(snf, snfAlt);

        ASSERT_EQ(R.rows(), m.columns());
        ASSERT_EQ(R.columns(), m.columns());
        ASSERT_EQ(invR.rows(), m.columns());
        ASSERT_EQ(invR.columns(), m.columns());
        ASSERT_EQ(C.rows(), m.rows());
        ASSERT_EQ(C.columns(), m.rows());
        ASSERT_EQ(invC.rows(), m.rows());
        ASSERT_EQ(invC.columns(), m.rows());

        EXPECT_TRUE((R * invR).isIdentity());
        EXPECT_TRUE((C * invC).isIdentity());
        EXPECT_EQ((C * m * R), snfAlt);
        EXPECT_EQ((invC * snfAlt * invR), m);
    }
}

static void verifySmithWithTranspose(const MatrixInt& m,
        std::initializer_list<long> diag) {
    verifySmith(m, diag);
    verifySmith(m.transpose(), diag);
}

TEST_F(MatrixTest, smith) {
    verifySmithWithTranspose(zero34, { });
    verifySmithWithTranspose(identity3, { 1, 1, 1 });
    verifySmithWithTranspose(one, { 1 });
    verifySmithWithTranspose(minusOne, { 1 });
    verifySmithWithTranspose(two, { 2 });
    verifySmithWithTranspose(vector, { 2 });
    verifySmithWithTranspose(shear2, { 1, 1 });
    verifySmithWithTranspose(square3, { 2, 6, 12 });
    verifySmithWithTranspose(rect34, { 1, 1, 6 });
    verifySmithWithTranspose(rect43, { 1, 1, 12 });
    verifySmithWithTranspose(redundant34, { 1, 4 });
    verifySmithWithTranspose(duplicate34, { 1, 1, 1 });
    verifySmithWithTranspose(perm4, { 1, 1, 1, 1 });
    verifySmithWithTranspose(fib1, { 1, 1, 1 });
    verifySmithWithTranspose(fib2, { 1, 1, 1 });
}

TEST_F(MatrixTest, preimageOfLattice) {
    // We begin with a small ad-hoc test with both free and torsion parts.
    //
    // The columns of the resulting matrix are not well-defined: any
    // collection of linearly independent columns with the same span over the
    // integers will do.
    //
    // The result shown here is what regina's (deterministic) algorithm
    // currently produces, and this has been checked by hand for correctness.
    EXPECT_EQ(MatrixInt({{0,3,0,2}, {0,0,5,0}}).preimageOfLattice({6,0}),
        MatrixInt({{0,1,0}, {6,0,-2}, {0,0,0}, {-6,0,3}}));
}

