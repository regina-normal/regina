
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "maths/approx.h"
#include "maths/numbertheory.h"
#include "split/nsignature.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NAbelianGroup;
using regina::NGroupPresentation;
using regina::NPerm;
using regina::NSignature;
using regina::NTetrahedron;
using regina::NTriangulation;

class NTriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NTriangulationTest);

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(standardness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(boundaryComponents);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(homologyH1Bdry);
    CPPUNIT_TEST(fundGroup);
    CPPUNIT_TEST(zeroEfficiency);
    CPPUNIT_TEST(turaevViro);
    CPPUNIT_TEST(propertyUpdates);

    CPPUNIT_TEST_SUITE_END();

    private:
        // Trivial:
        NTriangulation empty;
            /**< An empty triangulation. */
        NTriangulation singleTet;
            /**< A single tetrahedron with no face gluings. */

        // Closed orientable:
        NTriangulation s3;
            /**< A one-vertex 3-sphere. */
        NTriangulation s2xs1;
            /**< The product space S^2 x S^1. */
        NTriangulation rp3;
            /**< A two-vertex triangulation of RP^3. */
        NTriangulation lens3_1;
            /**< A two-vertex lens space L(3,1). */
        NTriangulation lens8_3;
            /**< The layered lens space L(8,3). */
        NTriangulation lens8_3_large;
            /**< The lens space L(8,3) with a non-minimal triangulation . */
        NTriangulation lens7_1_loop;
            /**< An untwisted layered loop representing L(7,1). */
        NTriangulation rp3rp3;
            /**< The connected sum RP^3 # RP^3. */
        NTriangulation q32xz3;
            /**< The orbit manifold S^3 / Q_32 x Z_3. */
        NTriangulation q28;
            /**< A twisted layered loop representing S^3 / Q_28. */

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

        // Invalid edges:
        NTriangulation invalidEdges;
            /**< Contains two invalid edges, but all vertices have
                 2-sphere links. */

        // Non-standard vertex links:
        NTriangulation twoProjPlaneCusps;
            /**< A subdivision of invalidEdges, resulting in all edges
                 valid but two projective plane cusps.  Note that this
                 triangulation has a 3-sphere orientable double cover. */
        NTriangulation cuspedGenusTwoTorus;
            /**< A solid genus two torus with a cusped boundary. */

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
            // Begin with trivial cases.
            // The empty triangulation needs no initialisation whatsoever.
            singleTet.addTetrahedron(new NTetrahedron());

            // Some of our triangulations can be constructed automatically.
            s3.insertLayeredLensSpace(1, 0);
            s2xs1.insertLayeredLensSpace(0, 1);
            rp3.insertLayeredLoop(2, false);
            lens8_3.insertLayeredLensSpace(8, 3);
            lens100_1.insertLayeredLensSpace(100, 1);
            lst3_4_7.insertLayeredSolidTorus(3, 4);
            q28.insertLayeredLoop(7, true);
            lens7_1_loop.insertLayeredLoop(7, false);

            // Some of our triangulations can be generated from
            // splitting surfaces.
            generateFromSig(lens8_3_large, "aabcb.cd.d");
            generateFromSig(rp3rp3, "aabccd.b.d");
            generateFromSig(q32xz3, "aabcdb.cedfef");

            // The rest alas must be done manually.
            NTetrahedron* r;
            NTetrahedron* s;
            NTetrahedron* t;
            NTetrahedron* u;

            // A two-tetrahedron two-vertex L(3,1) is straightforward to
            // construct using a vertex of degree two.
            r = new NTetrahedron();
            s = new NTetrahedron();
            r->joinTo(0, s, NPerm(0, 2, 3, 1));
            r->joinTo(1, s, NPerm());
            r->joinTo(2, s, NPerm());
            r->joinTo(3, s, NPerm());
            lens3_1.addTetrahedron(r);
            lens3_1.addTetrahedron(s);

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

            // For a triangulation with invalid edges, we simply fold
            // the faces of a tetrahedron together in pairs (as in a
            // 3-sphere triangulation) but apply a reflection to each fold.
            r = new NTetrahedron();
            r->joinTo(0, r, NPerm(1, 0, 3, 2));
            r->joinTo(2, r, NPerm(1, 0, 3, 2));
            invalidEdges.addTetrahedron(r);

            twoProjPlaneCusps.insertTriangulation(invalidEdges);
            twoProjPlaneCusps.barycentricSubdivision();

            // We create the cusped solid genus two torus by
            // constructing an ordinary solid genus two torus and then
            // converting the real boundary to an ideal vertex.
            r = new NTetrahedron();
            s = new NTetrahedron();
            t = new NTetrahedron();
            u = new NTetrahedron();
            r->joinTo(0, s, NPerm());
            r->joinTo(1, t, NPerm(1, 2, 3, 0));
            r->joinTo(2, u, NPerm(1, 0, 3, 2));
            s->joinTo(3, t, NPerm());
            t->joinTo(1, u, NPerm());
            cuspedGenusTwoTorus.addTetrahedron(r);
            cuspedGenusTwoTorus.addTetrahedron(s);
            cuspedGenusTwoTorus.addTetrahedron(t);
            cuspedGenusTwoTorus.addTetrahedron(u);
            cuspedGenusTwoTorus.cuspBoundary();
        }

        void tearDown() {
        }

        void validity() {
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not valid.",
                empty.isValid());
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron is not valid.",
                singleTet.isValid());
            CPPUNIT_ASSERT_MESSAGE("S^3 is not valid.",
                s3.isValid());
            CPPUNIT_ASSERT_MESSAGE("S^2 x S^1 is not valid.",
                s2xs1.isValid());
            CPPUNIT_ASSERT_MESSAGE("RP^3 is not valid.",
                rp3.isValid());
            CPPUNIT_ASSERT_MESSAGE("L(3,1) is not valid.",
                lens3_1.isValid());
            CPPUNIT_ASSERT_MESSAGE("Layered loop L(7,1) is not valid.",
                lens7_1_loop.isValid());
            CPPUNIT_ASSERT_MESSAGE("L(8,3) is not valid.",
                lens8_3.isValid());
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) is not valid.",
                lens8_3_large.isValid());
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 is not valid.",
                rp3rp3.isValid());
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_28 is not valid.",
                q28.isValid());
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
            CPPUNIT_ASSERT_MESSAGE("The triangulation with invalid edges "
                "is reported as valid.",
                ! invalidEdges.isValid());
            CPPUNIT_ASSERT_MESSAGE("The triangulation with projective plane "
                "cusps is not valid.",
                twoProjPlaneCusps.isValid());
            CPPUNIT_ASSERT_MESSAGE("The cusped solid genus two torus "
                "is not valid.",
                cuspedGenusTwoTorus.isValid());
        }

        void standardness() {
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not standard.",
                empty.isStandard());
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron is not standard.",
                singleTet.isStandard());
            CPPUNIT_ASSERT_MESSAGE("S^3 is not standard.",
                s3.isStandard());
            CPPUNIT_ASSERT_MESSAGE("S^2 x S^1 is not standard.",
                s2xs1.isStandard());
            CPPUNIT_ASSERT_MESSAGE("RP^3 is not standard.",
                rp3.isStandard());
            CPPUNIT_ASSERT_MESSAGE("L(3,1) is not standard.",
                lens3_1.isStandard());
            CPPUNIT_ASSERT_MESSAGE("Layered loop L(7,1) is not standard.",
                lens7_1_loop.isStandard());
            CPPUNIT_ASSERT_MESSAGE("L(8,3) is not standard.",
                lens8_3.isStandard());
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) is not standard.",
                lens8_3_large.isStandard());
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 is not standard.",
                rp3rp3.isStandard());
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_28 is not standard.",
                q28.isStandard());
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_32 x Z_3 is not standard.",
                q32xz3.isStandard());
            CPPUNIT_ASSERT_MESSAGE("L(100,1) is not standard.",
                lens100_1.isStandard());
            CPPUNIT_ASSERT_MESSAGE("LST(3,4,7) is not standard.",
                lst3_4_7.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The figure eight knot complement "
                "is not standard.",
                figure8.isStandard());
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 is not standard.",
                rp2xs1.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle is not standard.",
                solidKB.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The Gieseking manifold is not standard.",
                gieseking.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The triangulation with invalid edges "
                "is not standard.",
                invalidEdges.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The triangulation with projective plane "
                "cusps is standard.",
                ! twoProjPlaneCusps.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The cusped solid genus two torus "
                "is standard.",
                ! cuspedGenusTwoTorus.isStandard());
        }

        void orientability() {
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not orientable.",
                empty.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron is not orientable.",
                singleTet.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("S^3 is not orientable.",
                s3.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("S^2 x S^1 is not orientable.",
                s2xs1.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("RP^3 is not orientable.",
                rp3.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("L(3,1) is not orientable.",
                lens3_1.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("Layered loop L(7,1) is not orientable.",
                lens7_1_loop.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("L(8,3) is not orientable.",
                lens8_3.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) is not orientable.",
                lens8_3_large.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 is not orientable.",
                rp3rp3.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_28 is not orientable.",
                q28.isOrientable());
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
            CPPUNIT_ASSERT_MESSAGE("The triangulation with invalid edges "
                "is orientable.",
                ! invalidEdges.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("The triangulation with projective plane "
                "cusps is orientable.",
                ! twoProjPlaneCusps.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("The cusped solid genus two torus "
                "is not orientable.",
                cuspedGenusTwoTorus.isOrientable());
        }

        void boundaryComponents() {
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation has boundary "
                "components.", empty.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron has no boundary "
                "components.", singleTet.getNumberOfBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("S^3 has boundary components.",
                s3.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("S^2 x S^1 has boundary components.",
                s2xs1.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("RP^3 has boundary components.",
                rp3.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("L(3,1) has boundary components.",
                lens3_1.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("Layered loop L(7,1) "
                "has boundary components.",
                lens7_1_loop.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("L(8,3) has boundary components.",
                lens8_3.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) has boundary components.",
                lens8_3_large.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 has boundary components.",
                rp3rp3.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_28 has boundary components.",
                q28.getNumberOfBoundaryComponents() == 0);
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
            CPPUNIT_ASSERT_MESSAGE("The triangulation with invalid edges "
                "has boundary components.",
                invalidEdges.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("The triangulation with projective plane "
                "cusps has no boundary components.",
                twoProjPlaneCusps.getNumberOfBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The cusped solid genus two torus "
                "has no boundary components.",
                cuspedGenusTwoTorus.getNumberOfBoundaryComponents() > 0);

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
            verifyGroup(empty.getHomologyH1(),
                "H1(empty triangulation)", 0);
            verifyGroup(singleTet.getHomologyH1(),
                "H1(single tetrahedron)", 0);
            verifyGroup(s3.getHomologyH1(),
                "H1(S^3)", 0);
            verifyGroup(s2xs1.getHomologyH1(),
                "H1(S^2 x S^1)", 1);
            verifyGroup(rp3.getHomologyH1(),
                "H1(RP^3)", 0, 2);
            verifyGroup(lens3_1.getHomologyH1(),
                "H1(L(3,1))", 0, 3);
            verifyGroup(lens7_1_loop.getHomologyH1(),
                "H1(Loop L(7,1))", 0, 7);
            verifyGroup(lens8_3.getHomologyH1(),
                "H1(L(8,3))", 0, 8);
            verifyGroup(lens8_3_large.getHomologyH1(),
                "H1(Large L(8,3))", 0, 8);
            verifyGroup(rp3rp3.getHomologyH1(),
                "H1(RP^3 # RP^3)", 0, 2, 2);
            verifyGroup(q28.getHomologyH1(),
                "H1(S^3 / Q_28)", 0, 4);
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
            verifyGroup(invalidEdges.getHomologyH1(),
                "H1(tri with invalid edges)", 0);
            verifyGroup(twoProjPlaneCusps.getHomologyH1(),
                "H1(tri with projective plane cusps)", 0, 2);
            verifyGroup(cuspedGenusTwoTorus.getHomologyH1(),
                "H1(cusped solid genus two torus)", 2);
        }

        void homologyH1Bdry() {
            verifyGroup(empty.getHomologyH1Bdry(),
                "Boundary H1(empty triangulation)", 0);
            verifyGroup(singleTet.getHomologyH1Bdry(),
                "Boundary H1(single tetrahedron)", 0);
            verifyGroup(s3.getHomologyH1Bdry(),
                "Boundary H1(S^3)", 0);
            verifyGroup(s2xs1.getHomologyH1Bdry(),
                "Boundary H1(S^2 x S^1)", 0);
            verifyGroup(rp3.getHomologyH1Bdry(),
                "Boundary H1(RP^3)", 0);
            verifyGroup(lens3_1.getHomologyH1Bdry(),
                "Boundary H1(L(3,1))", 0);
            verifyGroup(lens7_1_loop.getHomologyH1Bdry(),
                "Boundary H1(Loop L(7,1))", 0);
            verifyGroup(lens8_3.getHomologyH1Bdry(),
                "Boundary H1(L(8,3))", 0);
            verifyGroup(lens8_3_large.getHomologyH1Bdry(),
                "Boundary H1(Large L(8,3))", 0);
            verifyGroup(rp3rp3.getHomologyH1Bdry(),
                "Boundary H1(RP^3 # RP^3)", 0);
            verifyGroup(q28.getHomologyH1Bdry(),
                "Boundary H1(S^3 / Q_28)", 0);
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
            verifyGroup(twoProjPlaneCusps.getHomologyH1Bdry(),
                "Boundary H1(tri with projective plane cusps)", 0, 2, 2);
            verifyGroup(cuspedGenusTwoTorus.getHomologyH1Bdry(),
                "Boundary H1(cusped solid genus two torus)", 4);
        }

        void verifyFundGroup(const NGroupPresentation& g,
                const std::string& grpName, const std::string& expected) {
            std::string actual = g.recogniseGroup();
            if (actual.empty())
                actual = "unknown";

            // Construct the error message.
            std::ostringstream msg;
            msg << grpName << " is " << actual << ", not " << expected << '.';

            // Check the group.
            if (expected != actual)
                CPPUNIT_FAIL(msg.str());
        }

        void fundGroup() {
            verifyFundGroup(empty.getFundamentalGroup(),
                "Fund(empty triangulation)", "0");
            verifyFundGroup(singleTet.getFundamentalGroup(),
                "Fund(single tetrahedron)", "0");
            verifyFundGroup(s3.getFundamentalGroup(),
                "Fund(S^3)", "0");
            verifyFundGroup(s2xs1.getFundamentalGroup(),
                "Fund(S^2 x S^1)", "Z");
            verifyFundGroup(rp3.getFundamentalGroup(),
                "Fund(RP^3)", "Z_2");
            verifyFundGroup(lens3_1.getFundamentalGroup(),
                "Fund(L(3,1))", "Z_3");
            verifyFundGroup(lens7_1_loop.getFundamentalGroup(),
                "Fund(Loop L(7,1))", "Z_7");
            verifyFundGroup(lens8_3.getFundamentalGroup(),
                "Fund(L(8,3))", "Z_8");
            verifyFundGroup(lens8_3_large.getFundamentalGroup(),
                "Fund(Large L(8,3))", "Z_8");
            //verifyFundGroup(rp3rp3.getFundamentalGroup(),
            //    "Fund(RP^3 # RP^3)", 0, 2, 2);
            //verifyFundGroup(q28.getFundamentalGroup(),
            //    "Fund(S^3 / Q_28)", 0, 4);
            //verifyFundGroup(q32xz3.getFundamentalGroup(),
            //    "Fund(S^3 / Q_32 x Z_3)", 0, 2, 6);
            verifyFundGroup(lens100_1.getFundamentalGroup(),
                "Fund(L(100,1))", "Z_100");
            verifyFundGroup(lst3_4_7.getFundamentalGroup(),
                "Fund(LST(3,4,7))", "Z");
            //verifyFundGroup(figure8.getFundamentalGroup(),
            //    "Fund(figure eight knot complement)", 1);
            //verifyFundGroup(rp2xs1.getFundamentalGroup(),
            //    "Fund(RP^2 x S^1)", 1, 2);
            verifyFundGroup(solidKB.getFundamentalGroup(),
                "Fund(solid Klein bottle)", "Z");
            //verifyFundGroup(gieseking.getFundamentalGroup(),
            //    "Fund(Gieseking manifold)", 1);
            verifyFundGroup(invalidEdges.getFundamentalGroup(),
                "Fund(tri with invalid edges)", "0");
            verifyFundGroup(twoProjPlaneCusps.getFundamentalGroup(),
                "Fund(tri with projective plane cusps)", "Z_2");
            verifyFundGroup(cuspedGenusTwoTorus.getFundamentalGroup(),
                "Fund(cusped solid genus two torus)", "Free (2 generators)");
        }

        void zeroEfficiency() {
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not "
                "0-efficient.", empty.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron is 0-efficient.",
                ! singleTet.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("S^3 is not 0-efficient.",
                s3.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("S^2 x S^1 is 0-efficient.",
                ! s2xs1.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("RP^3 is 0-efficient.",
                ! rp3.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("L(3,1) is 0-efficient.",
                ! lens3_1.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("Layered loop L(7,1) is 0-efficient.",
                ! lens7_1_loop.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("L(8,3) is not 0-efficient.",
                lens8_3.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) is 0-efficient.",
                ! lens8_3_large.isZeroEfficient());
                // Contains a non-vertex-linking 2-sphere.
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 is 0-efficient.",
                ! rp3rp3.isZeroEfficient());
                // Contains a pair of one-sided projective planes.
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_28 is not 0-efficient.",
                q28.isZeroEfficient());
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
            // Don't run this one, it takes too long.
            // CPPUNIT_ASSERT_MESSAGE("The cusped solid genus two torus "
            //     "is 0-efficient.",
            //     ! cuspedGenusTwoTorus.isZeroEfficient());
        }

        void verifyTV3(NTriangulation& t, const std::string& triName) {
            // Verify the Turaev-Viro invariants for r=3.
            // The expected values are described in the paper of Turaev
            // and Viro.
            // For the time being we will only use epsilon == -1 since
            // the expected value is easier to calculate.
            for (unsigned q0 = 2; q0 <= 4; q0++) {
                if (q0 == 3)
                    continue;

                double tv = t.turaevViro(3, q0);
                double expectedTV = 0.5;
                for (unsigned long i = 0; i < t.getHomologyH2Z2(); i++)
                    expectedTV += expectedTV;

                if (regina::isNonZero(tv - expectedTV)) {
                    std::ostringstream msg;
                    msg << "Turaev-Viro(" << triName << ", r = 3, root = "
                        << q0 << ") is " << tv << ", not " << expectedTV
                        << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyTVS3(unsigned r) {
            // Verify all Turaev-Viro invariants for S^3 for the given
            // value of r.
            // The expected values are described in the paper of Turaev
            // and Viro.
            for (unsigned q0 = 1; q0 < 2 * r; q0++) {
                if (regina::gcd(q0, r) > 1)
                    continue;

                double tv = s3.turaevViro(r, q0);

                double expectedTV = 2 * sin(M_PI * q0 / static_cast<double>(r));
                expectedTV = (expectedTV * expectedTV) /
                    (2 * static_cast<double>(r));

                if (regina::isNonZero(tv - expectedTV)) {
                    std::ostringstream msg;
                    msg << "Turaev-Viro(S^3, r = " << r << ", root = " << q0
                        << ") is " << tv << ", not " << expectedTV << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyTVRP3(unsigned r) {
            // Verify all Turaev-Viro invariants for RP^3 for the given
            // value of r.
            // The expected values are described in the paper of Turaev
            // and Viro.
            for (unsigned q0 = 1; q0 < 2 * r; q0++) {
                if (regina::gcd(q0, r) > 1)
                    continue;

                double tv = rp3.turaevViro(r, q0);

                double expectedTV;
                if (q0 % 2 == r % 2)
                    expectedTV = 0.0;
                else {
                    double real = cos(M_PI * q0 / static_cast<double>(r)) -
                        1.0;
                    double imag = sin(M_PI * q0 / static_cast<double>(r));
                    expectedTV = (real * real) + (imag * imag);
                    expectedTV /= static_cast<double>(r);
                }

                if (regina::isNonZero(tv - expectedTV)) {
                    std::ostringstream msg;
                    msg << "Turaev-Viro(RP^3, r = " << r << ", root = " << q0
                        << ") is " << tv << ", not " << expectedTV << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyTVL31(unsigned r) {
            // Verify all Turaev-Viro invariants for L(3,1) for the given
            // value of r.
            // The expected values are described in the paper of Turaev
            // and Viro.
            for (unsigned q0 = 1; q0 < 2 * r; q0++) {
                if (regina::gcd(q0, r) > 1)
                    continue;

                double tv = lens3_1.turaevViro(r, q0);

                unsigned q0pow = ((r - 2) / 3) + 1;
                double expectedTV = 2 * sin(M_PI * q0 * q0pow /
                    static_cast<double>(r));
                expectedTV = (expectedTV * expectedTV) /
                    (2 * static_cast<double>(r));

                if (regina::isNonZero(tv - expectedTV)) {
                    std::ostringstream msg;
                    msg << "Turaev-Viro(L(3,1), r = " << r << ", root = "
                        << q0 << ") is " << tv << ", not " << expectedTV
                        << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyTVS2xS1(unsigned r) {
            // Verify all Turaev-Viro invariants for S^2 x S^1 for the given
            // value of r.
            // The expected values are described in the paper of Turaev
            // and Viro.
            for (unsigned q0 = 1; q0 < 2 * r; q0++) {
                if (regina::gcd(q0, r) > 1)
                    continue;

                double tv = s2xs1.turaevViro(r, q0);

                double expectedTV = 1.0;

                if (regina::isNonZero(tv - expectedTV)) {
                    std::ostringstream msg;
                    msg << "Turaev-Viro(S^2 x S^1, r = " << r << ", root = "
                        << q0 << ") is " << tv << ", not " << expectedTV
                        << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void turaevViro() {
            verifyTV3(s3, "S^3");
            verifyTV3(s2xs1, "S^2 x S^1");
            verifyTV3(rp3, "RP^3");
            verifyTV3(lens3_1, "L(3,1)");
            verifyTV3(lens7_1_loop, "Layered loop L(7,1)");
            verifyTV3(lens8_3, "L(8,3)");
            verifyTV3(lens8_3_large, "Large L(8,3)");
            verifyTV3(rp3rp3, "RP^3 # RP^3");
            verifyTV3(q28, "S^3 / Q_28");
            verifyTV3(q32xz3, "S^3 / Q_32 x Z_3");
            verifyTV3(rp2xs1, "RP^2 x S^1");

            verifyTVS3(4); verifyTVS3(5); verifyTVS3(6);
            verifyTVS3(7); verifyTVS3(8);

            verifyTVRP3(4); verifyTVRP3(5); verifyTVRP3(6);
            verifyTVRP3(7); verifyTVRP3(8);

            verifyTVL31(4); verifyTVL31(5); verifyTVL31(6);
            verifyTVL31(7); verifyTVL31(8);

            verifyTVS2xS1(4); verifyTVS2xS1(5); verifyTVS2xS1(6);
            verifyTVS2xS1(7); verifyTVS2xS1(8);
        }

        void propertyUpdates() {
            // Begin with an empty triangulation and calculate various
            // properties.
            NTriangulation t;

            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not valid.",
                t.isValid());
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not orientable.",
                t.isOrientable());
            verifyGroup(t.getHomologyH1(),
                "H1(empty triangulation)", 0);
            verifyGroup(t.getHomologyH1Bdry(),
                "Boundary H1(empty triangulation)", 0);
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not "
                "0-efficient.", t.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation has a "
                "splitting surface.", ! t.hasSplittingSurface());
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation has a "
                "two-sphere boundary component.",
                ! t.hasTwoSphereBoundaryComponents());

            // Add a single tetrahedron.
            t.addTetrahedron(new NTetrahedron());

            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron is "
                "0-efficient.", ! t.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron has no "
                "splitting surface.", t.hasSplittingSurface());
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron has no "
                "two-sphere boundary components.",
                t.hasTwoSphereBoundaryComponents());

            // Glue the tetrahedron to itself to form a solid torus.
            t.getTetrahedron(0)->joinTo(0, t.getTetrahedron(0),
                NPerm(1, 2, 3, 0));
            t.gluingsHaveChanged();

            verifyGroup(t.getHomologyH1(),
                "H1(LST(1,2,3))", 1);
            verifyGroup(t.getHomologyH1Bdry(),
                "Boundary H1(LST(1,2,3))", 2);

            // Glue the remaining two faces in a non-orientable fashion.
            t.getTetrahedron(0)->joinTo(2, t.getTetrahedron(0),
                NPerm(1, 0, 3, 2));
            t.gluingsHaveChanged();

            CPPUNIT_ASSERT_MESSAGE("A bad 1-tetrahedron triangulation "
                "is valid.", ! t.isValid());
            CPPUNIT_ASSERT_MESSAGE("A bad 1-tetrahedron triangulation "
                "is orientable.", ! t.isOrientable());
        }
};

void addNTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NTriangulationTest::suite());
}

