
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

// Begin with the BGL includes, so we ensure that the compiler correctly
// handles regina's boost::graph_traits specialisations *after* the
// headers in which these graph_traits classes are used.
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/bandwidth.hpp>
#include <boost/graph/bipartite.hpp>

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "link/examplelink.h"
#include "link/graph.h"
#include "testsuite/link/testlink.h"

using regina::ExampleLink;
using regina::Link;

void testLinkGraphConcepts() {
    // Compile-time test.
    boost::function_requires<boost::GraphConcept<regina::Link>>();
    boost::function_requires<boost::VertexListGraphConcept<regina::Link>>();
    boost::function_requires<boost::EdgeListGraphConcept<regina::Link>>();
    boost::function_requires<boost::AdjacencyGraphConcept<regina::Link>>();
    boost::function_requires<boost::IncidenceGraphConcept<regina::Link>>();
    boost::function_requires<boost::BidirectionalGraphConcept<regina::Link>>();
}

class LinkGraphTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LinkGraphTest);

    CPPUNIT_TEST(bandwidth);
    CPPUNIT_TEST(bipartite);

    CPPUNIT_TEST_SUITE_END();

    private:
        /**
         * The empty link:
         */
        Link *empty;

        /**
         * Knots:
         */
        Link *unknot0, *unknot1, *unknot3, *unknotGordian;
        Link *trefoilLeft, *trefoilRight, *figureEight;

        /**
         * Links:
         */
        Link *unlink2_0, *unlink3_0;
        Link *unlink2_r2, *unlink2_r1r1;
        Link *hopf, *whitehead, *borromean;
        Link *trefoil_unknot0, *trefoil_unknot1, *trefoil_unknot_overlap;
        Link *adams6_28; // Figure 6.28 from Adams

    public:
        void setUp() {
            empty = new Link();
            empty->setLabel("Empty link");

            unknot0 = new Link(1);
            unknot0->setLabel("Unknot (0 crossings)");

            unknot1 = Link::fromData({ 1 }, { 1, -1 });
            unknot1->setLabel("Unknot (1 crossing)");

            unknot3 = Link::fromData({ 1, 1, -1 }, { 1, -2, -3, -1, 2, 3 });
            unknot3->setLabel("Unknot (3 crossings)");

            unknotGordian = ExampleLink::gordian();

            trefoilLeft = ExampleLink::trefoilLeft();

            trefoilRight = ExampleLink::trefoilRight();

            figureEight = ExampleLink::figureEight();

            unlink2_0 = new Link(2);
            unlink2_0->setLabel("Unlink (2 components)");

            unlink3_0 = new Link(3);
            unlink3_0->setLabel("Unlink (3 components)");

            unlink2_r2 = Link::fromData({ 1, -1 }, { 1, 2 }, { -1, -2 });
            unlink2_r2->setLabel("Unlink (2 components via R2)");

            unlink2_r1r1 = Link::fromData({ -1, 1 }, { 1, -1 }, { -2, 2 });
            unlink2_r1r1->setLabel("Unlink (2 components via R1+R1)");

            hopf = ExampleLink::hopf();

            whitehead = ExampleLink::whitehead();

            borromean = ExampleLink::borromean();

            trefoil_unknot0 = Link::fromData({ 1, 1, 1 },
                { 1, -2, 3, -1, 2, -3 }, { 0 });
            trefoil_unknot0->setLabel("Trefoil U unknot (separate)");

            trefoil_unknot1 = Link::fromData(
                { +1, +1, -1, +1 }, { 1, -2, 4, -1, 2, -4 }, { -3, 3 });
            trefoil_unknot1->setLabel("Trefoil U unknot (separate + twist)");

            trefoil_unknot_overlap = Link::fromData(
                { -1, +1, +1, +1, +1 }, { 2, -3, -4, -1, 5, -2, 3, -5 },
                { 4, 1 });
            trefoil_unknot_overlap->setLabel("Trefoil U unknot (with R2)");

            adams6_28 = Link::fromData({ +1, +1, -1, -1, +1, +1 },
                { -2, 1, -5, 6 }, { 2, -3, 4, -6, 5, -4, 3, -1 });
            adams6_28->setLabel("Adams, Figure 6.28");
        }

        void tearDown() {
            delete empty;
            delete unknot0;
            delete unknot1;
            delete unknot3;
            delete unknotGordian;
            delete trefoilLeft;
            delete trefoilRight;
            delete figureEight;
            delete unlink2_0;
            delete unlink3_0;
            delete unlink2_r2;
            delete unlink2_r1r1;
            delete hopf;
            delete whitehead;
            delete borromean;
            delete trefoil_unknot0;
            delete trefoil_unknot1;
            delete trefoil_unknot_overlap;
            delete adams6_28;
        }

        void testBandwidth(Link* l, long expected) {
            long result = boost::bandwidth(*l);
            if (result != expected) {
                std::ostringstream msg;
                msg << "Bandwidth of " << l->label()
                    << " returns " << result << ", not " << expected
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void bandwidth() {
            testBandwidth(empty, 0);
            testBandwidth(unknot0, 0);
            testBandwidth(unknot1, 0);
            testBandwidth(unknot3, 2);
            testBandwidth(trefoilLeft, 2);
            testBandwidth(trefoilRight, 2);
            testBandwidth(figureEight, 3);
            testBandwidth(unlink2_0, 0);
            testBandwidth(unlink3_0, 0);
            testBandwidth(unlink2_r2, 1);
            testBandwidth(unlink2_r1r1, 0);
            testBandwidth(hopf, 1);
            testBandwidth(whitehead, 4);
            testBandwidth(borromean, 5);
            testBandwidth(trefoil_unknot0, 2);
            testBandwidth(trefoil_unknot1, 3);
            testBandwidth(trefoil_unknot_overlap, 4);
        }

        void testBipartite(Link* l, bool expected) {
            bool result = boost::is_bipartite(*l);
            if (result != expected) {
                std::ostringstream msg;
                msg << "Bipartite test for " << l->label()
                    << " returns " << result << ", not " << expected
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void bipartite() {
            testBipartite(empty, true);
            testBipartite(unknot0, true);
            testBipartite(unknot1, false);
            testBipartite(unknot3, false);
            testBipartite(trefoilLeft, false);
            testBipartite(trefoilRight, false);
            testBipartite(figureEight, false);
            testBipartite(unlink2_0, true);
            testBipartite(unlink3_0, true);
            testBipartite(unlink2_r2, true);
            testBipartite(unlink2_r1r1, false);
            testBipartite(hopf, true);
            testBipartite(whitehead, false);
            testBipartite(borromean, false);
            testBipartite(trefoil_unknot0, false);
            testBipartite(trefoil_unknot1, false);
            testBipartite(trefoil_unknot_overlap, false);
        }
};

void addLinkGraph(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(LinkGraphTest::suite());
}

