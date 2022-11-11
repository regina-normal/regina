
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include "testsuite/maths/permtest.h"
#include "testsuite/maths/testmaths.h"

using regina::Perm;

class Perm7Test : public SmallPermTest<7> {
    CPPUNIT_TEST_SUITE(Perm7Test);

    // Generic inherited tests:
    CPPUNIT_TEST(permCode);
    CPPUNIT_TEST(sign);
    CPPUNIT_TEST(index);
    CPPUNIT_TEST(exhaustive);
    CPPUNIT_TEST(swaps);
    CPPUNIT_TEST(cachedInverse);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(cachedProducts);
    CPPUNIT_TEST(conjugates);
    CPPUNIT_TEST(cachedConjugates);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(clear);
    CPPUNIT_TEST(order);
    CPPUNIT_TEST(pow);
    CPPUNIT_TEST(cachedPow);
    CPPUNIT_TEST(rot);
    CPPUNIT_TEST(conjugacyMinimal);
    CPPUNIT_TEST(increment);
    CPPUNIT_TEST(tightEncoding);

    // Tests specific to Perm<7>:
    CPPUNIT_TEST(aliases);

    CPPUNIT_TEST_SUITE_END();

    public:
        void aliases() {
            for (Perm<7>::Index i = 0; i < Perm<7>::nPerms; ++i)
                if (Perm<7>::S7[i] != Perm<7>::Sn[i])
                    CPPUNIT_FAIL("Arrays S7 and Sn disagree for Perm<7>.");
        }
};

void addPerm7(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Perm7Test::suite());
}

