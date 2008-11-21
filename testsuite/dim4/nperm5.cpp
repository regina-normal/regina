
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
#include "dim4/nperm5.h"
#include "testsuite/dim4/testdim4.h"

using regina::NPerm5;

class NPerm5Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NPerm5Test);

    CPPUNIT_TEST(exhaustive);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void testPerm(int a, int b, int c, int d, int e) {
            NPerm5 p(a, b, c, d, e);

            // TODO: A *lot* more tests.  Coming...
        }

        void exhaustive() {
            NPerm5 id;
            CPPUNIT_ASSERT_MESSAGE("The default NPerm5 constructor does not "
                "give the identity permutation.", id.isIdentity());

            // Test all possible permutations.
            int tested = 0;
            int a, b, c, d, e;
            for (a = 0; a < 5; ++a) {
                for (b = 0; b < 5; ++b) {
                    if (b == a)
                        continue;
                    for (c = 0; c < 5; ++c) {
                        if (c == a || c == b)
                            continue;
                        for (d = 0; d < 5; ++d) {
                            if (d == a || d == b || d == c)
                                continue;
                            e = (10 - a - b - c - d);
                            testPerm(a, b, c, d, e);
                            ++tested;
                        }
                    }
                }
            }

            CPPUNIT_ASSERT_MESSAGE("All 120 permutations in S(5) were "
                "not tested.", tested == 120);
        }
};

void addNPerm5(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NPerm5Test::suite());
}

