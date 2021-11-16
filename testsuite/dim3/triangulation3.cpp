
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#define _USE_MATH_DEFINES // for M_PI, which is non-standard

#include <algorithm>
#include <cmath>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <cppunit/extensions/HelperMacros.h>
#include "link/link.h"
#include "maths/matrix.h"
#include "maths/numbertheory.h"
#include "split/signature.h"
#include "subcomplex/standardtri.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

#include "testsuite/exhaustive.h"
#include "testsuite/generic/triangulationtest.h"
#include "testsuite/dim3/testtriangulation.h"

using regina::AbelianGroup;
using regina::BoundaryComponent;
using regina::Component;
using regina::Example;
using regina::GroupPresentation;
using regina::Isomorphism;
using regina::Link;
using regina::NormalSurface;
using regina::NormalSurfaces;
using regina::Perm;
using regina::Signature;
using regina::StandardTriangulation;
using regina::Tetrahedron;
using regina::Triangle;
using regina::Triangulation;
using regina::Vertex;

class Triangulation3Test : public TriangulationTest<3> {
    CPPUNIT_TEST_SUITE(Triangulation3Test);

    // Generic tests:
    CPPUNIT_TEST(copyMove);
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryTriangles);
    CPPUNIT_TEST(boundaryBuild);
    CPPUNIT_TEST(edgeAccess);

    // Dimension-specific tests:
    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(standardness);
    CPPUNIT_TEST(boundaryComponents);
    CPPUNIT_TEST(vertexLinksSpecific);
    CPPUNIT_TEST(vertexLinks);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(homology);
    CPPUNIT_TEST(homologyBdry);
    CPPUNIT_TEST(fundGroup);
    CPPUNIT_TEST(fundGroupVsH1);
    CPPUNIT_TEST(zeroEfficiency);
    CPPUNIT_TEST(irreducibility);
    CPPUNIT_TEST(threeSphereRecognition);
    CPPUNIT_TEST(threeSphereRecognitionLarge);
    CPPUNIT_TEST(threeBallRecognition);
    CPPUNIT_TEST(solidTorusRecognition);
    CPPUNIT_TEST(torusXIntervalRecognition);
    CPPUNIT_TEST(turaevViro);
    CPPUNIT_TEST(barycentricSubdivision);
    CPPUNIT_TEST(idealToFinite);
    CPPUNIT_TEST(finiteToIdeal);
    CPPUNIT_TEST(pinchEdge);
    CPPUNIT_TEST(puncture);
    CPPUNIT_TEST(connectedSumWithSelf);
    CPPUNIT_TEST(dehydration);
    CPPUNIT_TEST(simplification);
    CPPUNIT_TEST(retriangulation);
    CPPUNIT_TEST(reordering);
    CPPUNIT_TEST(propertyUpdates);
    CPPUNIT_TEST(pachner<0>);
    CPPUNIT_TEST(pachner<1>);
    CPPUNIT_TEST(pachner<2>);
    CPPUNIT_TEST(pachner<3>);
    CPPUNIT_TEST(minimiseBoundary);
    CPPUNIT_TEST(fillTorus);
    CPPUNIT_TEST(meridianLongitude);
    CPPUNIT_TEST(retriangulate);
    CPPUNIT_TEST(swapping);

    CPPUNIT_TEST_SUITE_END();

    private:
        static constexpr const double epsilon = 0.00000001;
            /**< For comparing floating-point Turaev-Viro values. */

        // Closed orientable:
        Triangulation<3> s3;
            /**< A one-vertex 3-sphere. */
        Triangulation<3> rp3_1;
            /**< A one-vertex triangulation of RP^3. */
        Triangulation<3> rp3_2;
            /**< A two-vertex triangulation of RP^3. */
        Triangulation<3> lens3_1;
            /**< A two-vertex lens space L(3,1). */
        Triangulation<3> lens8_3;
            /**< The layered lens space L(8,3). */
        Triangulation<3> lens7_1_loop;
            /**< An untwisted layered loop representing L(7,1). */
        Triangulation<3> rp3rp3;
            /**< The connected sum RP^3 # RP^3. */
        Triangulation<3> q32xz3;
            /**< The orbit manifold S^3 / Q_32 x Z_3. */
        Triangulation<3> q28;
            /**< A twisted layered loop representing S^3 / Q_28. */
        Triangulation<3> weberSeifert;
            /**< The Weber-Seifert dodecahedral space.  With 23 tetrahedra,
                 this is too large for running normal surface algorithms
                 here in the test suite. */

        // Closed orientable, very large:
        Triangulation<3> lens100_1;
            /**< The lens space L(100,1). */

        // Bounded orientable:
        Triangulation<3> lst3_4_7;
            /**< The layered solid torus LST(3,4,7). */

        // Ideal orientable:
        Triangulation<3> figure8;
            /**< The figure eight knot complement. */
        Triangulation<3> trefoil;
            /**< The trefoil knot complement. */
        Triangulation<3> knot18;
            /**< An 18-crossing knot complement from the census. */

        // Closed non-orientable:
        Triangulation<3> rp2xs1;
            /**< The product RP^2 x S^1. */

        // Ideal non-orientable:
        Triangulation<3> gieseking;
            /**< The Gieseking manifold. */

        // Invalid edges:
        Triangulation<3> invalidEdges;
            /**< Contains two invalid edges, but all vertices have
                 2-sphere links. */

        // Non-standard vertex links:
        Triangulation<3> twoProjPlaneCusps;
            /**< A subdivision of invalidEdges, resulting in all edges
                 valid but two projective plane cusps.  Note that this
                 triangulation has a 3-sphere orientable double cover. */
        Triangulation<3> cuspedGenusTwoTorus;
            /**< A solid genus two torus with a cusped boundary. */
        Triangulation<3> pinchedSolidTorus;
            /**< A solid torus with one longitude pinched to a point. */
        Triangulation<3> pinchedSolidKB;
            /**< A solid Klein bottle with one longitude pinched to a point. */

        // Non-minimal:
        Triangulation<3> s3_large;
            /**< A non-minimal 3-sphere triangulation. */
        Triangulation<3> rp3_large;
            /**< The manifold RP^3. */
        Triangulation<3> lens8_3_large;
            /**< The lens space L(8,3). */
        Triangulation<3> q20_large;
            /**< The manifold S^3 / Q_20. */
        Triangulation<3> ball_large;
            /**< A four tetrahedron ball that used to cause crashes in
                 the simplification routines once upon a time. */
        Triangulation<3> ball_large_pillows;
            /**< A four tetrahedron ball formed by joining together two
                 triangular pillows. */
        Triangulation<3> ball_large_snapped;
            /**< A ball formed by joining together three snapped balls. */
        Triangulation<3> singleTet_bary;
            /**< The barycentric subdivision of a single tetrahedron. */
        Triangulation<3> fig8_bary;
            /**< The barycentric subdivision of the figure eight knot
                 complement. */

        // Disconnected triangulations:
        Triangulation<3> disjoint2;
            /**< A disjoint union of two triangulations. */
        Triangulation<3> disjoint3;
            /**< A disjoint union of three triangulations. */

    public:
        void setUp() override {
            TriangulationTest<3>::setUp();

            // Some of our triangulations can be constructed automatically.
            s3 = Example<3>::lens(1, 0);

            rp3_1 = Example<3>::lens(2, 1);

            rp3_2 = Example<3>::layeredLoop(2, false);

            lens8_3 = Example<3>::lens(8, 3);

            lens100_1 = Example<3>::lens(100, 1);

            lst3_4_7.insertLayeredSolidTorus(3, 4);

            q28 = Example<3>::layeredLoop(7, true);

            lens7_1_loop = Example<3>::layeredLoop(7, false);

            // Some of our triangulations can be generated from
            // splitting surfaces.
            rp3rp3 = Signature::parse("aabccd.b.d").triangulate();

            q32xz3 = Signature::parse("aabcdb.cedfef").triangulate();

            s3_large = Signature::parse("abc.abd.cef.de.fg.g").triangulate();

            lens8_3_large = Signature::parse("aabcb.cd.d").triangulate();

            rp3_large = Signature::parse("aabcdedcfb.fg.e.g").triangulate();

            q20_large = Signature::parse("abcdeabcdef.fg.g").triangulate();

            // Some are hard-coded in the calculation engine as sample
            // triangulations.
            weberSeifert = Example<3>::weberSeifert();

            figure8 = Example<3>::figureEight();

            trefoil = Example<3>::trefoil();

            rp2xs1 = Example<3>::rp2xs1();

            knot18 = Link::fromKnotSig(
                "sabcdeafghidejklmnopqgcbfqhinmjrpolkrlLvnvvNdM9aE").
                complement();

            gieseking = Example<3>::gieseking();

            cuspedGenusTwoTorus = Example<3>::cuspedGenusTwoTorus();

            singleTet_bary.newTetrahedron();
            singleTet_bary.barycentricSubdivision();

            fig8_bary = Example<3>::figureEight();
            fig8_bary.barycentricSubdivision();

            // The rest alas must be done manually.

            {
                // A two-tetrahedron two-vertex L(3,1) is straightforward to
                // construct using a vertex of degree two.
                auto [r, s] = lens3_1.newTetrahedra<2>();
                r->join(0, s, Perm<4>(0, 2, 3, 1));
                r->join(1, s, Perm<4>());
                r->join(2, s, Perm<4>());
                r->join(3, s, Perm<4>());
            }
            {
                // For a triangulation with invalid edges, we simply fold
                // the faces of a tetrahedron together in pairs (as in a
                // 3-sphere triangulation) but apply a reflection to each fold.
                auto r = invalidEdges.newTetrahedron();
                r->join(0, r, Perm<4>(1, 0, 3, 2));
                r->join(2, r, Perm<4>(1, 0, 3, 2));
            }

            twoProjPlaneCusps.insertTriangulation(invalidEdges);
            twoProjPlaneCusps.barycentricSubdivision();

            {
                // To construct a solid torus with a pinched longitude, we
                // identify two opposite faces of a square pyramid.
                auto [r, s] = pinchedSolidTorus.newTetrahedra<2>();
                r->join(3, s, Perm<4>(0, 1, 2, 3));
                r->join(2, s, Perm<4>(0, 3, 1, 2));
            }
            {
                // The pinched solid Klein bottle is much the same, except
                // for a twist before the opposite faces are identified.
                auto [r, s] = pinchedSolidKB.newTetrahedra<2>();
                r->join(3, s, Perm<4>(0, 1, 2, 3));
                r->join(2, s, Perm<4>(0, 2, 1, 3));
            }
            {
                // This ball used to cause a crash once upon a time.
                // Throw it into the test suite for good measure.
                auto [r, s, t, u] = ball_large.newTetrahedra<4>();
                r->join(2, r, Perm<4>(0,2));
                r->join(1, s, Perm<4>(2,0,1,3));
                s->join(2, t, Perm<4>());
                s->join(1, t, Perm<4>(2,0,1,3));
                t->join(1, u, Perm<4>(2,0,1,3));
                u->join(2, u, Perm<4>(1,2));
            }
            {
                // Make two triangular pillows, then join them together.
                // This crashed with 2-0 vertex moves once upon a time.
                auto [r, s, t, u] = ball_large_pillows.newTetrahedra<4>();
                r->join(0, s, Perm<4>());
                r->join(1, s, Perm<4>());
                r->join(2, s, Perm<4>());
                t->join(0, u, Perm<4>());
                t->join(1, u, Perm<4>());
                t->join(2, u, Perm<4>());
                r->join(3, t, Perm<4>());
            }
            {
                // Make three snapped balls and join them together.
                auto [r, s, t] = ball_large_snapped.newTetrahedra<3>();
                r->join(2, r, Perm<4>(2, 3));
                s->join(2, s, Perm<4>(2, 3));
                t->join(2, t, Perm<4>(2, 1));
                r->join(1, s, Perm<4>());
                s->join(0, t, Perm<4>());
            }

            // Build disconnected triangulations from others that we
            // already have.
            disjoint2.insertTriangulation(gieseking);
            disjoint2.insertTriangulation(cuspedGenusTwoTorus);

            disjoint3.insertTriangulation(sphereBundle);
            disjoint3.insertTriangulation(ball_large_pillows);
            disjoint3.insertTriangulation(figure8);
        }

        void tearDown() override {
        }

        /**
         * Run a given test over all hand-coded test cases that are not
         * obscenely large.
         */
        void testManualSmall(Triangulation3TestFunction f) {
            f(empty, "Empty triangulation");
            f(ball, "Single tetrahedron");
            f(s3, "S^3");
            f(sphere, "Generic S^3");
            f(simplicialSphere, "Simplicial S^3");
            f(sphereBundle, "S^2 x S^1");
            f(twistedSphereBundle, "S^2 x~ S^1");
            f(rp3_1, "RP^3 (1 vtx)");
            f(rp3_2, "RP^3 (2 vtx)");
            f(lens3_1, "L(3,1)");
            f(lens8_3, "L(8,3)");
            f(lens7_1_loop, "Layered loop L(7,1)");
            f(rp3rp3, "RP^3 # RP^3");
            f(q32xz3, "S^3 / Q_32 x Z_3");
            f(q28, "S^3 / Q_28");
            f(weberSeifert, "Weber-Seifert");
            f(lst3_4_7, "LST(3,4,7)");
            f(figure8, "Figure eight");
            f(trefoil, "Trefoil");
            f(rp2xs1, "RP^2 x S^1");
            f(ballBundle, "Solid torus");
            f(twistedBallBundle, "Solid Klein bottle");
            f(gieseking, "Gieseking");
            f(invalidEdges, "Triangulation with invalid edges");
            f(twoProjPlaneCusps, "Triangulation with RP^2 cusps");
            f(cuspedGenusTwoTorus, "Cusped solid genus 2 torus");
            f(pinchedSolidTorus, "Pinched solid torus");
            f(pinchedSolidKB, "Pinched solid Klein bottle");
            f(s3_large, "Large S^3");
            f(rp3_large, "Large RP^3");
            f(lens8_3_large, "Large L(8,3)");
            f(q20_large, "Large S^3 / Q_20");
            f(ball_large, "4-tetrahedron ball");
            f(ball_large_pillows, "4-tetrahedron pillow ball");
            f(ball_large_snapped, "3-tetrahedron snapped ball");
            f(singleTet_bary, "Subdivided tetrahedron");
            f(fig8_bary, "Subdivided figure eight");
            f(disjoint2, "Gieseking U (cusped genus 2 torus)");
            f(disjoint3,
                "(S^2 x S^1) U (B^3) U (Figure eight knot complement)");
        }

        /**
         * Run a given test over all hand-coded test cases.
         */
        void testManualAll(Triangulation3TestFunction f) {
            testManualSmall(f);
            f(lens100_1, "L(100,1)");
        }

        void copyMove() {
            testManualAll(verifyCopyMove);
        }

        void makeCanonical() {
            testManualAll(verifyMakeCanonical);
        }

        void isomorphismSignature() {
            testManualAll(verifyIsomorphismSignature);
        }

        void orient() {
            testManualAll(verifyOrient);
        }

        void doubleCover() {
            testManualAll(verifyDoubleCover);
        }

        void boundaryTriangles() {
            testManualAll(verifyBoundaryFacets);
        }

        void boundaryBuild() {
            testManualAll(verifyBoundaryBuild);
        }

        void validity() {
            verifyValid(empty, true, "Empty triangulation");
            verifyValid(sphere, true, "Generic S^3");
            verifyValid(simplicialSphere, true, "Simplicial S^3");
            verifyValid(sphereBundle, true, "S^2 x S^1");
            verifyValid(twistedSphereBundle, true, "S^2 x~ S^1");
            verifyValid(ball, true, "Single tetrahedron");
            verifyValid(ballBundle, true, "Solid torus");
            verifyValid(twistedBallBundle, true, "Solid Klein bottle");
            verifyValid(s3, true, "S^3");
            verifyValid(rp3_1, true, "RP^3 (1 vtx)");
            verifyValid(rp3_2, true, "RP^3 (2 vtx)");
            verifyValid(lens3_1, true, "L(3,1)");
            verifyValid(lens7_1_loop, true, "Layered loop L(7,1)");
            verifyValid(lens8_3, true, "L(8,3)");
            verifyValid(lens8_3_large, true, "Large L(8,3)");
            verifyValid(rp3rp3, true, "RP^3 # RP^3");
            verifyValid(q28, true, "S^3 / Q_28");
            verifyValid(weberSeifert, true, "Weber-Seifert");
            verifyValid(q32xz3, true, "S^3 / Q_32 x Z_3");
            verifyValid(lens100_1, true, "L(100,1)");
            verifyValid(ball_large, true, "4-tetrahedron ball");
            verifyValid(ball_large_pillows, true, "4-tetrahedron pillow ball");
            verifyValid(ball_large_snapped, true, "3-tetrahedron snapped ball");
            verifyValid(lst3_4_7, true, "LST(3,4,7)");
            verifyValid(figure8, true, "Figure eight");
            verifyValid(trefoil, true, "Trefoil");
            verifyValid(knot18, true, "18-crossing knot");
            verifyValid(rp2xs1, true, "RP^2 x S^1");
            verifyValid(gieseking, true, "Gieseking");
            verifyValid(invalidEdges, false,
                "Triangulation with invalid edges");
            verifyValid(twoProjPlaneCusps, true,
                "Triangulation with RP^2 cusps");
            verifyValid(cuspedGenusTwoTorus, true,
                "Cusped solid genus 2 torus");
            verifyValid(pinchedSolidTorus, false, "Pinched solid torus");
            verifyValid(pinchedSolidKB, false, "Pinched solid Klein bottle");
            verifyValid(s3_large, true, "Large S^3");
            verifyValid(rp3_large, true, "Large RP^3");
            verifyValid(lens8_3_large, true, "Large L(8,3)");
            verifyValid(q20_large, true, "Large S^3 / Q_20");
            verifyValid(singleTet_bary, true, "Subdivided tetrahedron");
            verifyValid(fig8_bary, true, "Subdivided figure eight");
            verifyValid(disjoint2, true, "Gieseking U (cusped genus 2 torus)");
            verifyValid(disjoint3, true,
                "(S^2 x S^1) U (B^3) U (Figure eight knot complement)");
        }

        void connectedness() {
            verifyConnected(empty, true, "Empty triangulation");
            verifyConnected(sphere, true, "Generic S^3");
            verifyConnected(simplicialSphere, true, "Simplicial S^3");
            verifyConnected(sphereBundle, true, "S^2 x S^1");
            verifyConnected(twistedSphereBundle, true, "S^2 x~ S^1");
            verifyConnected(ball, true, "Single tetrahedron");
            verifyConnected(ballBundle, true, "Solid torus");
            verifyConnected(twistedBallBundle, true, "Solid Klein bottle");
            verifyConnected(s3, true, "S^3");
            verifyConnected(rp3_1, true, "RP^3 (1 vtx)");
            verifyConnected(rp3_2, true, "RP^3 (2 vtx)");
            verifyConnected(lens3_1, true, "L(3,1)");
            verifyConnected(lens7_1_loop, true, "Layered loop L(7,1)");
            verifyConnected(lens8_3, true, "L(8,3)");
            verifyConnected(lens8_3_large, true, "Large L(8,3)");
            verifyConnected(rp3rp3, true, "RP^3 # RP^3");
            verifyConnected(q28, true, "S^3 / Q_28");
            verifyConnected(weberSeifert, true, "Weber-Seifert");
            verifyConnected(q32xz3, true, "S^3 / Q_32 x Z_3");
            verifyConnected(lens100_1, true, "L(100,1)");
            verifyConnected(ball_large, true, "4-tetrahedron ball");
            verifyConnected(ball_large_pillows, true,
                "4-tetrahedron pillow ball");
            verifyConnected(ball_large_snapped, true,
                "3-tetrahedron snapped ball");
            verifyConnected(lst3_4_7, true, "LST(3,4,7)");
            verifyConnected(figure8, true, "Figure eight");
            verifyConnected(trefoil, true, "Trefoil");
            verifyConnected(knot18, true, "18-crossing knot");
            verifyConnected(rp2xs1, true, "RP^2 x S^1");
            verifyConnected(gieseking, true, "Gieseking");
            verifyConnected(invalidEdges, true,
                "Triangulation with invalid edges");
            verifyConnected(twoProjPlaneCusps, true,
                "Triangulation with RP^2 cusps");
            verifyConnected(cuspedGenusTwoTorus, true,
                "Cusped solid genus 2 torus");
            verifyConnected(pinchedSolidTorus, true, "Pinched solid torus");
            verifyConnected(pinchedSolidKB, true, "Pinched solid Klein bottle");
            verifyConnected(s3_large, true, "Large S^3");
            verifyConnected(rp3_large, true, "Large RP^3");
            verifyConnected(lens8_3_large, true, "Large L(8,3)");
            verifyConnected(q20_large, true, "Large S^3 / Q_20");
            verifyConnected(singleTet_bary, true, "Subdivided tetrahedron");
            verifyConnected(fig8_bary, true, "Subdivided figure eight");
            verifyConnected(disjoint2, false,
                "Gieseking U (cusped genus 2 torus)");
            verifyConnected(disjoint3, false,
                "(S^2 x S^1) U (B^3) U (Figure eight knot complement)");
        }

        void orientability() {
            verifyOrientable(empty, true, "Empty triangulation");
            verifyOrientable(sphere, true, "Generic S^3");
            verifyOrientable(simplicialSphere, true, "Simplicial S^3");
            verifyOrientable(sphereBundle, true, "S^2 x S^1");
            verifyOrientable(twistedSphereBundle, false, "S^2 x~ S^1");
            verifyOrientable(ball, true, "Single tetrahedron");
            verifyOrientable(ballBundle, true, "Solid torus");
            verifyOrientable(twistedBallBundle, false, "Solid Klein bottle");
            verifyOrientable(s3, true, "S^3");
            verifyOrientable(rp3_1, true, "RP^3 (1 vtx)");
            verifyOrientable(rp3_2, true, "RP^3 (2 vtx)");
            verifyOrientable(lens3_1, true, "L(3,1)");
            verifyOrientable(lens7_1_loop, true, "Layered loop L(7,1)");
            verifyOrientable(lens8_3, true, "L(8,3)");
            verifyOrientable(lens8_3_large, true, "Large L(8,3)");
            verifyOrientable(rp3rp3, true, "RP^3 # RP^3");
            verifyOrientable(q28, true, "S^3 / Q_28");
            verifyOrientable(weberSeifert, true, "Weber-Seifert");
            verifyOrientable(q32xz3, true, "S^3 / Q_32 x Z_3");
            verifyOrientable(lens100_1, true, "L(100,1)");
            verifyOrientable(ball_large, true, "4-tetrahedron ball");
            verifyOrientable(ball_large_pillows, true,
                "4-tetrahedron pillow ball");
            verifyOrientable(ball_large_snapped, true,
                "3-tetrahedron snapped ball");
            verifyOrientable(lst3_4_7, true, "LST(3,4,7)");
            verifyOrientable(figure8, true, "Figure eight");
            verifyOrientable(trefoil, true, "Trefoil");
            verifyOrientable(knot18, true, "18-crossing knot");
            verifyOrientable(rp2xs1, false, "RP^2 x S^1");
            verifyOrientable(gieseking, false, "Gieseking");
            verifyOrientable(invalidEdges, false,
                "Triangulation with invalid edges");
            verifyOrientable(twoProjPlaneCusps, false,
                "Triangulation with RP^2 cusps");
            verifyOrientable(cuspedGenusTwoTorus, true,
                "Cusped solid genus 2 torus");
            verifyOrientable(pinchedSolidTorus, true, "Pinched solid torus");
            verifyOrientable(pinchedSolidKB, false,
                "Pinched solid Klein bottle");
            verifyOrientable(s3_large, true, "Large S^3");
            verifyOrientable(rp3_large, true, "Large RP^3");
            verifyOrientable(lens8_3_large, true, "Large L(8,3)");
            verifyOrientable(q20_large, true, "Large S^3 / Q_20");
            verifyOrientable(singleTet_bary, true, "Subdivided tetrahedron");
            verifyOrientable(fig8_bary, true, "Subdivided figure eight");
            verifyOrientable(disjoint2, false,
                "Gieseking U (cusped genus 2 torus)");
            verifyOrientable(disjoint3, true,
                "(S^2 x S^1) U (B^3) U (Figure eight knot complement)");
        }

        void standardness() {
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not standard.",
                empty.isStandard());
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron is not standard.",
                ball.isStandard());
            CPPUNIT_ASSERT_MESSAGE("S^3 is not standard.",
                s3.isStandard());
            CPPUNIT_ASSERT_MESSAGE("Generic S^3 is not standard.",
                sphere.isStandard());
            CPPUNIT_ASSERT_MESSAGE("Simplicial S^3 is not standard.",
                simplicialSphere.isStandard());
            CPPUNIT_ASSERT_MESSAGE("S^2 x S^1 is not standard.",
                sphereBundle.isStandard());
            CPPUNIT_ASSERT_MESSAGE("S^2 x~ S^1 is not standard.",
                twistedSphereBundle.isStandard());
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
            CPPUNIT_ASSERT_MESSAGE("The trefoil knot complement "
                "is not standard.",
                trefoil.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The 18-crossing knot complement "
                "is not standard.",
                knot18.isStandard());
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 is not standard.",
                rp2xs1.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The solid torus is not standard.",
                ballBundle.isStandard());
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle is not standard.",
                twistedBallBundle.isStandard());
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

        void verifyBoundaryEuler(const Triangulation<3>& tri,
                std::initializer_list<int> expect, const char* name) {
            // The argument expect holds the Euler characteristics we would
            // expect after triangulating the boundary components.

            if (tri.countBoundaryComponents() != expect.size()) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " has " << tri.countBoundaryComponents()
                    << " boundary components instead of the expected "
                    << expect.size() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            size_t i = 0;
            for (auto eit = expect.begin(); eit != expect.end(); ++i, ++eit) {
                BoundaryComponent<3>* bc = tri.boundaryComponent(i);

                // Do we have any pinched vertices?
                // If so, these interfere with the Euler characteristic count.
                long vPinch = 0;
                if (bc->isReal())
                    for (auto v : bc->vertices())
                        if (! v->isValid()) {
                            long punctures = v->buildLink().
                                countBoundaryComponents();
                            if (punctures > 1)
                                vPinch += (punctures - 1);
                        }

                long foundEuler = bc->eulerChar();
                long triEuler = bc->build().eulerCharTri();

                if (foundEuler != *eit - vPinch) {
                    std::ostringstream msg;
                    msg << "Boundary component " << i
                        << " of triangulation " << name
                        << " reports Euler characteristic " << foundEuler
                        << " instead of the expected "
                        << (*eit - vPinch) << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                if (triEuler != *eit) {
                    std::ostringstream msg;
                    msg << "Triangulated boundary component " << i
                        << " of triangulation " << name
                        << " has Euler characteristic " << triEuler
                        << " instead of the expected "
                        << *eit << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void boundaryComponents() {
            verifyBoundaryCount(empty, 0, 0, 0, "Empty triangulation");
            verifyBoundaryCount(sphere, 0, 0, 0, "Generic S^3");
            verifyBoundaryCount(simplicialSphere, 0, 0, 0, "Simplicial S^3");
            verifyBoundaryCount(sphereBundle, 0, 0, 0, "S^2 x S^1");
            verifyBoundaryCount(twistedSphereBundle, 0, 0, 0, "S^2 x~ S^1");
            verifyBoundaryCount(s3, 0, 0, 0, "S^3");
            verifyBoundaryCount(rp3_1, 0, 0, 0, "RP^3 (1 vtx)");
            verifyBoundaryCount(rp3_2, 0, 0, 0, "RP^3 (2 vtx)");
            verifyBoundaryCount(lens3_1, 0, 0, 0, "L(3,1)");
            verifyBoundaryCount(lens8_3, 0, 0, 0, "L(8,3)");
            verifyBoundaryCount(lens7_1_loop, 0, 0, 0, "Layered loop L(7,1)");
            verifyBoundaryCount(rp3rp3, 0, 0, 0, "RP^3 # RP^3");
            verifyBoundaryCount(q32xz3, 0, 0, 0, "S^3 / Q_32 x Z_3");
            verifyBoundaryCount(q28, 0, 0, 0, "S^3 / Q_28");
            verifyBoundaryCount(weberSeifert, 0, 0, 0, "Weber-Seifert");
            verifyBoundaryCount(lens100_1, 0, 0, 0, "L(100,1)");
            verifyBoundaryCount(rp2xs1, 0, 0, 0, "RP^2 x S^1");
            verifyBoundaryCount(invalidEdges, 0, 0, 0,
                "Triangulation with invalid edges");
            verifyBoundaryCount(s3_large, 0, 0, 0, "Large S^3");
            verifyBoundaryCount(rp3_large, 0, 0, 0, "Large RP^3");
            verifyBoundaryCount(lens8_3_large, 0, 0, 0, "Large L(8,3)");
            verifyBoundaryCount(q20_large, 0, 0, 0, "Large S^3 / Q_20");

            verifyBoundaryCount(ball, 1, 0, 0, "Single tetrahedron");
            verifyBoundaryEuler(ball, { 2 }, "Single tetrahedron");
            verifyBoundaryCount(ballBundle, 1, 0, 0, "Solid torus");
            verifyBoundaryEuler(ballBundle, { 0 }, "Solid torus");
            verifyBoundaryCount(twistedBallBundle, 1, 0, 0,
                "Solid Klein bottle");
            verifyBoundaryEuler(twistedBallBundle, { 0 }, "Solid Klein bottle");
            verifyBoundaryCount(lst3_4_7, 1, 0, 0, "LST(3,4,7)");
            verifyBoundaryEuler(lst3_4_7, { 0 }, "LST(3,4,7)");
            verifyBoundaryCount(ball_large, 1, 0, 0, "4-tetrahedron ball");
            verifyBoundaryEuler(ball_large, { 2 }, "4-tetrahedron ball");
            verifyBoundaryCount(ball_large_pillows, 1, 0, 0,
                "4-tetrahedron pillow ball");
            verifyBoundaryEuler(ball_large_pillows, { 2 },
                "4-tetrahedron pillow ball");
            verifyBoundaryCount(ball_large_snapped, 1, 0, 0,
                "3-tetrahedron snapped ball");
            verifyBoundaryEuler(ball_large_snapped, { 2 },
                "3-tetrahedron snapped ball");
            verifyBoundaryCount(singleTet_bary, 1, 0, 0,
                "Subdivided tetrahedron");
            verifyBoundaryEuler(singleTet_bary, { 2 },
                "Subdivided tetrahedron");

            verifyBoundaryCount(figure8, 0, 1, 0, "Figure eight");
            verifyBoundaryEuler(figure8, { 0 }, "Figure eight");
            verifyBoundaryCount(trefoil, 0, 1, 0, "Trefoil");
            verifyBoundaryEuler(trefoil, { 0 }, "Trefoil");
            verifyBoundaryCount(knot18, 0, 1, 0, "18-crossing knot");
            verifyBoundaryEuler(knot18, { 0 }, "18-crossing knot");
            verifyBoundaryCount(gieseking, 0, 1, 0, "Gieseking");
            verifyBoundaryEuler(gieseking, { 0 }, "Gieseking");
            verifyBoundaryCount(twoProjPlaneCusps, 0, 2, 0,
                "Triangulation with RP^2 cusps");
            verifyBoundaryEuler(twoProjPlaneCusps, { 1, 1 },
                "Triangulation with RP^2 cusps");
            verifyBoundaryCount(cuspedGenusTwoTorus, 0, 1, 0,
                "Cusped solid genus 2 torus");
            verifyBoundaryEuler(cuspedGenusTwoTorus, { -2 },
                "Cusped solid genus 2 torus");
            verifyBoundaryCount(fig8_bary, 0, 1, 0, "Subdivided figure eight");
            verifyBoundaryEuler(fig8_bary, { 0 }, "Subdivided figure eight");

            verifyBoundaryCount(pinchedSolidTorus, 1, 0, 0,
                "Pinched solid torus");
            // The boundary triangulates to a sphere, but the pinched
            // vertex gets duplicated when we triangulate this sphere.
            verifyBoundaryEuler(pinchedSolidTorus, { 2 },
                "Pinched solid torus");
            verifyBoundaryCount(pinchedSolidKB, 1, 0, 0,
                "Pinched solid Klein bottle");
            // The boundary triangulates to RP2, and the pinched vertex
            // does not get duplicated in the process.
            verifyBoundaryEuler(pinchedSolidKB, { 1 },
                "Pinched solid Klein bottle");

            verifyBoundaryCount(disjoint2, 0, 2, 0,
                "Gieseking U (cusped genus 2 torus)");
            verifyBoundaryEuler(disjoint2, { 0, -2 },
                "Gieseking U (cusped genus 2 torus)");
            verifyBoundaryCount(disjoint3, 1, 1, 0,
                "(S^2 x S^1) U (B^3) U (Figure eight knot complement)");
            verifyBoundaryEuler(disjoint3, { 2, 0 },
                "(S^2 x S^1) U (B^3) U (Figure eight knot complement)");
        }

        void verifyVertexCount(Triangulation<3>& tri, unsigned nVertices,
                const char* triName) {
            if (tri.countVertices() != nVertices) {
                std::ostringstream msg;
                msg << triName << " has " << tri.countVertices()
                    << " vertices, not " << nVertices << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyVertexDisc(Triangulation<3>& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<3>* v = tri.vertex(vertex);

            if (v->linkType() != Vertex<3>::DISC) {
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

        void verifyVertexSphere(Triangulation<3>& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<3>* v = tri.vertex(vertex);

            if (v->linkType() != Vertex<3>::SPHERE) {
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

        void verifyVertexTorus(Triangulation<3>& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<3>* v = tri.vertex(vertex);

            if (v->linkType() != Vertex<3>::TORUS) {
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

        void verifyVertexKB(Triangulation<3>& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<3>* v = tri.vertex(vertex);

            if (v->linkType() != Vertex<3>::KLEIN_BOTTLE) {
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

        void verifyVertexTorusG2(Triangulation<3>& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<3>* v = tri.vertex(vertex);

            if (v->linkType() != Vertex<3>::NON_STANDARD_CUSP) {
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

        void verifyVertexProjPlane(Triangulation<3>& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<3>* v = tri.vertex(vertex);

            if (v->linkType() != Vertex<3>::NON_STANDARD_CUSP) {
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

        void verifyVertexAnnulus(Triangulation<3>& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<3>* v = tri.vertex(vertex);

            if (v->linkType() != Vertex<3>::INVALID) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not listed as INVALID.";
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

        void verifyVertexMobius(Triangulation<3>& tri, unsigned vertex,
                const char* triName) {
            if (vertex >= tri.countVertices()) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " does not exist.  Only " << tri.countVertices()
                    << " vertices are available.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<3>* v = tri.vertex(vertex);

            if (v->linkType() != Vertex<3>::INVALID) {
                std::ostringstream msg;
                msg << triName << ", vertex " << vertex
                    << " link is not listed as INVALID.";
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
            verifyVertexCount(ball, 4, "Single tetrahedron");
            verifyVertexDisc(ball, 0, "Single tetrahedron");
            verifyVertexDisc(ball, 1, "Single tetrahedron");
            verifyVertexDisc(ball, 2, "Single tetrahedron");
            verifyVertexDisc(ball, 3, "Single tetrahedron");

            verifyVertexCount(s3, 1, "S^3");
            verifyVertexSphere(s3, 0, "S^3");

            verifyVertexCount(sphere, 4, "Generic S^3");
            verifyVertexSphere(sphere, 0, "Generic S^3");
            verifyVertexSphere(sphere, 1, "Generic S^3");
            verifyVertexSphere(sphere, 2, "Generic S^3");
            verifyVertexSphere(sphere, 3, "Generic S^3");

            verifyVertexCount(simplicialSphere, 5, "Simplicial S^3");
            verifyVertexSphere(simplicialSphere, 0, "Simplicial S^3");
            verifyVertexSphere(simplicialSphere, 1, "Simplicial S^3");
            verifyVertexSphere(simplicialSphere, 2, "Simplicial S^3");
            verifyVertexSphere(simplicialSphere, 3, "Simplicial S^3");
            verifyVertexSphere(simplicialSphere, 4, "Simplicial S^3");

            verifyVertexCount(sphereBundle, 1, "S^2 x S^1");
            verifyVertexSphere(sphereBundle, 0, "S^2 x S^1");

            verifyVertexCount(twistedSphereBundle, 1, "S^2 x~ S^1");
            verifyVertexSphere(twistedSphereBundle, 0, "S^2 x~ S^1");

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

            verifyVertexCount(weberSeifert, 1, "Weber-Seifert");
            verifyVertexSphere(weberSeifert, 0, "Weber-Seifert");

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

            verifyVertexCount(figure8, 1, "Figure eight");
            verifyVertexTorus(figure8, 0, "Figure eight");
            verifyVertexCount(trefoil, 1, "Trefoil");
            verifyVertexTorus(trefoil, 0, "Trefoil");
            verifyVertexCount(knot18, 1, "18-crossing knot");
            verifyVertexTorus(knot18, 0, "18-crossing knot");

            verifyVertexCount(rp2xs1, 1, "RP^2 x S^1");
            verifyVertexSphere(rp2xs1, 0, "RP^2 x S^1");

            verifyVertexCount(ballBundle, 1, "Solid torus");
            verifyVertexDisc(ballBundle, 0, "Solid torus");

            verifyVertexCount(twistedBallBundle, 2, "Solid Klein bottle");
            verifyVertexDisc(twistedBallBundle, 0, "Solid Klein bottle");
            verifyVertexDisc(twistedBallBundle, 1, "Solid Klein bottle");

            verifyVertexCount(gieseking, 1, "Gieseking");
            verifyVertexKB(gieseking, 0, "Gieseking");

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
            verifyVertexProjPlane(twoProjPlaneCusps, 1,
                "Triangulation with RP^2 cusps");
            verifyVertexSphere(twoProjPlaneCusps, 2,
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

        static void verifyVertexLinks(const Triangulation<3>& tri,
                const char* name) {
            for (unsigned long i = 0; i < tri.countVertices(); ++i) {
                Vertex<3>* v = tri.vertex(i);

                const Triangulation<2>& link = v->buildLink();
                Isomorphism<3> iso = v->buildLinkInclusion();

                if (link.size() != v->degree()) {
                    std::ostringstream msg;
                    msg << name << ", vertex " << i << ": "
                        << "link has incorrect number of triangles.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! link.isConnected()) {
                    std::ostringstream msg;
                    msg << name << ", vertex " << i << ": "
                        << "link of vertex is not connected.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (v->isIdeal()) {
                    // Vertex link should be closed but not a sphere.
                    if (! link.isClosed()) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link of ideal vertex is not a closed surface.";
                        CPPUNIT_FAIL(msg.str());
                    } else if (link.eulerChar() == 2) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link of ideal vertex is a sphere.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else if (! v->isStandard()) {
                    // Vertex link should have boundary and not be a disc.
                    if (link.isClosed()) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link of invalid vertex is closed.";
                        CPPUNIT_FAIL(msg.str());
                    } else if (link.eulerChar() == 1) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link of invalid vertex is a disc.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else if (! v->isBoundary()) {
                    // Vertex link should be a sphere.
                    if (! link.isClosed()) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link of internal vertex is not closed.";
                        CPPUNIT_FAIL(msg.str());
                    } else if (link.eulerChar() != 2) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link of internal vertex is not a sphere.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else {
                    // Vertex link should be a disc.
                    if (link.isClosed() || link.eulerChar() != 1) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link of real boundary vertex is not a disc.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                unsigned j, k;
                const Tetrahedron<3>* tet;
                Perm<4> perm;
                const regina::Triangle<2> *t, *adj;
                unsigned vNum;
                for (j = 0; j < v->degree(); ++j) {
                    tet = tri.tetrahedron(iso.tetImage(j));
                    perm = iso.facePerm(j);
                    vNum = perm[3];
                    if (tet->vertex(vNum) != v) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link does not map 3 -> vertex correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (perm[0] != tet->triangleMapping(vNum)[0] ||
                            perm[1] != tet->triangleMapping(vNum)[1] ||
                            perm[2] != tet->triangleMapping(vNum)[2]) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link does not map 0,1,2 -> opposite "
                            "triangle correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    for (k = 0; k < 3; ++k) {
                        t = link.triangle(j);
                        adj = t->adjacentTriangle(k);
                        if (adj) {
                            if (! tet->adjacentTetrahedron(perm[k])) {
                                std::ostringstream msg;
                                msg << name
                                    << ", vertex " << i << ": "
                                    << "link has extra adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            } else if (tet->adjacentTetrahedron(perm[k]) !=
                                    tri.tetrahedron(iso.tetImage(
                                    adj->index()))) {
                                std::ostringstream msg;
                                msg << name
                                    << ", vertex " << i << ": "
                                    << "link has wrong adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            } else if (tet->adjacentGluing(perm[k]) !=
                                    iso.facetPerm(adj->index()) *
                                    Perm<4>::extend(t->adjacentGluing(k)) *
                                    perm.inverse()) {
                                std::ostringstream msg;
                                msg << name
                                    << ", vertex " << i << ": "
                                    << "link has wrong adjacent gluing.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        } else {
                            if (tet->adjacentTetrahedron(perm[k])) {
                                std::ostringstream msg;
                                msg << name
                                    << ", vertex " << i << ": "
                                    << "link missing adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        }
                    }
                }
            }
        }

        void vertexLinks() {
            testManualAll(verifyVertexLinks);
            runCensusAllClosed(verifyVertexLinks);
            runCensusAllBounded(verifyVertexLinks);
            runCensusAllIdeal(verifyVertexLinks);
        }

        void verifyEuler(const Triangulation<3>& tri,
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
            verifyEuler(ball, 1, 1, "Single tetrahedron");
            verifyEuler(s3, 0, 0, "S^3");
            verifyEuler(sphere, 0, 0, "Generic S^3");
            verifyEuler(simplicialSphere, 0, 0, "Simplicial S^3");
            verifyEuler(sphereBundle, 0, 0, "S^2 x S^1");
            verifyEuler(twistedSphereBundle, 0, 0, "S^2 x~ S^1");
            verifyEuler(rp3_1, 0, 0, "RP^3 (1 vtx)");
            verifyEuler(rp3_2, 0, 0, "RP^3 (2 vtx)");
            verifyEuler(lens3_1, 0, 0, "L(3,1)");
            verifyEuler(lens8_3, 0, 0, "L(8,3)");
            verifyEuler(lens7_1_loop, 0, 0, "Layered loop L(7,1)");
            verifyEuler(rp3rp3, 0, 0, "RP^3 # RP^3");
            verifyEuler(q32xz3, 0, 0, "S^3 / Q_32 x Z_3");
            verifyEuler(q28, 0, 0, "S^3 / Q_28");
            verifyEuler(weberSeifert, 0, 0, "Weber-Seifert");
            verifyEuler(lens100_1, 0, 0, "L(100,1)");
            verifyEuler(lst3_4_7, 0, 0, "LST(3,4,7)");
            verifyEuler(figure8, 0, 1, "Figure eight");
            verifyEuler(trefoil, 0, 1, "Trefoil");
            verifyEuler(knot18, 0, 1, "18-crossing knot");
            verifyEuler(rp2xs1, 0, 0, "RP^2 x S^1");
            verifyEuler(ballBundle, 0, 0, "Solid torus");
            verifyEuler(twistedBallBundle, 0, 0, "Solid Klein bottle");
            verifyEuler(gieseking, 0, 1, "Gieseking");

            verifyEuler(invalidEdges, 1, -1,
                "Triangulation with invalid edges");
            verifyEuler(twoProjPlaneCusps, 1, 1,
                "Triangulation with RP^2 cusps");
            verifyEuler(cuspedGenusTwoTorus, -1, 2,
                "Cusped solid genus two torus");
            verifyEuler(pinchedSolidTorus, 0, 1, "Pinched solid torus");
            verifyEuler(pinchedSolidKB, 0, 1, "Pinched solid Klein bottle");

            {
                Triangulation<3> t(twoProjPlaneCusps);
                t.idealToFinite();
                verifyEuler(t, 1, 1, "Triangulation with RP^2 boundaries");
            }
            {
                Triangulation<3> t(cuspedGenusTwoTorus);
                t.idealToFinite();
                verifyEuler(t, -1, -1, "Solid genus two torus");
            }
            {
                Triangulation<3> t(pinchedSolidTorus);
                t.idealToFinite();
                verifyEuler(t, 0, 0, "Unpinched solid torus");
            }
            {
                Triangulation<3> t(pinchedSolidKB);
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
            verifyEuler(fig8_bary, 0, 1, "Subdivided figure eight");
        }

        void verifyGroup(const AbelianGroup& g, const std::string& grpName,
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

        void verifyGroup(const AbelianGroup& g, const std::string& grpName,
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
                        g.invariantFactor(0) != torsionDegree)
                    CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyGroup(const AbelianGroup& g, const std::string& grpName,
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
                        g.invariantFactor(0) != torsionDegree1 ||
                        g.invariantFactor(1) != torsionDegree2)
                    CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyGroup(const AbelianGroup& g, const std::string& grpName,
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
                        g.invariantFactor(0) != torsionDegree1 ||
                        g.invariantFactor(1) != torsionDegree2 ||
                        g.invariantFactor(2) != torsionDegree3)
                    CPPUNIT_FAIL(msg.str());
            }
        }

        void homology() {
            verifyGroup(empty.homology(),
                "H1(empty triangulation)", 0);
            verifyGroup(ball.homology(),
                "H1(single tetrahedron)", 0);
            verifyGroup(s3.homology(),
                "H1(S^3)", 0);
            verifyGroup(sphere.homology(),
                "H1(Generic S^3)", 0);
            verifyGroup(simplicialSphere.homology(),
                "H1(Simplicial S^3)", 0);
            verifyGroup(sphereBundle.homology(),
                "H1(S^2 x S^1)", 1);
            verifyGroup(twistedSphereBundle.homology(),
                "H1(S^2 x~ S^1)", 1);
            verifyGroup(rp3_1.homology(),
                "H1(RP^3, 1 vtx)", 0, 2);
            verifyGroup(rp3_2.homology(),
                "H1(RP^3, 2 vtx)", 0, 2);
            verifyGroup(lens3_1.homology(),
                "H1(L(3,1))", 0, 3);
            verifyGroup(lens7_1_loop.homology(),
                "H1(Loop L(7,1))", 0, 7);
            verifyGroup(lens8_3.homology(),
                "H1(L(8,3))", 0, 8);
            verifyGroup(lens8_3_large.homology(),
                "H1(Large L(8,3))", 0, 8);
            verifyGroup(rp3rp3.homology(),
                "H1(RP^3 # RP^3)", 0, 2, 2);
            verifyGroup(q28.homology(),
                "H1(S^3 / Q_28)", 0, 4);
            verifyGroup(weberSeifert.homology(),
                "H1(SeifertWeber)", 0, 5, 5, 5);
            verifyGroup(q32xz3.homology(),
                "H1(S^3 / Q_32 x Z_3)", 0, 2, 6);
            verifyGroup(lens100_1.homology(),
                "H1(L(100,1))", 0, 100);
            verifyGroup(ball_large.homology(),
                "H1(4-tetrahedron ball)", 0);
            verifyGroup(ball_large_pillows.homology(),
                "H1(4-tetrahedron pillow ball)", 0);
            verifyGroup(ball_large_snapped.homology(),
                "H1(3-tetrahedron snapped ball)", 0);
            verifyGroup(lst3_4_7.homology(),
                "H1(LST(3,4,7))", 1);
            verifyGroup(figure8.homology(),
                "H1(figure eight)", 1);
            verifyGroup(trefoil.homology(),
                "H1(trefoil)", 1);
            verifyGroup(knot18.homology(),
                "H1(18-crossing knot)", 1);
            verifyGroup(rp2xs1.homology(),
                "H1(RP^2 x S^1)", 1, 2);
            verifyGroup(ballBundle.homology(),
                "H1(solid torus)", 1);
            verifyGroup(twistedBallBundle.homology(),
                "H1(solid Klein bottle)", 1);
            verifyGroup(gieseking.homology(),
                "H1(Gieseking)", 1);
            verifyGroup(invalidEdges.homology(),
                "H1(tri with invalid edges)", 0);
            verifyGroup(twoProjPlaneCusps.homology(),
                "H1(tri with projective plane cusps)", 0, 2);
            verifyGroup(cuspedGenusTwoTorus.homology(),
                "H1(cusped solid genus two torus)", 2);
            verifyGroup(pinchedSolidTorus.homology(),
                "H1(pinched solid torus)", 1);
            verifyGroup(pinchedSolidKB.homology(),
                "H1(pinched solid Klein bottle)", 1);
        }

        void homologyBdry() {
            verifyGroup(empty.homologyBdry(),
                "Boundary H1(empty triangulation)", 0);
            verifyGroup(ball.homologyBdry(),
                "Boundary H1(single tetrahedron)", 0);
            verifyGroup(s3.homologyBdry(),
                "Boundary H1(S^3)", 0);
            verifyGroup(sphere.homologyBdry(),
                "Boundary H1(Generic S^3)", 0);
            verifyGroup(simplicialSphere.homologyBdry(),
                "Boundary H1(Simplicial S^3)", 0);
            verifyGroup(sphereBundle.homologyBdry(),
                "Boundary H1(S^2 x S^1)", 0);
            verifyGroup(twistedSphereBundle.homologyBdry(),
                "Boundary H1(S^2 x~ S^1)", 0);
            verifyGroup(rp3_1.homologyBdry(),
                "Boundary H1(RP^3, 1 vtx)", 0);
            verifyGroup(rp3_2.homologyBdry(),
                "Boundary H1(RP^3, 2 vtx)", 0);
            verifyGroup(lens3_1.homologyBdry(),
                "Boundary H1(L(3,1))", 0);
            verifyGroup(lens7_1_loop.homologyBdry(),
                "Boundary H1(Loop L(7,1))", 0);
            verifyGroup(lens8_3.homologyBdry(),
                "Boundary H1(L(8,3))", 0);
            verifyGroup(lens8_3_large.homologyBdry(),
                "Boundary H1(Large L(8,3))", 0);
            verifyGroup(rp3rp3.homologyBdry(),
                "Boundary H1(RP^3 # RP^3)", 0);
            verifyGroup(q28.homologyBdry(),
                "Boundary H1(S^3 / Q_28)", 0);
            verifyGroup(weberSeifert.homologyBdry(),
                "Boundary H1(Weber-Seifert)", 0);
            verifyGroup(q32xz3.homologyBdry(),
                "Boundary H1(S^3 / Q_32 x Z_3)", 0);
            verifyGroup(lens100_1.homologyBdry(),
                "Boundary H1(L(100,1))", 0);
            verifyGroup(ball_large.homologyBdry(),
                "Boundary H1(4-tetrahedron ball)", 0);
            verifyGroup(ball_large_pillows.homologyBdry(),
                "Boundary H1(4-tetrahedron pillow ball)", 0);
            verifyGroup(ball_large_snapped.homologyBdry(),
                "Boundary H1(3-tetrahedron snapped ball)", 0);
            verifyGroup(lst3_4_7.homologyBdry(),
                "Boundary H1(LST(3,4,7))", 2);
            verifyGroup(figure8.homologyBdry(),
                "Boundary H1(figure eight)", 2);
            verifyGroup(trefoil.homologyBdry(),
                "Boundary H1(trefoil)", 2);
            verifyGroup(knot18.homologyBdry(),
                "Boundary H1(18-crossing knot)", 2);
            verifyGroup(rp2xs1.homologyBdry(),
                "Boundary H1(RP^2 x S^1)", 0);
            verifyGroup(ballBundle.homologyBdry(),
                "Boundary H1(solid torus)", 2);
            verifyGroup(twistedBallBundle.homologyBdry(),
                "Boundary H1(solid Klein bottle)", 1, 2);
            verifyGroup(gieseking.homologyBdry(),
                "Boundary H1(Gieseking)", 1, 2);
            verifyGroup(twoProjPlaneCusps.homologyBdry(),
                "Boundary H1(tri with projective plane cusps)", 0, 2, 2);
            verifyGroup(cuspedGenusTwoTorus.homologyBdry(),
                "Boundary H1(cusped solid genus two torus)", 4);
        }

        static void verifyFundGroupVsH1(const Triangulation<3>& tri,
                const char* name) {
            GroupPresentation pi1(tri.fundamentalGroup());

            pi1.intelligentSimplify();

            // Abelianise, and make sure we get H1.
            size_t gen = pi1.countGenerators();
            size_t rel = pi1.countRelations();

            regina::MatrixInt m(rel, gen);
            for (size_t i = 0; i < rel; ++i) {
                const regina::GroupExpression& r = pi1.relation(i);
                for (const auto& t : r.terms())
                    m.entry(i, t.generator) += t.exponent;
            }

            AbelianGroup abelian;
            abelian.addGroup(m);

            if (abelian.detail() != tri.homology().detail()) {
                std::ostringstream msg;
                msg << "Abelianised fundamental group does not match H1 "
                    "for " << name << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void fundGroupVsH1() {
            verifyFundGroupVsH1(empty, "Empty triangulation");
            verifyFundGroupVsH1(ball, "Single tetrahedron");
            verifyFundGroupVsH1(s3, "S^3");
            verifyFundGroupVsH1(sphere, "Generic S^3");
            verifyFundGroupVsH1(simplicialSphere, "Simplicial S^3");
            verifyFundGroupVsH1(sphereBundle, "S^2 x S^1");
            verifyFundGroupVsH1(twistedSphereBundle, "S^2 x~ S^1");
            verifyFundGroupVsH1(rp3_1, "RP^3 (1 vtx)");
            verifyFundGroupVsH1(rp3_2, "RP^3 (2 vtx)");
            verifyFundGroupVsH1(lens3_1, "L(3,1)");
            verifyFundGroupVsH1(lens7_1_loop, "Layered loop L(7,1)");
            verifyFundGroupVsH1(lens8_3, "L(8,3)");
            verifyFundGroupVsH1(lens8_3_large, "Large L(8,3)");
            verifyFundGroupVsH1(rp3rp3, "RP^3 # RP^3");
            verifyFundGroupVsH1(q28, "S^3 / Q_28");
            verifyFundGroupVsH1(weberSeifert, "Weber-Seifert");
            verifyFundGroupVsH1(q32xz3, "S^3 / Q_32 x Z_3");
            verifyFundGroupVsH1(lens100_1, "L(100,1)");
            verifyFundGroupVsH1(ball_large, "4-tetrahedron ball");
            verifyFundGroupVsH1(ball_large_pillows,
                "4-tetrahedron pillow ball");
            verifyFundGroupVsH1(ball_large_snapped,
                "3-tetrahedron snapped ball");
            verifyFundGroupVsH1(lst3_4_7, "LST(3,4,7)");
            verifyFundGroupVsH1(figure8, "Figure eight");
            verifyFundGroupVsH1(trefoil, "Trefoil");
            verifyFundGroupVsH1(knot18, "18-crossing knot");
            verifyFundGroupVsH1(rp2xs1, "RP^2 x S^1");
            verifyFundGroupVsH1(ballBundle, "Solid torus");
            verifyFundGroupVsH1(twistedBallBundle, "Solid Klein bottle");
            verifyFundGroupVsH1(gieseking, "Gieseking");
            verifyFundGroupVsH1(twoProjPlaneCusps,
                "Triangulation with RP^2 cusps");
            verifyFundGroupVsH1(cuspedGenusTwoTorus,
                "Cusped solid genus 2 torus");

            runCensusAllClosed(verifyFundGroupVsH1);
            runCensusAllBounded(verifyFundGroupVsH1);
            runCensusAllIdeal(verifyFundGroupVsH1);
        }

        void fundGroup() {
            verifyFundGroup(empty, "0", "Empty triangulation");
            verifyFundGroup(ball, "0", "Single tetrahedron");
            verifyFundGroup(s3, "0", "S^3");
            verifyFundGroup(sphere, "0", "Generic S^3");
            verifyFundGroup(simplicialSphere, "0", "Simplicial S^3");
            verifyFundGroup(sphereBundle, "Z", "S^2 x S^1");
            verifyFundGroup(twistedSphereBundle, "Z", "S^2 x~ S^1");
            verifyFundGroup(rp3_1, "Z_2", "RP^3 (1 vtx)");
            verifyFundGroup(rp3_2, "Z_2", "RP^2 (2 vtx)");
            verifyFundGroup(lens3_1, "Z_3", "L(3,1)");
            verifyFundGroup(lens7_1_loop, "Z_7", "Layered loop L(7,1)");
            verifyFundGroup(lens8_3, "Z_8", "L(8,3)");
            verifyFundGroup(lens8_3_large, "Z_8", "Large L(8,3)");
            verifyFundGroup(lens100_1, "Z_100", "L(100,1)");
            verifyFundGroup(ball_large, "0", "4-tetrahedron ball");
            verifyFundGroup(ball_large_pillows, "0",
                "4-tetrahedron pillow ball");
            verifyFundGroup(ball_large_snapped, "0",
                "3-tetrahedron snapped ball");
            verifyFundGroup(lst3_4_7, "Z", "LST(3,4,7)");
            verifyFundGroup(figure8,
                "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b a^-1 b^2",
                "Figure eight");
                // Note: \u21A6 is the unicode mapsto symbol.
            verifyFundGroup(trefoil,
                "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b a^-1",
                "Trefoil");
                // Note: \u21A6 is the unicode mapsto symbol.
            verifyFundGroup(rp2xs1, "Z + Z_2", "RP^2 x S^1");
            verifyFundGroup(ballBundle, "Z", "Solid torus");
            verifyFundGroup(twistedBallBundle, "Z", "Solid Klein bottle");
            verifyFundGroup(invalidEdges, "0",
                "Triangulation with invalid edges");
            verifyFundGroup(twoProjPlaneCusps, "Z_2",
                "Triangulation with RP^2 cusps");
            verifyFundGroup(cuspedGenusTwoTorus, "Free(2)",
                "Cusped solid genus 2 torus");
            verifyFundGroup(pinchedSolidTorus, "Z", "Pinched solid torus");
            verifyFundGroup(pinchedSolidKB, "Z", "Pinched solid Klein bottle");
        }

        static void testZeroEfficiency(const Triangulation<3>& tri,
                const char* name) {
            bool ans = tri.isZeroEfficient();

            if (tri.isClosed() && tri.isConnected()) {
                if (tri.countVertices() > 2 && ans) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " has >2 vertices but is reported as 0-efficient.";
                    CPPUNIT_FAIL(msg.str());
                } else if (tri.countVertices() == 2 && ans &&
                        (! tri.homology().isTrivial())) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " has 2 vertices and non-trivial homology "
                        "but is reported as 0-efficient.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            // Test it the long way, directly from the definition.
            bool expected;
            if (tri.hasTwoSphereBoundaryComponents())
                expected = false;
            else {
                expected = true;
                NormalSurfaces s(tri, regina::NS_STANDARD);
                for (const NormalSurface& f : s) {
                    if (f.eulerChar() == 2 &&
                            (! f.hasRealBoundary()) &&
                            ! f.isVertexLinking()) {
                        // Normal sphere
                        expected = false;
                        break;
                    } else if (f.eulerChar() == 1 &&
                            (! f.hasRealBoundary()) &&
                            (! f.isTwoSided()) &&
                            ! f.isVertexLinking()) {
                        // Normal projective plane that doubles to a
                        // normal sphere
                        expected = false;
                        break;
                    } else if (f.eulerChar() == 1 &&
                            f.hasRealBoundary() && ! f.isVertexLinking()) {
                        // Normal disc
                        expected = false;
                        break;
                    }
                }
            }
            if (ans && ! expected) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " is reported as 0-efficient but should not be.";
                CPPUNIT_FAIL(msg.str());
            } else if (expected && ! ans) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " is not reported as 0-efficient but should be.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void zeroEfficiency() {
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not "
                "0-efficient.", empty.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("A single tetrahedron is 0-efficient.",
                ! ball.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("S^3 is not 0-efficient.",
                s3.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("Generic S^3 is 0-efficient.",
                ! sphere.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("Simplicial S^3 is 0-efficient.",
                ! simplicialSphere.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("S^2 x S^1 is 0-efficient.",
                ! sphereBundle.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("S^2 x~ S^1 is 0-efficient.",
                ! twistedSphereBundle.isZeroEfficient());
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
            CPPUNIT_ASSERT_MESSAGE("The trefoil knot complement "
                "is not 0-efficient.",
                trefoil.isZeroEfficient());
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 is not 0-efficient.",
                rp2xs1.isZeroEfficient());
                // Contains a two-sided projective plane.
            CPPUNIT_ASSERT_MESSAGE("The solid torus is 0-efficient.",
                ! ballBundle.isZeroEfficient());
                // Contains a non-trivial disc.
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle is 0-efficient.",
                ! twistedBallBundle.isZeroEfficient());
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
            CPPUNIT_ASSERT_MESSAGE("Generic S^3 is not irreducible.",
                sphere.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("Simplicial S^3 is not irreducible.",
                simplicialSphere.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("S^2 x S^1 is irreducible.",
                ! sphereBundle.isIrreducible());
            CPPUNIT_ASSERT_MESSAGE("S^2 x~ S^1 is irreducible.",
                ! twistedSphereBundle.isIrreducible());
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

        void verifyThreeSphere(const Triangulation<3>& tri, const char* name) {
            clearProperties(tri);
            if (! tri.isSphere()) {
                std::ostringstream msg;
                msg << "The 3-sphere " << name <<
                    " is not recognised as such.";
                CPPUNIT_FAIL(msg.str());
            }

            // Try again with a barycentric subdivision.
            Triangulation<3> big(tri);
            big.barycentricSubdivision();
            clearProperties(big);
            if (! big.isSphere()) {
                std::ostringstream msg;
                msg << "The barycentric subdivision of the 3-sphere "
                    << name << " is not recognised as such.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isValid() && tri.isClosed() && tri.isOrientable() &&
                    tri.isConnected()) {
                clearProperties(tri);
                if (! tri.summands().empty()) {
                    std::ostringstream msg;
                    msg << "The 3-sphere " << name
                        << " was reported as having one or more "
                        "connected sum component.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyNotThreeSphere(const Triangulation<3>& tri,
                const char* name) {
            clearProperties(tri);
            if (tri.isSphere()) {
                std::ostringstream msg;
                msg << "The non-3-sphere " << name <<
                    " is recognised as a 3-sphere.";
                CPPUNIT_FAIL(msg.str());
            }

            // Try again with a barycentric subdivision.
            Triangulation<3> big(tri);
            big.barycentricSubdivision();
            clearProperties(big);
            if (big.isSphere()) {
                std::ostringstream msg;
                msg << "The barycentric subdivision of the non-3-sphere "
                    << name << " is recognised as a 3-sphere.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isValid() && tri.isClosed() && tri.isOrientable() &&
                    tri.isConnected()) {
                clearProperties(tri);
                if (tri.summands().empty()) {
                    std::ostringstream msg;
                    msg << "The non-3-sphere " << name
                        << " was reported as having no "
                        "connected sum components.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifySigThreeSphere(const std::string& sigStr) {
            verifyThreeSphere(Signature::parse(sigStr).triangulate(),
                sigStr.c_str());
        }

        void verifySigNotThreeSphere(const std::string& sigStr) {
            verifyNotThreeSphere(Signature::parse(sigStr).triangulate(),
                sigStr.c_str());
        }

        void verifyIsoSigThreeSphere(const std::string& sigStr) {
            verifyThreeSphere(Triangulation<3>::fromIsoSig(sigStr),
                sigStr.c_str());
        }

        void verifyIsoSigNotThreeSphere(const std::string& sigStr) {
            verifyNotThreeSphere(Triangulation<3>::fromIsoSig(sigStr),
                sigStr.c_str());
        }

        static void testThreeSphere6(const Triangulation<3>& tri,
                const char* name) {
            // PRECONDITION: tri has <= 6 tetrahedra.
            //
            // We know from the closed orientable census data that the
            // only homology 3-sphere with <= 6 tetrahedra is the
            // Poincare homology 3-sphere.  This can be distinguished
            // from the real 3-sphere using the (5,1) Turaev-Viro
            // invariant -- for S^3 the invariant is 0.138197, and for
            // the Poincare homology sphere it is 0.947214.

            // What did 3-sphere recognition say?
            clearProperties(tri);
            bool found = tri.isSphere();

            clearProperties(tri);
            bool expected = (tri.isValid() &&
                         tri.isClosed() &&
                         tri.isOrientable() &&
                         tri.isConnected() &&
                         tri.homology().isTrivial() &&
                         tri.turaevViroApprox(5, 1) < 0.5);

            if (expected && ! found) {
                std::ostringstream msg;
                msg << "The census 3-sphere " << name
                    << " was not recognised as a 3-sphere.";
                CPPUNIT_FAIL(msg.str());
            } else if (found && ! expected) {
                std::ostringstream msg;
                msg << "The census non-3-sphere " << name
                    << " was recognised as a 3-sphere.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isValid() && tri.isClosed() && tri.isOrientable() &&
                    tri.isConnected()) {
                clearProperties(tri);
                auto comps = tri.summands();
                if (expected && ! comps.empty()) {
                    std::ostringstream msg;
                    msg << "The census 3-sphere " << name
                        << " was reported as having one or more "
                        "connected sum component.";
                    CPPUNIT_FAIL(msg.str());
                } else if (comps.empty() && ! expected) {
                    std::ostringstream msg;
                    msg << "The census non-3-sphere " << name
                        << " was reported as having no "
                        "connected sum components.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void threeSphereRecognition() {
            Triangulation<3>* tri;

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
            verifyThreeSphere(Example<3>::lens(1, 0), "L(1,0)");

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
            verifyNotThreeSphere(Example<3>::poincare(),
                "Poincare homology sphere (example)");

            // Poincare homology sphere as a plugged triangular solid torus:
            tri = new Triangulation<3>;
            auto tet = tri->newTetrahedra<5>();
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
            verifyNotThreeSphere(*tri, "Poincare homology sphere (plugged)");
            delete tri;

            // Poincare homology sphere as an augmented triangular solid
            // torus:
            verifyNotThreeSphere(
                regina::Example<3>::augTriSolidTorus(2, -1, 3, 1, 5, -4),
                "Poincare homology sphere (aug I)");

            // Poincare homology sphere as another augmented triangular solid
            // torus:
            verifyNotThreeSphere(
                regina::Example<3>::augTriSolidTorus(2, -1, 3, -2, 5, 1),
                "Poincare homology sphere (aug II)");

            // Let's make sure silly things like balls aren't picked up.
            tri = new Triangulation<3>();
            tri->newTetrahedron();
            verifyNotThreeSphere(*tri, "Standalone tetrahedron");
            delete tri;

            tri = new Triangulation<3>();
            tet[0] = tri->newTetrahedron();
            tet[0]->join(0, tet[0], Perm<4>(3, 1, 2, 0));
            verifyNotThreeSphere(*tri, "Snapped tetrahedron");
            delete tri;

            tri = new Triangulation<3>();
            tri->insertLayeredSolidTorus(1, 2);
            verifyNotThreeSphere(*tri, "LST(1,2,3)");
            delete tri;

            tri = new Triangulation<3>();
            tri->insertLayeredSolidTorus(3, 4);
            verifyNotThreeSphere(*tri, "LST(3,4,7)");
            delete tri;

            tri = new Triangulation<3>();
            verifyNotThreeSphere(*tri, "Empty triangulation");
            delete tri;

            // Some disconnected examples.
            verifyNotThreeSphere(disjoint2, "Disjoint, 2 components");
            verifyNotThreeSphere(disjoint3, "Disjoint, 3 components");

            {
                Triangulation<3> s3s3 = Example<3>::lens(1,0);
                s3s3.insertTriangulation(s3s3);
                verifyNotThreeSphere(s3s3, "S^3 U S^3");
            }

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

        void verifyThreeBall(const Triangulation<3>& tri,
                const std::string& triName) {
            clearProperties(tri);
            if (! tri.isBall()) {
                CPPUNIT_FAIL(("The 3-ball " + triName +
                    " is not recognised as such.").c_str());
            }

            // Try again with a barycentric subdivision.
            Triangulation<3> big(tri);
            big.barycentricSubdivision();
            clearProperties(big);
            if (! big.isBall()) {
                CPPUNIT_FAIL(("The barycentric subdivision of the 3-ball "
                    + triName + " is not recognised as such.").c_str());
            }
        }

        void verifyNotThreeBall(const Triangulation<3>& tri,
                const std::string& triName) {
            clearProperties(const_cast<Triangulation<3>&>(tri));
            if (tri.isBall()) {
                CPPUNIT_FAIL(("The non-3-ball" + triName +
                    " is recognised as a 3-ball.").c_str());
            }

            // Try again with a barycentric subdivision.
            Triangulation<3> big(tri);
            big.barycentricSubdivision();
            clearProperties(big);
            if (big.isBall()) {
                CPPUNIT_FAIL(("The barycentric subdivision of the non-3-ball "
                    + triName + " is recognised as a 3-ball.").c_str());
            }
        }

        void threeBallRecognition() {
            // Balls:
            {
                Triangulation<3> tri;
                tri.newTetrahedron();
                verifyThreeBall(tri, "Standalone tetrahedron");
            }

            {
                Triangulation<3> tri;
                auto tet = tri.newTetrahedron();
                tet->join(0, tet, Perm<4>(3, 1, 2, 0));
                verifyThreeBall(tri, "Snapped tetrahedron");
            }

            {
                Triangulation<3> tri;
                auto tet = tri.newTetrahedra<2>();
                tet[0]->join(0, tet[1], Perm<4>());
                tet[0]->join(1, tet[1], Perm<4>());
                tet[0]->join(2, tet[1], Perm<4>());
                verifyThreeBall(tri, "Triangular pillow");
            }

            {
                // This ball used to crash the simplification routines once
                // upon a time.  Throw it into the test suite for good measure.
                Triangulation<3> tri;
                auto tet = tri.newTetrahedra<4>();
                tet[0]->join(2, tet[0], Perm<4>(0,2));
                tet[0]->join(1, tet[1], Perm<4>(2,0,1,3));
                tet[1]->join(2, tet[2], Perm<4>());
                tet[1]->join(1, tet[2], Perm<4>(2,0,1,3));
                tet[2]->join(1, tet[3], Perm<4>(2,0,1,3));
                tet[3]->join(2, tet[3], Perm<4>(1,2));
                verifyThreeBall(tri, "4-tetrahedron ball");
            }

            // Non-balls:
            verifyNotThreeBall(Example<3>::lst(1, 2), "LST(1,2,3)");
            verifyNotThreeBall(Example<3>::lst(3, 4), "LST(3,4,7)");
            verifyNotThreeBall(Triangulation<3>(), "Empty triangulation");

            // Make a punctured Poincare homology sphere.
            {
                Triangulation<3> tri = Example<3>::poincare();
                tri.barycentricSubdivision();
                tri.removeTetrahedronAt(0);
                tri.intelligentSimplify();
                verifyNotThreeBall(tri, "Punctured Poincare homology sphere");
            }

            // Throw in a couple of closed manifolds for good measure.
            verifyNotThreeBall(Example<3>::lens(1, 0), "L(1,0)");
            verifyNotThreeBall(Example<3>::lens(2, 1), "L(2,1)");
            verifyNotThreeBall(Example<3>::poincare(),
                "Poincare homology sphere");

            // Some disconnected examples.
            verifyNotThreeBall(disjoint2, "Disjoint, 2 components");
            verifyNotThreeBall(disjoint3, "Disjoint, 3 components");

            {
                Triangulation<3> tri;
                tri.newTetrahedra(2);
                verifyNotThreeBall(tri, "B^3 U B^3");
            }
        }

        static void testSolidTorus4(const Triangulation<3>& tri,
                const char* name) {
            // PRECONDITION: tri is not ideal, and has <= 4 tetrahedra.
            //
            // We know from the bounded orientable census data that the only
            // triangulations  with torus boundary and H1=Z with <= 4 tetrahedra
            // that are *not* solid tori are isosigs eHLObcdddwun and
            // eHLObcdddwuj, both representing SFS [D: (2,1) (3,-2)].

            // What did solid torus recognition say?
            clearProperties(tri);
            bool found = tri.isSolidTorus();

            clearProperties(tri);
            bool expected = (tri.isValid() &&
                         (! tri.isIdeal()) &&
                         tri.isOrientable() &&
                         tri.isConnected() &&
                         tri.countBoundaryComponents() == 1 &&
                         tri.boundaryComponent(0)->eulerChar() == 0 &&
                         tri.homology().isZ() &&
                         (tri.size() < 4 ||
                            (tri.size() == 4 &&
                            tri.isoSig() != "eHLObcdddwun" &&
                            tri.isoSig() != "eHLObcdddwuj")));

            if (expected && ! found) {
                std::ostringstream msg;
                msg << "The census solid torus " << name
                    << " was not recognised as a solid torus.";
                CPPUNIT_FAIL(msg.str());
            } else if (found && ! expected) {
                std::ostringstream msg;
                msg << "The census solid torus " << name
                    << " was recognised as a solid torus.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifySolidTorus(const Triangulation<3>& tri,
                const char* triName) {
            Triangulation<3> bounded(tri);
            if (bounded.isIdeal())
                bounded.idealToFinite();
            clearProperties(bounded);

            Triangulation<3> ideal(tri);
            if (ideal.hasBoundaryTriangles())
                ideal.finiteToIdeal();
            clearProperties(ideal);

            Triangulation<3> boundedBig(bounded);
            boundedBig.barycentricSubdivision();
            clearProperties(boundedBig);

            Triangulation<3> idealBig(ideal);
            idealBig.barycentricSubdivision();
            clearProperties(idealBig);

            if (! bounded.isSolidTorus()) {
                std::ostringstream msg;
                msg << "The real solid torus " << triName
                    << " was not recognised as such.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! ideal.isSolidTorus()) {
                std::ostringstream msg;
                msg << "The ideal solid torus " << triName
                    << " was not recognised as such.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! boundedBig.isSolidTorus()) {
                std::ostringstream msg;
                msg << "The subdivided real solid torus " << triName
                    << " was not recognised as such.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! idealBig.isSolidTorus()) {
                std::ostringstream msg;
                msg << "The subdivided ideal solid torus " << triName
                    << " was not recognised as such.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyNotSolidTorus(const Triangulation<3>& tri,
                const char* triName) {
            Triangulation<3> bounded(tri);
            if (bounded.isIdeal())
                bounded.idealToFinite();
            clearProperties(bounded);

            Triangulation<3> ideal(tri);
            if (ideal.hasBoundaryTriangles())
                ideal.finiteToIdeal();
            clearProperties(ideal);

            Triangulation<3> boundedBig(bounded);
            boundedBig.barycentricSubdivision();
            clearProperties(boundedBig);

            Triangulation<3> idealBig(ideal);
            idealBig.barycentricSubdivision();
            clearProperties(idealBig);

            if (bounded.isSolidTorus()) {
                std::ostringstream msg;
                msg << "The real non-solid-torus " << triName
                    << " was recognised as a solid torus.";
                CPPUNIT_FAIL(msg.str());
            }
            if (ideal.isSolidTorus()) {
                std::ostringstream msg;
                msg << "The ideal non-solid-torus " << triName
                    << " was recognised as a solid torus.";
                CPPUNIT_FAIL(msg.str());
            }
            if (boundedBig.isSolidTorus()) {
                std::ostringstream msg;
                msg << "The subdivided real non-solid-torus " << triName
                    << " was recognised as a solid torus.";
                CPPUNIT_FAIL(msg.str());
            }
            if (idealBig.isSolidTorus()) {
                std::ostringstream msg;
                msg << "The subdivided ideal non-solid-torus " << triName
                    << " was recognised as a solid torus.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyIsoSigSolidTorus(const std::string& sigStr) {
            verifySolidTorus(Triangulation<3>::fromIsoSig(sigStr),
                sigStr.c_str());
        }

        void verifyIsoSigNotSolidTorus(const std::string& sigStr) {
            verifyNotSolidTorus(Triangulation<3>::fromIsoSig(sigStr),
                sigStr.c_str());
        }

        void solidTorusRecognition() {
            Triangulation<3>* tri;

            verifyNotSolidTorus(Triangulation<3>(), "Empty triangulation");

            {
                Triangulation<3> tri;
                tri.newTetrahedron();
                verifyNotSolidTorus(tri, "Single tetrahedron");
            }

            {
                Triangulation<3> tri;
                Tetrahedron<3>* tet = tri.newTetrahedron();
                tet->join(0, tet, Perm<4>(3, 1, 2, 0));
                verifyNotSolidTorus(tri, "Snapped tetrahedron");
            }

            {
                Triangulation<3> tri;
                tri.insertLayeredSolidTorus(1, 2);
                verifySolidTorus(tri, "LST(1,2,3)");
            }

            {
                Triangulation<3> tri;
                tri.insertLayeredSolidTorus(1, 20);
                verifySolidTorus(tri, "LST(1,20,21)");
            }

            {
                Triangulation<3> tri;
                tri.insertLayeredSolidTorus(1, 1);
                verifySolidTorus(tri, "LST(1,1,2)");
            }

            {
                Triangulation<3> tri;
                tri.insertLayeredSolidTorus(0, 1);
                verifySolidTorus(tri, "LST(0,1,1)");
            }

            verifySolidTorus(Triangulation<3>::fromIsoSig("cMcabbgds"),
                "Ideal solid torus");

            verifyNotSolidTorus(Example<3>::figureEight(),
                "Figure 8 Knot Complement");

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
            {
                Triangulation<3> tri = Example<3>::lens(1,0);
                verifyNotSolidTorus(tri, "L(1,0)");
            }

            {
                Triangulation<3> tri = Example<3>::lens(2,1);
                verifyNotSolidTorus(tri, "L(2,1)");
            }

            verifyNotSolidTorus(Example<3>::poincare(),
                "Poincare homology sphere");

            // Some disconnected triangulations:
            verifyNotSolidTorus(disjoint2, "2-component manifold");
            verifyNotSolidTorus(disjoint3, "3-component manifold");

            {
                Triangulation<3> tri;
                tri.insertLayeredSolidTorus(1, 2);
                tri.insertLayeredSolidTorus(1, 2);
                verifyNotSolidTorus(tri, "LST U LST");
            }

            // An exhaustive census run:
            runCensusAllBounded(&testSolidTorus4);
        }

        void verifyTxI(const Triangulation<3>& tri, const char* triName) {
            Triangulation<3> bounded(tri);
            if (bounded.isIdeal())
                bounded.idealToFinite();
            clearProperties(bounded);

            Triangulation<3> ideal(tri);
            if (ideal.hasBoundaryTriangles())
                ideal.finiteToIdeal();
            clearProperties(ideal);

            Triangulation<3> boundedBig(bounded);
            boundedBig.barycentricSubdivision();
            clearProperties(boundedBig);

            Triangulation<3> idealBig(ideal);
            idealBig.barycentricSubdivision();
            clearProperties(idealBig);

            if (! bounded.isTxI()) {
                std::ostringstream msg;
                msg << "The real T^2xI " << triName
                    << " was not recognised as such.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! ideal.isTxI()) {
                std::ostringstream msg;
                msg << "The ideal T^2xI " << triName
                    << " was not recognised as such.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! boundedBig.isTxI()) {
                std::ostringstream msg;
                msg << "The subdivided real T^2xI " << triName
                    << " was not recognised as such.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! idealBig.isTxI()) {
                std::ostringstream msg;
                msg << "The subdivided ideal T^2xI " << triName
                    << " was not recognised as such.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyNotTxI(const Triangulation<3>& tri, const char* triName) {
            Triangulation<3> bounded(tri);
            if (bounded.isIdeal())
                bounded.idealToFinite();
            clearProperties(bounded);

            Triangulation<3> ideal(tri);
            if (ideal.hasBoundaryTriangles())
                ideal.finiteToIdeal();
            clearProperties(ideal);

            Triangulation<3> boundedBig(bounded);
            boundedBig.barycentricSubdivision();
            clearProperties(boundedBig);

            Triangulation<3> idealBig(ideal);
            idealBig.barycentricSubdivision();
            clearProperties(idealBig);

            if (bounded.isTxI()) {
                std::ostringstream msg;
                msg << "The real non-T^2xI " << triName
                    << " was recognised as a T^2xI.";
                CPPUNIT_FAIL(msg.str());
            }
            if (ideal.isTxI()) {
                std::ostringstream msg;
                msg << "The ideal non-T^2xI " << triName
                    << " was recognised as a T^2xI.";
                CPPUNIT_FAIL(msg.str());
            }
            if (boundedBig.isTxI()) {
                std::ostringstream msg;
                msg << "The subdivided real non-T^2xI " << triName
                    << " was recognised as a T^2xI.";
                CPPUNIT_FAIL(msg.str());
            }
            if (idealBig.isTxI()) {
                std::ostringstream msg;
                msg << "The subdivided ideal non-T^2xI " << triName
                    << " was recognised as a T^2xI.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyIsoSigTxI(const std::string& sigStr) {
            verifyTxI(Triangulation<3>::fromIsoSig(sigStr), sigStr.c_str());
        }

        void verifyIsoSigNotTxI(const std::string& sigStr) {
            verifyNotTxI(Triangulation<3>::fromIsoSig(sigStr), sigStr.c_str());
        }

        void torusXIntervalRecognition() {
            verifyTxI( Triangulation<3>::fromIsoSig("eLAkbbcddadbdb"),
                "Ideal T2xI");

            verifyNotTxI(Triangulation<3>(), "Empty triangulation");

            {
                Triangulation<3> tri;
                tri.newTetrahedron();
                verifyNotTxI(tri, "Single tetrahedron");
            }

            {
                Triangulation<3> tri;
                Tetrahedron<3>* tet = tri.newTetrahedron();
                tet->join(0, tet, Perm<4>(3, 1, 2, 0));
                verifyNotTxI(tri, "Snapped tetrahedron");
            }

            // Now we check some homology-T2xI manifolds.

            // Some links from 4^2_1 thru 7^2_8
            // in Bailey and Roth's tables from Rolfsen's *Knots and links.*
            // (5^2_1 and 7^2_8 have the same exterior.)

            verifyIsoSigNotTxI("eLPkbdcddabgbg");
            verifyIsoSigNotTxI("eLPkbdcddhgggb");
            verifyIsoSigNotTxI("eLMkbcdddaeeda");
            verifyIsoSigNotTxI("eLMkbcddddedde");
            // verifyIsoSigNotTxI("gLLMQbcdefffmvftaog");
            // verifyIsoSigNotTxI("fLLQcbecdeepuwsua");
            // verifyIsoSigNotTxI("hLLAPkbcdefgggtsfxjjgb");
            // verifyIsoSigNotTxI("hLLMPkbcdfggfgmvfafwkf");
            // verifyIsoSigNotTxI("hLLzQkcdegffgguvuqpgvk");
            // verifyIsoSigNotTxI("iLLLAQccdegfhhghdcltautwa");
            // verifyIsoSigNotTxI("kLLLALQkceffehijjijiiealshealf");
            verifyIsoSigNotTxI("eLPkbdcddabobv");

            // Finally, the connected sum of the Poincare homology sphere and T2xI:
            // verifyIsoSigNotTxI("pLvwwLuPIIIkaddkomnjlllonobabtlqinfjwjnw");
        }

        void verifyTV3(Triangulation<3>& t, const std::string& triName) {
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
                for (unsigned long i = 0; i < t.homologyH2Z2(); i++)
                    expectedTV += expectedTV;

                if (fabs(tv - expectedTV) > epsilon) {
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
            double expectedTV, tv;
            for (unsigned q0 = 1; q0 < 2 * r; q0++) {
                if (regina::gcd(q0, r) > 1)
                    continue;

                expectedTV = 2 * sin(M_PI * q0 / static_cast<double>(r));
                expectedTV = (expectedTV * expectedTV) /
                    (2 * static_cast<double>(r));

                tv = s3.turaevViroApprox(r, q0);
                if (fabs(tv - expectedTV) > epsilon) {
                    std::ostringstream msg;
                    msg << "Turaev-Viro(S^3, r = " << r
                        << ", root = " << q0
                        << ") is " << tv << ", not " << expectedTV << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                tv = sphere.turaevViroApprox(r, q0);
                if (fabs(tv - expectedTV) > epsilon) {
                    std::ostringstream msg;
                    msg << "Turaev-Viro(Generic S^3, r = " << r
                        << ", root = " << q0
                        << ") is " << tv << ", not " << expectedTV << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                tv = simplicialSphere.turaevViroApprox(r, q0);
                if (fabs(tv - expectedTV) > epsilon) {
                    std::ostringstream msg;
                    msg << "Turaev-Viro(Simplicial S^3, r = " << r
                        << ", root = " << q0
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

                if (fabs(tv - expectedTV) > epsilon) {
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

                if (fabs(tv - expectedTV) > epsilon) {
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

                double tv = sphereBundle.turaevViroApprox(r, q0);

                double expectedTV = 1.0;

                if (fabs(tv - expectedTV) > epsilon) {
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
            verifyTV3(sphere, "Generic S^3");
            verifyTV3(simplicialSphere, "Simplicial S^3");
            verifyTV3(sphereBundle, "S^2 x S^1");
            verifyTV3(twistedSphereBundle, "S^2 x~ S^1");
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

        static void verifyBary(const Triangulation<3>& tri, const char* name) {
            Triangulation<3> b(tri);
            if (b.isOrientable())
                b.orient();

            b.barycentricSubdivision();
            clearProperties(b);

            // Note that subdivisions can turn invalid into valid, but
            // they can never turn valid into invalid.
            if (tri.isValid() && ! b.isValid()) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision breaks validity.";
                CPPUNIT_FAIL(msg.str());
            }

            // Subdivisions can also turn invalid into ideal.
            // Only consider the valid -> valid case here.
            if (tri.isValid() && (tri.isIdeal() != b.isIdeal())) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision breaks idealness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.hasBoundaryTriangles() != b.hasBoundaryTriangles()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks boundary triangles.";
                CPPUNIT_FAIL(msg.str());
            }

            // As with ideal, consider valid inputs only.
            if (tri.isValid() && (tri.isClosed() != b.isClosed())) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision breaks closedness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isOrientable() != b.isOrientable()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks orientability.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isOrientable() != b.isOriented()) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision breaks orientation.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isConnected() != b.isConnected()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks connectedness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.countComponents() != b.countComponents()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks connected components.";
                CPPUNIT_FAIL(msg.str());
            }

            // Invalid vertices can become new boundary components.
            // Only consider valid inputs here.
            if (tri.isValid() && (tri.countBoundaryComponents() !=
                    b.countBoundaryComponents())) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks boundary components.";
                CPPUNIT_FAIL(msg.str());
            }

            // The same problem with invalid triangulations and boundary
            // components bites us with Euler characteristic also.
            if (tri.isValid() &&
                    (tri.eulerCharTri() != b.eulerCharTri())) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks Euler char (tri).";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isValid() &&
                    (tri.eulerCharManifold() != b.eulerCharManifold())) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks Euler char (mfd).";
                CPPUNIT_FAIL(msg.str());
            }

            // Now run more expensive tests that will be better with
            // *small* triangulations.
            if (! tri.isValid())
                return;

            b.intelligentSimplify();

            if (tri.isOrientable() != b.isOriented()) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision followed by "
                    "simplification breaks orientation.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! (tri.homology() == b.homology())) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision breaks H1.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! (tri.homologyH2() == b.homologyH2())) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision breaks H2.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void barycentricSubdivision() {
            testManualAll(verifyBary);
        }

        static void verifyIdealToFinite(const Triangulation<3>& tri,
                const char* name) {
            Triangulation<3> finite(tri);
            finite.idealToFinite();
            clearProperties(finite);

            // Are there any ideal vertices remaining?
            if (finite.isIdeal()) {
                std::ostringstream msg;
                msg << name << ": idealToFinite() leaves ideal vertices.";
                CPPUNIT_FAIL(msg.str());
            }

            // Are there any invalid vertices remaining?
            for (Vertex<3>* v : finite.vertices())
                if (v->isBoundary() && ! v->isStandard()) {
                    std::ostringstream msg;
                    msg << name << ": idealToFinite() leaves "
                        "invalid vertices .";
                    CPPUNIT_FAIL(msg.str());
                }

            // Make sure the invalid edges are left alone.
            unsigned oldInvEdges = 0, newInvEdges = 0;
            for (regina::Edge<3>* edge : tri.edges())
                if (! edge->isValid())
                    ++oldInvEdges;
            for (regina::Edge<3>* edge : finite.edges())
                if (! edge->isValid())
                    ++newInvEdges;
            if (oldInvEdges != newInvEdges) {
                std::ostringstream msg;
                msg << name << ": idealToFinite() changes "
                    "invalid edges .";
                CPPUNIT_FAIL(msg.str());
            }

            // Make sure we don't change the number of boundary components.
            if (tri.countBoundaryComponents() !=
                    finite.countBoundaryComponents()) {
                std::ostringstream msg;
                msg << name << ": idealToFinite() changes "
                    "the number of boundary components.";
                CPPUNIT_FAIL(msg.str());
            }

            // In the case of a valid triangulation, ensure that the
            // boundary components are topologically unchanged.
            if (tri.isValid()) {
                using BCSpec = std::pair<long, bool>;

                std::vector<BCSpec> bcOld;
                for (BoundaryComponent<3>* bc : tri.boundaryComponents())
                    bcOld.emplace_back(bc->eulerChar(), bc->isOrientable());
                std::sort(bcOld.begin(), bcOld.end());

                std::vector<BCSpec> bcNew;
                for (BoundaryComponent<3>* bc : finite.boundaryComponents())
                    bcNew.emplace_back(bc->eulerChar(), bc->isOrientable());
                std::sort(bcNew.begin(), bcNew.end());

                if (bcOld != bcNew) {
                    std::ostringstream msg;
                    msg << name << ": idealToFinite() changes "
                        "the topology of one or more boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void idealToFinite() {
            testManualAll(verifyIdealToFinite);
        }

        static void verifyFiniteToIdeal(const Triangulation<3>& tri,
                const char* name) {
            Triangulation<3> ideal(tri);
            ideal.finiteToIdeal();
            clearProperties(ideal);

            // Are there any boundary triangles remaining?
            if (ideal.hasBoundaryTriangles()) {
                std::ostringstream msg;
                msg << name << ": finiteToIdeal() leaves boundary triangles.";
                CPPUNIT_FAIL(msg.str());
            }

            // Make sure the invalid edges are left alone.
            unsigned oldInvEdges = 0, newInvEdges = 0;
            for (regina::Edge<3>* edge : tri.edges())
                if (! edge->isValid())
                    ++oldInvEdges;
            for (regina::Edge<3>* edge : ideal.edges())
                if (! edge->isValid())
                    ++newInvEdges;
            if (oldInvEdges != newInvEdges) {
                std::ostringstream msg;
                msg << name << ": finiteToIdeal() changes invalid edges .";
                CPPUNIT_FAIL(msg.str());
            }

            // In the case of a valid triangulation, ensure that the
            // boundary components are topologically unchanged, except
            // for sphere which must vanish.
            if (tri.isValid()) {
                using BCSpec = std::pair<long, bool>;

                std::vector<BCSpec> bcOld;
                for (BoundaryComponent<3>* bc : tri.boundaryComponents())
                    if (bc->eulerChar() != 2)
                        bcOld.emplace_back(bc->eulerChar(), bc->isOrientable());
                std::sort(bcOld.begin(), bcOld.end());

                std::vector<BCSpec> bcNew;
                for (BoundaryComponent<3>* bc : ideal.boundaryComponents())
                    bcNew.emplace_back(bc->eulerChar(), bc->isOrientable());
                std::sort(bcNew.begin(), bcNew.end());

                if (bcOld != bcNew) {
                    std::ostringstream msg;
                    msg << name << ": finiteToIdeal() changes "
                        "the topology of one or more non-sphere "
                        "boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void finiteToIdeal() {
            testManualAll(verifyFiniteToIdeal);
        }

        void pinchEdge() {
            // Start with the snapped 1-tetrahedron triangulation of the
            // 3-sphere.  Edges 0 and 2 make a Hopf link, and edge 1 is
            // just an interval.
            {
                Triangulation<3> snap;
                Tetrahedron<3>* tet = snap.newTetrahedron();
                tet->join(0, tet, Perm<4>(0, 1));
                tet->join(2, tet, Perm<4>(2, 3));

                Triangulation<3> tmp0(snap);
                tmp0.pinchEdge(tmp0.edge(0));
                if (! tmp0.isSolidTorus())
                    CPPUNIT_FAIL("Snapped 3-sphere: pinching edge 0 "
                        "does not give a solid torus.");

                Triangulation<3> tmp1(snap);
                tmp1.pinchEdge(tmp1.edge(1));
                if (! tmp1.isSphere())
                    CPPUNIT_FAIL("Snapped 3-sphere: pinching edge 1 "
                        "does not give a 3-sphere.");

                Triangulation<3> tmp2(snap);
                tmp2.pinchEdge(tmp2.edge(2));
                if (! tmp2.isSolidTorus())
                    CPPUNIT_FAIL("Snapped 3-sphere: pinching edge 2 "
                        "does not give a solid torus.");
            }

            // Move on to the layered 1-tetrahedron triangulation of the
            // 3-sphere.
            // Edge 0 forms a trefoil, and edge 1 is unknotted.
            {
                Triangulation<3> layer;
                Tetrahedron<3>* tet = layer.newTetrahedron();
                tet->join(0, tet, Perm<4>(1, 2, 3, 0));
                tet->join(2, tet, Perm<4>(2, 3));

                Triangulation<3> tmp0(layer);
                tmp0.pinchEdge(tmp0.edge(0));
                if (! (tmp0.isValid() && tmp0.isIdeal() &&
                        (tmp0.countBoundaryComponents() == 1) &&
                        (tmp0.homology().isZ()) &&
                        (tmp0.boundaryComponent(0)->isOrientable()) &&
                        (tmp0.boundaryComponent(0)->eulerChar() == 0) &&
                        (! tmp0.isSolidTorus())))
                    CPPUNIT_FAIL("Layered 3-sphere: pinching edge 0 "
                        "does not give a non-trivial knot complement.");

                Triangulation<3> tmp1(layer);
                tmp1.pinchEdge(tmp1.edge(1));
                if (! tmp1.isSolidTorus())
                    CPPUNIT_FAIL("Layered 3-sphere: pinching edge 1 "
                        "does not give a solid torus.");
            }

            // Now try a 2-tetrahedron ball, where we pinch the internal edge
            // between the two tetrahedra and then truncate the resulting
            // invalid vertex.  The result should be a solid torus.
            {
                Triangulation<3> ball;
                auto [a, b] = ball.newTetrahedra<2>();
                a->join(0, b, Perm<4>());
                a->join(1, b, Perm<4>());

                // The internal edge joins vertices 2-3.
                Triangulation<3> tmp(ball);
                tmp.pinchEdge(tmp.tetrahedron(0)->edge(5));
                tmp.idealToFinite();
                if (! tmp.isSolidTorus())
                    CPPUNIT_FAIL("2-tetrahedron ball: pinching the "
                        "internal edge does not give a solid torus.");
            }
        }

        static void verifyPuncture(const Triangulation<3>& tri,
                const char* name) {
            unsigned long n = tri.size();
            if (n == 0)
                return;

            for (unsigned long i = 0; i <= n; ++i) {
                Triangulation<3> punc(tri);
                const Tetrahedron<3>* origTet;
                if (i == n) {
                    origTet = tri.tetrahedron(0);
                    punc.puncture();
                } else {
                    origTet = tri.tetrahedron(i);
                    punc.puncture(punc.tetrahedron(i));
                }

                if (punc.size() != n + 6) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture gives wrong # tetrahedra.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isValid() != tri.isValid()) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture changes validity.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isIdeal() != tri.isIdeal()) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture changes idealness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isStandard() != tri.isStandard()) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture changes standardness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isConnected() != tri.isConnected()) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture changes connectedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isOrientable() != tri.isOrientable()) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture changes orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isOriented() != tri.isOriented()) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture changes orientedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.isClosed()) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture gives a closed triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.countBoundaryComponents() !=
                        tri.countBoundaryComponents() + 1) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture gives wrong # boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.countBoundaryTriangles() !=
                        tri.countBoundaryTriangles() + 2) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture gives wrong # boundary triangles.";
                    CPPUNIT_FAIL(msg.str());
                }

                unsigned long nPunc = punc.size();
                BoundaryComponent<3>* bc = punc.tetrahedron(nPunc - 1)->
                    triangle(0)->boundaryComponent();
                if ((! bc) || bc != punc.tetrahedron(nPunc - 2)->
                        triangle(0)->boundaryComponent()) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture gives wrong boundary triangles.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (bc->countTriangles() != 2) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture gives wrong number of S^2 triangles.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (bc->eulerChar() != 2) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture gives wrong S^2 Euler characteristic.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (punc.tetrahedron(nPunc - 1)->vertex(1) !=
                        punc.tetrahedron(nPunc - 2)->vertex(1) ||
                        punc.tetrahedron(nPunc - 1)->vertex(2) !=
                        punc.tetrahedron(nPunc - 2)->vertex(3) ||
                        punc.tetrahedron(nPunc - 1)->vertex(3) !=
                        punc.tetrahedron(nPunc - 2)->vertex(2)) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture gives wrong S^2 vertex labels.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.eulerCharTri() != tri.eulerCharTri() + 1) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture gives wrong Euler characteristic (tri).";
                    CPPUNIT_FAIL(msg.str());
                }

                if (punc.eulerCharManifold() != tri.eulerCharManifold() + 1) {
                    std::ostringstream msg;
                    msg << name << ", tet " << i << ": "
                        << "puncture gives wrong Euler characteristic (mfd).";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri.isValid()) {
                    if (! (punc.homology() == tri.homology())) {
                        std::ostringstream msg;
                        msg << name << ", tet " << i << ": "
                            << "puncture changes H1.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    AbelianGroup expectH2(tri.homologyH2());
                    AbelianGroup foundH2(punc.homologyH2());
                    Component<3>* c = origTet->component();
                    if (! c->isClosed()) {
                        // X -> X + Z
                        expectH2.addRank();
                    } else if (! c->isOrientable()) {
                        // X + Z_2 -> X + Z
                        expectH2.addRank();
                        foundH2.addTorsion(2);
                    }

                    if (foundH2 != expectH2) {
                        std::ostringstream msg;
                        msg << name << ", tet " << i << ": "
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

        static void verifyConnectedSumWithSelf(const Triangulation<3>& tri,
                const char* name) {
            if (tri.countComponents() != 1)
                return;

            Triangulation<3> t(tri);
            t.connectedSumWith(t);

            // TODO: Check that homology doubles.

            // All of our remaining tests are for closed manifolds.
            if (! tri.isClosed())
                return;

            if (! t.isClosed()) {
                std::ostringstream msg;
                msg << name << ": tri # tri is not closed.";
                CPPUNIT_FAIL(msg.str());
            }

            // We are about to start running exponential-time algorithms.
            if (tri.size() > 10)
                return;

            std::vector<Triangulation<3>> sOld, sNew;
            try {
                sOld = tri.summands();
                sNew = t.summands();
            } catch (const regina::UnsolvedCase&) {
                if (tri.isOrientable()) {
                    std::ostringstream msg;
                    msg << name << ": tri and/or tri # tri failed to "
                        "compute connected sum decomposition.";
                    CPPUNIT_FAIL(msg.str());
                } else {
                    // One of those cases where connected sum decomposition
                    // legitimately fails.  The following tests are no use
                    // for us here.
                    return;
                }
            }

            if (sNew.size() != 2 * sOld.size()) {
                std::ostringstream msg;
                msg << name << ": tri # tri has " << sNew.size()
                    << " summands, not " << (2 * sOld.size()) << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (sOld.size() == 1) {
                // There should be two summands, each homeomorphic
                // to the original.
                if (sNew[0].homology() != tri.homology() ||
                        sNew[1].homology() != tri.homology()) {
                    std::ostringstream msg;
                    msg << name << ": tri # tri has "
                        "summands with the wrong homology.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void connectedSumWithSelf() {
            testManualSmall(verifyConnectedSumWithSelf);
        }

        void verifyDehydration(const Triangulation<3>& tri, const char* name) {
            std::string str = tri.dehydrate();
            if (str.empty()) {
                std::ostringstream msg;
                msg << name << ": Cannot dehydrate.";
                CPPUNIT_FAIL(msg.str());
            }

            try {
                if (! Triangulation<3>::rehydrate(str).isIsomorphicTo(tri)) {
                    std::ostringstream msg;
                    msg << name << ": Rehydration of \"" << str
                        << "\" is not isomorphic to the original.";
                    CPPUNIT_FAIL(msg.str());
                }
            } catch (const regina::InvalidArgument&) {
                std::ostringstream msg;
                msg << name << ": Cannot rehydrate \"" << str << "\".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyNoDehydration(const Triangulation<3>& tri,
                const char* name) {
            std::string dehydrate = tri.dehydrate();
            if (! dehydrate.empty()) {
                std::ostringstream msg;
                msg << name
                    << ": Should not dehydrate, but instead dehydrates to \""
                    << dehydrate << "\".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void dehydration() {
            verifyDehydration(empty, "Empty triangulation");
            verifyNoDehydration(ball, "Single tetrahedron");
            verifyDehydration(s3, "S^3");
            verifyDehydration(sphere, "Generic S^3");
            verifyDehydration(simplicialSphere, "Simplicial S^3");
            verifyDehydration(s3_large, "Large S^3");
            verifyDehydration(sphereBundle, "S^2 x S^1");
            verifyDehydration(twistedSphereBundle, "S^2 x~ S^1");
            verifyDehydration(rp3_1, "RP^3 (1 vtx)");
            verifyDehydration(rp3_2, "RP^3 (2 vtx)");
            verifyDehydration(rp3_large, "Large RP^3");
            verifyDehydration(lens3_1, "L(3,1)");
            verifyDehydration(lens8_3, "L(8,3)");
            verifyDehydration(lens8_3_large, "Large L(8,3)");
            verifyDehydration(lens7_1_loop, "Layered loop L(7,1)");
            verifyDehydration(rp3rp3, "RP^3 # RP^3");
            verifyDehydration(q32xz3, "S^3 / Q_32 x Z_3");
            verifyDehydration(q28, "S^3 / Q_28");
            verifyDehydration(q20_large, "Large S^3 / Q_20");
            verifyDehydration(weberSeifert, "Weber-Seifert");
            verifyNoDehydration(lens100_1, "L(100,1)");
            verifyNoDehydration(ball_large, "4-tetrahedron ball");
            verifyNoDehydration(ball_large_pillows,
                "4-tetrahedron pillow ball");
            verifyNoDehydration(ball_large_snapped,
                "3-tetrahedron snapped ball");
            verifyNoDehydration(singleTet_bary, "Subdivided tetrahedron");
            verifyNoDehydration(fig8_bary, "Subdivided figure eight");
            verifyNoDehydration(lst3_4_7, "LST(3,4,7)");
            verifyDehydration(figure8, "Figure eight");
            verifyDehydration(trefoil, "Trefoil");
            verifyNoDehydration(knot18, "18-crossing knot");
            verifyDehydration(rp2xs1, "RP^2 x S^1");
            verifyNoDehydration(ballBundle, "Solid torus");
            verifyNoDehydration(twistedBallBundle, "Solid Klein bottle");
            verifyDehydration(gieseking, "Gieseking");
            verifyDehydration(invalidEdges, "Triangulation with invalid edges");
            verifyDehydration(twoProjPlaneCusps,
                "Triangulation with RP^2 cusps");
            verifyDehydration(cuspedGenusTwoTorus,
                "Cusped solid genus 2 torus");
            verifyNoDehydration(pinchedSolidTorus, "Pinched solid torus");
            verifyNoDehydration(pinchedSolidKB, "Pinched solid Klein bottle");
            verifyNoDehydration(disjoint2,
                "Gieseking U (cusped genus 2 torus)");
            verifyNoDehydration(disjoint3,
                "(S^2 x S^1) U (B^3) U (Figure eight knot complement)");
        }

        void verifySimplification(const Triangulation<3>& tri,
                unsigned simpleSize, const char* simpleName) {
            Triangulation<3> t(tri);
            if (t.isOrientable())
                t.orient();

            t.intelligentSimplify();
            clearProperties(t);

            if (t.size() != simpleSize) {
                std::ostringstream msg;
                msg << "Large triangulation should simplify to " << simpleName
                    << ", but simplifies to " << t.size()
                    << " tetrahedra instead of the expected "
                    << simpleSize << ".";
                CPPUNIT_FAIL(msg.str());
            }
            if (tri.isOrientable() != t.isOriented()) {
                std::ostringstream msg;
                msg << "Simplification to " << simpleName
                    << " breaks orientation.";
                CPPUNIT_FAIL(msg.str());
            }

            auto std = StandardTriangulation::recognise(t);
            if (! std) {
                std::ostringstream msg;
                msg << "Large triangulation should simplify to " << simpleName
                    << ", but instead simplifies to something unrecognisable.";
                CPPUNIT_FAIL(msg.str());
            }

            if (std->name() != simpleName) {
                std::ostringstream msg;
                msg << "Large triangulation should simplify to " << simpleName
                    << ", but instead simplifies to " << std->name() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            // Make sure it does not simplify any further.
            Triangulation<3> t2(t);
            if (t2.intelligentSimplify()) {
                std::ostringstream msg;
                msg << "The simple triangulation " << std->name()
                    << " should not simplify any further, but it does.";
                CPPUNIT_FAIL(msg.str());
            }
            if (t2.dumpConstruction() != t.dumpConstruction()) {
                std::ostringstream msg;
                msg << "The simple triangulation " << std->name()
                    << " should not change at all when simplified again, "
                    "but it does.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyNoSimplification(const Triangulation<3>& tri,
                size_t size, const char* name) {
            if (tri.size() != size) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " has " << tri.size() << " tetrahedra, not "
                    << size << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Triangulation<3> t(tri);
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

            // A triangulation with two degree two projective plane cusps
            // (that should not be simplified away):
            verifyNoSimplification(Triangulation<3>::rehydrate("cabbbbxww"), 2,
                "Custom two-cusped triangluation");

            {
                // A triangulation with an invalid edge that simplifies
                // (where the invalid edge must not be simplified away):
                Triangulation<3> tri;
                auto tet = tri.newTetrahedra<4>();
                tet[0]->join(3, tet[2], Perm<4>());
                tet[0]->join(2, tet[1], Perm<4>(2, 3));
                tet[3]->join(3, tet[2], Perm<4>(2, 3));
                tet[3]->join(2, tet[1], Perm<4>(1, 0));
                if (tri.isValid())
                    CPPUNIT_FAIL("Custom invalid triangulation was not built "
                        "properly.");

                tri.intelligentSimplify();
                clearProperties(tri);
                if (tri.size() != 1)
                    CPPUNIT_FAIL("Custom invalid triangulation did not "
                        "simplify to 1 tetrahedron.");
                if (tri.isValid() || tri.edge(0)->isValid())
                    CPPUNIT_FAIL("Custom invalid triangulation did not "
                        "simplify to an invalid triangulation with an "
                        "invalid edge.");
            }

            {
                // A solid torus that once upon a time was incorrectly
                // simplified away to a ball.
                Triangulation<3> tri;
                auto tet = tri.newTetrahedra<3>();
                tet[2]->join(3, tet[2], Perm<4>(2, 3));
                tet[2]->join(1, tet[1], Perm<4>(0, 2, 3, 1));
                tet[2]->join(0, tet[0], Perm<4>(3, 0, 1, 2));
                tet[1]->join(3, tet[0], Perm<4>(0, 3, 1, 2));
                tet[1]->join(1, tet[0], Perm<4>());
                if (tri.homology().str() != "Z")
                    CPPUNIT_FAIL("Custom solid torus has incorrect H1.");
                tri.intelligentSimplify();
                clearProperties(tri);
                if (tri.homology().str() != "Z")
                    CPPUNIT_FAIL("Custom solid torus simplifies to "
                        "something different.");
            }
        }

        void verifyRetriangulation(const char* isoSig,
                unsigned heightNeeded, unsigned nThreads) {
            Triangulation<3> t(isoSig);
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

        static void testReordering(const Triangulation<3>& t,
                const char* name) {
            Triangulation<3> a(t);
            a.reorderTetrahedraBFS();
            clearProperties(a);

            Triangulation<3> b(t);
            b.reorderTetrahedraBFS(true);
            clearProperties(b);

            Triangulation<3> c = Isomorphism<3>::random(t.size()).apply(t);
            clearProperties(c);

            Triangulation<3> d(c);
            d.reorderTetrahedraBFS();
            clearProperties(d);

            Triangulation<3> e(c);
            e.reorderTetrahedraBFS(true);
            clearProperties(e);

            if (! t.isIsomorphicTo(a)) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " changes its isomorphism class when its tetrahedra "
                    "are reordered in the forward direction.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! t.isIsomorphicTo(b)) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " changes its isomorphism class when its tetrahedra "
                    "are reordered in the reverse direction.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! t.isIsomorphicTo(c)) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " changes its isomorphism class when a random "
                    "isomorphism is applied.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! t.isIsomorphicTo(d)) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " changes its isomorphism class when a random "
                    "isomorphism is applied and then its tetrahedra are "
                    "reordered in the forward direction.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! t.isIsomorphicTo(e)) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " changes its isomorphism class when a random "
                    "isomorphism is applied and then its tetrahedra are "
                    "reordered in the reverse direction.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void reordering() {
            testManualAll(testReordering);
        }

        void propertyUpdates() {
            // Begin with an empty triangulation and calculate various
            // properties.
            Triangulation<3> t;

            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not valid.",
                t.isValid());
            CPPUNIT_ASSERT_MESSAGE("The empty triangulation is not orientable.",
                t.isOrientable());
            verifyGroup(t.homology(),
                "H1(empty triangulation)", 0);
            verifyGroup(t.homologyBdry(),
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
            t.tetrahedron(0)->join(0, t.tetrahedron(0),
                Perm<4>(1, 2, 3, 0));

            verifyGroup(t.homology(),
                "H1(LST(1,2,3))", 1);
            verifyGroup(t.homologyBdry(),
                "Boundary H1(LST(1,2,3))", 2);

            // Glue the remaining two faces in a non-orientable fashion.
            t.tetrahedron(0)->join(2, t.tetrahedron(0),
                Perm<4>(1, 0, 3, 2));

            CPPUNIT_ASSERT_MESSAGE("A bad 1-tetrahedron triangulation "
                "is valid.", ! t.isValid());
            CPPUNIT_ASSERT_MESSAGE("A bad 1-tetrahedron triangulation "
                "is orientable.", ! t.isOrientable());
        }

        template <int k>
        void pachner() {
            testManualSmall(verifyPachner<k>);
            runCensusAllClosed(verifyPachner<k>, true);
            runCensusAllBounded(verifyPachner<k>, true);
            runCensusAllIdeal(verifyPachner<k>, true);
            verifyPachnerSimplicial<k>();
        }

        static void verifyMinimiseBoundaryDoesNothing(
                const Triangulation<3>& tri, const char* name) {
            Triangulation<3> copy(tri);
            if (copy.minimiseBoundary()) {
                std::ostringstream msg;
                msg << name << ": minimiseBoundary() "
                    "reported changes when it should not.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! copy.isIdenticalTo(tri)) {
                std::ostringstream msg;
                msg << name << ": minimiseBoundary() "
                    "made changes when it should not.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static bool hasMinimalBoundary(const Triangulation<3>& tri) {
            for (auto b : tri.boundaryComponents())
                if (b->countTriangles() > 2 && b->countVertices() > 1)
                    return false;
            return true;
        }

        static void verifyMinimiseBoundary(const Triangulation<3>& tri,
                const char* name) {
            if (hasMinimalBoundary(tri)) {
                verifyMinimiseBoundaryDoesNothing(tri, name);
                return;
            }

            Triangulation<3> copy(tri);
            if (copy.isOrientable())
                copy.orient();

            if (! copy.minimiseBoundary()) {
                std::ostringstream msg;
                msg << name << ": minimiseBoundary() "
                    "reported no changes when it should.";
                CPPUNIT_FAIL(msg.str());
            }

            if (copy.isIsomorphicTo(tri)) {
                std::ostringstream msg;
                msg << name << ": minimiseBoundary() "
                    "made no changes when it should.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! hasMinimalBoundary(copy)) {
                std::ostringstream msg;
                msg << name << ": minimiseBoundary() "
                    "did not minimise boundary.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.eulerCharTri() != copy.eulerCharTri()) {
                std::ostringstream msg;
                msg << name << ": minimiseBoundary() changed "
                    "Euler characteristic (triangulation).";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.eulerCharManifold() != copy.eulerCharManifold()) {
                std::ostringstream msg;
                msg << name << ": minimiseBoundary() changed "
                    "Euler characteristic (manifold).";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isOrientable() != copy.isOriented()) {
                std::ostringstream msg;
                msg << name << ": minimiseBoundary() broke orientation.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.homology() != copy.homology()) {
                std::ostringstream msg;
                msg << name << ": minimiseBoundary() changed homology.";
                CPPUNIT_FAIL(msg.str());
            }

            bool isOrig0eff = ! tri.nonTrivialSphereOrDisc();
            bool isPost0eff = ! copy.nonTrivialSphereOrDisc();

            if (isOrig0eff && ! isPost0eff) {
                std::ostringstream msg;
                msg << name << ": minimiseBoundary() broke 0-efficiency.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void minimiseBoundary() {
            runCensusAllClosed(verifyMinimiseBoundaryDoesNothing);
            runCensusAllIdeal(verifyMinimiseBoundaryDoesNothing);
            runCensusAllBounded(verifyMinimiseBoundary);

            // The cone of a 6-triangle torus whose boundary has no
            // close-book moves at the beginning (so a layering is required).
            {
                const char* sig = "gffjQafeefaaaa";
                verifyMinimiseBoundary(Triangulation<3>::fromIsoSig(sig), sig);
            }
        }

        void verifyFillTorus(unsigned long p1, unsigned long q1,
                unsigned long r1, unsigned long p2, unsigned long q2,
                unsigned long r2, unsigned long lensP, unsigned long lensQ) {
            // Fills LST(p1, q1, r1) with the curve (p2, q2, r2) and
            // verifies that we obtain the lens space L(lensP, lensQ).
            if (p1 > q1) {
                std::swap(p1, q1);
                std::swap(p2, q2);
            }
            if (p1 > r1) {
                std::swap(p1, r1);
                std::swap(p2, r2);
            }
            if (q1 > r1) {
                std::swap(q1, r1);
                std::swap(q2, r2);
            }

            // Run the same test five times: once normally, and the
            // others with a random isomorphism.  The purpose of the
            // random isomorphisms is to mix up the order in which
            // the boundary edges are indexed within the boundary component.
            for (int i = 0; i < 5; ++i) {
                Triangulation<3> t;
                Tetrahedron<3>* bdry = t.insertLayeredSolidTorus(p1, q1);
                regina::Edge<3> *e1, *e2, *e3;

                if (i == 0) {
                    if (r1 <= 2) {
                        e2 = bdry->edge(1, 2);
                        e3 = bdry->edge(0, 2);
                        e1 = bdry->edge(0, 1);
                    } else {
                        e1 = bdry->edge(1, 2);
                        e2 = bdry->edge(0, 2);
                        e3 = bdry->edge(0, 1);
                    }
                } else {
                    // Apply a random isomorphism.
                    unsigned idx = bdry->index();

                    Isomorphism<3> iso = Isomorphism<3>::random(t.size());
                    iso.applyInPlace(t);

                    Perm<4> p = iso.facetPerm(idx);
                    if (r1 <= 2) {
                        e2 = t.simplex(iso.simpImage(idx))->edge(p[1], p[2]);
                        e3 = t.simplex(iso.simpImage(idx))->edge(p[0], p[2]);
                        e1 = t.simplex(iso.simpImage(idx))->edge(p[0], p[1]);
                    } else {
                        e1 = t.simplex(iso.simpImage(idx))->edge(p[1], p[2]);
                        e2 = t.simplex(iso.simpImage(idx))->edge(p[0], p[2]);
                        e3 = t.simplex(iso.simpImage(idx))->edge(p[0], p[1]);
                    }
                }

                if (! (e1->isBoundary() && e2->isBoundary() &&
                        e3->isBoundary())) {
                    std::ostringstream msg;
                    msg << "Filling (" << p1 << "," << q1 << "," << r1
                        << ") <-> (" << p2 << "," << q2 << "," << r2
                        << ") gives boundary edges that are not "
                        "marked as boundary.";
                    CPPUNIT_FAIL(msg.str());
                }

                t.fillTorus(e1, e2, e3, p2, q2, r2);
                t.intelligentSimplify();

                if (lensP <= 4) {
                    // If we have a closed orientable manifold with
                    // at most 2 tetrahedra and the right homology,
                    // then we have the right lens space.
                    if (t.size() > 2) {
                        std::ostringstream msg;
                        msg << "Filling (" << p1 << "," << q1 << "," << r1
                            << ") <-> (" << p2 << "," << q2 << "," << r2
                            << ") does not reduce to <= 2 tetrahedra.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (! (t.isValid() && t.isClosed() && t.isOrientable())) {
                        std::ostringstream msg;
                        msg << "Filling (" << p1 << "," << q1 << "," << r1
                            << ") <-> (" << p2 << "," << q2 << "," << r2
                            << ") does not give a valid closed orientable "
                            "triangulation.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    const AbelianGroup& h1 = t.homology();
                    if (! h1.isZn(lensP)) {
                        std::ostringstream msg;
                        msg << "Filling (" << p1 << "," << q1 << "," << r1
                            << ") <-> (" << p2 << "," << q2 << "," << r2
                            << ") does not give the right homology for L("
                            << lensP << "," << lensQ << ").";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else {
                    Triangulation<3> compare = Example<3>::lens(lensP, lensQ);
                    compare.intelligentSimplify();

                    if (! t.isIsomorphicTo(compare)) {
                        std::ostringstream msg;
                        msg << "Filling (" << p1 << "," << q1 << "," << r1
                            << ") <-> (" << p2 << "," << q2 << "," << r2
                            << ") does not reduce to the minimal "
                            "triangulation of L(" << lensP << "," << lensQ
                            << ").";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void fillTorus() {
            // Examples from Jaco-Rubinstein "Layered-triangulations of
            // 3-manifolds":

            // LST(0,1,1) <-> LST(2,1,1) = L(2,1)
            // LST(0,1,1) <-> LST(1,2,1) = L(1,0) = S^3
            // LST(1,1,2) <-> LST(2,1,1) = L(3,1)
            // LST(1,1,2) <-> LST(1,1,2) = L(0,1) = S^2 x S^1
            // LST(1,3,4) <-> LST(2,1,1) = L(7,3)
            // LST(1,3,4) <-> LST(1,2,1) = L(5,1)
            // LST(1,3,4) <-> LST(1,1,2) = L(2,1)
            // LST(2,3,5) <-> LST(2,1,1) = L(8,3)
            // LST(2,3,5) <-> LST(1,2,1) = L(7,2)
            // LST(2,3,5) <-> LST(1,1,2) = L(1,0) = S^3
            // LST(9,7,2) <-> LST(5,3,8) = L(62,27)
            verifyFillTorus(0,1,1, 2,1,1, 2,1);
            verifyFillTorus(0,1,1, 1,2,1, 1,0);
            verifyFillTorus(1,1,2, 2,1,1, 3,1);
            verifyFillTorus(1,1,2, 1,1,2, 0,1);
            verifyFillTorus(1,3,4, 2,1,1, 7,3);
            verifyFillTorus(1,3,4, 1,2,1, 5,1);
            verifyFillTorus(1,3,4, 1,1,2, 2,1);
            verifyFillTorus(2,3,5, 2,1,1, 8,3);
            verifyFillTorus(2,3,5, 1,2,1, 7,2);
            verifyFillTorus(2,3,5, 1,1,2, 1,0);
            verifyFillTorus(9,7,2, 5,3,8, 62,27);

            // We should obtain L(13,8) = L(13,5) from LST gluings:
            // (1,0,1) <-> (5,13,8)
            // (1,2,1) <-> (5,3,8)
            // (1,2,3) <-> (5,3,2)
            verifyFillTorus(1,0,1, 5,13,8, 13,5);
            verifyFillTorus(1,2,1, 5,3,8, 13,5);
            verifyFillTorus(1,2,3, 5,3,2, 13,5);

            // We should obtain L(25,11) = L(25,9) from LST gluings:
            // (1,0,1) <-> (14,25,11)
            // (1,2,1) <-> (14,3,11)
            // (3,2,1) <-> (8,3,11)
            // (3,2,5) <-> (8,3,5)
            // (7,2,5) <-> (2,3,5)
            // (7,2,9) <-> (2,3,1)
            // (7,16,9) <-> (2,1,1)
            // (25,16,9) <-> (0,1,1)
            verifyFillTorus(1,0,1, 14,25,11, 25,9);
            verifyFillTorus(1,2,1, 14,3,11, 25,9);
            verifyFillTorus(3,2,1, 8,3,11, 25,9);
            verifyFillTorus(3,2,5, 8,3,5, 25,9);
            verifyFillTorus(7,2,5, 2,3,5, 25,9);
            verifyFillTorus(7,2,9, 2,3,1, 25,9);
            verifyFillTorus(7,16,9, 2,1,1, 25,9);
            verifyFillTorus(25,16,9, 0,1,1, 25,9);

            // We should obtain L(42,11) = L(42,19) from LST gluings:
            // (31,11,42) <-> (1,1,0)
            // (31,11,20) <-> (1,1,2)
            // (9,11,20) <-> (3,1,2)
            // (9,11,2) <-> (3,1,4)
            // (9,7,2) <-> (3,7,4)
            // (5,7,2) <-> (11,7,4)
            // (5,3,2) <-> (11,15,4)
            // (1,3,2) <-> (19,15,4)
            // (1,1,2) <-> (19,23,4)
            // (1,1,0) <-> (19,23,42)
            verifyFillTorus(31,11,42, 1,1,0, 42,11);
            verifyFillTorus(31,11,20, 1,1,2, 42,11);
            verifyFillTorus(9,11,20, 3,1,2, 42,11);
            verifyFillTorus(9,11,2, 3,1,4, 42,11);
            verifyFillTorus(9,7,2, 3,7,4, 42,11);
            verifyFillTorus(5,7,2, 11,7,4, 42,11);
            verifyFillTorus(5,3,2, 11,15,4, 42,11);
            verifyFillTorus(1,3,2, 19,15,4, 42,11);
            verifyFillTorus(1,1,2, 19,23,4, 42,11);
            verifyFillTorus(1,1,0, 19,23,42, 42,11);
        }

        void verifyMeridianLongitude(const Triangulation<3>& orig,
                const char* name) {
            Triangulation<3> t(orig); // something we can modify

            if (t.isIdeal()) {
                t.idealToFinite();
                t.intelligentSimplify();
            }

            if (t.countVertices() != 1) {
                std::ostringstream msg;
                msg << name << ": cannot build a one-vertex triangulation.";
                CPPUNIT_FAIL(msg.str());
            }

            auto curves = t.meridianLongitude();
            regina::Edge<3>* m = curves.first;
            regina::Edge<3>* l = curves.second;
            regina::Edge<3>* other = nullptr;
            for (auto e : t.boundaryComponent(0)->edges()) {
                if (e != m && e != l) {
                    other = e;
                    break;
                }
            }

            if ((! m) || (! l) || (! other)) {
                std::ostringstream msg;
                msg << name << ": boundary curves not identified.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! (m->isBoundary() && l->isBoundary() && other->isBoundary())) {
                std::ostringstream msg;
                msg << name << ": boundary curves not marked as boundary.";
                CPPUNIT_FAIL(msg.str());
            }

            const auto& mEmb = m->front();
            const auto& lEmb = l->front();
            const auto& oEmb = other->front();

            // If we fill along the curve p*meridian + q*longitude,
            // we should be left with homology Z_p.
            //
            // In particular, the longitude is the only curve that we can
            // fill along to obtain Z.
            {
                Triangulation<3> tmp(t);
                tmp.fillTorus(
                    tmp.simplex(mEmb.simplex()->index())->edge(mEmb.edge()),
                    tmp.simplex(lEmb.simplex()->index())->edge(lEmb.edge()),
                    tmp.simplex(oEmb.simplex()->index())->edge(oEmb.edge()),
                    1, 0, 1);
                if (! tmp.homology().isZ()) {
                    std::ostringstream msg;
                    msg << name << ": filling along longitude "
                        "does not give Z homology.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            {
                Triangulation<3> tmp(t);
                tmp.fillTorus(
                    tmp.simplex(mEmb.simplex()->index())->edge(mEmb.edge()),
                    tmp.simplex(lEmb.simplex()->index())->edge(lEmb.edge()),
                    tmp.simplex(oEmb.simplex()->index())->edge(oEmb.edge()),
                    2, 3, 5);
                if (! tmp.homology().isZn(3)) {
                    std::ostringstream msg;
                    msg << name << ": filling along "
                        "(3 * meridian +/- 2 * longitude) "
                        "does not give Z_3 homology.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            {
                Triangulation<3> tmp(t);
                tmp.fillTorus(
                    tmp.simplex(mEmb.simplex()->index())->edge(mEmb.edge()),
                    tmp.simplex(lEmb.simplex()->index())->edge(lEmb.edge()),
                    tmp.simplex(oEmb.simplex()->index())->edge(oEmb.edge()),
                    2, 3, 1);
                if (! tmp.homology().isZn(3)) {
                    std::ostringstream msg;
                    msg << name << ": filling along "
                        "(3 * meridian -/+ 2 * longitude) "
                        "does not give Z_3 homology.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            // To test the meridian, we use the fact that filling along
            // the meridian produces the 3-sphere.
            {
                Triangulation<3> tmp(t);
                tmp.fillTorus(
                    tmp.simplex(mEmb.simplex()->index())->edge(mEmb.edge()),
                    tmp.simplex(lEmb.simplex()->index())->edge(lEmb.edge()),
                    tmp.simplex(oEmb.simplex()->index())->edge(oEmb.edge()),
                    0, 1, 1);
                if (! tmp.isSphere()) {
                    std::ostringstream msg;
                    msg << name << ": filling along meridian "
                        "does not give the 3-sphere.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void meridianLongitude() {
            verifyMeridianLongitude(lst3_4_7, "LST(3,4,7)");
            verifyMeridianLongitude(figure8, "Figure eight");
            verifyMeridianLongitude(trefoil, "Trefoil");
            verifyMeridianLongitude(knot18, "18-crossing knot");
        }

        void verifyRetriangulate(const Triangulation<3>& tri,
                int height, int threads, bool track, size_t count,
                const char* name) {
            size_t tot = 0;
            bool broken = false;

            regina::ProgressTrackerOpen* tracker = nullptr;
            if (track)
                tracker = new regina::ProgressTrackerOpen();

            bool result = tri.retriangulate(height, threads, tracker,
                    [&tot, &broken, &tri](const Triangulation<3>& alt) {
                        ++tot;
                        if (alt.isValid() != tri.isValid()) {
                            broken = true; return true;
                        }
                        if (alt.isOrientable() != tri.isOrientable()) {
                            broken = true; return true;
                        }
                        if (alt.countBoundaryComponents() !=
                                tri.countBoundaryComponents()) {
                            broken = true; return true;
                        }
                        if (alt.homology() != tri.homology()) {
                            broken = true; return true;
                        }
                        return false;
                    });

            if (result != broken) {
                std::ostringstream msg;
                msg << name <<
                    ": retriangulate() return value differs from "
                    "action return values.";
                CPPUNIT_FAIL(msg.str());
            }
            if (broken) {
                std::ostringstream msg;
                msg << name << ": retriangulate() changed the manifold.";
                CPPUNIT_FAIL(msg.str());
            }
            if (count == 0) {
                std::cerr << name << " -> " << tot << std::endl;
                return;
            }
            if (tot != count) {
                std::ostringstream msg;
                msg << name << ": retriangulate() with height "
                    << height << " gave " << tot
                    << " triangulation(s) instead of " << count << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyRetriangulate(const Triangulation<3>& tri,
                int height, size_t count, const char* name) {
            // Single-threaded, no tracker:
            verifyRetriangulate(tri, height, 1, false, count, name);
            // Multi-threaded, with and without tracker:
            verifyRetriangulate(tri, height, 2, false, count, name);
            verifyRetriangulate(tri, height, 2, true, count, name);
        }

        void retriangulate() {
            // The counts here were computed using Regina 6.0 in
            // single-threaded mode.
            //
            // The expected counts should always be positive, so passing
            // an expected count of 0 will be treated as a request to display
            // the number of triangulations that were actually found.
            //
            verifyRetriangulate(s3, 0, 1, "S^3");
            verifyRetriangulate(s3, 1, 1, "S^3");
            verifyRetriangulate(s3, 2, 1, "S^3");
            verifyRetriangulate(s3, 3, 1, "S^3");
            verifyRetriangulate(figure8, 0, 1, "Figure eight");
            verifyRetriangulate(figure8, 1, 2, "Figure eight");
            verifyRetriangulate(figure8, 2, 6, "Figure eight");
            verifyRetriangulate(figure8, 3, 61, "Figure eight");
            verifyRetriangulate(weberSeifert, 0, 1, "Weber-Seifert");
            verifyRetriangulate(weberSeifert, 1, 10, "Weber-Seifert");
            verifyRetriangulate(lst3_4_7, 0, 1, "LST(3,4,7)");
            verifyRetriangulate(lst3_4_7, 1, 3, "LST(3,4,7)");
            verifyRetriangulate(lst3_4_7, 2, 13, "LST(3,4,7)");
            verifyRetriangulate(lst3_4_7, 3, 219, "LST(3,4,7)");
            verifyRetriangulate(q20_large, 0, 1, "Large S^3 / Q_20");
            verifyRetriangulate(q20_large, 1, 195, "Large S^3 / Q_20");
            verifyRetriangulate(q20_large, 2, 2997, "Large S^3 / Q_20");
        }

        void swapping() {
            Triangulation<3> a = Example<3>::figureEight();
            Triangulation<3> b = Example<3>::weberSeifert();

            a.homology();
            b.homology();

            swap(a, b);

            if (a.size() != 23) {
                CPPUNIT_FAIL("swap() did not swap tetrahedra correctly.");
            }
            if (! b.homology().isZ()) {
                CPPUNIT_FAIL("swap() did not swap properties correctly.");
            }

            std::iter_swap(&a, &b);

            if (a.size() != 2) {
                CPPUNIT_FAIL(
                    "std::iter_swap() did not swap tetrahedra correctly.");
            }
            if (b.homology() != weberSeifert.homology()) {
                CPPUNIT_FAIL(
                    "std::iter_swap() did not swap properties correctly.");
            }
        }
};

void addTriangulation3(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Triangulation3Test::suite());
}

