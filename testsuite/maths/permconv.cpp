
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "maths/permconv.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NPerm3;
using regina::NPerm4;
using regina::NPerm5;

class PermConvTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(PermConvTest);

    CPPUNIT_TEST(identity4to5to4);
    CPPUNIT_TEST(identity5to4to5);
    CPPUNIT_TEST(identity3to4to3);
    CPPUNIT_TEST(identity4to3to4);
    CPPUNIT_TEST(strings4to5);
    CPPUNIT_TEST(strings5to4);
    CPPUNIT_TEST(strings3to4);
    CPPUNIT_TEST(strings4to3);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void identity4to5to4() {
            NPerm4 p, q;
            for (int i = 0; i < 24; ++i) {
                p = NPerm4::S4[i];
                q = regina::perm5to4(regina::perm4to5(p));
                if (! (p == q && p.toString() == q.toString())) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " is changed after "
                        "NPerm4 -> NPerm5 -> NPerm4 conversion.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void identity5to4to5() {
            NPerm5 p, q;
            int done = 0;
            for (int i = 0; i < 120; ++i) {
                p = NPerm5::S5[i];
                if (p[4] != 4)
                    continue;

                q = regina::perm4to5(regina::perm5to4(p));
                if (! (p == q && p.toString() == q.toString())) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " is changed after "
                        "NPerm5 -> NPerm4 -> NPerm5 conversion.";
                    CPPUNIT_FAIL(msg.str());
                }
                ++done;
            }
            // Does it look like we tested everything that maps 4 to itself?
            if (done != 24)
                CPPUNIT_FAIL("The number of permutations tested was not 24.");
        }

        void identity3to4to3() {
            NPerm3 p, q;
            for (int i = 0; i < 6; ++i) {
                p = NPerm3::S3[i];
                q = regina::perm4to3(regina::perm3to4(p));
                if (! (p == q && p.toString() == q.toString())) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " is changed after "
                        "NPerm3 -> NPerm4 -> NPerm3 conversion.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void identity4to3to4() {
            NPerm4 p, q;
            int done = 0;
            for (int i = 0; i < 24; ++i) {
                p = NPerm4::S4[i];
                if (p[3] != 3)
                    continue;

                q = regina::perm3to4(regina::perm4to3(p));
                if (! (p == q && p.toString() == q.toString())) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " is changed after "
                        "NPerm4 -> NPerm3 -> NPerm4 conversion.";
                    CPPUNIT_FAIL(msg.str());
                }
                ++done;
            }
            // Does it look like we tested everything that maps 3 to itself?
            if (done != 6)
                CPPUNIT_FAIL("The number of permutations tested was not 6.");
        }

        void strings4to5() {
            NPerm4 p;
            std::string s1, s2;
            for (int i = 0; i < 24; ++i) {
                p = NPerm4::S4[i];
                s1 = p.toString() + "4";
                s2 = regina::perm4to5(p).toString();
                if (s1 != s2) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " gives the wrong "
                        "string representation after NPerm4 -> NPerm5 "
                        "conversion.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void strings5to4() {
            NPerm5 p;
            std::string s1, s2;
            int done = 0;
            for (int i = 0; i < 120; ++i) {
                p = NPerm5::S5[i];
                if (p[4] != 4)
                    continue;

                s1 = p.toString();
                s2 = regina::perm5to4(p).toString() + "4";
                if (s1 != s2) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " gives the wrong "
                        "string representation after NPerm5 -> NPerm4 "
                        "conversion.";
                    CPPUNIT_FAIL(msg.str());
                }
                ++done;
            }
            // Does it look like we tested everything that maps 4 to itself?
            if (done != 24)
                CPPUNIT_FAIL("The number of permutations tested was not 24.");
        }

        void strings3to4() {
            NPerm3 p;
            std::string s1, s2;
            for (int i = 0; i < 6; ++i) {
                p = NPerm3::S3[i];
                s1 = p.toString() + "3";
                s2 = regina::perm3to4(p).toString();
                if (s1 != s2) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " gives the wrong "
                        "string representation after NPerm3 -> NPerm4 "
                        "conversion.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void strings4to3() {
            NPerm4 p;
            std::string s1, s2;
            int done = 0;
            for (int i = 0; i < 24; ++i) {
                p = NPerm4::S4[i];
                if (p[3] != 3)
                    continue;

                s1 = p.toString();
                s2 = regina::perm4to3(p).toString() + "3";
                if (s1 != s2) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " gives the wrong "
                        "string representation after NPerm4 -> NPerm3 "
                        "conversion.";
                    CPPUNIT_FAIL(msg.str());
                }
                ++done;
            }
            // Does it look like we tested everything that maps 3 to itself?
            if (done != 6)
                CPPUNIT_FAIL("The number of permutations tested was not 6.");
        }
};

void addPermConv(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(PermConvTest::suite());
}

