
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include "census/census.h"
#include "link/examplelink.h"
#include "link/link.h"
#include "surface/normalsurfaces.h"

#include "utilities/tightencodingtest.h"

using regina::Algorithm;
using regina::Crossing;
using regina::ExampleLink;
using regina::Link;
using regina::Triangulation;
using regina::StrandRef;

// A link size beneath which we declare it trivial to compute Jones polynomials:
static constexpr int JONES_THRESHOLD = 20;

// Recognition of specific link complements:
static bool isFigureEightComplement(const Triangulation<3>& tri) {
    // True means yes, this is the figure eight knot complement.
    // False means we don't have a definitive answer.
    return tri.isoSig() == "cPcbbbiht";
}

static bool isTrefoilComplement(const Triangulation<3>& tri) {
    // True means yes, this is the trefoil complement.
    // False means we don't have a definitive answer.
    std::string sig = tri.isoSig();

    // Regina's simplification heuristics have been found to produce these
    // trefoil complements in practice:
    for (const char* s : {
            "cPcbbbadh", "cPcbbbadu",
            "dLQbcbcdlcj", "dLQbcbcdlcn", "dLQabccbrwj", "dLQabccbrwn",
            "eLAkbbcddainqv", "eLAkbcbddducqn", "eLAkbcbdddmcxj" })
        if (sig == s)
            return true;

    return false;
}

static bool isCensusManifold(const Triangulation<3>& tri,
        const std::string& name) {
    std::string sig = tri.isoSig();
    std::string altName = name + " : ";

    auto hits = regina::Census::lookup(sig);
    for (const auto& hit : hits)
        if (hit.name() == name ||
                hit.name().substr(0, altName.size()) == altName)
            return true;

    return false;
}

// Consistency checks for low-level manipulation of links:
static bool isConsistent(const Link& link) {
    for (Crossing* c : link.crossings()) {
        StrandRef lower(c, 0);
        StrandRef upper(c, 1);
        if (lower.next().prev() != lower || upper.next().prev() != upper)
            return false;
    }
    return true;
}

struct TestCase {
    Link link;
    const char* name;
};

class LinkTest : public testing::Test {
    protected:
        // Trivial case:
        TestCase empty { {}, "Empty" };

        // Unknots:
        TestCase unknot0 { { 1 }, "Unknot (0 crossings)" };
        TestCase unknot1 {
            Link::fromData({ 1 }, { 1, -1 }),
            "Unknot (1 crossing)" };
        TestCase unknot3 {
            Link::fromData({ 1, 1, -1 }, { 1, -2, -3, -1, 2, 3 }),
            "Unknot (3 crossings)" };
        TestCase unknotMonster { ExampleLink::monster(), "Unknot (monster)" };
        TestCase unknotGordian { ExampleLink::gordian(), "Unknot (Gordian)" };

        // Non-trivial single-component knots:
        TestCase trefoilLeft { ExampleLink::trefoilLeft(), "Trefoil (left)" };
        TestCase trefoilRight { ExampleLink::trefoil(), "Trefoil (right)" };
        TestCase trefoil_r1x2 {
            Link::fromData({ 1, 1, 1, -1, 1 },
                { 1, 4, -4, -2, 3, -1, 5, -5, 2, -3 }),
            "Trefoil (R1x2)" };
        TestCase trefoil_r1x6 {
            Link::fromData({ 1, 1, 1, -1, -1, 1, 1, 1, -1 },
                { 1, 4, -4, -2, -7, 7, 3, -8, 8, -1, -5, 5, 2, 6, -6, -3,
                  9, -9 }),
            "Trefoil (R1x6)" };
        TestCase figureEight { ExampleLink::figureEight(), "Figure eight" };
        TestCase figureEight_r1x2 {
            Link::fromData({ 1, 1, -1, -1, -1, 1 },
                { -1, 2, -3, 4, 5, -5, -2, 1, -4, 6, -6, 3 }),
            "Figure eight (R1x2)" };
        TestCase conway { ExampleLink::conway(), "Conway" };
        TestCase kinoshitaTerasaka {
            ExampleLink::kinoshitaTerasaka(),
            "Kinoshita-Terasaka" };
        TestCase gst { ExampleLink::gst(), "Gompf-Scharlemann-Thompson" };

        // Composite knots:
        TestCase rht_rht {
            Link::fromData({ 1, 1, 1, 1, 1, 1 },
                { 1, -2, 3, -1, 2, -3, 4, -5, 6, -4, 5, -6 }),
            "RH Trefoil # RH Trefoil" };
        TestCase rht_lht {
            Link::fromData({ 1, 1, 1, -1, -1, -1 },
                { 1, -2, 3, -1, 2, -3, 4, -5, 6, -4, 5, -6 }),
            "RH Trefoil # LH Trefoil" };

        // Unlinks:
        TestCase unlink2_0 { { 2 }, "Unlink (2 components)" };
        TestCase unlink3_0 { { 3 }, "Unlink (3 components)" };
        TestCase unlink2_r2 {
            Link::fromData({ 1, -1 }, { 1, 2 }, { -1, -2 }),
            "Unlink (2 components via R2)" };
        TestCase unlink2_r1r1 {
            Link::fromData({ -1, 1 }, { 1, -1 }, { -2, 2 }),
            "Unlink (2 components via R1+R1)" };

        // Non-trivial multiple-component links:
        TestCase hopf { ExampleLink::hopf(), "Hopf" };
        TestCase whitehead { ExampleLink::whitehead(), "Whitehead" };
        TestCase borromean { ExampleLink::borromean(), "Borromean rings" };
        TestCase trefoil_unknot0 {
            Link::fromData({ 1, 1, 1 }, { 1, -2, 3, -1, 2, -3 }, { 0 }),
            "Trefoil U unknot (separate)" };
        TestCase trefoil_unknot1 {
            Link::fromData({ +1, +1, -1, +1 }, { 1, -2, 4, -1, 2, -4 },
                { -3, 3 }),
            "Trefoil U unknot (separate + twist)" };
        TestCase trefoil_unknot_overlap {
            Link::fromData({ -1, +1, +1, +1, +1 },
                { 2, -3, -4, -1, 5, -2, 3, -5 }, { 4, 1 }),
            "Trefoil U unknot (with R2)" };
        TestCase adams6_28 {
            Link::fromData({ +1, +1, -1, -1, +1, +1 },
                { -2, 1, -5, 6 }, { 2, -3, 4, -6, 5, -4, 3, -1 }),
            "Adams Fig. 6.28" };

        /**
         * Run the given test over all of the example triangulations stored in
         * this test fixture.
         */
        void testManualCases(void (*f)(const Link&, const char*),
                bool includeGordian = true) {
            f(empty.link, empty.name);

            f(unknot0.link, unknot0.name);
            f(unknot1.link, unknot1.name);
            f(unknot3.link, unknot3.name);
            f(unknotMonster.link, unknotMonster.name);
            if (includeGordian)
                f(unknotGordian.link, unknotGordian.name);

            f(trefoilLeft.link, trefoilLeft.name);
            f(trefoilRight.link, trefoilRight.name);
            f(trefoil_r1x2.link, trefoil_r1x2.name);
            f(trefoil_r1x6.link, trefoil_r1x6.name);
            f(figureEight.link, figureEight.name);
            f(figureEight_r1x2.link, figureEight_r1x2.name);
            f(conway.link, conway.name);
            f(kinoshitaTerasaka.link, kinoshitaTerasaka.name);
            f(gst.link, gst.name);

            f(rht_rht.link, rht_rht.name);
            f(rht_lht.link, rht_lht.name);

            f(unlink2_0.link, unlink2_0.name);
            f(unlink3_0.link, unlink3_0.name);
            f(unlink2_r2.link, unlink2_r2.name);
            f(unlink2_r1r1.link, unlink2_r1r1.name);

            f(hopf.link, hopf.name);
            f(whitehead.link, whitehead.name);
            f(borromean.link, borromean.name);
            f(trefoil_unknot0.link, trefoil_unknot0.name);
            f(trefoil_unknot1.link, trefoil_unknot1.name);
            f(trefoil_unknot_overlap.link, trefoil_unknot_overlap.name);
            f(adams6_28.link, adams6_28.name);
        }
};

TEST_F(LinkTest, connected) {
    // Test connectivity of both the links and their model graphs.

    // The following links should give the same result for both the links and
    // their underlying graphs: either there are no zero-crossing components,
    // or there are but these do not change the result.
    EXPECT_TRUE(empty.link.isConnected());
    EXPECT_TRUE(empty.link.graph().isConnected());

    EXPECT_TRUE(unknot0.link.isConnected());
    EXPECT_TRUE(unknot0.link.graph().isConnected());
    EXPECT_TRUE(unknot1.link.isConnected());
    EXPECT_TRUE(unknot1.link.graph().isConnected());
    EXPECT_TRUE(unknot3.link.isConnected());
    EXPECT_TRUE(unknot3.link.graph().isConnected());
    EXPECT_TRUE(unknotMonster.link.isConnected());
    EXPECT_TRUE(unknotMonster.link.graph().isConnected());
    EXPECT_TRUE(unknotGordian.link.isConnected());
    EXPECT_TRUE(unknotGordian.link.graph().isConnected());

    EXPECT_TRUE(trefoilLeft.link.isConnected());
    EXPECT_TRUE(trefoilLeft.link.graph().isConnected());
    EXPECT_TRUE(trefoilRight.link.isConnected());
    EXPECT_TRUE(trefoilRight.link.graph().isConnected());
    EXPECT_TRUE(trefoil_r1x2.link.isConnected());
    EXPECT_TRUE(trefoil_r1x2.link.graph().isConnected());
    EXPECT_TRUE(trefoil_r1x6.link.isConnected());
    EXPECT_TRUE(trefoil_r1x6.link.graph().isConnected());
    EXPECT_TRUE(figureEight.link.isConnected());
    EXPECT_TRUE(figureEight.link.graph().isConnected());
    EXPECT_TRUE(figureEight_r1x2.link.isConnected());
    EXPECT_TRUE(figureEight_r1x2.link.graph().isConnected());
    EXPECT_TRUE(conway.link.isConnected());
    EXPECT_TRUE(conway.link.graph().isConnected());
    EXPECT_TRUE(kinoshitaTerasaka.link.isConnected());
    EXPECT_TRUE(kinoshitaTerasaka.link.graph().isConnected());
    EXPECT_TRUE(gst.link.isConnected());
    EXPECT_TRUE(gst.link.graph().isConnected());

    EXPECT_TRUE(rht_rht.link.isConnected());
    EXPECT_TRUE(rht_rht.link.graph().isConnected());
    EXPECT_TRUE(rht_lht.link.isConnected());
    EXPECT_TRUE(rht_lht.link.graph().isConnected());

    EXPECT_TRUE(unlink2_r2.link.isConnected());
    EXPECT_TRUE(unlink2_r2.link.graph().isConnected());
    EXPECT_FALSE(unlink2_r1r1.link.isConnected());
    EXPECT_FALSE(unlink2_r1r1.link.graph().isConnected());
    EXPECT_TRUE(hopf.link.isConnected());
    EXPECT_TRUE(hopf.link.graph().isConnected());
    EXPECT_TRUE(whitehead.link.isConnected());
    EXPECT_TRUE(whitehead.link.graph().isConnected());
    EXPECT_TRUE(borromean.link.isConnected());
    EXPECT_TRUE(borromean.link.graph().isConnected());
    EXPECT_FALSE(trefoil_unknot1.link.isConnected());
    EXPECT_FALSE(trefoil_unknot1.link.graph().isConnected());
    EXPECT_TRUE(trefoil_unknot_overlap.link.isConnected());
    EXPECT_TRUE(trefoil_unknot_overlap.link.graph().isConnected());

    // These links are disconnected, but since their graphs ignore
    // zero-crossing components the graphs _are_ connected.
    EXPECT_FALSE(unlink2_0.link.isConnected());
    EXPECT_TRUE(unlink2_0.link.graph().isConnected());
    EXPECT_FALSE(unlink3_0.link.isConnected());
    EXPECT_TRUE(unlink3_0.link.graph().isConnected());
    EXPECT_FALSE(trefoil_unknot0.link.isConnected());
    EXPECT_TRUE(trefoil_unknot0.link.graph().isConnected());
}

TEST_F(LinkTest, components) {
    EXPECT_EQ(empty.link.countComponents(), 0);

    EXPECT_EQ(unknot0.link.countComponents(), 1);
    EXPECT_EQ(unknot1.link.countComponents(), 1);
    EXPECT_EQ(unknot3.link.countComponents(), 1);
    EXPECT_EQ(unknotMonster.link.countComponents(), 1);
    EXPECT_EQ(unknotGordian.link.countComponents(), 1);

    EXPECT_EQ(trefoilLeft.link.countComponents(), 1);
    EXPECT_EQ(trefoilRight.link.countComponents(), 1);
    EXPECT_EQ(trefoil_r1x2.link.countComponents(), 1);
    EXPECT_EQ(trefoil_r1x6.link.countComponents(), 1);
    EXPECT_EQ(figureEight.link.countComponents(), 1);
    EXPECT_EQ(figureEight_r1x2.link.countComponents(), 1);
    EXPECT_EQ(conway.link.countComponents(), 1);
    EXPECT_EQ(kinoshitaTerasaka.link.countComponents(), 1);
    EXPECT_EQ(gst.link.countComponents(), 1);

    EXPECT_EQ(rht_rht.link.countComponents(), 1);
    EXPECT_EQ(rht_lht.link.countComponents(), 1);

    EXPECT_EQ(unlink2_0.link.countComponents(), 2);
    EXPECT_EQ(unlink3_0.link.countComponents(), 3);
    EXPECT_EQ(unlink2_r2.link.countComponents(), 2);
    EXPECT_EQ(unlink2_r1r1.link.countComponents(), 2);
    EXPECT_EQ(hopf.link.countComponents(), 2);
    EXPECT_EQ(whitehead.link.countComponents(), 2);
    EXPECT_EQ(borromean.link.countComponents(), 3);
    EXPECT_EQ(trefoil_unknot0.link.countComponents(), 2);
    EXPECT_EQ(trefoil_unknot1.link.countComponents(), 2);
    EXPECT_EQ(trefoil_unknot_overlap.link.countComponents(), 2);
}

