
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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
                q = regina::perm5to4(NPerm5::extend(p));
                if (! (p == q && p.str() == q.str())) {
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

                q = NPerm5::extend(regina::perm5to4(p));
                if (! (p == q && p.str() == q.str())) {
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
                q = regina::perm4to3(NPerm4::extend(p));
                if (! (p == q && p.str() == q.str())) {
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

                q = NPerm4::extend(regina::perm4to3(p));
                if (! (p == q && p.str() == q.str())) {
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
                s1 = p.str() + "4";
                s2 = NPerm5::extend(p).str();
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

                s1 = p.str();
                s2 = regina::perm5to4(p).str() + "4";
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
                s1 = p.str() + "3";
                s2 = NPerm4::extend(p).str();
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

                s1 = p.str();
                s2 = regina::perm4to3(p).str() + "3";
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

