
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "algebra/nabeliangroup.h"
#include "split/nsignature.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NAbelianGroup;
using regina::NPerm;
using regina::NSignature;
using regina::NTetrahedron;
using regina::NTriangulation;

class NTriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NTriangulationTest);

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(boundaryComponents);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(homologyH1Bdry);
    CPPUNIT_TEST(zeroEfficiency);

    CPPUNIT_TEST_SUITE_END();

    private:
        // Closed orientable:
        NTriangulation lens8_3;
            /**< The lens space L(8,3). */
        NTriangulation lens8_3_large;
            /**< The lens space L(8,3) with a non-minimal triangulation . */
        NTriangulation rp3rp3;
            /**< The connected sum RP^3 # RP^3. */
        NTriangulation q32xz3;
            /**< The orbit manifold S^3 / Q_32 x Z_3. */

        // Closed orientable, very large:
        NTriangulation lens100_1;
            /**< The lens space L(100,1). */

        // Bounded orientable:
        NTriangulation lst3_4_7;
            /**< The layered solid torus LST(3,4,7). */

        // Ideal orientable:
        NTriangulation figure8;
            /**< The figure eight knot complement. */

        // Closed non-orientable:
        NTriangulation rp2xs1;
            /**< The product RP^2 x S^1. */

        // Bounded non-orientable:
        NTriangulation solidKB;
            /**< A solid Klein bottle. */

        // Ideal non-orientable:
        NTriangulation gieseking;
            /**< The Gieseking manifold. */

    public:
        void generateFromSig(NTriangulation& tri, const std::string& sigStr) {
            NSignature* sig = NSignature::parse(sigStr);
            if (sig == 0)
                return;

            NTriangulation* triNew = sig->triangulate();
            delete sig;
            if (triNew == 0)
                return;

            tri.insertTriangulation(*triNew);
            delete triNew;
        }

        void setUp() {
            // Some of our triangulations can be constructed automatically.
            lens8_3.insertLensSpace(8, 3);
            lens100_1.insertLensSpace(100, 1);
            lst3_4_7.insertLayeredSolidTorus(3, 4);

            // Some of our triangulations can be generated from
            // splitting surfaces.
            generateFromSig(lens8_3_large, "aabcb.cd.d");
            generateFromSig(rp3rp3, "aabccd.b.d");
            generateFromSig(q32xz3, "aabcdb.cedfef");

            // The rest alas must be done manually.
            NTetrahedron* r;
            NTetrahedron* s;
            NTetrahedron* t;

            // The two-tetrahedron figure eight knot complement is
            // described at the beginning of chapter 8 of Richard
            // Rannard's PhD thesis.
            r = new NTetrahedron();
            s = new NTetrahedron();
            r->joinTo(0, s, NPerm(1, 3, 0, 2));
            r->joinTo(1, s, NPerm(2, 0, 3, 1));
            r->joinTo(2, s, NPerm(0, 3, 2, 1));
            r->joinTo(3, s, NPerm(2, 1, 0, 3));
            figure8.addTetrahedron(r);
            figure8.addTetrahedron(s);

            // A three-tetrahedron Klein bottle and a three-tetrahedron
            // RP^2 x S^1 formed by identifying its boundary faces are
            // described in section 3.5.1 of Ben Burton's PhD thesis.
            r = new NTetrahedron();
            s = new NTetrahedron();
            t = new NTetrahedron();
            s->joinTo(0, r, NPerm(0, 1, 2, 3));
            s->joinTo(3, r, NPerm(3, 0, 1, 2));
            s->joinTo(1, t, NPerm(3, 0, 1, 2));
            s->joinTo(2, t, NPerm(0, 1, 2, 3));
            solidKB.addTetrahedron(r);
            solidKB.addTetrahedron(s);
            solidKB.addTetrahedron(t);

            rp2xs1.insertTriangulation(solidKB);
            r = rp2xs1.getTetrahedron(0);
            t = rp2xs1.getTetrahedron(2);
            r->joinTo(1, t, NPerm(2, 3, 0, 1));
            r->joinTo(3, t, NPerm(2, 3, 0, 1));
            rp2xs1.gluingsHaveChanged();

            // The Gieseking manifold is simple enough; it has only one
            // tetrahedron.
            r = new NTetrahedron();
            r->joinTo(0, r, NPerm(1, 2, 0, 3));
            r->joinTo(2, r, NPerm(0, 2, 3, 1));
            gieseking.addTetrahedron(r);
        }

        void tearDown() {
        }

        void validity() {
            CPPUNIT_ASSERT_MESSAGE("L(8,3) is not valid.",
                lens8_3.isValid());
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) is not valid.",
                lens8_3_large.isValid());
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 is not valid.",
                rp3rp3.isValid());
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_32 x Z_3 is not valid.",
                q32xz3.isValid());
            CPPUNIT_ASSERT_MESSAGE("L(100,1) is not valid.",
                lens100_1.isValid());
            CPPUNIT_ASSERT_MESSAGE("LST(3,4,7) is not valid.",
                lst3_4_7.isValid());
            CPPUNIT_ASSERT_MESSAGE("The figure eight knot complement "
                "is not valid.",
                figure8.isValid());
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 is not valid.",
                rp2xs1.isValid());
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle is not valid.",
                solidKB.isValid());
            CPPUNIT_ASSERT_MESSAGE("The Gieseking manifold is not valid.",
                gieseking.isValid());
        }

        void orientability() {
            CPPUNIT_ASSERT_MESSAGE("L(8,3) is not orientable.",
                lens8_3.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) is not orientable.",
                lens8_3_large.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 is not orientable.",
                rp3rp3.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_32 x Z_3 is not orientable.",
                q32xz3.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("L(100,1) is not orientable.",
                lens100_1.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("LST(3,4,7) is not orientable.",
                lst3_4_7.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("The figure eight knot complement "
                "is not orientable.",
                figure8.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 is orientable.",
                ! rp2xs1.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle is orientable.",
                ! solidKB.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("The Gieseking manifold is orientable.",
                ! gieseking.isOrientable());
        }

        void boundaryComponents() {
            CPPUNIT_ASSERT_MESSAGE("L(8,3) has boundary components.",
                lens8_3.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) has boundary components.",
                lens8_3_large.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 has boundary components.",
                rp3rp3.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_32 x Z_3 has boundary components.",
                q32xz3.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("L(100,1) has boundary components.",
                lens100_1.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("LST(3,4,7) has no boundary components.",
                lst3_4_7.getNumberOfBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The figure eight knot complement "
                "has no boundary components.",
                figure8.getNumberOfBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 has boundary components.",
                rp2xs1.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle "
                "has no boundary components.",
                solidKB.getNumberOfBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The Gieseking manifold "
                "has no boundary components.",
                gieseking.getNumberOfBoundaryComponents() > 0);

            // TODO: Test the individual boundary components.
            // TODO: Check that nobody has too many boundary components.
        }

        void verifyGroup(const NAbelianGroup& g, const std::string& grpName,
                unsigned rank) {
            // Construct the error message.
            std::ostringstream msg;
            msg << grpName << " is " << g.toString() << ", not ";
            if (rank == 0)
                msg << "0";
            else if (rank == 1)
                msg << "Z";
            else
                msg << rank << " Z";
            msg << '.';

            // Check the group.
            if (g.getRank() != rank)
                CPPUNIT_FAIL(msg.str());
            else if (g.getNumberOfInvariantFactors() != 0)
                CPPUNIT_FAIL(msg.str());
        }

        void verifyGroup(const NAbelianGroup& g, const std::string& grpName,
                unsigned rank, unsigned long torsionDegree) {
            // Construct the error message.
            std::ostringstream msg;
            msg << grpName << " is " << g.toString() << ", not ";
            if (rank == 1)
                msg << "Z + ";
            else if (rank > 1)
                msg << rank << " Z + ";
            msg << "Z_" << torsionDegree << '.';

            // Check the group.
            if (g.getRank() != rank)
                CPPUNIT_FAIL(msg.str());
            else {
                // Ranks match.
                if (g.getNumberOfInvariantFactors() != 1 ||
                        g.getInvariantFactor(0) != torsionDegree)
                    CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyGroup(const NAbelianGroup& g, const std::string& grpName,
                unsigned rank, unsigned long torsionDegree1,
                unsigned long torsionDegree2) {
            // Construct the error message.
            std::ostringstream msg;
            msg << grpName << " is " << g.toString() << ", not ";
            if (rank == 1)
                msg << "Z + ";
            else if (rank > 1)
                msg << rank << " Z + ";
            msg << "Z_" << torsionDegree1 << " + Z_" << torsionDegree2 << '.';

            // Check the group.
            if (g.getRank() != rank)
                CPPUNIT_FAIL(msg.str());
            else {
                // Ranks match.
                if (g.getNumberOfInvariantFactors() != 2 ||
                        g.getInvariantFactor(0) != torsionDegree1 ||
                        g.getInvariantFactor(1) != torsionDegree2)
                    CPPUNIT_FAIL(msg.str());
            }
        }

        void homologyH1() {
            verifyGroup(lens8_3.getHomologyH1(),
                "H1(L(8,3))", 0, 8);
            verifyGroup(lens8_3_large.getHomologyH1(),
                "H1(Large L(8,3))", 0, 8);
            verifyGroup(rp3rp3.getHomologyH1(),
                "H1(RP^3 # RP^3)", 0, 2, 2);
            verifyGroup(q32xz3.getHomologyH1(),
                "H1(S^3 / Q_32 x Z_3)", 0, 2, 6);
            verifyGroup(lens100_1.getHomologyH1(),
                "H1(L(100,1))", 0, 100);
            verifyGroup(lst3_4_7.getHomologyH1(),
                "H1(LST(3,4,7))", 1);
            verifyGroup(figure8.getHomologyH1(),
                "H1(figure eight knot complement)", 1);
            verifyGroup(rp2xs1.getHomologyH1(),
                "H1(RP^2 x S^1)", 1, 2);
            verifyGroup(solidKB.getHomologyH1(),
                "H1(solid Klein bottle)", 1);
            verifyGroup(gieseking.getHomologyH1(),
                "H1(Gieseking manifold)", 1);
        }

        void homologyH1Bdry() {
            verifyGroup(lens8_3.getHomologyH1Bdry(),
                "Boundary H1(L(8,3))", 0);
            verifyGroup(lens8_3_large.getHomologyH1Bdry(),
                "Boundary H1(Large L(8,3))", 0);
            verifyGroup(rp3rp3.getHomologyH1Bdry(),
                "Boundary H1(RP^3 # RP^3)", 0);
            verifyGroup(q32xz3.getHomologyH1Bdry(),
                "Boundary H1(S^3 / Q_32 x Z_3)", 0);
            verifyGroup(lens100_1.getHomologyH1Bdry(),
                "Boundary H1(L(100,1))", 0);
            verifyGroup(lst3_4_7.getHomologyH1Bdry(),
                "Boundary H1(LST(3,4,7))", 2);
            verifyGroup(figure8.getHomologyH1Bdry(),
                "Boundary H1(figure eight knot complement)", 2);
            verifyGroup(rp2xs1.getHomologyH1Bdry(),
                "Boundary H1(RP^2 x S^1)", 0);
            verifyGroup(solidKB.getHomologyH1Bdry(),
                "Boundary H1(solid Klein bottle)", 1, 2);
            verifyGroup(gieseking.getHomologyH1Bdry(),
                "Boundary H1(Gieseking manifold)", 1, 2);
        }

        void zeroEfficiency() {
            CPPUNIT_ASSERT_MESSAGE("L(8,3) is not 0-efficient.",
                lens8_3.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) is 0-efficient.",
                ! lens8_3_large.isZeroEfficient());
                // Contains a non-vertex-linking 2-sphere.
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 is 0-efficient.",
                ! rp3rp3.isZeroEfficient());
                // Contains a pair of one-sided projective planes.
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_32 x Z_3 is not 0-efficient.",
                q32xz3.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("LST(3,4,7) is 0-efficient.",
                ! lst3_4_7.isZeroEfficient());
                // Contains a non-trivial disc.
            CPPUNIT_ASSERT_MESSAGE("The figure eight knot complement "
                "is not 0-efficient.",
                figure8.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 is not 0-efficient.",
                rp2xs1.isZeroEfficient());
                // Contains a two-sided projective plane.
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle is 0-efficient.",
                ! solidKB.isZeroEfficient());
                // Contains a non-trivial disc.
            CPPUNIT_ASSERT_MESSAGE("The Gieseking manifold is not 0-efficient.",
                gieseking.isZeroEfficient());
        }
};

void addNTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NTriangulationTest::suite());
}