static void verifyDiagramComponents(const Link& link, const char* name,
        std::initializer_list<std::string> expectBrief) {
    SCOPED_TRACE_CSTRING(name);

    auto foundComponents = link.diagramComponents();
    EXPECT_EQ(foundComponents.size(), expectBrief.size());

    auto found = foundComponents.begin();
    auto expect = expectBrief.begin();
    for ( ; found != foundComponents.end() && expect != expectBrief.end();
            ++found, ++expect)
        EXPECT_EQ(found->brief(), *expect);
}

TEST_F(LinkTest, diagramComponents) {
    // Just test a few things manually.
    {
        verifyDiagramComponents(empty.link, empty.name, {});
    }
    {
        verifyDiagramComponents(unknot0.link, unknot0.name, { "( )" });
    }
    {
        verifyDiagramComponents(unlink2_0.link, unlink2_0.name,
            { "( )", "( )" });
    }
    {
        verifyDiagramComponents(figureEight.link, figureEight.name,
            { "++-- ( _0 ^1 _2 ^3 _1 ^0 _3 ^2 )" });
    }
    {
        verifyDiagramComponents(whitehead.link, whitehead.name,
            { "--++- ( ^0 _1 ^4 _3 ^2 _4 ) ( _0 ^1 _2 ^3 )" });
    }
    {
        verifyDiagramComponents(trefoil_unknot0.link, trefoil_unknot0.name,
            { "+++ ( ^0 _1 ^2 _0 ^1 _2 )", "( )" });
    }
    {
        verifyDiagramComponents(trefoil_unknot1.link, trefoil_unknot1.name,
            { "+++ ( ^0 _1 ^2 _0 ^1 _2 )", "- ( _0 ^0 )" });
    }
    {
        verifyDiagramComponents(trefoil_unknot_overlap.link,
            trefoil_unknot_overlap.name,
            { "-++++ ( ^1 _2 _3 _0 ^4 _1 ^2 _4 ) ( ^3 ^0 )" });
    }
    {
        Link link = ExampleLink::whitehead();
        link.insertLink(Link(2));
        link.insertLink(ExampleLink::figureEight());
        link.insertLink(Link(1));
        verifyDiagramComponents(link, "Whitehead U Figure_Eight U 3x()",
            { "--++- ( ^0 _1 ^4 _3 ^2 _4 ) ( _0 ^1 _2 ^3 )",
              "++-- ( _0 ^1 _2 ^3 _1 ^0 _3 ^2 )", "( )", "( )", "( )" });
    }
}

TEST_F(LinkTest, linking) {
    EXPECT_EQ(empty.link.linking(), 0);

    EXPECT_EQ(unknot0.link.linking(), 0);
    EXPECT_EQ(unknot1.link.linking(), 0);
    EXPECT_EQ(unknot3.link.linking(), 0);
    EXPECT_EQ(unknotMonster.link.linking(), 0);
    EXPECT_EQ(unknotGordian.link.linking(), 0);

    EXPECT_EQ(trefoilLeft.link.linking(), 0);
    EXPECT_EQ(trefoilRight.link.linking(), 0);
    EXPECT_EQ(trefoil_r1x2.link.linking(), 0);
    EXPECT_EQ(trefoil_r1x6.link.linking(), 0);
    EXPECT_EQ(figureEight.link.linking(), 0);
    EXPECT_EQ(figureEight_r1x2.link.linking(), 0);
    EXPECT_EQ(conway.link.linking(), 0);
    EXPECT_EQ(kinoshitaTerasaka.link.linking(), 0);
    EXPECT_EQ(gst.link.linking(), 0);

    EXPECT_EQ(rht_rht.link.linking(), 0);
    EXPECT_EQ(rht_lht.link.linking(), 0);

    EXPECT_EQ(unlink2_0.link.linking(), 0);
    EXPECT_EQ(unlink3_0.link.linking(), 0);
    EXPECT_EQ(unlink2_r2.link.linking(), 0);
    EXPECT_EQ(unlink2_r1r1.link.linking(), 0);
    EXPECT_EQ(hopf.link.linking(), 1);
    EXPECT_EQ(whitehead.link.linking(), 0);
    EXPECT_EQ(borromean.link.linking(), 0);
    EXPECT_EQ(trefoil_unknot0.link.linking(), 0);
    EXPECT_EQ(trefoil_unknot1.link.linking(), 0);
    EXPECT_EQ(trefoil_unknot_overlap.link.linking(), 0);
}

static void verifyUnderOverForComponent(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (auto c : link.components()) {
        auto under = link.underForComponent(c);
        auto over = link.overForComponent(c);

        if (!c) {
            EXPECT_FALSE(under);
            EXPECT_FALSE(over);
            continue;
        }

        // Walk through the entire component.
        bool foundUnder = false, foundOver = false;
        bool hasUnder = false, hasOver = false;

        StrandRef s = c;
        do {
            if (s == under)
                foundUnder = true;
            if (s == over)
                foundOver = true;

            if (s.strand() == 0)
                hasUnder = true;
            else
                hasOver = true;

            ++s;
        } while (s != c);

        if (hasUnder) {
            EXPECT_TRUE(foundUnder);
            EXPECT_EQ(under.strand(), 0);
        } else {
            EXPECT_FALSE(foundUnder);
            EXPECT_FALSE(under);
        }

        if (hasOver) {
            EXPECT_TRUE(foundOver);
            EXPECT_EQ(over.strand(), 1);
        } else {
            EXPECT_FALSE(foundOver);
            EXPECT_FALSE(over);
        }
    }
}

TEST_F(LinkTest, underOverForComponent) {
    testManualCases(verifyUnderOverForComponent);
}

static void verifyWrithe(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    long sum = 0;
    for (size_t c = 0; c < link.countComponents(); ++c)
        sum += link.writheOfComponent(c);
    EXPECT_EQ(sum + 2 * link.linking(), link.writhe());
}

TEST_F(LinkTest, writhe) {
    testManualCases(verifyWrithe);
}

static void verifySeifertCircles(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    Link alt(link);
    while (alt.size() > 0)
        alt.resolve(alt.crossing(0));

    EXPECT_EQ(link.seifertCircles(), alt.countComponents());
}

TEST_F(LinkTest, seifertCircles) {
    testManualCases(verifySeifertCircles);
}

static void verifySelfFrame(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    Link framed(link);
    framed.selfFrame();

    EXPECT_EQ(framed.countComponents(), link.countComponents());
    for (size_t c = 0; c < framed.countComponents(); ++c)
        EXPECT_EQ(framed.writheOfComponent(c), 0);
    if (link.size() <= JONES_THRESHOLD)
        EXPECT_EQ(link.jones(), framed.jones());
}

TEST_F(LinkTest, selfFrame) {
    testManualCases(verifySelfFrame);
}

static void verifyParallel(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    long writhe = link.writhe();
    long linking = link.linking();

    // Compute the sum of writhe and |writhe| for each individual component.
    // We do this in quadratic time, so the code is simple enough to be sure
    // it's right.
    long writheSame = 0, absWritheSame = 0;
    for (size_t c = 0; c < link.countComponents(); ++c) {
        long writheComp = link.writheOfComponent(c);

        writheSame += writheComp;
        absWritheSame += (writheComp >= 0 ? writheComp : -writheComp);
    }

    for (int k = 0; k <= 3; ++k) {
        SCOPED_TRACE_NUMERIC(k);

        Link p = link.parallel(k, regina::Framing::Blackboard);
        EXPECT_EQ(p.countComponents(), k * link.countComponents());
        EXPECT_EQ(p.size(), k * k * link.size());
        EXPECT_EQ(p.writhe(), k * k * writhe);
        EXPECT_EQ(p.linking(), k * k * linking + (k * (k-1) * writheSame) / 2);

        p = link.parallel(k, regina::Framing::Seifert);
        EXPECT_EQ(p.countComponents(), k * link.countComponents());
        EXPECT_EQ(p.size(), k * k * link.size() + k * (k-1) * absWritheSame);
        EXPECT_EQ(p.writhe(), k * k * writhe - k * (k-1) * writheSame);
        EXPECT_EQ(p.linking(), k * k * linking);
    }
}

TEST_F(LinkTest, parallel) {
    testManualCases(verifyParallel);
}

static void verifyJones(const TestCase& test,
        const regina::Laurent<regina::Integer>& expected) {
    SCOPED_TRACE_CSTRING(test.name);

    // Since we are computing the Jones polynomial multiple times
    // (using different algorithms), we work with clones of the link
    // that do not clone any already-computed properties.

    // Always try the treewidth-based algorithm.
    EXPECT_EQ(Link(test.link, false).jones(Algorithm::Treewidth), expected);

    // Only try the naive algorithm if the link is small enough, since this
    // algorithm iterates through 2^n states.
    if (test.link.size() <= 40)
        EXPECT_EQ(Link(test.link, false).jones(Algorithm::Naive), expected);
}

TEST_F(LinkTest, jones) {
    verifyJones(empty, {});

    verifyJones(unknot0, {0, {1}});
    verifyJones(unknot1, {0, {1}});
    verifyJones(unknot3, {0, {1}});
    verifyJones(unknotMonster, {0, {1}});
    // Let's not attempt this with the (enormous) Gordian unknot.

    verifyJones(trefoilLeft, {-8, {-1,0,1,0,0,0,1}});
    verifyJones(trefoilRight, {2, {1,0,0,0,1,0,-1}});
    verifyJones(trefoil_r1x2, {2, {1,0,0,0,1,0,-1}});
    verifyJones(trefoil_r1x6, {2, {1,0,0,0,1,0,-1}});
    verifyJones(figureEight, {-4, {1,0,-1,0,1,0,-1,0,1}});
    verifyJones(figureEight_r1x2, {-4, {1,0,-1,0,1,0,-1,0,1}});

    // These three polynomials were computed using an old version of Regina.
    // For the mutant pair Conway and Kinoshita-Terasaka, the naive algorithm
    // was used.  For Gompf-Scharlemann-Thompson, the treewidth algorithm was
    // used (since this knot is too large for the naive algorithm to handle).
    verifyJones(conway,
        {-12, {1,0,-2,0,2,0,-2,0,1,0,0,0,0,0,2,0,-2,0,2,0,-1}});
    verifyJones(kinoshitaTerasaka,
        {-12, {1,0,-2,0,2,0,-2,0,1,0,0,0,0,0,2,0,-2,0,2,0,-1}});
    verifyJones(gst,
        {-14, {-1,0,1,0,0,0,0,0,-1,0,3,0,-3,0,5,0,-5,0,5,0,-3,0,-1,0,
               3,0,-4,0,2,0,-1,0,1,0,-1,0,1,0,-1,0,3,0,-4,0,3,0,-1}});

    verifyJones(rht_rht, {4, {1,0,0,0,2,0,-2,0,1,0,-2,0,1}});
    verifyJones(rht_lht, {-6, {-1,0,1,0,-1,0,3,0,-1,0,1,0,-1}});

    verifyJones(unlink2_0, {-1, {-1,0,-1}});
    verifyJones(unlink3_0, {-2, {1,0,2,0,1}});
    verifyJones(unlink2_r2, {-1, {-1,0,-1}});
    verifyJones(unlink2_r1r1, {-1, {-1,0,-1}});
    verifyJones(hopf, {1, {-1,0,0,0,-1}});
    verifyJones(whitehead, {-7, {1,0,-2,0,1,0,-2,0,1,0,-1}});
    verifyJones(borromean, {-6, {-1,0,3,0,-2,0,4,0,-2,0,3,0,-1}});
    verifyJones(trefoil_unknot0, {1, {-1,0,-1,0,-1,0,0,0,1}});
    verifyJones(trefoil_unknot1, {1, {-1,0,-1,0,-1,0,0,0,1}});
    verifyJones(trefoil_unknot_overlap, {1, {-1,0,-1,0,-1,0,0,0,1}});
}

static void verifyHomflyAZ(const TestCase& test,
        const regina::Laurent2<regina::Integer>& expected) {
    SCOPED_TRACE_CSTRING(test.name);

    // Since we are computing the HOMFLY polynomial multiple times
    // (using different algorithms), we work with clones of the link
    // that do not clone any already-computed properties.

    EXPECT_EQ(Link(test.link, false).homflyAZ(Algorithm::Backtrack), expected);
    EXPECT_EQ(Link(test.link, false).homflyAZ(Algorithm::Treewidth), expected);

    Link rev(test.link, false);
    rev.reverse();

    EXPECT_EQ(Link(rev, false).homflyAZ(Algorithm::Backtrack), expected);
    EXPECT_EQ(Link(rev, false).homflyAZ(Algorithm::Treewidth), expected);
}

