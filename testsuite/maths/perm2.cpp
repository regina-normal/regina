
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

#include "testsuite/maths/permtest.h"
#include "testsuite/maths/testmaths.h"

using regina::Perm;

class Perm2Test : public SmallPermTest<2> {
    CPPUNIT_TEST_SUITE(Perm2Test);

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
    CPPUNIT_TEST(rot);
    CPPUNIT_TEST(conjugacyMinimal);
    CPPUNIT_TEST(increment);
    CPPUNIT_TEST(tightEncoding);

    // Tests specific to Perm<2>:
    CPPUNIT_TEST(productsViaPerm4);
    CPPUNIT_TEST(aliases);
    CPPUNIT_TEST(contractFront);

    CPPUNIT_TEST_SUITE_END();

    public:
        void productsViaPerm4() {
            // An indirect test (using Perm<4> to verify).
            int a, d;
            for (a = 0; a < 2; ++a) {
                Perm<2> x = Perm<2>::S2[a]; // 0 -> a, 1 -> 1-a.

                for (d = 0; d < 2; ++d) {
                    Perm<2> y = Perm<2>::S2[d]; // 0 -> d, 1 -> 1-d.

                    Perm<2> product2 = x * y;
                    regina::Perm<4> product4 =
                        regina::Perm<4>(a, 1 - a, 2, 3) *
                        regina::Perm<4>(d, 1 - d, 2, 3);

                    if (product2[0] != product4[0] ||
                            product2[1] != product4[1]) {
                        std::ostringstream msg;
                        msg << "The product is incorrect for "
                            << x.str() << " * " << y.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void aliases() {
            unsigned i;

            for (i = 0; i < 2; ++i)
                if (Perm<2>::S2[i] != Perm<2>::Sn[i])
                    CPPUNIT_FAIL("Arrays S2 and Sn disagree for Perm<2>.");

            for (i = 0; i < 1; ++i)
                if (Perm<2>::S1[i] != Perm<2>::Sn_1[i])
                    CPPUNIT_FAIL("Arrays S1 and Sn_1 disagree for Perm<2>.");
        }

        void contractFront() {
            if (Perm<2>() !=
                   Perm<2>::contractFront(Perm<3>(0,1,2))) {
                CPPUNIT_FAIL("Perm<2>::contractFront(Perm<3>(0,1,2)) failed.");
            }
            if (Perm<2>(1,0) !=
                   Perm<2>::contractFront(Perm<3>(0,2,1))) {
                CPPUNIT_FAIL("Perm<2>::contractFront(Perm<3>(0,2,1)) failed.");
            }

            contractFrontChecks(Perm<2>(), Perm<4>());
            contractFrontChecks(Perm<2>(1,0), Perm<4>(0,1,3,2));

            contractFrontChecks(Perm<2>(), Perm<5>());
            contractFrontChecks(Perm<2>(1,0), Perm<5>(0,1,2,4,3));

            contractFrontChecks(Perm<2>(), Perm<6>());
            contractFrontChecks(Perm<2>(1,0), Perm<6>(0,1,2,3,5,4));

            contractFrontChecks(Perm<2>(), Perm<7>());
            contractFrontChecks(Perm<2>(1,0), Perm<7>(0,1,2,3,4,6,5));

            if (Perm<2>() !=
                    Perm<2>::contractFront(Perm<8>())) {
                CPPUNIT_FAIL("Perm<2>::contractFront(Perm<8>()) failed.");
            }
            if (Perm<2>() !=
                    Perm<2>::contractFront(Perm<8>(4,5))) {
                CPPUNIT_FAIL("Perm<2>::contractFront(Perm<8>(4,5)) failed.");
            }
            if (Perm<2>(1,0) !=
                    Perm<2>::contractFront(Perm<8>(6,7))) {
                CPPUNIT_FAIL("Perm<2>::contractFront(Perm<8>(6,7)) failed.");
            }
            if (Perm<2>(1,0) !=
                    Perm<2>::contractFront(Perm<8>(6,7)*Perm<8>(4,5))) {
                CPPUNIT_FAIL("Perm<2>::contractFront(Perm<8>(6,7)*Perm<8>(4,5)) failed.");
            }

            if (Perm<2>() !=
                    Perm<2>::contractFront(Perm<12>())) {
                CPPUNIT_FAIL("Perm<2>::contractFront(Perm<12>()) failed.");
            }
            if (Perm<2>() !=
                    Perm<2>::contractFront(Perm<12>(7,8))) {
                CPPUNIT_FAIL("Perm<2>::contractFront(Perm<12>(7,8)) failed.");
            }
            if (Perm<2>(1,0) !=
                    Perm<2>::contractFront(Perm<12>(10,11))) {
                CPPUNIT_FAIL("Perm<2>::contractFront(Perm<12>(10,11)) failed.");
            }
            if (Perm<2>(1,0) !=
                Perm<2>::contractFront(Perm<12>(10,11)*Perm<12>(7,8))) {
                CPPUNIT_FAIL("Perm<2>::contractFront(Perm<12>(10,11)*Perm<12>(7,8)) failed.");
            }

        }
};

void addPerm2(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Perm2Test::suite());
}

