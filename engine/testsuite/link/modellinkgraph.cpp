
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "link/examplelink.h"
#include "link/modellinkgraph.h"

#include "testhelper.h"

using regina::ModelLinkGraph;
using regina::ModelLinkGraphArc;
using regina::ModelLinkGraphCells;
using regina::ModelLinkGraphNode;
using regina::ExampleLink;
using regina::Link;

// Consistency checks for low-level manipulation of graphs:
static bool isConsistent(const ModelLinkGraph& graph) {
    for (auto* n : graph.nodes())
        for (int i = 0; i < 4; ++i) {
            ModelLinkGraphArc arc = n->arc(i);
            if (arc.traverse().traverse() != arc)
                return false;
        }
    return true;
}

struct TestCase {
    ModelLinkGraph graph;
    const char* name;
};

class ModelLinkGraphTest : public testing::Test {
    protected:
        // Connected planar graphs:
        TestCase empty { {}, "Empty" };
        TestCase twist { std::string("a1a0a3a2"), "Single twist" };
        TestCase hopf { std::string("bbbb,aaaa"), "Hopf link" };
        TestCase trefoil { std::string("bbcc,ccaa,aabb"), "Trefoil" };
        TestCase borromean { ExampleLink::borromean(), "Borromean rings" };

        // Connected non-planar graphs:
        TestCase virtualTrefoil { std::string("b3b2b0b1,a2a3a1a0"),
            "Virtual trefoil" };
        TestCase genus2 { std::string("b0b1b2d0,a0a1a2c0,b3c2c1d2,a3d3c3d1"),
            "Genus two graph" };

        // Disconnected graphs:
        TestCase disconnected2 {
            std::string("c3c2c0c1,d3d2d1d0,a2a3a1a0,b3b2b1b0"),
            "Virtual trefoil U Hopf link" };
        TestCase disconnected3 { {},
            "Virtual trefoil U genus two graph U Borromean rings" };

        ModelLinkGraphTest() {
            disconnected3.graph.insertGraph(virtualTrefoil.graph);
            disconnected3.graph.insertGraph(genus2.graph);
            disconnected3.graph.insertGraph(borromean.graph);
        }

        /**
         * Run the given test over all of the example triangulations stored in
         * this test fixture.
         */
        void testManualCases(void (*f)(const ModelLinkGraph&, const char*)) {
            f(empty.graph, empty.name);
            f(twist.graph, twist.name);
            f(hopf.graph, hopf.name);
            f(trefoil.graph, trefoil.name);
            f(borromean.graph, borromean.name);
            f(virtualTrefoil.graph, virtualTrefoil.name);
            f(genus2.graph, genus2.name);
            f(disconnected2.graph, disconnected2.name);
            f(disconnected3.graph, disconnected3.name);
        }
};

TEST_F(ModelLinkGraphTest, connected) {
    EXPECT_TRUE(empty.graph.isConnected());
    EXPECT_TRUE(twist.graph.isConnected());
    EXPECT_TRUE(hopf.graph.isConnected());
    EXPECT_TRUE(trefoil.graph.isConnected());
    EXPECT_TRUE(borromean.graph.isConnected());
    EXPECT_TRUE(virtualTrefoil.graph.isConnected());
    EXPECT_TRUE(genus2.graph.isConnected());
    EXPECT_FALSE(disconnected2.graph.isConnected());
    EXPECT_FALSE(disconnected3.graph.isConnected());
}

TEST_F(ModelLinkGraphTest, components) {
    EXPECT_EQ(empty.graph.countComponents(), 0);
    EXPECT_EQ(twist.graph.countComponents(), 1);
    EXPECT_EQ(hopf.graph.countComponents(), 1);
    EXPECT_EQ(trefoil.graph.countComponents(), 1);
    EXPECT_EQ(borromean.graph.countComponents(), 1);
    EXPECT_EQ(virtualTrefoil.graph.countComponents(), 1);
    EXPECT_EQ(genus2.graph.countComponents(), 1);
    EXPECT_EQ(disconnected2.graph.countComponents(), 2);
    EXPECT_EQ(disconnected3.graph.countComponents(), 3);
}

