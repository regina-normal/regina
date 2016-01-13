
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#define _USE_MATH_DEFINES // for M_PI, which is non-standard

#include <algorithm>
#include <cmath>
#include <sstream>
#include <vector>
#include <cppunit/extensions/HelperMacros.h>
#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "dim2/dim2triangulation.h"
#include "maths/approx.h"
#include "maths/nmatrixint.h"
#include "maths/numbertheory.h"
#include "maths/permconv.h"
#include "packet/ncontainer.h"
#include "split/nsignature.h"
#include "subcomplex/nstandardtri.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/nexampletriangulation.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nvertex.h"

#include "testsuite/exhaustive.h"
#include "testsuite/generic/generictriangulation.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::Dim2Triangulation;
using regina::NAbelianGroup;
using regina::NBoundaryComponent;
using regina::NComponent;
using regina::NExampleTriangulation;
using regina::NGroupPresentation;
using regina::NIsomorphism;
using regina::NNormalSurface;
using regina::NNormalSurfaceList;
using regina::NPerm4;
using regina::NSignature;
using regina::NStandardTriangulation;
using regina::NTetrahedron;
using regina::NTriangle;
using regina::NTriangulation;
using regina::NVertex;

class NTriangulationTest : public TriangulationTest<3> {
    CPPUNIT_TEST_SUITE(NTriangulationTest);

