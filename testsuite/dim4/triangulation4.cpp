
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "triangulation/example4.h"
#include "manifold/manifold.h"
#include "subcomplex/standardtri.h"
#include "triangulation/example3.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

#include "testsuite/exhaustive.h"
#include "testsuite/generic/triangulationtest.h"
#include "testsuite/dim4/testdim4.h"

using regina::BoundaryComponent;
using regina::Edge;
using regina::Example;
using regina::AbelianGroup;
using regina::Example;
using regina::GroupPresentation;
using regina::Isomorphism;
using regina::Pentachoron;
using regina::Perm;
using regina::StandardTriangulation;
using regina::Tetrahedron;
using regina::Triangulation;
using regina::Vertex;

class Triangulation4Test : public TriangulationTest<4> {
    CPPUNIT_TEST_SUITE(Triangulation4Test);

    // Generic tests:
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryFacets);
    CPPUNIT_TEST(boundaryBuild);

    // Dimension-specific tests:
    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(boundaryFlags);
    CPPUNIT_TEST(boundaryComponents);
    CPPUNIT_TEST(boundaryGluings);
    CPPUNIT_TEST(vertexLinksSpecific);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(fundGroup);
    CPPUNIT_TEST(barycentricSubdivision);
    CPPUNIT_TEST(eltMove15);
    CPPUNIT_TEST(vertexLinks);
    CPPUNIT_TEST(edgeLinks);
    CPPUNIT_TEST(idealToFinite);
    CPPUNIT_TEST(iBundle);
    CPPUNIT_TEST(s1Bundle);
    CPPUNIT_TEST(bundleWithMonodromy);

    CPPUNIT_TEST_SUITE_END();

    private:
        // Closed orientable:
        Triangulation<4> s4_doubleConeS3;
            /**< A double cone over the 3-sphere. */

        // Closed non-orientable:
        Triangulation<4> rp4;
            /**< Real projective 4-space, built from four pentachora. */

        // Bounded orientable:
        Triangulation<4> ball_foldedPent;
            /**< A single pentachoron with two facets folded together. */
        Triangulation<4> ball_singleConeS3;
            /**< A single cone over the 3-sphere. */
        Triangulation<4> ball_layerAndFold;
            /**< Layer one pentachoron onto another over a single edge.
                 Then fold together the two remaining facets of one of the
                 pentachora. */

        // Ideal orientable:
        Triangulation<4> idealPoincareProduct;
            /**< An ideal triangulation of the product P x I, where
                 P is the Poincare homology sphere. */
        Triangulation<4> idealCappellShaneson;
            /**< The simplest triangulated non-trivial 2-knot exterior */

        // Both ideal and real boundary:
        Triangulation<4> mixedPoincareProduct;
            /**< A triangulation of the product P x I with one real and one
                 ideal boundary, where P is the Poincare homology sphere. */

        // Invalid triangulations:
        Triangulation<4> idealFigEightProduct;
            /**< A double cone over the figure eight knot complement.  All
                 three vertices should have links that are invalid because
                 they are closed and cusped.  The edges joining each cone
                 point to the centre vertex should be invalid because they
                 have torus links. */
        Triangulation<4> mixedFigEightProduct;
            /**< A single cone over the figure eight knot complement.
                 Both vertices should be invalid; one has a closed and cusped
                 link, and the other has an invalid 3-manifold as its link.
                 The edge joining both vertices is invalid also, with a
                 torus link. */
        Triangulation<4> pillow_twoCycle;
            /**< A "tetrahedral pillow" whose two facets are identified
                 according to a permutation in S_4, which in this case is
                 a pair swap. */
        Triangulation<4> pillow_threeCycle;
            /**< A "tetrahedral pillow" whose two facets are identified
                 according to a permutation in S_4, which in this case is
                 a 3-cycle. */
        Triangulation<4> pillow_fourCycle;
            /**< A "tetrahedral pillow" whose two facets are identified
                 according to a permutation in S_4, which in this case is
                 a 4-cycle. */

        // Disconnected triangulations:
        Triangulation<4> disjoint2;
            /**< A disjoint union of two triangulations. */
        Triangulation<4> disjoint3;
            /**< A disjoint union of three triangulations. */

    public:
        void setUp() {
            TriangulationTest<4>::setUp();

            // We can pull some of our triangulations straight out of the can
            // via Example<4>.
            copyAndDelete(rp4, Example<4>::rp4(), "RP^4");

            // Some of our triangulations are built from 3-manifold
            // triangulations.
            Triangulation<3>* base;

            base = Example<3>::threeSphere();
            copyAndDelete(s4_doubleConeS3,
                Example<4>::doubleCone(*base),
                "S^4 (double cone)");
            copyAndDelete(ball_singleConeS3,
                Example<4>::singleCone(*base),
                "D^4 (single cone)");
            delete base;

            base = Example<3>::poincareHomologySphere();
            copyAndDelete(idealPoincareProduct,
                Example<4>::doubleCone(*base),
                "(S^3 / P_120) x I (double cone)");
            copyAndDelete(idealCappellShaneson,
                Example<4>::cappellShaneson(),
                "2-pentachoron Cappell-Shaneson 2-knot exterior");
            copyAndDelete(mixedPoincareProduct,
                Example<4>::singleCone(*base),
                "(S^3 / P_120) x I (single cone)");
            delete base;

            base = Example<3>::figureEight();
            copyAndDelete(idealFigEightProduct,
                Example<4>::doubleCone(*base),
                "Fig_8 x I (double cone)");
            copyAndDelete(mixedFigEightProduct,
                Example<4>::singleCone(*base),
                "Fig_8 x I (single cone)");
            delete base;

            // Build the rest manually.
            Pentachoron<4>* p[2];

            p[0] = ball_foldedPent.newPentachoron();
            p[0]->join(2, p[0], Perm<5>(2, 4));
            ball_foldedPent.setLabel("Folded pentachoron");

            p[0] = ball_layerAndFold.newPentachoron();
            p[1] = ball_layerAndFold.newPentachoron();
            p[0]->join(0, p[1], Perm<5>());
            p[0]->join(1, p[1], Perm<5>());
            p[0]->join(2, p[1], Perm<5>());
            p[0]->join(3, p[0], Perm<5>(3, 4));
            ball_layerAndFold.setLabel("Layered and folded ball");

            p[0] = pillow_twoCycle.newPentachoron();
            p[1] = pillow_twoCycle.newPentachoron();
            p[0]->join(0, p[1], Perm<5>());
            p[0]->join(1, p[1], Perm<5>());
            p[0]->join(2, p[1], Perm<5>());
            p[0]->join(3, p[1], Perm<5>());
            p[0]->join(4, p[1], Perm<5>(1, 2));
            pillow_twoCycle.setLabel("Invalid 2-cycle pillow");

            p[0] = pillow_threeCycle.newPentachoron();
            p[1] = pillow_threeCycle.newPentachoron();
            p[0]->join(0, p[1], Perm<5>());
            p[0]->join(1, p[1], Perm<5>());
            p[0]->join(2, p[1], Perm<5>());
            p[0]->join(3, p[1], Perm<5>());
            p[0]->join(4, p[1], Perm<5>(2, 0, 1, 3, 4));
            pillow_threeCycle.setLabel("Invalid 3-cycle pillow");

            p[0] = pillow_fourCycle.newPentachoron();
            p[1] = pillow_fourCycle.newPentachoron();
            p[0]->join(0, p[1], Perm<5>());
            p[0]->join(1, p[1], Perm<5>());
            p[0]->join(2, p[1], Perm<5>());
            p[0]->join(3, p[1], Perm<5>());
            p[0]->join(4, p[1], Perm<5>(3, 2, 0, 1, 4));
            pillow_fourCycle.setLabel("Invalid 4-cycle pillow");

            // Build disconnected triangulations from others that we
            // already have.
            disjoint2.insertTriangulation(sphereBundle);
            disjoint2.insertTriangulation(ball_layerAndFold);
            disjoint2.setLabel("Disjoint union of two triangulations");

            disjoint3.insertTriangulation(rp4);
            disjoint3.insertTriangulation(ball_layerAndFold);
            disjoint3.insertTriangulation(idealPoincareProduct);
            disjoint3.setLabel("Disjoint union of three triangulations");
        }

        void tearDown() {
        }

        /**
         * Run a given test over all hand-coded test cases.
         */
        void testManualAll(Triangulation4TestFunction f) {
            f(&empty);
            f(&sphere);
            f(&simplicialSphere);
            f(&s4_doubleConeS3);
            f(&sphereBundle);
            f(&rp4);
            f(&twistedSphereBundle);
            f(&ball);
            f(&ball_foldedPent);
            f(&ball_singleConeS3);
            f(&ball_layerAndFold);
            f(&ballBundle);
            f(&twistedBallBundle);
            f(&idealPoincareProduct);
            f(&idealCappellShaneson);
            f(&mixedPoincareProduct);
            f(&idealFigEightProduct);
            f(&mixedFigEightProduct);
            f(&pillow_twoCycle);
            f(&pillow_threeCycle);
            f(&pillow_fourCycle);
            f(&disjoint2);
            f(&disjoint3);
        }

        /**
         * Run a given test over all hand-coded test cases that are not
         * too large.
         */
        void testManualTiny(Triangulation4TestFunction f) {
            f(&empty);
            f(&sphere);
            f(&simplicialSphere);
            f(&s4_doubleConeS3);
            f(&sphereBundle);
            f(&rp4);
            f(&twistedSphereBundle);
            f(&ball);
            f(&ball_foldedPent);
            f(&ball_singleConeS3);
            f(&ball_layerAndFold);
            f(&ballBundle);
            f(&twistedBallBundle);
            // f(&idealPoincareProduct);
            f(&idealCappellShaneson);
            // f(&mixedPoincareProduct);
            f(&idealFigEightProduct);
            f(&mixedFigEightProduct);
            f(&pillow_twoCycle);
            f(&pillow_threeCycle);
            f(&pillow_fourCycle);
            f(&disjoint2);
            // f(&disjoint3);
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

        void boundaryFacets() {
            testManualAll(verifyBoundaryFacets);
        }

        void boundaryBuild() {
            testManualAll(verifyBoundaryBuild);
        }

        void verifyInvalid(const Triangulation<4>& tri,
                int invalidVertices, int invalidEdges,
                int invalidEdgeLinks, int invalidEdgeIDs,
                int invalidTriangles) {
            if (tri.isValid()) {
                CPPUNIT_FAIL("Triangulation " + tri.label() +
                    " is reported as valid.");
            }
            unsigned long i;
            int found;

            found = 0;
            for (i = 0; i < tri.countVertices(); ++i)
                if (! tri.vertex(i)->isValid())
                    ++found;
            if (found != invalidVertices) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label()
                    << " contains " << found << " invalid vertices "
                    "instead of the expected " << invalidVertices << ".";
                CPPUNIT_FAIL(msg.str());
            }

            found = 0;
            for (i = 0; i < tri.countEdges(); ++i)
                if (! tri.edge(i)->isValid())
                    ++found;
            if (found != invalidEdges) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label()
                    << " contains " << found << " invalid edges "
                    "instead of the expected " << invalidEdges << ".";
                CPPUNIT_FAIL(msg.str());
            }

            found = 0;
            for (i = 0; i < tri.countEdges(); ++i)
                if (tri.edge(i)->hasBadLink())
                    ++found;
            if (found != invalidEdgeLinks) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label()
                    << " contains " << found << " invalid edge links "
                    "instead of the expected " << invalidEdgeLinks << ".";
                CPPUNIT_FAIL(msg.str());
            }

            found = 0;
            for (i = 0; i < tri.countEdges(); ++i)
                if (tri.edge(i)->hasBadIdentification())
                    ++found;
            if (found != invalidEdgeIDs) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label() << " contains "
                    << found << " invalid edge self-identifications "
                    "instead of the expected " << invalidEdgeIDs << ".";
                CPPUNIT_FAIL(msg.str());
            }

            found = 0;
            for (i = 0; i < tri.countTriangles(); ++i)
                if (! tri.triangle(i)->isValid())
                    ++found;
            if (found != invalidTriangles) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label()
                    << " contains " << found << " invalid triangles "
                    "instead of the expected " << invalidTriangles << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void validity() {
            verifyValid(empty);
            verifyValid(sphere);
            verifyValid(simplicialSphere);
            verifyValid(s4_doubleConeS3);
            verifyValid(sphereBundle);
            verifyValid(rp4);
            verifyValid(twistedSphereBundle);
            verifyValid(ball);
            verifyValid(ball_foldedPent);
            verifyValid(ball_singleConeS3);
            verifyValid(ball_layerAndFold);
            verifyValid(ballBundle);
            verifyValid(twistedBallBundle);
            verifyValid(idealPoincareProduct);
            verifyValid(idealCappellShaneson);
            verifyValid(mixedPoincareProduct);
            verifyInvalid(idealFigEightProduct, 3, 2, 2, 0, 0);
            verifyInvalid(mixedFigEightProduct, 2, 1, 1, 0, 0);
            verifyInvalid(pillow_twoCycle, 2, 2, 1, 1, 2);
            verifyInvalid(pillow_threeCycle, 0, 0, 0, 0, 1);
            verifyInvalid(pillow_fourCycle, 0, 1, 1, 1, 0);
        }

        void connectedness() {
            verifyConnected(empty);
            verifyConnected(sphere);
            verifyConnected(simplicialSphere);
            verifyConnected(s4_doubleConeS3);
            verifyConnected(sphereBundle);
            verifyConnected(rp4);
            verifyConnected(twistedSphereBundle);
            verifyConnected(ball);
            verifyConnected(ball_foldedPent);
            verifyConnected(ball_singleConeS3);
            verifyConnected(ball_layerAndFold);
            verifyConnected(ballBundle);
            verifyConnected(twistedBallBundle);
            verifyConnected(idealPoincareProduct);
            verifyConnected(idealCappellShaneson);
            verifyConnected(mixedPoincareProduct);
            verifyConnected(idealFigEightProduct);
            verifyConnected(mixedFigEightProduct);
            verifyConnected(pillow_twoCycle);
            verifyConnected(pillow_threeCycle);
            verifyConnected(pillow_fourCycle);
            verifyConnected(disjoint2, false);
            verifyConnected(disjoint3, false);
        }

        void orientability() {
            verifyOrientable(empty);
            verifyOrientable(sphere);
            verifyOrientable(simplicialSphere);
            verifyOrientable(s4_doubleConeS3);
            verifyOrientable(sphereBundle);
            verifyOrientable(rp4, false);
            verifyOrientable(twistedSphereBundle, false);
            verifyOrientable(ball);
            verifyOrientable(ball_foldedPent);
            verifyOrientable(ball_singleConeS3);
            verifyOrientable(ball_layerAndFold);
            verifyOrientable(ballBundle);
            verifyOrientable(twistedBallBundle, false);
            verifyOrientable(idealPoincareProduct);
            verifyOrientable(idealCappellShaneson);
            verifyOrientable(mixedPoincareProduct);
            verifyOrientable(idealFigEightProduct);
            verifyOrientable(mixedFigEightProduct);
            verifyOrientable(pillow_twoCycle, false);
            verifyOrientable(pillow_threeCycle);
            verifyOrientable(pillow_fourCycle, false);
        }

        void verifyBoundary(const Triangulation<4>& tri,
                bool realBdry = false, int idealVertices = 0,
                bool invalidVertices = false, bool valid = true) {
            bool closed = ! (realBdry || idealVertices || invalidVertices);

            if (closed) {
                if (! tri.isClosed())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as being not closed.");
            } else {
                if (tri.isClosed())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as being closed.");
            }

            if (realBdry) {
                if (! tri.hasBoundaryTetrahedra())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as having no boundary tetrahedra.");
            } else {
                if (tri.hasBoundaryTetrahedra())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as having boundary tetrahedra.");
            }

            if (idealVertices && valid) {
                if (! tri.isIdeal())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as being not ideal.");
            } else {
                if (tri.isIdeal())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as being ideal.");
            }

            unsigned long i;
            int found;

            found = 0;
            for (i = 0; i < tri.countVertices(); ++i)
                if (tri.vertex(i)->isIdeal())
                    ++found;
            if (found != idealVertices) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label()
                    << " contains " << found << " ideal vertices "
                    "instead of the expected " << idealVertices << ".";
                CPPUNIT_FAIL(msg.str());
            }

            // Hum, we've already check this but might as well cross-check.
            if (valid) {
                if (! tri.isValid())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as being invalid.");
            } else {
                if (tri.isValid())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as being valid.");
            }
        }

        void boundaryFlags() {
            verifyBoundary(empty);
            verifyBoundary(sphere);
            verifyBoundary(simplicialSphere);
            verifyBoundary(s4_doubleConeS3);
            verifyBoundary(sphereBundle);
            verifyBoundary(rp4);
            verifyBoundary(twistedSphereBundle);
            verifyBoundary(ball, true);
            verifyBoundary(ball_foldedPent, true);
            verifyBoundary(ball_singleConeS3, true);
            verifyBoundary(ball_layerAndFold, true);
            verifyBoundary(ballBundle, true);
            verifyBoundary(twistedBallBundle, true);
            verifyBoundary(idealPoincareProduct, false, 2);
            verifyBoundary(idealCappellShaneson, false, 1, 0, true);
            verifyBoundary(mixedPoincareProduct, true, 1);
            verifyBoundary(idealFigEightProduct, false, 0, true, false);
            verifyBoundary(mixedFigEightProduct, true, 0, true, false);
            verifyBoundary(pillow_twoCycle, false, 0, true, false);
            verifyBoundary(pillow_threeCycle, false, 1, false, false);
            verifyBoundary(pillow_fourCycle, false, 0, false, false);
        }

        void verifyBoundaryTri(const Triangulation<4>& tri,
                unsigned whichBdry, const char* bdryManifold,
                bool makeBdryFinite = false) {
            std::string ans;

            Triangulation<3> t(*(tri.boundaryComponent(whichBdry)->build()));
            t.intelligentSimplify();

            if (makeBdryFinite) {
                t.idealToFinite();
                t.intelligentSimplify();
            }

            StandardTriangulation* std =
                StandardTriangulation::isStandardTriangulation(&t);
            if (! std)
                ans = "<unrecognised triangulation>";
            else {
                regina::Manifold* mfd = std->manifold();
                if (! mfd)
                    ans = "<unrecognised manifold>";
                else {
                    ans = mfd->name();
                    delete mfd;
                }
                delete std;
            }

            if (ans != bdryManifold) {
                std::ostringstream msg;
                msg << "Boundary component " << whichBdry
                    << " of triangulation " << tri.label()
                    << " simplifies to " << ans
                    << " instead of the expected " << bdryManifold << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void boundaryComponents() {
            verifyBoundaryCount(empty, 0);
            verifyBoundaryCount(sphere, 0);
            verifyBoundaryCount(simplicialSphere, 0);
            verifyBoundaryCount(s4_doubleConeS3, 0);
            verifyBoundaryCount(sphereBundle, 0);
            verifyBoundaryCount(rp4, 0);
            verifyBoundaryCount(twistedSphereBundle, 0);
            verifyBoundaryCount(ball, 1);
            verifyBoundaryTri(ball, 0, "S3");
            verifyBoundaryCount(ball_foldedPent, 1);
            verifyBoundaryTri(ball_foldedPent, 0, "S3");
            verifyBoundaryCount(ball_singleConeS3, 1);
            verifyBoundaryTri(ball_singleConeS3, 0, "S3");
            verifyBoundaryCount(ball_layerAndFold, 1);
            verifyBoundaryTri(ball_layerAndFold, 0, "S3");
            verifyBoundaryCount(ballBundle, 1);
            verifyBoundaryTri(ballBundle, 0, "S2 x S1");
            verifyBoundaryCount(twistedBallBundle, 1);
            verifyBoundaryTri(twistedBallBundle, 0, "S2 x~ S1");
            verifyBoundaryCount(idealPoincareProduct, 0, 2);
            verifyBoundaryTri(idealPoincareProduct, 0, "S3/P120");
            verifyBoundaryTri(idealPoincareProduct, 1, "S3/P120");
            verifyBoundaryCount(idealCappellShaneson, 0, 1);
            verifyBoundaryTri(idealCappellShaneson, 0, "S2 x S1");
            verifyBoundaryCount(mixedPoincareProduct, 1, 1);
            verifyBoundaryTri(mixedPoincareProduct, 0, "S3/P120");
            verifyBoundaryTri(mixedPoincareProduct, 1, "S3/P120");
            verifyBoundaryCount(idealFigEightProduct, 0, 0, 3);
            // Boundary 0 of idealFigEightProduct should be the
            // suspension of a torus.  I think.
            verifyBoundaryTri(idealFigEightProduct, 0,
                "<unrecognised triangulation>");
            verifyBoundaryH1(idealFigEightProduct, 0, "2 Z");
            verifyBoundaryTri(idealFigEightProduct, 1,
                "Figure eight knot complement");
            verifyBoundaryTri(idealFigEightProduct, 2,
                "Figure eight knot complement");
            verifyBoundaryCount(mixedFigEightProduct, 1, 0, 1);
            verifyBoundaryTri(mixedFigEightProduct, 0,
                "Figure eight knot complement");
            verifyBoundaryTri(mixedFigEightProduct, 1,
                "Figure eight knot complement");
            verifyBoundaryCount(pillow_twoCycle, 0, 0, 2);
            // I *think* the links of the two invalid vertices for
            // pillow_twoCycle are (RP2 x I), but with one RP2 cusp and
            // one invalid edge (as opposed to two RP2 cusps).
            verifyBoundaryTri(pillow_twoCycle, 0,
                "<unrecognised triangulation>", true);
            verifyBoundaryH1(pillow_twoCycle, 0, "Z_2");
            verifyBoundaryTri(pillow_twoCycle, 1,
                "<unrecognised triangulation>", true);
            verifyBoundaryH1(pillow_twoCycle, 1, "Z_2");
            verifyBoundaryCount(pillow_threeCycle, 0, 1);
            verifyBoundaryTri(pillow_threeCycle, 0, "L(3,1)");
            verifyBoundaryCount(pillow_fourCycle, 0);
        }

        void verifyBoundaryGluings(const Triangulation<4>& tri) {
            BoundaryComponent<4>* bc;
            const Tetrahedron<3> *tet3, *adj3;
            Tetrahedron<4> *tet4, *adj4;

            int n = tri.countBoundaryComponents();
            int count;
            int i, j;
            int triangle;
            for (i = 0; i < n; ++i) {
                bc = tri.boundaryComponent(i);
                if (bc->isIdeal())
                    continue;

                count = bc->countTetrahedra();
                for (j = 0; j < count; ++j) {
                    tet4 = bc->tetrahedron(j);
                    tet3 = bc->build()->tetrahedron(j);
                    for (triangle = 0; triangle < 4; ++triangle) {
                        adj3 = tet3->adjacentTetrahedron(triangle);
                        if (adj3) {
                            adj4 = bc->tetrahedron(adj3->markedIndex());
                            if (tet4->triangle(triangle) !=
                                    adj4->triangle(
                                    tet3->adjacentFace(triangle))) {
                                std::ostringstream msg;
                                msg << "Boundary tetrahedron adjacency "
                                    "test failed for " << tri.label()
                                    << ", BC #" << i << ", tet #" << j
                                    << ", triangle #" << triangle << ".";
                                CPPUNIT_FAIL(msg.str());
                            }
                        }
                    }
                }
            }
        }

        void boundaryGluings() {
            verifyBoundaryGluings(ball);
            verifyBoundaryGluings(ball_foldedPent);
            verifyBoundaryGluings(ball_singleConeS3);
            verifyBoundaryGluings(ball_layerAndFold);
            verifyBoundaryGluings(ballBundle);
            verifyBoundaryGluings(twistedBallBundle);
        }

        void verifyLinkCount(const Triangulation<4>& tri, unsigned nVert) {
            if (tri.countVertices() != nVert) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label()
                    << " has " << tri.countVertices()
                    << " vertices, not the expected " << nVert << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyLinksSpheres(const Triangulation<4>& tri, unsigned nVert) {
            verifyLinkCount(tri, nVert);

            for (unsigned i = 0; i < nVert; ++i) {
                Triangulation<3> t(*(tri.vertex(i)->buildLink()));
                t.intelligentSimplify();

                std::string link;
                StandardTriangulation* std =
                    StandardTriangulation::isStandardTriangulation(&t);
                if (! std)
                    link = "<unrecognised triangulation>";
                else {
                    regina::Manifold* mfd = std->manifold();
                    if (! mfd)
                        link = "<unrecognised manifold>";
                    else {
                        link = mfd->name();
                        delete mfd;
                    }
                    delete std;
                }

                if (link != "S3") {
                    std::ostringstream msg;
                    msg << "Vertex " << i << " of triangulation "
                        << tri.label() << " simplifies to "
                        << link << ", not S3 as expected.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyLinksBalls(const Triangulation<4>& tri, unsigned nVert) {
            verifyLinkCount(tri, nVert);

            for (unsigned i = 0; i < nVert; ++i) {
                Triangulation<3> t(*(tri.vertex(i)->buildLink()));
                t.intelligentSimplify();

                std::string link;
                StandardTriangulation* std =
                    StandardTriangulation::isStandardTriangulation(&t);
                if (! std)
                    link = "<unrecognised triangulation>";
                else {
                    regina::Manifold* mfd = std->manifold();
                    if (! mfd)
                        link = "<unrecognised manifold>";
                    else {
                        link = mfd->name();
                        delete mfd;
                    }
                    delete std;
                }

                if (link != "B3") {
                    std::ostringstream msg;
                    msg << "Vertex " << i << " of triangulation "
                        << tri.label() << " simplifies to "
                        << link << ", not B3 as expected.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyLink(const Triangulation<4>& tri,
                unsigned whichVertex, const char* manifold) {
            std::string link;

            Triangulation<3> t(*(tri.vertex(whichVertex)->buildLink()));
            t.intelligentSimplify();

            StandardTriangulation* std =
                StandardTriangulation::isStandardTriangulation(&t);
            if (! std)
                link = "<unrecognised triangulation>";
            else {
                regina::Manifold* mfd = std->manifold();
                if (! mfd)
                    link = "<unrecognised manifold>";
                else {
                    link = mfd->name();
                    delete mfd;
                }
                delete std;
            }

            if (link != manifold) {
                std::ostringstream msg;
                msg << "Vertex " << whichVertex
                    << " of triangulation " << tri.label()
                    << " simplifies to " << link
                    << " instead of the expected " << manifold << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyLinkH1(const Triangulation<4>& tri,
                unsigned whichVertex, const char* h1) {
            // For links where we have little hope of recognising the
            // underlying triangulation or manifold.

            // Do a barycentric subdivision to turn any invalid edges
            // into proper RP^2 ideal boundaries.
            Triangulation<3> t(*(tri.vertex(whichVertex)->buildLink()));
            t.barycentricSubdivision();
            t.intelligentSimplify();

            std::string ans = t.homology().str();

            if (ans != h1) {
                std::ostringstream msg;
                msg << "Vertex " << whichVertex
                    << " of triangulation " << tri.label()
                    << " has first homology " << ans
                    << " instead of the expected " << h1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void vertexLinksSpecific() {
            verifyLinksSpheres(empty, 0);
            verifyLinksSpheres(sphere, 5);
            verifyLinksSpheres(simplicialSphere, 6);
            verifyLinksSpheres(s4_doubleConeS3, 3);
            verifyLinksSpheres(sphereBundle, 1);
            verifyLinksSpheres(rp4, 3);
            verifyLinksSpheres(twistedSphereBundle, 1);
            verifyLinksBalls(ball, 5);
            verifyLinksBalls(ball_foldedPent, 4);
            verifyLinkCount(ball_singleConeS3, 2);
            verifyLink(ball_singleConeS3, 0, "B3");
            verifyLink(ball_singleConeS3, 1, "S3");
            verifyLinksBalls(ball_layerAndFold, 4);
            verifyLinksBalls(ballBundle, 2);
            verifyLinksBalls(twistedBallBundle, 1);
            verifyLinkCount(idealPoincareProduct, 3);
            verifyLink(idealPoincareProduct, 0, "S3");
            verifyLink(idealPoincareProduct, 1, "S3/P120");
            verifyLink(idealPoincareProduct, 2, "S3/P120");
            verifyLinkCount(idealCappellShaneson, 1);
            verifyLink(idealCappellShaneson, 0, "S2 x S1");
            verifyLinkCount(mixedPoincareProduct, 2);
            verifyLink(mixedPoincareProduct, 0, "B3");
            verifyLink(mixedPoincareProduct, 1, "S3/P120");
            verifyLinkCount(idealFigEightProduct, 3);
            // The next link should be (?) the suspension of a torus.
            verifyLink(idealFigEightProduct, 0, "<unrecognised triangulation>");
            verifyLinkH1(idealFigEightProduct, 0, "2 Z");
            verifyLink(idealFigEightProduct, 1, "Figure eight knot complement");
            verifyLink(idealFigEightProduct, 2, "Figure eight knot complement");
            verifyLinkCount(mixedFigEightProduct, 2);
            // The next link should be (?) the cone of a torus.
            verifyLink(mixedFigEightProduct, 0, "<unrecognised triangulation>");
            verifyLinkH1(mixedFigEightProduct, 0, "2 Z");
            verifyLink(mixedFigEightProduct, 1, "Figure eight knot complement");
            verifyLinkCount(pillow_twoCycle, 4);
            // Two of these vertex links are invalid 3-manifold
            // triangulations (specifically, with invalid edges).
            // I *think* these are each triangulations of (RP^2 x I) with one
            // RP^2 at an ideal vertex and one RP^2 inside an invalid edge.
            verifyLink(pillow_twoCycle, 0, "<unrecognised triangulation>");
            verifyLinkH1(pillow_twoCycle, 0, "Z_2");
            verifyLink(pillow_twoCycle, 1, "S3");
            verifyLink(pillow_twoCycle, 2, "<unrecognised triangulation>");
            verifyLinkH1(pillow_twoCycle, 2, "Z_2");
            verifyLink(pillow_twoCycle, 3, "S3");
            verifyLinkCount(pillow_threeCycle, 3);
            verifyLink(pillow_threeCycle, 0, "S3");
            verifyLink(pillow_threeCycle, 1, "L(3,1)");
            verifyLink(pillow_threeCycle, 2, "S3");
            verifyLinksSpheres(pillow_fourCycle, 2);
        }

        void verifyEulerChar(const Triangulation<4>& tri,
                long expectedManifold, long expectedTri) {
            long eulerManifold = tri.eulerCharManifold();
            long eulerTri = tri.eulerCharTri();

            if (eulerManifold != expectedManifold) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label() << " gives "
                    "manifold Euler characteristic = " << eulerManifold
                    << " instead of the expected " << expectedManifold << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (eulerTri != expectedTri) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label() << " gives "
                    "triangulation Euler characteristic = " << eulerTri
                    << " instead of the expected " << expectedTri << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void eulerChar() {
            verifyEulerChar(empty, 0, 0);
            verifyEulerChar(sphere, 2, 2);
            verifyEulerChar(simplicialSphere, 2, 2);
            verifyEulerChar(s4_doubleConeS3, 2, 2);
            verifyEulerChar(sphereBundle, 0, 0);
            verifyEulerChar(rp4, 1, 1);
            verifyEulerChar(twistedSphereBundle, 0, 0);
            verifyEulerChar(ball, 1, 1);
            verifyEulerChar(ball_foldedPent, 1, 1);
            verifyEulerChar(ball_singleConeS3, 1, 1);
            verifyEulerChar(ball_layerAndFold, 1, 1);
            verifyEulerChar(ballBundle, 0, 0);
            verifyEulerChar(twistedBallBundle, 0, 0);
            verifyEulerChar(idealPoincareProduct, 0, 2);
            verifyEulerChar(mixedPoincareProduct, 0, 1);
            verifyEulerCharTri(idealFigEightProduct, 1);
            verifyEulerCharTri(mixedFigEightProduct, 1);
            verifyEulerCharTri(pillow_twoCycle, 2);
            verifyEulerCharTri(pillow_threeCycle, 2);
            verifyEulerCharTri(pillow_fourCycle, 0);
        }

        void homologyH1() {
            verifyHomology(empty, "0");
            verifyHomology(sphere, "0");
            verifyHomology(simplicialSphere, "0");
            verifyHomology(s4_doubleConeS3, "0");
            verifyHomology(sphereBundle, "Z");
            verifyHomology(rp4, "Z_2");
            verifyHomology(twistedSphereBundle, "Z");
            verifyHomology(ball, "0");
            verifyHomology(ball_foldedPent, "0");
            verifyHomology(ball_singleConeS3, "0");
            verifyHomology(ball_layerAndFold, "0");
            verifyHomology(ballBundle, "Z");
            verifyHomology(twistedBallBundle, "Z");
            verifyHomology(idealPoincareProduct, "0");
            verifyHomology(idealCappellShaneson, "Z");
            verifyHomology(mixedPoincareProduct, "0");
            verifyHomology(idealFigEightProduct, "Z");
            verifyHomology(mixedFigEightProduct, "Z");
        }

        void fundGroup() {
            verifyFundGroup(empty, "0");
            verifyFundGroup(sphere, "0");
            verifyFundGroup(simplicialSphere, "0");
            verifyFundGroup(s4_doubleConeS3, "0");
            verifyFundGroup(sphereBundle, "Z");
            verifyFundGroup(rp4, "Z_2");
            verifyFundGroup(twistedSphereBundle, "Z");
            verifyFundGroup(ball, "0");
            verifyFundGroup(ball_foldedPent, "0");
            verifyFundGroup(ball_singleConeS3, "0");
            verifyFundGroup(ball_layerAndFold, "0");
            verifyFundGroup(ballBundle, "Z");
            verifyFundGroup(twistedBallBundle, "Z");
            verifyFundGroup(idealPoincareProduct, "");
            verifyFundGroup(mixedPoincareProduct, "");
            verifyFundGroup(idealFigEightProduct,
                "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b a^-1 b^2");
            verifyFundGroup(mixedFigEightProduct,
                "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b a^-1 b^2");
        }

        static void verifyBary(Triangulation<4>* tri) {
            Triangulation<4> b(*tri);
            b.barycentricSubdivision();

            // Note that subdivisions can turn invalid into valid, but
            // they can never turn valid into invalid.
            if (tri->isValid() && ! b.isValid()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks validity.";
                CPPUNIT_FAIL(msg.str());
            }

            // Ideal triangulations must be valid, so only consider the
            // valid -> valid case here.
            if (tri->isValid() && (tri->isIdeal() != b.isIdeal())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks idealness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->hasBoundaryTetrahedra() != b.hasBoundaryTetrahedra()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks boundary tetrahedra.";
                CPPUNIT_FAIL(msg.str());
            }

            // Some invalid -> valid cases can turn non-closed into closed.
            // Just consider valid only.
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

            // Invalid vertices and edges can wreak havoc on the
            // counting and labelling of boundary components (see
            // the BoundaryComponent class notes for details).
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

            if (! (tri->homologyH1() == b.homologyH1())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks H1.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! (tri->homologyH2() == b.homologyH2())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks H2.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void barycentricSubdivision() {
            testManualTiny(verifyBary);
        }

        static void verifyEltMove15(Triangulation<4>* tri) {
            unsigned long n = tri->size();
            for (unsigned long i = 0; i < n; ++i) {
                Triangulation<4> large(*tri);
                large.oneFiveMove(large.pentachoron(i));

                if (large.size() != n + 4) {
                    std::ostringstream msg;
                    msg << tri->label() << ", pent " << i << ": "
                        << "1-5 move gives wrong # pentachora.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.isValid() != tri->isValid()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", pent " << i << ": "
                        << "1-5 move changes validity.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.isOrientable() != tri->isOrientable()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", pent " << i << ": "
                        << "1-5 move changes orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.isClosed() != tri->isClosed()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", pent " << i << ": "
                        << "1-5 move changes closedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.countBoundaryComponents() !=
                        tri->countBoundaryComponents()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", pent " << i << ": "
                        << "1-5 move changes # boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.eulerCharTri() != tri->eulerCharTri()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", pent " << i << ": "
                        << "1-5 move changes Euler characteristic.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri->isValid()) {
                    if (! (large.homologyH1() == tri->homologyH1())) {
                        std::ostringstream msg;
                        msg << tri->label() << ", pent " << i << ": "
                            << "1-5 move changes H1.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if (! (large.homologyH2() == tri->homologyH2())) {
                        std::ostringstream msg;
                        msg << tri->label() << ", pent " << i << ": "
                            << "1-5 move changes H2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                // Shrink.
                if (large.isIsomorphicTo(*tri).get()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", pent " << i << ": "
                        << "1-5 move: result is isomorphic.";
                    CPPUNIT_FAIL(msg.str());
                }

                bool res =
                    large.collapseEdge(large.pentachoron(n + 3)->edge(
                    regina::Edge<4>::edgeNumber[0][4]), true, true);

                if (! res) {
                    std::ostringstream msg;
                    msg << tri->label() << ", pent " << i << ": "
                        << "1-5 move: could not recollapse edge.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! large.isIsomorphicTo(*tri).get()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", pent " << i << ": "
                        << "1-5 move: recollapse is not isomorphic.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void eltMove15() {
            testManualAll(verifyEltMove15);
            runCensusAllBounded(verifyEltMove15);
            runCensusAllNoBdry(verifyEltMove15);
        }

        static void verifyVertexLinks(Triangulation<4>* tri) {
            for (unsigned long i = 0; i < tri->countVertices(); ++i) {
                Vertex<4>* v = tri->vertex(i);
                Isomorphism<4>* iso;

                const Triangulation<3>* link = v->buildLink();
                Triangulation<3>* link2 = v->buildLinkDetail(true, &iso);

                if (link->size() != v->degree()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", vertex " << i << ": "
                        << "link has incorrect number of tetrahedra.";
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

                if (v->isValid()) {
                    if (v->isBoundary() && v->boundaryComponent()->
                            countTetrahedra() > 0) {
                        if (! link->isBall()) {
                            std::ostringstream msg;
                            msg << tri->label() << ", vertex "
                                << i << ": "
                                << "link of real boundary vertex "
                                "is not a 3-ball.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else if (v->isBoundary()) {
                        if (! link->isClosed()) {
                            std::ostringstream msg;
                            msg << tri->label() << ", vertex "
                                << i << ": "
                                << "link of ideal boundary vertex "
                                "is not a closed 3-manifold.";
                            CPPUNIT_FAIL(msg.str());
                        }
                        if (link->isThreeSphere()) {
                            std::ostringstream msg;
                            msg << tri->label() << ", vertex "
                                << i << ": "
                                << "link of ideal boundary vertex "
                                "is a 3-sphere.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (! link->isThreeSphere()) {
                            std::ostringstream msg;
                            msg << tri->label() << ", vertex "
                                << i << ": "
                                << "link of internal edge is not a 3-sphere.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                } else {
                    // Invalid vertex.
                    if (! v->isBoundary()) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex "
                            << i << ": "
                            << "invalid vertex is not marked as boundary.";
                        CPPUNIT_FAIL(msg.str());
                    } else if (v->boundaryComponent()->
                            countTetrahedra() > 0) {
                        // Link should have boundary faces but not be a 3-ball.
                        if (! link->hasBoundaryTriangles()) {
                            std::ostringstream msg;
                            msg << tri->label() << ", vertex "
                                << i << ": "
                                << "link of invalid real boundary vertex "
                                "has no boundary faces.";
                            CPPUNIT_FAIL(msg.str());
                        }
                        if (link->isBall()) {
                            std::ostringstream msg;
                            msg << tri->label() << ", vertex "
                                << i << ": "
                                << "link of invalid real boundary vertex "
                                "is a 3-ball.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        // Link should have no boundary faces, but not
                        // be a closed 3-manifold.
                        if (link->hasBoundaryTriangles()) {
                            std::ostringstream msg;
                            msg << tri->label() << ", vertex "
                                << i << ": "
                                << "link of invalid ideal vertex "
                                "has boundary faces.";
                            CPPUNIT_FAIL(msg.str());
                        }
                        if (link->isClosed()) {
                            std::ostringstream msg;
                            msg << tri->label() << ", vertex "
                                << i << ": "
                                << "link of invalid ideal vertex "
                                "is a closed 3-manifold.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }

                // Make sure the edge link matches what happens on
                // the vertex links.
                unsigned j, k;
                Pentachoron<4>* p;
                Perm<5> perm;
                const regina::Tetrahedron<3> *t, *adj;
                unsigned vNum;
                for (j = 0; j < v->degree(); ++j) {
                    p = tri->pentachoron(iso->pentImage(j));
                    perm = iso->facetPerm(j);
                    vNum = perm[4];
                    if (p->vertex(vNum) != v) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link does not map 4 -> vertex correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (perm[0] != p->tetrahedronMapping(vNum)[0] ||
                            perm[1] != p->tetrahedronMapping(vNum)[1] ||
                            perm[2] != p->tetrahedronMapping(vNum)[2] ||
                            perm[3] != p->tetrahedronMapping(vNum)[3]) {
                        std::ostringstream msg;
                        msg << tri->label() << ", vertex " << i << ": "
                            << "link does not map 0,1,2,3 -> opposite "
                            "tetrahedron correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    for (k = 0; k < 4; ++k) {
                        t = link->tetrahedron(j);
                        adj = t->adjacentTetrahedron(k);
                        if (adj) {
                            if (! p->adjacentPentachoron(perm[k])) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", vertex " << i << ": "
                                    << "link has extra adjacent tetrahedron.";
                                CPPUNIT_FAIL(msg.str());
                            } else if (p->adjacentPentachoron(perm[k]) !=
                                    tri->pentachoron(iso->pentImage(
                                    adj->index()))) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", vertex " << i << ": "
                                    << "link has wrong adjacent tetrahedron.";
                                CPPUNIT_FAIL(msg.str());
                            } else if (p->adjacentGluing(perm[k]) !=
                                    iso->facetPerm(adj->index()) *
                                    Perm<5>::extend(t->adjacentGluing(k)) *
                                    perm.inverse()) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", vertex " << i << ": "
                                    << "link has wrong adjacent gluing.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        } else {
                            if (p->adjacentPentachoron(perm[k])) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", vertex " << i << ": "
                                    << "link missing adjacent tetrahedron.";
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
            runCensusAllBounded(verifyVertexLinks);
            runCensusAllNoBdry(verifyVertexLinks);
        }

        static void verifyEdgeLinks(Triangulation<4>* tri) {
            for (unsigned long i = 0; i < tri->countEdges(); ++i) {
                Edge<4>* e = tri->edge(i);
                Isomorphism<4>* iso;

                const Triangulation<2>* link = e->buildLink();
                Triangulation<2>* link2 = e->buildLinkDetail(true, &iso);

                if (link->countTriangles() != e->degree()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", edge " << i << ": "
                        << "link has incorrect number of triangles.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! link2->isIdenticalTo(*link)) {
                    std::ostringstream msg;
                    msg << tri->label() << ", edge " << i << ": "
                        << "variants of buildLink() give different results.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! link->isConnected()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", edge " << i << ": "
                        << "link of edge is not connected.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (e->isBoundary()) {
                    if (link->isClosed()) {
                        std::ostringstream msg;
                        msg << tri->label() << ", edge " << i << ": "
                            << "link of boundary edge is closed.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else {
                    if (! link->isClosed()) {
                        std::ostringstream msg;
                        msg << tri->label() << ", edge " << i << ": "
                            << "link of internal edge is not closed.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                if (e->isValid()) {
                    if (e->isBoundary()) {
                        if (link->eulerChar() != 1) {
                            std::ostringstream msg;
                            msg << tri->label() << ", edge "
                                << i << ": "
                                << "link of boundary edge is not a disc.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (link->eulerChar() != 2) {
                            std::ostringstream msg;
                            msg << tri->label() << ", edge "
                                << i << ": "
                                << "link of internal edge is not a sphere.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }

                // Make sure the edge link matches what happens on
                // the vertex links.
                unsigned j, k;
                Pentachoron<4>* p;
                Vertex<4>* v;
                Perm<5> perm;
                const Triangulation<3>* vLink;
                for (j = 0; j < 2; ++j) {
                    p = e->front().pentachoron();
                    perm = e->front().vertices();

                    // In the vertex link at the jth end of this edge,
                    // find the vertex that this edge projects down to.
                    v = p->vertex(perm[j]);
                    vLink = v->buildLink();

                    for (k = 0; k < v->degree(); ++k)
                        if (v->embedding(k).pentachoron() == p &&
                                v->embedding(k).vertex() == perm[j])
                            break;
                    if (k == v->degree()) {
                        std::ostringstream msg;
                        msg << tri->label() << ", edge " << i << ": "
                            << "misconstructed vertex link.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    Vertex<3>* match = vLink->tetrahedron(k)->vertex(
                        p->tetrahedronMapping(perm[j]).preImageOf(
                        perm[1-j]));

                    if (! e->hasBadIdentification()) {
                        if (! match->buildLink()->isIsomorphicTo(*link).get()) {
                            std::ostringstream msg;
                            msg << tri->label() << ", edge "
                                << i << ": "
                                << "non-isomorphic 2-D triangulations in "
                                "edge vs vertex links.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (match->degree() != 2 * e->degree()) {
                            std::ostringstream msg;
                            msg << tri->label() << ", edge " << i << ": "
                                << "mismatched degrees in edge vs vertex links.";
                            CPPUNIT_FAIL(msg.str());
                        }

                        // It's hard to guarantee anything about Euler
                        // characteristic in this setting, sigh.
                    }
                }

                const regina::Triangle<2> *t, *adj;
                unsigned eNum;
                for (j = 0; j < e->degree(); ++j) {
                    p = tri->pentachoron(iso->pentImage(j));
                    perm = iso->facetPerm(j);
                    eNum = Edge<4>::edgeNumber[perm[3]][perm[4]];
                    if (p->edge(eNum) != e ||
                            p->edgeMapping(eNum)[0] != perm[3] ||
                            p->edgeMapping(eNum)[1] != perm[4]) {
                        std::ostringstream msg;
                        msg << tri->label() << ", edge " << i << ": "
                            << "link does not map 3,4 -> edge correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (perm[0] != p->triangleMapping(eNum)[0] ||
                            perm[1] != p->triangleMapping(eNum)[1] ||
                            perm[2] != p->triangleMapping(eNum)[2]) {
                        std::ostringstream msg;
                        msg << tri->label() << ", edge " << i << ": "
                            << "link does not map 0,1,2 -> opposite "
                            "triangle correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    for (k = 0; k < 3; ++k) {
                        t = link->triangle(j);
                        adj = t->adjacentTriangle(k);
                        if (adj) {
                            if (! p->adjacentPentachoron(perm[k])) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", edge " << i << ": "
                                    << "link has extra adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            } else if (p->adjacentPentachoron(perm[k]) !=
                                    tri->pentachoron(iso->pentImage(
                                    adj->index()))) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", edge " << i << ": "
                                    << "link has wrong adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            } else if ((! e->hasBadIdentification()) &&
                                    p->adjacentGluing(perm[k]) !=
                                    iso->facetPerm(adj->index()) *
                                    Perm<5>::extend(t->adjacentGluing(k)) *
                                    perm.inverse()) {
                                // Note: we expect broken gluings with
                                // reverse self-identifications.
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", edge " << i << ": "
                                    << "link has wrong adjacent gluing.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        } else {
                            if (p->adjacentPentachoron(perm[k])) {
                                std::ostringstream msg;
                                msg << tri->label()
                                    << ", edge " << i << ": "
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

        void edgeLinks() {
            // Notes:
            // idealFigEightProduct -> has torus link
            // mixedFigEightProduct -> has torus link
            // pillow_fourCycle -> has PP link
            testManualAll(verifyEdgeLinks);
            runCensusAllBounded(verifyEdgeLinks);
            runCensusAllNoBdry(verifyEdgeLinks);
        }

        static void verifyIdealToFinite(Triangulation<4>* tri) {
            bool shouldTruncate = false;
            if (tri->isValid() && ! tri->isIdeal()) {
                // Should not truncate any vertices.
            } else {
                for (unsigned i = 0; i < tri->countVertices(); ++i)
                    if (tri->vertex(i)->isIdeal() ||
                            ! tri->vertex(i)->isValid()) {
                        shouldTruncate = true;
                        break;
                    }
            }

            if (! shouldTruncate) {
                // The idealToFinite routine should leave tri unchanged.
                Triangulation<4> other(*tri);
                other.idealToFinite();
                if (! other.isIdenticalTo(*tri)) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "idealToFinite modifies a triangulation with "
                        "no truncatable vertices.";
                    CPPUNIT_FAIL(msg.str());
                }
                return;
            }

            // Test the same triangulation under some random isomorphisms,
            // since the idealToFinite() code implements cases separately for
            // truncating differently-labelled vertices.
            //
            // We use just a couple of different isomorphisms here, since this
            // is a slow test and we are running it over a great many examples.
            for (unsigned i = 0; i < 2; ++i) {
                Triangulation<4> other(*tri);
                if (i > 0) {
                    Isomorphism<4>* iso = Isomorphism<4>::random(
                        other.size());
                    iso->applyInPlace(&other);
                    delete iso;
                }

                other.idealToFinite();

                if (other.isIdenticalTo(*tri)) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "idealToFinite does not modify a triangulation with "
                        "truncatable vertices.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (! other.hasBoundaryTetrahedra()) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "idealToFinite does not produce real boundary.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (other.isIdeal()) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "idealToFinite does not remove ideal vertices.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (other.isOrientable() != tri->isOrientable()) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "idealToFinite changes orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (other.isClosed() != tri->isClosed()) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "idealToFinite changes closedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (other.isConnected() != tri->isConnected()) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "idealToFinite changes connectedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri->isValid()) {
                    // Subdivision can change invalid to valid, but never
                    // the other way.
                    if (! other.isValid()) {
                        std::ostringstream msg;
                        msg << tri->label() << ": "
                            << "idealToFinite destroys validity.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    // Subdivisions can change these properties for
                    // invalid triangulations.
                    if (other.eulerCharManifold() != tri->eulerCharManifold()) {
                        std::ostringstream msg;
                        msg << tri->label() << ": "
                            << "idealToFinite changes Euler characteristic.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if (other.countBoundaryComponents() !=
                            tri->countBoundaryComponents()) {
                        std::ostringstream msg;
                        msg << tri->label() << ": "
                            << "idealToFinite changes # boundary components.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    // These properties can only be computed for valid
                    // triangulations.
                    other.intelligentSimplify();

                    if (! (other.homologyH1() == tri->homologyH1())) {
                        std::ostringstream msg;
                        msg << tri->label() << ": "
                            << "idealToFinite changes H1.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if (! (other.homologyH2() == tri->homologyH2())) {
                        std::ostringstream msg;
                        msg << tri->label() << ": "
                            << "idealToFinite changes H2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void idealToFinite() {
            testManualAll(verifyIdealToFinite);
            runCensusAllBounded(verifyIdealToFinite); // Never change
            runCensusAllNoBdry(verifyIdealToFinite); // Sometimes change
        }

        static void verifyIBundle(Triangulation<3>* tri) {
            // For now, only work with compact triangulations.
            if ((! tri->isValid()) || tri->isIdeal())
                return;

            Triangulation<4>* b = Example<4>::iBundle(*tri);

            if (! b->isValid()) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "iBundle gives an invalid triangulation.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b->isOrientable() != tri->isOrientable()) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "iBundle has mismatched orientability.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b->countComponents() != tri->countComponents()) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "iBundle has the wrong number of components.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b->eulerCharTri() != tri->eulerCharTri() ||
                    b->eulerCharManifold() != tri->eulerCharManifold()) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "iBundle gives the wrong Euler characteristic.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned long expectBdryComp = 2 * tri->countComponents();
            unsigned long i;
            for (i = 0; i < tri->countComponents(); ++i)
                if (tri->component(i)->countBoundaryTriangles())
                    --expectBdryComp;

            if (b->countBoundaryComponents() != expectBdryComp) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "iBundle has the wrong number of "
                        "boundary components.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned long expectBdryTets = 2 * tri->countTetrahedra() +
                20 * tri->countBoundaryTriangles();

            if (b->countBoundaryFacets() != expectBdryTets) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "iBundle has the wrong number of "
                        "boundary tetrahedra.";
                CPPUNIT_FAIL(msg.str());
            }

            // TODO: Check isomorphisms from tri onto the boundary of b.

            // Simplify the triangulation before running any more
            // expensive tests.
            b->intelligentSimplify();

            if (b->homologyH1() != tri->homologyH1()) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "iBundle gives a mismatched H1.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b->homologyH2() != tri->homologyH2()) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "iBundle gives a mismatched H2.";
                CPPUNIT_FAIL(msg.str());
            }

            delete b;
        }

        void iBundle() {
            runCensusAllClosed(verifyIBundle);
            runCensusAllBounded(verifyIBundle);
        }

        static void verifyS1Bundle(Triangulation<3>* tri) {
            // For now, only work with compact triangulations.
            if ((! tri->isValid()) || tri->isIdeal())
                return;

            Triangulation<4>* b = Example<4>::s1Bundle(*tri);

            if (! b->isValid()) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "s1Bundle gives an invalid triangulation.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b->isOrientable() != tri->isOrientable()) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "s1Bundle has mismatched orientability.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b->countComponents() != tri->countComponents()) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "s1Bundle has the wrong number of components.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b->eulerCharTri() != 0 || b->eulerCharManifold() != 0) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "s1Bundle gives the wrong Euler characteristic.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b->countBoundaryComponents() !=
                    tri->countBoundaryComponents()) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "s1Bundle has the wrong number of "
                        "boundary components.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned long expectBdryTets =
                20 * tri->countBoundaryTriangles();

            if (b->countBoundaryFacets() != expectBdryTets) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "s1Bundle has the wrong number of "
                        "boundary tetrahedra.";
                CPPUNIT_FAIL(msg.str());
            }

            // Simplify the triangulation before running any more
            // expensive tests.
            b->intelligentSimplify();

            regina::AbelianGroup expectH1(tri->homologyH1());
            expectH1.addRank();

            if (b->homologyH1() != expectH1) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "s1Bundle gives incorrect H1.";
                CPPUNIT_FAIL(msg.str());
            }

            regina::AbelianGroup expectH2(tri->homologyH2());
            expectH2.addGroup(tri->homology());

            if (b->homologyH2() != expectH2) {
                std::ostringstream msg;
                msg << tri->label() << ": "
                    << "s1Bundle gives incorrect H2.";
                CPPUNIT_FAIL(msg.str());
            }

            delete b;
        }

        void s1Bundle() {
            runCensusAllClosed(verifyS1Bundle);
            runCensusAllBounded(verifyS1Bundle);
        }

        static void verifyBundleWithMonodromy(Triangulation<3>* tri) {
            // For now, only work with compact triangulations.
            if ((! tri->isValid()) || tri->isIdeal())
                return;

            std::list<Isomorphism<3>*> autos;
            tri->findAllIsomorphisms(*tri, back_inserter(autos));

            for (Isomorphism<3>* aut : autos) {
                Triangulation<4>* b =
                    Example<4>::bundleWithMonodromy(*tri, *aut);

                if (! b->isValid()) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "bundleWithMonodromy gives an "
                        "invalid triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (b->isOrientable() && ! tri->isOrientable()) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "bundleWithMonodromy destroys non-orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (b->countComponents() != tri->countComponents()) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "bundleWithMonodromy has the wrong number "
                        "of components.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (b->eulerCharTri() != 0 || b->eulerCharManifold() != 0) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "bundleWithMonodromy gives the wrong "
                        "Euler characteristic.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (b->countBoundaryComponents() !=
                        tri->countBoundaryComponents()) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "bundleWithMonodromy gives the wrong number of "
                            "boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }

                size_t expectBdryTets = 20 * tri->countBoundaryTriangles();

                if (b->countBoundaryFacets() != expectBdryTets) {
                    std::ostringstream msg;
                    msg << tri->label() << ": "
                        << "bundleWithMonodromy gives the wrong number of "
                            "boundary tetrahedra.";
                    CPPUNIT_FAIL(msg.str());
                }

                // TODO: It would be nice to test something about
                // homology here.  Note that, since we are passing a
                // combinatorial isomorphism, b must have (M x S1) as a
                // finite sheeted cover.

                delete b;
                delete aut;
            }
        }

        void bundleWithMonodromy() {
            runCensusAllClosed(verifyBundleWithMonodromy, false /* small */);
            runCensusAllBounded(verifyBundleWithMonodromy, false /* small */);

            // A case for which using inverse isomorphism gluings will
            // definitely break things, since the gluings are not involutions.
            // This is the two-vertex, two-tetrahedron L(3,1);
            // the degree two vertex is 0(0) == 1(1).
            Triangulation<3> tri;
            Tetrahedron<3>* t0 = tri.newTetrahedron();
            Tetrahedron<3>* t1 = tri.newTetrahedron();
            t0->join(0, t1, regina::Perm<4>(1,3,0,2));
            t0->join(1, t1, regina::Perm<4>(1,2,3,0));
            t0->join(2, t1, regina::Perm<4>(1,2,3,0));
            t0->join(3, t1, regina::Perm<4>(1,2,3,0));
            tri.setLabel("Hand-coded L(3,1)");
            verifyBundleWithMonodromy(&tri);
        }
};

void addTriangulation4(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Triangulation4Test::suite());
}

