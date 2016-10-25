
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
#include "maths/perm.h"
#include "testsuite/dim3/testtriangulation.h"

using regina::Perm;

template <int a, int b>
class PermConvTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(PermConvTest);

    CPPUNIT_TEST(identity);
    CPPUNIT_TEST(strings);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void identity() {
            typename Perm<b>::Index i, j;

            {
                Perm<a> p, q;
                for (i = 0; i < Perm<a>::nPerms; ++i) {
                    p = Perm<a>::atIndex(i);
                    q = regina::Perm<a>::contract(Perm<b>::extend(p));
                    if (! (p == q && p.str() == q.str())) {
                        std::ostringstream msg;
                        msg << "Permutation #" << i << " is changed after "
                            "Perm<" << a << "> -> Perm<" << b
                            << "> -> Perm<" << a << "> conversion.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }

            {
                Perm<b> p, q;
                int done = 0;
                bool id;
                for (i = 0; i < Perm<b>::nPerms; ++i) {
                    p = Perm<b>::atIndex(i);

                    id = true;
                    for (j = a; j < b; ++j)
                        if (p[j] != j) {
                            id = false;
                            break;
                        }
                    if (! id)
                        continue;

                    q = Perm<b>::extend(regina::Perm<a>::contract(p));
                    if (! (p == q && p.str() == q.str())) {
                        std::ostringstream msg;
                        msg << "Permutation #" << i << " is changed after "
                            "Perm<" << b << "> -> Perm<" << a
                            << "> -> Perm<" << b << "> conversion.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    ++done;
                }
                // Does it look like we tested everything that maps a,...,b-1
                // to themselves?
                if (done != Perm<a>::nPerms) {
                    std::ostringstream msg;
                    msg << "The number of permutations tested was not "
                        "Perm<" << a << ">::nPerms .";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void strings() {
            typename Perm<b>::Index i, j;
            std::string s1, s2;

            {
                Perm<a> p;
                for (i = 0; i < Perm<a>::nPerms; ++i) {
                    p = Perm<a>::atIndex(i);
                    s1 = p.str();
                    for (j = a; j < b; ++j)
                        s1 += (j < 10 ? ('0' + j) : ('a' + j - 10));
                    s2 = Perm<b>::extend(p).str();
                    if (s1 != s2) {
                        std::ostringstream msg;
                        msg << "Permutation #" << i << " gives the wrong "
                            "string representation after Perm<" << a
                            << "> -> Perm<" << b << "> conversion.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }

            {
                Perm<b> p;
                int done = 0;
                bool id;
                for (i = 0; i < Perm<b>::nPerms; ++i) {
                    p = Perm<b>::atIndex(i);

                    id = true;
                    for (j = a; j < b; ++j)
                        if (p[j] != j) {
                            id = false;
                            break;
                        }
                    if (! id)
                        continue;

                    s1 = p.str();
                    s2 = regina::Perm<a>::contract(p).str();
                    for (j = a; j < b; ++j)
                        s2 += (j < 10 ? ('0' + j) : ('a' + j - 10));
                    if (s1 != s2) {
                        std::ostringstream msg;
                        msg << "Permutation #" << i << " gives the wrong "
                            "string representation after Perm<" << b
                            << "> -> Perm<" << a << "> conversion.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    ++done;
                }
                // Does it look like we tested everything that maps a,...,b-1
                // to themselves?
                if (done != Perm<a>::nPerms) {
                    std::ostringstream msg;
                    msg << "The number of permutations tested was not "
                        "Perm<" << a << ">::nPerms .";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }
};

void addPermConv(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(PermConvTest<2, 3>::suite());
    runner.addTest(PermConvTest<2, 4>::suite());
    runner.addTest(PermConvTest<2, 5>::suite());
    runner.addTest(PermConvTest<2, 6>::suite());
    runner.addTest(PermConvTest<2, 7>::suite());
    runner.addTest(PermConvTest<3, 4>::suite());
    runner.addTest(PermConvTest<3, 5>::suite());
    runner.addTest(PermConvTest<3, 6>::suite());
    runner.addTest(PermConvTest<3, 7>::suite());
    runner.addTest(PermConvTest<4, 5>::suite());
    runner.addTest(PermConvTest<4, 6>::suite());
    runner.addTest(PermConvTest<4, 7>::suite());
    runner.addTest(PermConvTest<5, 6>::suite());
    runner.addTest(PermConvTest<5, 7>::suite());
    runner.addTest(PermConvTest<6, 7>::suite());
}

