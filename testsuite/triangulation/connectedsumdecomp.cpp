
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cmath>
#include <memory>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "algebra/nabeliangroup.h"
#include "manifold/nmanifold.h"
#include "packet/ncontainer.h"
#include "split/nsignature.h"
#include "subcomplex/nstandardtri.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NAbelianGroup;
using regina::NContainer;
using regina::NManifold;
using regina::NPerm;
using regina::NSignature;
using regina::NStandardTriangulation;
using regina::NTetrahedron;
using regina::NTriangulation;

class ConnectedSumDecompTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ConnectedSumDecompTest);

    CPPUNIT_TEST(threeSpheres);
    CPPUNIT_TEST(specialCases);
    CPPUNIT_TEST(primes);
    CPPUNIT_TEST(nonTrivialSums);

    CPPUNIT_TEST_SUITE_END();

    public:
        NTriangulation* generateFromSig(const std::string& sigStr) {
            NSignature* sig = NSignature::parse(sigStr);
            if (sig == 0)
                return 0;

            NTriangulation* triNew = sig->triangulate();
            delete sig;
            return triNew;
        }

        void setUp() {
        }

        void tearDown() {
        }

        NTriangulation* verifyThreeSphere(NTriangulation* tri,
                const std::string& triName) {
            NContainer summands;
            unsigned long ans = tri->connectedSumDecomposition(&summands);

            CPPUNIT_ASSERT_MESSAGE("The 3-sphere " + triName +
                " is reported to have prime summands.",
                ans == 0 && summands.getNumberOfChildren() == 0);

            return tri;
        }

        void verifySigThreeSphere(const std::string& sigStr) {
            delete verifyThreeSphere(generateFromSig(sigStr), sigStr);
        }

        NTriangulation* verifyPrime(NTriangulation* tri,
                const std::string& triName, const std::string& manifold) {
            // Recall that ASSERTS throw exceptions, so after testing
            // them we can assume their conditions to be true.
            NContainer summands;
            unsigned long ans = tri->connectedSumDecomposition(&summands);

            CPPUNIT_ASSERT_MESSAGE("The prime 3-manifold " + triName +
                " is reported to be a 3-sphere.",
                ans > 0 && summands.getFirstTreeChild() != 0);
            CPPUNIT_ASSERT_MESSAGE("The prime 3-manifold " + triName +
                " is reported to be composite.", ans == 1 &&
                summands.getFirstTreeChild() == summands.getLastTreeChild());

            NTriangulation* summand = static_cast<NTriangulation*>(
                summands.getFirstTreeChild());

            std::auto_ptr<NStandardTriangulation> stdTri(
                NStandardTriangulation::isStandardTriangulation(summand));
            CPPUNIT_ASSERT_MESSAGE("The single prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri.get() != 0);

            std::auto_ptr<NManifold> stdManifold(stdTri->getManifold());
            CPPUNIT_ASSERT_MESSAGE("The single prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold.get() != 0);

            std::string stdName = stdManifold->getName();
            CPPUNIT_ASSERT_MESSAGE("The single prime summand of " + triName +
                " forms " + stdName + ", not " + manifold + ".",
                stdName == manifold);

            if (manifold != "RP3" && manifold != "S2 x S1" &&
                    manifold != "L(3,1)")
                CPPUNIT_ASSERT_MESSAGE("The single prime summand of " +
                    triName + " is not 0-efficient.",
                    summand->isZeroEfficient());

            CPPUNIT_ASSERT_MESSAGE("The single prime summand of " + triName +
                " has an inconsistent first homology group.",
                summand->getHomologyH1() == tri->getHomologyH1());

            return tri;
        }

        void verifySigPrime(const std::string& sigStr,
                const std::string& manifold) {
            delete verifyPrime(generateFromSig(sigStr), sigStr, manifold);
        }

        /**
         * NOTE: The two manifold names must be given in lexicographical order.
         */
        NTriangulation* verifyPair(NTriangulation* tri,
                const std::string& triName, const std::string& manifold1,
                const std::string& manifold2) {
            // Recall that ASSERTS throw exceptions, so after testing
            // them we can assume their conditions to be true.
            NContainer summands;
            unsigned long ans = tri->connectedSumDecomposition(&summands);

            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to be a 3-sphere.",
                ans > 0 && summands.getFirstTreeChild() != 0);
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to be prime.", ans > 1 &&
                summands.getFirstTreeChild() != summands.getLastTreeChild());
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to have more than two summands.", ans == 2 &&
                summands.getFirstTreeChild()->getNextTreeSibling() ==
                summands.getLastTreeChild());

            NTriangulation* summand1 = static_cast<NTriangulation*>(
                summands.getFirstTreeChild());
            NTriangulation* summand2 = static_cast<NTriangulation*>(
                summands.getLastTreeChild());

            std::auto_ptr<NStandardTriangulation> stdTri1(
                NStandardTriangulation::isStandardTriangulation(summand1));
            CPPUNIT_ASSERT_MESSAGE("The first prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri1.get() != 0);

            std::auto_ptr<NStandardTriangulation> stdTri2(
                NStandardTriangulation::isStandardTriangulation(summand2));
            CPPUNIT_ASSERT_MESSAGE("The second prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri2.get() != 0);

            std::auto_ptr<NManifold> stdManifold1(stdTri1->getManifold());
            CPPUNIT_ASSERT_MESSAGE("The first prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold1.get() != 0);

            std::auto_ptr<NManifold> stdManifold2(stdTri2->getManifold());
            CPPUNIT_ASSERT_MESSAGE("The second prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold2.get() != 0);

            // Obtain the manifold names in lexicographical order.
            std::string stdName1 = stdManifold1->getName();
            std::string stdName2 = stdManifold2->getName();
            if (stdName2 < stdName1) {
                std::string tmp = stdName2;
                stdName2 = stdName1;
                stdName1 = tmp;

                // Swap the summands also so we can correctly analyse
                // them later.
                NTriangulation* tmpTri = summand2;
                summand2 = summand1;
                summand1 = tmpTri;
            }

            CPPUNIT_ASSERT_MESSAGE("The first prime summand of " + triName +
                " forms " + stdName1 + ", not " + manifold1 + ".",
                stdName1 == manifold1);
            CPPUNIT_ASSERT_MESSAGE("The second prime summand of " + triName +
                " forms " + stdName2 + ", not " + manifold2 + ".",
                stdName2 == manifold2);

            // Test that the homologies are consistent.
            NAbelianGroup combined(summand1->getHomologyH1());
            combined.addGroup(summand2->getHomologyH1());
            CPPUNIT_ASSERT_MESSAGE("The prime summands of " + triName +
                " have inconsistent first homology groups.",
                tri->getHomologyH1() == combined);


            // Finish with a 0-efficiency test.
            if (manifold1 != "RP3" && manifold1 != "S2 x S1" &&
                    manifold1 != "L(3,1)")
                CPPUNIT_ASSERT_MESSAGE("The first prime summand of " +
                    triName + " is not 0-efficient.",
                    summand1->isZeroEfficient());
            if (manifold2 != "RP3" && manifold2 != "S2 x S1" &&
                    manifold2 != "L(3,1)")
                CPPUNIT_ASSERT_MESSAGE("The second prime summand of " +
                    triName + " is not 0-efficient.",
                    summand2->isZeroEfficient());

            // All above board.
            return tri;
        }

        void verifySigPair(const std::string& sigStr,
                const std::string& manifold1, const std::string& manifold2) {
            delete verifyPair(generateFromSig(sigStr), sigStr, manifold1,
                manifold2);
        }

        NTriangulation* verifyRP3x3(NTriangulation* tri,
                const std::string& triName) {
            // Recall that ASSERTS throw exceptions, so after testing
            // them we can assume their conditions to be true.
            NContainer summands;
            unsigned long ans = tri->connectedSumDecomposition(&summands);

            NTriangulation* summand1 = static_cast<NTriangulation*>(
                summands.getFirstTreeChild());
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to be a 3-sphere.", ans > 0 && summand1 != 0);
            NTriangulation* summand2 = static_cast<NTriangulation*>(
                summand1->getNextTreeSibling());
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to be prime.", ans > 1 && summand2 != 0);
            NTriangulation* summand3 = static_cast<NTriangulation*>(
                summand2->getNextTreeSibling());
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to have only two prime summands.",
                ans > 2 && summand3 != 0);
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to have more than three summands.", ans == 3 &&
                summand3 == summands.getLastTreeChild());

            std::auto_ptr<NStandardTriangulation> stdTri1(
                NStandardTriangulation::isStandardTriangulation(summand1));
            CPPUNIT_ASSERT_MESSAGE("The first prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri1.get() != 0);

            std::auto_ptr<NStandardTriangulation> stdTri2(
                NStandardTriangulation::isStandardTriangulation(summand2));
            CPPUNIT_ASSERT_MESSAGE("The second prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri2.get() != 0);

            std::auto_ptr<NStandardTriangulation> stdTri3(
                NStandardTriangulation::isStandardTriangulation(summand3));
            CPPUNIT_ASSERT_MESSAGE("The third prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri3.get() != 0);

            std::auto_ptr<NManifold> stdManifold1(stdTri1->getManifold());
            CPPUNIT_ASSERT_MESSAGE("The first prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold1.get() != 0);

            std::auto_ptr<NManifold> stdManifold2(stdTri2->getManifold());
            CPPUNIT_ASSERT_MESSAGE("The second prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold2.get() != 0);

            std::auto_ptr<NManifold> stdManifold3(stdTri3->getManifold());
            CPPUNIT_ASSERT_MESSAGE("The third prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold3.get() != 0);

            // Obtain the manifold names in lexicographical order.
            std::string stdName1 = stdManifold1->getName();
            std::string stdName2 = stdManifold2->getName();
            std::string stdName3 = stdManifold3->getName();

            CPPUNIT_ASSERT_MESSAGE("One of the three prime summands of " +
                triName + " does not form RP3.",
                stdName1 == "RP3" && stdName2 == "RP3" && stdName3 == "RP3");

            // Test that the homologies are consistent.
            NAbelianGroup combined(summand1->getHomologyH1());
            combined.addGroup(summand2->getHomologyH1());
            combined.addGroup(summand3->getHomologyH1());
            CPPUNIT_ASSERT_MESSAGE("The prime summands of " + triName +
                " have inconsistent first homology groups.",
                tri->getHomologyH1() == combined);

            // All above board.
            return tri;
        }

        void verifySigRP3x3(const std::string& sigStr) {
            delete verifyRP3x3(generateFromSig(sigStr), sigStr);
        }

        void threeSpheres() {
            // 3-spheres obtained from splitting surface signatures:
            verifySigThreeSphere("(a)(a)");
            verifySigThreeSphere("(ab)(a)(b)");
            verifySigThreeSphere("(ab)(a)(b)");
            verifySigThreeSphere("(abc)(a)(b)(c)");
            verifySigThreeSphere("(ab)(ac)(b)(c)");
            verifySigThreeSphere("(abcd)(a)(b)(c)(d)");
            verifySigThreeSphere("(abc)(abd)(c)(d)");
            verifySigThreeSphere("(abc)(acd)(b)(d)");
            verifySigThreeSphere("(abc)(ad)(b)(c)(d)");
            verifySigThreeSphere("(ab)(ac)(bd)(cd)");
            verifySigThreeSphere("(ab)(ac)(bd)(c)(d)");
            verifySigThreeSphere("(abcd)(aefg)(b)(c)(d)(e)(f)(g)");

            // 3-spheres obtained as Lens spaces:
            NTriangulation* tri;

            tri = new NTriangulation();
            tri->insertLayeredLensSpace(1,0);
            delete verifyThreeSphere(tri, "L(1,0)");
        }

        void specialCases() {
            // Triangulations obtained from splitting surface signatures:
            verifySigPrime("(aab)(b)", "RP3");
            verifySigPrime("(ab)(ab)", "RP3");
            verifySigPrime("(aabcb)(c)", "RP3");
            verifySigPrime("(aabc)(b)(c)", "RP3");
            verifySigPrime("(aabcdcb)(d)", "RP3");
            verifySigPrime("(aabcdb)(c)(d)", "RP3");
            verifySigPrime("(aabcd)(b)(c)(d)", "RP3");
            verifySigPrime("(aabc)(bd)(c)(d)", "RP3");
            verifySigPrime("(abac)(bd)(cd)", "RP3");
            verifySigPrime("(abac)(bd)(c)(d)", "RP3");
            verifySigPrime("(abcd)(ac)(bd)", "RP3");
            verifySigPrime("(aab)(bc)(cd)(d)", "RP3");
            verifySigPrime("(abc)(ab)(cd)(d)", "RP3");
            verifySigPrime("(abc)(ad)(bd)(c)", "RP3");
            verifySigPrime("(abac)(b)(c)", "S2 x S1");
            verifySigPrime("(abacdc)(b)(d)", "S2 x S1");
            verifySigPrime("(abcabd)(c)(d)", "S2 x S1");
            verifySigPrime("(abacd)(b)(c)(d)", "S2 x S1");
            verifySigPrime("(aabc)(bd)(cd)", "S2 x S1");
            verifySigPrime("(abacde)(cf)(fg)(b)(d)(e)(g)", "S2 x S1");
            verifySigPrime("(abc)(abc)", "L(3,1)");
            verifySigPrime("(abc)(acb)", "L(3,1)");
        }

        void primes() {
            NTriangulation* tri;

            // Triangulations obtained from splitting surface signatures:
            verifySigPrime("(aa)", "L(4,1)");
            verifySigPrime("(aabb)", "L(8,3)");
            verifySigPrime("(abab)", "S3/Q8");
            verifySigPrime("(aabccb)", "L(12,5)");
            verifySigPrime("(abcabc)", "S3/Q12");
            verifySigPrime("(aab)(bcc)", "L(6,1)");
            verifySigPrime("(aab)(bc)(c)", "L(4,1)");
            verifySigPrime("(ab)(ac)(bc)", "L(4,1)");
            verifySigPrime("(aabcddcb)", "L(16,7)");
            verifySigPrime("(abcdabcd)", "S3/Q16");
            verifySigPrime("(aabbc)(cd)(d)", "L(8,3)");
            verifySigPrime("(aabcb)(cdd)", "L(14,3)");
            verifySigPrime("(aabcb)(cd)(d)", "L(8,3)");
            verifySigPrime("(ababc)(cd)(d)", "S3/Q8");
            verifySigPrime("(abac)(bdcd)", "L(4,1)");
            verifySigPrime("(abac)(bcd)(d)", "L(4,1)");
            verifySigPrime("(abac)(bdd)(c)", "L(4,1)");
            verifySigPrime("(abcd)(abcd)", "L(4,1)");
            verifySigPrime("(abcd)(adcb)", "L(4,1)");
            verifySigPrime("(aab)(bcd)(c)(d)", "L(4,1)");
            verifySigPrime("(abc)(abd)(cd)", "L(8,3)");
            verifySigPrime("(abc)(acd)(bd)", "S3/Q8");
            verifySigPrime("(abcdefgh)(abcdefgh)", "L(8,1)");

            // And of course the Poincare homology sphere(S3/P120).
            // We'll build this a few different ways.

            // Poincare homology sphere as a plugged triangular solid torus:
            tri = new NTriangulation;
            NTetrahedron* tet[5];
            int i;
            for (i = 0; i < 5; i++)
                tet[i] = new NTetrahedron;
            tet[0]->joinTo(0, tet[4], NPerm(1,0,2,3));
            tet[0]->joinTo(1, tet[3], NPerm(0,2,3,1));
            tet[0]->joinTo(2, tet[1], NPerm(0,1,3,2));
            tet[0]->joinTo(3, tet[2], NPerm(2,1,3,0));
            tet[1]->joinTo(0, tet[3], NPerm(1,3,2,0));
            tet[1]->joinTo(1, tet[2], NPerm(0,2,3,1));
            tet[1]->joinTo(2, tet[4], NPerm(2,1,0,3));
            tet[2]->joinTo(1, tet[4], NPerm(0,2,3,1));
            tet[2]->joinTo(3, tet[3], NPerm(3,1,2,0));
            tet[3]->joinTo(3, tet[4], NPerm(0,1,2,3));
            for (i = 0; i < 5; i++)
                tri->addTetrahedron(tet[i]);
            delete verifyPrime(tri, "the Poincare homology sphere (plugged)",
                "S3/P120");

            // Poincare homology sphere as an augmented triangular solid
            // torus:
            tri = new NTriangulation();
            tri->insertAugTriSolidTorus(2, -1, 3, 1, 5, -4);
            delete verifyPrime(tri, "the Poincare homology sphere (aug I)",
                "S3/P120");

            // Poincare homology sphere as another augmented triangular solid
            // torus:
            tri = new NTriangulation();
            tri->insertAugTriSolidTorus(2, -1, 3, -2, 5, 1);
            delete verifyPrime(tri, "the Poincare homology sphere (aug I)",
                "S3/P120");
        }

        void nonTrivialSums() {
            verifySigPair("(aabccd)(b)(d)", "RP3", "RP3");
            verifySigPair("(abacbd)(cd)", "RP3", "RP3");
            verifySigPair("(aabcdecb)(dfeg)(fg)", "L(12,5)", "RP3");
            verifySigPair("(aabbc)(cdef)(egg)(d)(f)", "L(4,1)", "L(8,3)");
            verifySigPair("(ababc)(cdef)(egg)(d)(f)", "L(4,1)", "S3/Q8");
            verifySigPair("(abcd)(acbe)(dfeg)(f)(g)", "L(3,1)", "S2 x S1");
            verifySigPair("(abcd)(adce)(befg)(f)(g)", "L(3,1)", "RP3");
            verifySigPair("(abcde)(aff)(cgg)(b)(d)(e)", "L(4,1)", "L(4,1)");
            verifySigPair("(abcde)(adf)(bfg)(ce)(g)", "RP3", "S2 x S1");
            verifySigPair("(ababc)(cde)(dfg)(fg)(e)", "RP3", "S3/Q8");
            verifySigPair("(abcdef)(acegg)(bdf)", "L(3,1)", "L(4,1)");
            verifySigPair("(abacde)(dffgg)(b)(c)(e)", "L(8,3)", "S2 x S1");
            verifySigRP3x3("(aabccdeffeg)(b)(d)(g)");
            verifySigRP3x3("(aabcde)(cfg)(dgf)(b)(e)");
        }
};

void addConnectedSumDecomp(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(ConnectedSumDecompTest::suite());
}

