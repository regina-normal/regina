
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

#include <cmath>
#include <memory>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "split/nsignature.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NPerm;
using regina::NSignature;
using regina::NTetrahedron;
using regina::NTriangulation;

class ThreeSphereTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ThreeSphereTest);

    CPPUNIT_TEST(threeSphereRecognition);
    CPPUNIT_TEST(threeBallRecognition);

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
            if (! tri->isThreeSphere()) {
                CPPUNIT_FAIL(("The 3-sphere " + triName +
                    " is not recognised as such.").c_str());
            }

            return tri;
        }

        NTriangulation* verifyNotThreeSphere(NTriangulation* tri,
                const std::string& triName) {
            if (tri->isThreeSphere()) {
                CPPUNIT_FAIL(("The non-3-sphere " + triName +
                    " is recognised as a 3-sphere.").c_str());
            }

            return tri;
        }

        void verifySigThreeSphere(const std::string& sigStr) {
            delete verifyThreeSphere(generateFromSig(sigStr), sigStr);
        }

        void verifySigNotThreeSphere(const std::string& sigStr) {
            delete verifyNotThreeSphere(generateFromSig(sigStr), sigStr);
        }

        void threeSphereRecognition() {
            NTriangulation* tri;

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
            tri = new NTriangulation();
            tri->insertLayeredLensSpace(1,0);
            delete verifyThreeSphere(tri, "L(1,0)");

            // Non-3-spheres obtained from splitting surface signatures:
            verifySigNotThreeSphere("(aab)(b)");
            verifySigNotThreeSphere("(ab)(ab)");
            verifySigNotThreeSphere("(aabcb)(c)");
            verifySigNotThreeSphere("(aabc)(b)(c)");
            verifySigNotThreeSphere("(aabcdcb)(d)");
            verifySigNotThreeSphere("(aabcdb)(c)(d)");
            verifySigNotThreeSphere("(aabcd)(b)(c)(d)");
            verifySigNotThreeSphere("(aabc)(bd)(c)(d)");
            verifySigNotThreeSphere("(abac)(bd)(cd)");
            verifySigNotThreeSphere("(abac)(bd)(c)(d)");
            verifySigNotThreeSphere("(abcd)(ac)(bd)");
            verifySigNotThreeSphere("(aab)(bc)(cd)(d)");
            verifySigNotThreeSphere("(abc)(ab)(cd)(d)");
            verifySigNotThreeSphere("(abc)(ad)(bd)(c)");
            verifySigNotThreeSphere("(abac)(b)(c)");
            verifySigNotThreeSphere("(abacdc)(b)(d)");
            verifySigNotThreeSphere("(abcabd)(c)(d)");
            verifySigNotThreeSphere("(abacd)(b)(c)(d)");
            verifySigNotThreeSphere("(aabc)(bd)(cd)");
            verifySigNotThreeSphere("(abacde)(cf)(fg)(b)(d)(e)(g)");
            verifySigNotThreeSphere("(abc)(abc)");
            verifySigNotThreeSphere("(abc)(acb)");

            verifySigNotThreeSphere("(aa)");
            verifySigNotThreeSphere("(aabb)");
            verifySigNotThreeSphere("(abab)");
            verifySigNotThreeSphere("(aabccb)");
            verifySigNotThreeSphere("(abcabc)");
            verifySigNotThreeSphere("(aab)(bcc)");
            verifySigNotThreeSphere("(aab)(bc)(c)");
            verifySigNotThreeSphere("(ab)(ac)(bc)");
            verifySigNotThreeSphere("(aabcddcb)");
            verifySigNotThreeSphere("(abcdabcd)");
            verifySigNotThreeSphere("(aabbc)(cd)(d)");
            verifySigNotThreeSphere("(aabcb)(cdd)");
            verifySigNotThreeSphere("(aabcb)(cd)(d)");
            verifySigNotThreeSphere("(ababc)(cd)(d)");
            verifySigNotThreeSphere("(abac)(bdcd)");
            verifySigNotThreeSphere("(abac)(bcd)(d)");
            verifySigNotThreeSphere("(abac)(bdd)(c)");
            verifySigNotThreeSphere("(abcd)(abcd)");
            verifySigNotThreeSphere("(abcd)(adcb)");
            verifySigNotThreeSphere("(aab)(bcd)(c)(d)");
            verifySigNotThreeSphere("(abc)(abd)(cd)");
            verifySigNotThreeSphere("(abc)(acd)(bd)");
            verifySigNotThreeSphere("(abcdefgh)(abcdefgh)");

            verifySigNotThreeSphere("(aabccd)(b)(d)");;
            verifySigNotThreeSphere("(abacbd)(cd)");;
            verifySigNotThreeSphere("(aabcdecb)(dfeg)(fg)");
            verifySigNotThreeSphere("(aabbc)(cdef)(egg)(d)(f)");
            verifySigNotThreeSphere("(ababc)(cdef)(egg)(d)(f)");
            verifySigNotThreeSphere("(abcd)(acbe)(dfeg)(f)(g)");
            verifySigNotThreeSphere("(abcd)(adce)(befg)(f)(g)");
            verifySigNotThreeSphere("(abcde)(aff)(cgg)(b)(d)(e)");
            verifySigNotThreeSphere("(abcde)(adf)(bfg)(ce)(g)");
            verifySigNotThreeSphere("(ababc)(cde)(dfg)(fg)(e)");
            verifySigNotThreeSphere("(abcdef)(acegg)(bdf)");
            verifySigNotThreeSphere("(abacde)(dffgg)(b)(c)(e)");
            verifySigNotThreeSphere("(aabccdeffeg)(b)(d)(g)");
            verifySigNotThreeSphere("(aabcde)(cfg)(dgf)(b)(e)");

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
            delete verifyNotThreeSphere(tri,
                "Poincare homology sphere (plugged)");

            // Poincare homology sphere as an augmented triangular solid
            // torus:
            tri = new NTriangulation();
            tri->insertAugTriSolidTorus(2, -1, 3, 1, 5, -4);
            delete verifyNotThreeSphere(tri,
                "Poincare homology sphere (aug I)");

            // Poincare homology sphere as another augmented triangular solid
            // torus:
            tri = new NTriangulation();
            tri->insertAugTriSolidTorus(2, -1, 3, -2, 5, 1);
            delete verifyNotThreeSphere(tri,
                "Poincare homology sphere (aug II)");
        }

        void threeBallRecognition() {
        }
};

void addThreeSphere(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(ThreeSphereTest::suite());
}

