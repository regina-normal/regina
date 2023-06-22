
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

#include "link/examplelink.h"
#include "link/graph.h"

using regina::ExampleLink;
using regina::Link;

void linkGraphConcepts() {
    // Compile-time test.
    boost::function_requires<boost::GraphConcept<Link>>();
    boost::function_requires<boost::VertexListGraphConcept<Link>>();
    boost::function_requires<boost::EdgeListGraphConcept<Link>>();
    boost::function_requires<boost::AdjacencyGraphConcept<Link>>();
    boost::function_requires<boost::IncidenceGraphConcept<Link>>();
    boost::function_requires<boost::BidirectionalGraphConcept<Link>>();
}

class LinkGraphTest : public testing::Test {
    protected:
        Link empty;

        Link unknot0 { 1 };
        Link unknot1 { Link::fromData({ 1 }, { 1, -1 }) };
        Link unknot3 { Link::fromData({ 1, 1, -1 }, { 1, -2, -3, -1, 2, 3 }) };
        Link unknotGordian { ExampleLink::gordian() };

        Link trefoilLeft { ExampleLink::trefoilLeft() };
        Link trefoilRight { ExampleLink::trefoilRight() };
        Link figureEight { ExampleLink::figureEight() };

        Link unlink2_0 { 2 };
        Link unlink3_0 { 3 };
        Link unlink2_r2 { Link::fromData({ 1, -1 }, { 1, 2 }, { -1, -2 }) };
        Link unlink2_r1r1 { Link::fromData({ -1, 1 }, { 1, -1 }, { -2, 2 }) };

        Link hopf { ExampleLink::hopf() };
        Link whitehead { ExampleLink::whitehead() };
        Link borromean { ExampleLink::borromean() };
        Link trefoil_unknot0 { Link::fromData({ 1, 1, 1 },
            { 1, -2, 3, -1, 2, -3 }, { 0 }) };
        Link trefoil_unknot1 { Link::fromData({ +1, +1, -1, +1 },
            { 1, -2, 4, -1, 2, -4 }, { -3, 3 }) };
        Link trefoil_unknot_overlap { Link::fromData({ -1, +1, +1, +1, +1 },
            { 2, -3, -4, -1, 5, -2, 3, -5 }, { 4, 1 }) };
        Link adams6_28 { Link::fromData({ +1, +1, -1, -1, +1, +1 },
            { -2, 1, -5, 6 }, { 2, -3, 4, -6, 5, -4, 3, -1 }) };
};

TEST_F(LinkGraphTest, bandwidth) {
    EXPECT_EQ(boost::bandwidth(empty), 0);
    EXPECT_EQ(boost::bandwidth(unknot0), 0);
    EXPECT_EQ(boost::bandwidth(unknot1), 0);
    EXPECT_EQ(boost::bandwidth(unknot3), 2);
    EXPECT_EQ(boost::bandwidth(trefoilLeft), 2);
    EXPECT_EQ(boost::bandwidth(trefoilRight), 2);
    EXPECT_EQ(boost::bandwidth(figureEight), 3);
    EXPECT_EQ(boost::bandwidth(unlink2_0), 0);
    EXPECT_EQ(boost::bandwidth(unlink3_0), 0);
    EXPECT_EQ(boost::bandwidth(unlink2_r2), 1);
    EXPECT_EQ(boost::bandwidth(unlink2_r1r1), 0);
    EXPECT_EQ(boost::bandwidth(hopf), 1);
    EXPECT_EQ(boost::bandwidth(whitehead), 4);
    EXPECT_EQ(boost::bandwidth(borromean), 5);
    EXPECT_EQ(boost::bandwidth(trefoil_unknot0), 2);
    EXPECT_EQ(boost::bandwidth(trefoil_unknot1), 3);
    EXPECT_EQ(boost::bandwidth(trefoil_unknot_overlap), 4);
}

TEST_F(LinkGraphTest, bipartite) {
    EXPECT_TRUE(boost::is_bipartite(empty));
    EXPECT_TRUE(boost::is_bipartite(unknot0));
    EXPECT_FALSE(boost::is_bipartite(unknot1));
    EXPECT_FALSE(boost::is_bipartite(unknot3));
    EXPECT_FALSE(boost::is_bipartite(trefoilLeft));
    EXPECT_FALSE(boost::is_bipartite(trefoilRight));
    EXPECT_FALSE(boost::is_bipartite(figureEight));
    EXPECT_TRUE(boost::is_bipartite(unlink2_0));
    EXPECT_TRUE(boost::is_bipartite(unlink3_0));
    EXPECT_TRUE(boost::is_bipartite(unlink2_r2));
    EXPECT_FALSE(boost::is_bipartite(unlink2_r1r1));
    EXPECT_TRUE(boost::is_bipartite(hopf));
    EXPECT_FALSE(boost::is_bipartite(whitehead));
    EXPECT_FALSE(boost::is_bipartite(borromean));
    EXPECT_FALSE(boost::is_bipartite(trefoil_unknot0));
    EXPECT_FALSE(boost::is_bipartite(trefoil_unknot1));
    EXPECT_FALSE(boost::is_bipartite(trefoil_unknot_overlap));
}