static void verifyHomflyLM(const TestCase& test,
        const regina::Laurent2<regina::Integer>& expected) {
    SCOPED_TRACE_CSTRING(test.name);

    // Since we are computing the HOMFLY polynomial multiple times
    // (using different algorithms), we work with clones of the link
    // that do not clone any already-computed properties.

    EXPECT_EQ(Link(test.link, false).homflyLM(Algorithm::Backtrack), expected);
    EXPECT_EQ(Link(test.link, false).homflyLM(Algorithm::Treewidth), expected);

    Link rev(test.link, false);
    rev.reverse();

    EXPECT_EQ(Link(rev, false).homflyLM(Algorithm::Backtrack), expected);
    EXPECT_EQ(Link(rev, false).homflyLM(Algorithm::Treewidth), expected);
}

TEST_F(LinkTest, homfly) {
    verifyHomflyAZ(empty, {});
    verifyHomflyLM(empty, {});

    verifyHomflyAZ(unknot0, {{0,0,1}});
    verifyHomflyLM(unknot0, {{0,0,1}});
    verifyHomflyAZ(unknot1, {{0,0,1}});
    verifyHomflyLM(unknot1, {{0,0,1}});
    verifyHomflyAZ(unknot3, {{0,0,1}});
    verifyHomflyLM(unknot3, {{0,0,1}});
    verifyHomflyAZ(unknotMonster, {{0,0,1}});
    verifyHomflyLM(unknotMonster, {{0,0,1}});
    // Let's not attempt this with the (enormous) Gordian unknot.

    verifyHomflyLM(trefoilLeft, {{4,0,-1}, {2,2,1}, {2,0,-2}});
    verifyHomflyLM(trefoilRight, {{-2,2,1}, {-2,0,-2}, {-4,0,-1}});
    verifyHomflyAZ(trefoilRight, {{-2,2,1}, {-2,0,2}, {-4,0,-1}});
    verifyHomflyLM(trefoil_r1x2, {{-2,2,1}, {-2,0,-2}, {-4,0,-1}});
    verifyHomflyLM(trefoil_r1x6, {{-2,2,1}, {-2,0,-2}, {-4,0,-1}});
    verifyHomflyLM(figureEight, {{2,0,-1}, {0,2,1}, {0,0,-1}, {-2,0,-1}});
    verifyHomflyLM(figureEight_r1x2, {{2,0,-1}, {0,2,1}, {0,0,-1}, {-2,0,-1}});

    // These two polynomials (which form a mutant pair) were computed using an
    // old version of Regina, using Kauffman's algorithm (Algorithm::Backtrack).
    verifyHomflyLM(conway,
        {{4,4,1}, {4,2,-3}, {4,0,2}, {2,6,-1}, {2,4,6}, {2,2,-11}, {2,0,6},
         {0,6,-1}, {0,4,6}, {0,2,-11}, {0,0,7}, {-2,4,1}, {-2,2,-3}, {-2,0,2}});
    verifyHomflyLM(kinoshitaTerasaka,
        {{4,4,1}, {4,2,-3}, {4,0,2}, {2,6,-1}, {2,4,6}, {2,2,-11}, {2,0,6},
         {0,6,-1}, {0,4,6}, {0,2,-11}, {0,0,7}, {-2,4,1}, {-2,2,-3}, {-2,0,2}});

    // Again, this was computed with an old Regina using Kauffman's algorithm.
    // We skip the test here because (on my machine) it takes around 5s to run
    // with Algorithm::Treewidth and around 50s to run with Algorithm::Naive.
#if 0
    verifyHomflyLM(gst,
        {{4,8,-1}, {4,6,6}, {4,4,-11}, {4,2,8}, {4,0,-2}, {2,12,-1}, {2,10,10},
         {2,8,-35}, {2,6,49}, {2,4,-21}, {2,2,-7}, {2,0,5}, {0,14,1},
         {0,12,-12}, {0,10,53}, {0,8,-102}, {0,6,67}, {0,4,36}, {0,2,-63},
         {0,0,21}, {-2,16,-1}, {-2,14,16}, {-2,12,-104}, {-2,10,355},
         {-2,8,-685}, {-2,6,744}, {-2,4,-422}, {-2,2,100}, {-2,0,-3},
         {-4,18,1}, {-4,16,-18}, {-4,14,137}, {-4,12,-575}, {-4,10,1457},
         {-4,8,-2296}, {-4,6,2233}, {-4,4,-1279}, {-4,2,385}, {-4,0,-45},
         {-6,18,1}, {-6,16,-17}, {-6,14,122}, {-6,12,-484}, {-6,10,1168},
         {-6,8,-1776}, {-6,6,1698}, {-6,4,-978}, {-6,2,304}, {-6,0,-38},
         {-8,16,-1}, {-8,14,14}, {-8,12,-79}, {-8,10,233}, {-8,8,-393},
         {-8,6,392}, {-8,4,-228}, {-8,2,71}, {-8,0,-9}});
#endif

    verifyHomflyLM(rht_rht,
        {{-4,4,1}, {-4,2,-4}, {-4,0,4}, {-6,2,-2}, {-6,0,4}, {-8,0,1}});
    verifyHomflyLM(rht_lht,
        {{2,2,-1}, {2,0,2}, {0,4,1}, {0,2,-4}, {0,0,5}, {-2,2,-1}, {-2,0,2}});

    verifyHomflyLM(unlink2_0, {{1,-1,-1}, {-1,-1,-1}});
    verifyHomflyLM(unlink3_0, {{2,-2,1}, {0,-2,2}, {-2,-2,1}});
    verifyHomflyLM(unlink2_r2, {{1,-1,-1}, {-1,-1,-1}});
    verifyHomflyLM(unlink2_r1r1, {{1,-1,-1}, {-1,-1,-1}});

    verifyHomflyLM(hopf, {{-1,1,-1}, {-1,-1,1}, {-3,-1,1}});

    // Computed with old Regina using Kauffman's algorithm:
    verifyHomflyLM(whitehead,
        {{3,1,1}, {1,3,-1}, {1,1,2}, {1,-1,-1}, {-1,1,1}, {-1,-1,-1}});
    verifyHomflyLM(borromean,
        {{2,2,-1}, {2,-2,1}, {0,4,1}, {0,2,-2}, {0,-2,2},
        {-2,2,-1}, {-2,-2,1}});

    verifyHomflyLM(trefoil_unknot0,
        {{-1,1,-1}, {-1,-1,2}, {-3,1,-1}, {-3,-1,3}, {-5,-1,1}});
    verifyHomflyLM(trefoil_unknot1,
        {{-1,1,-1}, {-1,-1,2}, {-3,1,-1}, {-3,-1,3}, {-5,-1,1}});
    verifyHomflyLM(trefoil_unknot_overlap,
        {{-1,1,-1}, {-1,-1,2}, {-3,1,-1}, {-3,-1,3}, {-5,-1,1}});

    // This is different from Adams' claim regarding the HOMFLY polynomial of
    // this link.  But... Adams does get his arithmetic wrong elsewhere, and a
    // calculation by hand using the Skein relation agrees with the polynomial
    // below.
    verifyHomflyLM(adams6_28,
        {{1,1,1}, {-1,3,-1}, {-1,1,1}, {-3,1,2}, {-3,-1,-1}, {-5,-1,-1}});

    // TODO: Verify that knot composition multiplies HOMFLY polynomials
    // TODO: Verify that HOMFLY gives Jones by:
    //   * l = it^-1, m = i(t^-1/2 - t^1/2)
    //   * a = t^-1, z = t^1/2 - t^-1/2
}

static void verifyComplementBasic(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    Triangulation<3> c = link.complement();

    EXPECT_EQ(c.countComponents(), 1);

    size_t ideal = 0;
    for (auto v : c.vertices()) {
        regina::Vertex<3>::LinkType t = v->linkType();
        if (t == regina::Vertex<3>::TORUS)
            ++ideal;
        else
            EXPECT_EQ(t, regina::Vertex<3>::SPHERE);
    }
    EXPECT_EQ(ideal, link.countComponents());
}

static void verifyComplementTrefoilUnknot(const TestCase& test) {
    // A specialised test for the link (trefoil U unknot).
    SCOPED_TRACE_CSTRING(test.name);

    // Find a separating sphere in the complement.
    bool foundSplit = false;
    regina::NormalSurfaces vtx(test.link.complement(),
        regina::NormalCoords::Standard);
    for (const regina::NormalSurface& s : vtx) {
        if (s.eulerChar() != 2)
            continue;
        // s must be a separating 2-sphere.

        Triangulation<3> cut = s.cutAlong();
        ASSERT_EQ(cut.countComponents(), 2);

        cut.finiteToIdeal(); // Fills the sphere boundaries with balls.
        cut.intelligentSimplify();
        auto comp = cut.triangulateComponents();

        if (comp[0].isIdeal() && comp[1].isIdeal()) {
            // This should be the sphere that separates the link components.
            // Note: there may be many such spheres, and _every_ one should
            // produce the same two complements.
            if ((comp[0].isSolidTorus() && isTrefoilComplement(comp[1])) ||
                    (comp[1].isSolidTorus() && isTrefoilComplement(comp[0])))
                foundSplit = true;
            else
                ADD_FAILURE() << "Link splits into unexpected components: "
                    << comp[0].isoSig() << ' ' << comp[1].isoSig();
        }
    }

    if (! foundSplit)
        ADD_FAILURE() << "Link does not split as expected";
}

TEST_F(LinkTest, complement) {
    testManualCases(verifyComplementBasic);

    // Some extra tests for specific complements that are easy to recognise.

    EXPECT_TRUE(empty.link.complement().isSphere());

    EXPECT_TRUE(unknot0.link.complement().isSolidTorus());
    EXPECT_TRUE(unknot1.link.complement().isSolidTorus());
    EXPECT_TRUE(unknot3.link.complement().isSolidTorus());
    EXPECT_TRUE(unknotMonster.link.complement().isSolidTorus());
    // Skip unknotGordian due to its size.

    EXPECT_EQ(unlink2_0.link.complement().group().recogniseGroup(), "Free(2)");
    EXPECT_EQ(unlink3_0.link.complement().group().recogniseGroup(), "Free(3)");
    EXPECT_EQ(unlink2_r2.link.complement().group().recogniseGroup(), "Free(2)");
    EXPECT_EQ(unlink2_r1r1.link.complement().group().recogniseGroup(),
        "Free(2)");

    EXPECT_EQ(hopf.link.complement().group().recogniseGroup(), "2 Z");

    // For some knots and links, it is reasonable to assume that
    // intelligentSimplify() will reach a minimal triangulation.

    EXPECT_TRUE(isFigureEightComplement(figureEight.link.complement()));
    EXPECT_TRUE(isFigureEightComplement(figureEight_r1x2.link.complement()));

    EXPECT_TRUE(isTrefoilComplement(trefoilLeft.link.complement()));
    EXPECT_TRUE(isTrefoilComplement(trefoilRight.link.complement()));
    EXPECT_TRUE(isTrefoilComplement(trefoil_r1x2.link.complement()));
    EXPECT_TRUE(isTrefoilComplement(trefoil_r1x6.link.complement()));

    EXPECT_TRUE(isCensusManifold(whitehead.link.complement(), "m129"));
    EXPECT_TRUE(isCensusManifold(borromean.link.complement(), "t12067"));

    // Some very specialised tests:

    verifyComplementTrefoilUnknot(trefoil_unknot0);
    verifyComplementTrefoilUnknot(trefoil_unknot1);
    verifyComplementTrefoilUnknot(trefoil_unknot_overlap);
}

static void verifyR1Count(const TestCase& test, size_t up, size_t down) {
    // Most of the time, up == #crossings * 8.  However, there will be more
    // moves available if the link has any zero-crossing unknot components.

    SCOPED_TRACE_CSTRING(test.name);

    // We solemnly promise not to alter the link; however, r1() requires the
    // link to be non-const.
    Link& link(const_cast<Link&>(test.link));

    size_t foundUp = 0, foundDown = 0;

    for (int side = 0; side < 2; ++side)
        for (int sign = -1; sign <= 1; sign += 2)
            if (link.r1(StrandRef(), side, sign, true, false))
                ++foundUp;

    if (link.r1(nullptr, true, false))
        ++foundDown;

    for (auto c : link.crossings()) {
        for (int side = 0; side < 2; ++side)
            for (int sign = -1; sign <= 1; sign += 2)
                for (int strand = 0; strand < 2; ++strand)
                    if (link.r1(c->strand(strand), side, sign, true, false))
                        ++foundUp;

        if (link.r1(c, true, false))
            ++foundDown;
    }

    EXPECT_EQ(foundUp, up);
    EXPECT_EQ(foundDown, down);
}

