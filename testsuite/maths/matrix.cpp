
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

using regina::Integer;
using regina::Matrix;
using regina::MatrixInt;

class MatrixTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MatrixTest);

    CPPUNIT_TEST(echelonForm);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        void verifyEchelonForm(const char* name,
                std::initializer_list<std::initializer_list<Integer>> data) {
            MatrixInt init(data);

            MatrixInt m = init;
            MatrixInt n = init.transpose();

            {
                // Use the more complex global columnEchelonForm() to compare.
                MatrixInt r = MatrixInt::identity(init.columns());
                MatrixInt ri = MatrixInt::identity(init.columns());
                std::vector<unsigned> rowList;
                for (unsigned i = 0; i < init.rows(); ++i)
                    rowList.push_back(i);
                regina::columnEchelonForm(init, r, ri, rowList);
            }

            unsigned long rankCol = m.columnEchelonForm();
            unsigned long rankRow = n.rowEchelonForm();

            if (rankCol != rankRow) {
                std::ostringstream msg;
                msg << "Row/column echelon forms for " << name
                    << " give different ranks.";
                CPPUNIT_FAIL(msg.str());
            }
            if (n.transpose() != m) {
                std::ostringstream msg;
                msg << "Column and row echelon forms for " << name
                    << " and its transpose give different results.";
                CPPUNIT_FAIL(msg.str());
            }
            if (m != init) {
                std::ostringstream msg;
                msg << "Global vs member column echelon forms for " << name
                    << " give different results.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void echelonForm() {
            verifyEchelonForm("zero 3x4 example",
                {{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }});
            verifyEchelonForm("zero 4x3 example",
                {{ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }});
            verifyEchelonForm("identity 3x3 example",
                {{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }});
            verifyEchelonForm("simple 3x3 example",
                {{ 2, 4, 4 }, { -6, 6, 12 }, { 10, -4, -16 }});
            verifyEchelonForm("simple 3x4 example",
                {{ 4, -17, 0, 6 }, { -2, 4, 9, 0 }, { 6, -3, -2, 10 }});
            verifyEchelonForm("simple 4x3 example",
                {{ 4, -17, 0 }, { 6, -2, 4 }, { 9, 0, 6 }, { -3, -2, 10 }});
            verifyEchelonForm("duplicate column example",
                {{ 1, 1, 1, 1 }, { 0, 0, 2, 3 }, { 0, 0, 3, 5 }});
            verifyEchelonForm("duplicate row example",
                {{ 1, 0, 0 }, { 1, 0, 0 }, { 1, 2, 3 }, { 1, 3, 5 }});
            verifyEchelonForm("4x3 redundant rows and columns",
                {{ 3, 1, 2 }, { 8, 4, 8 }, { 11, 5, 10 }, { -5, -3, -6 }});
            verifyEchelonForm("3x4 redundant rows and columns",
                {{ 3, 8, 11, -5 }, { 1, 4, 5, -3 }, { 2, 8, 10, -6 }});
        }
};

void addMatrix(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(MatrixTest::suite());
}

