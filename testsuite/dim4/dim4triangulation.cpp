
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "dim4/dim4exampletriangulation.h"
#include "dim4/dim4triangulation.h"
#include "triangulation/nexampletriangulation.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/dim4/testdim4.h"

using regina::Dim4ExampleTriangulation;
using regina::Dim4Pentachoron;
using regina::Dim4Triangulation;
using regina::NAbelianGroup;
using regina::NExampleTriangulation;
using regina::NGroupPresentation;
using regina::NPerm5;
using regina::NTriangulation;

class Dim4TriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Dim4TriangulationTest);

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(boundaryComponents);
    CPPUNIT_TEST(vertexLinks);
    CPPUNIT_TEST(eulerCharacteristic);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(fundGroup);

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

        // Closed non-orientable:
        Dim4Triangulation rp4;
            /**< Real projective 4-space, built from four pentachora. */

        // Bounded orientable:
        Dim4Triangulation ball_singlePent;
            /**< A single pentachoron with no face gluings. */
        Dim4Triangulation ball_foldedPent;
            /**< A single pentachoron with two facets folded together. */
        Dim4Triangulation ball_singleConeS3;
            /**< A single cone over the 3-sphere. */

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
            copyAndDelete(rp4, Dim4ExampleTriangulation::rp4(), "RP^4");

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
            verifyValid(rp4);
            verifyValid(ball_singlePent);
            verifyValid(ball_foldedPent);
            verifyValid(ball_singleConeS3);
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
            verifyConnected(rp4);
            verifyConnected(ball_singlePent);
            verifyConnected(ball_foldedPent);
            verifyConnected(ball_singleConeS3);
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
            verifyOrientable(rp4, false);
            verifyOrientable(ball_singlePent);
            verifyOrientable(ball_foldedPent);
            verifyOrientable(ball_singleConeS3);
            verifyOrientable(idealPoincareProduct);
            verifyOrientable(mixedPoincareProduct);
            verifyOrientable(idealFigEightProduct);
            verifyOrientable(mixedFigEightProduct);
            verifyOrientable(pillow_twoCycle, false);
            verifyOrientable(pillow_threeCycle);
            verifyOrientable(pillow_fourCycle, false);
        }

        void boundaryComponents() {
            // TODO
        }

        void vertexLinks() {
            // TODO
        }

        void eulerCharacteristic() {
            // TODO
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
            verifyHomologyH1(rp4, "Z_2");
            verifyHomologyH1(ball_singlePent, "0");
            verifyHomologyH1(ball_foldedPent, "0");
            verifyHomologyH1(ball_singleConeS3, "0");
            verifyHomologyH1(idealPoincareProduct, "0");
            verifyHomologyH1(mixedPoincareProduct, "0");
            verifyHomologyH1(idealFigEightProduct, "Z");
            verifyHomologyH1(mixedFigEightProduct, "Z");
        }

        void fundGroup() {
            // TODO
        }

        // TODO: isClosed(), isIdeal(), hasBoundaryTetrahedra(),
        // triangulations of vertex links and real boundary components
};

void addDim4Triangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Dim4TriangulationTest::suite());
}

