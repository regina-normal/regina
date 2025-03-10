
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "maths/matrixops.h"
#include "maths/matrix.h"

#include "testhelper.h"

using regina::MatrixInt;

class MatrixTest : public testing::Test {
    protected:
        MatrixInt zero34 { 3, 4 };
        MatrixInt zero43 { 4, 3 };
        MatrixInt identity3 { MatrixInt::identity(3) };

        // SNF diagonal: (2, 6, 12)
        MatrixInt square3 {{ 2, 4, 4 },
                           { -6, 6, 12 },
                           { 10, -4, -16 }};

        // SNF diagonal: (1, 1, 6)
        MatrixInt rect34 {{ 4, -17, 0, 6 },
                          { -2, 4, 9, 0 },
                          { 6, -3, -2, 10 }};

        // SNF diagonal: (1, 1, 12)
        MatrixInt rect43 {{ 4, -17, 0 },
                          { 6, -2, 4 },
                          { 9, 0, 6 },
                          { -3, -2, 10 }};

        // SNF diagonal: (1, 4, 0)
        MatrixInt redundant34 {{ 3, 8, 11, -5 },
                               { 1, 4, 5, -3 },
                               { 2, 8, 10, -6 }};
        MatrixInt redundant43 { redundant34.transpose() };

        // SNF diagonal: (1, 1, 1); has a duplicate column
        MatrixInt duplicate34 {{ 1, 1, 1, 1 },
                               { 0, 0, 2, 3 },
                               { 0, 0, 3, 5 }};
        MatrixInt duplicate43 { duplicate34.transpose() };
};

static void verifySNF(const MatrixInt& m, std::initializer_list<long> diag) {
    SCOPED_TRACE_REGINA(m);

    MatrixInt ans(m);
    regina::smithNormalForm(ans);

    ASSERT_EQ(ans.rows(), m.rows());
    ASSERT_EQ(ans.columns(), m.columns());

    for (size_t r = 0; r < ans.rows(); ++r)
        for (size_t c = 0; c < ans.columns(); ++c) {
            if (r != c || r >= diag.size())
                EXPECT_EQ(ans.entry(r, c), 0);
        }

    size_t i = 0;
    for (auto d : diag) {
        EXPECT_EQ(ans.entry(i, i), d);
        ++i;
    }
}

TEST_F(MatrixTest, smithNormalForm) {
    verifySNF(zero34, { });
    verifySNF(zero43, { });
    verifySNF(identity3, { 1, 1, 1 });
    verifySNF(square3, { 2, 6, 12 });
    verifySNF(rect34, { 1, 1, 6 });
    verifySNF(rect43, { 1, 1, 12 });
    verifySNF(redundant34, { 1, 4 });
    verifySNF(redundant43, { 1, 4 });
    verifySNF(duplicate34, { 1, 1, 1 });
    verifySNF(duplicate43, { 1, 1, 1 });
}

static void verifySNFBasis(const MatrixInt& m, bool metrical) {
    SCOPED_TRACE_REGINA(m);

    // We assume here that the one-argument smithNormalForm() is known to
    // be working.  We compare its results with the change-of-basis variant
    // of smithNormalForm(), and verify that the change-of-basis matrices
    // behave as advertised.

    // Get the SNF result from the one-argument routine.
    MatrixInt snf(m);
    regina::smithNormalForm(snf);

    // Do it now with the five-argument routine, to collect
    // change of basis matrices.
    MatrixInt snfBasis(m);
    MatrixInt R, C, invR, invC;
    if (metrical)
        regina::metricalSmithNormalForm(snfBasis, R, invR, C, invC);
    else
        regina::smithNormalForm(snfBasis, R, invR, C, invC);

    EXPECT_EQ(snf, snfBasis);

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
    EXPECT_EQ((C * m * R), snfBasis);
    EXPECT_EQ((invC * snfBasis * invR), m);
}

TEST_F(MatrixTest, smithNormalFormBasis) {
    verifySNFBasis(zero34, false);
    verifySNFBasis(zero43, false);
    verifySNFBasis(identity3, false);
    verifySNFBasis(square3, false);
    verifySNFBasis(rect34, false);
    verifySNFBasis(rect43, false);
    verifySNFBasis(redundant34, false);
    verifySNFBasis(redundant43, false);
    verifySNFBasis(duplicate34, false);
    verifySNFBasis(duplicate43, false);
}

TEST_F(MatrixTest, metricalSmithNormalForm) {
    verifySNFBasis(zero34, true);
    verifySNFBasis(zero43, true);
    verifySNFBasis(identity3, true);
    verifySNFBasis(square3, true);
    verifySNFBasis(rect34, true);
    verifySNFBasis(rect43, true);
    verifySNFBasis(redundant34, true);
    verifySNFBasis(redundant43, true);
    verifySNFBasis(duplicate34, true);
    verifySNFBasis(duplicate43, true);
}

static void verifyEchelonForm(const MatrixInt& m) {
    SCOPED_TRACE_REGINA(m);

    MatrixInt m1 = m;
    MatrixInt m2 = m.transpose();

    size_t rankCol = m1.columnEchelonForm();
    size_t rankRow = m2.rowEchelonForm();

    EXPECT_EQ(rankCol, rankRow);
    EXPECT_EQ(m2.transpose(), m1);

    // Verify that m2 is actually in row echelon form.
    {
        size_t fromCol = 0;
        for (size_t r = 0; r < m2.rows(); ++r) {
            // The initial non-zero entry in this row must appear in
            // column ≥ fromCol.
            do {
                // Whether or not m2[r, fromCol] is zero, the entire
                // column beneath this position must be zero.
                for (size_t i = r + 1; i < m2.rows(); ++i)
                    EXPECT_EQ(m2.entry(i, fromCol), 0);

                if (m2.entry(r, fromCol) != 0)
                    break;
                ++fromCol;
            } while (fromCol < m2.columns());

            if (fromCol == m2.columns())
                break;

            // The first non-zero entry in this row is m2[r, fromCol].
            auto corner = m2.entry(r, fromCol);
            EXPECT_GT(corner, 0);
            for (size_t i = 0; i < r; ++i) {
                EXPECT_GE(m2.entry(i, fromCol), 0);
                EXPECT_LT(m2.entry(i, fromCol), corner);
            }

            ++fromCol;
            if (fromCol == m2.columns())
                break;
        }
    }

    // Compare results with the more complex global columnEchelonForm().
    MatrixInt copy(m);
    auto r = MatrixInt::identity(copy.columns());
    auto ri = MatrixInt::identity(copy.columns());
    std::vector<size_t> rowList;
    for (size_t i = 0; i < copy.rows(); ++i)
        rowList.push_back(i);
    regina::columnEchelonForm(copy, r, ri, rowList);

    EXPECT_EQ(copy, m1);
}

TEST_F(MatrixTest, echelonForm) {
    verifyEchelonForm(zero34);
    verifyEchelonForm(zero43);
    verifyEchelonForm(identity3);
    verifyEchelonForm(square3);
    verifyEchelonForm(rect34);
    verifyEchelonForm(rect43);
    verifyEchelonForm(redundant34);
    verifyEchelonForm(redundant43);
    verifyEchelonForm(duplicate34);
    verifyEchelonForm(duplicate43);
}
