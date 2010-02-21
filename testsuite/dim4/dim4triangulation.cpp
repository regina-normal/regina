
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "dim4/dim4exampletriangulation.h"
#include "dim4/dim4isomorphism.h"
#include "dim4/dim4triangulation.h"
#include "manifold/nmanifold.h"
#include "subcomplex/nstandardtri.h"
#include "triangulation/nexampletriangulation.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/dim4/testdim4.h"

using regina::Dim4ExampleTriangulation;
using regina::Dim4Isomorphism;
using regina::Dim4Pentachoron;
using regina::Dim4Triangulation;
using regina::NAbelianGroup;
using regina::NExampleTriangulation;
using regina::NGroupPresentation;
using regina::NPerm5;
using regina::NStandardTriangulation;
using regina::NTriangulation;

class Dim4TriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Dim4TriangulationTest);

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(boundary);
    CPPUNIT_TEST(boundaryComponents);
    CPPUNIT_TEST(vertexLinks);
    CPPUNIT_TEST(eulerCharacteristic);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(fundGroup);
    CPPUNIT_TEST(makeCanonical);

    CPPUNIT_TEST_SUITE_END();

    private:
        // Trivial:
        Dim4Triangulation empty;
            /**< The empty triangulation. */

        // Closed orientable:
        Dim4Triangulation s4_id;
            /**< The 4-sphere, with two pentachora whose boundaries are
                 identified according to the identity map. */
        Dim4Triangulation s4_doubleConeS3;
            /**< A double cone over the 3-sphere. */
        Dim4Triangulation s3xs1;
            /**< The product S^3 x S^1. */

        // Closed non-orientable:
        Dim4Triangulation rp4;
            /**< Real projective 4-space, built from four pentachora. */
        Dim4Triangulation s3xs1Twisted;
            /**< The twisted product S^3 x~ S^1. */

        // Bounded orientable:
        Dim4Triangulation ball_singlePent;
            /**< A single pentachoron with no face gluings. */
        Dim4Triangulation ball_foldedPent;
            /**< A single pentachoron with two facets folded together. */
        Dim4Triangulation ball_singleConeS3;
            /**< A single cone over the 3-sphere. */
        Dim4Triangulation ball_layerAndFold;
            /**< Layer one pentachoron onto another over a single edge.
                 Then fold together the two remaining facets of one of the
                 pentachora. */

        // Ideal orientable:
        Dim4Triangulation idealPoincareProduct;
            /**< An ideal triangulation of the product P x I, where
                 P is the Poincare homology sphere. */

        // Both ideal and real boundary:
        Dim4Triangulation mixedPoincareProduct;
            /**< A triangulation of the product P x I with one real and one
                 ideal boundary, where P is the Poincare homology sphere. */

        // Invalid triangulations:
        Dim4Triangulation idealFigEightProduct;
            /**< A double cone over the figure eight knot complement.  All
                 three vertices should have links that are invalid because
                 they are closed and cusped.  The edges joining each cone
                 point to the centre vertex should be invalid because they
                 have torus links. */
        Dim4Triangulation mixedFigEightProduct;
            /**< A single cone over the figure eight knot complement.
                 Both vertices should be invalid; one has a closed and cusped
                 link, and the other has an invalid 3-manifold as its link.
                 The edge joining both vertices is invalid also, with a
                 torus link. */
        Dim4Triangulation pillow_twoCycle;
            /**< A "tetrahedral pillow" whose two facets are identified
                 according to a permutation in S_4, which in this case is
                 a pair swap. */
        Dim4Triangulation pillow_threeCycle;
            /**< A "tetrahedral pillow" whose two facets are identified
                 according to a permutation in S_4, which in this case is
                 a 3-cycle. */
        Dim4Triangulation pillow_fourCycle;
            /**< A "tetrahedral pillow" whose two facets are identified
                 according to a permutation in S_4, which in this case is
                 a 4-cycle. */

    public:
        void copyAndDelete(Dim4Triangulation& dest, Dim4Triangulation* source,
                const char* name) {
            dest.insertTriangulation(*source);
            dest.setPacketLabel(name);
            delete source;
        }

        void setUp() {
            // The empty triangulation needs no initialisation whatsoever.

            // We can pull some of our triangulations straight out of the can
            // via Dim4ExampleTriangulation.
            copyAndDelete(s4_id, Dim4ExampleTriangulation::fourSphere(),
                "S^4 (identity)");
            copyAndDelete(s3xs1, Dim4ExampleTriangulation::s3xs1(),
                "S^3 x S^1");
            copyAndDelete(rp4, Dim4ExampleTriangulation::rp4(), "RP^4");
            copyAndDelete(s3xs1Twisted,
                Dim4ExampleTriangulation::s3xs1Twisted(), "S^3 x~ S^1");

            // Some of our triangulations are built from 3-manifold
            // triangulations.
            NTriangulation* base;

            base = NExampleTriangulation::threeSphere();
            copyAndDelete(s4_doubleConeS3,
                Dim4ExampleTriangulation::doubleCone(*base),
                "S^4 (double cone)");
            copyAndDelete(ball_singleConeS3,
                Dim4ExampleTriangulation::singleCone(*base),
                "D^4 (single cone)");
            delete base;

            base = NExampleTriangulation::poincareHomologySphere();
            copyAndDelete(idealPoincareProduct,
                Dim4ExampleTriangulation::doubleCone(*base),
                "(S^3 / P_120) x I (double cone)");
            copyAndDelete(mixedPoincareProduct,
                Dim4ExampleTriangulation::singleCone(*base),
                "(S^3 / P_120) x I (single cone)");
            delete base;

            base = NExampleTriangulation::figureEightKnotComplement();
            copyAndDelete(idealFigEightProduct,
                Dim4ExampleTriangulation::doubleCone(*base),
                "Fig_8 x I (double cone)");
            copyAndDelete(mixedFigEightProduct,
                Dim4ExampleTriangulation::singleCone(*base),
                "Fig_8 x I (single cone)");
            delete base;

            // Build the rest manually.
            Dim4Pentachoron* p[2];

            ball_singlePent.addPentachoron(new Dim4Pentachoron());
            ball_singlePent.setPacketLabel("D^4 (single pentachoron)");

            p[0] = new Dim4Pentachoron();
            p[0]->joinTo(2, p[0], NPerm5(2, 4));
            ball_foldedPent.addPentachoron(p[0]);
            ball_foldedPent.setPacketLabel("Folded pentachoron");

            p[0] = new Dim4Pentachoron();
            p[1] = new Dim4Pentachoron();
            p[0]->joinTo(0, p[1], NPerm5());
            p[0]->joinTo(1, p[1], NPerm5());
            p[0]->joinTo(2, p[1], NPerm5());
            p[0]->joinTo(3, p[0], NPerm5(3, 4));
            ball_layerAndFold.addPentachoron(p[0]);
            ball_layerAndFold.addPentachoron(p[1]);
            ball_layerAndFold.setPacketLabel("Layered and folded ball");

            p[0] = new Dim4Pentachoron();
            p[1] = new Dim4Pentachoron();
            p[0]->joinTo(0, p[1], NPerm5());
            p[0]->joinTo(1, p[1], NPerm5());
            p[0]->joinTo(2, p[1], NPerm5());
            p[0]->joinTo(3, p[1], NPerm5());
            p[0]->joinTo(4, p[1], NPerm5(1, 2));
            pillow_twoCycle.addPentachoron(p[0]);
            pillow_twoCycle.addPentachoron(p[1]);
            pillow_twoCycle.setPacketLabel("Invalid 2-cycle pillow");

            p[0] = new Dim4Pentachoron();
            p[1] = new Dim4Pentachoron();
            p[0]->joinTo(0, p[1], NPerm5());
            p[0]->joinTo(1, p[1], NPerm5());
            p[0]->joinTo(2, p[1], NPerm5());
            p[0]->joinTo(3, p[1], NPerm5());
            p[0]->joinTo(4, p[1], NPerm5(2, 0, 1, 3, 4));
            pillow_threeCycle.addPentachoron(p[0]);
            pillow_threeCycle.addPentachoron(p[1]);
            pillow_threeCycle.setPacketLabel("Invalid 3-cycle pillow");

            p[0] = new Dim4Pentachoron();
            p[1] = new Dim4Pentachoron();
            p[0]->joinTo(0, p[1], NPerm5());
            p[0]->joinTo(1, p[1], NPerm5());
            p[0]->joinTo(2, p[1], NPerm5());
            p[0]->joinTo(3, p[1], NPerm5());
            p[0]->joinTo(4, p[1], NPerm5(3, 2, 0, 1, 4));
            pillow_fourCycle.addPentachoron(p[0]);
            pillow_fourCycle.addPentachoron(p[1]);
            pillow_fourCycle.setPacketLabel("Invalid 4-cycle pillow");
        }

        void tearDown() {
        }

        void verifyValid(const Dim4Triangulation& tri) {
            if (! tri.isValid()) {
                CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                    " is reported as invalid.");
            }
            unsigned long i;
            for (i = 0; i < tri.getNumberOfVertices(); ++i)
                if (! tri.getVertex(i)->isValid()) {
                    std::ostringstream msg;
                    msg << "Vertex " << i << " of triangulation "
                        << tri.getPacketLabel() << " is reported as invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            for (i = 0; i < tri.getNumberOfEdges(); ++i)
                if ((! tri.getEdge(i)->isValid()) ||
                        tri.getEdge(i)->hasBadLink() ||
                        tri.getEdge(i)->hasBadIdentification()) {
                    std::ostringstream msg;
                    msg << "Edge " << i << " of triangulation "
                        << tri.getPacketLabel() << " is reported as invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            for (i = 0; i < tri.getNumberOfFaces(); ++i)
                if (! tri.getFace(i)->isValid()) {
                    std::ostringstream msg;
                    msg << "Face " << i << " of triangulation "
                        << tri.getPacketLabel() << " is reported as invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
        }

        void verifyInvalid(const Dim4Triangulation& tri,
                int invalidVertices, int invalidEdges,
                int invalidEdgeLinks, int invalidEdgeIDs, int invalidFaces) {
            if (tri.isValid()) {
                CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                    " is reported as valid.");
            }
            unsigned long i;
            int found;

            found = 0;
            for (i = 0; i < tri.getNumberOfVertices(); ++i)
                if (! tri.getVertex(i)->isValid())
                    ++found;
            if (found != invalidVertices) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel()
                    << " contains " << found << " invalid vertices "
                    "instead of the expected " << invalidVertices << ".";
                CPPUNIT_FAIL(msg.str());
            }

            found = 0;
            for (i = 0; i < tri.getNumberOfEdges(); ++i)
                if (! tri.getEdge(i)->isValid())
                    ++found;
            if (found != invalidEdges) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel()
                    << " contains " << found << " invalid edges "
                    "instead of the expected " << invalidEdges << ".";
                CPPUNIT_FAIL(msg.str());
            }

            found = 0;
            for (i = 0; i < tri.getNumberOfEdges(); ++i)
                if (tri.getEdge(i)->hasBadLink())
                    ++found;
            if (found != invalidEdgeLinks) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel()
                    << " contains " << found << " invalid edge links "
                    "instead of the expected " << invalidEdgeLinks << ".";
                CPPUNIT_FAIL(msg.str());
            }

            found = 0;
            for (i = 0; i < tri.getNumberOfEdges(); ++i)
                if (tri.getEdge(i)->hasBadIdentification())
                    ++found;
            if (found != invalidEdgeIDs) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel() << " contains "
                    << found << " invalid edge self-identifications "
                    "instead of the expected " << invalidEdgeIDs << ".";
                CPPUNIT_FAIL(msg.str());
            }

            found = 0;
            for (i = 0; i < tri.getNumberOfFaces(); ++i)
                if (! tri.getFace(i)->isValid())
                    ++found;
            if (found != invalidFaces) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel()
                    << " contains " << found << " invalid faces "
                    "instead of the expected " << invalidFaces << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void validity() {
            verifyValid(empty);
            verifyValid(s4_id);
            verifyValid(s4_doubleConeS3);
            verifyValid(s3xs1);
            verifyValid(rp4);
            verifyValid(s3xs1Twisted);
            verifyValid(ball_singlePent);
            verifyValid(ball_foldedPent);
            verifyValid(ball_singleConeS3);
            verifyValid(ball_layerAndFold);
            verifyValid(idealPoincareProduct);
            verifyValid(mixedPoincareProduct);
            verifyInvalid(idealFigEightProduct, 3, 2, 2, 0, 0);
            verifyInvalid(mixedFigEightProduct, 2, 1, 1, 0, 0);
            verifyInvalid(pillow_twoCycle, 2, 2, 1, 1, 2);
            verifyInvalid(pillow_threeCycle, 0, 0, 0, 0, 1);
            verifyInvalid(pillow_fourCycle, 0, 1, 0, 1, 0);
        }

        void verifyConnected(const Dim4Triangulation& tri) {
            if (! tri.isConnected())
                CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                    " is reported as disconnected.");
        }

        void connectedness() {
            verifyConnected(empty);
            verifyConnected(s4_id);
            verifyConnected(s4_doubleConeS3);
            verifyConnected(s3xs1);
            verifyConnected(rp4);
            verifyConnected(s3xs1Twisted);
            verifyConnected(ball_singlePent);
            verifyConnected(ball_foldedPent);
            verifyConnected(ball_singleConeS3);
            verifyConnected(ball_layerAndFold);
            verifyConnected(idealPoincareProduct);
            verifyConnected(mixedPoincareProduct);
            verifyConnected(idealFigEightProduct);
            verifyConnected(mixedFigEightProduct);
            verifyConnected(pillow_twoCycle);
            verifyConnected(pillow_threeCycle);
            verifyConnected(pillow_fourCycle);
        }

        void verifyOrientable(const Dim4Triangulation& tri,
                bool isOrientable = true) {
            if (isOrientable) {
                if (! tri.isOrientable())
                    CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                        " is reported as non-orientable.");
            } else {
                if (tri.isOrientable())
                    CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                        " is reported as orientable.");
            }
        }

        void orientability() {
            verifyOrientable(empty);
            verifyOrientable(s4_id);
            verifyOrientable(s4_doubleConeS3);
            verifyOrientable(s3xs1);
            verifyOrientable(rp4, false);
            verifyOrientable(s3xs1Twisted, false);
            verifyOrientable(ball_singlePent);
            verifyOrientable(ball_foldedPent);
            verifyOrientable(ball_singleConeS3);
            verifyOrientable(ball_layerAndFold);
            verifyOrientable(idealPoincareProduct);
            verifyOrientable(mixedPoincareProduct);
            verifyOrientable(idealFigEightProduct);
            verifyOrientable(mixedFigEightProduct);
            verifyOrientable(pillow_twoCycle, false);
            verifyOrientable(pillow_threeCycle);
            verifyOrientable(pillow_fourCycle, false);
        }

        void verifyBoundary(const Dim4Triangulation& tri,
                bool realBdry = false, int idealVertices = 0,
                bool invalidVertices = false, bool valid = true) {
            bool closed = ! (realBdry || idealVertices || invalidVertices);

            if (closed) {
                if (! tri.isClosed())
                    CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                        " is reported as being not closed.");
            } else {
                if (tri.isClosed())
                    CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                        " is reported as being closed.");
            }

            if (realBdry) {
                if (! tri.hasBoundaryTetrahedra())
                    CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                        " is reported as having no boundary tetrahedra.");
            } else {
                if (tri.hasBoundaryTetrahedra())
                    CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                        " is reported as having boundary tetrahedra.");
            }

            if (idealVertices && valid) {
                if (! tri.isIdeal())
                    CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                        " is reported as being not ideal.");
            } else {
                if (tri.isIdeal())
                    CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                        " is reported as being ideal.");
            }

            unsigned long i;
            int found;

            found = 0;
            for (i = 0; i < tri.getNumberOfVertices(); ++i)
                if (tri.getVertex(i)->isIdeal())
                    ++found;
            if (found != idealVertices) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel()
                    << " contains " << found << " ideal vertices "
                    "instead of the expected " << idealVertices << ".";
                CPPUNIT_FAIL(msg.str());
            }

            // Hum, we've already check this but might as well cross-check.
            if (valid) {
                if (! tri.isValid())
                    CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                        " is reported as being invalid.");
            } else {
                if (tri.isValid())
                    CPPUNIT_FAIL("Triangulation " + tri.getPacketLabel() +
                        " is reported as being valid.");
            }
        }

        void boundary() {
            verifyBoundary(empty);
            verifyBoundary(s4_id);
            verifyBoundary(s4_doubleConeS3);
            verifyBoundary(s3xs1);
            verifyBoundary(rp4);
            verifyBoundary(s3xs1Twisted);
            verifyBoundary(ball_singlePent, true);
            verifyBoundary(ball_foldedPent, true);
            verifyBoundary(ball_singleConeS3, true);
            verifyBoundary(ball_layerAndFold, true);
            verifyBoundary(idealPoincareProduct, false, 2);
            verifyBoundary(mixedPoincareProduct, true, 1);
            verifyBoundary(idealFigEightProduct, false, 0, true, false);
            verifyBoundary(mixedFigEightProduct, true, 0, true, false);
            verifyBoundary(pillow_twoCycle, false, 0, true, false);
            verifyBoundary(pillow_threeCycle, false, 1, false, false);
            verifyBoundary(pillow_fourCycle, false, 0, false, false);
        }

        void verifyBoundaryCount(const Dim4Triangulation& tri, unsigned nBdry) {
            unsigned long ans = tri.getNumberOfBoundaryComponents();
            if (ans != nBdry) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel() << " gives "
                    << ans << " boundary component(s) instead of the expected "
                    << nBdry << "." << std::endl;
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyBoundaryTri(const Dim4Triangulation& tri,
                unsigned whichBdry, const char* bdryManifold,
                bool makeBdryFinite = false) {
            std::string ans;

            regina::NTriangulation t(
                *(tri.getBoundaryComponent(whichBdry)->getTriangulation()));
            t.intelligentSimplify();

            if (makeBdryFinite) {
                t.idealToFinite();
                t.intelligentSimplify();
            }

            NStandardTriangulation* std =
                NStandardTriangulation::isStandardTriangulation(&t);
            if (! std)
                ans = "<unrecognised triangulation>";
            else {
                regina::NManifold* mfd = std->getManifold();
                if (! mfd)
                    ans = "<unrecognised manifold>";
                else {
                    ans = mfd->getName();
                    delete mfd;
                }
                delete std;
            }

            if (ans != bdryManifold) {
                std::ostringstream msg;
                msg << "Boundary component " << whichBdry
                    << " of triangulation " << tri.getPacketLabel()
                    << " simplifies to " << ans
                    << " instead of the expected " << bdryManifold << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyBoundaryH1(const Dim4Triangulation& tri,
                unsigned whichBdry, const char* h1) {
            // For boundaries where we have little hope of recognising the
            // underlying triangulation or manifold.

            // Do a barycentric subdivision to turn any invalid edges
            // into proper RP^2 ideal boundaries.
            NTriangulation t(
                *(tri.getBoundaryComponent(whichBdry)->getTriangulation()));
            t.barycentricSubdivision();
            t.intelligentSimplify();

            std::string ans = t.getHomologyH1().toString();

            if (ans != h1) {
                std::ostringstream msg;
                msg << "Boundary component " << whichBdry
                    << " of triangulation " << tri.getPacketLabel()
                    << " has first homology " << ans
                    << " instead of the expected " << h1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void boundaryComponents() {
            verifyBoundaryCount(empty, 0);
            verifyBoundaryCount(s4_id, 0);
            verifyBoundaryCount(s4_doubleConeS3, 0);
            verifyBoundaryCount(s3xs1, 0);
            verifyBoundaryCount(rp4, 0);
            verifyBoundaryCount(s3xs1Twisted, 0);
            verifyBoundaryCount(ball_singlePent, 1);
            verifyBoundaryTri(ball_singlePent, 0, "S3");
            verifyBoundaryCount(ball_foldedPent, 1);
            verifyBoundaryTri(ball_foldedPent, 0, "S3");
            verifyBoundaryCount(ball_singleConeS3, 1);
            verifyBoundaryTri(ball_singleConeS3, 0, "S3");
            verifyBoundaryCount(ball_layerAndFold, 1);
            verifyBoundaryTri(ball_layerAndFold, 0, "S3");
            verifyBoundaryCount(idealPoincareProduct, 2);
            verifyBoundaryTri(idealPoincareProduct, 0, "S3/P120");
            verifyBoundaryTri(idealPoincareProduct, 1, "S3/P120");
            verifyBoundaryCount(mixedPoincareProduct, 2);
            verifyBoundaryTri(mixedPoincareProduct, 0, "S3/P120");
            verifyBoundaryTri(mixedPoincareProduct, 1, "S3/P120");
            verifyBoundaryCount(idealFigEightProduct, 3);
            // Boundary 0 of idealFigEightProduct should be the
            // suspension of a torus.  I think.
            verifyBoundaryTri(idealFigEightProduct, 0,
                "<unrecognised triangulation>");
            verifyBoundaryH1(idealFigEightProduct, 0, "2 Z");
            verifyBoundaryTri(idealFigEightProduct, 1,
                "Figure eight knot complement");
            verifyBoundaryTri(idealFigEightProduct, 2,
                "Figure eight knot complement");
            verifyBoundaryCount(mixedFigEightProduct, 2);
            verifyBoundaryTri(mixedFigEightProduct, 0,
                "Figure eight knot complement");
            verifyBoundaryTri(mixedFigEightProduct, 1,
                "Figure eight knot complement");
            verifyBoundaryCount(pillow_twoCycle, 2);
            // I *think* the links of the two invalid vertices for
            // pillow_twoCycle are (RP2 x I), but with one RP2 cusp and
            // one invalid edge (as opposed to two RP2 cusps).
            verifyBoundaryTri(pillow_twoCycle, 0,
                "<unrecognised triangulation>", true);
            verifyBoundaryH1(pillow_twoCycle, 0, "Z_2");
            verifyBoundaryTri(pillow_twoCycle, 1,
                "<unrecognised triangulation>", true);
            verifyBoundaryH1(pillow_twoCycle, 1, "Z_2");
            verifyBoundaryCount(pillow_threeCycle, 1);
            verifyBoundaryTri(pillow_threeCycle, 0, "L(3,1)");
            verifyBoundaryCount(pillow_fourCycle, 0);
        }

        void verifyLinkCount(const Dim4Triangulation& tri, unsigned nVert) {
            if (tri.getNumberOfVertices() != nVert) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel()
                    << " has " << tri.getNumberOfVertices()
                    << " vertices, not the expected " << nVert << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyLinksSpheres(const Dim4Triangulation& tri, unsigned nVert) {
            verifyLinkCount(tri, nVert);

            for (unsigned i = 0; i < nVert; ++i) {
                NTriangulation t(*(tri.getVertex(i)->getLink()));
                t.intelligentSimplify();

                std::string link;
                NStandardTriangulation* std =
                    NStandardTriangulation::isStandardTriangulation(&t);
                if (! std)
                    link = "<unrecognised triangulation>";
                else {
                    regina::NManifold* mfd = std->getManifold();
                    if (! mfd)
                        link = "<unrecognised manifold>";
                    else {
                        link = mfd->getName();
                        delete mfd;
                    }
                    delete std;
                }

                if (link != "S3") {
                    std::ostringstream msg;
                    msg << "Vertex " << i << " of triangulation "
                        << tri.getPacketLabel() << " simplifies to "
                        << link << ", not S3 as expected.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyLinksBalls(const Dim4Triangulation& tri, unsigned nVert) {
            verifyLinkCount(tri, nVert);

            for (unsigned i = 0; i < nVert; ++i) {
                NTriangulation t(*(tri.getVertex(i)->getLink()));
                t.intelligentSimplify();

                std::string link;
                NStandardTriangulation* std =
                    NStandardTriangulation::isStandardTriangulation(&t);
                if (! std)
                    link = "<unrecognised triangulation>";
                else {
                    regina::NManifold* mfd = std->getManifold();
                    if (! mfd)
                        link = "<unrecognised manifold>";
                    else {
                        link = mfd->getName();
                        delete mfd;
                    }
                    delete std;
                }

                if (link != "B3") {
                    std::ostringstream msg;
                    msg << "Vertex " << i << " of triangulation "
                        << tri.getPacketLabel() << " simplifies to "
                        << link << ", not B3 as expected.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyLink(const Dim4Triangulation& tri,
                unsigned whichVertex, const char* manifold) {
            std::string link;

            NTriangulation t(*(tri.getVertex(whichVertex)->getLink()));
            t.intelligentSimplify();

            NStandardTriangulation* std =
                NStandardTriangulation::isStandardTriangulation(&t);
            if (! std)
                link = "<unrecognised triangulation>";
            else {
                regina::NManifold* mfd = std->getManifold();
                if (! mfd)
                    link = "<unrecognised manifold>";
                else {
                    link = mfd->getName();
                    delete mfd;
                }
                delete std;
            }

            if (link != manifold) {
                std::ostringstream msg;
                msg << "Vertex " << whichVertex
                    << " of triangulation " << tri.getPacketLabel()
                    << " simplifies to " << link
                    << " instead of the expected " << manifold << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyLinkH1(const Dim4Triangulation& tri,
                unsigned whichVertex, const char* h1) {
            // For links where we have little hope of recognising the
            // underlying triangulation or manifold.

            // Do a barycentric subdivision to turn any invalid edges
            // into proper RP^2 ideal boundaries.
            NTriangulation t(*(tri.getVertex(whichVertex)->getLink()));
            t.barycentricSubdivision();
            t.intelligentSimplify();

            std::string ans = t.getHomologyH1().toString();

            if (ans != h1) {
                std::ostringstream msg;
                msg << "Vertex " << whichVertex
                    << " of triangulation " << tri.getPacketLabel()
                    << " has first homology " << ans
                    << " instead of the expected " << h1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void vertexLinks() {
            verifyLinksSpheres(empty, 0);
            verifyLinksSpheres(s4_id, 5);
            verifyLinksSpheres(s4_doubleConeS3, 3);
            verifyLinksSpheres(s3xs1, 1);
            verifyLinksSpheres(rp4, 3);
            verifyLinksSpheres(s3xs1Twisted, 1);
            verifyLinksBalls(ball_singlePent, 5);
            verifyLinksBalls(ball_foldedPent, 4);
            verifyLinkCount(ball_singleConeS3, 2);
            verifyLink(ball_singleConeS3, 0, "B3");
            verifyLink(ball_singleConeS3, 1, "S3");
            verifyLinksBalls(ball_layerAndFold, 4);
            verifyLinkCount(idealPoincareProduct, 3);
            verifyLink(idealPoincareProduct, 0, "S3");
            verifyLink(idealPoincareProduct, 1, "S3/P120");
            verifyLink(idealPoincareProduct, 2, "S3/P120");
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

        void verifyEulerChar(const Dim4Triangulation& tri,
                long expectedManifold, long expectedTri) {
            long eulerManifold = tri.getEulerCharManifold();
            long eulerTri = tri.getEulerCharTri();

            if (eulerManifold != expectedManifold) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel() << " gives "
                    "manifold Euler characteristic = " << eulerManifold
                    << " instead of the expected " << expectedManifold << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (eulerTri != expectedTri) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel() << " gives "
                    "triangulation Euler characteristic = " << eulerTri
                    << " instead of the expected " << expectedTri << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyEulerCharTri(const Dim4Triangulation& tri,
                long expectedTri) {
            long eulerTri = tri.getEulerCharTri();

            if (eulerTri != expectedTri) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel() << " gives "
                    "triangulation Euler characteristic = " << eulerTri
                    << " instead of the expected " << expectedTri << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void eulerCharacteristic() {
            verifyEulerChar(empty, 0, 0);
            verifyEulerChar(s4_id, 2, 2);
            verifyEulerChar(s4_doubleConeS3, 2, 2);
            verifyEulerChar(s3xs1, 0, 0);
            verifyEulerChar(rp4, 1, 1);
            verifyEulerChar(s3xs1Twisted, 0, 0);
            verifyEulerChar(ball_singlePent, 1, 1);
            verifyEulerChar(ball_foldedPent, 1, 1);
            verifyEulerChar(ball_singleConeS3, 1, 1);
            verifyEulerChar(ball_layerAndFold, 1, 1);
            verifyEulerChar(idealPoincareProduct, 0, 2);
            verifyEulerChar(mixedPoincareProduct, 0, 1);
            verifyEulerCharTri(idealFigEightProduct, 1);
            verifyEulerCharTri(mixedFigEightProduct, 1);
            verifyEulerCharTri(pillow_twoCycle, 2);
            verifyEulerCharTri(pillow_threeCycle, 2);
            verifyEulerCharTri(pillow_fourCycle, 0);
        }

        void verifyHomologyH1(const Dim4Triangulation& tri, const char* H1) {
            std::string ans = tri.getHomologyH1().toString();
            if (ans != H1) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel()
                    << " has homology H1 = " << ans
                    << " instead of the expected " << H1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void homologyH1() {
            verifyHomologyH1(empty, "0");
            verifyHomologyH1(s4_id, "0");
            verifyHomologyH1(s4_doubleConeS3, "0");
            verifyHomologyH1(s3xs1, "Z");
            verifyHomologyH1(rp4, "Z_2");
            verifyHomologyH1(s3xs1Twisted, "Z");
            verifyHomologyH1(ball_singlePent, "0");
            verifyHomologyH1(ball_foldedPent, "0");
            verifyHomologyH1(ball_singleConeS3, "0");
            verifyHomologyH1(ball_layerAndFold, "0");
            verifyHomologyH1(idealPoincareProduct, "0");
            verifyHomologyH1(mixedPoincareProduct, "0");
            verifyHomologyH1(idealFigEightProduct, "Z");
            verifyHomologyH1(mixedFigEightProduct, "Z");
        }

        void verifyFundGroup(const Dim4Triangulation& tri, const char* group) {
            std::string ans = tri.getFundamentalGroup().recogniseGroup();
            if (ans != group) {
                std::string showAns = ans;
                if (showAns.empty())
                    showAns = "<unrecognised>";

                std::string showGroup = group;
                if (showGroup.empty())
                    showGroup = "<unrecognised>";

                std::ostringstream msg;
                msg << "Triangulation " << tri.getPacketLabel()
                    << " has fundamental group = " << showAns
                    << " instead of the expected " << showGroup << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void fundGroup() {
            verifyFundGroup(empty, "0");
            verifyFundGroup(s4_id, "0");
            verifyFundGroup(s4_doubleConeS3, "0");
            verifyFundGroup(s3xs1, "Z");
            verifyFundGroup(rp4, "Z_2");
            verifyFundGroup(s3xs1Twisted, "Z");
            verifyFundGroup(ball_singlePent, "0");
            verifyFundGroup(ball_foldedPent, "0");
            verifyFundGroup(ball_singleConeS3, "0");
            verifyFundGroup(ball_layerAndFold, "0");
            verifyFundGroup(idealPoincareProduct, "");
            verifyFundGroup(mixedPoincareProduct, "");
            verifyFundGroup(idealFigEightProduct, "");
            verifyFundGroup(mixedFigEightProduct, "");
        }

        void verifyMakeCanonical(const Dim4Triangulation& tri,
                int trials = 10) {
            Dim4Triangulation canonical(tri);
            canonical.makeCanonical();

            for (int i = 0; i < trials; ++i) {
                Dim4Isomorphism* iso = Dim4Isomorphism::random(
                    tri.getNumberOfPentachora());
                Dim4Triangulation* t = iso->apply(&tri);
                delete iso;

                t->makeCanonical();

                if (! t->isIsomorphicTo(tri).get()) {
                    std::ostringstream msg;
                    msg << "Canonical form for "
                        << tri.getPacketLabel() << " is non-isomorphic.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (t->toStringLong() != canonical.toStringLong()) {
                    std::ostringstream msg;
                    msg << "Canonical form for "
                        << tri.getPacketLabel() << " is inconsistent.";
                    CPPUNIT_FAIL(msg.str());
                }

                delete t;
            }
        }

        void makeCanonical() {
            verifyMakeCanonical(empty);
            verifyMakeCanonical(s4_id);
            verifyMakeCanonical(s4_doubleConeS3);
            verifyMakeCanonical(s3xs1);
            verifyMakeCanonical(rp4);
            verifyMakeCanonical(s3xs1Twisted);
            verifyMakeCanonical(ball_singlePent);
            verifyMakeCanonical(ball_foldedPent);
            verifyMakeCanonical(ball_singleConeS3);
            verifyMakeCanonical(ball_layerAndFold);
            verifyMakeCanonical(idealPoincareProduct);
            verifyMakeCanonical(mixedPoincareProduct);
            verifyMakeCanonical(idealFigEightProduct);
            verifyMakeCanonical(mixedFigEightProduct);
            verifyMakeCanonical(pillow_twoCycle);
            verifyMakeCanonical(pillow_threeCycle);
            verifyMakeCanonical(pillow_fourCycle);
        }
};

void addDim4Triangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Dim4TriangulationTest::suite());
}

