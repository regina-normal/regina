
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "maths/binom.h"
#include "testsuite/maths/testmaths.h"

using regina::binomSmall;
using regina::binomMedium;

class BinomialTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(BinomialTest);

    CPPUNIT_TEST(consistency);
    CPPUNIT_TEST(relations);

    CPPUNIT_TEST_SUITE_END();

    private:
        static constexpr int maxSmall = 16;
        static constexpr int maxMedium = 29;

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        void consistency() {
            int n, k;
            for (n = 0; n <= maxSmall; ++n)
                for (k = 0; k <= n; ++k) {
                    CPPUNIT_ASSERT_EQUAL(
                        static_cast<long>(binomSmall(n, k)),
                        binomMedium(n, k));
                }
        }

        void relations() {
            int n, k;
            for (n = 0; n <= maxMedium; ++n) {
                CPPUNIT_ASSERT_EQUAL(binomMedium(n, 0), long(1));
                CPPUNIT_ASSERT_EQUAL(binomMedium(n, n), long(1));
                if (n > 0) {
                    CPPUNIT_ASSERT_EQUAL(binomMedium(n, 1), long(n));
                    CPPUNIT_ASSERT_EQUAL(binomMedium(n, n-1), long(n));

                    for (k = 1; k < n; ++k)
                        CPPUNIT_ASSERT_EQUAL(binomMedium(n, k),
                            binomMedium(n-1, k-1) + binomMedium(n-1, k));
                }
            }
        }
};

void addBinomial(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(BinomialTest::suite());
}

