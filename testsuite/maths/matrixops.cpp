
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/matrixops.h"
#include "maths/matrix.h"
#include "testsuite/maths/testmaths.h"

using regina::MatrixInt;

class MatrixOpsTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MatrixOpsTest);

    CPPUNIT_TEST(smithNormalForm);
    CPPUNIT_TEST(smithNormalFormBasis);
    CPPUNIT_TEST(metricalSmithNormalForm);

    CPPUNIT_TEST_SUITE_END();

    private:
        MatrixInt zero34;
            /**< A zero 3-by-4 matrix. */

        MatrixInt zero43;
            /**< A zero 4-by-3 matrix. */

        MatrixInt identity3;
            /**< A 3-by-3 identity matrix. */

        MatrixInt square3;
            /**< A simple 3-by-3 integer matrix given as an example in the
                 Smith normal form page on Wikipedia (September 2007).
                 The Smith normal form has diagonal (2, 6, 12). */

        MatrixInt rect34;
            /**< A more complex 3-by-4 example whose Smith normal form has
                 diagonal (1, 1, 6). */

        MatrixInt rect43;
            /**< A 4-by-3 matrix, filled with the same data as \a rect34 but
                 wrapping to a new row every third element instead of every
                 fourth.  The Smith normal form has diagonal (1, 1, 12). */

        MatrixInt red43;
            /**< A 4-by-3 matrix with both redundant rows and columns.
                 The Smith normal form has diagonal (1, 4, 0). */

        MatrixInt dup34;
            /**< A 3-by-4 matrix with a trivial duplicate row and a Smith
                 normal form with diagonal (1, 1, 1). */

        MatrixInt dup43;
            /**< The exact transpose of \a dup34. */

    public:
        MatrixOpsTest() : zero34(3, 4), zero43(4, 3), identity3(3, 3),
                square3(3, 3), rect34(3, 4), rect43(4, 3),
                red43(4, 3), dup34(3, 4), dup43(4, 3) {
        }

        void setUp() {
            zero34.initialise(0L);
            zero43.initialise(0L);
            identity3.makeIdentity();

            square3.entry(0, 0) = 2;   square3.entry(0, 1) = 4;
            square3.entry(0, 2) = 4;
            square3.entry(1, 0) = -6;  square3.entry(1, 1) = 6;
            square3.entry(1, 2) = 12;
            square3.entry(2, 0) = 10;  square3.entry(2, 1) = -4;
            square3.entry(2, 2) = -16;

            rect34.entry(0, 0) = 4;  rect34.entry(0, 1) = -17;
            rect34.entry(0, 2) = 0L; rect34.entry(0, 3) = 6;
            rect34.entry(1, 0) = -2; rect34.entry(1, 1) = 4;
            rect34.entry(1, 2) = 9;  rect34.entry(1, 3) = 0L;
            rect34.entry(2, 0) = 6;  rect34.entry(2, 1) = -3;
            rect34.entry(2, 2) = -2; rect34.entry(2, 3) = 10;

            int i;
            for (i = 0; i < 12; ++i)
                rect43.entry(i / 3, i % 3) = rect34.entry(i / 4, i % 4);

            red43.entry(0, 0) = 3;  red43.entry(0, 1) = 1;
            red43.entry(0, 2) = 2;
            red43.entry(1, 0) = 8;  red43.entry(1, 1) = 4;
            red43.entry(1, 2) = 8;
            red43.entry(2, 0) = 11; red43.entry(2, 1) = 5;
            red43.entry(2, 2) = 10;
            red43.entry(3, 0) = -5; red43.entry(3, 1) = -3;
            red43.entry(3, 2) = -6;

            dup34.entry(0, 0) = 1;  dup34.entry(0, 1) = 1;
            dup34.entry(0, 2) = 1;  dup34.entry(0, 3) = 1;
            dup34.entry(1, 0) = 0L; dup34.entry(1, 1) = 0L;
            dup34.entry(1, 2) = 2;  dup34.entry(1, 3) = 3;
            dup34.entry(2, 0) = 0L; dup34.entry(2, 1) = 0L;
            dup34.entry(2, 2) = 3;  dup34.entry(2, 3) = 5;

            for (i = 0; i < 12; ++i)
                dup43.entry(i / 3, i % 3) = dup34.entry(i % 3, i / 3);
        }

        void tearDown() {
        }

        static void checkSNF3(const MatrixInt& m, const char* name,
                long d0, long d1, long d2) {
            MatrixInt ans(m);
            regina::smithNormalForm(ans);

            if (ans.rows() != m.rows() || ans.columns() != m.columns()) {
                std::ostringstream msg;
                msg << "Smith normal form for " << name
                    << " has incorrect dimensions.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned r, c;
            for (r = 0; r < m.rows(); ++r)
                for (c = 0; c < m.columns(); ++c)
                    if (r != c) {
                        // Off the main diagonal.
                        if (ans.entry(r, c) != 0) {
                            std::ostringstream msg;
                            msg << "Smith normal form for " << name
                                << " has a non-zero off-diagonal entry at "
                                << "(" << r << ", " << c << ").";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        // On the main diagonal.
                        if (ans.entry(r, c) < 0) {
                            std::ostringstream msg;
                            msg << "Smith normal form for " << name
                                << " has a negative diagonal entry at "
                                << "(" << r << ", " << c << ").";
                            CPPUNIT_FAIL(msg.str());
                        }

                        if (r > 0) {
                            if (ans.entry(r - 1, c - 1) == 0) {
                                // We're following a zero.  Better be all zeroes
                                // from here onwards.
                                if (ans.entry(r, c) != 0) {
                                    std::ostringstream msg;
                                    msg << "Smith normal form for " << name
                                        << " has non-zero entries following"
                                           " zero entries on the main"
                                           " diagonal.";
                                    CPPUNIT_FAIL(msg.str());
                                }
                            } else {
                                // We're not following a zero.  Check for
                                // divisibility.
                                if (ans.entry(r, c) %
                                        ans.entry(r - 1, c - 1) != 0) {
                                    std::ostringstream msg;
                                    msg << "Smith normal form for " << name
                                        << " breaks the divisibility chain: "
                                        << ans.entry(r, c)
                                        << " does not divide "
                                        << ans.entry(r - 1, c - 1) << ".";
                                    CPPUNIT_FAIL(msg.str());
                                }
                            }
                        }
                    }

            if (ans.entry(0, 0) != d0) {
                std::ostringstream msg;
                msg << "Smith normal form for " << name
                    << " has (0, 0) entry " << ans.entry(0, 0)
                    << " instead of " << d0 << ".";
                CPPUNIT_FAIL(msg.str());
            }
            if (ans.entry(1, 1) != d1) {
                std::ostringstream msg;
                msg << "Smith normal form for " << name
                    << " has (1, 1) entry " << ans.entry(1, 1)
                    << " instead of " << d1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
            if (ans.entry(2, 2) != d2) {
                std::ostringstream msg;
                msg << "Smith normal form for " << name
                    << " has (2, 2) entry " << ans.entry(2, 2)
                    << " instead of " << d2 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void smithNormalForm() {
            checkSNF3(zero34, "zero 3x4 example", 0, 0, 0);
            checkSNF3(zero43, "zero 4x3 example", 0, 0, 0);
            checkSNF3(identity3, "identity 3x3 example", 1, 1, 1);
            checkSNF3(square3, "simple 3x3 example", 2, 6, 12);
            checkSNF3(rect34, "simple 3x4 example", 1, 1, 6);
            checkSNF3(rect43, "simple 4x3 example", 1, 1, 12);
            checkSNF3(red43, "redundant 4x3 example", 1, 4, 0);
            checkSNF3(dup34, "duplicated 3x4 example", 1, 1, 1);
            checkSNF3(dup43, "duplicated 4x3 example", 1, 1, 1);
        }

        static void checkSNFBasis(const MatrixInt& m, const char* name) {
            // Assume here that the one-argument smithNormalForm() is known to
            // be working.
            //
            // What we do here is examine the change of basis matrices and
            // verify that they behave as advertised.

            // Get the SNF result from the one-argument routine.
            MatrixInt snf(m);
            regina::smithNormalForm(snf);

            // Do it now with the five-argument routine, to collect
            // change of basis matrices.
            MatrixInt snfBasis(m);
            MatrixInt R(m.columns(), m.columns());
            MatrixInt C(m.rows(), m.rows());
            MatrixInt invR(R);
            MatrixInt invC(C);

            regina::smithNormalForm(snfBasis, R, invR, C, invC);

            if (! (R * invR)->isIdentity()) {
                CPPUNIT_FAIL(std::string("In smithNormalForm(") +
                    name + "), rowSpaceBasis and rowSpaceBasisInv are "
                    "not inverses.");
            }

            if (! (C * invC)->isIdentity()) {
                CPPUNIT_FAIL(std::string("In smithNormalForm(") +
                    name + "), colSpaceBasis and colSpaceBasisInv are "
                    "not inverses.");
            }

            if (*(C * *(m * R)) != snfBasis) {
                CPPUNIT_FAIL(std::string("In smithNormalForm(") +
                    name + "), colSpaceBasis and rowSpaceBasis do not "
                    "satisfy the required relationship.");
            }

            if (*(invC * *(snfBasis * invR)) != m) {
                CPPUNIT_FAIL(std::string("In smithNormalForm(") +
                    name + "), colSpaceBasisInv and rowSpaceBasisInv do "
                    "not satisfy the required relationship.");
            }
        }

        void smithNormalFormBasis() {
            checkSNFBasis(zero34, "zero 3x4 example");
            checkSNFBasis(zero43, "zero 4x3 example");
            checkSNFBasis(identity3, "identity 3x3 example");
            checkSNFBasis(square3, "simple 3x3 example");
            checkSNFBasis(rect34, "simple 3x4 example");
            checkSNFBasis(rect43, "simple 4x3 example");
            checkSNFBasis(red43, "redundant 4x3 example");
            checkSNFBasis(dup34, "duplicated 3x4 example");
            checkSNFBasis(dup43, "duplicated 4x3 example");
        }

        static void checkMetricalSNF(const MatrixInt& m, const char* name) {
            // Assume here that the one-argument smithNormalForm() is known to
            // be working.
            //
            // What we do here is examine the change of basis matrices and
            // verify that they behave as advertised.

            // Get the SNF result from the one-argument routine.
            MatrixInt snf(m);
            regina::smithNormalForm(snf);

            // Do it now with the five-argument routine, to collect
            // change of basis matrices.
            MatrixInt snfBasis(m);
            MatrixInt R(m.columns(), m.columns());
            MatrixInt C(m.rows(), m.rows());
            MatrixInt invR(R);
            MatrixInt invC(C);

            regina::metricalSmithNormalForm(snfBasis, &R, &invR, &C, &invC);

            if (! (R * invR)->isIdentity()) {
                CPPUNIT_FAIL(std::string("In metricalSmithNormalForm(") +
                    name + "), rowSpaceBasis and rowSpaceBasisInv are "
                    "not inverses.");
            }

            if (! (C * invC)->isIdentity()) {
                CPPUNIT_FAIL(std::string("In metricalSmithNormalForm(") +
                    name + "), colSpaceBasis and colSpaceBasisInv are "
                    "not inverses.");
            }

            if (*(C * *(m * R)) != snfBasis) {
                CPPUNIT_FAIL(std::string("In metricalSmithNormalForm(") +
                    name + "), colSpaceBasis and rowSpaceBasis do not "
                    "satisfy the required relationship.");
            }

            if (*(invC * *(snfBasis * invR)) != m) {
                CPPUNIT_FAIL(std::string("In metricalSmithNormalForm(") +
                    name + "), colSpaceBasisInv and rowSpaceBasisInv do "
                    "not satisfy the required relationship.");
            }
        }

        void metricalSmithNormalForm() {
            checkMetricalSNF(zero34, "zero 3x4 example");
            checkMetricalSNF(zero43, "zero 4x3 example");
            checkMetricalSNF(identity3, "identity 3x3 example");
            checkMetricalSNF(square3, "simple 3x3 example");
            checkMetricalSNF(rect34, "simple 3x4 example");
            checkMetricalSNF(rect43, "simple 4x3 example");
            checkMetricalSNF(red43, "redundant 4x3 example");
            checkMetricalSNF(dup34, "duplicated 3x4 example");
            checkMetricalSNF(dup43, "duplicated 4x3 example");
        }
};

void addMatrixOps(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(MatrixOpsTest::suite());
}

