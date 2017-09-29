
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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
#include "triangulation/dim3.h"
#include "triangulation/graph.h"
#include "testsuite/dim3/testtriangulation.h"

using regina::Triangulation;

void testConcepts() {
    // Compile-time test.
    boost::function_requires<boost::GraphConcept<regina::Triangulation<3>>>();
    boost::function_requires<boost::VertexListGraphConcept<regina::Triangulation<3>>>();
    boost::function_requires<boost::EdgeListGraphConcept<regina::Triangulation<3>>>();
    boost::function_requires<boost::AdjacencyGraphConcept<regina::Triangulation<3>>>();
    boost::function_requires<boost::IncidenceGraphConcept<regina::Triangulation<3>>>();
    boost::function_requires<boost::BidirectionalGraphConcept<regina::Triangulation<3>>>();
}

class DualGraph3Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(DualGraph3Test);

    CPPUNIT_TEST(bandwidth);
    CPPUNIT_TEST(bipartite);

    CPPUNIT_TEST_SUITE_END();

    private:
        Triangulation<3> lens13_3;
            /**< A layered lens space.
                 The dual graph is a double-ended chain through vertices
                 0,1,2,3 in order. */
        Triangulation<3> aug;
            /**< A triangulation with no non-trivial symmetries whatsoever.
                 The dual graph has triangles (4,5,6), (4,5,7) and (5,6,8),
                 a double edge (8,9), and a loop at 9. */
        Triangulation<3> ball;
            /**< A standalone tetrahedron. */

    public:
        void setUp() {
            lens13_3.insertLayeredLensSpace(13, 3);
            lens13_3.setLabel("L(13,3)");

            aug.insertAugTriSolidTorus(3, -1, 5, -3, 2, -1);
            aug.setLabel("Aug((3,-1), (5,-3), (2,-1))");

            ball.newTetrahedron();
            ball.setLabel("Ball");
        }

        void tearDown() {
        }

        void testBandwidth(const Triangulation<3>& t, long expected) {
            long result = boost::bandwidth(t);
            if (result != expected) {
                std::ostringstream msg;
                msg << "Bandwidth of " << t.label()
                    << " returns " << result << ", not " << expected
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void bandwidth() {
            testBandwidth(lens13_3, 1);
            testBandwidth(aug, 3);
            testBandwidth(ball, 0);
        }

        void testBipartite(const Triangulation<3>& t, bool expected) {
            bool result = boost::is_bipartite(t,
                regina::graph::InherentPropertyMap<3, boost::vertex_index_t>());
            if (result != expected) {
                std::ostringstream msg;
                msg << "Bipartite test for " << t.label()
                    << " returns " << result << ", not " << expected
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void bipartite() {
            testBipartite(lens13_3, false);
            testBipartite(aug, false);
            testBipartite(ball, true);
        }
};

void addDualGraph3(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(DualGraph3Test::suite());
}