TEST_F(LinkTest, r1Count) {
    verifyR1Count(empty, 0, 0);

    verifyR1Count(unknot0, 4, 0);
    verifyR1Count(unknot1, 8, 1);
    verifyR1Count(unknot3, 24, 0);
    verifyR1Count(unknotMonster, 80, 0);
    verifyR1Count(unknotGordian, 1128, 0);

    verifyR1Count(trefoilLeft, 24, 0);
    verifyR1Count(trefoilRight, 24, 0);
    verifyR1Count(trefoil_r1x2, 40, 2);
    verifyR1Count(trefoil_r1x6, 72, 6);
    verifyR1Count(figureEight, 32, 0);
    verifyR1Count(figureEight_r1x2, 48, 2);
    verifyR1Count(conway, 88, 0);
    verifyR1Count(kinoshitaTerasaka, 88, 0);
    verifyR1Count(gst, 384, 0);

    verifyR1Count(rht_rht, 48, 0);
    verifyR1Count(rht_lht, 48, 0);

    verifyR1Count(unlink2_0, 4, 0);
    verifyR1Count(unlink3_0, 4, 0);
    verifyR1Count(unlink2_r2, 16, 0);
    verifyR1Count(unlink2_r1r1, 16, 2);

    verifyR1Count(hopf, 16, 0);
    verifyR1Count(whitehead, 40, 0);
    verifyR1Count(borromean, 48, 0);
    verifyR1Count(trefoil_unknot0, 28, 0);
    verifyR1Count(trefoil_unknot1, 32, 1);
    verifyR1Count(trefoil_unknot_overlap, 40, 0);
    verifyR1Count(adams6_28, 48, 0);
}

static void verifyR2Count(const TestCase& test, size_t up,
        size_t downByCrossing, size_t downByStrand) {
    // Most of the time, downByStrand == downByCrossing * 2.
    // However, this can differ in the case of an unknotted loop placed on top
    // of another strand - here there are 2 moves by crossing, but only 3 moves
    // by strand (all of which produce identical results).

    SCOPED_TRACE_CSTRING(test.name);

    // We solemnly promise not to alter the link; however, r2() requires the
    // link to be non-const.
    Link& link(const_cast<Link&>(test.link));

    size_t foundUp = 0, foundDownByCrossing = 0, foundDownByStrand = 0;

    for (int side1 = 0; side1 < 2; ++side1)
        for (int side2 = 0; side2 < 2; ++side2)
            if (link.r2({}, side1, {}, side2, true, false))
                ++foundUp;

    for (auto c : link.crossings()) {
        for (int side1 = 0; side1 < 2; ++side1)
            for (int side2 = 0; side2 < 2; ++side2)
                for (int str1 = 0; str1 < 2; ++str1) {
                    if (link.r2({}, side1, c->strand(str1), side2, true, false))
                        ++foundUp;
                    if (link.r2(c->strand(str1), side1, {}, side2, true, false))
                        ++foundUp;

                    for (auto c2 : link.crossings())
                        for (int str2 = 0; str2 < 2; ++str2)
                            if (link.r2(c->strand(str1), side1,
                                    c2->strand(str2), side2, true, false))
                                ++foundUp;
                }

        if (link.r2(c, true, false))
            ++foundDownByCrossing;

        for (int strand = 0; strand < 2; ++strand)
            if (link.r2(c->strand(strand), true, false))
                ++foundDownByStrand;
    }

    if (link.r2(nullptr, true, false))
        ++foundDownByCrossing;

    if (link.r2(StrandRef(), true, false))
        ++foundDownByStrand;

    EXPECT_EQ(foundUp, up);
    EXPECT_EQ(foundDownByCrossing, downByCrossing);
    EXPECT_EQ(foundDownByStrand, downByStrand);
}

TEST_F(LinkTest, r2Count) {
    verifyR2Count(empty, 0, 0, 0);

    verifyR2Count(unknot0, 0, 0, 0);
    verifyR2Count(unknot1, 2, 0, 0);
    verifyR2Count(unknot3, 18, 2, 4);
    verifyR2Count(unknotMonster, 116, 0, 0); // computed using Regina 7.3
    verifyR2Count(unknotGordian, 2046, 0, 0); // computed using Regina 7.3

    verifyR2Count(trefoilLeft, 18, 0, 0);
    verifyR2Count(trefoilRight, 18, 0, 0);
    verifyR2Count(trefoil_r1x2, 58, 0, 0);
    verifyR2Count(trefoil_r1x6, 160, 0, 0);
    verifyR2Count(figureEight, 28, 0, 0);
    verifyR2Count(figureEight_r1x2, 66, 0, 0);
    verifyR2Count(conway, 120, 0, 0);
    verifyR2Count(kinoshitaTerasaka, 118, 0, 0);
    verifyR2Count(gst, 612, 0, 0);

    verifyR2Count(rht_rht, 62, 0, 0); // merges bigon-bigon, triangle-triangle
    verifyR2Count(rht_lht, 60, 0, 0); // merges bigon-triangle, bigon-triangle

    verifyR2Count(unlink2_0, 4, 0, 0);
    verifyR2Count(unlink3_0, 4, 0, 0);
    verifyR2Count(unlink2_r2, 8, 2, 4);
    verifyR2Count(unlink2_r1r1, 36, 0, 0);

    verifyR2Count(hopf, 8, 0, 0);
    verifyR2Count(whitehead, 40, 0, 0);
    verifyR2Count(borromean, 48, 0, 0);
    verifyR2Count(trefoil_unknot0, 66, 0, 0);
    verifyR2Count(trefoil_unknot1, 116, 0, 0);
    verifyR2Count(trefoil_unknot_overlap, 46, 2, 3);
    verifyR2Count(adams6_28, 54, 0, 0);
}

static void verifyR3Count(const TestCase& test, size_t movesByCrossing) {
    // All of the time, movesByStrand == 3 * movesByCrossing.

    SCOPED_TRACE_CSTRING(test.name);

    // We solemnly promise not to alter the link; however, r2() requires the
    // link to be non-const.
    Link& link(const_cast<Link&>(test.link));

    size_t foundByCrossing = 0, foundByStrand = 0;

    for (int side = 0; side < 2; ++side) {
        if (link.r3(nullptr, side, true, false))
            ++foundByCrossing;

        if (link.r3(StrandRef(), side, true, false))
            ++foundByStrand;
    }

    for (auto c : link.crossings())
        for (int side = 0; side < 2; ++side) {
            if (link.r3(c, side, true, false))
                ++foundByCrossing;

            for (int strand = 0; strand < 2; ++strand)
                if (link.r3(c->strand(strand), side, true, false))
                    ++foundByStrand;
        }

    EXPECT_EQ(foundByCrossing, movesByCrossing);
    EXPECT_EQ(foundByStrand, movesByCrossing * 3);
}

TEST_F(LinkTest, r3Count) {
    verifyR3Count(empty, 0);

    verifyR3Count(unknot0, 0);
    verifyR3Count(unknot1, 0);
    verifyR3Count(unknot3, 2);
    verifyR3Count(unknotMonster, 2); // computed using Regina 7.3
    verifyR3Count(unknotGordian, 60); // computed using Regina 7.3

    verifyR3Count(trefoilLeft, 0);
    verifyR3Count(trefoilRight, 0);
    verifyR3Count(trefoil_r1x2, 0);
    verifyR3Count(trefoil_r1x6, 0);
    verifyR3Count(figureEight, 0);
    verifyR3Count(figureEight_r1x2, 0);
    verifyR3Count(conway, 0);
    verifyR3Count(kinoshitaTerasaka, 0);
    verifyR3Count(gst, 10);

    verifyR3Count(rht_rht, 0);
    verifyR3Count(rht_lht, 0);

    verifyR3Count(unlink2_0, 0);
    verifyR3Count(unlink3_0, 0);
    verifyR3Count(unlink2_r2, 0);
    verifyR3Count(unlink2_r1r1, 0);

    verifyR3Count(hopf, 0);
    verifyR3Count(whitehead, 0);
    verifyR3Count(borromean, 0);
    verifyR3Count(trefoil_unknot0, 0);
    verifyR3Count(trefoil_unknot1, 0);
    verifyR3Count(trefoil_unknot_overlap, 0);
    verifyR3Count(adams6_28, 0);
}

// For each of the following Reimeister verification functions, we pass the
// link by value (thus allowing for moves where possible), and modify it
// directly.  For the trace we use briefResult since we already have this in
// string form (the original link would be better but that's more expensive).

static void verifyR1Down(Link link, int crossing, const char* briefResult) {
    SCOPED_TRACE_CSTRING(briefResult);
    SCOPED_TRACE_NUMERIC(crossing);

    EXPECT_TRUE(link.r1(link.crossing(crossing)));
    EXPECT_TRUE(isConsistent(link));
    EXPECT_EQ(link.brief(), briefResult);
}

static void verifyR1Up(Link link, int crossing, int strand, int side, int sign,
        const char* briefResult) {
    SCOPED_TRACE_CSTRING(briefResult);
    SCOPED_TRACE_NUMERIC(crossing);
    SCOPED_TRACE_NUMERIC(strand);
    SCOPED_TRACE_NUMERIC(side);
    SCOPED_TRACE_NUMERIC(sign);

    StrandRef s;
    if (crossing >= 0)
        s = link.crossing(crossing)->strand(strand);

    EXPECT_TRUE(link.r1(s, side, sign));
    EXPECT_TRUE(isConsistent(link));
    EXPECT_EQ(link.brief(), briefResult);
}

static void verifyR2Down(Link link, int crossing, const char* briefResult) {
    SCOPED_TRACE_CSTRING(briefResult);
    SCOPED_TRACE_NUMERIC(crossing);

    EXPECT_TRUE(link.r2(link.crossing(crossing)));
    EXPECT_TRUE(isConsistent(link));
    EXPECT_EQ(link.brief(), briefResult);
}

static void verifyR2Down(Link link, int crossing, int strand,
        const char* briefResult) {
    SCOPED_TRACE_CSTRING(briefResult);
    SCOPED_TRACE_NUMERIC(crossing);
    SCOPED_TRACE_NUMERIC(strand);

    EXPECT_TRUE(link.r2(link.crossing(crossing)->strand(strand)));
    EXPECT_TRUE(isConsistent(link));
    EXPECT_EQ(link.brief(), briefResult);
}

static void verifyR2Up(Link link, int upperCrossing, int upperStrand,
        int upperSide, int lowerCrossing, int lowerStrand, int lowerSide,
        const char* briefResult) {
    SCOPED_TRACE_CSTRING(briefResult);
    SCOPED_TRACE_NUMERIC(upperCrossing);
    SCOPED_TRACE_NUMERIC(upperStrand);
    SCOPED_TRACE_NUMERIC(upperSide);
    SCOPED_TRACE_NUMERIC(lowerCrossing);
    SCOPED_TRACE_NUMERIC(lowerStrand);
    SCOPED_TRACE_NUMERIC(lowerSide);

    StrandRef upper, lower;
    if (upperCrossing >= 0)
        upper = link.crossing(upperCrossing)->strand(upperStrand);
    if (lowerCrossing >= 0)
        lower = link.crossing(lowerCrossing)->strand(lowerStrand);

    EXPECT_TRUE(link.r2(upper, upperSide, lower, lowerSide));
    EXPECT_TRUE(isConsistent(link));
    EXPECT_EQ(link.brief(), briefResult);
}

static void verifyR3(Link link, int crossing, int side,
        const char* briefResult) {
    SCOPED_TRACE_CSTRING(briefResult);
    SCOPED_TRACE_NUMERIC(crossing);
    SCOPED_TRACE_NUMERIC(side);

    EXPECT_TRUE(link.r3(link.crossing(crossing), side));
    EXPECT_TRUE(isConsistent(link));
    EXPECT_EQ(link.brief(), briefResult);
}

static void verifyR3(Link link, int crossing, int strand, int side,
        const char* briefResult) {
    SCOPED_TRACE_CSTRING(briefResult);
    SCOPED_TRACE_NUMERIC(crossing);
    SCOPED_TRACE_NUMERIC(strand);
    SCOPED_TRACE_NUMERIC(side);

    EXPECT_TRUE(link.r3(link.crossing(crossing)->strand(strand), side));
    EXPECT_TRUE(isConsistent(link));
    EXPECT_EQ(link.brief(), briefResult);
}

