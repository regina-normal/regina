
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
#include "triangulation/facenumbering.h"

using regina::FaceNumbering;
using regina::Perm;

class Perm4Test : public SmallPermTest<4> {
    CPPUNIT_TEST_SUITE(Perm4Test);

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

    // Tests specific to Perm<4>:
    CPPUNIT_TEST(pairs);
    CPPUNIT_TEST(databases);
    CPPUNIT_TEST(aliases);
    CPPUNIT_TEST(S2);
    CPPUNIT_TEST(S3);
    CPPUNIT_TEST(contractFront);

    CPPUNIT_TEST_SUITE_END();

    public:
        void pairs() {
            for (unsigned idx = 0; idx < 24; ++idx) {
                Perm<4> p4 = Perm<4>::S4[idx];
                Perm<6> p6 = p4.pairs();

                for (int i = 0; i < 4; ++i)
                    for (int j = i + 1; j < 4; ++j) {
                        // Look at how p4 maps the pair {i,j}.
                        int e = FaceNumbering<3,1>::edgeNumber[i][j];
                        int f = FaceNumbering<3,1>::edgeNumber[p4[i]][p4[j]];
                        if (p6[e] != f) {
                            CPPUNIT_FAIL("Perm<4>::pairs() does not give "
                                "consistent results.");
                        }
                    }
            }
        }

        void databases() {
            unsigned i;
            for (i = 0; i < 6; ++i) {
                if (Perm<4>::S3[i] != Perm<4>::extend(regina::Perm<3>::S3[i]))
                    CPPUNIT_FAIL("Perm<4> and Perm<3> do not agree on S3.");
            }
            for (i = 0; i < 6; ++i) {
                if (Perm<4>::orderedS3[i] !=
                        Perm<4>::extend(regina::Perm<3>::orderedS3[i]))
                    CPPUNIT_FAIL("Perm<4> and Perm<3> do not agree on orderedS3.");
            }
            for (i = 0; i < 2; ++i) {
                if (Perm<4>::S2[i] != Perm<4>::extend(regina::Perm<3>::S2[i]))
                    CPPUNIT_FAIL("Perm<4> and Perm<3> do not agree on S2.");
            }
        }

        void aliases() {
            unsigned i;

            for (i = 0; i < 24; ++i)
                if (Perm<4>::S4[i] != Perm<4>::Sn[i])
                    CPPUNIT_FAIL("Arrays S4 and Sn disagree for Perm<4>.");

            for (i = 0; i < 6; ++i)
                if (Perm<4>::S3[i] != Perm<4>::Sn_1[i])
                    CPPUNIT_FAIL("Arrays S3 and Sn_1 disagree for Perm<4>.");
        }

        void S2() {
            for (unsigned i = 0; i < 2; ++i) {
                if (! looksEqual(Perm<4>::S2[i],
                        Perm<4>::extend(Perm<2>::S2[i])))
                    CPPUNIT_FAIL("S2 permutations do not match "
                        "Perm<2> extensions.");
                if (Perm<2>::S2[i] != Perm<2>::contract(Perm<4>::S2[i]))
                    CPPUNIT_FAIL("Contracted S2 permutations do not "
                        "match Perm<2>.");
            }
        }

        void S3() {
            for (unsigned i = 0; i < 6; ++i) {
                if (! looksEqual(Perm<4>::S3[i],
                        Perm<4>::extend(Perm<3>::S3[i])))
                    CPPUNIT_FAIL("S3 permutations do not match "
                        "Perm<3> extensions.");
                if (! looksEqual(Perm<4>::Sn_1[i],
                        Perm<4>::extend(Perm<3>::S3[i])))
                    CPPUNIT_FAIL("S3 permutations do not match "
                        "Perm<3> extensions.");
                if (! looksEqual(Perm<4>::orderedS3[i],
                        Perm<4>::extend(Perm<3>::orderedS3[i])))
                    CPPUNIT_FAIL("S3 permutations do not match "
                        "Perm<3> extensions.");
                if (Perm<3>::S3[i] != Perm<3>::contract(Perm<4>::S3[i]))
                    CPPUNIT_FAIL("Contracted S3 permutations do not "
                        "match Perm<3>.");
                if (Perm<3>::S3[i] != Perm<3>::contract(Perm<4>::Sn_1[i]))
                    CPPUNIT_FAIL("Contracted S3 permutations do not "
                        "match Perm<3>.");
                if (Perm<3>::orderedS3[i] !=
                        Perm<3>::contract(Perm<4>::orderedS3[i]))
                    CPPUNIT_FAIL("Contracted S3 permutations do not "
                        "match Perm<3>.");
            }
        }

        void contractFront() {
            if (Perm<4>(1,2,0,3) !=
                   Perm<4>::contractFront(Perm<5>(0,2,3,1,4))) {
                CPPUNIT_FAIL("Perm<4>::contractFront(Perm<5>(0,2,3,1,4)) failed");
            }
            if (Perm<4>(1,2,3,0) !=
                   Perm<4>::contractFront(Perm<5>(0,2,3,4,1))) {
                CPPUNIT_FAIL("Perm<4>::contractFront(Perm<5>(0,2,3,4,1)) failed");
            }
            if (Perm<4>(1,3,0,2) !=
                   Perm<4>::contractFront(Perm<5>(0,2,4,1,3))) {
                CPPUNIT_FAIL("Perm<4>::contractFront(Perm<5>(0,2,4,1,3)) failed");
            }
            if (Perm<4>(1,3,2,0) !=
                   Perm<4>::contractFront(Perm<5>(0,2,4,3,1))) {
                CPPUNIT_FAIL("Perm<4>::contractFront(Perm<5>(0,2,4,3,1)) failed");
            }

            contractFrontChecks(Perm<4>(2,0,1,3), Perm<6>(0,1,4,2,3,5));
            contractFrontChecks(Perm<4>(2,0,3,1), Perm<7>(0,1,2,5,3,6,4));

            if (Perm<4>(0,1) !=
                   Perm<4>::contractFront(Perm<10>(6,7))) {
                CPPUNIT_FAIL("Perm<4>::contractFront(Perm<10>(6,7))");
            }

            if (Perm<4>(1,2) !=
                   Perm<4>::contractFront(Perm<11>(8,9))) {
                CPPUNIT_FAIL("Perm<4>::contractFront(Perm<11>(8,9))");
            }
        }
};

void addPerm4(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Perm4Test::suite());
}

