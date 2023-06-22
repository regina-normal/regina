
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "triangulation/dim3.h"
#include "triangulation/example3.h"
#include "triangulation/graph.h"

using regina::Example;
using regina::Triangulation;

void dualGraph3Concepts() {
    // Compile-time test.
    boost::function_requires<boost::GraphConcept<Triangulation<3>>>();
    boost::function_requires<boost::VertexListGraphConcept<Triangulation<3>>>();
    boost::function_requires<boost::EdgeListGraphConcept<Triangulation<3>>>();
    boost::function_requires<boost::AdjacencyGraphConcept<Triangulation<3>>>();
    boost::function_requires<boost::IncidenceGraphConcept<Triangulation<3>>>();
    boost::function_requires<boost::BidirectionalGraphConcept<Triangulation<3>>>();
}

class DualGraph3Test : public testing::Test {
    protected:
        Triangulation<3> empty;

        // Dual graph is a double-ended chain spanning 4 vertices.
        Triangulation<3> lens13_3 { Example<3>::lens(13, 3) };

        // Dual graph has no non-trivial symmetries.
        // It has triangles (4,5,6), (4,5,7) and (5,6,8), a double edge (8,9),
        // and a loop at 9.
        Triangulation<3> aug {
            Example<3>::augTriSolidTorus(3, -1, 5, -3, 2, -1) };

        Triangulation<3> weeks { Example<3>::weeks() };

        Triangulation<3> ball { Triangulation<3>::fromGluings(1, {}) };

        // A disjoint union of a 6-tetrahedron loop and a 4-tetrahedron chain.
        Triangulation<3> disconnected;

    public:
        DualGraph3Test() {
            disconnected.insertLayeredSolidTorus(4, 7);
            disconnected.tetrahedron(disconnected.size() - 1)->unjoin(0);
            disconnected.insertTriangulation(Example<3>::layeredLoop(6, true));
        }
};

TEST_F(DualGraph3Test, bandwidth) {
    EXPECT_EQ(boost::bandwidth(empty), 0);
    EXPECT_EQ(boost::bandwidth(lens13_3), 1);
    EXPECT_EQ(boost::bandwidth(aug), 3);
    EXPECT_EQ(boost::bandwidth(weeks), 4);
    EXPECT_EQ(boost::bandwidth(ball), 0);
    EXPECT_EQ(boost::bandwidth(disconnected), 5);
}

TEST_F(DualGraph3Test, bipartite) {
    EXPECT_TRUE(boost::is_bipartite(empty));
    EXPECT_FALSE(boost::is_bipartite(lens13_3));
    EXPECT_FALSE(boost::is_bipartite(aug));
    EXPECT_FALSE(boost::is_bipartite(weeks));
    EXPECT_TRUE(boost::is_bipartite(ball));
    EXPECT_TRUE(boost::is_bipartite(disconnected));
}
