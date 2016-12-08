
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

#include <cmath>
#include <memory>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "algebra/abeliangroup.h"
#include "manifold/manifold.h"
#include "packet/container.h"
#include "split/signature.h"
#include "subcomplex/standardtri.h"
#include "triangulation/dim3.h"

#include "testsuite/exhaustive.h"
#include "testsuite/dim3/testtriangulation.h"

using regina::AbelianGroup;
using regina::Container;
using regina::Manifold;
using regina::Perm;
using regina::Signature;
using regina::StandardTriangulation;
using regina::Tetrahedron;
using regina::Triangulation;

class ConnectedSumDecompTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ConnectedSumDecompTest);

    CPPUNIT_TEST(threeSpheres);
    CPPUNIT_TEST(specialCases);
    CPPUNIT_TEST(primes);
    CPPUNIT_TEST(nonTrivialSums);
    CPPUNIT_TEST(extendedCensus);

    CPPUNIT_TEST_SUITE_END();

    public:
        Triangulation<3>* generateFromSig(const std::string& sigStr) {
            Signature* sig = Signature::parse(sigStr);
            if (sig == 0)
                return 0;

            Triangulation<3>* triNew = sig->triangulate();
            delete sig;
            return triNew;
        }

        void setUp() {
        }

        void tearDown() {
        }

        Triangulation<3>* verifyThreeSphere(Triangulation<3>* tri,
                const std::string& triName) {
            Container summands;
            unsigned long ans = tri->connectedSumDecomposition(&summands);

            CPPUNIT_ASSERT_MESSAGE("The 3-sphere " + triName +
                " is reported to have prime summands.",
                ans == 0 && summands.countChildren() == 0);

            return tri;
        }

        void verifySigThreeSphere(const std::string& sigStr) {
            delete verifyThreeSphere(generateFromSig(sigStr), sigStr);
        }

        Triangulation<3>* verifyPrime(Triangulation<3>* tri,
                const std::string& triName, const std::string& manifold) {
            // Recall that ASSERTS throw exceptions, so after testing
            // them we can assume their conditions to be true.
            Container summands;
            unsigned long ans = tri->connectedSumDecomposition(&summands);

            CPPUNIT_ASSERT_MESSAGE("The prime 3-manifold " + triName +
                " is reported to be a 3-sphere.",
                ans > 0 && summands.firstChild() != 0);
            CPPUNIT_ASSERT_MESSAGE("The prime 3-manifold " + triName +
                " is reported to be composite.", ans == 1 &&
                summands.firstChild() == summands.lastChild());

            Triangulation<3>* summand = static_cast<Triangulation<3>*>(
                summands.firstChild());

            std::unique_ptr<StandardTriangulation> stdTri(
                StandardTriangulation::isStandardTriangulation(summand));
            CPPUNIT_ASSERT_MESSAGE("The single prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri.get() != 0);

            std::unique_ptr<Manifold> stdManifold(stdTri->manifold());
            CPPUNIT_ASSERT_MESSAGE("The single prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold.get() != 0);

            std::string stdName = stdManifold->name();
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
                summand->homology() == tri->homology());

            return tri;
        }

        void verifySigPrime(const std::string& sigStr,
                const std::string& manifold) {
            delete verifyPrime(generateFromSig(sigStr), sigStr, manifold);
        }

        /**
         * NOTE: The two manifold names must be given in lexicographical order.
         */
        Triangulation<3>* verifyPair(Triangulation<3>* tri,
                const std::string& triName, const std::string& manifold1,
                const std::string& manifold2) {
            // Recall that ASSERTS throw exceptions, so after testing
            // them we can assume their conditions to be true.
            Container summands;
            unsigned long ans = tri->connectedSumDecomposition(&summands);

            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to be a 3-sphere.",
                ans > 0 && summands.firstChild() != 0);
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to be prime.", ans > 1 &&
                summands.firstChild() != summands.lastChild());
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to have more than two summands.", ans == 2 &&
                summands.firstChild()->nextSibling() == summands.lastChild());

            Triangulation<3>* summand1 = static_cast<Triangulation<3>*>(
                summands.firstChild());
            Triangulation<3>* summand2 = static_cast<Triangulation<3>*>(
                summands.lastChild());

            std::unique_ptr<StandardTriangulation> stdTri1(
                StandardTriangulation::isStandardTriangulation(summand1));
            CPPUNIT_ASSERT_MESSAGE("The first prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri1.get() != 0);

            std::unique_ptr<StandardTriangulation> stdTri2(
                StandardTriangulation::isStandardTriangulation(summand2));
            CPPUNIT_ASSERT_MESSAGE("The second prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri2.get() != 0);

            std::unique_ptr<Manifold> stdManifold1(stdTri1->manifold());
            CPPUNIT_ASSERT_MESSAGE("The first prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold1.get() != 0);

            std::unique_ptr<Manifold> stdManifold2(stdTri2->manifold());
            CPPUNIT_ASSERT_MESSAGE("The second prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold2.get() != 0);

            // Obtain the manifold names in lexicographical order.
            std::string stdName1 = stdManifold1->name();
            std::string stdName2 = stdManifold2->name();
            if (stdName2 < stdName1) {
                std::string tmp = stdName2;
                stdName2 = stdName1;
                stdName1 = tmp;

                // Swap the summands also so we can correctly analyse
                // them later.
                Triangulation<3>* tmpTri = summand2;
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
            AbelianGroup combined(summand1->homology());
            combined.addGroup(summand2->homology());
            CPPUNIT_ASSERT_MESSAGE("The prime summands of " + triName +
                " have inconsistent first homology groups.",
                tri->homology() == combined);


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

        Triangulation<3>* verifyRP3x3(Triangulation<3>* tri,
                const std::string& triName) {
            // Recall that ASSERTS throw exceptions, so after testing
            // them we can assume their conditions to be true.
            Container summands;
            unsigned long ans = tri->connectedSumDecomposition(&summands);

            Triangulation<3>* summand1 = static_cast<Triangulation<3>*>(
                summands.firstChild());
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to be a 3-sphere.", ans > 0 && summand1 != 0);
            Triangulation<3>* summand2 = static_cast<Triangulation<3>*>(
                summand1->nextSibling());
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to be prime.", ans > 1 && summand2 != 0);
            Triangulation<3>* summand3 = static_cast<Triangulation<3>*>(
                summand2->nextSibling());
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to have only two prime summands.",
                ans > 2 && summand3 != 0);
            CPPUNIT_ASSERT_MESSAGE("The composite 3-manifold " + triName +
                " is reported to have more than three summands.", ans == 3 &&
                summand3 == summands.lastChild());

            std::unique_ptr<StandardTriangulation> stdTri1(
                StandardTriangulation::isStandardTriangulation(summand1));
            CPPUNIT_ASSERT_MESSAGE("The first prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri1.get() != 0);

            std::unique_ptr<StandardTriangulation> stdTri2(
                StandardTriangulation::isStandardTriangulation(summand2));
            CPPUNIT_ASSERT_MESSAGE("The second prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri2.get() != 0);

            std::unique_ptr<StandardTriangulation> stdTri3(
                StandardTriangulation::isStandardTriangulation(summand3));
            CPPUNIT_ASSERT_MESSAGE("The third prime summand of " + triName +
                " forms an unrecognised triangulation.", stdTri3.get() != 0);

            std::unique_ptr<Manifold> stdManifold1(stdTri1->manifold());
            CPPUNIT_ASSERT_MESSAGE("The first prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold1.get() != 0);

            std::unique_ptr<Manifold> stdManifold2(stdTri2->manifold());
            CPPUNIT_ASSERT_MESSAGE("The second prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold2.get() != 0);

            std::unique_ptr<Manifold> stdManifold3(stdTri3->manifold());
            CPPUNIT_ASSERT_MESSAGE("The third prime summand of " + triName +
                " forms an unrecognised 3-manifold.", stdManifold3.get() != 0);

            // Obtain the manifold names in lexicographical order.
            std::string stdName1 = stdManifold1->name();
            std::string stdName2 = stdManifold2->name();
            std::string stdName3 = stdManifold3->name();

            CPPUNIT_ASSERT_MESSAGE("One of the three prime summands of " +
                triName + " does not form RP3.",
                stdName1 == "RP3" && stdName2 == "RP3" && stdName3 == "RP3");

            // Test that the homologies are consistent.
            AbelianGroup combined(summand1->homology());
            combined.addGroup(summand2->homology());
            combined.addGroup(summand3->homology());
            CPPUNIT_ASSERT_MESSAGE("The prime summands of " + triName +
                " have inconsistent first homology groups.",
                tri->homology() == combined);

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
            Triangulation<3>* tri;

            tri = new Triangulation<3>();
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
            Triangulation<3>* tri;

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
            tri = new Triangulation<3>;
            Tetrahedron<3>* tet[5];
            int i;
            for (i = 0; i < 5; i++)
                tet[i] = tri->newTetrahedron();
            tet[0]->join(0, tet[4], Perm<4>(1,0,2,3));
            tet[0]->join(1, tet[3], Perm<4>(0,2,3,1));
            tet[0]->join(2, tet[1], Perm<4>(0,1,3,2));
            tet[0]->join(3, tet[2], Perm<4>(2,1,3,0));
            tet[1]->join(0, tet[3], Perm<4>(1,3,2,0));
            tet[1]->join(1, tet[2], Perm<4>(0,2,3,1));
            tet[1]->join(2, tet[4], Perm<4>(2,1,0,3));
            tet[2]->join(1, tet[4], Perm<4>(0,2,3,1));
            tet[2]->join(3, tet[3], Perm<4>(3,1,2,0));
            tet[3]->join(3, tet[4], Perm<4>(0,1,2,3));
            delete verifyPrime(tri, "the Poincare homology sphere (plugged)",
                "S3/P120");

            // Poincare homology sphere as an augmented triangular solid
            // torus:
            tri = new Triangulation<3>();
            tri->insertAugTriSolidTorus(2, -1, 3, 1, 5, -4);
            delete verifyPrime(tri, "the Poincare homology sphere (aug I)",
                "S3/P120");

            // Poincare homology sphere as another augmented triangular solid
            // torus:
            tri = new Triangulation<3>();
            tri->insertAugTriSolidTorus(2, -1, 3, -2, 5, 1);
            delete verifyPrime(tri, "the Poincare homology sphere (aug II)",
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

        static void testDecomp(Triangulation<3>* tri) {
            // Checked the connectedSumDecomposition() preconditions.
            if (! (tri->isValid() && tri->isClosed() && tri->isConnected()))
                return;

            Container parent;
            long ncomp = tri->connectedSumDecomposition(&parent);

            if (ncomp == -1) {
                // The routine reported an embedded two-sided projective plane.
                if (tri->isOrientable()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << tri->label()
                        << " is orientable but reports an embedded "
                        "two-sided projective plane.";
                    CPPUNIT_FAIL(msg.str());
                }
                return;
            }

            if (ncomp != parent.countChildren()) {
                std::ostringstream msg;
                msg << "Triangulation " << tri->label()
                    << " reports a different number of connected sum "
                    "components from how many it actually builds.";
                CPPUNIT_FAIL(msg.str());
            }

            AbelianGroup h1;
            Triangulation<3>* term;
            bool foundNor = false;
            for (regina::Packet* p = parent.firstChild(); p;
                    p = p->nextSibling()) {
                term = static_cast<Triangulation<3>*>(p);
                if (! term->isOrientable())
                    foundNor = true;
                if (! term->isZeroEfficient()) {
                    // Special cases: 2-tetrahedron RP3, L(3,1), S2xS1, S2x~S1.
                    if (! (term->size() == 2 &&
                            (term->isoSig() == "cMcabbgqw" /* RP3 */ ||
                            term->isoSig() == "cMcabbgqj" /* L(3,1) */ ||
                            term->isoSig() == "cPcbbbaai" /* L(3,1) */ ||
                            term->isoSig() == "cMcabbjaj" /* S2xS1 */ ||
                            term->isoSig() == "cPcbbbajs" /* S2x~S1 */))) {
                        std::ostringstream msg;
                        msg << "Triangulation " << tri->label()
                            << " reports a non-zero-efficient summand "
                            << term->isoSig() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
                if (term->isThreeSphere()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << tri->label()
                        << " reports a 3-sphere summand "
                        << term->isoSig() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                h1.addGroup(term->homology());
            }

            if ((! foundNor) && (! tri->isOrientable())) {
                std::ostringstream msg;
                msg << "Triangulation " << tri->label()
                    << " is non-orientable but none of its summands are.";
                CPPUNIT_FAIL(msg.str());
            }

            if (foundNor && tri->isOrientable()) {
                std::ostringstream msg;
                msg << "Triangulation " << tri->label()
                    << " is orientable but one of its summands is not.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! (h1 == tri->homology())) {
                std::ostringstream msg;
                msg << "Triangulation " << tri->label()
                    << " has first homology that does not match "
                    "the combination of its summands' first homologies.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void extendedCensus() {
            runCensusAllClosed(&testDecomp);
            runCensusMinClosed(&testDecomp);
        }
};

void addConnectedSumDecomp(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(ConnectedSumDecompTest::suite());
}