TEST_F(ModelLinkGraphTest, traversals) {
    EXPECT_EQ(empty.graph.countTraversals(), 0);
    EXPECT_EQ(twist.graph.countTraversals(), 1);
    EXPECT_EQ(hopf.graph.countTraversals(), 2);
    EXPECT_EQ(trefoil.graph.countTraversals(), 1);
    EXPECT_EQ(borromean.graph.countTraversals(), 3);
    EXPECT_EQ(virtualTrefoil.graph.countTraversals(), 1);
    EXPECT_EQ(genus2.graph.countTraversals(), 3);
    EXPECT_EQ(disconnected2.graph.countTraversals(), 3);
    EXPECT_EQ(disconnected3.graph.countTraversals(), 7);
}

TEST_F(ModelLinkGraphTest, genus) {
    EXPECT_EQ(empty.graph.genus(), 0);
    EXPECT_EQ(twist.graph.genus(), 0);
    EXPECT_EQ(hopf.graph.genus(), 0);
    EXPECT_EQ(trefoil.graph.genus(), 0);
    EXPECT_EQ(borromean.graph.genus(), 0);
    EXPECT_EQ(virtualTrefoil.graph.genus(), 1);
    EXPECT_EQ(genus2.graph.genus(), 2);
    EXPECT_EQ(disconnected2.graph.genus(), 1);
    EXPECT_EQ(disconnected3.graph.genus(), 3);
}

TEST_F(ModelLinkGraphTest, simple) {
    EXPECT_TRUE(empty.graph.isSimple());
    EXPECT_FALSE(twist.graph.isSimple());
    EXPECT_FALSE(hopf.graph.isSimple());
    EXPECT_FALSE(trefoil.graph.isSimple());
    EXPECT_TRUE(borromean.graph.isSimple());
    EXPECT_FALSE(virtualTrefoil.graph.isSimple());
    EXPECT_FALSE(genus2.graph.isSimple());
    EXPECT_FALSE(disconnected2.graph.isSimple());
    EXPECT_FALSE(disconnected3.graph.isSimple());
}

static void verifyReflect(const TestCase& test, bool symmetricUnderReflection) {
    SCOPED_TRACE_CSTRING(test.name);

    const ModelLinkGraph& g = test.graph;
    ModelLinkGraph alt = g;
    alt.reflect();

    EXPECT_TRUE(isConsistent(alt));
    EXPECT_EQ(g.isConnected(), alt.isConnected());
    EXPECT_EQ(g.countComponents(), alt.countComponents());
    EXPECT_EQ(g.genus(), alt.genus());
    EXPECT_EQ(g.isSimple(), alt.isSimple());

    if (g.countComponents() <= 1) {
        if (symmetricUnderReflection) {
            // We don't know if g == alt (i.e., whether they use the same
            // labelling).
            EXPECT_EQ(g.canonical(true), alt.canonical(true));
            EXPECT_EQ(g.canonical(false), alt.canonical(false));
        } else {
            EXPECT_NE(g, alt);
            EXPECT_EQ(g.canonical(true), alt.canonical(true));
            EXPECT_NE(g.canonical(false), alt.canonical(false));
        }
    }

    alt.reflect();
    EXPECT_EQ(alt, g);
}

TEST_F(ModelLinkGraphTest, reflect) {
    verifyReflect(empty, true);
    verifyReflect(twist, true);
    verifyReflect(hopf, true);
    verifyReflect(trefoil, true);
    verifyReflect(borromean, true);
    verifyReflect(virtualTrefoil, true);
    verifyReflect(genus2, false);
    verifyReflect(disconnected2, true);
    verifyReflect(disconnected3, true);
}

static void verifyRandomise(const ModelLinkGraph& g, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (int i = 0; i < 20; ++i) {
        ModelLinkGraph alt = g;
        alt.randomise();

        EXPECT_TRUE(isConsistent(alt));
    }

    if (g.countComponents() <= 1) {
        ModelLinkGraph c0 = g.canonical(true);
        ModelLinkGraph c1 = g.canonical(false);
        EXPECT_TRUE(isConsistent(c0));
        EXPECT_TRUE(isConsistent(c1));

        for (int i = 0; i < 20; ++i) {
            ModelLinkGraph alt = g;
            alt.randomise();

            EXPECT_EQ(alt.canonical(true), c0);
            EXPECT_EQ(alt.canonical(false), c1);
        }
    }
}

TEST_F(ModelLinkGraphTest, randomise) {
    testManualCases(verifyRandomise);
}