    // Generic tests:
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);

    // Dimension-specific tests:
    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(standardness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(boundaryComponents);
    CPPUNIT_TEST(boundaryTriangles);
    CPPUNIT_TEST(vertexLinksSpecific);
    CPPUNIT_TEST(vertexLinks);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(homologyH1Bdry);
    CPPUNIT_TEST(fundGroup);
    CPPUNIT_TEST(fundGroupVsH1);
    CPPUNIT_TEST(zeroEfficiency);
    CPPUNIT_TEST(irreducibility);
    CPPUNIT_TEST(threeSphereRecognition);
    CPPUNIT_TEST(threeSphereRecognitionLarge);
    CPPUNIT_TEST(threeBallRecognition);
    CPPUNIT_TEST(solidTorusRecognition);
    CPPUNIT_TEST(turaevViro);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(barycentricSubdivision);
    CPPUNIT_TEST(idealToFinite);
    CPPUNIT_TEST(finiteToIdeal);
    CPPUNIT_TEST(drillEdge);
    CPPUNIT_TEST(puncture);
    CPPUNIT_TEST(connectedSumWithSelf);
    CPPUNIT_TEST(dehydration);
    CPPUNIT_TEST(simplification);
    CPPUNIT_TEST(retriangulation);
    CPPUNIT_TEST(reordering);
    CPPUNIT_TEST(propertyUpdates);
    CPPUNIT_TEST(eltMove14);

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
        NTriangulation rp3_1;
            /**< A one-vertex triangulation of RP^3. */
        NTriangulation rp3_2;
            /**< A two-vertex triangulation of RP^3. */
        NTriangulation lens3_1;
            /**< A two-vertex lens space L(3,1). */
        NTriangulation lens8_3;
            /**< The layered lens space L(8,3). */
        NTriangulation lens7_1_loop;
            /**< An untwisted layered loop representing L(7,1). */
        NTriangulation rp3rp3;
            /**< The connected sum RP^3 # RP^3. */
        NTriangulation q32xz3;
            /**< The orbit manifold S^3 / Q_32 x Z_3. */
        NTriangulation q28;
            /**< A twisted layered loop representing S^3 / Q_28. */
        NTriangulation weberSeifert;
            /**< The Weber-Seifert dodecahedral space.  With 23 tetrahedra,
                 this is too large for running normal surface algorithms
                 here in the test suite. */

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
        NTriangulation pinchedSolidTorus;
            /**< A solid torus with one longitude pinched to a point. */
        NTriangulation pinchedSolidKB;
            /**< A solid Klein bottle with one longitude pinched to a point. */

        // Non-minimal:
        NTriangulation s3_large;
            /**< A non-minimal 3-sphere triangulation. */
        NTriangulation rp3_large;
            /**< The manifold RP^3. */
        NTriangulation lens8_3_large;
            /**< The lens space L(8,3). */
        NTriangulation q20_large;
            /**< The manifold S^3 / Q_20. */
        NTriangulation ball_large;
            /**< A four tetrahedron ball that used to cause crashes in
                 the simplification routines once upon a time. */
        NTriangulation ball_large_pillows;
            /**< A four tetrahedron ball formed by joining together two
                 triangular pillows. */
        NTriangulation ball_large_snapped;
            /**< A ball formed by joining together three snapped balls. */
        NTriangulation singleTet_bary;
            /**< The barycentric subdivision of a single tetrahedron. */
        NTriangulation fig8_bary;
            /**< The barycentric subdivision of the figure eight knot
                 complement. */

        // Disconnected triangulations:
        NTriangulation disjoint2;
            /**< A disjoint union of two triangulations. */
        NTriangulation disjoint3;
            /**< A disjoint union of three triangulations. */

    public:
        void copyAndDelete(NTriangulation& dest, NTriangulation* source) {
            dest.insertTriangulation(*source);
            dest.setPacketLabel(source->label());
            delete source;
        }

        void generateFromSig(NTriangulation& tri, const std::string& sigStr) {
            NSignature* sig = NSignature::parse(sigStr);
            if (sig == 0)
                return;

            NTriangulation* triNew = sig->triangulate();
            delete sig;
            if (triNew == 0)
                return;

            tri.insertTriangulation(*triNew);
            tri.setPacketLabel(sigStr);
            delete triNew;
        }

        NTriangulation* generateFromSig(const std::string& sigStr) {
            NSignature* sig = NSignature::parse(sigStr);
            if (sig == 0)
                return 0;

            NTriangulation* triNew = sig->triangulate();
            if (! triNew)
                return 0;

            triNew->setPacketLabel(sigStr);
            delete sig;
            return triNew;
        }

        void setUp() {
            // Begin with trivial cases.
            // The empty triangulation needs no initialisation whatsoever.
            empty.setPacketLabel("Empty triangulation");

            singleTet.newTetrahedron();
            singleTet.setPacketLabel("Single tetrahedron");

            // Some of our triangulations can be constructed automatically.
            s3.insertLayeredLensSpace(1, 0);
            s3.setPacketLabel("S^3");

            s2xs1.insertLayeredLensSpace(0, 1);
            s2xs1.setPacketLabel("S^2 x S^1");

            rp3_1.insertLayeredLensSpace(2, 1);
            rp3_1.setPacketLabel("RP^3 (1 vtx)");

            rp3_2.insertLayeredLoop(2, false);
            rp3_2.setPacketLabel("RP^3 (2 vtx)");

            lens8_3.insertLayeredLensSpace(8, 3);
            lens8_3.setPacketLabel("L(8,3)");

            lens100_1.insertLayeredLensSpace(100, 1);
            lens100_1.setPacketLabel("L(100,1)");

            lst3_4_7.insertLayeredSolidTorus(3, 4);
            lst3_4_7.setPacketLabel("LST(3,4,7)");

            q28.insertLayeredLoop(7, true);
            q28.setPacketLabel("S^3 / Q_28");

            lens7_1_loop.insertLayeredLoop(7, false);
            lens7_1_loop.setPacketLabel("Layered loop L(7,1)");

            // Some of our triangulations can be generated from
            // splitting surfaces.
            generateFromSig(rp3rp3, "aabccd.b.d");
            rp3rp3.setPacketLabel("RP^3 # RP^3");

            generateFromSig(q32xz3, "aabcdb.cedfef");
            q32xz3.setPacketLabel("S^3 / Q_32 x Z_3");

            generateFromSig(s3_large, "abc.abd.cef.de.fg.g");
            s3_large.setPacketLabel("Large S^3");

            generateFromSig(lens8_3_large, "aabcb.cd.d");
            lens8_3_large.setPacketLabel("Large L(8,3)");

            generateFromSig(rp3_large, "aabcdedcfb.fg.e.g");
            rp3_large.setPacketLabel("Large RP^3");

            generateFromSig(q20_large, "abcdeabcdef.fg.g");
            q20_large.setPacketLabel("Large S^3 / Q_20");

            // Some are hard-coded in the calculation engine as sample
            // triangulations.
            copyAndDelete(weberSeifert, NExampleTriangulation::weberSeifert());
            weberSeifert.setPacketLabel("Weber-Seifert");

            copyAndDelete(figure8,
                NExampleTriangulation::figureEight());
            figure8.setPacketLabel("Figure 8 knot complement");

            copyAndDelete(solidKB, NExampleTriangulation::solidKleinBottle());
            solidKB.setPacketLabel("Solid Klein bottle");

            copyAndDelete(rp2xs1, NExampleTriangulation::rp2xs1());
            rp2xs1.setPacketLabel("RP^2 x S^1");

            copyAndDelete(gieseking, NExampleTriangulation::gieseking());
            gieseking.setPacketLabel("Gieseking manifold");

            copyAndDelete(cuspedGenusTwoTorus,
                NExampleTriangulation::cuspedGenusTwoTorus());
            cuspedGenusTwoTorus.setPacketLabel("Cusped solid genus 2 torus");

            singleTet_bary.newTetrahedron();
            singleTet_bary.barycentricSubdivision();
            singleTet_bary.setPacketLabel("Subdivided tetrahedron");

            copyAndDelete(fig8_bary,
                NExampleTriangulation::figureEight());
            fig8_bary.barycentricSubdivision();
            fig8_bary.setPacketLabel("Subdivided figure 8 knot complement");

            // The rest alas must be done manually.
            NTetrahedron* r;
            NTetrahedron* s;
            NTetrahedron* t;
            NTetrahedron* u;

            // A two-tetrahedron two-vertex L(3,1) is straightforward to
            // construct using a vertex of degree two.
            r = lens3_1.newTetrahedron();
            s = lens3_1.newTetrahedron();
            r->joinTo(0, s, NPerm4(0, 2, 3, 1));
            r->joinTo(1, s, NPerm4());
            r->joinTo(2, s, NPerm4());
            r->joinTo(3, s, NPerm4());
            lens3_1.setPacketLabel("L(3,1)");

            // For a triangulation with invalid edges, we simply fold
            // the faces of a tetrahedron together in pairs (as in a
            // 3-sphere triangulation) but apply a reflection to each fold.
            r = invalidEdges.newTetrahedron();
            r->joinTo(0, r, NPerm4(1, 0, 3, 2));
            r->joinTo(2, r, NPerm4(1, 0, 3, 2));
            invalidEdges.setPacketLabel("Triangulation with invalid edges");

            twoProjPlaneCusps.insertTriangulation(invalidEdges);
            twoProjPlaneCusps.barycentricSubdivision();
            twoProjPlaneCusps.setPacketLabel("Triangulation with RP^2 cusps");

            // To construct a solid torus with a pinched longitude, we
            // identify two opposite faces of a square pyramid.
            r = pinchedSolidTorus.newTetrahedron();
            s = pinchedSolidTorus.newTetrahedron();
            r->joinTo(3, s, NPerm4(0, 1, 2, 3));
            r->joinTo(2, s, NPerm4(0, 3, 1, 2));
            pinchedSolidTorus.setPacketLabel("Pinched solid torus");

            // The pinched solid Klein bottle is much the same, except
            // for a twist before the opposite faces are identified.
            r = pinchedSolidKB.newTetrahedron();
            s = pinchedSolidKB.newTetrahedron();
            r->joinTo(3, s, NPerm4(0, 1, 2, 3));
            r->joinTo(2, s, NPerm4(0, 2, 1, 3));
            pinchedSolidKB.setPacketLabel("Pinched solid Klein bottle");

            // This ball used to cause a crash once upon a time.
            // Throw it into the test suite for good measure.
            r = ball_large.newTetrahedron();
            s = ball_large.newTetrahedron();
            t = ball_large.newTetrahedron();
            u = ball_large.newTetrahedron();
            r->joinTo(2, r, NPerm4(0,2));
            r->joinTo(1, s, NPerm4(2,0,1,3));
            s->joinTo(2, t, NPerm4());
            s->joinTo(1, t, NPerm4(2,0,1,3));
            t->joinTo(1, u, NPerm4(2,0,1,3));
            u->joinTo(2, u, NPerm4(1,2));
            ball_large.setPacketLabel("4-tetrahedron ball");

            // Make two triangular pillows, then join them together.
            // This crashed with 2-0 vertex moves once upon a time.
            r = ball_large_pillows.newTetrahedron();
            s = ball_large_pillows.newTetrahedron();
            t = ball_large_pillows.newTetrahedron();
            u = ball_large_pillows.newTetrahedron();
            r->joinTo(0, s, NPerm4());
            r->joinTo(1, s, NPerm4());
            r->joinTo(2, s, NPerm4());
            t->joinTo(0, u, NPerm4());
            t->joinTo(1, u, NPerm4());
            t->joinTo(2, u, NPerm4());
            r->joinTo(3, t, NPerm4());
            ball_large_pillows.setPacketLabel("4-tetrahedron pillow ball");

            // Make three snapped balls and join them together.
            r = ball_large_snapped.newTetrahedron();
            s = ball_large_snapped.newTetrahedron();
            t = ball_large_snapped.newTetrahedron();
            r->joinTo(2, r, NPerm4(2, 3));
            s->joinTo(2, s, NPerm4(2, 3));
            t->joinTo(2, t, NPerm4(2, 1));
            r->joinTo(1, s, NPerm4());
            s->joinTo(0, t, NPerm4());
            ball_large_snapped.setPacketLabel("3-tetrahedron snapped ball");

            // Build disconnected triangulations from others that we
            // already have.
            disjoint2.insertTriangulation(gieseking);
            disjoint2.insertTriangulation(cuspedGenusTwoTorus);
            disjoint2.setPacketLabel("Gieseking U (cusped genus 2 torus)");

            disjoint3.insertTriangulation(s2xs1);
            disjoint3.insertTriangulation(ball_large_pillows);
            disjoint3.insertTriangulation(figure8);
            disjoint3.setPacketLabel("(S^2 x S^1) U (B^3) U "
                "(Figure eight knot complement)");
        }

        void tearDown() {
        }

        /**
         * Run a given test over all hand-coded test cases that are not
         * obscenely large.
         */
        void testManualSmall(NTriangulationTestFunction f) {
            f(&empty);
            f(&singleTet);
            f(&s3);
            f(&s2xs1);
            f(&rp3_1);
            f(&rp3_2);
            f(&lens3_1);
            f(&lens8_3);
            f(&lens7_1_loop);
            f(&rp3rp3);
            f(&q32xz3);
            f(&q28);
            f(&weberSeifert);
            f(&lst3_4_7);
            f(&figure8);
            f(&rp2xs1);
            f(&solidKB);
            f(&gieseking);
            f(&invalidEdges);
            f(&twoProjPlaneCusps);
            f(&cuspedGenusTwoTorus);
            f(&pinchedSolidTorus);
            f(&pinchedSolidKB);
            f(&s3_large);
            f(&rp3_large);
            f(&lens8_3_large);
            f(&q20_large);
            f(&ball_large);
            f(&ball_large_pillows);
            f(&ball_large_snapped);
            f(&singleTet_bary);
            f(&fig8_bary);
            f(&disjoint2);
            f(&disjoint3);
        }

        /**
         * Run a given test over all hand-coded test cases.
         */
        void testManualAll(NTriangulationTestFunction f) {
            testManualSmall(f);
            f(&lens100_1);
        }

        void makeCanonical() {
            testManualAll(verifyMakeCanonical);
        }

        void isomorphismSignature() {
            testManualAll(verifyIsomorphismSignature);
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
            CPPUNIT_ASSERT_MESSAGE("RP^3 (1 vtx) is not valid.",
                rp3_1.isValid());
            CPPUNIT_ASSERT_MESSAGE("RP^3 (2 vtx) is not valid.",
                rp3_2.isValid());
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
            CPPUNIT_ASSERT_MESSAGE(
                "The Weber-Seifert dodecahedral space is not valid.",
                weberSeifert.isValid());
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_32 x Z_3 is not valid.",
                q32xz3.isValid());
            CPPUNIT_ASSERT_MESSAGE("L(100,1) is not valid.",
                lens100_1.isValid());
            CPPUNIT_ASSERT_MESSAGE("The 4-tetrahedron ball is not valid.",
                ball_large.isValid());
            CPPUNIT_ASSERT_MESSAGE(
                "The 4-tetrahedron pillow ball is not valid.",
                ball_large_pillows.isValid());
            CPPUNIT_ASSERT_MESSAGE(
                "The 3-tetrahedron snapped ball is not valid.",
                ball_large_snapped.isValid());
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
            CPPUNIT_ASSERT_MESSAGE("The pinched solid torus "
                "is reported as valid.",
                ! pinchedSolidTorus.isValid());
            CPPUNIT_ASSERT_MESSAGE("The pinched solid Klein bottle "
                "is reported as valid.",
                ! pinchedSolidKB.isValid());
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
            CPPUNIT_ASSERT_MESSAGE("RP^3 (1 vtx) is not standard.",
                rp3_1.isStandard());
            CPPUNIT_ASSERT_MESSAGE("RP^3 (2 vtx) is not standard.",
                rp3_2.isStandard());
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
            CPPUNIT_ASSERT_MESSAGE(
                "The Weber-Seifert dodecahedral space is not standard.",
                weberSeifert.isStandard());
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_32 x Z_3 is not standard.",
                q32xz3.isStandard());
            CPPUNIT_ASSERT_MESSAGE("L(100,1) is not standard.",
                lens100_1.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The 4-tetrahedron ball is not standard.",
                ball_large.isStandard());
            CPPUNIT_ASSERT_MESSAGE(
                "The 4-tetrahedron pillow ball is not standard.",
                ball_large_pillows.isStandard());
            CPPUNIT_ASSERT_MESSAGE(
                "The 3-tetrahedron snapped ball is not standard.",
                ball_large_snapped.isStandard());
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
            CPPUNIT_ASSERT_MESSAGE("The pinched solid torus is standard.",
                ! pinchedSolidTorus.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The pinched solid Klein bottle "
                "is standard.",
                ! pinchedSolidKB.isStandard());
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
            CPPUNIT_ASSERT_MESSAGE("RP^3 (1 vtx) is not orientable.",
                rp3_1.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("RP^3 (2 vtx) is not orientable.",
                rp3_2.isOrientable());
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
            CPPUNIT_ASSERT_MESSAGE(
                "The Weber-Seifert dodecahedral space is not orientable.",
                weberSeifert.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_32 x Z_3 is not orientable.",
                q32xz3.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("L(100,1) is not orientable.",
                lens100_1.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("The 4-tetrahedron ball is not orientable.",
                ball_large.isOrientable());
            CPPUNIT_ASSERT_MESSAGE(
                "The 4-tetrahedron pillow ball is not orientable.",
                ball_large_pillows.isOrientable());
            CPPUNIT_ASSERT_MESSAGE(
                "The 3-tetrahedron snapped ball is not orientable.",
                ball_large_snapped.isOrientable());
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
            CPPUNIT_ASSERT_MESSAGE("The pinched solid torus is not orientable.",
                pinchedSolidTorus.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("The pinched solid Klein bottle "
                "is orientable.",
                ! pinchedSolidKB.isOrientable());
        }

        void boundaryComponents() {
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation has boundary "
                "components.", empty.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron has no boundary "
                "components.", singleTet.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("S^3 has boundary components.",
                s3.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("S^2 x S^1 has boundary components.",
                s2xs1.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("RP^3 (1 vtx) has boundary components.",
                rp3_1.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("RP^3 (2 vtx) has boundary components.",
                rp3_2.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("L(3,1) has boundary components.",
                lens3_1.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("Layered loop L(7,1) "
                "has boundary components.",
                lens7_1_loop.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("L(8,3) has boundary components.",
                lens8_3.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) has boundary components.",
                lens8_3_large.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 has boundary components.",
                rp3rp3.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_28 has boundary components.",
                q28.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE(
                "The Weber-Seifert dodecahedral space has boundary components.",
                weberSeifert.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_32 x Z_3 has boundary components.",
                q32xz3.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("L(100,1) has boundary components.",
                lens100_1.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("The 4-tetrahedron ball has no boundary "
                "components.", ball_large.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The 4-tetrahedron pillow ball has no "
                "boundary components.",
                ball_large_pillows.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The 3-tetrahedron snapped ball has no "
                "boundary components.",
                ball_large_snapped.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("LST(3,4,7) has no boundary components.",
                lst3_4_7.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The figure eight knot complement "
                "has no boundary components.",
                figure8.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 has boundary components.",
                rp2xs1.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle "
                "has no boundary components.",
                solidKB.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The Gieseking manifold "
                "has no boundary components.",
                gieseking.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The triangulation with invalid edges "
                "has boundary components.",
                invalidEdges.countBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("The triangulation with projective plane "
                "cusps has no boundary components.",
                twoProjPlaneCusps.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The cusped solid genus two torus "
                "has no boundary components.",
                cuspedGenusTwoTorus.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The pinched solid torus "
                "has no boundary components.",
                pinchedSolidTorus.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The pinched solid torus "
                "has too many boundary components.",
                pinchedSolidTorus.countBoundaryComponents() < 2);
            CPPUNIT_ASSERT_MESSAGE("The pinched solid Klein bottle "
                "has no boundary components.",
                pinchedSolidKB.countBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The pinched solid Klein bottle "
                "has too many boundary components.",
                pinchedSolidKB.countBoundaryComponents() < 2);

            // TODO: Test the individual boundary components.
            // TODO: Check that nobody has too many boundary components.
        }

        static void verifyBoundaryTriangles(NTriangulation* tri) {
            unsigned long found = 0;

            unsigned long i, j;
            for (i = 0; i < tri->size(); ++i)
                for (j = 0; j < 4; ++j)
                    if (! tri->getTetrahedron(i)->adjacentTetrahedron(j))
                        ++found;

            if (found != tri->countBoundaryTriangles()) {
                std::ostringstream msg;
                msg << tri->label()
                    << " reports the wrong number of boundary triangles.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned long c;
            regina::NComponent* comp;
            for (c = 0; c < tri->countComponents(); ++c) {
                comp = tri->component(c);
                found = 0;

                for (i = 0; i < comp->size(); ++i)
                    for (j = 0; j < 4; ++j)
                        if (! comp->getTetrahedron(i)->adjacentTetrahedron(j))
                            ++found;

                if (found != comp->countBoundaryTriangles()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << " reports the wrong number of "
                        "boundary triangles in component " << c << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void boundaryTriangles() {
            testManualAll(verifyBoundaryTriangles);
        }

        void verifyVertexCount(NTriangulation& tri, unsigned nVertices,
                const char* triName) {
            if (tri.countVertices() != nVertices) {
                std::ostringstream msg;
                msg << triName << " has " << tri.countVertices()
                    << " vertices, not " << nVertices << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyVertexDisc(NTriangulation& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            NVertex* v = tri.getVertex(vertex);

            if (v->link() != NVertex::DISC) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not listed as DISC.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isLinkClosed()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is closed.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isIdeal()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is ideal.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isBoundary()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " is not a boundary vertex.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isStandard()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is non-standard.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isLinkOrientable()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is non-orientable.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->linkEulerChar() != 1) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link has Euler characteristic "
                    << v->linkEulerChar() << ", not 1.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyVertexSphere(NTriangulation& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            NVertex* v = tri.getVertex(vertex);

            if (v->link() != NVertex::SPHERE) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not listed as SPHERE.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isLinkClosed()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not closed.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isIdeal()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is ideal.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isBoundary()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " is a boundary vertex.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isStandard()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is non-standard.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isLinkOrientable()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is non-orientable.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->linkEulerChar() != 2) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link has Euler characteristic "
                    << v->linkEulerChar() << ", not 2.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyVertexTorus(NTriangulation& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            NVertex* v = tri.getVertex(vertex);

            if (v->link() != NVertex::TORUS) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not listed as TORUS.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isLinkClosed()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not closed.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isIdeal()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not ideal.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isBoundary()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " is not a boundary vertex.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isStandard()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is non-standard.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isLinkOrientable()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is non-orientable.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->linkEulerChar() != 0) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link has Euler characteristic "
                    << v->linkEulerChar() << ", not 0.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyVertexKB(NTriangulation& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            NVertex* v = tri.getVertex(vertex);

            if (v->link() != NVertex::KLEIN_BOTTLE) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not listed as KLEIN_BOTTLE.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isLinkClosed()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not closed.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isIdeal()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not ideal.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isBoundary()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " is not a boundary vertex.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isStandard()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is non-standard.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isLinkOrientable()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is orientable.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->linkEulerChar() != 0) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link has Euler characteristic "
                    << v->linkEulerChar() << ", not 0.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyVertexTorusG2(NTriangulation& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            NVertex* v = tri.getVertex(vertex);

            if (v->link() != NVertex::NON_STANDARD_CUSP) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not listed as NON_STANDARD_CUSP.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isLinkClosed()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not closed.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isIdeal()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not ideal.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isBoundary()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " is not a boundary vertex.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isStandard()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is standard.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isLinkOrientable()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is non-orientable.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->linkEulerChar() != -2) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link has Euler characteristic "
                    << v->linkEulerChar() << ", not -2.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyVertexProjPlane(NTriangulation& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            NVertex* v = tri.getVertex(vertex);

            if (v->link() != NVertex::NON_STANDARD_CUSP) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not listed as NON_STANDARD_CUSP.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isLinkClosed()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not closed.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isIdeal()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not ideal.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isBoundary()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " is not a boundary vertex.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isStandard()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is standard.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isLinkOrientable()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is orientable.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->linkEulerChar() != 1) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link has Euler characteristic "
                    << v->linkEulerChar() << ", not 1.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyVertexAnnulus(NTriangulation& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            NVertex* v = tri.getVertex(vertex);

            if (v->link() != NVertex::NON_STANDARD_BDRY) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not listed as NON_STANDARD_BDRY.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isLinkClosed()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is closed.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isIdeal()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is ideal.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isBoundary()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " is not a boundary vertex.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isStandard()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is standard.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isLinkOrientable()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is non-orientable.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->linkEulerChar() != 0) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link has Euler characteristic "
                    << v->linkEulerChar() << ", not 0.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyVertexMobius(NTriangulation& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            NVertex* v = tri.getVertex(vertex);

            if (v->link() != NVertex::NON_STANDARD_BDRY) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not listed as NON_STANDARD_BDRY.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isLinkClosed()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is closed.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isIdeal()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is ideal.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! v->isBoundary()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " is not a boundary vertex.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isStandard()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is standard.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->isLinkOrientable()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is orientable.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v->linkEulerChar() != 0) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link has Euler characteristic "
                    << v->linkEulerChar() << ", not 0.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void vertexLinksSpecific() {
            verifyVertexCount(singleTet, 4, "Single tetrahedron");
            verifyVertexDisc(singleTet, 0, "Single tetrahedron");
            verifyVertexDisc(singleTet, 1, "Single tetrahedron");
            verifyVertexDisc(singleTet, 2, "Single tetrahedron");
            verifyVertexDisc(singleTet, 3, "Single tetrahedron");

            verifyVertexCount(s3, 1, "S^3");
            verifyVertexSphere(s3, 0, "S^3");

            verifyVertexCount(s2xs1, 1, "S^2 x S^1");
            verifyVertexSphere(s2xs1, 0, "S^2 x S^1");

            verifyVertexCount(rp3_1, 1, "RP^3 (1 vtx)");
            verifyVertexSphere(rp3_1, 0, "RP^3 (1 vtx)");

            verifyVertexCount(rp3_2, 2, "RP^3 (2 vtx)");
            verifyVertexSphere(rp3_2, 0, "RP^3 (2 vtx)");
            verifyVertexSphere(rp3_2, 1, "RP^3 (2 vtx)");

            verifyVertexCount(lens3_1, 2, "L(3,1)");
            verifyVertexSphere(lens3_1, 0, "L(3,1)");
            verifyVertexSphere(lens3_1, 1, "L(3,1)");

            verifyVertexCount(lens8_3, 1, "L(8,3)");
            verifyVertexSphere(lens8_3, 0, "L(8,3)");

            verifyVertexCount(lens8_3_large, 1, "Large L(8,3)");
            verifyVertexSphere(lens8_3_large, 0, "Large L(8,3)");

            verifyVertexCount(lens7_1_loop, 2, "Layered loop L(7,1)");
            verifyVertexSphere(lens7_1_loop, 0, "Layered loop L(7,1)");
            verifyVertexSphere(lens7_1_loop, 1, "Layered loop L(7,1)");

            verifyVertexCount(rp3rp3, 1, "RP^3 # RP^3");
            verifyVertexSphere(rp3rp3, 0, "RP^3 # RP^3");

            verifyVertexCount(q32xz3, 1, "S^3 / Q_32 x Z_3");
            verifyVertexSphere(q32xz3, 0, "S^3 / Q_32 x Z_3");

            verifyVertexCount(q28, 1, "S^3 / Q_28");
            verifyVertexSphere(q28, 0, "S^3 / Q_28");

            verifyVertexCount(weberSeifert, 1,
                "Weber-Seifert dodecahedral space");
            verifyVertexSphere(weberSeifert, 0,
                "Weber-Seifert dodecahedral space");

            verifyVertexCount(lens100_1, 1, "L(100,1)");
            verifyVertexSphere(lens100_1, 0, "L(100,1)");

            verifyVertexCount(ball_large, 5, "4-tetrahedron ball");
            verifyVertexDisc(ball_large, 0, "4-tetrahedron ball");
            verifyVertexDisc(ball_large, 1, "4-tetrahedron ball");
            verifyVertexSphere(ball_large, 2, "4-tetrahedron ball");
            verifyVertexDisc(ball_large, 3, "4-tetrahedron ball");
            verifyVertexDisc(ball_large, 4, "4-tetrahedron ball");

            verifyVertexCount(ball_large_pillows, 5,
                "4-tetrahedron pillow ball");
            verifyVertexDisc(ball_large_pillows, 0,
                "4-tetrahedron pillow ball");
            verifyVertexDisc(ball_large_pillows, 1,
                "4-tetrahedron pillow ball");
            verifyVertexDisc(ball_large_pillows, 2,
                "4-tetrahedron pillow ball");
            verifyVertexSphere(ball_large_pillows, 3,
                "4-tetrahedron pillow ball");
            verifyVertexSphere(ball_large_pillows, 4,
                "4-tetrahedron pillow ball");

            verifyVertexCount(ball_large_snapped, 4,
                "3-tetrahedron snapped ball");
            verifyVertexSphere(ball_large_snapped, 0,
                "3-tetrahedron snapped ball");
            verifyVertexDisc(ball_large_snapped, 1,
                "3-tetrahedron snapped ball");
            verifyVertexDisc(ball_large_snapped, 2,
                "3-tetrahedron snapped ball");
            verifyVertexDisc(ball_large_snapped, 3,
                "3-tetrahedron snapped ball");

            verifyVertexCount(lst3_4_7, 1, "LST(3,4,7)");
            verifyVertexDisc(lst3_4_7, 0, "LST(3,4,7)");

            verifyVertexCount(figure8, 1, "Figure eight knot complement");
            verifyVertexTorus(figure8, 0, "Figure eight knot complement");

            verifyVertexCount(rp2xs1, 1, "RP^2 x S^1");
            verifyVertexSphere(rp2xs1, 0, "RP^2 x S^1");

            verifyVertexCount(solidKB, 2, "Solid Klein bottle");
            verifyVertexDisc(solidKB, 0, "Solid Klein bottle");
            verifyVertexDisc(solidKB, 1, "Solid Klein bottle");

            verifyVertexCount(gieseking, 1, "Gieseking manifold");
            verifyVertexKB(gieseking, 0, "Gieseking manifold");

            verifyVertexCount(invalidEdges, 2,
                "Triangulation with invalid edges");
            verifyVertexSphere(invalidEdges, 0,
                "Triangulation with invalid edges");
            verifyVertexSphere(invalidEdges, 1,
                "Triangulation with invalid edges");

            verifyVertexCount(twoProjPlaneCusps, 9,
                "Triangulation with RP^2 cusps");
            verifyVertexSphere(twoProjPlaneCusps, 0,
                "Triangulation with RP^2 cusps");
            verifyVertexSphere(twoProjPlaneCusps, 1,
                "Triangulation with RP^2 cusps");
            verifyVertexProjPlane(twoProjPlaneCusps, 2,
                "Triangulation with RP^2 cusps");
            verifyVertexSphere(twoProjPlaneCusps, 3,
                "Triangulation with RP^2 cusps");
            verifyVertexSphere(twoProjPlaneCusps, 4,
                "Triangulation with RP^2 cusps");
            verifyVertexSphere(twoProjPlaneCusps, 5,
                "Triangulation with RP^2 cusps");
            verifyVertexSphere(twoProjPlaneCusps, 6,
                "Triangulation with RP^2 cusps");
            verifyVertexSphere(twoProjPlaneCusps, 7,
                "Triangulation with RP^2 cusps");
            verifyVertexProjPlane(twoProjPlaneCusps, 8,
                "Triangulation with RP^2 cusps");

            verifyVertexCount(cuspedGenusTwoTorus, 2,
                "Cusped solid genus two torus");
            verifyVertexSphere(cuspedGenusTwoTorus, 0,
                "Cusped solid genus two torus");
            verifyVertexTorusG2(cuspedGenusTwoTorus, 1,
                "Cusped solid genus two torus");

            verifyVertexCount(pinchedSolidTorus, 3,
                "Pinched solid torus");
            verifyVertexAnnulus(pinchedSolidTorus, 0,
                "Pinched solid torus");
            verifyVertexDisc(pinchedSolidTorus, 1,
                "Pinched solid torus");
            verifyVertexDisc(pinchedSolidTorus, 2,
                "Pinched solid torus");

            verifyVertexCount(pinchedSolidKB, 3,
                "Pinched solid Klein bottle");
            verifyVertexMobius(pinchedSolidKB, 0,
                "Pinched solid Klein bottle");
            verifyVertexDisc(pinchedSolidKB, 1,
                "Pinched solid Klein bottle");
            verifyVertexDisc(pinchedSolidKB, 2,
                "Pinched solid Klein bottle");
        }

        static void verifyVertexLinks(NTriangulation* tri) {
            for (unsigned long i = 0; i < tri->countVertices(); ++i) {
                NVertex* v = tri->getVertex(i);
                NIsomorphism* iso;

                const Dim2Triangulation* link = v->buildLink();
                Dim2Triangulation* link2 = v->buildLinkDetail(true, &iso);

                if (link->size() != v->getDegree()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", vertex " << i << ": "
                        << "link has incorrect number of triangles.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! link2->isIdenticalTo(*link)) {
                    std::ostringstream msg;
                    msg << tri->label() << ", vertex " << i << ": "
                        << "variants of buildLink() give different results.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! link->isConnected()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", vertex " << i << ": "
                        << "link of vertex is not connected.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (v->isIdeal()) {
                    // Vertex link should be closed but not a sphere.
                    if (! link->isClosed()) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link of ideal vertex is not a closed surface.";
                        CPPUNIT_FAIL(msg.str());
                    } else if (link->eulerChar() == 2) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link of ideal vertex is a sphere.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else if (! v->isStandard()) {
                    // Vertex link should have boundary and not be a disc.
                    if (link->isClosed()) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link of invalid vertex is closed.";
                        CPPUNIT_FAIL(msg.str());
                    } else if (link->eulerChar() == 1) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link of invalid vertex is a disc.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else if (! v->isBoundary()) {
                    // Vertex link should be a sphere.
                    if (! link->isClosed()) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link of internal vertex is not closed.";
                        CPPUNIT_FAIL(msg.str());
                    } else if (link->eulerChar() != 2) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link of internal vertex is not a sphere.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else {
                    // Vertex link should be a disc.
                    if (link->isClosed() || link->eulerChar() != 1) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link of real boundary vertex is not a disc.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                unsigned j, k;
                NTetrahedron* tet;
                NPerm4 perm;
                const regina::Dim2Triangle *t, *adj;
                unsigned vNum;
                for (j = 0; j < v->getDegree(); ++j) {
                    tet = tri->getTetrahedron(iso->tetImage(j));
                    perm = iso->facePerm(j);
                    vNum = perm[3];
                    if (tet->getVertex(vNum) != v) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link does not map 3 -> vertex correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (perm[0] != tet->getTriangleMapping(vNum)[0] ||
                            perm[1] != tet->getTriangleMapping(vNum)[1] ||
                            perm[2] != tet->getTriangleMapping(vNum)[2]) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link does not map 0,1,2 -> opposite "
                            "triangle correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    for (k = 0; k < 3; ++k) {
                        t = link->getTriangle(j);
                        adj = t->adjacentTriangle(k);
                        if (adj) {
                            if (! tet->adjacentTetrahedron(perm[k])) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", vertex " << i << ": "
                                    << "link has extra adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            } else if (tet->adjacentTetrahedron(perm[k]) !=
                                    tri->getTetrahedron(iso->tetImage(
                                    link->triangleIndex(adj)))) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", vertex " << i << ": "
                                    << "link has wrong adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            } else if (tet->adjacentGluing(perm[k]) !=
                                    iso->facetPerm(link->triangleIndex(adj)) *
                                    perm3to4(t->adjacentGluing(k)) *
                                    perm.inverse()) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", vertex " << i << ": "
                                    << "link has wrong adjacent gluing.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        } else {
                            if (tet->adjacentTetrahedron(perm[k])) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", vertex " << i << ": "
                                    << "link missing adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        }
                    }
                }

                delete link2;
                delete iso;
            }
        }

        void vertexLinks() {
            testManualAll(verifyVertexLinks);
            runCensusAllClosed(verifyVertexLinks);
            runCensusAllBounded(verifyVertexLinks);
            runCensusAllIdeal(verifyVertexLinks);
        }

        void verifyEuler(const NTriangulation& tri,
                long expectedManifold, long expectedTri, const char* triName) {
            long eulerManifold = tri.eulerCharManifold();
            long eulerTri = tri.eulerCharTri();

            if (eulerManifold != expectedManifold) {
                std::ostringstream msg;
                msg << triName << " gives a manifold Euler characteristic of "
                    << eulerManifold << " instead of "
                    << expectedManifold << ".";
                CPPUNIT_FAIL(msg.str());
            }
            if (eulerTri != expectedTri) {
                std::ostringstream msg;
                msg << triName
                    << " gives a triangulation Euler characteristic of "
                    << eulerTri << " instead of " << expectedTri << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void eulerChar() {
            verifyEuler(empty, 0, 0, "Empty triangulation");
            verifyEuler(singleTet, 1, 1, "Single tetrahedron");
            verifyEuler(s3, 0, 0, "S^3");
            verifyEuler(s2xs1, 0, 0, "S^2 x S^1");
            verifyEuler(rp3_1, 0, 0, "RP^3 (1 vtx)");
            verifyEuler(rp3_2, 0, 0, "RP^3 (2 vtx)");
            verifyEuler(lens3_1, 0, 0, "L(3,1)");
            verifyEuler(lens8_3, 0, 0, "L(8,3)");
            verifyEuler(lens7_1_loop, 0, 0, "Layered loop L(7,1)");
            verifyEuler(rp3rp3, 0, 0, "RP^3 # RP^3");
            verifyEuler(q32xz3, 0, 0, "S^3 / Q_32 x Z_3");
            verifyEuler(q28, 0, 0, "S^3 / Q_28");
            verifyEuler(weberSeifert, 0, 0, "Weber-Seifert dodecahedral space");
            verifyEuler(lens100_1, 0, 0, "L(100,1)");
            verifyEuler(lst3_4_7, 0, 0, "LST(3,4,7)");
            verifyEuler(figure8, 0, 1, "Figure eight knot complement");
            verifyEuler(rp2xs1, 0, 0, "RP^2 x S^1");
            verifyEuler(solidKB, 0, 0, "Solid Klein bottle");
            verifyEuler(gieseking, 0, 1, "Gieseking manifold");

            verifyEuler(invalidEdges, 1, -1,
                "Triangulation with invalid edges");
            verifyEuler(twoProjPlaneCusps, 1, 1,
                "Triangulation with RP^2 cusps");
            verifyEuler(cuspedGenusTwoTorus, -1, 2,
                "Cusped solid genus two torus");
            verifyEuler(pinchedSolidTorus, 0, 1, "Pinched solid torus");
            verifyEuler(pinchedSolidKB, 0, 1, "Pinched solid Klein bottle");

            {
                NTriangulation t(twoProjPlaneCusps);
                t.idealToFinite();
                verifyEuler(t, 1, 1, "Triangulation with RP^2 boundaries");
            }
            {
                NTriangulation t(cuspedGenusTwoTorus);
                t.idealToFinite();
                verifyEuler(t, -1, -1, "Solid genus two torus");
            }
            {
                NTriangulation t(pinchedSolidTorus);
                t.idealToFinite();
                verifyEuler(t, 0, 0, "Unpinched solid torus");
            }
            {
                NTriangulation t(pinchedSolidKB);
                t.idealToFinite();
                verifyEuler(t, 0, 0, "Unpinched solid Klein bottle");
            }

            verifyEuler(s3_large, 0, 0, "Large S^3");
            verifyEuler(rp3_large, 0, 0, "Large RP^3");
            verifyEuler(lens8_3_large, 0, 0, "Large L(8,3)");
            verifyEuler(q20_large, 0, 0, "Large S^3 / Q_20");
            verifyEuler(ball_large, 1, 1, "4-tetrahedron ball");
            verifyEuler(ball_large_pillows, 1, 1, "4-tetrahedron pillow ball");
            verifyEuler(ball_large_snapped, 1, 1, "3-tetrahedron snapped ball");
            verifyEuler(fig8_bary, 0, 1, "Large figure eight knot complement");
        }

        void verifyGroup(const NAbelianGroup& g, const std::string& grpName,
                unsigned rank) {
            // Construct the error message.
            std::ostringstream msg;
            msg << grpName << " is " << g.str() << ", not ";
            if (rank == 0)
                msg << "0";
            else if (rank == 1)
                msg << "Z";
            else
                msg << rank << " Z";
            msg << '.';

            // Check the group.
            if (g.rank() != rank)
                CPPUNIT_FAIL(msg.str());
            else if (g.countInvariantFactors() != 0)
                CPPUNIT_FAIL(msg.str());
        }

        void verifyGroup(const NAbelianGroup& g, const std::string& grpName,
                unsigned rank, unsigned long torsionDegree) {
            // Construct the error message.
            std::ostringstream msg;
            msg << grpName << " is " << g.str() << ", not ";
            if (rank == 1)
                msg << "Z + ";
            else if (rank > 1)
                msg << rank << " Z + ";
            msg << "Z_" << torsionDegree << '.';

            // Check the group.
            if (g.rank() != rank)
                CPPUNIT_FAIL(msg.str());
            else {
                // Ranks match.
                if (g.countInvariantFactors() != 1 ||
                        g.getInvariantFactor(0) != torsionDegree)
                    CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyGroup(const NAbelianGroup& g, const std::string& grpName,
                unsigned rank, unsigned long torsionDegree1,
                unsigned long torsionDegree2) {
            // Construct the error message.
            std::ostringstream msg;
            msg << grpName << " is " << g.str() << ", not ";
            if (rank == 1)
                msg << "Z + ";
            else if (rank > 1)
                msg << rank << " Z + ";
            msg << "Z_" << torsionDegree1 << " + Z_" << torsionDegree2 << '.';

            // Check the group.
            if (g.rank() != rank)
                CPPUNIT_FAIL(msg.str());
            else {
                // Ranks match.
                if (g.countInvariantFactors() != 2 ||
                        g.getInvariantFactor(0) != torsionDegree1 ||
                        g.getInvariantFactor(1) != torsionDegree2)
                    CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyGroup(const NAbelianGroup& g, const std::string& grpName,
                unsigned rank, unsigned long torsionDegree1,
                unsigned long torsionDegree2, unsigned long torsionDegree3) {
            // Construct the error message.
            std::ostringstream msg;
            msg << grpName << " is " << g.str() << ", not ";
            if (rank == 1)
                msg << "Z + ";
            else if (rank > 1)
                msg << rank << " Z + ";
            msg << "Z_" << torsionDegree1 << " + Z_" << torsionDegree2
                << " + Z_" << torsionDegree3 << '.';

            // Check the group.
            if (g.rank() != rank)
                CPPUNIT_FAIL(msg.str());
            else {
                // Ranks match.
                if (g.countInvariantFactors() != 3 ||
                        g.getInvariantFactor(0) != torsionDegree1 ||
                        g.getInvariantFactor(1) != torsionDegree2 ||
                        g.getInvariantFactor(2) != torsionDegree3)
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
            verifyGroup(rp3_1.getHomologyH1(),
                "H1(RP^3, 1 vtx)", 0, 2);
            verifyGroup(rp3_2.getHomologyH1(),
                "H1(RP^3, 2 vtx)", 0, 2);
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
            verifyGroup(weberSeifert.getHomologyH1(),
                "H1(SeifertWeber)", 0, 5, 5, 5);
            verifyGroup(q32xz3.getHomologyH1(),
                "H1(S^3 / Q_32 x Z_3)", 0, 2, 6);
            verifyGroup(lens100_1.getHomologyH1(),
                "H1(L(100,1))", 0, 100);
            verifyGroup(ball_large.getHomologyH1(),
                "H1(4-tetrahedron ball)", 0);
            verifyGroup(ball_large_pillows.getHomologyH1(),
                "H1(4-tetrahedron pillow ball)", 0);
            verifyGroup(ball_large_snapped.getHomologyH1(),
                "H1(3-tetrahedron snapped ball)", 0);
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
            verifyGroup(pinchedSolidTorus.getHomologyH1(),
                "H1(pinched solid torus)", 1);
            verifyGroup(pinchedSolidKB.getHomologyH1(),
                "H1(pinched solid Klein bottle)", 1);
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
            verifyGroup(rp3_1.getHomologyH1Bdry(),
                "Boundary H1(RP^3, 1 vtx)", 0);
            verifyGroup(rp3_2.getHomologyH1Bdry(),
                "Boundary H1(RP^3, 2 vtx)", 0);
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
            verifyGroup(weberSeifert.getHomologyH1Bdry(),
                "Boundary H1(Weber-Seifert)", 0);
            verifyGroup(q32xz3.getHomologyH1Bdry(),
                "Boundary H1(S^3 / Q_32 x Z_3)", 0);
            verifyGroup(lens100_1.getHomologyH1Bdry(),
                "Boundary H1(L(100,1))", 0);
            verifyGroup(ball_large.getHomologyH1Bdry(),
                "Boundary H1(4-tetrahedron ball)", 0);
            verifyGroup(ball_large_pillows.getHomologyH1Bdry(),
                "Boundary H1(4-tetrahedron pillow ball)", 0);
            verifyGroup(ball_large_snapped.getHomologyH1Bdry(),
                "Boundary H1(3-tetrahedron snapped ball)", 0);
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

        static void verifyFundGroupVsH1(NTriangulation* tri) {
            NGroupPresentation* pi1 =
                new NGroupPresentation(tri->getFundamentalGroup());

            pi1->intelligentSimplify();

            // Abelianise, and make sure we get H1.
            size_t gen = pi1->countGenerators();
            size_t rel = pi1->countRelations();

            regina::NMatrixInt m(rel, gen);
            size_t i, j;
            for (i = 0; i < rel; ++i) {
                const regina::NGroupExpression& r = pi1->getRelation(i);
                for (j = 0; j < r.countTerms(); ++j) {
                    const regina::NGroupExpressionTerm& t = r.getTerm(j);
                    m.entry(i, t.generator) += t.exponent;
                }
            }
            delete pi1;

            NAbelianGroup abelian;
            abelian.addGroup(m);

            if (abelian.detail() != tri->getHomologyH1().detail()) {
                std::ostringstream msg;
                msg << "Abelianised fundamental group does not match H1 "
                    "for " << tri->label() << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void fundGroupVsH1() {
            verifyFundGroupVsH1(&empty);
            verifyFundGroupVsH1(&singleTet);
            verifyFundGroupVsH1(&s3);
            verifyFundGroupVsH1(&s2xs1);
            verifyFundGroupVsH1(&rp3_1);
            verifyFundGroupVsH1(&rp3_2);
            verifyFundGroupVsH1(&lens3_1);
            verifyFundGroupVsH1(&lens7_1_loop);
            verifyFundGroupVsH1(&lens8_3);
            verifyFundGroupVsH1(&lens8_3_large);
            verifyFundGroupVsH1(&rp3rp3);
            verifyFundGroupVsH1(&q28);
            verifyFundGroupVsH1(&weberSeifert);
            verifyFundGroupVsH1(&q32xz3);
            verifyFundGroupVsH1(&lens100_1);
            verifyFundGroupVsH1(&ball_large);
            verifyFundGroupVsH1(&ball_large_pillows);
            verifyFundGroupVsH1(&ball_large_snapped);
            verifyFundGroupVsH1(&lst3_4_7);
            verifyFundGroupVsH1(&figure8);
            verifyFundGroupVsH1(&rp2xs1);
            verifyFundGroupVsH1(&solidKB);
            verifyFundGroupVsH1(&gieseking);
            verifyFundGroupVsH1(&twoProjPlaneCusps);
            verifyFundGroupVsH1(&cuspedGenusTwoTorus);

            runCensusAllClosed(verifyFundGroupVsH1);
            runCensusAllBounded(verifyFundGroupVsH1);
            runCensusAllIdeal(verifyFundGroupVsH1);
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
            verifyFundGroup(rp3_1.getFundamentalGroup(),
                "Fund(RP^3, 1 vtx)", "Z_2");
            verifyFundGroup(rp3_2.getFundamentalGroup(),
                "Fund(RP^3, 2 vtx)", "Z_2");
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
            //verifyGroup(weberSeifert.getHomologyH1(),
            //    "Fund(SeifertWeber)", 0, 5, 5, 5);
            //verifyFundGroup(q32xz3.getFundamentalGroup(),
            //    "Fund(S^3 / Q_32 x Z_3)", 0, 2, 6);
            verifyFundGroup(lens100_1.getFundamentalGroup(),
                "Fund(L(100,1))", "Z_100");
            verifyFundGroup(ball_large.getFundamentalGroup(),
                "Fund(4-tetrahedron ball)", "0");
            verifyFundGroup(ball_large_pillows.getFundamentalGroup(),
                "Fund(4-tetrahedron pillow ball)", "0");
            verifyFundGroup(ball_large_snapped.getFundamentalGroup(),
                "Fund(3-tetrahedron snapped ball)", "0");
            verifyFundGroup(lst3_4_7.getFundamentalGroup(),
                "Fund(LST(3,4,7))", "Z");
            verifyFundGroup(figure8.getFundamentalGroup(),
                "Fund(figure eight knot complement)",
                "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b a^-1 b^2");
                // Note: \u21A6 is the unicode mapsto symbol.
            verifyFundGroup(rp2xs1.getFundamentalGroup(),
                "Fund(RP^2 x S^1)", "Z + Z_2");
            verifyFundGroup(solidKB.getFundamentalGroup(),
                "Fund(solid Klein bottle)", "Z");
            //verifyFundGroup(gieseking.getFundamentalGroup(),
            //    "Fund(Gieseking manifold)", 1);
            verifyFundGroup(invalidEdges.getFundamentalGroup(),
                "Fund(tri with invalid edges)", "0");
            verifyFundGroup(twoProjPlaneCusps.getFundamentalGroup(),
                "Fund(tri with projective plane cusps)", "Z_2");
            verifyFundGroup(cuspedGenusTwoTorus.getFundamentalGroup(),
                "Fund(cusped solid genus two torus)", "Free(2)");
            verifyFundGroup(pinchedSolidTorus.getFundamentalGroup(),
                "Fund(pinched solid torus)", "Z");
            verifyFundGroup(pinchedSolidKB.getFundamentalGroup(),
                "Fund(pinched solid Klein bottle)", "Z");
        }

        static void testZeroEfficiency(NTriangulation* tri) {
            bool ans = tri->isZeroEfficient();

            if (tri->isClosed() && tri->isConnected()) {
                if (tri->countVertices() > 2 && ans) {
                    std::ostringstream msg;
                    msg << "Triangulation " << tri->label()
                        << " has >2 vertices but is reported as 0-efficient.";
                    CPPUNIT_FAIL(msg.str());
                } else if (tri->countVertices() == 2 && ans &&
                        (! tri->getHomologyH1().isTrivial())) {
                    std::ostringstream msg;
                    msg << "Triangulation " << tri->label()
                        << " has 2 vertices and non-trivial homology "
                        "but is reported as 0-efficient.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            // Test it the long way, directly from the definition.
            bool expected;
            if (tri->hasTwoSphereBoundaryComponents())
                expected = false;
            else {
                expected = true;
                NNormalSurfaceList* s = NNormalSurfaceList::enumerate(
                    tri, regina::NS_STANDARD);
                const NNormalSurface* f;
                for (size_t i = 0; i < s->size(); ++i) {
                    f = s->getSurface(i);
                    if (f->eulerChar() == 2 &&
                            (! f->hasRealBoundary()) &&
                            ! f->isVertexLinking()) {
                        // Normal sphere
                        expected = false;
                        break;
                    } else if (f->eulerChar() == 1 &&
                            (! f->hasRealBoundary()) &&
                            (! f->isTwoSided()) &&
                            ! f->isVertexLinking()) {
                        // Normal projective plane that doubles to a
                        // normal sphere
                        expected = false;
                        break;
                    } else if (f->eulerChar() == 1 &&
                            f->hasRealBoundary() && ! f->isVertexLinking()) {
                        // Normal disc
                        expected = false;
                        break;
                    }
                }
                delete s;
            }
            if (ans && ! expected) {
                std::ostringstream msg;
                msg << "Triangulation " << tri->label()
                    << " is reported as 0-efficient but should not be.";
                CPPUNIT_FAIL(msg.str());
            } else if (expected && ! ans) {
                std::ostringstream msg;
                msg << "Triangulation " << tri->label()
                    << " is not reported as 0-efficient but should be.";
                CPPUNIT_FAIL(msg.str());
            }
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
            CPPUNIT_ASSERT_MESSAGE("RP^3 (1 vtx) is 0-efficient.",
                ! rp3_1.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("RP^3 (2 vtx) is 0-efficient.",
                ! rp3_2.isZeroEfficient());
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
            CPPUNIT_ASSERT_MESSAGE("The 4-tetrahedron ball is 0-efficient.",
                ! ball_large.isZeroEfficient());
                // Contains a non-trivial disc.
            CPPUNIT_ASSERT_MESSAGE(
                "The 4-tetrahedron pillow ball is 0-efficient.",
                ! ball_large_pillows.isZeroEfficient());
                // Contains a non-trivial disc.
            CPPUNIT_ASSERT_MESSAGE(
                "The 3-tetrahedron snapped ball is 0-efficient.",
                ! ball_large_snapped.isZeroEfficient());
                // Contains a non-trivial disc.
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

            runCensusAllClosed(testZeroEfficiency);
            runCensusAllBounded(testZeroEfficiency);
            runCensusAllIdeal(testZeroEfficiency);
        }

        void irreducibility() {
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not "
                "irreducible.", empty.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("S^3 is not irreducible.",
                s3.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("S^2 x S^1 is irreducible.",
                ! s2xs1.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("RP^3 (1 vtx) is not irreducible.",
                rp3_1.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("RP^3 (2 vtx) is not irreducible.",
                rp3_2.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("L(3,1) is not irreducible.",
                lens3_1.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("Layered loop L(7,1) is not irreducible.",
                lens7_1_loop.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("L(8,3) is not irreducible.",
                lens8_3.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("Large L(8,3) is not irreducible.",
                lens8_3_large.isIrreducible());
                // Contains a non-vertex-linking 2-sphere.
            CPPUNIT_ASSERT_MESSAGE("RP^3 # RP^3 is irreducible.",
                ! rp3rp3.isIrreducible());
                // Contains a pair of one-sided projective planes.
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_28 is not irreducible.",
                q28.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("S^3 / Q_32 x Z_3 is not irreducible.",
                q32xz3.isIrreducible());
        }

        NTriangulation* verifyThreeSphere(NTriangulation* tri,
                const char* name = 0) {
            if (name)
                tri->setPacketLabel(name);

            if (! tri->isThreeSphere()) {
                CPPUNIT_FAIL(("The 3-sphere " + tri->label() +
                    " is not recognised as such.").c_str());
            }

            // Try again with a barycentric subdivision.
            NTriangulation big(*tri);
            big.barycentricSubdivision();
            if (! big.isThreeSphere()) {
                CPPUNIT_FAIL(("The barycentric subdivision of the 3-sphere "
                    + tri->label() + " is not recognised as such.").c_str());
            }

            if (tri->isValid() && tri->isClosed() && tri->isOrientable() &&
                    tri->isConnected()) {
                regina::NContainer parent;
                long comps = tri->connectedSumDecomposition(&parent);
                if (comps != 0) {
                    std::ostringstream msg;
                    msg << "The 3-sphere " << tri->label()
                        << " was reported as having one or more "
                        "connected sum component.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            return tri;
        }

        NTriangulation* verifyNotThreeSphere(NTriangulation* tri,
                const char* name = 0) {
            if (name)
                tri->setPacketLabel(name);

            if (tri->isThreeSphere()) {
                CPPUNIT_FAIL(("The non-3-sphere " + tri->label() +
                    " is recognised as a 3-sphere.").c_str());
            }

            // Try again with a barycentric subdivision.
            NTriangulation big(*tri);
            big.barycentricSubdivision();
            if (big.isThreeSphere()) {
                CPPUNIT_FAIL(("The barycentric subdivision of the non-3-sphere "
                    + tri->label()
                    + " is recognised as a 3-sphere.").c_str());
            }

            if (tri->isValid() && tri->isClosed() && tri->isOrientable() &&
                    tri->isConnected()) {
                regina::NContainer parent;
                long comps = tri->connectedSumDecomposition(&parent);
                if (comps == 0) {
                    std::ostringstream msg;
                    msg << "The non-3-sphere " << tri->label()
                        << " was reported as having no "
                        "connected sum components.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            return tri;
        }

        void verifySigThreeSphere(const std::string& sigStr) {
            delete verifyThreeSphere(generateFromSig(sigStr));
        }

        void verifySigNotThreeSphere(const std::string& sigStr) {
            delete verifyNotThreeSphere(generateFromSig(sigStr));
        }

        void verifyIsoSigThreeSphere(const std::string& sigStr) {
            NTriangulation* t = NTriangulation::fromIsoSig(sigStr);
            t->setPacketLabel(sigStr);
            delete verifyThreeSphere(t);
        }

        void verifyIsoSigNotThreeSphere(const std::string& sigStr) {
            NTriangulation* t = NTriangulation::fromIsoSig(sigStr);
            t->setPacketLabel(sigStr);
            delete verifyNotThreeSphere(t);
        }

        static void testThreeSphere6(NTriangulation* tri) {
            // PRECONDITION: tri has <= 6 tetrahedra.
            //
            // We know from the closed orientable census data that the
            // only homology 3-sphere with <= 6 tetrahedra is the
            // Poincare homology 3-sphere.  This can be distinguished
            // from the real 3-sphere using the (5,1) Turaev-Viro
            // invariant -- for S^3 the invariant is 0.138197, and for
            // the Poincare homology sphere it is 0.947214.

            // What did 3-sphere recognition say?
            bool found = tri->isThreeSphere();

            bool expected = (tri->isValid() &&
                         tri->isClosed() &&
                         tri->isOrientable() &&
                         tri->isConnected() &&
                         tri->getHomologyH1().isTrivial() &&
                         tri->turaevViroApprox(5, 1) < 0.5);

            if (expected && ! found) {
                std::ostringstream msg;
                msg << "The census 3-sphere " << tri->label()
                    << " was not recognised as a 3-sphere.";
                CPPUNIT_FAIL(msg.str());
            } else if (found && ! expected) {
                std::ostringstream msg;
                msg << "The census non-3-sphere " << tri->label()
                    << " was recognised as a 3-sphere.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->isValid() && tri->isClosed() && tri->isOrientable() &&
                    tri->isConnected()) {
                regina::NContainer parent;
                long comps = tri->connectedSumDecomposition(&parent);
                if (expected && comps != 0) {
                    std::ostringstream msg;
                    msg << "The census 3-sphere " << tri->label()
                        << " was reported as having one or more "
                        "connected sum component.";
                    CPPUNIT_FAIL(msg.str());
                } else if (comps == 0 && ! expected) {
                    std::ostringstream msg;
                    msg << "The census non-3-sphere " << tri->label()
                        << " was reported as having no "
                        "connected sum components.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
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
            tri->setPacketLabel("L(1,0)");
            delete verifyThreeSphere(tri);

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

            // First, one out of the can:
            tri = NExampleTriangulation::poincareHomologySphere();
            delete verifyNotThreeSphere(tri,
                "Poincare homology sphere (example)");

            // Poincare homology sphere as a plugged triangular solid torus:
            tri = new NTriangulation;
            NTetrahedron* tet[5];
            int i;
            for (i = 0; i < 5; i++)
                tet[i] = tri->newTetrahedron();
            tet[0]->joinTo(0, tet[4], NPerm4(1,0,2,3));
            tet[0]->joinTo(1, tet[3], NPerm4(0,2,3,1));
            tet[0]->joinTo(2, tet[1], NPerm4(0,1,3,2));
            tet[0]->joinTo(3, tet[2], NPerm4(2,1,3,0));
            tet[1]->joinTo(0, tet[3], NPerm4(1,3,2,0));
            tet[1]->joinTo(1, tet[2], NPerm4(0,2,3,1));
            tet[1]->joinTo(2, tet[4], NPerm4(2,1,0,3));
            tet[2]->joinTo(1, tet[4], NPerm4(0,2,3,1));
            tet[2]->joinTo(3, tet[3], NPerm4(3,1,2,0));
            tet[3]->joinTo(3, tet[4], NPerm4(0,1,2,3));
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

            // Let's make sure silly things like balls aren't picked up.
            tri = new NTriangulation();
            tri->newTetrahedron();
            delete verifyNotThreeSphere(tri, "Standalone tetrahedron");

            tri = new NTriangulation();
            tet[0] = tri->newTetrahedron();
            tet[0]->joinTo(0, tet[0], NPerm4(3, 1, 2, 0));
            delete verifyNotThreeSphere(tri, "Snapped tetrahedron");

            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(1, 2);
            delete verifyNotThreeSphere(tri, "LST(1,2,3)");

            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(3, 4);
            delete verifyNotThreeSphere(tri, "LST(3,4,7)");

            tri = new NTriangulation();
            delete verifyNotThreeSphere(tri, "Empty triangulation");

            // Some disconnected examples.
            verifyNotThreeSphere(&disjoint2, "Disjoint, 2 components");
            verifyNotThreeSphere(&disjoint3, "Disjoint, 3 components");

            tri = new NTriangulation();
            tri->insertLayeredLensSpace(1,0);
            tri->insertLayeredLensSpace(1,0);
            tri->setPacketLabel("S^3 U S^3");
            delete verifyNotThreeSphere(tri);

            // An exhaustive census run:
            runCensusMinClosed(&testThreeSphere6);
            runCensusAllClosed(&testThreeSphere6);
            runCensusAllBounded(&testThreeSphere6);
            runCensusAllIdeal(&testThreeSphere6);
        }

        void threeSphereRecognitionLarge() {
            // Comment out every second one of these tests, since all of them
            // combined take a good 5-10 seconds (an annoyance if
            // you run the test suite often).

            // Homology spheres obtained from the hyperbolic census:
            verifyIsoSigNotThreeSphere("kLLLPLQkccfeghjiijjlnahgnqqadk");
            // verifyIsoSigNotThreeSphere("lLLLLPMQcbcgfhhihjkkktsmgsojfldor");
            verifyIsoSigNotThreeSphere("lLvAvMQQcbefjjkiihkjklaljfxrkvufd");
            // verifyIsoSigNotThreeSphere("lvLAAzMQcdefegihjkkjkjwarujwdaapj");

            // 3-sphere triangulations that are difficult to simplify
            // (taken from the 2013 Hyamfest paper on Regina):
            verifyIsoSigThreeSphere("jLLvQPQbeghfifiihxxaaxxjxar");
            //verifyIsoSigThreeSphere("jLLLAQPbeggfhhhiihhlhegbgdw");
            verifyIsoSigThreeSphere("jLLLPQPaegdehfgiibddbsabspr");
            //verifyIsoSigThreeSphere("jLLLPQPaegdehfgiibddbsabspk");
            verifyIsoSigThreeSphere("jLLLPQPaegdehfgiibddbsabwpr");
            //verifyIsoSigThreeSphere("jLLLPQPaegdehfgiibddbsabwpk");
            verifyIsoSigThreeSphere("jLLLPQPaegdehfgiibddbsabsdw");
            //verifyIsoSigThreeSphere("jLLLPQPaegdehfgiibddbsabsds");
            verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggliajgr");
            //verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggliajgk");
            verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggliawgr");
            //verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggliawgk");
            verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggllajgw");
            //verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggllajgj");
            verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggllawgw");
            //verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggllawgj");
            verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggiiargr");
            //verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggiiargk");
            verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggiiakgr");
            //verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggiiakgk");
            verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggilargw");
            //verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggilargj");
            verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggilakgw");
            //verifyIsoSigThreeSphere("kLAzwwQkbcbdehjihijhlwggilakgj");
            verifyIsoSigThreeSphere("kLLLAPPkbeggfihjiijhhlhehgdahw");
            //verifyIsoSigThreeSphere("kLLvQPPkbeghfifjhjjxxaaxxjxrvc");
        }

        NTriangulation* verifyThreeBall(NTriangulation* tri,
                const std::string& triName) {
            if (! tri->isBall()) {
                CPPUNIT_FAIL(("The 3-ball " + triName +
                    " is not recognised as such.").c_str());
            }

            // Try again with a barycentric subdivision.
            NTriangulation big(*tri);
            big.barycentricSubdivision();
            if (! big.isBall()) {
                CPPUNIT_FAIL(("The barycentric subdivision of the 3-ball "
                    + triName + " is not recognised as such.").c_str());
            }

            return tri;
        }

        NTriangulation* verifyNotThreeBall(NTriangulation* tri,
                const std::string& triName) {
            if (tri->isBall()) {
                CPPUNIT_FAIL(("The non-3-ball" + triName +
                    " is recognised as a 3-ball.").c_str());
            }

            // Try again with a barycentric subdivision.
            NTriangulation big(*tri);
            big.barycentricSubdivision();
            if (big.isBall()) {
                CPPUNIT_FAIL(("The barycentric subdivision of the non-3-ball "
                    + triName + " is recognised as a 3-ball.").c_str());
            }

            return tri;
        }

        void threeBallRecognition() {
            NTriangulation* tri;
            NTetrahedron* tet[4];

            // Balls:
            tri = new NTriangulation();
            tri->newTetrahedron();
            delete verifyThreeBall(tri, "Standalone tetrahedron");

            tri = new NTriangulation();
            tet[0] = tri->newTetrahedron();
            tet[0]->joinTo(0, tet[0], NPerm4(3, 1, 2, 0));
            delete verifyThreeBall(tri, "Snapped tetrahedron");

            tri = new NTriangulation();
            tet[0] = tri->newTetrahedron();
            tet[1] = tri->newTetrahedron();
            tet[0]->joinTo(0, tet[1], NPerm4());
            tet[0]->joinTo(1, tet[1], NPerm4());
            tet[0]->joinTo(2, tet[1], NPerm4());
            delete verifyThreeBall(tri, "Triangular pillow");

            // This ball used to crash the simplification routines once
            // upon a time.  Throw it into the test suite for good measure.
            tri = new NTriangulation();
            tet[0] = tri->newTetrahedron();
            tet[1] = tri->newTetrahedron();
            tet[2] = tri->newTetrahedron();
            tet[3] = tri->newTetrahedron();
            tet[0]->joinTo(2, tet[0], NPerm4(0,2));
            tet[0]->joinTo(1, tet[1], NPerm4(2,0,1,3));
            tet[1]->joinTo(2, tet[2], NPerm4());
            tet[1]->joinTo(1, tet[2], NPerm4(2,0,1,3));
            tet[2]->joinTo(1, tet[3], NPerm4(2,0,1,3));
            tet[3]->joinTo(2, tet[3], NPerm4(1,2));
            delete verifyThreeBall(tri, "4-tetrahedron ball");

            // Non-balls:
            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(1, 2);
            delete verifyNotThreeBall(tri, "LST(1,2,3)");

            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(3, 4);
            delete verifyNotThreeBall(tri, "LST(3,4,7)");

            tri = new NTriangulation();
            delete verifyNotThreeBall(tri, "Empty triangulation");

            // Make a punctured Poincare homology sphere.
            tri = NExampleTriangulation::poincareHomologySphere();
            tri->barycentricSubdivision();
            tri->removeTetrahedronAt(0);
            tri->intelligentSimplify();
            delete verifyNotThreeBall(tri,
                "Punctured Poincare homology sphere");

            // Throw in a couple of closed manifolds for good measure.
            tri = new NTriangulation();
            tri->insertLayeredLensSpace(1,0);
            delete verifyNotThreeBall(tri, "L(1,0)");

            tri = new NTriangulation();
            tri->insertLayeredLensSpace(2,1);
            delete verifyNotThreeBall(tri, "L(2,1)");

            tri = NExampleTriangulation::poincareHomologySphere();
            delete verifyNotThreeBall(tri, "Poincare homology sphere");

            // Some disconnected examples.
            verifyNotThreeBall(&disjoint2, "Disjoint, 2 components");
            verifyNotThreeBall(&disjoint3, "Disjoint, 3 components");

            tri = new NTriangulation();
            tri->newTetrahedron();
            tri->newTetrahedron();
            delete verifyNotThreeBall(tri, "B^3 U B^3");
        }

        static void testSolidTorus4(NTriangulation* tri) {
            // PRECONDITION: tri is not ideal, and has <= 4 tetrahedra.
            //
            // We know from the bounded orientable census data that the only
            // triangulations  with torus boundary and H1=Z with <= 4 tetrahedra
            // that are *not* solid tori are isosigs eHLObcdddwun and
            // eHLObcdddwuj, both representing SFS [D: (2,1) (3,-2)].

            // What did solid torus recognition say?
            bool found = tri->isSolidTorus();

            bool expected = (tri->isValid() &&
                         (! tri->isIdeal()) &&
                         tri->isOrientable() &&
                         tri->isConnected() &&
                         tri->countBoundaryComponents() == 1 &&
                         tri->boundaryComponent(0)->eulerChar() == 0 &&
                         tri->getHomologyH1().isZ() &&
                         (tri->size() < 4 ||
                            (tri->size() == 4 &&
                            tri->isoSig() != "eHLObcdddwun" &&
                            tri->isoSig() != "eHLObcdddwuj")));

            if (expected && ! found) {
                std::ostringstream msg;
                msg << "The census solid torus " << tri->label()
                    << " was not recognised as a solid torus.";
                CPPUNIT_FAIL(msg.str());
            } else if (found && ! expected) {
                std::ostringstream msg;
                msg << "The census solid torus " << tri->label()
                    << " was recognised as a solid torus.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        NTriangulation* verifySolidTorus(NTriangulation* tri,
                const char* triName = 0) {
            if (triName)
                tri->setPacketLabel(triName);

            NTriangulation bounded(*tri);
            if (bounded.isIdeal())
                bounded.idealToFinite();

            NTriangulation ideal(*tri);
            if (ideal.hasBoundaryTriangles())
                ideal.finiteToIdeal();

            NTriangulation boundedBig(bounded);
            boundedBig.barycentricSubdivision();

            NTriangulation idealBig(ideal);
            idealBig.barycentricSubdivision();

            if (! bounded.isSolidTorus()) {
                CPPUNIT_FAIL(("The real solid torus " +
                    tri->label() + " is not recognised as such.").c_str());
            }
            if (! ideal.isSolidTorus()) {
                CPPUNIT_FAIL(("The ideal solid torus " +
                    tri->label() + " is not recognised as such.").c_str());
            }
            if (! boundedBig.isSolidTorus()) {
                CPPUNIT_FAIL(("The subdivided real solid torus " +
                    tri->label() + " is not recognised as such.").c_str());
            }
            if (! idealBig.isSolidTorus()) {
                CPPUNIT_FAIL(("The subdivided ideal solid torus " +
                    tri->label() + " is not recognised as such.").c_str());
            }

            return tri;
        }

        NTriangulation* verifyNotSolidTorus(NTriangulation* tri,
                const char* triName = 0) {
            if (triName)
                tri->setPacketLabel(triName);

            NTriangulation bounded(*tri);
            if (bounded.isIdeal())
                bounded.idealToFinite();

            NTriangulation ideal(*tri);
            if (ideal.hasBoundaryTriangles())
                ideal.finiteToIdeal();

            NTriangulation boundedBig(bounded);
            boundedBig.barycentricSubdivision();

            NTriangulation idealBig(ideal);
            idealBig.barycentricSubdivision();

            if (bounded.isSolidTorus()) {
                CPPUNIT_FAIL(("The real non-solid-torus " +
                    tri->label() + " is recognised as a solid torus.").c_str());
            }
            if (ideal.isSolidTorus()) {
                CPPUNIT_FAIL(("The ideal non-solid-torus " +
                    tri->label() + " is recognised as a solid torus.").c_str());
            }
            if (boundedBig.isSolidTorus()) {
                CPPUNIT_FAIL(("The subdivided real non-solid-torus " +
                    tri->label() + " is recognised as a solid torus.").c_str());
            }
            if (idealBig.isSolidTorus()) {
                CPPUNIT_FAIL(("The subdivided ideal non-solid-torus " +
                    tri->label() + " is recognised as a solid torus.").c_str());
            }

            return tri;
        }

        void verifyIsoSigSolidTorus(const std::string& sigStr) {
            NTriangulation* t = NTriangulation::fromIsoSig(sigStr);
            t->setPacketLabel(sigStr);
            delete verifySolidTorus(t);
        }

        void verifyIsoSigNotSolidTorus(const std::string& sigStr) {
            NTriangulation* t = NTriangulation::fromIsoSig(sigStr);
            t->setPacketLabel(sigStr);
            delete verifyNotSolidTorus(t);
        }

        void solidTorusRecognition() {
            NTriangulation* tri;

            tri = new NTriangulation();
            delete verifyNotSolidTorus(tri, "Empty triangulation");

            tri = new NTriangulation();
            tri->newTetrahedron();
            delete verifyNotSolidTorus(tri, "Single tetrahedron");

            tri = new NTriangulation();
            NTetrahedron* tet = tri->newTetrahedron();
            tet->joinTo(0, tet, NPerm4(3, 1, 2, 0));
            delete verifyNotSolidTorus(tri, "Snapped tetrahedron");

            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(1, 2);
            delete verifySolidTorus(tri, "LST(1,2,3)");

            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(1, 20);
            delete verifySolidTorus(tri, "LST(1,20,21)");

            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(1, 1);
            delete verifySolidTorus(tri, "LST(1,1,2)");

            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(0, 1);
            delete verifySolidTorus(tri, "LST(0,1,1)");

            tri = NTriangulation::fromIsoSig("cMcabbgds");
            delete verifySolidTorus(tri, "Ideal solid torus");

            tri = NExampleTriangulation::figureEight();
            delete verifyNotSolidTorus(tri, "Figure 8 Knot Complement");

            // Some knot complements (with real boundary):
            verifyIsoSigNotSolidTorus("fHLykbcdeedwuqs");
            verifyIsoSigNotSolidTorus("fLHPccdeeeqcieh");
            verifyIsoSigNotSolidTorus(
                "oLALzLwMPOcbcbefijklnlmnnxxjnxmitnmfbae");
            verifyIsoSigNotSolidTorus(
                "rLLLvKPjQvQMkacfkljmjlmlppopqqjkgtaxknokbmgwvij");
            verifyIsoSigNotSolidTorus(
                "uLLvMPvwMwAMQkcacfgihjmklnnrqstrqrtnkvjhavkbveekgjxfcvp");

            // Some Seifert fibred spaces with torus boundary and Z homology:
            verifyIsoSigNotSolidTorus("eHLObcdddwun");
            verifyIsoSigNotSolidTorus("eHLObcdddwuj");

            // The connected sum of the Poincare homology sphere and the
            // solid torus:
            verifyIsoSigNotSolidTorus("kLHKwvQQcceeijhjijakaawsnrsn");

            // Some larger solid tori from the census:
            verifyIsoSigSolidTorus("iHbfPPPbdfgfhhtbedbac");
            verifyIsoSigSolidTorus("iHerzQPadgffhhbbiqbao");
            verifyIsoSigSolidTorus("iLAvPQacbbgehfgdicdffnf");

            // Throw in a couple of closed manifolds for good measure.
            tri = new NTriangulation();
            tri->insertLayeredLensSpace(1,0);
            delete verifyNotSolidTorus(tri, "L(1,0)");

            tri = new NTriangulation();
            tri->insertLayeredLensSpace(2,1);
            delete verifyNotSolidTorus(tri, "L(2,1)");

            tri = NExampleTriangulation::poincareHomologySphere();
            delete verifyNotSolidTorus(tri, "Poincare homology sphere");

            // Some disconnected triangulations:
            verifyNotSolidTorus(&disjoint2, "2-component manifold");
            verifyNotSolidTorus(&disjoint3, "3-component manifold");

            tri = new NTriangulation();
            tri->insertLayeredSolidTorus(1, 2);
            tri->insertLayeredSolidTorus(1, 2);
            delete verifyNotSolidTorus(tri, "LST U LST");

            // An exhaustive census run:
            runCensusAllBounded(&testSolidTorus4);
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

                double tv = t.turaevViroApprox(3, q0);
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

                double tv = s3.turaevViroApprox(r, q0);

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

                double tv = rp3_2.turaevViroApprox(r, q0);

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

                double tv = lens3_1.turaevViroApprox(r, q0);

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

                double tv = s2xs1.turaevViroApprox(r, q0);

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
            verifyTV3(rp3_1, "RP^3 (1 vtx)");
            verifyTV3(rp3_2, "RP^3 (2 vtx)");
            verifyTV3(lens3_1, "L(3,1)");
            verifyTV3(lens7_1_loop, "Layered loop L(7,1)");
            verifyTV3(lens8_3, "L(8,3)");
            verifyTV3(lens8_3_large, "Large L(8,3)");
            verifyTV3(rp3rp3, "RP^3 # RP^3");
            verifyTV3(q28, "S^3 / Q_28");
            verifyTV3(weberSeifert, "Weber-Seifert");
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

        static void verifyDoubleCover(NTriangulation* tri) {
            // PRE: tri is either empty or connected.
            if (! tri->isConnected())
                return;

            NTriangulation cover(*tri);
            cover.makeDoubleCover();

            if (tri->isEmpty()) {
                if (! cover.isEmpty())
                    CPPUNIT_FAIL("Empty triangulation: "
                        "Double cover is non-empty.");
                return;
            }

            // We have a non-empty connected triangulation.
            if (tri->isOrientable()) {
                // We should simply come away with two identical copies
                // of tri.
                regina::NContainer parent;
                if (cover.splitIntoComponents(&parent) != 2) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover does not "
                        "contain precisely two components.";
                    CPPUNIT_FAIL(msg.str());
                }

                NTriangulation* child = static_cast<NTriangulation*>(
                    parent.firstChild());
                while (child) {
                    if (! tri->isIsomorphicTo(*child).get()) {
                        std::ostringstream msg;
                        msg << tri->label()
                            << ": Orientable double cover "
                            "contains a component not isomorphic to the "
                            "original.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    child = static_cast<NTriangulation*>(child->nextSibling());
                }
            } else {
                // We should come away with a proper connected double cover.
                if (cover.countComponents() != 1) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover does not "
                        "contain precisely one component.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! cover.isOrientable()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover is not "
                        "orientable.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (cover.size() != 2 * tri->size()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover does not "
                        "contain precisely twice as many tetrahedra.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (cover.countTriangles() != 2 * tri->countTriangles()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover does not "
                        "contain precisely twice as many triangles.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri->isValid() && cover.countEdges() !=
                        2 * tri->countEdges()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover does not "
                        "contain precisely twice as many edges.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri->isValid() && (! tri->isIdeal()) &&
                        cover.countVertices() != 2 * tri->countVertices()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover does not "
                        "contain precisely twice as many vertices.";
                    CPPUNIT_FAIL(msg.str());
                }

                // We expect the first homology group to be identical,
                // or to be missing a copy of Z_2.
                if (! (tri->getHomologyH1() == cover.getHomologyH1())) {
                    NAbelianGroup hCover(cover.getHomologyH1());
                    hCover.addTorsionElement(2);
                    if (! (tri->getHomologyH1() == hCover)) {
                        std::ostringstream msg;
                        msg << tri->label()
                            << ": Orientable double cover has H1 = "
                            << cover.getHomologyH1().str()
                            << ", which does not match the original H1 = "
                            << tri->getHomologyH1().str() << '.';
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void doubleCover() {
            testManualAll(verifyDoubleCover);
        }

        static void verifyBary(NTriangulation* tri) {
            NTriangulation b(*tri);
            b.barycentricSubdivision();

            // Note that subdivisions can turn invalid into valid, but
            // they can never turn valid into invalid.
            if (tri->isValid() && ! b.isValid()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks validity.";
                CPPUNIT_FAIL(msg.str());
            }

            // Subdivisions can also turn invalid into ideal.
            // Only consider the valid -> valid case here.
            if (tri->isValid() && (tri->isIdeal() != b.isIdeal())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks idealness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->hasBoundaryTriangles() != b.hasBoundaryTriangles()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks boundary triangles.";
                CPPUNIT_FAIL(msg.str());
            }

            // As with ideal, consider valid inputs only.
            if (tri->isValid() && (tri->isClosed() != b.isClosed())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks closedness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->isOrientable() != b.isOrientable()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks orientability.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->isConnected() != b.isConnected()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks connectedness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->countComponents() != b.countComponents()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks connected components.";
                CPPUNIT_FAIL(msg.str());
            }

            // Invalid vertices can become new boundary components.
            // Only consider valid inputs here.
            if (tri->isValid() && (tri->countBoundaryComponents() !=
                    b.countBoundaryComponents())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks boundary components.";
                CPPUNIT_FAIL(msg.str());
            }

            // The same problem with invalid triangulations and boundary
            // components bites us with Euler characteristic also.
            if (tri->isValid() &&
                    (tri->eulerCharTri() != b.eulerCharTri())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks Euler char (tri).";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->isValid() &&
                    (tri->eulerCharManifold() != b.eulerCharManifold())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks Euler char (mfd).";
                CPPUNIT_FAIL(msg.str());
            }

            // Now run more expensive tests that will be better with
            // *small* triangulations.
            if (! tri->isValid())
                return;

            b.intelligentSimplify();

            if (! (tri->getHomologyH1() == b.getHomologyH1())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks H1.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! (tri->getHomologyH2() == b.getHomologyH2())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks H2.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void barycentricSubdivision() {
            testManualAll(verifyBary);
        }

        static void verifyIdealToFinite(NTriangulation* tri) {
            NTriangulation finite(*tri);
            finite.idealToFinite();

            // Are there any ideal vertices remaining?
            if (finite.isIdeal()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": idealToFinite() leaves ideal vertices.";
                CPPUNIT_FAIL(msg.str());
            }

            // Are there any invalid vertices remaining?
            for (NTriangulation::VertexIterator vit =
                    finite.getVertices().begin(); vit !=
                    finite.getVertices().end(); ++vit)
                if ((*vit)->isBoundary() && ! (*vit)->isStandard()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": idealToFinite() leaves "
                        "invalid vertices .";
                    CPPUNIT_FAIL(msg.str());
                }

            // Make sure the invalid edges are left alone.
            unsigned oldInvEdges = 0, newInvEdges = 0;
            NTriangulation::EdgeIterator eit;
            for (eit = tri->getEdges().begin();
                    eit != tri->getEdges().end(); ++eit)
                if (! (*eit)->isValid())
                    ++oldInvEdges;
            for (eit = finite.getEdges().begin();
                    eit != finite.getEdges().end(); ++eit)
                if (! (*eit)->isValid())
                    ++newInvEdges;
            if (oldInvEdges != newInvEdges) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": idealToFinite() changes "
                    "invalid edges .";
                CPPUNIT_FAIL(msg.str());
            }

            // Make sure we don't change the number of boundary components.
            if (tri->countBoundaryComponents() !=
                    finite.countBoundaryComponents()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": idealToFinite() changes "
                    "the number of boundary components.";
                CPPUNIT_FAIL(msg.str());
            }

            // In the case of a valid triangulation, ensure that the
            // boundary components are topologically unchanged.
            if (tri->isValid()) {
                typedef std::pair<long, bool> BCSpec;
                NTriangulation::BoundaryComponentIterator bcit;

                std::vector<BCSpec> bcOld;
                for (bcit = tri->boundaryComponents().begin();
                        bcit != tri->boundaryComponents().end(); ++bcit)
                    bcOld.push_back(BCSpec((*bcit)->eulerChar(),
                        (*bcit)->isOrientable()));
                std::sort(bcOld.begin(), bcOld.end());

                std::vector<BCSpec> bcNew;
                for (bcit = finite.boundaryComponents().begin();
                        bcit != finite.boundaryComponents().end(); ++bcit)
                    bcNew.push_back(BCSpec((*bcit)->eulerChar(),
                        (*bcit)->isOrientable()));
                std::sort(bcNew.begin(), bcNew.end());

                if (bcOld != bcNew) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": idealToFinite() changes "
                        "the topology of one or more boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void idealToFinite() {
            testManualAll(verifyIdealToFinite);
        }

        static void verifyFiniteToIdeal(NTriangulation* tri) {
            NTriangulation ideal(*tri);
            ideal.finiteToIdeal();

            // Are there any boundary triangles remaining?
            if (ideal.hasBoundaryTriangles()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": finiteToIdeal() leaves boundary triangles.";
                CPPUNIT_FAIL(msg.str());
            }

            // Make sure the invalid edges are left alone.
            unsigned oldInvEdges = 0, newInvEdges = 0;
            NTriangulation::EdgeIterator eit;
            for (eit = tri->getEdges().begin();
                    eit != tri->getEdges().end(); ++eit)
                if (! (*eit)->isValid())
                    ++oldInvEdges;
            for (eit = ideal.getEdges().begin();
                    eit != ideal.getEdges().end(); ++eit)
                if (! (*eit)->isValid())
                    ++newInvEdges;
            if (oldInvEdges != newInvEdges) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": finiteToIdeal() changes "
                    "invalid edges .";
                CPPUNIT_FAIL(msg.str());
            }

            // In the case of a valid triangulation, ensure that the
            // boundary components are topologically unchanged, except
            // for sphere which must vanish.
            if (tri->isValid()) {
                typedef std::pair<long, bool> BCSpec;
                NTriangulation::BoundaryComponentIterator bcit;

                std::vector<BCSpec> bcOld;
                for (bcit = tri->boundaryComponents().begin();
                        bcit != tri->boundaryComponents().end(); ++bcit)
                    if ((*bcit)->eulerChar() != 2)
                        bcOld.push_back(
                            BCSpec((*bcit)->eulerChar(),
                            (*bcit)->isOrientable()));
                std::sort(bcOld.begin(), bcOld.end());

                std::vector<BCSpec> bcNew;
                for (bcit = ideal.boundaryComponents().begin();
                        bcit != ideal.boundaryComponents().end(); ++bcit)
                    bcNew.push_back(BCSpec((*bcit)->eulerChar(),
                        (*bcit)->isOrientable()));
                std::sort(bcNew.begin(), bcNew.end());

                if (bcOld != bcNew) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": finiteToIdeal() changes "
                        "the topology of one or more non-sphere "
                        "boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void finiteToIdeal() {
            testManualAll(verifyFiniteToIdeal);
        }

        void drillEdge() {
            // Start with the snapped 1-tetrahedron triangulation of the
            // 3-sphere.  Edges 0 and 2 make a Hopf link, and edge 1 is
            // just an interval.
            {
                NTriangulation snap;
                NTetrahedron* tet = snap.newTetrahedron();
                tet->joinTo(0, tet, NPerm4(0, 1));
                tet->joinTo(2, tet, NPerm4(2, 3));

                NTriangulation tmp0(snap);
                tmp0.drillEdge(tmp0.getEdge(0));
                if (! tmp0.isSolidTorus())
                    CPPUNIT_FAIL("Snapped 3-sphere: drilling edge 0 "
                        "does not give a solid torus.");

                NTriangulation tmp1(snap);
                tmp1.drillEdge(tmp1.getEdge(1));
                if (! tmp1.isBall())
                    CPPUNIT_FAIL("Snapped 3-sphere: drilling edge 1 "
                        "does not give a 3-ball.");

                NTriangulation tmp2(snap);
                tmp2.drillEdge(tmp2.getEdge(2));
                if (! tmp2.isSolidTorus())
                    CPPUNIT_FAIL("Snapped 3-sphere: drilling edge 2 "
                        "does not give a solid torus.");
            }

            // Move on to the layered 1-tetrahedron triangulation of the
            // 3-sphere.
            // Edge 0 forms a trefoil, and edge 1 is unknotted.
            {
                NTriangulation layer;
                NTetrahedron* tet = layer.newTetrahedron();
                tet->joinTo(0, tet, NPerm4(1, 2, 3, 0));
                tet->joinTo(2, tet, NPerm4(2, 3));

                NTriangulation tmp0(layer);
                tmp0.drillEdge(tmp0.getEdge(0));
                if (! (tmp0.isValid() && (! tmp0.isIdeal()) &&
                        (tmp0.countBoundaryComponents() == 1) &&
                        (tmp0.getHomologyH1().isZ()) &&
                        (tmp0.boundaryComponent(0)->isOrientable()) &&
                        (tmp0.boundaryComponent(0)->eulerChar() == 0) &&
                        (! tmp0.isSolidTorus())))
                    CPPUNIT_FAIL("Layered 3-sphere: drilling edge 0 "
                        "does not give a non-trivial knot complement.");

                NTriangulation tmp1(layer);
                tmp1.drillEdge(tmp1.getEdge(1));
                if (! tmp1.isSolidTorus())
                    CPPUNIT_FAIL("Layered 3-sphere: drilling edge 1 "
                        "does not give a solid torus.");
            }

            // Next try a (1,2,3) layered solid torus.  Every edge is a
            // boundary edge, and so drilling it should leave us with
            // a solid torus again.
            {
                NTriangulation lst;
                lst.insertLayeredSolidTorus(1, 2);

                NTriangulation tmp0(lst);
                tmp0.drillEdge(tmp0.getEdge(0));
                if (! tmp0.isSolidTorus())
                    CPPUNIT_FAIL("LST(1,2,3): drilling edge 0 "
                        "does not give a solid torus.");

                NTriangulation tmp1(lst);
                tmp1.drillEdge(tmp1.getEdge(1));
                if (! tmp1.isSolidTorus())
                    CPPUNIT_FAIL("LST(1,2,3): drilling edge 1 "
                        "does not give a solid torus.");

                NTriangulation tmp2(lst);
                tmp2.drillEdge(tmp2.getEdge(2));
                if (! tmp2.isSolidTorus())
                    CPPUNIT_FAIL("LST(1,2,3): drilling edge 2 "
                        "does not give a solid torus.");
            }

            // Now try a 2-tetrahedron ball, where we drill the internal edge
            // between the two tetrahedra.  The result should be a solid torus.
            {
                NTriangulation ball;
                NTetrahedron* a = ball.newTetrahedron();
                NTetrahedron* b = ball.newTetrahedron();
                a->joinTo(0, b, NPerm4());
                a->joinTo(1, b, NPerm4());

                // The internal edge joins vertices 2-3.
                NTriangulation tmp(ball);
                tmp.drillEdge(tmp.getTetrahedron(0)->getEdge(5));
                if (! tmp.isSolidTorus())
                    CPPUNIT_FAIL("2-tetrahedron ball: drilling the "
                        "internal edge does not give a solid torus.");
            }
        }

        static void verifyPuncture(NTriangulation* tri) {
            unsigned long n = tri->size();
            if (n == 0)
                return;

            for (unsigned long i = 0; i <= n; ++i) {
                NTriangulation punc(*tri);
                NTetrahedron* origTet;
                if (i == n) {
                    origTet = tri->getTetrahedron(0);
                    punc.puncture();
                } else {
                    origTet = tri->getTetrahedron(i);
                    punc.puncture(punc.getTetrahedron(i));
                }

                if (punc.size() != n + 6) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture gives wrong # tetrahedra.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isValid() != tri->isValid()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture changes validity.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isIdeal() != tri->isIdeal()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture changes idealness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isStandard() != tri->isStandard()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture changes standardness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isConnected() != tri->isConnected()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture changes connectedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isOrientable() != tri->isOrientable()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture changes orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isOriented() != tri->isOriented()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture changes orientedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isClosed()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture gives a closed triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.countBoundaryComponents() !=
                        tri->countBoundaryComponents() + 1) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture gives wrong # boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.countBoundaryTriangles() !=
                        tri->countBoundaryTriangles() + 2) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture gives wrong # boundary triangles.";
                    CPPUNIT_FAIL(msg.str());
                }

                unsigned long nPunc = punc.size();
                NBoundaryComponent* bc = punc.getTetrahedron(nPunc - 1)->
                    getTriangle(0)->boundaryComponent();
                if (bc == 0 || bc != punc.getTetrahedron(nPunc - 2)->
                        getTriangle(0)->boundaryComponent()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture gives wrong boundary triangles.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (bc->countTriangles() != 2) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture gives wrong number of S^2 triangles.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (bc->eulerChar() != 2) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture gives wrong S^2 Euler characteristic.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (punc.getTetrahedron(nPunc - 1)->getVertex(1) !=
                        punc.getTetrahedron(nPunc - 2)->getVertex(1) ||
                        punc.getTetrahedron(nPunc - 1)->getVertex(2) !=
                        punc.getTetrahedron(nPunc - 2)->getVertex(3) ||
                        punc.getTetrahedron(nPunc - 1)->getVertex(3) !=
                        punc.getTetrahedron(nPunc - 2)->getVertex(2)) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture gives wrong S^2 vertex labels.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.eulerCharTri() != tri->eulerCharTri() + 1) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture gives wrong Euler characteristic (tri).";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.eulerCharManifold() != tri->eulerCharManifold() + 1) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "puncture gives wrong Euler characteristic (mfd).";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri->isValid()) {
                    if (! (punc.getHomologyH1() == tri->getHomologyH1())) {
                        std::ostringstream msg;
                        msg << tri->label() << ", tet " << i << ": "
                            << "puncture changes H1.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    NAbelianGroup expectH2(tri->getHomologyH2());
                    NAbelianGroup foundH2(punc.getHomologyH2());
                    NComponent* c = origTet->component();
                    if (! c->isClosed()) {
                        // X -> X + Z
                        expectH2.addRank();
                    } else if (! c->isOrientable()) {
                        // X + Z_2 -> X + Z
                        expectH2.addRank();
                        foundH2.addTorsionElement(2);
                    }

                    if (foundH2 != expectH2) {
                        std::ostringstream msg;
                        msg << tri->label() << ", tet " << i << ": "
                            << "puncture gives the wrong H2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void puncture() {
            testManualSmall(verifyPuncture);
            runCensusAllClosed(verifyPuncture, true);
            runCensusAllBounded(verifyPuncture, true);
            runCensusAllIdeal(verifyPuncture, true);
        }

        static void verifyConnectedSumWithSelf(NTriangulation* tri) {
            if (tri->countComponents() != 1)
                return;

            NTriangulation t(*tri);
            t.connectedSumWith(t);

            // TODO: Check that homology doubles.

            // All of our remaining tests are for closed manifolds.
            if (! tri->isClosed())
                return;

            if (! t.isClosed()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": tri # tri is not closed.";
                CPPUNIT_FAIL(msg.str());
            }

            // We are about to start running exponential-time algorithms.
            if (tri->size() > 10)
                return;

            long nOld = tri->connectedSumDecomposition();
            if (nOld < 0 && ! tri->isOrientable()) {
                // One of those cases where connected sum decomposition
                // legitimately fails.  The following tests are no use
                // for us here.
                return;
            }

            long nNew = t.connectedSumDecomposition();
            if (nNew >= 0 || tri->isOrientable()) {
                if (nNew != 2 * nOld) {
                    std::ostringstream msg;
                    msg << tri->label() << ": tri # tri has "
                        << nNew << " summands, not " << (2 * nOld) << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                if (nOld == 1) {
                    // There should be two summands, each homeomorphic
                    // to the original.
                    NTriangulation* c1 = static_cast<NTriangulation*>(
                        t.firstChild());
                    NTriangulation* c2 = static_cast<NTriangulation*>(
                        c1->nextSibling());
                    if (c1->getHomologyH1() != tri->getHomologyH1() ||
                            c2->getHomologyH1() != tri->getHomologyH1()) {
                        std::ostringstream msg;
                        msg << tri->label() << ": tri # tri has "
                            "summands with the wrong homology.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void connectedSumWithSelf() {
            testManualSmall(verifyConnectedSumWithSelf);
        }

        void verifyDehydration(const NTriangulation& tri) {
            std::string dehydrate = tri.dehydrate();
            if (dehydrate.empty()) {
                std::ostringstream msg;
                msg << tri.label() << ": Cannot dehydrate.";
                CPPUNIT_FAIL(msg.str());
            }

            NTriangulation rehydrate;
            if (! rehydrate.insertRehydration(dehydrate)) {
                std::ostringstream msg;
                msg << tri.label()
                    << ": Cannot rehydrate \"" << dehydrate << "\".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! rehydrate.isIsomorphicTo(tri).get()) {
                std::ostringstream msg;
                msg << tri.label()
                    << ": Rehydration of \"" << dehydrate
                    << "\" is not isomorphic to the original.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyNoDehydration(const NTriangulation& tri) {
            std::string dehydrate = tri.dehydrate();
            if (! dehydrate.empty()) {
                std::ostringstream msg;
                msg << tri.label()
                    << ": Should not dehydrate, but instead dehydrates to \""
                    << dehydrate << "\".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void dehydration() {
            verifyDehydration(empty);
            verifyNoDehydration(singleTet);
            verifyDehydration(s3);
            verifyDehydration(s3_large);
            verifyDehydration(s2xs1);
            verifyDehydration(rp3_1);
            verifyDehydration(rp3_2);
            verifyDehydration(rp3_large);
            verifyDehydration(lens3_1);
            verifyDehydration(lens8_3);
            verifyDehydration(lens8_3_large);
            verifyDehydration(lens7_1_loop);
            verifyDehydration(rp3rp3);
            verifyDehydration(q32xz3);
            verifyDehydration(q28);
            verifyDehydration(q20_large);
            verifyDehydration(weberSeifert);
            verifyNoDehydration(lens100_1);
            verifyNoDehydration(ball_large);
            verifyNoDehydration(ball_large_pillows);
            verifyNoDehydration(ball_large_snapped);
            verifyNoDehydration(singleTet_bary);
            verifyNoDehydration(fig8_bary);
            verifyNoDehydration(lst3_4_7);
            verifyDehydration(figure8);
            verifyDehydration(rp2xs1);
            verifyNoDehydration(solidKB);
            verifyDehydration(gieseking);
            verifyDehydration(invalidEdges);
            verifyDehydration(twoProjPlaneCusps);
            verifyDehydration(cuspedGenusTwoTorus);
            verifyNoDehydration(pinchedSolidTorus);
            verifyNoDehydration(pinchedSolidKB);
            verifyNoDehydration(disjoint2);
            verifyNoDehydration(disjoint3);
        }

        void verifySimplification(const NTriangulation& tri,
                unsigned simpleSize, const char* simpleName) {
            NTriangulation t(tri);
            t.intelligentSimplify();

            if (t.size() != simpleSize) {
                std::ostringstream msg;
                msg << "Large triangulation should simplify to " << simpleName
                    << ", but simplifies to " << t.size()
                    << " tetrahedra instead of the expected "
                    << simpleSize << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NStandardTriangulation* std =
                NStandardTriangulation::isStandardTriangulation(&t);
            if (std == 0) {
                std::ostringstream msg;
                msg << "Large triangulation should simplify to " << simpleName
                    << ", but instead simplifies to something unrecognisable.";
                CPPUNIT_FAIL(msg.str());
            }

            if (std->getName() != simpleName) {
                std::ostringstream msg;
                msg << "Large triangulation should simplify to " << simpleName
                    << ", but instead simplifies to " << std->getName() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            // Make sure it does not simplify any further.
            NTriangulation t2(t);
            if (t2.intelligentSimplify()) {
                std::ostringstream msg;
                msg << "The simple triangulation " << std->getName()
                    << " should not simplify any further, but it does.";
                CPPUNIT_FAIL(msg.str());
            }
            if (t2.dumpConstruction() != t.dumpConstruction()) {
                std::ostringstream msg;
                msg << "The simple triangulation " << std->getName()
                    << " should not change at all when simplified again, "
                    "but it does.";
                CPPUNIT_FAIL(msg.str());
            }
            delete std;
        }

        void verifyNoSimplification(const NTriangulation& tri,
                const char* name) {
            NTriangulation t(tri);
            if (t.intelligentSimplify()) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " simplifies but should not.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void simplification() {
            verifySimplification(s3_large, 1, "L(1,0)");
            verifySimplification(rp3_large, 2, "L(2,1)");
            verifySimplification(lens8_3_large, 2, "L(8,3)");
            verifySimplification(q20_large, 5, "C~(5)");
            verifySimplification(ball_large, 1, "B3 (3-vtx)");
            verifySimplification(ball_large_pillows, 1, "B3 (3-vtx)");
            verifySimplification(ball_large_snapped, 1, "B3 (3-vtx)");
            verifySimplification(fig8_bary, 2, "SnapPea m004");
            verifySimplification(singleTet_bary, 1, "B3 (3-vtx)");

            // Some triangulations that should not simplify.
            NTriangulation* tri;
            NTetrahedron* tet[4];

            // A triangulation with two degree two projective plane cusps
            // (that should not be simplified away):
            tri = new NTriangulation();
            tri->insertRehydration("cabbbbxww");
            if (tri->size() != 2)
                CPPUNIT_FAIL("Custom two-cusped triangulation failed "
                    "to rehydrate.");
            verifyNoSimplification(*tri, "Custom two-cusped triangluation");
            delete tri;

            // A triangulation with an invalid edge that simplifies
            // (where the invalid edge must not be simplified away):
            tri = new NTriangulation();
            tet[0] = tri->newTetrahedron();
            tet[1] = tri->newTetrahedron();
            tet[2] = tri->newTetrahedron();
            tet[3] = tri->newTetrahedron();
            tet[0]->joinTo(3, tet[2], NPerm4());
            tet[0]->joinTo(2, tet[1], NPerm4(2, 3));
            tet[3]->joinTo(3, tet[2], NPerm4(2, 3));
            tet[3]->joinTo(2, tet[1], NPerm4(1, 0));
            if (tri->isValid())
                CPPUNIT_FAIL("Custom invalid triangulation was not built "
                    "properly.");

            tri->intelligentSimplify();
            if (tri->size() != 1)
                CPPUNIT_FAIL("Custom invalid triangulation did not simplify "
                    "to 1 tetrahedron.");
            if (tri->isValid() || tri->getEdge(0)->isValid())
                CPPUNIT_FAIL("Custom invalid triangulation did not simplify "
                    "to an invalid triangulation with an invalid edge.");
            delete tri;

            // A solid torus that once upon a time was incorrectly simplified
            // away to a ball.
            tri = new NTriangulation();
            tet[0] = tri->newTetrahedron();
            tet[1] = tri->newTetrahedron();
            tet[2] = tri->newTetrahedron();
            tet[2]->joinTo(3, tet[2], NPerm4(2, 3));
            tet[2]->joinTo(1, tet[1], NPerm4(0, 2, 3, 1));
            tet[2]->joinTo(0, tet[0], NPerm4(3, 0, 1, 2));
            tet[1]->joinTo(3, tet[0], NPerm4(0, 3, 1, 2));
            tet[1]->joinTo(1, tet[0], NPerm4());
            if (tri->getHomologyH1().str() != "Z")
                CPPUNIT_FAIL("Custom solid torus has incorrect H1.");
            tri->intelligentSimplify();
            if (tri->getHomologyH1().str() != "Z")
                CPPUNIT_FAIL("Custom solid torus simplifies to "
                    "something different.");
            delete tri;
        }

        void verifyRetriangulation(const char* isoSig,
                unsigned heightNeeded, unsigned nThreads) {
            NTriangulation t(isoSig);
            if (t.isEmpty())
                CPPUNIT_FAIL("Triangulation not constructed from isosig.");

            unsigned nInit = t.size();

            for (unsigned h = 0; h < heightNeeded; ++h) {
                if (t.simplifyExhaustive(h, nThreads)) {
                    std::ostringstream msg;
                    msg << "Triangulation " << isoSig
                        << " incorrectly simplifies with height " << h << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (t.size() != nInit) {
                    std::ostringstream msg;
                    msg << "Triangulation " << isoSig
                        << " was changed during unsuccessful exhaustive "
                        "simplification with height " << h << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (! t.simplifyExhaustive(heightNeeded, nThreads)) {
                std::ostringstream msg;
                msg << "Triangulation " << isoSig
                    << " fails to simplify with height " << heightNeeded << ".";
                CPPUNIT_FAIL(msg.str());
            }
            if (t.size() == nInit) {
                    std::ostringstream msg;
                    msg << "Triangulation " << isoSig
                        << " did not reduce during successful exhaustive "
                        "simplification with height " << heightNeeded << ".";
                    CPPUNIT_FAIL(msg.str());
            }
        }

        void retriangulation() {
            verifyRetriangulation("hLALPkbcbefgfghxwnxark", 3, 1);
            verifyRetriangulation("hLALPkbcbefgfghxwnxark", 3, 2);
        }

        static void testReordering(NTriangulation* t) {
            NTriangulation a(*t);
            a.reorderTetrahedraBFS();

            NTriangulation b(*t);
            b.reorderTetrahedraBFS(true);

            NIsomorphism* iso = NIsomorphism::random(t->size());
            NTriangulation* c = iso->apply(t);
            delete iso;

            NTriangulation d(*c);
            d.reorderTetrahedraBFS();

            NTriangulation e(*c);
            e.reorderTetrahedraBFS(true);

            if (! t->isIsomorphicTo(a).get()) {
                std::ostringstream msg;
                msg << "Triangulation " << t->label()
                    << " changes its isomorphism class when its tetrahedra "
                    "are reordered in the forward direction.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! t->isIsomorphicTo(b).get()) {
                std::ostringstream msg;
                msg << "Triangulation " << t->label()
                    << " changes its isomorphism class when its tetrahedra "
                    "are reordered in the reverse direction.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! t->isIsomorphicTo(*c).get()) {
                std::ostringstream msg;
                msg << "Triangulation " << t->label()
                    << " changes its isomorphism class when a random "
                    "isomorphism is applied.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! t->isIsomorphicTo(d).get()) {
                std::ostringstream msg;
                msg << "Triangulation " << t->label()
                    << " changes its isomorphism class when a random "
                    "isomorphism is applied and then its tetrahedra are "
                    "reordered in the forward direction.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! t->isIsomorphicTo(e).get()) {
                std::ostringstream msg;
                msg << "Triangulation " << t->label()
                    << " changes its isomorphism class when a random "
                    "isomorphism is applied and then its tetrahedra are "
                    "reordered in the reverse direction.";
                CPPUNIT_FAIL(msg.str());
            }

            delete c;
        }

        void reordering() {
            testManualAll(testReordering);
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
            t.newTetrahedron();

            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron is "
                "0-efficient.", ! t.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron has no "
                "splitting surface.", t.hasSplittingSurface());
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron has no "
                "two-sphere boundary components.",
                t.hasTwoSphereBoundaryComponents());

            // Glue the tetrahedron to itself to form a solid torus.
            t.getTetrahedron(0)->joinTo(0, t.getTetrahedron(0),
                NPerm4(1, 2, 3, 0));

            verifyGroup(t.getHomologyH1(),
                "H1(LST(1,2,3))", 1);
            verifyGroup(t.getHomologyH1Bdry(),
                "Boundary H1(LST(1,2,3))", 2);

            // Glue the remaining two faces in a non-orientable fashion.
            t.getTetrahedron(0)->joinTo(2, t.getTetrahedron(0),
                NPerm4(1, 0, 3, 2));

            CPPUNIT_ASSERT_MESSAGE("A bad 1-tetrahedron triangulation "
                "is valid.", ! t.isValid());
            CPPUNIT_ASSERT_MESSAGE("A bad 1-tetrahedron triangulation "
                "is orientable.", ! t.isOrientable());
        }

        static void verifyEltMove14(NTriangulation* tri) {
            unsigned long n = tri->size();
            for (unsigned long i = 0; i < n; ++i) {
                NTriangulation large(*tri);
                large.oneFourMove(large.getTetrahedron(i));

                if (large.size() != n + 3) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "1-4 move gives wrong # tetrahedra.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.isValid() != tri->isValid()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "1-4 move changes validity.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.isOrientable() != tri->isOrientable()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "1-4 move changes orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.isClosed() != tri->isClosed()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "1-4 move changes closedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.countBoundaryComponents() !=
                        tri->countBoundaryComponents()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "1-4 move changes # boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.eulerCharTri() != tri->eulerCharTri()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "1-4 move changes Euler characteristic.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri->isValid()) {
                    if (! (large.getHomologyH1() == tri->getHomologyH1())) {
                        std::ostringstream msg;
                        msg << tri->label() << ", tet " << i << ": "
                            << "1-4 move changes H1.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if (! (large.getHomologyH2() == tri->getHomologyH2())) {
                        std::ostringstream msg;
                        msg << tri->label() << ", tet " << i << ": "
                            << "1-4 move changes H2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                // Shrink.
                if (large.isIsomorphicTo(*tri).get()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "1-4 move: result is isomorphic.";
                    CPPUNIT_FAIL(msg.str());
                }

                bool res =
                    large.collapseEdge(large.getTetrahedron(n + 2)->getEdge(
                    regina::NEdge::edgeNumber[0][3]), true, true);

                if (! res) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "1-4 move: could not recollapse edge.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! large.isIsomorphicTo(*tri).get()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tet " << i << ": "
                        << "1-4 move: recollapse is not isomorphic.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void eltMove14() {
            testManualSmall(verifyEltMove14);
            runCensusAllClosed(verifyEltMove14, true);
            runCensusAllBounded(verifyEltMove14, true);
            runCensusAllIdeal(verifyEltMove14, true);
        }
};

void addNTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NTriangulationTest::suite());
}

