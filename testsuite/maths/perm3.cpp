
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

class Perm3Test : public SmallPermTest<3> {
    CPPUNIT_TEST_SUITE(Perm3Test);

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

    // Tests specific to Perm<3>:
    CPPUNIT_TEST(productsViaPerm4);
    CPPUNIT_TEST(aliases);
    CPPUNIT_TEST(S2);
    CPPUNIT_TEST(contractFront);

    CPPUNIT_TEST_SUITE_END();

    public:
        void productsViaPerm4() {
            // An indirect test, using Perm<4> to verify.
            int a, b, c, d, e, f;
            for (a = 0; a < 3; ++a)
                for (b = 0; b < 3; ++b) {
                    if (b == a)
                        continue;
                    c = 3 - a - b;
                    Perm<3> x(a, b, c);

                    for (d = 0; d < 3; ++d)
                        for (e = 0; e < 3; ++e) {
                            if (e == d)
                                continue;
                            f = 3 - d - e;
                            Perm<3> y(d, e, f);

                            Perm<3> product3 = x * y;
                            regina::Perm<4> product4 =
                                regina::Perm<4>(a, b, c, 3) *
                                regina::Perm<4>(d, e, f, 3);

                            if (product3[0] != product4[0] ||
                                    product3[1] != product4[1] ||
                                    product3[2] != product4[2]) {
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

            for (i = 0; i < 6; ++i)
                if (Perm<3>::S3[i] != Perm<3>::Sn[i])
                    CPPUNIT_FAIL("Arrays S3 and Sn disagree for Perm<3>.");

            for (i = 0; i < 2; ++i)
                if (Perm<3>::S2[i] != Perm<3>::Sn_1[i])
                    CPPUNIT_FAIL("Arrays S2 and Sn_1 disagree for Perm<3>.");
        }

        void S2() {
            for (unsigned i = 0; i < 2; ++i) {
                if (! looksEqual(Perm<3>::S2[i],
                        Perm<3>::extend(Perm<2>::S2[i])))
                    CPPUNIT_FAIL("S2 permutations do not match "
                        "Perm<2> extensions.");
                if (! looksEqual(Perm<3>::Sn_1[i],
                        Perm<3>::extend(Perm<2>::S2[i])))
                    CPPUNIT_FAIL("S2 permutations do not match "
                        "Perm<2> extensions.");
                if (Perm<2>::S2[i] != Perm<2>::contract(Perm<3>::S2[i]))
                    CPPUNIT_FAIL("Contracted S2 permutations do not "
                        "match Perm<2>.");
                if (Perm<2>::S2[i] != Perm<2>::contract(Perm<3>::Sn_1[i]))
                    CPPUNIT_FAIL("Contracted S2 permutations do not "
                        "match Perm<2>.");
            }
        }
    
        void contractFront() {
            if (Perm<3>(0,1,2) !=
                   Perm<3>::contractFront(Perm<4>(0,1,2,3))) {
                CPPUNIT_FAIL("Perm<3>::contractFront(Perm<4>(0,1,2,3)) failed.");
            }
            if (Perm<3>(0,2,1) !=
                   Perm<3>::contractFront(Perm<4>(0,1,3,2))) {
                CPPUNIT_FAIL("Perm<3>::contractFront(Perm<4>(0,1,3,2)) failed.");
            }
            if (Perm<3>(1,0,2) !=
                   Perm<3>::contractFront(Perm<4>(0,2,1,3))) {
                CPPUNIT_FAIL("Perm<3>::contractFront(Perm<4>(0,2,1,3)) failed.");
            }
            if (Perm<3>(1,2,0) !=
                   Perm<3>::contractFront(Perm<4>(0,2,3,1))) {
                CPPUNIT_FAIL("Perm<3>::contractFront(Perm<4>(0,2,3,1)) failed.");
            }
            if (Perm<3>(2,0,1) !=
                   Perm<3>::contractFront(Perm<4>(0,3,1,2))) {
                CPPUNIT_FAIL("Perm<3>::contractFront(Perm<4>(0,3,1,2)) failed.");
            }
            if (Perm<3>(2,1,0) !=
                   Perm<3>::contractFront(Perm<4>(0,3,2,1))) {
                CPPUNIT_FAIL("Perm<3>::contractFront(Perm<4>(0,3,2,1)) failed.");
            }

            contractFrontChecks(Perm<3>(0,1,2), Perm<5>(0,1,2,3,4));
            contractFrontChecks(Perm<3>(0,2,1), Perm<5>(0,1,2,4,3));
            contractFrontChecks(Perm<3>(1,0,2), Perm<5>(0,1,3,2,4));
            contractFrontChecks(Perm<3>(1,2,0), Perm<5>(0,1,3,4,2));
            contractFrontChecks(Perm<3>(2,0,1), Perm<5>(0,1,4,2,3));
            contractFrontChecks(Perm<3>(2,1,0), Perm<5>(0,1,4,3,2));

            contractFrontChecks(Perm<3>(0,1,2), Perm<6>(0,1,2,3,4,5));
            contractFrontChecks(Perm<3>(0,2,1), Perm<6>(0,1,2,3,5,4));
            contractFrontChecks(Perm<3>(1,0,2), Perm<6>(0,1,2,4,3,5));
            contractFrontChecks(Perm<3>(1,2,0), Perm<6>(0,1,2,4,5,3));
            contractFrontChecks(Perm<3>(2,0,1), Perm<6>(0,1,2,5,3,4));
            contractFrontChecks(Perm<3>(2,1,0), Perm<6>(0,1,2,5,4,3));

            if (Perm<3>(1,2,0) !=
                   Perm<3>::contractFront(Perm<10>(3,4) * Perm<10>(7,8) * Perm<10>(8,9))) {
                CPPUNIT_FAIL("Perm<3>::contractFront(Perm<10>(...)) failed.");
            }
        }   
};

void addPerm3(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Perm3Test::suite());
}

