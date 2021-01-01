
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
#include "triangulation/dim3.h"
#include "triangulation/example3.h"
#include "triangulation/graph.h"
#include "testsuite/dim3/testtriangulation.h"

using regina::Triangulation;

void testDualGraph3Concepts() {
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
        Triangulation<3> empty;
            /**< An empty triangulation. */
        Triangulation<3> lens13_3;
            /**< A layered lens space.
                 The dual graph is a double-ended chain through vertices
                 0,1,2,3 in order. */
        Triangulation<3> aug;
            /**< A triangulation with no non-trivial symmetries whatsoever.
                 The dual graph has triangles (4,5,6), (4,5,7) and (5,6,8),
                 a double edge (8,9), and a loop at 9. */
        Triangulation<3> weeks;
            /**< The ready-made triangulation of the Weeks manifold. */
        Triangulation<3> ball;
            /**< A standalone tetrahedron. */
        Triangulation<3> disconnected;
            /**< A disjoint union of a 6-tetrahedron loop and a
                 4-tetrahedron chain. */

    public:
        void copyAndDelete(Triangulation<3>& dest, Triangulation<3>* source) {
            dest.insertTriangulation(*source);
            dest.setLabel(source->label());
            delete source;
        }

        void setUp() {
            empty.setLabel("Empty");

            lens13_3.insertLayeredLensSpace(13, 3);
            lens13_3.setLabel("L(13,3)");

            aug.insertAugTriSolidTorus(3, -1, 5, -3, 2, -1);
            aug.setLabel("Aug((3,-1), (5,-3), (2,-1))");

            copyAndDelete(weeks, regina::Example<3>::weeks());

            ball.newTetrahedron();
            ball.setLabel("Ball");

            disconnected.insertLayeredSolidTorus(4, 7);
            disconnected.tetrahedron(disconnected.size() - 1)->unjoin(0);
            disconnected.insertLayeredLoop(6, true);
            disconnected.setLabel("Loop(6) U Chain(4)");
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
            testBandwidth(empty, 0);
            testBandwidth(lens13_3, 1);
            testBandwidth(aug, 3);
            testBandwidth(weeks, 4);
            testBandwidth(ball, 0);
            testBandwidth(disconnected, 5);
        }

        void testBipartite(const Triangulation<3>& t, bool expected) {
            bool result = boost::is_bipartite(t);
            if (result != expected) {
                std::ostringstream msg;
                msg << "Bipartite test for " << t.label()
                    << " returns " << result << ", not " << expected
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void bipartite() {
            testBipartite(empty, true);
            testBipartite(lens13_3, false);
            testBipartite(aug, false);
            testBipartite(weeks, false);
            testBipartite(ball, true);
            testBipartite(disconnected, true);
        }
};

void addDualGraph3(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(DualGraph3Test::suite());
}