TEST_F(LinkTest, reidemeister) {
    // Single twist:
    verifyR1Down(Link::fromData({ -1 }, { 1, -1 }), 0, "( )");

    // Two twists:
    {
        Link link = Link::fromData({ 1, -1 }, { -1, 1, 2, -2 });
        verifyR1Down(link, 0, "- ( ^0 _0 )");
        verifyR2Down(link, 0, "( )");
        verifyR2Down(link, 0, 1, "( )");
        verifyR2Down(link, 1, 0, "( )");
    }
    {
        Link link = Link::fromData({ 1, -1 }, { 1, 2, -2, -1 });
        verifyR1Down(link, 0, "- ( ^0 _0 )");
        verifyR2Down(link, 0, "( )");
        verifyR2Down(link, 0, 1, "( )");
        verifyR2Down(link, 1, 0, "( )");
    }
    {
        Link link = Link::fromData({ 1, -1 }, { 2, -2, -1, 1 });
        verifyR1Down(link, 0, "- ( ^0 _0 )");
        verifyR2Down(link, 0, "( )");
        verifyR2Down(link, 0, 1, "( )");
        verifyR2Down(link, 1, 0, "( )");
    }
    {
        Link link = Link::fromData({ 1, -1 }, { -2, -1, 1, 2 });
        verifyR1Down(link, 0, "- ( _0 ^0 )");
        verifyR2Down(link, 0, "( )");
        verifyR2Down(link, 0, 1, "( )");
        verifyR2Down(link, 1, 0, "( )");
    }

    // Overlapping loops:
    {
        Link link = Link::fromData({ 1, -1 }, { 1, 2 }, { -2, -1 });
        verifyR2Down(link, 0, "( ) ( )");
        verifyR2Down(link, 1, "( ) ( )");
        verifyR2Down(link, 0, 0, "( ) ( )");
        verifyR2Down(link, 0, 1, "( ) ( )");
        verifyR2Down(link, 1, 0, "( ) ( )");
        verifyR2Down(link, 1, 1, "( ) ( )");
    }

    // Loop overlapping twist:
    {
        Link link = Link::fromData({ -1, 1, -1 }, { -1, 1, 3, 2 }, { -2, -3 });
        verifyR2Down(link, 2, "- ( _0 ^0 ) ( )");
        verifyR2Down(link, 2, 1, "- ( _0 ^0 ) ( )");
        verifyR2Down(link, 1, 0, "- ( _0 ^0 ) ( )");
        verifyR2Down(link, 2, 0, "- ( _0 ^0 ) ( )");
        verifyR3(link, 0, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )");
        verifyR3(link, 0, 1, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )");
        verifyR3(link, 1, 1, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )");
        verifyR3(link, 1, 0, 1, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )");
    }
    {
        Link link = Link::fromData({ -1, 1, -1 }, { 3, 2, -1, 1 }, { -2, -3 });
        verifyR2Down(link, 2, "- ( _0 ^0 ) ( )");
        verifyR2Down(link, 2, 1, "- ( _0 ^0 ) ( )");
        verifyR2Down(link, 1, 0, "- ( _0 ^0 ) ( )");
        verifyR2Down(link, 2, 0, "- ( _0 ^0 ) ( )");
        verifyR3(link, 0, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )");
        verifyR3(link, 0, 1, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )");
        verifyR3(link, 1, 1, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )");
        verifyR3(link, 1, 0, 1, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )");
    }
    {
        Link link = Link::fromData({ -1, 1, -1 }, { 2, -1, 1, 3 }, { -2, -3 });
        verifyR2Down(link, 2, "- ( _0 ^0 ) ( )");
        verifyR2Down(link, 2, 1, "- ( _0 ^0 ) ( )");
        verifyR2Down(link, 1, 0, "- ( _0 ^0 ) ( )");
        verifyR2Down(link, 2, 0, "- ( _0 ^0 ) ( )");
        verifyR3(link, 0, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )");
        verifyR3(link, 0, 1, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )");
        verifyR3(link, 1, 1, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )");
        verifyR3(link, 1, 0, 1, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 1, -1, -3, -2 }, { 2, 3 });
        verifyR2Down(link, 1, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 2, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 2, 0, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 1, 1, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 2, 1, "+ ( ^0 _0 ) ( )");
        verifyR3(link, 1, 1, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )");
        verifyR3(link, 0, 0, 0, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )");
        verifyR3(link, 1, 0, 0, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )");
        verifyR3(link, 1, 1, 1, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { -3, -2, 1, -1 }, { 2, 3 });
        verifyR2Down(link, 1, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 2, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 2, 0, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 1, 1, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 2, 1, "+ ( ^0 _0 ) ( )");
        verifyR3(link, 1, 1, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )");
        verifyR3(link, 0, 0, 0, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )");
        verifyR3(link, 1, 0, 0, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )");
        verifyR3(link, 1, 1, 1, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { -2, 1, -1, -3 }, { 2, 3 });
        verifyR2Down(link, 1, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 2, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 2, 0, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 1, 1, "+ ( ^0 _0 ) ( )");
        verifyR2Down(link, 2, 1, "+ ( ^0 _0 ) ( )");
        verifyR3(link, 1, 1, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )");
        verifyR3(link, 0, 0, 0, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )");
        verifyR3(link, 1, 0, 0, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )");
        verifyR3(link, 1, 1, 1, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )");
    }

    // Three triangles:
    {
        Link link = Link::fromData({ 1, -1, 1, -1, 1, -1 }, { 5, 4 },
            { 6, -6, -5, -3, 1, -1, -2, 2, 3, -4 });
        verifyR3(link, 4, 1,
            "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )");
        verifyR3(link, 4, 1, 1,
            "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )");
        verifyR3(link, 5, 0, 1,
            "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )");
        verifyR3(link, 3, 0, 1,
            "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )");
        verifyR3(link, 4, 0,
            "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )");
        verifyR3(link, 4, 1, 0,
            "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )");
        verifyR3(link, 4, 0, 1,
            "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )");
        verifyR3(link, 2, 1, 1,
            "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )");
        verifyR3(link, 1, 0,
            "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )");
        verifyR3(link, 1, 1, 0,
            "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )");
        verifyR3(link, 2, 0, 0,
            "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )");
        verifyR3(link, 0, 0, 0,
            "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )");
    }
    {
        Link link = Link::fromData({ -1, +1, -1, +1, -1, +1 }, { -5, -4 },
            { -6, 6, 5, 3, -1, 1, 2, -2, -3, 4 });
        verifyR3(link, 5, 1,
            "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )");
        verifyR3(link, 4, 0, 1,
            "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )");
        verifyR3(link, 5, 1, 1,
            "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )");
        verifyR3(link, 3, 1, 1,
            "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )");
        verifyR3(link, 4, 1,
            "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )");
        verifyR3(link, 4, 0, 0,
            "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )");
        verifyR3(link, 4, 1, 1,
            "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )");
        verifyR3(link, 2, 0, 1,
            "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )");
        verifyR3(link, 0, 0,
            "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )");
        verifyR3(link, 1, 0, 0,
            "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )");
        verifyR3(link, 2, 1, 0,
            "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )");
        verifyR3(link, 0, 1, 0,
            "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )");
    }

    // Bad trefoil:
    {
        Link link = Link::fromData({ 1, 1, -1 }, { 1, -2, -3, -1, 2, 3 });
        verifyR2Down(link, 2, "+ ( _0 ^0 )");
        verifyR2Down(link, 2, 1, "+ ( _0 ^0 )");
        verifyR2Down(link, 2, 0, "+ ( _0 ^0 )");
        verifyR3(link, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )");
        verifyR3(link, 1, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )");
        verifyR3(link, 2, 0, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )");
        verifyR3(link, 0, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )");
    }
    {
        Link link = Link::fromData({ 1, 1, -1 }, { -3, -1, 2, 3, 1, -2 });
        verifyR2Down(link, 2, "+ ( ^0 _0 )");
        verifyR2Down(link, 2, 1, "+ ( ^0 _0 )");
        verifyR2Down(link, 2, 0, "+ ( ^0 _0 )");
        verifyR3(link, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )");
        verifyR3(link, 1, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )");
        verifyR3(link, 2, 0, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )");
        verifyR3(link, 0, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )");
    }
    {
        Link link = Link::fromData({ -1, -1, 1 }, { 1, -2, -3, -1, 2, 3 });
        verifyR2Down(link, 2, "- ( _0 ^0 )");
        verifyR2Down(link, 2, 1, "- ( _0 ^0 )");
        verifyR2Down(link, 2, 0, "- ( _0 ^0 )");
        verifyR3(link, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )");
        verifyR3(link, 1, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )");
        verifyR3(link, 2, 0, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )");
        verifyR3(link, 0, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )");
    }
    {
        Link link = Link::fromData({ -1, -1, 1 }, { -3, -1, 2, 3, 1, -2 });
        verifyR2Down(link, 2, "- ( ^0 _0 )");
        verifyR2Down(link, 2, 1, "- ( ^0 _0 )");
        verifyR2Down(link, 2, 0, "- ( ^0 _0 )");
        verifyR3(link, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )");
        verifyR3(link, 1, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )");
        verifyR3(link, 2, 0, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )");
        verifyR3(link, 0, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )");
    }

    // Bad figure eight:
    {
        Link link = Link::fromData({ -1, 1, 1, -1},
            { 1, 2, -4, -3, -2, -1, 3, 4 });
        verifyR2Down(link, 0, "+- ( _1 _0 ^0 ^1 )");
        verifyR2Down(link, 0, 1, "+- ( _1 _0 ^0 ^1 )");
        verifyR2Down(link, 1, 0, "+- ( _1 _0 ^0 ^1 )");
        verifyR2Down(link, 2, "-+ ( ^0 ^1 _1 _0 )");
        verifyR2Down(link, 2, 1, "-+ ( ^0 ^1 _1 _0 )");
        verifyR2Down(link, 3, 0, "-+ ( ^0 ^1 _1 _0 )");
        verifyR3(link, 0, 1, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )");
        verifyR3(link, 0, 1, 1, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )");
        verifyR3(link, 0, 0, 0, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )");
        verifyR3(link, 2, 0, 0, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )");
    }
    {
        Link link = Link::fromData({ -1, 1, 1, -1},
            { 2, -4, -3, -2, -1, 3, 4, 1 });
        verifyR2Down(link, 0, "+- ( _1 _0 ^0 ^1 )");
        verifyR2Down(link, 0, 1, "+- ( _1 _0 ^0 ^1 )");
        verifyR2Down(link, 1, 0, "+- ( _1 _0 ^0 ^1 )");
    }
    {
        Link link = Link::fromData({ -1, 1, 1, -1},
            { -2, -1, 3, 4, 1, 2, -4, -3 });
        verifyR2Down(link, 0, "+- ( ^0 ^1 _1 _0 )");
        verifyR2Down(link, 0, 1, "+- ( ^0 ^1 _1 _0 )");
        verifyR2Down(link, 1, 0, "+- ( ^0 ^1 _1 _0 )");
    }
    {
        Link link = Link::fromData({ -1, 1, 1, -1},
            { -1, 3, 4, 1, 2, -4, -3, -2 });
        verifyR2Down(link, 0, "+- ( ^0 ^1 _1 _0 )");
        verifyR2Down(link, 0, 1, "+- ( ^0 ^1 _1 _0 )");
        verifyR2Down(link, 1, 0, "+- ( ^0 ^1 _1 _0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, -1, 1},
            { -1, -2, 4, 3, 2, 1, -3, -4 });
        verifyR2Down(link, 1, "-+ ( ^1 ^0 _0 _1 )");
        verifyR2Down(link, 0, 0, "-+ ( ^1 ^0 _0 _1 )");
        verifyR2Down(link, 1, 1, "-+ ( ^1 ^0 _0 _1 )");
        verifyR2Down(link, 3, "+- ( _0 _1 ^1 ^0 )");
        verifyR2Down(link, 2, 0, "+- ( _0 _1 ^1 ^0 )");
        verifyR2Down(link, 3, 1, "+- ( _0 _1 ^1 ^0 )");
        verifyR3(link, 2, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )");
        verifyR3(link, 0, 0, 1, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )");
        verifyR3(link, 0, 1, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )");
        verifyR3(link, 2, 1, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )");
    }
    {
        Link link = Link::fromData({ 1, -1, -1, 1},
            { -2, 4, 3, 2, 1, -3, -4, -1 });
        verifyR2Down(link, 1, "-+ ( ^1 ^0 _0 _1 )");
        verifyR2Down(link, 0, 0, "-+ ( ^1 ^0 _0 _1 )");
        verifyR2Down(link, 1, 1, "-+ ( ^1 ^0 _0 _1 )");
    }
    {
        Link link = Link::fromData({ 1, -1, -1, 1},
            { 2, 1, -3, -4, -1, -2, 4, 3 });
        verifyR2Down(link, 1, "-+ ( _0 _1 ^1 ^0 )");
        verifyR2Down(link, 0, 0, "-+ ( _0 _1 ^1 ^0 )");
        verifyR2Down(link, 1, 1, "-+ ( _0 _1 ^1 ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, -1, 1},
            { 1, -3, -4, -1, -2, 4, 3, 2 });
        verifyR2Down(link, 1, "-+ ( _0 _1 ^1 ^0 )");
        verifyR2Down(link, 0, 0, "-+ ( _0 _1 ^1 ^0 )");
        verifyR2Down(link, 1, 1, "-+ ( _0 _1 ^1 ^0 )");
    }

    // Excessive trefoil:
    {
        Link link = Link::fromData({ -1, 1, -1, -1, -1 },
            { -2, -3, 4, -5, 1, 2, 3, -4, 5, -1 });
        verifyR2Down(link, 0, "--- ( _0 ^1 _2 ^0 _1 ^2 )");
        verifyR2Down(link, 0, 0, "--- ( _0 ^1 _2 ^0 _1 ^2 )");
        verifyR2Down(link, 0, 1, "--- ( _0 ^1 _2 ^0 _1 ^2 )");
        verifyR2Down(link, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 )");
        verifyR2Down(link, 1, 0, "--- ( ^1 _2 ^0 _1 ^2 _0 )");
        verifyR2Down(link, 1, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1, 1, 1 },
            { 2, 3, -4, 5, -1, -2, -3, 4, -5, 1 });
        verifyR2Down(link, 0, "+++ ( ^0 _1 ^2 _0 ^1 _2 )");
        verifyR2Down(link, 0, 0, "+++ ( ^0 _1 ^2 _0 ^1 _2 )");
        verifyR2Down(link, 0, 1, "+++ ( ^0 _1 ^2 _0 ^1 _2 )");
        verifyR2Down(link, 1, "+++ ( _1 ^2 _0 ^1 _2 ^0 )");
        verifyR2Down(link, 1, 0, "+++ ( _1 ^2 _0 ^1 _2 ^0 )");
        verifyR2Down(link, 1, 1, "+++ ( _1 ^2 _0 ^1 _2 ^0 )");
    }

    // Dangling twists:
    {
        Link link = Link::fromData({ 1, -1, 1, -1, -1, 1, -1, -1 }, { -3, -4 },
            { 1, -1, -2, 7, 6, 5, 4, 3, -5, -6, -7, 8, -8, 2 });
        verifyR1Down(link, 0,
            "-+--+-- ( _1 _2 ) ( _0 ^5 ^4 ^3 ^2 ^1 _3 _4 _5 ^6 _6 ^0 )");
        verifyR1Down(link, 7,
            "+-+--+- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^4 ^3 ^2 _4 _5 _6 ^1 )");
        verifyR2Down(link, 1,
            "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )");
        verifyR2Down(link, 1, 1,
            "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )");
        verifyR2Down(link, 0, 0,
            "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )");
        verifyR2Down(link, 6,
            "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
        verifyR2Down(link, 6, 1,
            "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
        verifyR2Down(link, 5, 0,
            "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
        verifyR2Down(link, 5,
            "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
        verifyR2Down(link, 5, 1,
            "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
        verifyR2Down(link, 4, 0,
            "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )");
        verifyR2Down(link, 3,
            "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )");
        verifyR2Down(link, 3, 1,
            "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )");
        verifyR2Down(link, 2, 0,
            "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )");
        verifyR3(link, 4, 0,
            "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )");
        verifyR3(link, 4, 1, 0,
            "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )");
        verifyR3(link, 2, 0, 1,
            "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )");
        verifyR3(link, 2, 1, 0,
            "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1, -1, -1, 1, -1, -1 }, { -3, -4 },
            { 6, 5, 4, 3, -5, -6, -7, 8, -8, 2, 1, -1, -2, 7 });
        verifyR2Down(link, 6,
            "+-+--- ( _2 _3 ) ( ^4 ^3 ^2 _4 ^5 _5 ^1 ^0 _0 _1 )");
        verifyR2Down(link, 5,
            "+-+--- ( _2 _3 ) ( ^3 ^2 _4 ^5 _5 ^1 ^0 _0 _1 ^4 )");
    }
    {
        Link link = Link::fromData({ -1, 1, -1, 1, 1, -1, 1, 1 }, { 3, 4 },
            { -1, 1, 2, -7, -6, -5, -4, -3, 5, 6, 7, -8, 8, -2 });
        verifyR1Down(link, 0,
            "+-++-++ ( ^1 ^2 ) ( ^0 _5 _4 _3 _2 _1 ^3 ^4 ^5 _6 ^6 _0 )");
        verifyR1Down(link, 7,
            "-+-++-+ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _4 _3 _2 ^4 ^5 ^6 _1 )");
        verifyR2Down(link, 0,
            "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )");
        verifyR2Down(link, 0, 1,
            "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )");
        verifyR2Down(link, 1, 0,
            "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )");
        verifyR2Down(link, 5,
            "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
        verifyR2Down(link, 5, 1,
            "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
        verifyR2Down(link, 6, 0,
            "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
        verifyR2Down(link, 4,
            "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
        verifyR2Down(link, 4, 1,
            "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
        verifyR2Down(link, 5, 0,
            "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )");
        verifyR2Down(link, 2,
            "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )");
        verifyR2Down(link, 2, 1,
            "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )");
        verifyR2Down(link, 3, 0,
            "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )");
        verifyR3(link, 2, 1,
            "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )");
        verifyR3(link, 2, 1, 1,
            "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )");
        verifyR3(link, 2, 0, 0,
            "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )");
        verifyR3(link, 4, 0, 0,
            "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )");
    }
    {
        Link link = Link::fromData({ -1, 1, -1, 1, 1, -1, 1, 1 }, { 3, 4 },
            { -6, -5, -4, -3, 5, 6, 7, -8, 8, -2, -1, 1, 2, -7 });
        verifyR2Down(link, 5,
            "-+-+++ ( ^2 ^3 ) ( _4 _3 _2 ^4 _5 ^5 _1 _0 ^0 ^1 )");
        verifyR2Down(link, 4,
            "-+-+++ ( ^2 ^3 ) ( _3 _2 ^4 _5 ^5 _1 _0 ^0 ^1 _4 )");
    }

    // Figure eight with twist and three unknots:
    {
        Link link = Link::fromData({ 1, 1, -1, -1, -1 },
            { 0 }, { -1, 2, -4, 3, -2, 1, -3, 4, 5, -5 }, { 0 }, { 0 });
        verifyR1Up(link, -1, 0, 0, -1,
            "++---- ( ^5 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
        verifyR1Up(link, -1, 0, 0, 1,
            "++---+ ( _5 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
        verifyR1Up(link, -1, 0, 1, -1,
            "++---- ( _5 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
        verifyR1Up(link, -1, 0, 1, 1,
            "++---+ ( ^5 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
        verifyR1Up(link, 4, 1, 0, -1,
            "++---- ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 _5 _4 ) ( ) ( )");
        verifyR1Up(link, 4, 1, 0, 1,
            "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 ^5 _4 ) ( ) ( )");
        verifyR1Up(link, 4, 1, 1, -1,
            "++---- ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 ^5 _4 ) ( ) ( )");
        verifyR1Up(link, 4, 1, 1, 1,
            "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 _5 _4 ) ( ) ( )");
        verifyR1Up(link, 1, 0, 0, -1,
            "++---- ( ) ( _0 ^1 _3 ^2 _1 ^5 _5 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
        verifyR1Up(link, 1, 0, 0, 1,
            "++---+ ( ) ( _0 ^1 _3 ^2 _1 _5 ^5 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
        verifyR1Up(link, 1, 0, 1, -1,
            "++---- ( ) ( _0 ^1 _3 ^2 _1 _5 ^5 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
        verifyR1Up(link, 1, 0, 1, 1,
            "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^5 _5 ^0 _2 ^3 ^4 _4 ) ( ) ( )");
        // Note: for R2, the implementation always adds the two new
        // crossings in the order (+, -).
        verifyR2Up(link, -1, 0, 0, -1, 0, 0, "++---+- "
            "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _6 _5 ) ( )");
        verifyR2Up(link, -1, 0, 0, -1, 0, 1, "++---+- "
            "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _6 _5 ) ( )");
        verifyR2Up(link, -1, 0, 1, -1, 0, 0, "++---+- "
            "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _5 _6 ) ( )");
        verifyR2Up(link, -1, 0, 1, -1, 0, 1, "++---+- "
            "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _5 _6 ) ( )");
        verifyR2Up(link, -1, 0, 0, 4, 0, 0, "++---+- "
            "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 _6 _5 ) ( ) ( )");
        verifyR2Up(link, -1, 0, 1, 4, 0, 0, "++---+- "
            "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 _5 _6 ) ( ) ( )");
        verifyR2Up(link, 4, 0, 0, -1, 0, 0, "++---+- "
            "( _6 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ^5 ^6 ) ( ) ( )");
        verifyR2Up(link, 4, 0, 0, -1, 0, 1, "++---+- "
            "( _6 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ^6 ^5 ) ( ) ( )");
        verifyR2Up(link, -1, 0, 0, 4, 1, 1, "++---+- "
            "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _6 _5 _4 ) ( ) ( )");
        verifyR2Up(link, -1, 0, 1, 4, 1, 1, "++---+- "
            "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 _6 _4 ) ( ) ( )");
        verifyR2Up(link, 4, 1, 1, -1, 0, 0, "++---+- "
            "( _5 _6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 ^6 _4 ) ( ) ( )");
        verifyR2Up(link, 4, 1, 1, -1, 0, 1, "++---+- "
            "( _5 _6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^6 ^5 _4 ) ( ) ( )");
        verifyR2Up(link, 4, 0, 0, 3, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _6 _5 ^4 _4 ^5 ^6 ) ( ) ( )");
        verifyR2Up(link, 3, 1, 0, 4, 0, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^5 ^6 ^4 _4 _6 _5 ) ( ) ( )");
        verifyR2Up(link, 4, 1, 1, 1, 0, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 _5 _6 ^0 _2 ^3 ^4 ^6 ^5 _4 ) ( ) ( )");
        verifyR2Up(link, 1, 0, 1, 4, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^6 ^5 ^0 _2 ^3 ^4 _5 _6 _4 ) ( ) ( )");
        verifyR2Up(link, 2, 0, 0, 1, 1, 1, "++---+- "
            "( ) ( _0 ^1 _6 _5 _3 ^2 _1 ^0 _2 ^6 ^5 ^3 ^4 _4 ) ( ) ( )");
        verifyR2Up(link, 1, 1, 1, 2, 0, 0, "++---+- "
            "( ) ( _0 ^1 ^5 ^6 _3 ^2 _1 ^0 _2 _5 _6 ^3 ^4 _4 ) ( ) ( )");
    }
}

// Our links here will typically be temporaries, so allow them to be moved in.
static void verifyResolve(Link link, int crossing, const char* briefResult) {
    // Ideally we'd put link in the trace, but this is a bit expensive and
    // we already have briefResult in string form.
    SCOPED_TRACE_CSTRING(briefResult);

    link.resolve(link.crossing(crossing));

    EXPECT_TRUE(isConsistent(link));
    EXPECT_EQ(link.brief(), briefResult);
}

TEST_F(LinkTest, resolve) {
    // Single twists:
    verifyResolve(Link::fromData({ +1 }, { 1, -1 }), 0, "( ) ( )");
    verifyResolve(Link::fromData({ +1 }, { -1, 1 }), 0, "( ) ( )");
    verifyResolve(Link::fromData({ -1 }, { 1, -1 }), 0, "( ) ( )");
    verifyResolve(Link::fromData({ -1 }, { -1, 1 }), 0, "( ) ( )");

    // Trefoils with a single twist:
    verifyResolve(
        Link::fromData({ -1, +1, -1, -1 }, { 4, -1, 2, -2, 3, -4, 1, -3 }),
        1, "--- ( ^2 _0 ^1 _2 ^0 _1 ) ( )");
    verifyResolve(
        Link::fromData({ -1, +1, -1, -1 }, { 2, -2, 3, -4, 1, -3, 4, -1 }),
        1, "--- ( ^1 _2 ^0 _1 ^2 _0 ) ( )");
    verifyResolve(
        Link::fromData({ -1, +1, -1, -1 }, { -2, 3, -4, 1, -3, 4, -1, 2 }),
        1, "--- ( ^1 _2 ^0 _1 ^2 _0 ) ( )");
    verifyResolve(
        Link::fromData({ +1, -1, +1, +1 }, { 4, -1, -2, 2, 3, -4, 1, -3 }),
        1, "+++ ( ^2 _0 ^1 _2 ^0 _1 ) ( )");
    verifyResolve(
        Link::fromData({ +1, -1, +1, +1 }, { -2, 2, 3, -4, 1, -3, 4, -1 }),
        1, "+++ ( ^1 _2 ^0 _1 ^2 _0 ) ( )");
    verifyResolve(
        Link::fromData({ +1, -1, +1, +1 }, { 2, 3, -4, 1, -3, 4, -1, -2 }),
        1, "+++ ( ^1 _2 ^0 _1 ^2 _0 ) ( )");

    // Figure eight knots:
    verifyResolve(
        Link::fromData({ +1, +1, -1, -1 }, { 3, -1, 2, -3, 4, -2, 1, -4 }),
        2, "++- ( _0 ^1 ) ( ^2 _1 ^0 _2 )");
    verifyResolve(
        Link::fromData({ +1, +1, -1, -1 }, { -3, 4, -2, 1, -4, 3, -1, 2 }),
        2, "++- ( ^2 _1 ^0 _2 ) ( _0 ^1 )");
    verifyResolve(
        Link::fromData({ +1, +1, -1, -1 }, { 2, -3, 4, -2, 1, -4, 3, -1 }),
        2, "++- ( ^1 _0 ) ( ^2 _1 ^0 _2 )");
    verifyResolve(
        Link::fromData({ +1, +1, -1, -1 }, { 4, -2, 1, -4, 3, -1, 2, -3 }),
        2, "++- ( ^2 _1 ^0 _2 ) ( _0 ^1 )");

    // Figure eight knots with a link:
    verifyResolve(
        Link::fromData({ +1, +1, +1, +1, -1, -1 },
            { 2, -5, 6, -2, 1, 3, -4, -6, 5, -1 }, { -3, 4 }),
        2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )");
    verifyResolve(
        Link::fromData({ +1, +1, +1, +1, -1, -1 },
            { 2, -5, 6, -2, 1, 3, -4, -6, 5, -1 }, { 4, -3 }),
        2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )");
    verifyResolve(
        Link::fromData({ +1, +1, +1, +1, -1, -1 },
            { 3, -4, -6, 5, -1, 2, -5, 6, -2, 1 }, { -3, 4 }),
        2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )");
    verifyResolve(
        Link::fromData({ +1, +1, +1, +1, -1, -1 },
            { 3, -4, -6, 5, -1, 2, -5, 6, -2, 1 }, { 4, -3 }),
        2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )");
}

static void verifyKnotSig(const Link& link, bool reflect, bool reverse) {
    SCOPED_TRACE_NUMERIC(reflect);
    SCOPED_TRACE_NUMERIC(reverse);

    std::string sig = link.knotSig(reflect, reverse);
    EXPECT_FALSE(sig.empty());

    {
        Link alt(link, false);
        alt.rotate();
        EXPECT_EQ(alt.knotSig(reflect, reverse), sig);
    }
    if (reflect) {
        Link alt(link, false);
        alt.reflect();
        EXPECT_EQ(alt.knotSig(reflect, reverse), sig);
    }
    if (reverse) {
        Link alt(link, false);
        alt.reverse();
        EXPECT_EQ(alt.knotSig(reflect, reverse), sig);

        for (size_t i = 1; i < alt.countComponents(); ++i) {
            alt.reverse(alt.component(i));
            EXPECT_EQ(alt.knotSig(reflect, reverse), sig);
        }
    }
    if (reflect && reverse) {
        Link alt(link, false);
        alt.reflect();
        alt.reverse();
        EXPECT_EQ(alt.knotSig(reflect, reverse), sig);

        for (size_t i = 1; i < alt.countComponents(); ++i) {
            alt.reverse(alt.component(i));
            EXPECT_EQ(alt.knotSig(reflect, reverse), sig);
        }
    }

    Link recon;
    ASSERT_NO_THROW({ recon = Link::fromKnotSig(sig); });

    EXPECT_EQ(recon.size(), link.size());
    EXPECT_EQ(recon.countComponents(), link.countComponents());
    EXPECT_EQ(recon.knotSig(reflect, reverse), sig);
    if (link.size() <= JONES_THRESHOLD) {
        if (reverse && link.countComponents() > 1) {
            // We could reverse some but not all components, which will do
            // unusual things to the Jones polynomial.  At least we can
            // still span of exponents.
            auto reconJones = recon.jones();
            auto linkJones = link.jones();
            EXPECT_EQ(reconJones.maxExp() - reconJones.minExp(),
                linkJones.maxExp() - linkJones.minExp());
        } else if (reflect) {
            // The only possible change to the Jones polynomial is x -> x^-1.
            auto reconJones = recon.jones();
            auto linkJones = link.jones();

            auto reconJonesAlt = recon.jones();
            auto linkJonesAlt = link.jones();
            reconJonesAlt.invertX();
            linkJonesAlt.invertX();

            if (reconJonesAlt < reconJones)
                reconJones.swap(reconJonesAlt);
            if (linkJonesAlt < linkJones)
                linkJones.swap(linkJonesAlt);

            EXPECT_EQ(reconJones, linkJones);
        } else {
            EXPECT_EQ(recon.jones(), link.jones());
        }
    }

    // Verify the "magic" string constructor.
    EXPECT_NO_THROW({ EXPECT_EQ(Link(sig), recon); });
}

static void verifyKnotSig(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    verifyKnotSig(link, true, true);
    verifyKnotSig(link, true, false);
    verifyKnotSig(link, false, true);
    verifyKnotSig(link, false, false);
}

TEST_F(LinkTest, knotSig) {
    testManualCases(verifyKnotSig);

    // Test signatures that respect / ignore reflection:
    EXPECT_EQ(trefoilRight.link.knotSig(true, true),  "dabcabcv-");
    EXPECT_EQ(trefoilRight.link.knotSig(false, true), "dabcabcv-");
    EXPECT_EQ(trefoilLeft.link.knotSig(true, true),   "dabcabcv-");
    EXPECT_EQ(trefoilLeft.link.knotSig(false, true) , "dabcabcva");

    // Test that reflection applies to the entire diagram only, not individual
    // connected components:
    {
        Link l = ExampleLink::trefoilRight();
        l.insertLink(ExampleLink::trefoilRight());
        EXPECT_EQ(l.sig(true, true), "dabcabcv-dabcabcv-");
        EXPECT_EQ(l.sig(false, true), "dabcabcv-dabcabcv-");
    }
    {
        Link l = ExampleLink::trefoilRight();
        l.insertLink(ExampleLink::trefoilLeft());
        EXPECT_EQ(l.sig(true, true), "dabcabcv-dabcabcva");
        EXPECT_EQ(l.sig(false, true), "dabcabcv-dabcabcva");
    }
    {
        Link l = ExampleLink::trefoilLeft();
        l.insertLink(ExampleLink::trefoilRight());
        EXPECT_EQ(l.sig(true, true), "dabcabcv-dabcabcva");
        EXPECT_EQ(l.sig(false, true), "dabcabcv-dabcabcva");
    }
    {
        Link l = ExampleLink::trefoilLeft();
        l.insertLink(ExampleLink::trefoilLeft());
        EXPECT_EQ(l.sig(true, true), "dabcabcv-dabcabcv-");
        EXPECT_EQ(l.sig(false, true), "dabcabcvadabcabcva");
    }

    // A link where all four reflection/reversal options give different sigs:
    Link asymmetric = Link::fromOrientedGauss(
        "-<6 +>3 -<5 +>2 -<4 -<1 +>1 +>5 -<3 +>6 -<2 +>4");
    EXPECT_EQ(asymmetric.knotSig(true, true),   "gaabcdefbcfedPQ--");
    EXPECT_EQ(asymmetric.knotSig(true, false),  "gaabcdefdcbefPQ--");
    EXPECT_EQ(asymmetric.knotSig(false, true),  "gaabcdefbcfedPQaa");
    EXPECT_EQ(asymmetric.knotSig(false, false), "gaabcdefdcbefPQaa");

    // Verify some signatures against actual hard-coded strings, to ensure
    // that the single-component knot signature format from Regina ≤ 7.3
    // matches the more general format in Regina ≥ 7.4.
    //
    // The following knot signatures were all computed using Regina 7.3.
    EXPECT_EQ(unknot0.link.sig(), "a");
    EXPECT_EQ(unknot1.link.sig(), "baabd");
    EXPECT_EQ(unknot3.link.sig(), "dabcabchT");
    EXPECT_EQ(unknotMonster.link.sig(), "kabcdefghijbefgdcjahixfvbdwGd");
    EXPECT_EQ(unknotGordian.link.sig(), "-cncaabacadaeafagahaiajakalamanaoapaqarasatauavawaxayazaAaBaCaDaEaFavaGataHaIaJapaoaKaLalaMaNaOahaPaQaeaRaSabaTaUaVaWaXajaNaYaZafaQa0a1a2aUa3auaGa4a5a6aza7aBa8a9a+a-aabbbcbdbebfbgbhbibjbkblbmbnbobpbqbrbdbsbtbubvbwbxbybzbAbBb+aEaCbDbxaEb4aFbGbqaJaHbIbJbLamaobKbLbMbNbObibPbxbQbRbSbTb-aSacaUb0aVbWbVaXbHaYbraGbZb0bmb1b2b3bMbfb4b5b6bub7bRbzb8b9bAbSb+b-bsb5bacgbNbbc2b1blbccZbnaKaJbdcXaecWbfc1aUbdaRaabgc+b7bvbhcPbjbicbc3bLbjcpbMakadcIbHbIaYbsaFbkc6ayaDbCbDalc8a9b8bybQbwbhc6btb-bcbmcqbjcKbnb0bcckbicObhbac4bebrbmcbbgcTbBb9alcCa7aAakc5aEbwaFaaaTa2afcVbPagaZaYaOaiaecWaXb3aVB3NNT3NVoYNthlfdnryeRZac44044idYmmqb5MT6MAwhJx3YEkl-T9wMNu-F+rMu86EgBAxURTr1DzB6E60Z-7pYJKn2T");
    EXPECT_EQ(trefoilLeft.link.sig(), "dabcabcv-");
    EXPECT_EQ(trefoilRight.link.sig(), "dabcabcv-");
    EXPECT_EQ(trefoil_r1x2.link.sig(), "faabcdeebcd1eFo");
    EXPECT_EQ(trefoil_r1x6.link.sig(), "jaabccdeefggbhhdiifnwo-KN");
    EXPECT_EQ(figureEight.link.sig(), "eabcdbadcvbZa");
    EXPECT_EQ(figureEight_r1x2.link.sig(), "gaabcdbeffdcevtBy");
    EXPECT_EQ(conway.link.sig(), "labcdbefcdghiefjkgaijkhRswfFoWa");
    EXPECT_EQ(kinoshitaTerasaka.link.sig(), "labcdefgahefhijbkdijckgBvQcndZl");
    EXPECT_EQ(gst.link.sig(), "WabcdefghijklmnopqrsetuvinwxohvyazABCDEpguFbzGBHIJqftKcAGCHLsrMIDNOPQjmRNOSTklUSPVyFKdLMJExwRUTQVFyCHZX4sCdFzZe6yV7-D5cCbUGXTBhbG");
    EXPECT_EQ(rht_rht.link.sig(), "gabcabcdefdefvv--");
    EXPECT_EQ(rht_lht.link.sig(), "gabcabcdefdefvv-a");

    // Add some hard-coded link signatures also, to ensure that nothing changes
    // as we optimise the underlying algorithms.  For now we can only work
    // with connected link diagrams, hence the fairly small list below.
    EXPECT_EQ(empty.link.sig(), "_");
    EXPECT_EQ(hopf.link.sig(), "cabcabjp"); // verified by hand
    EXPECT_EQ(whitehead.link.sig(), "fabcadefbcedvfpd"); // verified by hand
    EXPECT_EQ(borromean.link.sig(), "gabcdgaecfgbfdeLwto"); // verified by hand
    EXPECT_EQ(trefoil_unknot_overlap.link.sig(), "fabcdeadefbcxb7h");
    EXPECT_EQ(adams6_28.link.sig(), "gabcadefdgbcefvv--"); // verified by hand
}

static void verifyGaussAndDT(const TestCase& test,
        bool testGauss = true, bool testDT = true) {
    SCOPED_TRACE_CSTRING(test.name);
    ASSERT_EQ(test.link.countComponents(), 1);

    // For "non-composite-like" knot diagrams, the only possible ambiguity
    // is reflection.  Use the reflection-distinguishing knot signature to
    // tell whether we reflected upon reconstruction.
    std::string targetSig = test.link.knotSig(false);

    if (testGauss) {
        std::string code = test.link.gauss();

        Link recon;
        ASSERT_NO_THROW({ recon = Link::fromGauss(code); });

        EXPECT_EQ(recon.size(), test.link.size());
        EXPECT_EQ(recon.countComponents(), test.link.countComponents());

        // Verify the "magic" string constructor.
        EXPECT_NO_THROW({ EXPECT_EQ(Link(code), recon); });

        // If we reflected, undo this for our subsequent tests.
        if (recon.knotSig(false) != targetSig)
            recon.reflect();

        EXPECT_EQ(recon.knotSig(false), targetSig);
        if (test.link.size() <= JONES_THRESHOLD)
            EXPECT_EQ(recon.jones(), test.link.jones());
    }

    if (testDT) {
        for (int alpha = 0; alpha <= 1; ++alpha) {
            if (alpha && test.link.size() > 26) {
                EXPECT_THROW({ test.link.dt(alpha); }, regina::NotImplemented);
            } else {
                std::string code = test.link.dt(alpha);

                Link recon;
                ASSERT_NO_THROW({ recon = Link::fromDT(code); });

                EXPECT_EQ(recon.size(), test.link.size());
                EXPECT_EQ(recon.countComponents(), test.link.countComponents());

                // Verify the "magic" string constructor, _except_ for the
                // special case of the one-crossing unknot whose alphabetical
                // D-T code "a" is also a valid knot signature.
                EXPECT_NO_THROW({
                    if (alpha && test.link.size() == 1)
                        EXPECT_EQ(Link(code), Link(1));
                    else
                        EXPECT_EQ(Link(code), recon);
                });

                // If we reflected, undo this for our subsequent tests.
                if (recon.knotSig(false) != targetSig)
                    recon.reflect();

                EXPECT_EQ(recon.knotSig(false), targetSig);
                if (test.link.size() <= JONES_THRESHOLD)
                    EXPECT_EQ(recon.jones(), test.link.jones());
            }
        }
    }
}

TEST_F(LinkTest, gaussAndDT) {
    // We test Gauss and D-T codes together, since they both have the same
    // ambiguity with knot composition.

    // The following cases are precisely those single-component knots without
    // ambiguous Gauss/D-T codes.
    verifyGaussAndDT(unknot0);
    verifyGaussAndDT(unknot1);
    verifyGaussAndDT(unknot3);
    verifyGaussAndDT(unknotMonster);
    verifyGaussAndDT(unknotGordian);

    verifyGaussAndDT(trefoilLeft);
    verifyGaussAndDT(trefoilRight);
    verifyGaussAndDT(figureEight);
    verifyGaussAndDT(conway);
    verifyGaussAndDT(kinoshitaTerasaka);
    verifyGaussAndDT(gst);

    // The following cases _should_ be ambiguous, but by sheer luck work with
    // at least one of our codes:
    verifyGaussAndDT(trefoil_r1x2, true, false);
    verifyGaussAndDT(figureEight_r1x2, true, false);
    verifyGaussAndDT(rht_rht, true, false);
    verifyGaussAndDT(rht_lht, false, true);
}

static void verifyOrientedGauss(const Link& link, const char* name) {
    if (link.countComponents() != 1)
        return;

    SCOPED_TRACE_CSTRING(name);

    std::string code = link.orientedGauss();

    Link recon;
    ASSERT_NO_THROW({ recon = Link::fromOrientedGauss(code); });

    // Oriented gauss codes reconstruct the labelling precisely.
    EXPECT_EQ(recon, link);
    EXPECT_EQ(recon.orientedGauss(), code);

    // Verify the "magic" string constructor.
    EXPECT_NO_THROW({ EXPECT_EQ(Link(code), recon); });
}

TEST_F(LinkTest, orientedGauss) {
    testManualCases(verifyOrientedGauss);
}

static void verifyJenkins(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    std::string code = link.jenkins();

    Link recon;
    ASSERT_NO_THROW({ recon = Link::fromJenkins(code); });

    // Jenkins format reconstructs the labelling precisely.
    EXPECT_EQ(recon, link);
    EXPECT_EQ(recon.jenkins(), code);
}

TEST_F(LinkTest, jenkins) {
    testManualCases(verifyJenkins);
}

static void verifyPDCode(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    // The PD code will throw away zero-crossing components.
    // Count how many of these we are expecting to lose.
    size_t lost = 0;
    for (auto c : link.components())
        if (! c)
            ++lost;

    std::string code = link.pd();

    Link recon;
    ASSERT_NO_THROW({ recon = Link::fromPD(code); });

    EXPECT_EQ(recon.size(), link.size());
    EXPECT_EQ(recon.countComponents() + lost, link.countComponents());
    EXPECT_EQ(recon.writhe(), link.writhe());
    EXPECT_EQ(recon.linking(), link.linking());

    {
        size_t i = 0;
        size_t j = 0;
        while (true) {
            // Skip past zero-crossing unknot components in link.
            while (j < link.countComponents() && ! link.component(j))
                ++j;

            if (i == recon.countComponents() ||
                    j == link.countComponents()) {
                // Both iterations should finish together.
                EXPECT_EQ(i, recon.countComponents());
                EXPECT_EQ(j, link.countComponents());
                break;
            }

            EXPECT_EQ(recon.writheOfComponent(i),
                link.writheOfComponent(j));

            ++i;
            ++j;
        }
    }

    if (link.size() == 0) {
        // The reconstructed link should be empty.
        EXPECT_EQ(recon.countComponents(), 0);
    } else if (link.size() <= JONES_THRESHOLD) {
        // The reconstructed link should have jones() != 0.
        // Check that jones() is what it should be.
        auto jones = recon.jones();
        if (lost) {
            // Multiply jones by (-t - t^-1)^lost.
            const regina::Laurent<regina::Integer> f(-1, { -1, 0, -1 });
            for (size_t i = 0; i < lost; ++i)
                jones *= f;
        }
        EXPECT_EQ(jones, link.jones());
    }

    // Try building a code with extra separator symbols.
    {
        auto data = link.pdData();

        std::ostringstream longCode;
        longCode << "PD[";
        int i = 0;
        for (const auto& tuple : data) {
            switch (i % 5) {
                case 0: longCode << " X(";
                case 1: longCode << " Xp {";
                case 2: longCode << " Xm[";
                case 3: longCode << " P[";
                default: longCode << ' ';
            }
            ++i;

            for (auto k : tuple)
                longCode << ' ' << k << ",\t";
            longCode << "), ";
        }
        longCode << "] ";

        EXPECT_NO_THROW({ EXPECT_EQ(Link::fromPD(longCode.str()), recon); });
    }

    // Verify the "magic" string constructor.
    EXPECT_NO_THROW({ EXPECT_EQ(Link(code), recon); });
}

TEST_F(LinkTest, pdCode) {
    testManualCases(verifyPDCode);
}

TEST_F(LinkTest, invalidCode) {
    static const char* code = "INVALID";

    EXPECT_THROW({ Link::fromKnotSig(code); }, regina::InvalidArgument);
    EXPECT_THROW({ Link::fromGauss(code); }, regina::InvalidArgument);
    EXPECT_THROW({ Link::fromDT(code); }, regina::InvalidArgument);
    EXPECT_THROW({ Link::fromOrientedGauss(code); }, regina::InvalidArgument);
    EXPECT_THROW({ Link::fromJenkins(code); }, regina::InvalidArgument);
    EXPECT_THROW({ Link::fromPD(code); }, regina::InvalidArgument);

    // Finally, the "magic" constructor:
    EXPECT_THROW({ Link l(code); }, regina::InvalidArgument);
}

static void verifyRewrite(const TestCase& test, int height, int threads,
        bool track, size_t count) {
    // For now, this should only be called for knots that are
    // equivalent to their mirror image.
    SCOPED_TRACE_CSTRING(test.name);
    SCOPED_TRACE_NUMERIC(height);
    SCOPED_TRACE_NUMERIC(threads);

    size_t tot = 0;
    auto jones = test.link.jones();

    std::unique_ptr<regina::ProgressTrackerOpen> tracker;
    if (track)
        tracker.reset(new regina::ProgressTrackerOpen());

    bool result = test.link.rewrite(height, threads, tracker.get(),
            [&tot, &jones](const Link& alt) {
        ++tot;
        EXPECT_EQ(alt.jones(), jones);
        return false;
    });
    if (track)
        EXPECT_TRUE(tracker->isFinished());
    EXPECT_FALSE(result);
    EXPECT_EQ(tot, count);
}

static void verifyRewrite(const TestCase& test, int height, size_t count) {
    // Single-threaded, no tracker:
    verifyRewrite(test, height, 1, false, count);
    // Multi-threaded, with and without tracker:
    verifyRewrite(test, height, 2, false, count);
    verifyRewrite(test, height, 2, true, count);
}

TEST_F(LinkTest, rewrite) {
    // The counts here were computed using Regina 6.0 in single-threaded mode
    // (except for the zero-crossing cases, which were computed by hand).

    verifyRewrite(empty, 0, 1);
    verifyRewrite(empty, 1, 1);
    verifyRewrite(empty, 2, 1);
    verifyRewrite(unknot0, 0, 1);
    verifyRewrite(unknot0, 1, 2);
    verifyRewrite(unknot0, 2, 6);
    verifyRewrite(unknot0, 3, 22);
    verifyRewrite(unknot3, 0, 22);
    verifyRewrite(unknot3, 1, 131);
    verifyRewrite(unknot3, 2, 998);
    verifyRewrite(unlink2_0, 0, 1);
    verifyRewrite(unlink2_0, 1, 2);
    verifyRewrite(unlink2_0, 2, 9);
    verifyRewrite(unlink3_0, 0, 1);
    verifyRewrite(unlink3_0, 1, 2);
    verifyRewrite(unlink3_0, 2, 9);
    verifyRewrite(figureEight, 0, 1);
    verifyRewrite(figureEight, 1, 8);
    verifyRewrite(figureEight, 2, 137);
    verifyRewrite(figureEight_r1x2, 0, 137);
    verifyRewrite(figureEight, 3, 2401);
    verifyRewrite(figureEight_r1x2, 1, 2401);
    // verifyRewrite(figureEight_r1x2, 2, 44985);
    verifyRewrite(rht_lht, 0, 1);
    verifyRewrite(rht_lht, 1, 35);
    verifyRewrite(rht_lht, 2, 1131);
}

static void verifyGroup(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    regina::GroupPresentation fromLink = link.group();
    regina::GroupPresentation fromComp = link.complement().group();

    EXPECT_EQ(fromLink.abelianRank(), link.countComponents());

    // Verify (as far as possible) that the two groups are isomorphic.
    // We assume here that both groups have been simplified.

    if (fromLink.countGenerators() <= 1 || fromComp.countGenerators() <= 1 ||
            fromLink.countRelations() == 0 || fromComp.countRelations() == 0) {
        // For trivial, cyclic or free groups, we expect both groups to
        // simplify to the same canonical presentation.
        EXPECT_EQ(fromLink.countGenerators(), fromComp.countGenerators());
        EXPECT_EQ(fromLink.relations(), fromComp.relations());
    } else {
        // Both groups have ≥ 2 generators and ≥ 1 relation.
        // We can't reliably test isomorphism, but we *can* reliably test
        // abelian invariants and low-index covers.
        EXPECT_EQ(fromLink.abelianisation(), fromComp.abelianisation());

        regina::for_constexpr<2, 6>([&fromLink, &fromComp](auto index) {
            SCOPED_TRACE_NUMERIC(index);

            std::vector<std::string> coversLink;
            fromLink.enumerateCovers<index>([&coversLink](
                    const regina::GroupPresentation& c) {
                coversLink.push_back(c.abelianisation().str());
            });
            std::sort(coversLink.begin(), coversLink.end());

            std::vector<std::string> coversComp;
            fromComp.enumerateCovers<index>([&coversComp](
                    const regina::GroupPresentation& c) {
                coversComp.push_back(c.abelianisation().str());
            });
            std::sort(coversComp.begin(), coversComp.end());

            EXPECT_EQ(coversLink, coversComp);
        });
    }
}

TEST_F(LinkTest, group) {
    testManualCases(verifyGroup, false);
}

static void verifySmallCells(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    const regina::ModelLinkGraph graph(link);
    if (! graph.isConnected()) {
        // We cannot build the dual cell decomposition.
        // Don't test this link.
        return;
    }
    const auto& cells = graph.cells();

    // Verify that loops(), bigons() and triangles() match what we see from
    // the dual cell decomposition.
    for (auto n : graph.nodes()) {
        SCOPED_TRACE_NUMERIC(n->index());

        int foundLoops = 0;
        int foundBigons = 0;
        int foundTriangles = 0;

        for (int i = 0; i < 4; ++i) {
            size_t cell = cells.cell(n->arc(i));
            switch (cells.size(cell)) {
                case 1:
                    ++foundLoops;
                    break;
                case 2:
                    {
                        auto n0 = cells.arc(cell, 0).node();
                        auto n1 = cells.arc(cell, 1).node();
                        if (n0 != n1)
                            ++foundBigons;
                    }
                    break;
                case 3:
                    {
                        auto n0 = cells.arc(cell, 0).node();
                        auto n1 = cells.arc(cell, 1).node();
                        auto n2 = cells.arc(cell, 2).node();
                        if (n0 != n1 && n0 != n2 && n1 != n2)
                            ++foundTriangles;
                    }
                    break;
                default:
                    break;
            }
        }

        EXPECT_EQ(foundLoops, n->loops());
        EXPECT_EQ(foundBigons, n->bigons());
        EXPECT_EQ(foundTriangles, n->triangles());
    }
}

TEST_F(LinkTest, smallCells) {
    testManualCases(verifySmallCells);
}

TEST_F(LinkTest, swapping) {
    Link a = ExampleLink::trefoilLeft();
    Link b = ExampleLink::figureEight();

    a.jones();
    b.jones();

    // Verify that crossings and properties are being swapped correctly.

    swap(a, b);
    EXPECT_EQ(a.size(), 4);
    EXPECT_EQ(a.jones(), figureEight.link.jones());

    std::iter_swap(&a, &b);
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a.jones(), trefoilLeft.link.jones());
}

static bool looksIdentical(const Link& a, const Link& b) {
    if (a.size() != b.size())
        return false;
    if (a.countComponents() != b.countComponents())
        return false;
    if (a.brief() != b.brief())
        return false;
    return true;
}

void verifyCopyMove(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    if (link.size() == 0) {
        Crossing* c0 = nullptr;

        Link copy(link);
        EXPECT_EQ(copy.size(), 0);
        EXPECT_TRUE(looksIdentical(copy, link));

        Link move(std::move(copy));
        EXPECT_EQ(move.size(), 0);
        EXPECT_TRUE(looksIdentical(move, link));

        Link copyAss(2); // A two-component unlink
        copyAss = link;
        EXPECT_EQ(copyAss.size(), 0);
        EXPECT_TRUE(looksIdentical(copyAss, link));

        Link moveAss(2); // A two-component unlink
        moveAss = std::move(copyAss);
        EXPECT_EQ(moveAss.size(), 0);
        EXPECT_TRUE(looksIdentical(moveAss, link));
    } else {
        Crossing* c0 = link.crossing(0);

        Link copy(link);
        EXPECT_TRUE(looksIdentical(copy, link));

        // Copy construction should use different crossings.
        ASSERT_GT(copy.size(), 0);
        Crossing* c1 = copy.crossing(0);
        EXPECT_NE(c1, c0);

        Link move(std::move(copy));
        EXPECT_TRUE(looksIdentical(move, link));

        // Move construction should use the same crossings.
        ASSERT_GT(move.size(), 0);
        Crossing* c2 = move.crossing(0);
        EXPECT_EQ(c2, c1);

        Link copyAss(2); // A two-component unlink
        copyAss = link;
        EXPECT_TRUE(looksIdentical(copyAss, link));

        // Copy assignment should use different crossings.
        ASSERT_GT(copyAss.size(), 0);
        Crossing* c3 = copyAss.crossing(0);
        EXPECT_NE(c3, c0);

        Link moveAss(2); // A two-component unlink
        moveAss = std::move(copyAss);
        EXPECT_TRUE(looksIdentical(moveAss, link));

        // Move assignment should use the same crossings.
        ASSERT_GT(moveAss.size(), 0);
        Crossing* c4 = moveAss.crossing(0);
        EXPECT_EQ(c4, c3);
    }
}

TEST_F(LinkTest, copyMove) {
    testManualCases(verifyCopyMove);
}

static void verifyTightEncoding(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);
    TightEncodingTest<Link>::verifyTightEncoding(link);
}

TEST_F(LinkTest, tightEncoding) {
    testManualCases(verifyTightEncoding);
}
