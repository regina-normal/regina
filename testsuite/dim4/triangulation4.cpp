
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include <unistd.h>
#include "manifold/manifold.h"
#include "progress/progresstracker.h"
#include "subcomplex/standardtri.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/example3.h"
#include "triangulation/example4.h"

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
    CPPUNIT_TEST(copyMove);
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryFacets);
    CPPUNIT_TEST(boundaryBuild);
    CPPUNIT_TEST(edgeAccess);
    CPPUNIT_TEST(pachner<0>);
    CPPUNIT_TEST(pachner<1>);
    CPPUNIT_TEST(pachner<2>);
    CPPUNIT_TEST(pachner<3>);
    CPPUNIT_TEST(pachner<4>);
    CPPUNIT_TEST(chainComplex<1>);
    CPPUNIT_TEST(chainComplex<2>);
    CPPUNIT_TEST(chainComplex<3>);

    // Dimension-specific tests:
    CPPUNIT_TEST(magic);
    CPPUNIT_TEST(events);
    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(boundaryFlags);
    CPPUNIT_TEST(boundaryComponents);
    CPPUNIT_TEST(boundaryGluings);
    CPPUNIT_TEST(vertexLinksSpecific);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(homologyH2);
    CPPUNIT_TEST(homologyH3);
    CPPUNIT_TEST(fundGroup);
    CPPUNIT_TEST(barycentricSubdivision);
    CPPUNIT_TEST(vertexLinks);
    CPPUNIT_TEST(edgeLinks);
    CPPUNIT_TEST(idealToFinite);
    CPPUNIT_TEST(snapEdge);
    CPPUNIT_TEST(iBundle);
    CPPUNIT_TEST(s1Bundle);
    CPPUNIT_TEST(bundleWithMonodromy);
    CPPUNIT_TEST(fourFourMove);
    CPPUNIT_TEST(retriangulate);

    CPPUNIT_TEST_SUITE_END();

    private:
        // Closed orientable:
        Triangulation<4> s4_doubleConeS3;
            /**< A double cone over the 3-sphere. */
        Triangulation<4> cp2;
            /**< The complex projective plane. */
        Triangulation<4> s2xs2;
            /**< The product of a 2-sphere with a 2-sphere. */

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
        void setUp() override {
            TriangulationTest<4>::setUp();

            // We can pull some of our triangulations straight out of the can
            // via Example<4>.
            cp2 = Example<4>::cp2();
            s2xs2 = Example<4>::s2xs2();
            rp4 = Example<4>::rp4();

            // Some of our triangulations are built from 3-manifold
            // triangulations.
            {
                Triangulation<3> base = Example<3>::threeSphere();
                s4_doubleConeS3 = Example<4>::doubleCone(base);
                ball_singleConeS3 = Example<4>::singleCone(base);
            }

            {
                Triangulation<3> base = Example<3>::poincare();
                idealPoincareProduct = Example<4>::doubleCone(base);
                idealCappellShaneson = Example<4>::cappellShaneson();
                mixedPoincareProduct = Example<4>::singleCone(base);
            }

            {
                Triangulation<3> base = Example<3>::figureEight();
                idealFigEightProduct = Example<4>::doubleCone(base);
                mixedFigEightProduct = Example<4>::singleCone(base);
            }

            // Build the rest manually.
            Pentachoron<4>* p[2];

            {
                auto p = ball_foldedPent.newPentachoron();
                p->join(2, p, Perm<5>(2, 4));
            }
            {
                auto p = ball_layerAndFold.newPentachora<2>();
                p[0]->join(0, p[1], Perm<5>());
                p[0]->join(1, p[1], Perm<5>());
                p[0]->join(2, p[1], Perm<5>());
                p[0]->join(3, p[0], Perm<5>(3, 4));
            }
            {
                auto p = pillow_twoCycle.newPentachora<2>();
                p[0]->join(0, p[1], Perm<5>());
                p[0]->join(1, p[1], Perm<5>());
                p[0]->join(2, p[1], Perm<5>());
                p[0]->join(3, p[1], Perm<5>());
                p[0]->join(4, p[1], Perm<5>(1, 2));
            }
            {
                auto p = pillow_threeCycle.newPentachora<2>();
                p[0]->join(0, p[1], Perm<5>());
                p[0]->join(1, p[1], Perm<5>());
                p[0]->join(2, p[1], Perm<5>());
                p[0]->join(3, p[1], Perm<5>());
                p[0]->join(4, p[1], Perm<5>(2, 0, 1, 3, 4));
            }
            {
                auto p = pillow_fourCycle.newPentachora<2>();
                p[0]->join(0, p[1], Perm<5>());
                p[0]->join(1, p[1], Perm<5>());
                p[0]->join(2, p[1], Perm<5>());
                p[0]->join(3, p[1], Perm<5>());
                p[0]->join(4, p[1], Perm<5>(3, 2, 0, 1, 4));
            }

            // Build disconnected triangulations from others that we
            // already have.
            disjoint2.insertTriangulation(sphereBundle);
            disjoint2.insertTriangulation(ball_layerAndFold);

            disjoint3.insertTriangulation(rp4);
            disjoint3.insertTriangulation(ball_layerAndFold);
            disjoint3.insertTriangulation(idealPoincareProduct);
        }

        void tearDown() override {
        }

        /**
         * Run a given test over all hand-coded test cases.
         */
        void testManualAll(Triangulation4TestFunction f) {
            f(empty, "Empty");
            f(sphere, "Sphere");
            f(simplicialSphere, "Simplicial S^4");
            f(s4_doubleConeS3, "S^4 (double cone)");
            f(sphereBundle, "Sphere bundle");
            f(cp2, "CP^2");
            f(s2xs2, "S^2 x S^2");
            f(rp4, "RP^4");
            f(twistedSphereBundle, "Twisted sphere bundle");
            f(ball, "Ball");
            f(ball_foldedPent, "Folded pentachoron");
            f(ball_singleConeS3, "D^4 (single cone)");
            f(ball_layerAndFold, "Layered and folded ball");
            f(ballBundle, "Ball bundle");
            f(twistedBallBundle, "Twisted ball bundle");
            f(idealPoincareProduct, "(S^3 / P_120) x I (double cone)");
            f(idealCappellShaneson, "Ideal Cappell-Shaneson");
            f(mixedPoincareProduct, "(S^3 / P_120) x I (single cone)");
            f(idealFigEightProduct, "Fig_8 x I (double cone)");
            f(mixedFigEightProduct, "Fig_8 x I (single cone)");
            f(pillow_twoCycle, "Invalid 2-cycle pillow");
            f(pillow_threeCycle, "Invalid 3-cycle pillow");
            f(pillow_fourCycle, "Invalid 4-cycle pillow");
            f(disjoint2, "Disjoint(2)");
            f(disjoint3, "Disjoint(3)");
        }

        /**
         * Run a given test over all hand-coded test cases that are not
         * too large.
         */
        void testManualTiny(Triangulation4TestFunction f) {
            f(empty, "Empty");
            f(sphere, "Sphere");
            f(simplicialSphere, "Simplicial S^4");
            f(s4_doubleConeS3, "S^4 (double cone)");
            f(sphereBundle, "Sphere bundle");
            f(cp2, "CP^2");
            f(s2xs2, "S^2 x S^2");
            f(rp4, "RP^4");
            f(twistedSphereBundle, "Twisted sphere bundle");
            f(ball, "Ball");
            f(ball_foldedPent, "Folded pentachoron");
            f(ball_singleConeS3, "D^4 (single cone)");
            f(ball_layerAndFold, "Layered and folded ball");
            f(ballBundle, "Ball bundle");
            f(twistedBallBundle, "Twisted ball bundle");
            // f(idealPoincareProduct, "(S^3 / P_120) x I (double cone)");
            f(idealCappellShaneson, "Ideal Cappell-Shaneson");
            // f(mixedPoincareProduct, "(S^3 / P_120) x I (single cone)");
            f(idealFigEightProduct, "Fig_8 x I (double cone)");
            f(mixedFigEightProduct, "Fig_8 x I (single cone)");
            f(pillow_twoCycle, "Invalid 2-cycle pillow");
            f(pillow_threeCycle, "Invalid 3-cycle pillow");
            f(pillow_fourCycle, "Invalid 4-cycle pillow");
            f(disjoint2, "Disjoint(2)");
            // f(disjoint3, "Disjoint(3)");
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

        void boundaryFacets() {
            testManualAll(verifyBoundaryFacets);
        }

        void boundaryBuild() {
            testManualAll(verifyBoundaryBuild);
        }

        template <int k>
        void pachner() {
            testManualAll(verifyPachner<k>);
            runCensusAllBounded(verifyPachner<k>);
            runCensusAllNoBdry(verifyPachner<k>);
            verifyPachnerSimplicial<k>();
        }

        static void verifyFourFourMove(
                const Triangulation<4>& tri, const char* name ) {
            // Tests 4-4 moves on edges.
            for ( int i = 0; i < tri.countEdges(); ++i ) {
                Triangulation<4> newTri(tri);
                if ( newTri.isOrientable() ) {
                    newTri.orient();
                }
                bool legal = newTri.fourFourMove( newTri.edge(i) );
                clearProperties( newTri );

                if ( not legal ) {
                    Triangulation<4> oriented(tri);
                    if ( tri.isOrientable() ) {
                        oriented.orient();
                    }
                    if ( newTri != oriented ) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            "disallowed 4-4 move is not identical.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    continue;
                }

                // The move was performed (hopefully correctly).

                if ( newTri.size() != tri.size() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "4-4 move gives wrong triangulation size.";
                    CPPUNIT_FAIL(msg.str());
                }

                if ( newTri.isValid() != tri.isValid() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "4-4 move changes validity.";
                    CPPUNIT_FAIL(msg.str());
                }

                if ( newTri.isOrientable() != tri.isOrientable() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "4-4 move changes orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if ( tri.isOrientable() and not newTri.isOriented() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "4-4 move loses orientation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if ( newTri.isClosed() != tri.isClosed() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "4-4 move loses closedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if ( newTri.countBoundaryComponents() !=
                        tri.countBoundaryComponents() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "4-4 move changes # boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }

                if ( newTri.eulerCharTri() != tri.eulerCharTri() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "4-4 move changes Euler characteristic.";
                    CPPUNIT_FAIL(msg.str());
                }

                if ( tri.isValid() ) {
                    if ( not ( newTri.homology() == tri.homology() ) ) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            "4-4 move changes H1.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if ( not ( newTri.homology<2>() == tri.homology<2>() ) ) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            "4-4 move changes H2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                // Randomly relabel the pentachora, but preserve orientation.
                Isomorphism<4> iso = Isomorphism<4>::random(
                        newTri.size(), true );
                iso.applyInPlace( newTri );
                clearProperties( newTri );

                // Test the inverse 4-4 move.
                {
                    regina::Triangulation<4> copy(newTri);

                    // Find the edge on which to perform the inverse move.
                    // We perform a naive search that will find this edge if
                    // it exists, but may produce additional false positives.
                    // In the event that we do end up with false positives,
                    // we give up on testing this particular inverse move.
                    Edge<4>* edge = nullptr;
                    bool giveUp = false;
                    for ( int ii = 0; ii < copy.countEdges(); ++ii ) {
                        if ( copy.edge(ii)->degree() != 4 ) {
                            continue;
                        }
                        // A good edge meets four good pentachora.
                        bool goodEdge = true;
                        Edge<4>* e = copy.edge(ii);
                        for ( auto& emb : *e ) {
                            bool goodPent = false;
                            for ( int j = 1; j < 5; ++j ) {
                                if ( emb.pentachoron()->index() ==
                                        iso.simpImage(
                                            newTri.size() - j )
                                        ) {
                                    goodPent = true;
                                    break;
                                }
                            }
                            if ( not goodPent ) {
                                goodEdge = false;
                                break;
                            }
                        }
                        if ( goodEdge ) {
                            if ( edge == nullptr ) {
                                edge = e;
                            } else {
                                giveUp = true;
                                break;
                            }
                        }
                    }
                    if ( giveUp ) {
                        continue;
                    }

                    if ( edge == nullptr ) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            "could not find edge on which to perform "
                            "inverse 4-4 move.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    legal = copy.fourFourMove( edge );
                    clearProperties(copy);

                    if ( not legal ) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            "could not undo 4-4 move with inverse move.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if ( not copy.isIsomorphicTo(tri) ) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            "4-4 move followed by inverse move is not "
                            "isomorphic.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if ( tri.isOrientable() and not copy.isOriented() ) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            "4-4 move followed by inverse move loses "
                            "orientation.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void fourFourMove() {
            testManualAll( verifyFourFourMove );
            runCensusAllBounded( verifyFourFourMove );
            runCensusAllNoBdry( verifyFourFourMove );
        }

        static void verifySnapEdge(
                const Triangulation<4>& tri, const char* name ) {
            // Tests the snapEdge() operation.
            Triangulation<4> oriented(tri);
            if ( oriented.isOrientable() ) {
                oriented.orient();
            }
            for ( int i = 0; i < tri.countEdges(); ++i ) {
                Triangulation<4> newTri(oriented);
                Edge<4>* e = newTri.edge(i);

                // Start by working out whether e has the right properties
                // for snapEdge() to work.
                bool legal = ( e->vertex(0) != e->vertex(1) and
                        not ( e->vertex(0)->isBoundary() and
                            e->vertex(1)->isBoundary() ) );

                bool res = newTri.snapEdge(e);
                clearProperties( newTri );
                if ( res != legal ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "snapEdge() determines legality incorrectly.";
                    CPPUNIT_FAIL( msg.str() );
                }
                if ( not legal ) {
                    if ( newTri != oriented ) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            "disallowed snapEdge() is not identical.";
                        CPPUNIT_FAIL( msg.str() );
                    }
                    continue;
                }

                // The snapEdge() operation was performed.

                if ( newTri.size() != tri.size() + 4 ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "snapEdge() gives wrong triangulation size.";
                    CPPUNIT_FAIL( msg.str() );
                }

                if ( newTri.countVertices() != tri.countVertices() - 1 ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "snapEdge() does not reduce # vertices by one.";
                    CPPUNIT_FAIL( msg.str() );
                }

                if ( newTri.isValid() != tri.isValid() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "snapEdge() changes validity.";
                    CPPUNIT_FAIL( msg.str() );
                }

                if ( newTri.isOrientable() != tri.isOrientable() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "snapEdge() changes orientability.";
                    CPPUNIT_FAIL( msg.str() );
                }

                if ( tri.isOrientable() and not newTri.isOriented() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "snapEdge() loses orientation.";
                    CPPUNIT_FAIL( msg.str() );
                }

                if ( newTri.isClosed() != tri.isClosed() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "snapEdge() loses closedness.";
                    CPPUNIT_FAIL( msg.str() );
                }

                if ( newTri.countBoundaryComponents() !=
                        tri.countBoundaryComponents() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "snapEdge() changes # boundary components.";
                    CPPUNIT_FAIL( msg.str() );
                }

                if ( newTri.eulerCharTri() != tri.eulerCharTri() ) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        "snapEdge() changes Euler characteristic.";
                    CPPUNIT_FAIL( msg.str() );
                }

                if ( tri.isValid() ) {
                    if ( not ( newTri.homology() == tri.homology() ) ) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            "snapEdge() changes H1.";
                        CPPUNIT_FAIL( msg.str() );
                    }

                    if ( not ( newTri.homology<2>() ==
                                tri.homology<2>() ) ) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            "snapEdge() changes H2.";
                        CPPUNIT_FAIL( msg.str() );
                    }
                }
            }
        }

        void snapEdge() {
            testManualAll( verifySnapEdge );
            runCensusAllBounded( verifySnapEdge );
            runCensusAllNoBdry( verifySnapEdge );
        }

        template <int k>
        void chainComplex() {
            testManualAll(verifyChainComplex<k>);
        }

        static void verifyMagic(const Triangulation<4>& t, const char* name) {
            std::string sig = t.isoSig();

            {
                Triangulation<4> recon(sig);
                if (recon.isoSig() != sig) {
                    std::ostringstream msg;
                    msg << name << ": cannot reconstruct from "
                        "isomorphism signature using magic constructor.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void magic() {
            testManualAll(verifyMagic);

            try {
                Triangulation<4> t("INVALID");
                CPPUNIT_FAIL("The magic constructor did not throw an "
                    "exception for an invalid triangulation description.");
            } catch (const regina::InvalidArgument&) {
            }
        }

        void events() {
            /**
             * A struct that watches for changes on a triangulation,
             * and computes and stores rank(H2) immediately after a change.
             *
             * We use H2 because this is a property managed by the subclass
             * Triangulation<4>, not the parent class TriangulationBase<4>.
             *
             * The main purpose of this test is to ensure that
             * packetWasChanged is (a) fired, (b) fired only once, and
             * (c) fired at the corret time (i.e., after the subclass data
             * has been copied).
             */
            struct Watcher : public regina::PacketListener {
                int rank { -1 };
                int events { 0 };

                void packetWasChanged(regina::Packet& p) {
                    rank = static_cast<regina::PacketOf<Triangulation<4>>&>
                        (p).homology<2>().rank();
                    ++events;
                }
            };

            {
                auto p = regina::make_packet(
                    Example<4>::doubleCone(Example<3>::s2xs1()));
                Watcher w;
                p->listen(&w);

                if (p->homology<2>().rank() != 1) {
                    CPPUNIT_FAIL("doubleCone(S2xS1) should have rank(H2) = 1.");
                }

                // Copy assignment that changes H2
                *p = rp4;

                if (w.events != 1) {
                    std::ostringstream msg;
                    msg << "Packet event packetWasChanged() was called "
                        << w.events << " times during copy assignment.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (w.rank != 0) {
                    CPPUNIT_FAIL("Triangulation should have rank(H2) = 0 "
                        "in event handler after copy assignment.");
                }
                if (p->homology<2>().rank() != 0) {
                    CPPUNIT_FAIL("Triangulation should have rank(H2) = 0"
                        "after copy assignment.");
                }
            }
            {
                auto p = regina::make_packet(
                    Example<4>::doubleCone(Example<3>::s2xs1()));
                Watcher w;
                p->listen(&w);

                if (p->homology<2>().rank() != 1) {
                    CPPUNIT_FAIL("doubleCone(S2xS1) should have rank(H2) = 1.");
                }

                // Move assignment that changes H2
                // The extra insertTriangulation() is to ensure that the
                // move is not optimised away.
                Triangulation<4> t = rp4;
                t.insertTriangulation(t);
                *p = std::move(t);

                if (w.events != 1) {
                    std::ostringstream msg;
                    msg << "Packet event packetWasChanged() was called "
                        << w.events << " times during move assignment.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (w.rank != 0) {
                    CPPUNIT_FAIL("Triangulation should have rank(H2) = 0 "
                        "in event handler after move assignment.");
                }
                if (p->homology<2>().rank() != 0) {
                    CPPUNIT_FAIL("Triangulation should have rank(H2) = 0"
                        "after move assignment.");
                }
            }
        }

        void verifyInvalid(const Triangulation<4>& tri,
                int invalidVertices, int invalidEdges,
                int invalidEdgeLinks, int invalidEdgeIDs,
                int invalidTriangles, const char* name) {
            if (tri.isValid()) {
                std::ostringstream msg;
                msg << "Triangulation " << name << " is reported as valid.";
                CPPUNIT_FAIL(msg.str());
            }
            unsigned long i;
            int found;

            found = 0;
            for (i = 0; i < tri.countVertices(); ++i)
                if (! tri.vertex(i)->isValid())
                    ++found;
            if (found != invalidVertices) {
                std::ostringstream msg;
                msg << "Triangulation " << name
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
                msg << "Triangulation " << name
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
                msg << "Triangulation " << name
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
                msg << "Triangulation " << name << " contains "
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
                msg << "Triangulation " << name
                    << " contains " << found << " invalid triangles "
                    "instead of the expected " << invalidTriangles << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void validity() {
            verifyValid(empty, true, "Empty");
            verifyValid(sphere, true, "Sphere");
            verifyValid(simplicialSphere, true, "Simplicial S^4");
            verifyValid(s4_doubleConeS3, true, "S^4 (double cone)");
            verifyValid(sphereBundle, true, "Sphere bundle");
            verifyValid(cp2, true, "CP^2");
            verifyValid(s2xs2, true, "S^2 x S^2");
            verifyValid(rp4, true, "RP^4");
            verifyValid(twistedSphereBundle, true, "Twisted sphere bundle");
            verifyValid(ball, true, "Ball");
            verifyValid(ball_foldedPent, true, "Folded pentachoron");
            verifyValid(ball_singleConeS3, true, "D^4 (single cone)");
            verifyValid(ball_layerAndFold, true, "Layered and folded ball");
            verifyValid(ballBundle, true, "Ball bundle");
            verifyValid(twistedBallBundle, true, "Twisted ball bundle");
            verifyValid(idealPoincareProduct, true,
                "(S^3 / P_120) x I (double cone)");
            verifyValid(idealCappellShaneson, true, "Ideal Cappell-Shaneson");
            verifyValid(mixedPoincareProduct, true,
                "(S^3 / P_120) x I (single cone)");
            verifyInvalid(idealFigEightProduct, 3, 2, 2, 0, 0,
                "Fig_8 x I (double cone)");
            verifyInvalid(mixedFigEightProduct, 2, 1, 1, 0, 0,
                "Fig_8 x I (single cone)");
            verifyInvalid(pillow_twoCycle, 2, 2, 1, 1, 2,
                "Invalid 2-cycle pillow");
            verifyInvalid(pillow_threeCycle, 0, 0, 0, 0, 1,
                "Invalid 3-cycle pillow");
            verifyInvalid(pillow_fourCycle, 0, 1, 1, 1, 0,
                "Invalid 4-cycle pillow");
        }

        void connectedness() {
            verifyConnected(empty, true, "Empty");
            verifyConnected(sphere, true, "Sphere");
            verifyConnected(simplicialSphere, true, "Simplicial S^4");
            verifyConnected(s4_doubleConeS3, true, "S^4 (double cone)");
            verifyConnected(sphereBundle, true, "Sphere bundle");
            verifyConnected(cp2, true, "CP^2");
            verifyConnected(s2xs2, true, "S^2 x S^2");
            verifyConnected(rp4, true, "RP^4");
            verifyConnected(twistedSphereBundle, true, "Twisted sphere bundle");
            verifyConnected(ball, true, "Ball");
            verifyConnected(ball_foldedPent, true, "Folded pentachoron");
            verifyConnected(ball_singleConeS3, true, "D^4 (single cone)");
            verifyConnected(ball_layerAndFold, true, "Layered and folded ball");
            verifyConnected(ballBundle, true, "Ball bundle");
            verifyConnected(twistedBallBundle, true, "Twisted ball bundle");
            verifyConnected(idealPoincareProduct, true,
                "(S^3 / P_120) x I (double cone)");
            verifyConnected(idealCappellShaneson, true,
                "Ideal Cappell-Shaneson");
            verifyConnected(mixedPoincareProduct, true,
                "(S^3 / P_120) x I (single cone)");
            verifyConnected(idealFigEightProduct, true,
                "Fig_8 x I (double cone)");
            verifyConnected(mixedFigEightProduct, true,
                "Fig_8 x I (single cone)");
            verifyConnected(pillow_twoCycle, true, "Invalid 2-cycle pillow");
            verifyConnected(pillow_threeCycle, true, "Invalid 3-cycle pillow");
            verifyConnected(pillow_fourCycle, true, "Invalid 4-cycle pillow");
            verifyConnected(disjoint2, false, "Disjoint(2)");
            verifyConnected(disjoint3, false, "Disjoint(3)");
        }

        void orientability() {
            verifyOrientable(empty, true, "Empty");
            verifyOrientable(sphere, true, "Sphere");
            verifyOrientable(simplicialSphere, true, "Simplicial S^4");
            verifyOrientable(s4_doubleConeS3, true, "S^4 (double cone)");
            verifyOrientable(sphereBundle, true, "Sphere bundle");
            verifyOrientable(cp2, true, "CP^2");
            verifyOrientable(s2xs2, true, "S^2 x S^2");
            verifyOrientable(rp4, false, "RP^4");
            verifyOrientable(twistedSphereBundle, false,
                "Twisted sphere bundle");
            verifyOrientable(ball, true, "Ball");
            verifyOrientable(ball_foldedPent, true, "Folded pentachoron");
            verifyOrientable(ball_singleConeS3, true, "D^4 (single cone)");
            verifyOrientable(ball_layerAndFold, true,
                "Layered and folded ball");
            verifyOrientable(ballBundle, true, "Ball bundle");
            verifyOrientable(twistedBallBundle, false, "Twisted ball bundle");
            verifyOrientable(idealPoincareProduct, true,
                "(S^3 / P_120) x I (double cone)");
            verifyOrientable(idealCappellShaneson, true,
                "Ideal Cappell-Shaneson");
            verifyOrientable(mixedPoincareProduct, true,
                "(S^3 / P_120) x I (single cone)");
            verifyOrientable(idealFigEightProduct, true,
                "Fig_8 x I (double cone)");
            verifyOrientable(mixedFigEightProduct, true,
                "Fig_8 x I (single cone)");
            verifyOrientable(pillow_twoCycle, false, "Invalid 2-cycle pillow");
            verifyOrientable(pillow_threeCycle, true, "Invalid 3-cycle pillow");
            verifyOrientable(pillow_fourCycle, false, "Invalid 4-cycle pillow");
        }

        void verifyBoundary(const Triangulation<4>& tri, bool realBdry,
                int idealVertices, bool invalidVertices, bool valid,
                const char* name) {
            bool closed = ! (realBdry || idealVertices || invalidVertices);

            if (closed) {
                if (! tri.isClosed()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as being not closed.";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (tri.isClosed()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as being closed.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (realBdry) {
                if (! tri.hasBoundaryTetrahedra()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as having no boundary tetrahedra.";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (tri.hasBoundaryTetrahedra()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as having boundary tetrahedra.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (idealVertices && valid) {
                if (! tri.isIdeal()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as being not ideal.";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (tri.isIdeal()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as being ideal.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            unsigned long i;
            int found;

            found = 0;
            for (i = 0; i < tri.countVertices(); ++i)
                if (tri.vertex(i)->isIdeal())
                    ++found;
            if (found != idealVertices) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " contains " << found << " ideal vertices "
                    "instead of the expected " << idealVertices << ".";
                CPPUNIT_FAIL(msg.str());
            }

            // Hum, we've already check this but might as well cross-check.
            if (valid) {
                if (! tri.isValid()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as being invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (tri.isValid()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as being valid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void boundaryFlags() {
            verifyBoundary(empty, false, 0, false, true, "Empty");
            verifyBoundary(sphere, false, 0, false, true, "Sphere");
            verifyBoundary(simplicialSphere, false, 0, false, true,
                "Simplicial S^4");
            verifyBoundary(s4_doubleConeS3, false, 0, false, true,
                "S^4 (double cone)");
            verifyBoundary(sphereBundle, false, 0, false, true,
                "Sphere bundle");
            verifyBoundary(cp2, false, 0, false, true, "CP^2");
            verifyBoundary(s2xs2, false, 0, false, true, "S^2 x S^2");
            verifyBoundary(rp4, false, 0, false, true, "RP^4");
            verifyBoundary(twistedSphereBundle, false, 0, false, true,
                "Twisted sphere bundle");
            verifyBoundary(ball, true, 0, false, true, "Ball");
            verifyBoundary(ball_foldedPent, true, 0, false, true,
                "Folded pentachoron");
            verifyBoundary(ball_singleConeS3, true, 0, false, true,
                "D^4 (single cone)");
            verifyBoundary(ball_layerAndFold, true, 0, false, true,
                "Layered and folded ball");
            verifyBoundary(ballBundle, true, 0, false, true, "Ball bundle");
            verifyBoundary(twistedBallBundle, true, 0, false, true,
                "Twisted ball bundle");
            verifyBoundary(idealPoincareProduct, false, 2, false, true,
                "(S^3 / P_120) x I (double cone)");
            verifyBoundary(idealCappellShaneson, false, 1, false, true,
                "Ideal Cappell-Shaneson");
            verifyBoundary(mixedPoincareProduct, true, 1, false, true,
                "(S^3 / P_120) x I (single cone)");
            verifyBoundary(idealFigEightProduct, false, 0, true, false,
                "Fig_8 x I (double cone)");
            verifyBoundary(mixedFigEightProduct, true, 0, true, false,
                "Fig_8 x I (single cone)");
            verifyBoundary(pillow_twoCycle, false, 0, true, false,
                "Invalid 2-cycle pillow");
            verifyBoundary(pillow_threeCycle, false, 1, false, false,
                "Invalid 3-cycle pillow");
            verifyBoundary(pillow_fourCycle, false, 0, false, false,
                "Invalid 4-cycle pillow");
        }

        void verifyBoundaryTri(const Triangulation<4>& tri,
                unsigned whichBdry, const char* bdryManifold,
                bool makeBdryFinite, const char* name) {
            std::string ans;

            Triangulation<3> t = tri.boundaryComponent(whichBdry)->build();
            t.intelligentSimplify();

            if (makeBdryFinite) {
                t.idealToFinite();
                t.intelligentSimplify();
            }

            auto std = StandardTriangulation::recognise(t);
            if (! std)
                ans = "<unrecognised triangulation>";
            else {
                auto mfd = std->manifold();
                if (! mfd)
                    ans = "<unrecognised manifold>";
                else
                    ans = mfd->name();
            }

            if (ans != bdryManifold) {
                std::ostringstream msg;
                msg << "Boundary component " << whichBdry
                    << " of triangulation " << name
                    << " simplifies to " << ans
                    << " instead of the expected " << bdryManifold << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyBoundaryEuler(const Triangulation<4>& tri,
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
                BoundaryComponent<4>* bc = tri.boundaryComponent(i);

                // Do we have any pinched vertices or edges?
                // If so, these interfere with the Euler characteristic count.
                long vPinch = 0;
                long ePinch = 0;
                if (bc->isReal()) {
                    for (auto v : bc->vertices())
                        if (! v->isValid()) {
                            // Beware: the vertex *link* is a 3-manifold
                            // triangulation, and could itself have both real
                            // and ideal boundary components.
                            long realBdries = 0;
                            for (auto p : v->buildLink().boundaryComponents())
                                if (p->isReal())
                                    ++realBdries;
                            if (realBdries > 1)
                                vPinch += (realBdries - 1);
                        }
                    for (auto e : bc->edges())
                        if (! e->isValid()) {
                            long punctures = e->buildLink().
                                countBoundaryComponents();
                            if (punctures > 1)
                                ePinch += (punctures - 1);
                        }
                }

                long foundEuler = bc->eulerChar();
                long triEuler = bc->build().eulerCharTri();

                if (foundEuler != *eit - vPinch + ePinch) {
                    std::ostringstream msg;
                    msg << "Boundary component " << i
                        << " of triangulation " << name
                        << " reports Euler characteristic " << foundEuler
                        << " instead of the expected "
                        << (*eit - vPinch + ePinch) << ".";
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
            verifyBoundaryCount(empty, 0, 0, 0, "Empty");
            verifyBoundaryCount(sphere, 0, 0, 0, "Sphere");
            verifyBoundaryCount(simplicialSphere, 0, 0, 0, "Simplicial S^4");
            verifyBoundaryCount(s4_doubleConeS3, 0, 0, 0, "S^4 (double cone)");
            verifyBoundaryCount(sphereBundle, 0, 0, 0, "Sphere bundle");
            verifyBoundaryCount(cp2, 0, 0, 0, "CP^2");
            verifyBoundaryCount(s2xs2, 0, 0, 0, "S^2 x S^2");
            verifyBoundaryCount(rp4, 0, 0, 0, "RP^4");
            verifyBoundaryCount(twistedSphereBundle, 0, 0, 0,
                "Twisted sphere bundle");
            verifyBoundaryCount(ball, 1, 0, 0, "Ball");
            verifyBoundaryEuler(ball, { 0 }, "Ball");
            verifyBoundaryTri(ball, 0, "S3", false, "Ball");
            verifyBoundaryCount(ball_foldedPent, 1, 0, 0, "Folded pentachoron");
            verifyBoundaryEuler(ball_foldedPent, { 0 }, "Folded pentachoron");
            verifyBoundaryTri(ball_foldedPent, 0, "S3", false,
                "Folded pentachoron");
            verifyBoundaryCount(ball_singleConeS3, 1, 0, 0,
                "D^4 (single cone)");
            verifyBoundaryEuler(ball_singleConeS3, { 0 }, "D^4 (single cone)");
            verifyBoundaryTri(ball_singleConeS3, 0, "S3", false,
                "D^4 (single cone)");
            verifyBoundaryCount(ball_layerAndFold, 1, 0, 0,
                "Layered and folded ball");
            verifyBoundaryEuler(ball_layerAndFold, { 0 },
                "Layered and folded ball");
            verifyBoundaryTri(ball_layerAndFold, 0, "S3", false,
                "Layered and folded ball");
            verifyBoundaryCount(ballBundle, 1, 0, 0, "Ball bundle");
            verifyBoundaryEuler(ballBundle, { 0 }, "Ball bundle");
            verifyBoundaryTri(ballBundle, 0, "S2 x S1", false, "Ball bundle");
            verifyBoundaryCount(twistedBallBundle, 1, 0, 0,
                "Twisted ball bundle");
            verifyBoundaryEuler(twistedBallBundle, { 0 },
                "Twisted ball bundle");
            verifyBoundaryTri(twistedBallBundle, 0, "S2 x~ S1", false,
                "Twisted ball bundle");
            verifyBoundaryCount(idealPoincareProduct, 0, 2, 0,
                "(S^3 / P_120) x I (double cone)");
            verifyBoundaryEuler(idealPoincareProduct, { 0, 0 },
                "(S^3 / P_120) x I (double cone)");
            verifyBoundaryTri(idealPoincareProduct, 0, "S3/P120", false,
                "(S^3 / P_120) x I (double cone)");
            verifyBoundaryTri(idealPoincareProduct, 1, "S3/P120", false,
                "(S^3 / P_120) x I (double cone)");
            verifyBoundaryCount(idealCappellShaneson, 0, 1, 0,
                "Ideal Cappell-Shaneson");
            verifyBoundaryEuler(idealCappellShaneson, { 0 },
                "Ideal Cappell-Shaneson");
            verifyBoundaryTri(idealCappellShaneson, 0, "S2 x S1", false,
                "Ideal Cappell-Shaneson");
            verifyBoundaryCount(mixedPoincareProduct, 1, 1, 0,
                "(S^3 / P_120) x I (single cone)");
            verifyBoundaryEuler(mixedPoincareProduct, { 0, 0 },
                "(S^3 / P_120) x I (single cone)");
            verifyBoundaryTri(mixedPoincareProduct, 0, "S3/P120", false,
                "(S^3 / P_120) x I (single cone)");
            verifyBoundaryTri(mixedPoincareProduct, 1, "S3/P120", false,
                "(S^3 / P_120) x I (single cone)");
            verifyBoundaryCount(idealFigEightProduct, 0, 0, 3,
                "Fig_8 x I (double cone)");
            // Boundary 0 of idealFigEightProduct should be the
            // suspension of a torus.  I think.
            verifyBoundaryEuler(idealFigEightProduct, { 2, 1, 1 },
                "Fig_8 x I (double cone)");
            verifyBoundaryTri(idealFigEightProduct, 0,
                "<unrecognised triangulation>", false,
                "Fig_8 x I (double cone)");
            verifyBoundaryH1(idealFigEightProduct, 0, "2 Z",
                "Fig_8 x I (double cone)");
            verifyBoundaryTri(idealFigEightProduct, 1,
                "Figure eight knot complement", false,
                "Fig_8 x I (double cone)");
            verifyBoundaryTri(idealFigEightProduct, 2,
                "Figure eight knot complement", false,
                "Fig_8 x I (double cone)");
            verifyBoundaryCount(mixedFigEightProduct, 1, 0, 1,
                "Fig_8 x I (single cone)");
            verifyBoundaryEuler(mixedFigEightProduct, { 1, 1 },
                "Fig_8 x I (single cone)");
            verifyBoundaryTri(mixedFigEightProduct, 0,
                "Figure eight knot complement", false,
                "Fig_8 x I (single cone)");
            verifyBoundaryTri(mixedFigEightProduct, 1,
                "Figure eight knot complement", false,
                "Fig_8 x I (single cone)");
            verifyBoundaryCount(pillow_twoCycle, 0, 0, 2,
                "Invalid 2-cycle pillow");
            // I *think* the links of the two invalid vertices for
            // pillow_twoCycle are (RP2 x I), but with one RP2 cusp and
            // one invalid edge (as opposed to two RP2 cusps).
            // Think of a triangular pillow with its two triangular
            // faces glued together via a reflection.
            verifyBoundaryEuler(pillow_twoCycle, { 0, 0 },
                "Invalid 2-cycle pillow");
            verifyBoundaryTri(pillow_twoCycle, 0,
                "<unrecognised triangulation>", true,
                "Invalid 2-cycle pillow");
            verifyBoundaryH1(pillow_twoCycle, 0, "Z_2",
                "Invalid 2-cycle pillow");
            verifyBoundaryTri(pillow_twoCycle, 1,
                "<unrecognised triangulation>", true,
                "Invalid 2-cycle pillow");
            verifyBoundaryH1(pillow_twoCycle, 1, "Z_2",
                "Invalid 2-cycle pillow");
            verifyBoundaryCount(pillow_threeCycle, 0, 1, 0,
                "Invalid 3-cycle pillow");
            verifyBoundaryEuler(pillow_threeCycle, { 0 },
                "Invalid 3-cycle pillow");
            verifyBoundaryTri(pillow_threeCycle, 0, "L(3,1)", false,
                "Invalid 3-cycle pillow");
            verifyBoundaryCount(pillow_fourCycle, 0, 0, 0,
                "Invalid 4-cycle pillow");
        }

        void verifyBoundaryGluings(const Triangulation<4>& tri,
                const char* name) {
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
                    tet3 = bc->build().tetrahedron(j);
                    for (triangle = 0; triangle < 4; ++triangle) {
                        adj3 = tet3->adjacentTetrahedron(triangle);
                        if (adj3) {
                            adj4 = bc->tetrahedron(adj3->markedIndex());
                            if (tet4->triangle(triangle) !=
                                    adj4->triangle(
                                    tet3->adjacentFace(triangle))) {
                                std::ostringstream msg;
                                msg << "Boundary tetrahedron adjacency "
                                    "test failed for " << name
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
            verifyBoundaryGluings(ball, "Ball");
            verifyBoundaryGluings(ball_foldedPent, "Folded pentachoron");
            verifyBoundaryGluings(ball_singleConeS3, "D^4 (single cone)");
            verifyBoundaryGluings(ball_layerAndFold, "Layered and folded ball");
            verifyBoundaryGluings(ballBundle, "Ball bundle");
            verifyBoundaryGluings(twistedBallBundle, "Twisted ball bundle");
        }

        void verifyLinkCount(const Triangulation<4>& tri, unsigned nVert,
                const char* name) {
            if (tri.countVertices() != nVert) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " has " << tri.countVertices()
                    << " vertices, not the expected " << nVert << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyLinksSpheres(const Triangulation<4>& tri, unsigned nVert,
                const char* name) {
            verifyLinkCount(tri, nVert, name);

            for (unsigned i = 0; i < nVert; ++i) {
                Triangulation<3> t = tri.vertex(i)->buildLink();
                t.intelligentSimplify();

                std::string link;
                auto std = StandardTriangulation::recognise(t);
                if (! std)
                    link = "<unrecognised triangulation>";
                else {
                    auto mfd = std->manifold();
                    if (! mfd)
                        link = "<unrecognised manifold>";
                    else
                        link = mfd->name();
                }

                if (link != "S3") {
                    std::ostringstream msg;
                    msg << "Vertex " << i << " of triangulation "
                        << name << " simplifies to "
                        << link << ", not S3 as expected.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyLinksBalls(const Triangulation<4>& tri, unsigned nVert,
                const char* name) {
            verifyLinkCount(tri, nVert, name);

            for (unsigned i = 0; i < nVert; ++i) {
                Triangulation<3> t = tri.vertex(i)->buildLink();
                t.intelligentSimplify();

                std::string link;
                auto std = StandardTriangulation::recognise(t);
                if (! std)
                    link = "<unrecognised triangulation>";
                else {
                    auto mfd = std->manifold();
                    if (! mfd)
                        link = "<unrecognised manifold>";
                    else
                        link = mfd->name();
                }

                if (link != "B3") {
                    std::ostringstream msg;
                    msg << "Vertex " << i << " of triangulation "
                        << name << " simplifies to "
                        << link << ", not B3 as expected.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void verifyLink(const Triangulation<4>& tri,
                unsigned whichVertex, const char* manifold, const char* name) {
            std::string link;

            Triangulation<3> t = tri.vertex(whichVertex)->buildLink();
            t.intelligentSimplify();

            auto std = StandardTriangulation::recognise(t);
            if (! std)
                link = "<unrecognised triangulation>";
            else {
                auto mfd = std->manifold();
                if (! mfd)
                    link = "<unrecognised manifold>";
                else
                    link = mfd->name();
            }

            if (link != manifold) {
                std::ostringstream msg;
                msg << "Vertex " << whichVertex
                    << " of triangulation " << name
                    << " simplifies to " << link
                    << " instead of the expected " << manifold << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyLinkH1(const Triangulation<4>& tri,
                unsigned whichVertex, const char* h1, const char* name) {
            // For links where we have little hope of recognising the
            // underlying triangulation or manifold.

            // Do a barycentric subdivision to turn any invalid edges
            // into proper RP^2 ideal boundaries.
            Triangulation<3> t = tri.vertex(whichVertex)->buildLink();
            t.barycentricSubdivision();
            t.intelligentSimplify();

            std::string ans = t.homology().str();

            if (ans != h1) {
                std::ostringstream msg;
                msg << "Vertex " << whichVertex
                    << " of triangulation " << name
                    << " has first homology " << ans
                    << " instead of the expected " << h1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void vertexLinksSpecific() {
            verifyLinksSpheres(empty, 0, "Empty");
            verifyLinksSpheres(sphere, 5, "Sphere");
            verifyLinksSpheres(simplicialSphere, 6, "Simplicial S^4");
            verifyLinksSpheres(s4_doubleConeS3, 3, "S^4 (double cone)");
            verifyLinksSpheres(sphereBundle, 1, "Sphere bundle");
            verifyLinksSpheres(cp2, 4, "CP^2");
            verifyLinksSpheres(s2xs2, 5, "S^2 x S^2");
            verifyLinksSpheres(rp4, 3, "RP^4");
            verifyLinksSpheres(twistedSphereBundle, 1, "Twisted sphere bundle");
            verifyLinksBalls(ball, 5, "Ball");
            verifyLinksBalls(ball_foldedPent, 4, "Folded pentachoron");
            verifyLinkCount(ball_singleConeS3, 2, "D^4 (single cone)");
            verifyLink(ball_singleConeS3, 0, "B3", "D^4 (single cone)");
            verifyLink(ball_singleConeS3, 1, "S3", "D^4 (single cone)");
            verifyLinksBalls(ball_layerAndFold, 4, "Layered and folded ball");
            verifyLinksBalls(ballBundle, 2, "Ball bundle");
            verifyLinksBalls(twistedBallBundle, 1, "Twisted ball bundle");
            verifyLinkCount(idealPoincareProduct, 3,
                "(S^3 / P_120) x I (double cone)");
            verifyLink(idealPoincareProduct, 0, "S3",
                "(S^3 / P_120) x I (double cone)");
            verifyLink(idealPoincareProduct, 1, "S3/P120",
                "(S^3 / P_120) x I (double cone)");
            verifyLink(idealPoincareProduct, 2, "S3/P120",
                "(S^3 / P_120) x I (double cone)");
            verifyLinkCount(idealCappellShaneson, 1, "Ideal Cappell-Shaneson");
            verifyLink(idealCappellShaneson, 0, "S2 x S1",
                "Ideal Cappell-Shaneson");
            verifyLinkCount(mixedPoincareProduct, 2,
                "(S^3 / P_120) x I (single cone)");
            verifyLink(mixedPoincareProduct, 0, "B3",
                "(S^3 / P_120) x I (single cone)");
            verifyLink(mixedPoincareProduct, 1, "S3/P120",
                "(S^3 / P_120) x I (single cone)");
            verifyLinkCount(idealFigEightProduct, 3, "Fig_8 x I (double cone)");
            // The next link should be (?) the suspension of a torus.
            verifyLink(idealFigEightProduct, 0, "<unrecognised triangulation>",
                "Fig_8 x I (double cone)");
            verifyLinkH1(idealFigEightProduct, 0, "2 Z",
                "Fig_8 x I (double cone)");
            verifyLink(idealFigEightProduct, 1, "Figure eight knot complement",
                "Fig_8 x I (double cone)");
            verifyLink(idealFigEightProduct, 2, "Figure eight knot complement",
                "Fig_8 x I (double cone)");
            verifyLinkCount(mixedFigEightProduct, 2, "Fig_8 x I (single cone)");
            // The next link should be (?) the cone of a torus.
            verifyLink(mixedFigEightProduct, 0, "<unrecognised triangulation>",
                "Fig_8 x I (single cone)");
            verifyLinkH1(mixedFigEightProduct, 0, "2 Z",
                "Fig_8 x I (single cone)");
            verifyLink(mixedFigEightProduct, 1, "Figure eight knot complement",
                "Fig_8 x I (single cone)");
            verifyLinkCount(pillow_twoCycle, 4, "Invalid 2-cycle pillow");
            // Two of these vertex links are invalid 3-manifold
            // triangulations (specifically, with invalid edges).
            // I *think* these are each triangulations of (RP^2 x I) with one
            // RP^2 at an ideal vertex and one RP^2 inside an invalid edge.
            verifyLink(pillow_twoCycle, 0, "<unrecognised triangulation>",
                "Invalid 2-cycle pillow");
            verifyLinkH1(pillow_twoCycle, 0, "Z_2", "Invalid 2-cycle pillow");
            verifyLink(pillow_twoCycle, 1, "S3", "Invalid 2-cycle pillow");
            verifyLink(pillow_twoCycle, 2, "<unrecognised triangulation>",
                "Invalid 2-cycle pillow");
            verifyLinkH1(pillow_twoCycle, 2, "Z_2", "Invalid 2-cycle pillow");
            verifyLink(pillow_twoCycle, 3, "S3", "Invalid 2-cycle pillow");
            verifyLinkCount(pillow_threeCycle, 3, "Invalid 3-cycle pillow");
            verifyLink(pillow_threeCycle, 0, "S3", "Invalid 3-cycle pillow");
            verifyLink(pillow_threeCycle, 1, "L(3,1)",
                "Invalid 3-cycle pillow");
            verifyLink(pillow_threeCycle, 2, "S3", "Invalid 3-cycle pillow");
            verifyLinksSpheres(pillow_fourCycle, 2, "Invalid 4-cycle pillow");
        }

        void verifyEulerChar(const Triangulation<4>& tri,
                long expectedManifold, long expectedTri, const char* name) {
            long eulerManifold = tri.eulerCharManifold();
            long eulerTri = tri.eulerCharTri();

            if (eulerManifold != expectedManifold) {
                std::ostringstream msg;
                msg << "Triangulation " << name << " gives "
                    "manifold Euler characteristic = " << eulerManifold
                    << " instead of the expected " << expectedManifold << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (eulerTri != expectedTri) {
                std::ostringstream msg;
                msg << "Triangulation " << name << " gives "
                    "triangulation Euler characteristic = " << eulerTri
                    << " instead of the expected " << expectedTri << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void eulerChar() {
            verifyEulerChar(empty, 0, 0, "Empty");
            verifyEulerChar(sphere, 2, 2, "Sphere");
            verifyEulerChar(simplicialSphere, 2, 2, "Simplicial S^4");
            verifyEulerChar(s4_doubleConeS3, 2, 2, "S^4 (double cone)");
            verifyEulerChar(sphereBundle, 0, 0, "Sphere bundle");
            verifyEulerChar(cp2, 3, 3, "CP^2");
            verifyEulerChar(s2xs2, 4, 4, "S^2 x S^2");
            verifyEulerChar(rp4, 1, 1, "RP^4");
            verifyEulerChar(twistedSphereBundle, 0, 0, "Twisted sphere bundle");
            verifyEulerChar(ball, 1, 1, "Ball");
            verifyEulerChar(ball_foldedPent, 1, 1, "Folded pentachoron");
            verifyEulerChar(ball_singleConeS3, 1, 1, "D^4 (single cone)");
            verifyEulerChar(ball_layerAndFold, 1, 1, "Layered and folded ball");
            verifyEulerChar(ballBundle, 0, 0, "Ball bundle");
            verifyEulerChar(twistedBallBundle, 0, 0, "Twisted ball bundle");
            verifyEulerChar(idealPoincareProduct, 0, 2,
                "(S^3 / P_120) x I (double cone)");
            verifyEulerChar(mixedPoincareProduct, 0, 1,
                "(S^3 / P_120) x I (single cone)");
            verifyEulerCharTri(idealFigEightProduct, 1,
                "Fig_8 x I (double cone)");
            verifyEulerCharTri(mixedFigEightProduct, 1,
                "Fig_8 x I (single cone)");
            verifyEulerCharTri(pillow_twoCycle, 2, "Invalid 2-cycle pillow");
            verifyEulerCharTri(pillow_threeCycle, 2, "Invalid 3-cycle pillow");
            verifyEulerCharTri(pillow_fourCycle, 0, "Invalid 4-cycle pillow");
        }

        void homologyH1() {
            verifyHomology<1>(empty, "0", "Empty");
            verifyHomology<1>(sphere, "0", "Sphere");
            verifyHomology<1>(simplicialSphere, "0", "Simplicial S^4");
            verifyHomology<1>(s4_doubleConeS3, "0", "S^4 (double cone)");
            verifyHomology<1>(sphereBundle, "Z", "Sphere bundle");
            verifyHomology<1>(cp2, "0", "CP^2");
            verifyHomology<1>(s2xs2, "0", "S^2 x S^2");
            verifyHomology<1>(rp4, "Z_2", "RP^4");
            verifyHomology<1>(twistedSphereBundle, "Z",
                "Twisted sphere bundle");
            verifyHomology<1>(ball, "0", "Ball");
            verifyHomology<1>(ball_foldedPent, "0", "Folded pentachoron");
            verifyHomology<1>(ball_singleConeS3, "0", "D^4 (single cone)");
            verifyHomology<1>(ball_layerAndFold, "0",
                "Layered and folded ball");
            verifyHomology<1>(ballBundle, "Z", "Ball bundle");
            verifyHomology<1>(twistedBallBundle, "Z", "Twisted ball bundle");
            verifyHomology<1>(idealPoincareProduct, "0",
                "(S^3 / P_120) x I (double cone)");
            verifyHomology<1>(idealCappellShaneson, "Z",
                "Ideal Cappell-Shaneson");
            verifyHomology<1>(mixedPoincareProduct, "0",
                "(S^3 / P_120) x I (single cone)");
            verifyHomology<1>(idealFigEightProduct, "Z",
                "Fig_8 x I (double cone)");
            verifyHomology<1>(mixedFigEightProduct, "Z",
                "Fig_8 x I (single cone)");
        }

        void homologyH2() {
            verifyHomology<2>(empty, "0", "Empty");
            verifyHomology<2>(sphere, "0", "Sphere");
            verifyHomology<2>(simplicialSphere, "0", "Simplicial S^4");
            verifyHomology<2>(s4_doubleConeS3, "0", "S^4 (double cone)");
            verifyHomology<2>(sphereBundle, "0", "Sphere bundle");
            verifyHomology<2>(cp2, "Z", "CP^2");
            verifyHomology<2>(s2xs2, "2 Z", "S^2 x S^2");
            verifyHomology<2>(rp4, "0", "RP^4");
            verifyHomology<2>(twistedSphereBundle, "0",
                "Twisted sphere bundle");
            verifyHomology<2>(ball, "0", "Ball");
            verifyHomology<2>(ball_foldedPent, "0", "Folded pentachoron");
            verifyHomology<2>(ball_singleConeS3, "0", "D^4 (single cone)");
            verifyHomology<2>(ball_layerAndFold, "0",
                "Layered and folded ball");
            verifyHomology<2>(ballBundle, "0", "Ball bundle");
            verifyHomology<2>(twistedBallBundle, "0", "Twisted ball bundle");
            verifyHomology<2>(idealPoincareProduct, "0",
                "(S^3 / P_120) x I (double cone)");
            verifyHomology<2>(idealCappellShaneson, "0",
                "Ideal Cappell-Shaneson");
            verifyHomology<2>(mixedPoincareProduct, "0",
                "(S^3 / P_120) x I (single cone)");
        }

        void homologyH3() {
            verifyHomology<3>(empty, "0", "Empty");
            verifyHomology<3>(sphere, "0", "Sphere");
            verifyHomology<3>(simplicialSphere, "0", "Simplicial S^4");
            verifyHomology<3>(s4_doubleConeS3, "0", "S^4 (double cone)");
            verifyHomology<3>(sphereBundle, "Z", "Sphere bundle");
            verifyHomology<3>(cp2, "0", "CP^2");
            verifyHomology<3>(s2xs2, "0", "S^2 x S^2");
            verifyHomology<3>(rp4, "Z_2", "RP^4");
            verifyHomology<3>(twistedSphereBundle, "Z_2",
                "Twisted sphere bundle");
            verifyHomology<3>(ball, "0", "Ball");
            verifyHomology<3>(ball_foldedPent, "0", "Folded pentachoron");
            verifyHomology<3>(ball_singleConeS3, "0", "D^4 (single cone)");
            verifyHomology<3>(ball_layerAndFold, "0",
                "Layered and folded ball");
            verifyHomology<3>(ballBundle, "0", "Ball bundle");
            verifyHomology<3>(twistedBallBundle, "0", "Twisted ball bundle");
            verifyHomology<3>(idealPoincareProduct, "Z",
                "(S^3 / P_120) x I (double cone)");
            verifyHomology<3>(idealCappellShaneson, "0",
                "Ideal Cappell-Shaneson");
            verifyHomology<3>(mixedPoincareProduct, "Z",
                "(S^3 / P_120) x I (single cone)");
        }

        void fundGroup() {
            verifyFundGroup(empty, "0", "Empty");
            verifyFundGroup(sphere, "0", "Sphere");
            verifyFundGroup(simplicialSphere, "0", "Simplicial S^4");
            verifyFundGroup(s4_doubleConeS3, "0", "S^4 (double cone)");
            verifyFundGroup(sphereBundle, "Z", "Sphere bundle");
            verifyFundGroup(cp2, "0", "CP^2");
            verifyFundGroup(s2xs2, "0", "S^2 x S^2");
            verifyFundGroup(rp4, "Z_2", "RP^4");
            verifyFundGroup(twistedSphereBundle, "Z", "Twisted sphere bundle");
            verifyFundGroup(ball, "0", "Ball");
            verifyFundGroup(ball_foldedPent, "0", "Folded pentachoron");
            verifyFundGroup(ball_singleConeS3, "0", "D^4 (single cone)");
            verifyFundGroup(ball_layerAndFold, "0", "Layered and folded ball");
            verifyFundGroup(ballBundle, "Z", "Ball bundle");
            verifyFundGroup(twistedBallBundle, "Z", "Twisted ball bundle");
            verifyFundGroup(idealPoincareProduct, "",
                "(S^3 / P_120) x I (double cone)");
            verifyFundGroup(mixedPoincareProduct, "",
                "(S^3 / P_120) x I (single cone)");
            verifyFundGroup(idealFigEightProduct,
                "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b^2 a^-1 b",
                "Fig_8 x I (double cone)");
            verifyFundGroup(mixedFigEightProduct,
                "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b^2 a^-1 b",
                "Fig_8 x I (single cone)");
        }

        static void verifyBary(const Triangulation<4>& tri, const char* name) {
            Triangulation<4> b(tri);
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

            // Ideal triangulations must be valid, so only consider the
            // valid -> valid case here.
            if (tri.isValid() && (tri.isIdeal() != b.isIdeal())) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision breaks idealness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.hasBoundaryTetrahedra() != b.hasBoundaryTetrahedra()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks boundary tetrahedra.";
                CPPUNIT_FAIL(msg.str());
            }

            // Some invalid -> valid cases can turn non-closed into closed.
            // Just consider valid only.
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

            // Invalid vertices and edges can wreak havoc on the
            // counting and labelling of boundary components (see
            // the BoundaryComponent class notes for details).
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

            if (! (tri.homology<2>() == b.homology<2>())) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision breaks H2.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyBarycentricInvalidToValid(const Triangulation<4>& tri,
                const char* name) {
            Triangulation<4> use(tri);

            if (use.isValid()) {
                std::ostringstream msg;
                msg << name << ": Incorrectly marked as valid.";
                CPPUNIT_FAIL(msg.str());
            }

            use.barycentricSubdivision();

            if (! use.isValid()) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision is invalid.";
                CPPUNIT_FAIL(msg.str());
            }

            clearProperties(use);

            if (! use.isValid()) {
                std::ostringstream msg;
                msg << name << ": Barycentric subdivision is invalid.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void barycentricSubdivision() {
            testManualTiny(verifyBary);

            verifyBarycentricInvalidToValid(pillow_threeCycle,
                "Invalid 3-cycle pillow");
            verifyBarycentricInvalidToValid(pillow_fourCycle,
                "Invalid 4-cycle pillow");
        }

        static void verifyVertexLinks(const Triangulation<4>& tri,
                const char* name) {
            for (unsigned long i = 0; i < tri.countVertices(); ++i) {
                Vertex<4>* v = tri.vertex(i);

                const Triangulation<3>& link = v->buildLink();
                Isomorphism<4> iso = v->buildLinkInclusion();

                if (link.size() != v->degree()) {
                    std::ostringstream msg;
                    msg << name << ", vertex " << i << ": "
                        << "link has incorrect number of tetrahedra.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! link.isConnected()) {
                    std::ostringstream msg;
                    msg << name << ", vertex " << i << ": "
                        << "link of vertex is not connected.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (v->isValid()) {
                    if (v->isBoundary() && v->boundaryComponent()->
                            countTetrahedra() > 0) {
                        if (! link.isBall()) {
                            std::ostringstream msg;
                            msg << name << ", vertex "
                                << i << ": "
                                << "link of real boundary vertex "
                                "is not a 3-ball.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else if (v->isBoundary()) {
                        if (! link.isClosed()) {
                            std::ostringstream msg;
                            msg << name << ", vertex "
                                << i << ": "
                                << "link of ideal boundary vertex "
                                "is not a closed 3-manifold.";
                            CPPUNIT_FAIL(msg.str());
                        }
                        if (link.isSphere()) {
                            std::ostringstream msg;
                            msg << name << ", vertex "
                                << i << ": "
                                << "link of ideal boundary vertex "
                                "is a 3-sphere.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (! link.isSphere()) {
                            std::ostringstream msg;
                            msg << name << ", vertex "
                                << i << ": "
                                << "link of internal edge is not a 3-sphere.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                } else {
                    // Invalid vertex.
                    if (! v->isBoundary()) {
                        std::ostringstream msg;
                        msg << name << ", vertex "
                            << i << ": "
                            << "invalid vertex is not marked as boundary.";
                        CPPUNIT_FAIL(msg.str());
                    } else if (v->boundaryComponent()->
                            countTetrahedra() > 0) {
                        // Link should have boundary faces but not be a 3-ball.
                        if (! link.hasBoundaryTriangles()) {
                            std::ostringstream msg;
                            msg << name << ", vertex "
                                << i << ": "
                                << "link of invalid real boundary vertex "
                                "has no boundary faces.";
                            CPPUNIT_FAIL(msg.str());
                        }
                        if (link.isBall()) {
                            std::ostringstream msg;
                            msg << name << ", vertex "
                                << i << ": "
                                << "link of invalid real boundary vertex "
                                "is a 3-ball.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        // Link should have no boundary faces, but not
                        // be a closed 3-manifold.
                        if (link.hasBoundaryTriangles()) {
                            std::ostringstream msg;
                            msg << name << ", vertex "
                                << i << ": "
                                << "link of invalid ideal vertex "
                                "has boundary faces.";
                            CPPUNIT_FAIL(msg.str());
                        }
                        if (link.isClosed()) {
                            std::ostringstream msg;
                            msg << name << ", vertex "
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
                const Pentachoron<4>* p;
                Perm<5> perm;
                const regina::Tetrahedron<3> *t, *adj;
                unsigned vNum;
                for (j = 0; j < v->degree(); ++j) {
                    p = tri.pentachoron(iso.pentImage(j));
                    perm = iso.facetPerm(j);
                    vNum = perm[4];
                    if (p->vertex(vNum) != v) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link does not map 4 -> vertex correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (perm[0] != p->tetrahedronMapping(vNum)[0] ||
                            perm[1] != p->tetrahedronMapping(vNum)[1] ||
                            perm[2] != p->tetrahedronMapping(vNum)[2] ||
                            perm[3] != p->tetrahedronMapping(vNum)[3]) {
                        std::ostringstream msg;
                        msg << name << ", vertex " << i << ": "
                            << "link does not map 0,1,2,3 -> opposite "
                            "tetrahedron correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    for (k = 0; k < 4; ++k) {
                        t = link.tetrahedron(j);
                        adj = t->adjacentTetrahedron(k);
                        if (adj) {
                            if (! p->adjacentPentachoron(perm[k])) {
                                std::ostringstream msg;
                                msg << name << ", vertex " << i << ": "
                                    << "link has extra adjacent tetrahedron.";
                                CPPUNIT_FAIL(msg.str());
                            } else if (p->adjacentPentachoron(perm[k]) !=
                                    tri.pentachoron(iso.pentImage(
                                    adj->index()))) {
                                std::ostringstream msg;
                                msg << name << ", vertex " << i << ": "
                                    << "link has wrong adjacent tetrahedron.";
                                CPPUNIT_FAIL(msg.str());
                            } else if (p->adjacentGluing(perm[k]) !=
                                    iso.facetPerm(adj->index()) *
                                    Perm<5>::extend(t->adjacentGluing(k)) *
                                    perm.inverse()) {
                                std::ostringstream msg;
                                msg << name << ", vertex " << i << ": "
                                    << "link has wrong adjacent gluing.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        } else {
                            if (p->adjacentPentachoron(perm[k])) {
                                std::ostringstream msg;
                                msg << name << ", vertex " << i << ": "
                                    << "link missing adjacent tetrahedron.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        }
                    }
                }
            }
        }

        void vertexLinks() {
            testManualAll(verifyVertexLinks);
            runCensusAllBounded(verifyVertexLinks);
            runCensusAllNoBdry(verifyVertexLinks);
        }

        static void verifyEdgeLinks(const Triangulation<4>& tri,
                const char* name) {
            for (unsigned long i = 0; i < tri.countEdges(); ++i) {
                Edge<4>* e = tri.edge(i);

                const Triangulation<2>& link = e->buildLink();
                Isomorphism<4> iso = e->buildLinkInclusion();

                if (link.countTriangles() != e->degree()) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        << "link has incorrect number of triangles.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! link.isConnected()) {
                    std::ostringstream msg;
                    msg << name << ", edge " << i << ": "
                        << "link of edge is not connected.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (e->isBoundary()) {
                    if (link.isClosed()) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            << "link of boundary edge is closed.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else {
                    if (! link.isClosed()) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            << "link of internal edge is not closed.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                if (e->isValid()) {
                    if (e->isBoundary()) {
                        if (link.eulerChar() != 1) {
                            std::ostringstream msg;
                            msg << name << ", edge " << i << ": "
                                << "link of boundary edge is not a disc.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (link.eulerChar() != 2) {
                            std::ostringstream msg;
                            msg << name << ", edge " << i << ": "
                                << "link of internal edge is not a sphere.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }

                // Make sure the edge link matches what happens on
                // the vertex links.
                unsigned j, k;
                const Pentachoron<4>* p;
                Vertex<4>* v;
                Perm<5> perm;
                for (j = 0; j < 2; ++j) {
                    p = e->front().pentachoron();
                    perm = e->front().vertices();

                    // In the vertex link at the jth end of this edge,
                    // find the vertex that this edge projects down to.
                    v = p->vertex(perm[j]);
                    const Triangulation<3>& vLink = v->buildLink();

                    for (k = 0; k < v->degree(); ++k)
                        if (v->embedding(k).pentachoron() == p &&
                                v->embedding(k).vertex() == perm[j])
                            break;
                    if (k == v->degree()) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            << "misconstructed vertex link.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    Vertex<3>* match = vLink.tetrahedron(k)->vertex(
                        p->tetrahedronMapping(perm[j]).pre(perm[1-j]));

                    if (! e->hasBadIdentification()) {
                        if (! match->buildLink().isIsomorphicTo(link)) {
                            std::ostringstream msg;
                            msg << name << ", edge " << i << ": "
                                << "non-isomorphic 2-D triangulations in "
                                "edge vs vertex links.";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (match->degree() != 2 * e->degree()) {
                            std::ostringstream msg;
                            msg << name << ", edge " << i << ": "
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
                    p = tri.pentachoron(iso.pentImage(j));
                    perm = iso.facetPerm(j);
                    eNum = Edge<4>::edgeNumber[perm[3]][perm[4]];
                    if (p->edge(eNum) != e ||
                            p->edgeMapping(eNum)[0] != perm[3] ||
                            p->edgeMapping(eNum)[1] != perm[4]) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            << "link does not map 3,4 -> edge correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (perm[0] != p->triangleMapping(eNum)[0] ||
                            perm[1] != p->triangleMapping(eNum)[1] ||
                            perm[2] != p->triangleMapping(eNum)[2]) {
                        std::ostringstream msg;
                        msg << name << ", edge " << i << ": "
                            << "link does not map 0,1,2 -> opposite "
                            "triangle correctly.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    for (k = 0; k < 3; ++k) {
                        t = link.triangle(j);
                        adj = t->adjacentTriangle(k);
                        if (adj) {
                            if (! p->adjacentPentachoron(perm[k])) {
                                std::ostringstream msg;
                                msg << name << ", edge " << i << ": "
                                    << "link has extra adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            } else if (p->adjacentPentachoron(perm[k]) !=
                                    tri.pentachoron(iso.pentImage(
                                    adj->index()))) {
                                std::ostringstream msg;
                                msg << name << ", edge " << i << ": "
                                    << "link has wrong adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            } else if ((! e->hasBadIdentification()) &&
                                    p->adjacentGluing(perm[k]) !=
                                    iso.facetPerm(adj->index()) *
                                    Perm<5>::extend(t->adjacentGluing(k)) *
                                    perm.inverse()) {
                                // Note: we expect broken gluings with
                                // reverse self-identifications.
                                std::ostringstream msg;
                                msg << name << ", edge " << i << ": "
                                    << "link has wrong adjacent gluing.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        } else {
                            if (p->adjacentPentachoron(perm[k])) {
                                std::ostringstream msg;
                                msg << name << ", edge " << i << ": "
                                    << "link missing adjacent triangle.";
                                CPPUNIT_FAIL(msg.str());
                            }
                        }
                    }
                }
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

        static void verifyIdealToFinite(const Triangulation<4>& tri,
                const char* name) {
            bool shouldTruncate = false;
            if (tri.isValid() && ! tri.isIdeal()) {
                // Should not truncate any vertices.
            } else {
                for (unsigned i = 0; i < tri.countVertices(); ++i)
                    if (tri.vertex(i)->isIdeal() ||
                            ! tri.vertex(i)->isValid()) {
                        shouldTruncate = true;
                        break;
                    }
            }

            if (! shouldTruncate) {
                // The idealToFinite routine should leave tri unchanged.
                Triangulation<4> other(tri);
                other.idealToFinite();
                if (other != tri) {
                    std::ostringstream msg;
                    msg << name << ": "
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
                Triangulation<4> other(tri);
                if (i > 0)
                    Isomorphism<4>::random(other.size()).applyInPlace(other);

                other.idealToFinite();
                clearProperties(other);

                if (other == tri) {
                    std::ostringstream msg;
                    msg << name << ": "
                        << "idealToFinite does not modify a triangulation with "
                        "truncatable vertices.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (! other.hasBoundaryTetrahedra()) {
                    std::ostringstream msg;
                    msg << name << ": "
                        << "idealToFinite does not produce real boundary.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (other.isIdeal()) {
                    std::ostringstream msg;
                    msg << name << ": "
                        << "idealToFinite does not remove ideal vertices.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (other.isOrientable() != tri.isOrientable()) {
                    std::ostringstream msg;
                    msg << name << ": idealToFinite changes orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (other.isClosed() != tri.isClosed()) {
                    std::ostringstream msg;
                    msg << name << ": idealToFinite changes closedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (other.isConnected() != tri.isConnected()) {
                    std::ostringstream msg;
                    msg << name << ": idealToFinite changes connectedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri.isValid()) {
                    // Subdivision can change invalid to valid, but never
                    // the other way.
                    if (! other.isValid()) {
                        std::ostringstream msg;
                        msg << name << ": idealToFinite destroys validity.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    // Subdivisions can change these properties for
                    // invalid triangulations.
                    if (other.eulerCharManifold() != tri.eulerCharManifold()) {
                        std::ostringstream msg;
                        msg << name << ": "
                            << "idealToFinite changes Euler characteristic.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if (other.countBoundaryComponents() !=
                            tri.countBoundaryComponents()) {
                        std::ostringstream msg;
                        msg << name << ": "
                            << "idealToFinite changes # boundary components.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    // These properties can only be computed for valid
                    // triangulations.
                    other.intelligentSimplify();

                    if (! (other.homology() == tri.homology())) {
                        std::ostringstream msg;
                        msg << name << ": idealToFinite changes H1.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if (! (other.homology<2>() == tri.homology<2>())) {
                        std::ostringstream msg;
                        msg << name << ": idealToFinite changes H2.";
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

        static void verifyIBundle(const Triangulation<3>& tri,
                const char* name) {
            // For now, only work with compact triangulations.
            if ((! tri.isValid()) || tri.isIdeal())
                return;

            Triangulation<4> b = Example<4>::iBundle(tri);

            if (! b.isValid()) {
                std::ostringstream msg;
                msg << name << ": iBundle gives an invalid triangulation.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b.isOrientable() != tri.isOrientable()) {
                std::ostringstream msg;
                msg << name << ": iBundle has mismatched orientability.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b.countComponents() != tri.countComponents()) {
                std::ostringstream msg;
                msg << name << ": iBundle has the wrong number of components.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b.eulerCharTri() != tri.eulerCharTri() ||
                    b.eulerCharManifold() != tri.eulerCharManifold()) {
                std::ostringstream msg;
                msg << name << ": "
                    << "iBundle gives the wrong Euler characteristic.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned long expectBdryComp = 2 * tri.countComponents();
            unsigned long i;
            for (i = 0; i < tri.countComponents(); ++i)
                if (tri.component(i)->countBoundaryTriangles())
                    --expectBdryComp;

            if (b.countBoundaryComponents() != expectBdryComp) {
                std::ostringstream msg;
                msg << name << ": iBundle has the wrong number of "
                        "boundary components.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned long expectBdryTets = 2 * tri.countTetrahedra() +
                20 * tri.countBoundaryTriangles();

            if (b.countBoundaryFacets() != expectBdryTets) {
                std::ostringstream msg;
                msg << name << ": iBundle has the wrong number of "
                        "boundary tetrahedra.";
                CPPUNIT_FAIL(msg.str());
            }

            // TODO: Check isomorphisms from tri onto the boundary of b.

            // Simplify the triangulation before running any more
            // expensive tests.
            b.intelligentSimplify();

            if (b.homology() != tri.homology()) {
                std::ostringstream msg;
                msg << name << ": iBundle gives a mismatched H1.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b.homology<2>() != tri.homology<2>()) {
                std::ostringstream msg;
                msg << name << ": iBundle gives a mismatched H2.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void iBundle() {
            runCensusAllClosed(verifyIBundle);
            runCensusAllBounded(verifyIBundle);
        }

        static void verifyS1Bundle(const Triangulation<3>& tri,
                const char* name) {
            // For now, only work with compact triangulations.
            if ((! tri.isValid()) || tri.isIdeal())
                return;

            Triangulation<4> b = Example<4>::s1Bundle(tri);

            if (! b.isValid()) {
                std::ostringstream msg;
                msg << name << ": s1Bundle gives an invalid triangulation.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b.isOrientable() != tri.isOrientable()) {
                std::ostringstream msg;
                msg << name << ": s1Bundle has mismatched orientability.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b.countComponents() != tri.countComponents()) {
                std::ostringstream msg;
                msg << name << ": s1Bundle has the wrong number of components.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b.eulerCharTri() != 0 || b.eulerCharManifold() != 0) {
                std::ostringstream msg;
                msg << name << ": "
                    << "s1Bundle gives the wrong Euler characteristic.";
                CPPUNIT_FAIL(msg.str());
            }

            if (b.countBoundaryComponents() != tri.countBoundaryComponents()) {
                std::ostringstream msg;
                msg << name << ": s1Bundle has the wrong number of "
                        "boundary components.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned long expectBdryTets =
                20 * tri.countBoundaryTriangles();

            if (b.countBoundaryFacets() != expectBdryTets) {
                std::ostringstream msg;
                msg << name << ": s1Bundle has the wrong number of "
                        "boundary tetrahedra.";
                CPPUNIT_FAIL(msg.str());
            }

            // Simplify the triangulation before running any more
            // expensive tests.
            b.intelligentSimplify();

            regina::AbelianGroup expectH1 = tri.homology();
            expectH1.addRank();

            if (b.homology() != expectH1) {
                std::ostringstream msg;
                msg << name << ": s1Bundle gives incorrect H1.";
                CPPUNIT_FAIL(msg.str());
            }

            regina::AbelianGroup expectH2(tri.homology<2>());
            expectH2.addGroup(tri.homology());

            if (b.homology<2>() != expectH2) {
                std::ostringstream msg;
                msg << name << ": s1Bundle gives incorrect H2.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void s1Bundle() {
            runCensusAllClosed(verifyS1Bundle);
            runCensusAllBounded(verifyS1Bundle);
        }

        static void verifyBundleWithMonodromy(const Triangulation<3>& tri,
                const char* name) {
            // For now, only work with compact triangulations.
            if ((! tri.isValid()) || tri.isIdeal())
                return;

            tri.findAllIsomorphisms(tri, [&tri, name](
                    const Isomorphism<3>& aut) {
                Triangulation<4> b = Example<4>::bundleWithMonodromy(tri, aut);

                if (! b.isValid()) {
                    std::ostringstream msg;
                    msg << name << ": bundleWithMonodromy gives an "
                        "invalid triangulation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (b.isOrientable() && ! tri.isOrientable()) {
                    std::ostringstream msg;
                    msg << name << ": "
                        << "bundleWithMonodromy destroys non-orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (b.countComponents() != tri.countComponents()) {
                    std::ostringstream msg;
                    msg << name << ": bundleWithMonodromy has the wrong number "
                        "of components.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (b.eulerCharTri() != 0 || b.eulerCharManifold() != 0) {
                    std::ostringstream msg;
                    msg << name << ": bundleWithMonodromy gives the wrong "
                        "Euler characteristic.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (b.countBoundaryComponents() !=
                        tri.countBoundaryComponents()) {
                    std::ostringstream msg;
                    msg << name << ": "
                        << "bundleWithMonodromy gives the wrong number of "
                            "boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }

                size_t expectBdryTets = 20 * tri.countBoundaryTriangles();

                if (b.countBoundaryFacets() != expectBdryTets) {
                    std::ostringstream msg;
                    msg << name << ": "
                        << "bundleWithMonodromy gives the wrong number of "
                            "boundary tetrahedra.";
                    CPPUNIT_FAIL(msg.str());
                }

                // TODO: It would be nice to test something about
                // homology here.  Note that, since we are passing a
                // combinatorial isomorphism, b must have (M x S1) as a
                // finite sheeted cover.

                return false;
            });
        }

        void bundleWithMonodromy() {
            runCensusAllClosed(verifyBundleWithMonodromy, false /* small */);
            runCensusAllBounded(verifyBundleWithMonodromy, false /* small */);

            // A case for which using inverse isomorphism gluings will
            // definitely break things, since the gluings are not involutions.
            // This is the two-vertex, two-tetrahedron L(3,1);
            // the degree two vertex is 0(0) == 1(1).
            Triangulation<3> tri;
            auto [t0, t1] = tri.newTetrahedra<2>();
            t0->join(0, t1, regina::Perm<4>(1,3,0,2));
            t0->join(1, t1, regina::Perm<4>(1,2,3,0));
            t0->join(2, t1, regina::Perm<4>(1,2,3,0));
            t0->join(3, t1, regina::Perm<4>(1,2,3,0));
            verifyBundleWithMonodromy(tri, "Hand-coded L(3,1)");
        }

        void verifyRetriangulate(const Triangulation<4>& tri,
                int height, int threads, bool track, size_t count,
                const char* name) {
            size_t tot = 0;
            bool broken = false;

            regina::ProgressTrackerOpen* tracker = nullptr;
            if (track)
                tracker = new regina::ProgressTrackerOpen();

            bool result = tri.retriangulate(height, threads, tracker,
                    [&tot, &broken, &tri](const Triangulation<4>& alt) {
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

        void verifyRetriangulate(const Triangulation<4>& tri,
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
            // The counts that are commented out are too slow, though
            // they can be brought back in again as the retriangulation
            // code gets faster.
            //
            // The expected counts should always be positive, so passing
            // an expected count of 0 will be treated as a request to display
            // the number of triangulations that were actually found.
            //
            verifyRetriangulate(s4_doubleConeS3, 0, 1, "S^4 (double cone)");
            verifyRetriangulate(s4_doubleConeS3, 1, 1, "S^4 (double cone)");
            verifyRetriangulate(s4_doubleConeS3, 2, 15, "S^4 (double cone)");
            // verifyRetriangulate(s4_doubleConeS3, 4, 12316);
            verifyRetriangulate(rp4, 0, 1, "RP^4");
            verifyRetriangulate(rp4, 1, 1, "RP^4");
            verifyRetriangulate(rp4, 2, 53, "RP^4");
            // verifyRetriangulate(rp4, 4, 3854489);
            verifyRetriangulate(ball_layerAndFold, 0, 1,
                "Layered and folded ball");
            verifyRetriangulate(ball_layerAndFold, 1, 1,
                "Layered and folded ball");
            verifyRetriangulate(ball_layerAndFold, 2, 4,
                "Layered and folded ball");
            verifyRetriangulate(ball_layerAndFold, 4, 863,
                "Layered and folded ball");
            verifyRetriangulate(idealCappellShaneson, 0, 1,
                "Ideal Cappell-Shaneson");
            verifyRetriangulate(idealCappellShaneson, 1, 1,
                "Ideal Cappell-Shaneson");
            verifyRetriangulate(idealCappellShaneson, 2, 9,
                "Ideal Cappell-Shaneson");
            verifyRetriangulate(idealCappellShaneson, 4, 1610,
                "Ideal Cappell-Shaneson");
        }
};

void addTriangulation4(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Triangulation4Test::suite());
}