static void verifyExtendedPlantri(const ModelLinkGraph& g, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    if (g.size() == 0) {
        EXPECT_THROW({ g.extendedPlantri(); }, regina::FailedPrecondition);
        EXPECT_THROW({ ModelLinkGraph::fromExtendedPlantri(""); },
            regina::InvalidArgument);
    } else {
        std::string code = g.extendedPlantri();

        ModelLinkGraph recon;
        ASSERT_NO_THROW({ recon = ModelLinkGraph::fromExtendedPlantri(code); });

        EXPECT_EQ(recon, g);
        EXPECT_EQ(recon.extendedPlantri(), g);

        // Verify the "magic" string constructor.
        EXPECT_NO_THROW({ EXPECT_EQ(ModelLinkGraph(code), recon); });
    }
}

TEST_F(ModelLinkGraphTest, extendedPlantri) {
    testManualCases(verifyExtendedPlantri);
}

static void verifyInvalidCode(const char* code) {
    SCOPED_TRACE_CSTRING(code);

    EXPECT_THROW({ ModelLinkGraph::fromPlantri(code); },
        regina::InvalidArgument);
    EXPECT_THROW({ ModelLinkGraph::fromExtendedPlantri(code); },
        regina::InvalidArgument);

    // Finally, the "magic" constructor:
    EXPECT_THROW({ ModelLinkGraph g(code); }, regina::InvalidArgument);
}

TEST_F(ModelLinkGraphTest, invalidCode) {
    verifyInvalidCode("INVALID");
    verifyInvalidCode("");
}

TEST_F(ModelLinkGraphTest, swapping) {
    ModelLinkGraph a = ExampleLink::trefoilLeft().graph();
    ModelLinkGraph b = ExampleLink::figureEight().graph();
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(b.size(), 4);

    swap(a, b);
    EXPECT_EQ(a.size(), 4);
    EXPECT_EQ(b.size(), 3);

    std::iter_swap(&a, &b);
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(b.size(), 4);
}

static bool looksIdentical(const ModelLinkGraph& a, const ModelLinkGraph& b) {
    if (a.size() != b.size())
        return false;
    if (a.str() != b.str())
        return false;
    return true;
}

void verifyCopyMove(const ModelLinkGraph& graph, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    if (graph.size() == 0) {
        ModelLinkGraph copy(graph);
        EXPECT_EQ(copy.size(), 0);
        EXPECT_TRUE(looksIdentical(copy, graph));

        ModelLinkGraph move(std::move(copy));
        EXPECT_EQ(move.size(), 0);
        EXPECT_TRUE(looksIdentical(move, graph));

        ModelLinkGraph copyAss("bbbb,aaaa");
        copyAss = graph;
        EXPECT_EQ(copyAss.size(), 0);
        EXPECT_TRUE(looksIdentical(copyAss, graph));

        ModelLinkGraph moveAss("bbbb,aaaa");
        moveAss = std::move(copyAss);
        EXPECT_EQ(moveAss.size(), 0);
        EXPECT_TRUE(looksIdentical(moveAss, graph));
    } else {
        ModelLinkGraphNode* n0 = graph.node(0);

        ModelLinkGraph copy(graph);
        EXPECT_TRUE(looksIdentical(copy, graph));

        // Copy construction should use different nodes.
        ASSERT_GT(copy.size(), 0);
        ModelLinkGraphNode* n1 = copy.node(0);
        EXPECT_NE(n1, n0);

        ModelLinkGraph move(std::move(copy));
        EXPECT_TRUE(looksIdentical(move, graph));

        // Move construction should use the same nodes.
        ASSERT_GT(move.size(), 0);
        ModelLinkGraphNode* n2 = move.node(0);
        EXPECT_EQ(n2, n1);

        ModelLinkGraph copyAss("bbbb,aaaa");
        copyAss = graph;
        EXPECT_TRUE(looksIdentical(copyAss, graph));

        // Copy assignment should use different nodes.
        ASSERT_GT(copyAss.size(), 0);
        ModelLinkGraphNode* n3 = copyAss.node(0);
        EXPECT_NE(n3, n0);

        ModelLinkGraph moveAss("bbbb,aaaa");
        moveAss = std::move(copyAss);
        EXPECT_TRUE(looksIdentical(moveAss, graph));

        // Move assignment should use the same nodes.
        ASSERT_GT(moveAss.size(), 0);
        ModelLinkGraphNode* n4 = moveAss.node(0);
        EXPECT_EQ(n4, n3);
    }
}

TEST_F(ModelLinkGraphTest, copyMove) {
    testManualCases(verifyCopyMove);
}

