
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/matrixops.h"
#include "maths/nmatrixint.h"
#include "testsuite/maths/testmaths.h"

using regina::NMatrixInt;

class MatrixOpsTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MatrixOpsTest);

    CPPUNIT_TEST(smithNormalForm);
    CPPUNIT_TEST(smithNormalFormBasis);

    CPPUNIT_TEST_SUITE_END();

    private:
        NMatrixInt square3;
            /**< A simple 3-by-3 integer matrix given as an example in the
                 Smith normal form page on Wikipedia (September 2007).
                 The Smith normal form has diagonal (2, 6, 12). */

        NMatrixInt rect34;
            /**< A more complex 3-by-4 example whose Smith normal form has
                 diagonal (1, 1, 6). */

        NMatrixInt rect43;
            /**< A 4-by-3 matrix, filled with the same data as \a rect34 but
                 wrapping to a new row every third element instead of every
                 fourth.  The Smith normal form has diagonal (1, 1, 12). */

        NMatrixInt red43;
            /**< A 4-by-3 matrix with both redundant rows and columns.
                 The Smith normal form has diagonal (1, 4, 0). */

    public:
        MatrixOpsTest() : square3(3, 3), rect34(3, 4), rect43(4, 3),
                red43(4, 3) {
        }

        void setUp() {
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

            red43.entry(0, 0) = 3; red43.entry(0, 1) = 1;
            red43.entry(0, 2) = 2;
            red43.entry(1, 0) = 8; red43.entry(1, 1) = 4;
            red43.entry(1, 2) = 8;
            red43.entry(2, 0) = 11; red43.entry(2, 1) = 5;
            red43.entry(2, 2) = 10;
            red43.entry(3, 0) = -5; red43.entry(3, 1) = -3;
            red43.entry(3, 2) = -6;
        }

        void tearDown() {
        }

        static void checkSNF3(const NMatrixInt& m, const char* name,
                long d0, long d1, long d2) {
            NMatrixInt ans(m);
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
                    if (r != c && ans.entry(r, c) != 0) {
                        std::ostringstream msg;
                        msg << "Smith normal form for " << name
                            << " has a non-zero off-diagonal entry at "
                            << "(" << r << ", " << c << ").";
                        CPPUNIT_FAIL(msg.str());
                    } else if (r == c && r > 0) {
                        if (ans.entry(r, c) % ans.entry(r - 1, c - 1) != 0) {
                            std::ostringstream msg;
                            msg << "Smith normal form for " << name
                                << " breaks the divisibility chain: "
                                << ans.entry(r, c) << " does not divide "
                                << ans.entry(r - 1, c - 1) << ".";
                            CPPUNIT_FAIL(msg.str());
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
            checkSNF3(square3, "simple 3x3 example", 2, 6, 12);
            checkSNF3(rect34, "simple 3x4 example", 1, 1, 6);
            checkSNF3(rect43, "simple 4x3 example", 1, 1, 12);
            checkSNF3(red43, "redundant 4x3 example", 1, 4, 0);
        }

        static void checkSNFBasis(const NMatrixInt& m, const char* name) {
            // Assume here that the one-argument smithNormalForm() is known to
            // be working.
            //
            // What we do here is examine the change of basis matrices and
            // verify that they behave as advertised.

            // Get the SNF result from the one-argument routine.
            NMatrixInt snf(m);
            regina::smithNormalForm(snf);

            // Do it now with the five-argument routine, to collect
            // change of basis matrices.
            NMatrixInt snfBasis(m);
            NMatrixInt R(m.columns(), m.columns());
            NMatrixInt C(m.rows(), m.rows());
            NMatrixInt invR(R);
            NMatrixInt invC(C);

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
            checkSNFBasis(square3, "simple 3x3 example");
            checkSNFBasis(rect34, "simple 3x4 example");
            checkSNFBasis(rect43, "simple 4x3 example");
            checkSNFBasis(red43, "redundant 4x3 example");
        }
};

void addMatrixOps(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(MatrixOpsTest::suite());
}

