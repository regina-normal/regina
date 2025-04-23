
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

struct TestCase {
    ModelLinkGraph graph;
    const char* name;
};

class ModelLinkGraphTest : public testing::Test {
    protected:
        TestCase empty { {}, "Empty" };
        TestCase hopf { std::string("bbbb,aaaa"), "Hopf link" };
        TestCase trefoil { std::string("bbcc,ccaa,aabb"), "Trefoil" };
        TestCase virtualTrefoil { std::string("b3b2b0b1,a2a3a1a0"),
            "Virtual trefoil" };

        /**
         * Run the given test over all of the example triangulations stored in
         * this test fixture.
         */
        void testManualCases(void (*f)(const ModelLinkGraph&, const char*)) {
            f(empty.graph, empty.name);
            f(hopf.graph, hopf.name);
            f(trefoil.graph, trefoil.name);
        }
};

TEST_F(ModelLinkGraphTest, connected) {
    EXPECT_TRUE(empty.graph.isConnected());
    EXPECT_TRUE(hopf.graph.isConnected());
    EXPECT_TRUE(trefoil.graph.isConnected());
    EXPECT_TRUE(virtualTrefoil.graph.isConnected());
}

TEST_F(ModelLinkGraphTest, components) {
    EXPECT_EQ(empty.graph.countComponents(), 0);
    EXPECT_EQ(hopf.graph.countComponents(), 1);
    EXPECT_EQ(trefoil.graph.countComponents(), 1);
    EXPECT_EQ(virtualTrefoil.graph.countComponents(), 1);
}

TEST_F(ModelLinkGraphTest, genus) {
    EXPECT_EQ(empty.graph.genus(), 0);
    EXPECT_EQ(hopf.graph.genus(), 0);
    EXPECT_EQ(trefoil.graph.genus(), 0);
    EXPECT_EQ(virtualTrefoil.graph.genus(), 1);
}

TEST_F(ModelLinkGraphTest, simple) {
    EXPECT_TRUE(empty.graph.isSimple());
    EXPECT_FALSE(hopf.graph.isSimple());
    EXPECT_FALSE(trefoil.graph.isSimple());
    EXPECT_FALSE(virtualTrefoil.graph.isSimple());
}

// TODO: Isomorphism testing via canonicalPlantri and canonical

static void verifyReflect(const TestCase& test, bool symmetricUnderReflection) {
    SCOPED_TRACE_CSTRING(test.name);

    const ModelLinkGraph& g = test.graph;
    ModelLinkGraph alt = g;
    alt.reflect();

    EXPECT_EQ(g.isConnected(), alt.isConnected());
    EXPECT_EQ(g.countComponents(), alt.countComponents());
    EXPECT_EQ(g.genus(), alt.genus());
    EXPECT_EQ(g.isSimple(), alt.isSimple());

    if (g.countComponents() == 1) {
        if (symmetricUnderReflection) {
            // We don't know if g == alt (i.e., whether they use the same
            // labelling).
            EXPECT_EQ(g.canonical(), alt.canonical());
        } else {
            EXPECT_NE(g, alt);
            EXPECT_NE(g.canonical(), alt.canonical());
        }
    }

    alt.reflect();
    EXPECT_EQ(alt, g);
}

TEST_F(ModelLinkGraphTest, reflect) {
    verifyReflect(empty, true);
    verifyReflect(hopf, true);
    verifyReflect(trefoil, true);
    verifyReflect(virtualTrefoil, true);
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

