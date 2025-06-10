
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

#include "census/census.h"
#include "link/examplelink.h"
#include "link/link.h"
#include "surface/normalsurfaces.h"

#include "testexhaustive.h"
#include "utilities/tightencodingtest.h"

using regina::Algorithm;
using regina::Crossing;
using regina::ExampleLink;
using regina::FailedPrecondition;
using regina::GroupPresentation;
using regina::InvalidArgument;
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
            "eLAkbbcddaikhc", "eLAkbbcddainqv", "eLAkbcbddducqn", "eLAkbcbdddmcxj" })
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

static void verifyTopologicallySame(const Link& a, const Link& b) {
    // Used (for example) when testing Reidemeister moves.
    EXPECT_EQ(a.countComponents(), b.countComponents());
    EXPECT_EQ(a.linking2(), b.linking2());
    if (a.countComponents() == 1 && b.countComponents() == 1)
        EXPECT_EQ(a.oddWrithe(), b.oddWrithe());
    if (a.size() <= JONES_THRESHOLD && b.size() <= JONES_THRESHOLD)
        EXPECT_EQ(a.jones(), b.jones());
}

static void verifyIsomorphic(const GroupPresentation& a,
        const GroupPresentation& b, bool fastTestsOnly = false) {
    if (a.countGenerators() <= 1 || b.countGenerators() <= 1 ||
            a.countRelations() == 0 || b.countRelations() == 0) {
        // For trivial, cyclic or free groups, we expect Regina should be
        // able to simplify both groups to the same canonical presentation.
        EXPECT_EQ(a.countGenerators(), b.countGenerators());
        EXPECT_EQ(a.relations(), b.relations());
        return;
    }

    // Both groups have ≥ 2 generators and ≥ 1 relation.
    // In general we can't reliably test isomorphism, but we *can* reliably
    // test abelian invariants and low-index covers.
    EXPECT_EQ(a.abelianisation(), b.abelianisation());

    auto compareGroups = [&a, &b](auto index) {
        SCOPED_TRACE_NUMERIC(index);

        std::vector<std::string> coversA;
        a.enumerateCovers<index>([&coversA](const GroupPresentation& c) {
            coversA.push_back(c.abelianisation().str());
        });
        std::sort(coversA.begin(), coversA.end());

        std::vector<std::string> coversB;
        b.enumerateCovers<index>([&coversB](const GroupPresentation& c) {
            coversB.push_back(c.abelianisation().str());
        });
        std::sort(coversB.begin(), coversB.end());

        EXPECT_EQ(coversA, coversB);
    };

    if (fastTestsOnly || a.countGenerators() > 5 || b.countGenerators() > 5)
        regina::for_constexpr<2, 4>(compareGroups);
    else
        regina::for_constexpr<2, 6>(compareGroups);
}

static regina::Laurent<regina::Integer> jonesModReflection(const Link& link) {
    auto jones1 = link.jones();
    auto jones2 = jones1;
    jones2.invertX();
    if (jones1 <= jones2)
        return jones1;
    else
        return jones2;
}

static Link addTrivialComponents(const Link& link, size_t nTrivial) {
    Link ans(link);
    ans.insertLink(Link(nTrivial));
    return ans;
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

        // Virtual knots:
        TestCase virtualTrefoil { ExampleLink::virtualTrefoil(),
            "Virtual trefoil" };
        TestCase kishino { ExampleLink::kishino(), "Kishino virtual knot" };
        TestCase gpv { ExampleLink::gpv(),
            "Goussarov-Polyak-Viro virtual knot" };

        // Virtual multiple-component links:
        TestCase virtualLink2 {
            Link::fromData({ +1 }, { 1 }, { -1 }),
            "1-crossing, 2-component virtual link" };
        TestCase virtualLink3 {
            Link::fromData({ +1, +1 }, { 1 }, { -2 }, { -1, 2 }),
            "2-crossing, 3-component virtual link" };
        TestCase virtualTrefoilx2 { ExampleLink::virtualTrefoil().parallel(2),
            "Parallel virtual trefoils" };

        // A virtual disconnected diagram, constructed as
        // (virtualLink3 U virtualTrefoil U hopf):
        TestCase virtualDisconnected {
            Link::fromData(
                { +1, +1, +1, +1, +1, +1 },
                { 1 }, { -2 }, { -1, 2 },
                { 3, -4, -3, 4 },
                { 5, -6 }, { -5, 6 }),
            "Disconnected virtual link diagram" };

        /**
         * Run the given test over all of the example links stored in
         * this test fixture.
         */
        void testManualCases(void (*f)(const Link&, const char*),
                bool includeGordian = true, bool includeVirtual = true) {
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

            if (includeVirtual) {
                f(virtualTrefoil.link, virtualTrefoil.name);
                f(kishino.link, kishino.name);
                f(gpv.link, gpv.name);
                f(virtualLink2.link, virtualLink2.name);
                f(virtualLink3.link, virtualLink3.name);
                f(virtualTrefoilx2.link, virtualTrefoilx2.name);
                f(virtualDisconnected.link, virtualDisconnected.name);
            }
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
    EXPECT_TRUE(adams6_28.link.isConnected());
    EXPECT_TRUE(adams6_28.link.graph().isConnected());

    EXPECT_TRUE(virtualTrefoil.link.isConnected());
    EXPECT_TRUE(virtualTrefoil.link.graph().isConnected());
    EXPECT_TRUE(kishino.link.isConnected());
    EXPECT_TRUE(kishino.link.graph().isConnected());
    EXPECT_TRUE(gpv.link.isConnected());
    EXPECT_TRUE(gpv.link.graph().isConnected());
    EXPECT_TRUE(virtualLink2.link.isConnected());
    EXPECT_TRUE(virtualLink2.link.graph().isConnected());
    EXPECT_TRUE(virtualLink3.link.isConnected());
    EXPECT_TRUE(virtualLink3.link.graph().isConnected());
    EXPECT_TRUE(virtualTrefoilx2.link.isConnected());
    EXPECT_TRUE(virtualTrefoilx2.link.graph().isConnected());
    EXPECT_FALSE(virtualDisconnected.link.isConnected());
    EXPECT_FALSE(virtualDisconnected.link.graph().isConnected());

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
    EXPECT_EQ(adams6_28.link.countComponents(), 2);

    EXPECT_EQ(virtualTrefoil.link.countComponents(), 1);
    EXPECT_EQ(kishino.link.countComponents(), 1);
    EXPECT_EQ(gpv.link.countComponents(), 1);
    EXPECT_EQ(virtualLink2.link.countComponents(), 2);
    EXPECT_EQ(virtualLink3.link.countComponents(), 3);
    EXPECT_EQ(virtualTrefoilx2.link.countComponents(), 2);
    EXPECT_EQ(virtualDisconnected.link.countComponents(), 6);
}

static void verifyVirtualGenus(const TestCase& test, size_t expect) {
    SCOPED_TRACE_CSTRING(test.name);

    EXPECT_EQ(test.link.virtualGenus(), expect);
    EXPECT_EQ(regina::ModelLinkGraph(test.link).genus(), expect);
    if (expect == 0)
        EXPECT_TRUE(test.link.isClassical());
    else
        EXPECT_FALSE(test.link.isClassical());
}

TEST_F(LinkTest, virtualGenus) {
    verifyVirtualGenus(empty, 0);

    verifyVirtualGenus(unknot0, 0);
    verifyVirtualGenus(unknot1, 0);
    verifyVirtualGenus(unknot3, 0);
    verifyVirtualGenus(unknotMonster, 0);
    verifyVirtualGenus(unknotGordian, 0);

    verifyVirtualGenus(trefoilLeft, 0);
    verifyVirtualGenus(trefoilRight, 0);
    verifyVirtualGenus(trefoil_r1x2, 0);
    verifyVirtualGenus(trefoil_r1x6, 0);
    verifyVirtualGenus(figureEight, 0);
    verifyVirtualGenus(figureEight_r1x2, 0);
    verifyVirtualGenus(conway, 0);
    verifyVirtualGenus(kinoshitaTerasaka, 0);
    verifyVirtualGenus(gst, 0);

    verifyVirtualGenus(rht_rht, 0);
    verifyVirtualGenus(rht_lht, 0);

    verifyVirtualGenus(unlink2_0, 0);
    verifyVirtualGenus(unlink3_0, 0);
    verifyVirtualGenus(unlink2_r2, 0);
    verifyVirtualGenus(unlink2_r1r1, 0);
    verifyVirtualGenus(hopf, 0);
    verifyVirtualGenus(whitehead, 0);
    verifyVirtualGenus(borromean, 0);
    verifyVirtualGenus(trefoil_unknot0, 0);
    verifyVirtualGenus(trefoil_unknot1, 0);
    verifyVirtualGenus(trefoil_unknot_overlap, 0);
    verifyVirtualGenus(adams6_28, 0);

    verifyVirtualGenus(virtualTrefoil, 1);
    verifyVirtualGenus(kishino, 2);
    verifyVirtualGenus(gpv, 1);
    verifyVirtualGenus(virtualLink2, 1);
    verifyVirtualGenus(virtualLink3, 1);
    verifyVirtualGenus(virtualTrefoilx2, 1);
    verifyVirtualGenus(virtualDisconnected, 2);
}

static void verifyDiagramComponents(const Link& link, const char* name,
        std::initializer_list<std::string> expectBrief,
        std::initializer_list<size_t> expectIndices) {
    SCOPED_TRACE_CSTRING(name);

    size_t totalGenus = link.virtualGenus();

    auto foundComponents = link.diagramComponents();
    auto foundIndices = link.diagramComponentIndices();

    EXPECT_EQ(foundComponents.size(), expectBrief.size());
    EXPECT_EQ(foundIndices.second,
        expectBrief.size() - link.countTrivialComponents());
    EXPECT_EQ(foundComponents.size(), link.countDiagramComponents());

    auto found = foundComponents.begin();
    auto expect = expectBrief.begin();
    for ( ; found != foundComponents.end() && expect != expectBrief.end();
            ++found, ++expect)
        EXPECT_EQ(found->brief(), *expect);

    EXPECT_EQ(foundIndices.first.size(), link.size());
    auto expectIndex = expectIndices.begin();
    for (size_t crossing = 0;
            crossing < link.size() && expectIndex != expectIndices.end();
            ++crossing, ++expectIndex) {
        SCOPED_TRACE_NUMERIC(crossing);
        EXPECT_EQ(foundIndices.first[crossing], *expectIndex);
    }

    size_t foundGenus = 0;
    for (const Link& c : foundComponents)
        foundGenus += c.virtualGenus();
    EXPECT_EQ(totalGenus, foundGenus);
}

TEST_F(LinkTest, diagramComponents) {
    // Just test a few things manually.
    verifyDiagramComponents(empty.link, empty.name, {}, {});
    verifyDiagramComponents(unknot0.link, unknot0.name, { "( )" }, {});
    verifyDiagramComponents(unlink2_0.link, unlink2_0.name,
        { "( )",
          "( )" },
        { });
    verifyDiagramComponents(figureEight.link, figureEight.name,
        { "++-- ( _0 ^1 _2 ^3 _1 ^0 _3 ^2 )" },
        { 0, 0, 0, 0 });
    verifyDiagramComponents(whitehead.link, whitehead.name,
        { "--++- ( ^0 _1 ^4 _3 ^2 _4 ) ( _0 ^1 _2 ^3 )" },
        { 0, 0, 0, 0, 0 });
    verifyDiagramComponents(trefoil_unknot0.link, trefoil_unknot0.name,
        { "+++ ( ^0 _1 ^2 _0 ^1 _2 )",
          "( )" },
        { 0, 0, 0 });
    verifyDiagramComponents(trefoil_unknot1.link, trefoil_unknot1.name,
        { "+++ ( ^0 _1 ^2 _0 ^1 _2 )",
          "- ( _0 ^0 )" },
        { 0, 0, 1, 0 });
    verifyDiagramComponents(trefoil_unknot_overlap.link,
        trefoil_unknot_overlap.name,
        { "-++++ ( ^1 _2 _3 _0 ^4 _1 ^2 _4 ) ( ^3 ^0 )" },
        { 0, 0, 0, 0, 0 });
    verifyDiagramComponents(virtualLink2.link, virtualLink2.name,
        { "+ ( ^0 ) ( _0 )" },
        { 0, 0 });
    verifyDiagramComponents(virtualLink3.link, virtualLink3.name,
        { "++ ( ^0 ) ( _1 ) ( _0 ^1 )" },
        { 0, 0 });
    verifyDiagramComponents(virtualDisconnected.link, virtualLink3.name,
        { "++ ( ^0 ) ( _1 ) ( _0 ^1 )",
          "++ ( ^0 _1 _0 ^1 )",
          "++ ( ^0 _1 ) ( _0 ^1 )" },
        { 0, 0, 1, 1, 2, 2 });
    {
        Link link = ExampleLink::whitehead();
        link.insertLink(Link(2));
        link.insertLink(ExampleLink::figureEight());
        link.insertLink(Link(1));
        verifyDiagramComponents(link, "Whitehead U Figure_Eight U 3x()",
            { "--++- ( ^0 _1 ^4 _3 ^2 _4 ) ( _0 ^1 _2 ^3 )",
              "++-- ( _0 ^1 _2 ^3 _1 ^0 _3 ^2 )",
              "( )",
              "( )",
              "( )" },
            { 0, 0, 0, 0, 0, 1, 1, 1, 1 });
    }
}

TEST_F(LinkTest, isAlternating) {
    EXPECT_TRUE(empty.link.isAlternating());

    EXPECT_TRUE(unknot0.link.isAlternating());
    EXPECT_TRUE(unknot1.link.isAlternating());
    EXPECT_FALSE(unknot3.link.isAlternating());
    EXPECT_FALSE(unknotMonster.link.isAlternating());
    EXPECT_FALSE(unknotGordian.link.isAlternating());

    EXPECT_TRUE(trefoilLeft.link.isAlternating());
    EXPECT_TRUE(trefoilRight.link.isAlternating());
    EXPECT_FALSE(trefoil_r1x2.link.isAlternating());
    EXPECT_FALSE(trefoil_r1x6.link.isAlternating());
    EXPECT_TRUE(figureEight.link.isAlternating());
    EXPECT_FALSE(figureEight_r1x2.link.isAlternating());
    EXPECT_FALSE(conway.link.isAlternating());
    EXPECT_FALSE(kinoshitaTerasaka.link.isAlternating());
    EXPECT_FALSE(gst.link.isAlternating());

    EXPECT_TRUE(rht_rht.link.isAlternating());
    EXPECT_TRUE(rht_lht.link.isAlternating());

    EXPECT_TRUE(unlink2_0.link.isAlternating());
    EXPECT_TRUE(unlink3_0.link.isAlternating());
    EXPECT_FALSE(unlink2_r2.link.isAlternating());
    EXPECT_TRUE(unlink2_r1r1.link.isAlternating());
    EXPECT_TRUE(hopf.link.isAlternating());
    EXPECT_TRUE(whitehead.link.isAlternating());
    EXPECT_TRUE(borromean.link.isAlternating());
    EXPECT_TRUE(trefoil_unknot0.link.isAlternating());
    EXPECT_TRUE(trefoil_unknot1.link.isAlternating());
    EXPECT_FALSE(trefoil_unknot_overlap.link.isAlternating());
    EXPECT_TRUE(adams6_28.link.isAlternating());

    EXPECT_FALSE(virtualTrefoil.link.isAlternating());
    EXPECT_FALSE(kishino.link.isAlternating());
    EXPECT_FALSE(gpv.link.isAlternating());
    EXPECT_FALSE(virtualLink2.link.isAlternating());
    EXPECT_FALSE(virtualLink3.link.isAlternating());
    EXPECT_FALSE(virtualTrefoilx2.link.isAlternating());
    EXPECT_FALSE(virtualDisconnected.link.isAlternating());
}

static void verifyMakeAlternating(const TestCase& test, bool isPossible) {
    SCOPED_TRACE_CSTRING(test.name);

    Link alt(test.link, false);
    bool success = alt.makeAlternating();

    if (test.link.isAlternating()) {
        EXPECT_TRUE(success);
        EXPECT_EQ(alt, test.link);
    } else if (isPossible) {
        EXPECT_TRUE(success);
        EXPECT_NE(alt, test.link);
        EXPECT_TRUE(alt.isAlternating());
        EXPECT_EQ(alt.size(), test.link.size());

        // The model graph may change labelling, because the ordering of arcs
        // is based on which strands are over/under at each crossing.
        EXPECT_EQ(alt.graph().canonical(false /* no reflections */),
            test.link.graph().canonical(false /* no reflections */));
    } else {
        EXPECT_FALSE(success);
        EXPECT_EQ(alt, test.link);
    }
}

TEST_F(LinkTest, makeAlternating) {
    verifyMakeAlternating(empty, true);

    verifyMakeAlternating(unknot0, true);
    verifyMakeAlternating(unknot1, true);
    verifyMakeAlternating(unknot3, true);
    verifyMakeAlternating(unknotMonster, true);
    verifyMakeAlternating(unknotGordian, true);

    verifyMakeAlternating(trefoilLeft, true);
    verifyMakeAlternating(trefoilRight, true);
    verifyMakeAlternating(trefoil_r1x2, true);
    verifyMakeAlternating(trefoil_r1x6, true);
    verifyMakeAlternating(figureEight, true);
    verifyMakeAlternating(figureEight_r1x2, true);
    verifyMakeAlternating(conway, true);
    verifyMakeAlternating(kinoshitaTerasaka, true);
    verifyMakeAlternating(gst, true);

    verifyMakeAlternating(rht_rht, true);
    verifyMakeAlternating(rht_lht, true);

    verifyMakeAlternating(unlink2_0, true);
    verifyMakeAlternating(unlink3_0, true);
    verifyMakeAlternating(unlink2_r2, true);
    verifyMakeAlternating(unlink2_r1r1, true);
    verifyMakeAlternating(hopf, true);
    verifyMakeAlternating(whitehead, true);
    verifyMakeAlternating(borromean, true);
    verifyMakeAlternating(trefoil_unknot0, true);
    verifyMakeAlternating(trefoil_unknot1, true);
    verifyMakeAlternating(trefoil_unknot_overlap, true);
    verifyMakeAlternating(adams6_28, true);

    verifyMakeAlternating(virtualTrefoil, false);
    verifyMakeAlternating(kishino, false);
    verifyMakeAlternating(gpv, false);
    verifyMakeAlternating(virtualLink2, false);
    verifyMakeAlternating(virtualLink3, false);
    verifyMakeAlternating(virtualTrefoilx2, true);
    verifyMakeAlternating(virtualDisconnected, false);
}

static void verifyLinking(const TestCase& test, long expect) {
    SCOPED_TRACE_CSTRING(test.name);
    EXPECT_EQ(test.link.linking(), expect);
    EXPECT_EQ(test.link.linking2(), 2 * expect);
}

static void verifyOnlyLinking2(const TestCase& test, long expect) {
    SCOPED_TRACE_CSTRING(test.name);
    EXPECT_NE(expect % 2, 0);
    EXPECT_THROW({ test.link.linking(); }, regina::NotImplemented);
    EXPECT_EQ(test.link.linking2(), expect);
}

TEST_F(LinkTest, linking) {
    verifyLinking(empty, 0);

    verifyLinking(unknot0, 0);
    verifyLinking(unknot1, 0);
    verifyLinking(unknot3, 0);
    verifyLinking(unknotMonster, 0);
    verifyLinking(unknotGordian, 0);

    verifyLinking(trefoilLeft, 0);
    verifyLinking(trefoilRight, 0);
    verifyLinking(trefoil_r1x2, 0);
    verifyLinking(trefoil_r1x6, 0);
    verifyLinking(figureEight, 0);
    verifyLinking(figureEight_r1x2, 0);
    verifyLinking(conway, 0);
    verifyLinking(kinoshitaTerasaka, 0);
    verifyLinking(gst, 0);

    verifyLinking(rht_rht, 0);
    verifyLinking(rht_lht, 0);

    verifyLinking(unlink2_0, 0);
    verifyLinking(unlink3_0, 0);
    verifyLinking(unlink2_r2, 0);
    verifyLinking(unlink2_r1r1, 0);
    verifyLinking(hopf, 1);
    verifyLinking(whitehead, 0);
    verifyLinking(borromean, 0);
    verifyLinking(trefoil_unknot0, 0);
    verifyLinking(trefoil_unknot1, 0);
    verifyLinking(trefoil_unknot_overlap, 0);
    verifyLinking(adams6_28, 2);

    verifyLinking(virtualTrefoil, 0);
    verifyLinking(kishino, 0);
    verifyLinking(gpv, 0);
    verifyOnlyLinking2(virtualLink2, 1);
    verifyLinking(virtualLink3, 1);
    verifyLinking(virtualTrefoilx2, 0);
    verifyLinking(virtualDisconnected, 2);
}

static void verifyWrithe(const TestCase& test, long expectWrithe,
        std::initializer_list<long> expectComponents) {
    SCOPED_TRACE_CSTRING(test.name);

    EXPECT_EQ(test.link.writhe(), expectWrithe);

    long sum = 0;
    auto comp = 0;
    auto it = expectComponents.begin();
    while (comp < test.link.countComponents() && it != expectComponents.end()) {
        EXPECT_EQ(test.link.writheOfComponent(comp), *it);
        sum += *it;
        ++comp;
        ++it;
    }
    EXPECT_EQ(comp, test.link.countComponents());
    EXPECT_TRUE(it == expectComponents.end());
    EXPECT_EQ(sum + test.link.linking2(), test.link.writhe());
}

TEST_F(LinkTest, writhe) {
    verifyWrithe(empty, 0, {});

    verifyWrithe(unknot0, 0, {0});
    verifyWrithe(unknot1, 1, {1});
    verifyWrithe(unknot3, 1, {1});
    verifyWrithe(unknotMonster, 2, {2});
    verifyWrithe(unknotGordian, -33, {-33});

    verifyWrithe(trefoilLeft, -3, {-3});
    verifyWrithe(trefoilRight, 3, {3});
    verifyWrithe(trefoil_r1x2, 3, {3});
    verifyWrithe(trefoil_r1x6, 3, {3});
    verifyWrithe(figureEight, 0, {0});
    verifyWrithe(figureEight_r1x2, 0, {0});
    verifyWrithe(conway, -1, {-1});
    verifyWrithe(kinoshitaTerasaka, -1, {-1});
    verifyWrithe(gst, 2, {2});

    verifyWrithe(rht_rht, 6, {6});
    verifyWrithe(rht_lht, 0, {0});

    verifyWrithe(unlink2_0, 0, {0,0});
    verifyWrithe(unlink3_0, 0, {0,0,0});
    verifyWrithe(unlink2_r2, 0, {0,0});
    verifyWrithe(unlink2_r1r1, 0, {-1,1});
    verifyWrithe(hopf, 2, {0,0});
    verifyWrithe(whitehead, -1, {-1,0});
    verifyWrithe(borromean, 0, {0,0,0});
    verifyWrithe(trefoil_unknot0, 3, {3,0});
    verifyWrithe(trefoil_unknot1, 2, {3,-1});
    verifyWrithe(trefoil_unknot_overlap, 3, {3,0});
    verifyWrithe(adams6_28, 2, {0,-2});

    verifyWrithe(virtualTrefoil, 2, {2});
    verifyWrithe(kishino, 0, {0});
    verifyWrithe(gpv, -4, {-4});
    verifyWrithe(virtualLink2, 1, {0,0});
    verifyWrithe(virtualLink3, 2, {0,0,0});
    verifyWrithe(virtualTrefoilx2, 4, {2, 2});
    verifyWrithe(virtualDisconnected, 6, {0,0,0,2,0,0});
}

TEST_F(LinkTest, oddWrithe) {
    // Classical knots:
    EXPECT_EQ(unknot0.link.oddWrithe(), 0);
    EXPECT_EQ(unknot1.link.oddWrithe(), 0);
    EXPECT_EQ(unknot3.link.oddWrithe(), 0);
    EXPECT_EQ(unknotMonster.link.oddWrithe(), 0);
    EXPECT_EQ(unknotGordian.link.oddWrithe(), 0);

    EXPECT_EQ(trefoilLeft.link.oddWrithe(), 0);
    EXPECT_EQ(trefoilRight.link.oddWrithe(), 0);
    EXPECT_EQ(trefoil_r1x2.link.oddWrithe(), 0);
    EXPECT_EQ(trefoil_r1x6.link.oddWrithe(), 0);
    EXPECT_EQ(figureEight.link.oddWrithe(), 0);
    EXPECT_EQ(figureEight_r1x2.link.oddWrithe(), 0);
    EXPECT_EQ(conway.link.oddWrithe(), 0);
    EXPECT_EQ(kinoshitaTerasaka.link.oddWrithe(), 0);
    EXPECT_EQ(gst.link.oddWrithe(), 0);

    EXPECT_EQ(rht_rht.link.oddWrithe(), 0);
    EXPECT_EQ(rht_lht.link.oddWrithe(), 0);

    // Virtual knots:
    EXPECT_EQ(virtualTrefoil.link.oddWrithe(), 2);
    EXPECT_EQ(kishino.link.oddWrithe(), 0);
    EXPECT_EQ(gpv.link.oddWrithe(), -4);

    // Links with ≠ 1 component:
    EXPECT_THROW({ empty.link.oddWrithe(); }, FailedPrecondition);

    EXPECT_THROW({ unlink2_0.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ unlink3_0.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ unlink2_r2.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ unlink2_r1r1.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ hopf.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ whitehead.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ borromean.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ trefoil_unknot0.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ trefoil_unknot1.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ trefoil_unknot_overlap.link.oddWrithe(); },
        FailedPrecondition);
    EXPECT_THROW({ adams6_28.link.oddWrithe(); }, FailedPrecondition);

    EXPECT_THROW({ virtualLink2.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ virtualLink3.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ virtualTrefoilx2.link.oddWrithe(); }, FailedPrecondition);
    EXPECT_THROW({ virtualDisconnected.link.oddWrithe(); }, FailedPrecondition);
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

    verifyTopologicallySame(framed, link);
    for (size_t c = 0; c < framed.countComponents(); ++c)
        EXPECT_EQ(framed.writheOfComponent(c), 0);
}

TEST_F(LinkTest, selfFrame) {
    testManualCases(verifySelfFrame);
}

static void verifyWhiteheadDouble(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    if (link.countComponents() != 1) {
        EXPECT_THROW({ link.whiteheadDouble(); }, FailedPrecondition);
        return;
    }

    long writhe = link.writhe();

    Link pos = link.whiteheadDouble();
    Link neg = link.whiteheadDouble(false);

    EXPECT_EQ(pos.countComponents(), 1);
    EXPECT_EQ(neg.countComponents(), 1);
    EXPECT_EQ(pos.size(), 4 * link.size() + 2 * std::abs(writhe) + 2);
    EXPECT_EQ(neg.size(), 4 * link.size() + 2 * std::abs(writhe) + 2);
    EXPECT_EQ(pos.writhe(), 2 * writhe + 2);
    EXPECT_EQ(neg.writhe(), 2 * writhe - 2);

    if (link.size() < 20 && link.isClassical()) {
        using P = regina::Polynomial<regina::Integer>;
        EXPECT_EQ(pos.alexander(), regina::RingTraits<P>::one);
        EXPECT_EQ(neg.alexander(), regina::RingTraits<P>::one);
    }
}

TEST_F(LinkTest, whiteheadDouble) {
    testManualCases(verifyWhiteheadDouble);
}

static void verifyParallel(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    long writhe = link.writhe();
    long linking2 = link.linking2();

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
        EXPECT_EQ(p.linking2(), k * k * linking2 + k * (k-1) * writheSame);

        p = link.parallel(k, regina::Framing::Seifert);
        EXPECT_EQ(p.countComponents(), k * link.countComponents());
        EXPECT_EQ(p.size(), k * k * link.size() + k * (k-1) * absWritheSame);
        EXPECT_EQ(p.writhe(), k * k * writhe - k * (k-1) * writheSame);
        EXPECT_EQ(p.linking2(), k * k * linking2);
    }
}

TEST_F(LinkTest, parallel) {
    testManualCases(verifyParallel);
}

static void verifyAlexander(const TestCase& test,
        const regina::Polynomial<regina::Integer>& expected) {
    SCOPED_TRACE_CSTRING(test.name);

    // In case we later modify this code to compute the Alexander polynomial
    // multiple times using different algorithms, we will work with clones of
    // the link that do not clone any already-computed properties.

    EXPECT_EQ(Link(test.link, false).alexander(), expected);
}

TEST_F(LinkTest, alexander) {
    verifyAlexander(unknot0, {1});
    verifyAlexander(unknot1, {1});
    verifyAlexander(unknot3, {1});
    verifyAlexander(unknotMonster, {1});
    // Let's not attempt this with the (enormous) Gordian unknot.

    verifyAlexander(trefoilLeft, {1,-1,1});
    verifyAlexander(trefoilRight, {1,-1,1});
    verifyAlexander(trefoil_r1x2, {1,-1,1});
    verifyAlexander(trefoil_r1x6, {1,-1,1});
    verifyAlexander(figureEight, {1,-3,1});
    verifyAlexander(figureEight_r1x2, {1,-3,1});

    verifyAlexander(conway, {1});
    verifyAlexander(kinoshitaTerasaka, {1});
    verifyAlexander(gst, {1,-2,1,0,2,-5,2,7,-13,7,2,-5,2,0,1,-2,1});

    verifyAlexander(rht_rht, {1,-2,3,-2,1});
    verifyAlexander(rht_lht, {1,-2,3,-2,1});
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

static void verifyJonesConsistent(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);
    EXPECT_EQ(Link(link, false).jones(Algorithm::Treewidth),
        Link(link, false).jones(Algorithm::Naive));
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

    // This polynomial was computed using Regina 7.4 (and verified using
    // both algorithms).
    verifyJones(adams6_28, {-3, {-1,0,2,0,-2,0,2,0,-3,0,1,0,-1}});

    verifyJones(virtualTrefoil, {2, {1,1,0,-1}});
    verifyJones(kishino, {0, {1}});
    verifyJones(gpv, {-10, {1,0,-2,-2,1,2,1}});
    verifyJones(virtualLink2, {1, {-1,-1}});
    verifyJones(virtualLink3, {2, {1,2,1}});
    verifyJones(virtualDisconnected,
        {3, {-1,-3,-5,-6,-6,-5,-4,-4,-3,-1,1,2,2,1}});

    // This polynomial was computed using Regina 7.4 (and verified using
    // both algorithms).
    verifyJones(virtualTrefoilx2, {1, {-1,0,0,0,-2,0,-1,0,1,0,1,0,1,0,-1}});

    // Run through a small census and ensure that both algorithms give
    // the same Jones polynomial in both cases.
    runCensusAllVirtual(verifyJonesConsistent);

    // Check that the multithreaded naive algorithm gives the same answers as
    // the single-threaded treewidth algorithm.
    EXPECT_EQ(ExampleLink::borromean().jones(Algorithm::Naive, 4),
        ExampleLink::borromean().jones());
    EXPECT_EQ(ExampleLink::borromean().parallel(2).jones(Algorithm::Naive, 4),
        ExampleLink::borromean().parallel(2).jones());
}

static void verifyHomflyAZ(const TestCase& test,
        const regina::Laurent2<regina::Integer>& expected) {
    SCOPED_TRACE_CSTRING(test.name);

    // Since we are computing the HOMFLY-PT polynomial multiple times
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

    // Since we are computing the HOMFLY-PT polynomial multiple times
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

    // This is different from Adams' claim regarding the HOMFLY-PT polynomial of
    // this link.  But... Adams does get his arithmetic wrong elsewhere, and a
    // calculation by hand using the Skein relation agrees with the polynomial
    // below.
    verifyHomflyLM(adams6_28,
        {{1,1,1}, {-1,3,-1}, {-1,1,1}, {-3,1,2}, {-3,-1,-1}, {-5,-1,-1}});

    // TODO: Verify that knot composition multiplies HOMFLY-PT polynomials
    // TODO: Verify that HOMFLY-PT gives Jones by:
    //   * l = it^-1, m = i(t^-1/2 - t^1/2)
    //   * a = t^-1, z = t^1/2 - t^-1/2

    // Verify that Regina will refuse to work with virtual link diagrams.
    EXPECT_THROW({ virtualTrefoil.link.homflyAZ(); }, FailedPrecondition);
    EXPECT_THROW({ virtualTrefoil.link.homflyLM(); }, FailedPrecondition);
    EXPECT_THROW({ kishino.link.homflyAZ(); }, FailedPrecondition);
    EXPECT_THROW({ kishino.link.homflyLM(); }, FailedPrecondition);
    EXPECT_THROW({ gpv.link.homflyAZ(); }, FailedPrecondition);
    EXPECT_THROW({ gpv.link.homflyLM(); }, FailedPrecondition);
    EXPECT_THROW({ virtualLink2.link.homflyAZ(); }, FailedPrecondition);
    EXPECT_THROW({ virtualLink2.link.homflyLM(); }, FailedPrecondition);
    EXPECT_THROW({ virtualLink3.link.homflyAZ(); }, FailedPrecondition);
    EXPECT_THROW({ virtualLink3.link.homflyLM(); }, FailedPrecondition);
    EXPECT_THROW({ virtualTrefoilx2.link.homflyAZ(); }, FailedPrecondition);
    EXPECT_THROW({ virtualTrefoilx2.link.homflyLM(); }, FailedPrecondition);
    EXPECT_THROW({ virtualDisconnected.link.homflyAZ(); }, FailedPrecondition);
    EXPECT_THROW({ virtualDisconnected.link.homflyLM(); }, FailedPrecondition);
}

static void verifyArrow(const Link& link, const char* name,
        const regina::Arrow& expected) {
    SCOPED_TRACE_CSTRING(name);

    // Since we are computing the arrow polynomial multiple times
    // (using different algorithms), we work with clones of the link
    // that do not clone any already-computed properties.

    // Always try the treewidth-based algorithm.
    EXPECT_EQ(Link(link, false).arrow(Algorithm::Treewidth), expected);

    // Only try the naive algorithm if the link is small enough, since this
    // algorithm iterates through 2^n states.
    if (link.size() <= 40)
        EXPECT_EQ(Link(link, false).arrow(Algorithm::Naive), expected);
}

static void verifyArrowConsistent(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);
    EXPECT_EQ(Link(link, false).arrow(Algorithm::Treewidth),
        Link(link, false).arrow(Algorithm::Naive));
}

static void verifyArrowClassical(const TestCase& test) {
    SCOPED_TRACE_CSTRING(test.name);

    // For classical links, the arrow and Jones polynomials should be the same
    // (once we rewrite them using the same variable A).

    regina::Arrow arrow = Link(test.link, false).arrow();

    regina::Laurent<regina::Integer> jones = Link(test.link, false).jones();
    jones.scaleUp(-2);

    EXPECT_EQ(arrow, jones);

    {
        // Check that deducing the arrow polynomial from the bracket gives the
        // same answer as computing the arrow polynomial directly.
        // (This deduction is only made for classical links.)
        Link tmp(test.link, false);
        EXPECT_FALSE(tmp.knowsArrow());
        tmp.jones();
        EXPECT_TRUE(tmp.knowsArrow());
        EXPECT_EQ(tmp.arrow(), arrow);
    }
}

TEST_F(LinkTest, arrow) {
    verifyArrowClassical(empty);

    verifyArrowClassical(unknot0);
    verifyArrowClassical(unknot1);
    verifyArrowClassical(unknot3);
    verifyArrowClassical(unknotMonster);
    // Let's not attempt this with the (enormous) Gordian unknot.

    verifyArrowClassical(trefoilLeft);
    verifyArrowClassical(trefoilRight);
    verifyArrowClassical(trefoil_r1x2);
    verifyArrowClassical(trefoil_r1x6);
    verifyArrowClassical(figureEight);
    verifyArrowClassical(figureEight_r1x2);
    verifyArrowClassical(conway);
    verifyArrowClassical(kinoshitaTerasaka);
    // GST is also too large for arrow polynomials right now.
    verifyArrowClassical(rht_rht);
    verifyArrowClassical(rht_lht);

    verifyArrowClassical(unlink2_0);
    verifyArrowClassical(unlink3_0);
    verifyArrowClassical(unlink2_r2);
    verifyArrowClassical(unlink2_r1r1);
    verifyArrowClassical(hopf);
    verifyArrowClassical(whitehead);
    verifyArrowClassical(borromean);
    verifyArrowClassical(trefoil_unknot0);
    verifyArrowClassical(trefoil_unknot1);
    verifyArrowClassical(trefoil_unknot_overlap);
    verifyArrowClassical(adams6_28);

    // Our virtual trefoil diagram is identical to Figure 22 from Kauffman,
    // "Introduction to virtual knot theory", JKTR 21 (2012).  We therefore
    // treat Kauffman's example as an independent verification.  Note that
    // Kauffman does not normalise his arrow polynomial using the writhe.
    verifyArrow(virtualTrefoil.link, virtualTrefoil.name,
        { {{}, {-4, {1}}},
          {{1}, {-10, {-1,0,0,0,1}}}
        });

    // Our Kishino diagram is identical to the one in Dye-Kauffman (see below)
    // except for relabelling, and so its arrow polynomial is independently
    // verified by Dye-Kauffman.
    verifyArrow(kishino.link, kishino.name,
        { {{}, {-4, {1,0,0,0,1,0,0,0,1}}},
          {{0,1}, {0, {2}}},
          {{2}, {-4, {-1,0,0,0,-2,0,0,0,-1}}}
        });

    // This was computed using Regina 7.4.
    verifyArrow(gpv.link, gpv.name,
        { {{}, {8, {1}}},
          {{0,1}, {12, {1,0,0,0,-2,0,0,0,1}}},
          {{1}, {10, {2,0,0,0,-2}}}
        });

    // This is a reflection of the virtual Hopf link in Dye-Kauffman (see
    // below).  The expected polynomial below is therefore the polynomial from
    // Dye-Kauffman but replacing A <-> A^-1.
    verifyArrow(virtualLink2.link, virtualLink2.name,
        { {{}, {-2, {-1}}},
          {{1}, {-4, {-1}}} });

    // These were computed using Regina 7.4.
    verifyArrow(virtualLink3.link, virtualLink3.name,
        { {{}, {-8, {1,0,0,0,1}}},
          {{1}, {-6, {2}}} });
    verifyArrow(virtualTrefoilx2.link, virtualTrefoilx2.name,
        { {{}, {-26, {1,0,0,0,-3,0,0,0,-1,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,-1}}},
          {{0,2}, {-30, {-1,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,-1}}},
          {{2}, {-22, {2,0,0,0,2,0,0,0,-2,0,0,0,-2}}}
        });

    // The following link is the disjoint union
    // (virtualLink3 U virtualTrefoil U hopf), and so the expected polynomial
    // below is the product of the three individual polynomials for those
    // three individual links, along with two factors of (-A^2 - A^-2).
    verifyArrow(virtualDisconnected.link, virtualDisconnected.name,
        { {{}, {-26, {-1,0,0,0,-3,0,0,0,-4,0,0,0,-4,0,0,0,-3,0,0,0,-1}}},
          {{1}, {-32, {1,0,0,0,2,0,0,0,-1,0,0,0,-4,0,0,0,-5,0,0,0,-6,0,0,0,-3}}},
          {{2}, {-30, {2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,-2,0,0,0,-2}}}
        });

    // Verify all of the examples from Dye & Kauffman, JKTR 18 (2009).

    // Section 3.1: Virtual Hopf link
    verifyArrow(Link::fromData({-1}, {1}, {-1}), "Virtual Hopf link",
        { {{}, {2, {-1}}},
          {{1}, {4, {-1}}} });

    // Section 3.2: Virtualised trefoil (this is _not_ the virtual trefoil!)
    verifyArrow(Link::fromData({+1,+1,-1}, {1,-2,3,-1,2,-3}),
        "Virtualised trefoil",
        { {{}, {-8, {1}}},
          {{2}, {-8, {-1,0,0,0,0,0,0,0,1}}} });

    // Section 3.3: Kishino's knot
    verifyArrow(Link::fromData({+1,-1,+1,-1}, {1,-2,4,-3,-4,3,-1,2}),
        "Kishino's knot",
        { {{}, {-4, {1,0,0,0,1,0,0,0,1}}},
          {{0,1}, {0, {2}}},
          {{2}, {-4, {-1,0,0,0,-2,0,0,0,-1}}}
        });

    // Section 3.4: Slavik's knot
    verifyArrow(
        Link::fromData({+1,+1,-1,-1,-1}, {1,-3,4,-1,2,-5,3,-4,5,-2}),
        "Slavik's knot",
        { {{}, {0, {1}}} });

    // Section 3.5: Miyazawa's knot
    // Note: our answer differs from Dye-Kauffman by sign (+/-) for the
    // coefficient of K_1^2 A^-4.  However, our answer is consistent with the
    // Jones polynomial for that same knot, and so it seems likely that the
    // sign error is in the Dye-Kauffman paper.
    verifyArrow(Link::fromData({+1,-1,+1,+1}, {1,-2,-3,-1,3,4,2,-4}),
        "Miyazawa's knot",
        { {{}, {-8, {1,0,0,0,2}}},
          {{0,1}, {-8, {1,0,0,0,1}}},
          {{1}, {-10, {-1,0,0,0,1}}},
          {{2}, {-8, {-2,0,0,0,-2}}}
        });

    // Section 3.6: Two knots differentiated only by K_1 and K_3
    verifyArrow(Link::fromSignedGauss("O1-O2-O3-O4+U1-U3-U2-U4+"),
        "Knot 4.93",
        { {{}, {8, {1}}},
          {{0,0,1}, {10, {1}}},
          {{1}, {6, {1}}},
          {{1,1}, {6, {-1,0,0,0,-1}}},
          {{2}, {0, {1,0,0,0,0,0,0,0,-1}}}
        });
    verifyArrow(Link::fromSignedGauss("O1-O2-U3-O4+U2-U1-O3-U4+"),
        "Knot 4.103",
        { {{}, {8, {1}}},
          {{0,0,1}, {6, {1}}},
          {{1}, {10, {1}}},
          {{1,1}, {6, {-1,0,0,0,-1}}},
          {{2}, {0, {1,0,0,0,0,0,0,0,-1}}}
        });

    // Section 3.8: Two virtual torus links
    // Note: In the Dye-Kauffman paper, the polynomials for these links do not
    // appear to be normalised using the writhe (though this is probably
    // deliberate, since in the paper they use the subscript <..>_A instead of
    // <..>_NA when writing these two polynomials).
    verifyArrow(Link::fromData({+1,+1,+1}, {1,-2,3}, {-1,2,-3}),
        "Virtual torus link VT",
        { {{}, {-6, {-1}}},
          {{1}, {-16, {-1,0,0,0,1,0,0,0,-1}}}
        });
    verifyArrow(Link::fromData({-1,-1,-1}, {3,-2,1}, {-1,2,-3}),
        "Virtual torus link RV",
        { {{}, {2, {-1,0,0,0,1,0,0,0,-1}}},
          {{1}, {12, {-1}}}
        });

    // Run through a small census and ensure that both algorithms give
    // the same arrow polynomial in both cases.
    runCensusAllVirtual(verifyArrowConsistent);

    // Check that the multithreaded algorithm gives the same answers as the
    // single-threaded algorithm.  All polynomials below were computing using
    // Reging 7.4 in single-threaded mode.
    EXPECT_EQ(ExampleLink::gpv().arrow(Algorithm::Default, 2),
        regina::Arrow({
            {{}, {8, {1}}},
            {{0,1}, {12, {1,0,0,0,-2,0,0,0,1}}},
            {{1}, {10, {2,0,0,0,-2}}}
        }));
    EXPECT_EQ(ExampleLink::gpv().parallel(2).arrow(Algorithm::Default, 4),
        regina::Arrow({
            {{}, {2, {-1,0,0,0,1,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,2,0,0,0,
                -4,0,0,0,-7,0,0,0,-1,0,0,0,11,0,0,0,-1,0,0,0,-5,0,0,0,1}}},
            {{0,0,0,2}, {30, {-1,0,0,0,1,0,0,0,3,0,0,0,-3,0,0,0,-3,0,0,0,
                3,0,0,0,1,0,0,0,-1}}},
            {{0,0,2}, {26, {-2,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,
                -6,0,0,0,0,0,0,0,2}}},
            {{0,2}, {22, {-3,0,0,0,-1,0,0,0,6,0,0,0,2,0,0,0,-3,0,0,0,-1}}},
            {{2}, {18, {-2,0,0,0,0,0,0,0,2,0,0,0,-2,0,0,0,0,0,0,0,0,0,0,0,
                -2,0,0,0,2,0,0,0,2}}},
            {{4}, {18, {-1,0,0,0,-3,0,0,0,-1,0,0,0,5,0,0,0,5,0,0,0,-1,0,0,0,
                -3,0,0,0,-1}}}
        }));
}

static void verifyAffineIndex(const Link& link, const char* name,
        const regina::Laurent<regina::Integer>& expected) {
    // The affine index polynomial is not cached.
    // No need to fuss about cloning the given link.
    SCOPED_TRACE_CSTRING(name);

    EXPECT_EQ(link.affineIndex(), expected);

    // Verify that the polynomial behaves as expected under simple knot
    // transformations.
    // See Kauffman, JKTR (2018), Proposition 4.7.
    regina::Laurent<regina::Integer> expectInverted = expected;
    expectInverted.invertX();

    {
        // Reverse the orientation of the knot.
        Link reverse(link);
        reverse.reverse();
        EXPECT_EQ(reverse.affineIndex(), expectInverted);
    }
    {
        // Switch upper/lower at each crossing.
        // Kauffman in JKTR (2013) says P(t) becomes -P(t).
        // However, I get -P(t^-1), and curiously _this_ is what Kauffman
        // says in JKTR (2018) and also in his 2021 paper for Turaev.
        // The latter papers cite the 2013 paper for the proof, so it would
        // have been nice if he'd at least _mentioned_ that the source paper
        // got the formula wrong.
        Link changeAll(link);
        changeAll.changeAll();
        EXPECT_EQ(changeAll.affineIndex(), -expectInverted);
    }
    {
        // Switch +/- at each crossing.
        Link reflect(link);
        reflect.reflect();
        EXPECT_EQ(reflect.affineIndex(), -expectInverted);
    }
}

static void verifyAffineIndex(const TestCase& test,
        const regina::Laurent<regina::Integer>& expected) {
    verifyAffineIndex(test.link, test.name, expected);
}

TEST_F(LinkTest, affineIndex) {
    // For classical knots, the affine index polynomial is always zero.
    verifyAffineIndex(unknot0, {});
    verifyAffineIndex(unknot1, {});
    verifyAffineIndex(unknot3, {});
    verifyAffineIndex(unknotMonster, {});
    verifyAffineIndex(unknotGordian, {});

    verifyAffineIndex(trefoilLeft, {});
    verifyAffineIndex(trefoilRight, {});
    verifyAffineIndex(trefoil_r1x2, {});
    verifyAffineIndex(trefoil_r1x6, {});
    verifyAffineIndex(figureEight, {});
    verifyAffineIndex(figureEight_r1x2, {});

    verifyAffineIndex(conway, {});
    verifyAffineIndex(kinoshitaTerasaka, {});
    verifyAffineIndex(gst, {});

    verifyAffineIndex(rht_rht, {});
    verifyAffineIndex(rht_lht, {});

    // For virtual knots, this polynomial is meaningful.
    // Note: The polynomial for GPV has not been independently verified, and
    // instead was computed using Regina 7.4.
    verifyAffineIndex(virtualTrefoil, {-1, {1,-2,1}});
    verifyAffineIndex(kishino, {});
    verifyAffineIndex(gpv, {-1, {-2,4,-2}});

    // Some more examples from Kauffman's papers:
    verifyAffineIndex(Link::fromData({+1,+1,+1}, {-1,2,3,1,-2,-3}),
        "Kauffman JKTR 2013 Figure 4", {-2, {1,0,-2,0,1}});
    verifyAffineIndex(Link::fromData({-1,+1,+1,-1}, {-1,-2,4,-3,-4,1,3,2}),
        "Kauffman JKTR 2013 Figure 17", {-1, {-1,1,1,-1}});
    verifyAffineIndex(Link::fromData({+1,+1,-1,-1}, {-1,2,1,-2,-3,4,3,-4}),
        "Kauffman JKTR 2018 Figure 22", {});
    verifyAffineIndex(Link::fromData({-1,+1,-1,-1}, {1,-2,4,-3,2,-1,3,-4}),
        "Kauffman JKTR 2018 Figure 29", {-2, {-1,0,2,0,-1}});
    verifyAffineIndex(Link::fromData({+1,+1,+1,+1}, {-1,2,-3,-2,4,1,3,-4}),
        "Kauffman JKTR 2018 Figure 31a", {-1, {2,-3,0,1}});
    // For the next example, Kauffman says t^2-1 but I get t^2-t-1+t^-1.
    // Kauffman's papers have other mistakes in them, so I'm calling this as
    // correct here in Regina.
    verifyAffineIndex(Link::fromData({+1,-1,+1,+1}, {-1,-2,-3,2,4,1,3,-4}),
        "Kauffman JKTR 2018 Figure 31b", {-1, {1,-1,-1,1}});

    // The affine index polynomial is not available for empty or
    // multiple-component links.
    EXPECT_THROW({ empty.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ unlink2_0.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ unlink3_0.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ unlink2_r2.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ unlink2_r1r1.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ hopf.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ whitehead.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ borromean.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ trefoil_unknot0.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ trefoil_unknot1.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ trefoil_unknot_overlap.link.affineIndex(); },
        FailedPrecondition);
    EXPECT_THROW({ adams6_28.link.affineIndex(); }, FailedPrecondition);

    EXPECT_THROW({ virtualLink2.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ virtualLink3.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ virtualTrefoilx2.link.affineIndex(); }, FailedPrecondition);
    EXPECT_THROW({ virtualDisconnected.link.affineIndex(); },
        FailedPrecondition);
}

static void verifyComplementBasic(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    Triangulation<3> c = link.complement();
    size_t virtualGenus = link.virtualGenus();

    EXPECT_EQ(c.countComponents(), 1);
    EXPECT_TRUE(c.isOrientable());
    EXPECT_TRUE(c.isOriented());
    EXPECT_FALSE(c.hasBoundaryFacets());

    // Verify that we have the right number (and genus) of ideal vertices.
    size_t torus = 0;
    ssize_t higherGenus[2] = { -1, -1 };
    for (auto v : c.vertices()) {
        EXPECT_TRUE(v->isLinkClosed());
        if (v->isIdeal()) {
            long euler = v->linkEulerChar();
            EXPECT_TRUE(euler <= 0);
            EXPECT_TRUE(euler % 2 == 0);
            // We already tested orientability of the overall triangulation,
            // which is enough to ensure orientability of the vertex link.
            if (euler == 0)
                ++torus;
            else {
                // We don't expect more than two higher genus vertices.
                ssize_t genus = (2 - euler) / 2;
                EXPECT_EQ(higherGenus[1], -1);
                if (higherGenus[0] == -1)
                    higherGenus[0] = genus;
                else
                    higherGenus[1] = genus;
            }
        }
    }
    switch (virtualGenus) {
        case 0:
            EXPECT_EQ(torus, link.countComponents());
            EXPECT_EQ(higherGenus[0], -1);
            break;
        case 1:
            EXPECT_EQ(torus, link.countComponents() + 2);
            EXPECT_EQ(higherGenus[0], -1);
            break;
        default:
            EXPECT_EQ(torus, link.countComponents());
            EXPECT_EQ(higherGenus[0], virtualGenus);
            EXPECT_EQ(higherGenus[1], virtualGenus);
            break;
    }

    // For classical links, verify that the link groups look the same also.
    // Don't do this for enormous link diagrams.
    if (link.size() <= 20 && virtualGenus == 0)
        verifyIsomorphic(link.group(), c.group());
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
        cut.simplify();
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
    // simplify() will reach a minimal triangulation.

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

static void verifyLongComplement(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    if (link.countComponents() != 1 || ! link.isClassical()) {
        EXPECT_THROW(link.longComplement(), regina::FailedPrecondition);
        return;
    }

    Triangulation<3> comp = link.longComplement();

    EXPECT_EQ(comp.countComponents(), 1);
    EXPECT_TRUE(comp.isOrientable());
    EXPECT_TRUE(comp.isOriented());
    EXPECT_FALSE(comp.isValid());
    EXPECT_TRUE(comp.hasBoundaryFacets());
    EXPECT_FALSE(comp.isIdeal());

    // Ensure there is a single invalid vertex, and that its link is an annulus.
    regina::Vertex<3>* invalid = nullptr;
    for (auto v : comp.vertices()) {
        if (invalid)
            EXPECT_TRUE(v->isValid());
        else if (! v->isValid())
            invalid = v;
    }
    EXPECT_TRUE(invalid);
    if (invalid) {
        // For an invalid vertex (whose link must be a surface with one or
        // more punctures), the following tests are enough to ensure that the
        // link is an annulus.
        EXPECT_TRUE(invalid->isLinkOrientable());
        EXPECT_EQ(invalid->linkEulerChar(), 0);
    }

    // Verify that the link groups look the same also.
    // Don't do this for enormous link diagrams.
    if (link.size() <= 20)
        verifyIsomorphic(link.group(), comp.group());

    // Truncating the invalid vertex should give us back the ordinary
    // complement.
    if (invalid) {
        comp.truncate(invalid); // may break orientedness
        comp.simplify();

        EXPECT_EQ(comp.countComponents(), 1);
        EXPECT_TRUE(comp.isOrientable());
        EXPECT_TRUE(comp.isValid());
        EXPECT_FALSE(comp.isIdeal());
        EXPECT_TRUE(comp.hasBoundaryFacets());
        EXPECT_EQ(comp.countBoundaryComponents(), 1);
        EXPECT_TRUE(comp.boundaryComponent(0)->isOrientable());
        EXPECT_EQ(comp.boundaryComponent(0)->eulerChar(), 0);
    }
}

TEST_F(LinkTest, longComplement) {
    testManualCases(verifyLongComplement);
}

static void verifyR1Count(const TestCase& test, size_t up, size_t down) {
    // Most of the time, up == #crossings * 8.  However, there will be more
    // moves available if the link has any zero-crossing unknot components.

    SCOPED_TRACE_CSTRING(test.name);

    size_t foundUp = 0, foundDown = 0;

    for (int side = 0; side < 2; ++side)
        for (int sign = -1; sign <= 1; sign += 2)
            if (test.link.hasR1(StrandRef(), side, sign))
                ++foundUp;

    if (test.link.hasR1(nullptr))
        ++foundDown;

    for (auto c : test.link.crossings()) {
        for (int side = 0; side < 2; ++side)
            for (int sign = -1; sign <= 1; sign += 2)
                for (int strand = 0; strand < 2; ++strand)
                    if (test.link.hasR1(c->strand(strand), side, sign))
                        ++foundUp;

        if (test.link.hasR1(c))
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

    verifyR1Count(virtualTrefoil, 16, 0);
    verifyR1Count(kishino, 32, 0);
    verifyR1Count(gpv, 32, 0);
    verifyR1Count(virtualLink2, 8, 0);
    verifyR1Count(virtualLink3, 16, 0);
    verifyR1Count(virtualTrefoilx2, 96, 0);
    verifyR1Count(virtualDisconnected, 48, 0);
}

static void verifyR2Count(const TestCase& test,
        size_t upClassical, size_t upVirtual,
        size_t downByCrossing, size_t downByStrand) {
    // Most of the time, downByStrand == downByCrossing * 2.
    // However, this can differ in the case of an unknotted loop placed on top
    // of another strand - here there are 2 moves by crossing, but only 3 moves
    // by strand (all of which produce identical results).

    SCOPED_TRACE_CSTRING(test.name);

    size_t foundUpClassical = 0, foundUpVirtual = 0;
    size_t foundDownByCrossing = 0, foundDownByStrand = 0;

    for (int side1 = 0; side1 < 2; ++side1)
        for (int side2 = 0; side2 < 2; ++side2) {
            if (test.link.hasR2({}, side1, {}, side2))
                ++foundUpClassical;
            if (test.link.hasR2Virtual({}, side1, {}, side2))
                ++foundUpVirtual;
        }

    for (auto c : test.link.crossings()) {
        for (int side1 = 0; side1 < 2; ++side1)
            for (int side2 = 0; side2 < 2; ++side2)
                for (int str1 = 0; str1 < 2; ++str1) {
                    if (test.link.hasR2({}, side1, c->strand(str1), side2))
                        ++foundUpClassical;
                    if (test.link.hasR2(c->strand(str1), side1, {}, side2))
                        ++foundUpClassical;

                    if (test.link.hasR2Virtual({}, side1,
                            c->strand(str1), side2))
                        ++foundUpVirtual;
                    if (test.link.hasR2Virtual(c->strand(str1), side1,
                            {}, side2))
                        ++foundUpVirtual;

                    for (auto c2 : test.link.crossings())
                        for (int str2 = 0; str2 < 2; ++str2) {
                            if (test.link.hasR2(c->strand(str1), side1,
                                    c2->strand(str2), side2))
                                ++foundUpClassical;
                            if (test.link.hasR2Virtual(c->strand(str1), side1,
                                    c2->strand(str2), side2))
                                ++foundUpVirtual;
                        }
                }

        if (test.link.hasR2(c))
            ++foundDownByCrossing;

        for (int strand = 0; strand < 2; ++strand)
            if (test.link.hasR2(c->strand(strand)))
                ++foundDownByStrand;
    }

    if (test.link.hasR2(nullptr))
        ++foundDownByCrossing;

    if (test.link.hasR2(StrandRef()))
        ++foundDownByStrand;

    EXPECT_EQ(foundUpClassical, upClassical);
    EXPECT_EQ(foundUpVirtual, upVirtual);
    EXPECT_EQ(foundDownByCrossing, downByCrossing);
    EXPECT_EQ(foundDownByStrand, downByStrand);
}

TEST_F(LinkTest, r2Count) {
    // Note: for an n-crossing link with no zero-crossing components, the
    // number of _virtual_ increasing R2 moves should always be 4*2n(2n-1).
    verifyR2Count(empty, 0, 0, 0, 0);

    verifyR2Count(unknot0, 0, 0, 0, 0);
    verifyR2Count(unknot1, 2, 8, 0, 0);
    verifyR2Count(unknot3, 18, 120, 2, 4);
    verifyR2Count(unknotMonster, 116, 1520, 0, 0); // computed w/ Regina 7.3
    verifyR2Count(unknotGordian, 2046, 316968, 0, 0); // computed w/ Regina 7.3

    verifyR2Count(trefoilLeft, 18, 120, 0, 0);
    verifyR2Count(trefoilRight, 18, 120, 0, 0);
    verifyR2Count(trefoil_r1x2, 58, 360, 0, 0);
    verifyR2Count(trefoil_r1x6, 160, 1224, 0, 0);
    verifyR2Count(figureEight, 28, 224, 0, 0);
    verifyR2Count(figureEight_r1x2, 66, 528, 0, 0);
    verifyR2Count(conway, 120, 1848, 0, 0);
    verifyR2Count(kinoshitaTerasaka, 118, 1848, 0, 0);
    verifyR2Count(gst, 612, 36480, 0, 0);

    verifyR2Count(rht_rht, 62, 528, 0, 0); // merges bigon-bigon, triangle-triangle
    verifyR2Count(rht_lht, 60, 528, 0, 0); // merges bigon-triangle, bigon-triangle

    verifyR2Count(unlink2_0, 4, 4, 0, 0);
    verifyR2Count(unlink3_0, 4, 4, 0, 0);
    verifyR2Count(unlink2_r2, 8, 48, 2, 4);
    verifyR2Count(unlink2_r1r1, 36, 48, 0, 0);

    verifyR2Count(hopf, 8, 48, 0, 0);
    verifyR2Count(whitehead, 40, 360, 0, 0);
    verifyR2Count(borromean, 48, 528, 0, 0);
    verifyR2Count(trefoil_unknot0, 66, 120 + 2*6*4, 0, 0);
    verifyR2Count(trefoil_unknot1, 116, 224, 0, 0);
    verifyR2Count(trefoil_unknot_overlap, 46, 360, 2, 3);
    verifyR2Count(adams6_28, 54, 528, 0, 0);

    verifyR2Count(virtualTrefoil, 28, 48, 0, 0);
    verifyR2Count(kishino, 136, 224, 0, 0);
    verifyR2Count(gpv, 60, 224, 0, 0);
    verifyR2Count(virtualLink2, 8, 8, 0, 0);
    verifyR2Count(virtualLink3, 20, 48, 0, 0);
    verifyR2Count(virtualTrefoilx2, 262, 2208, 0, 0);
    verifyR2Count(virtualDisconnected, 20 + 28 + 8 + 8*8*6, 528, 0, 0);
}

static void verifyR3Count(const TestCase& test, size_t movesByCrossing) {
    // All of the time, movesByStrand == 3 * movesByCrossing.

    SCOPED_TRACE_CSTRING(test.name);

    size_t foundByCrossing = 0, foundByStrand = 0;

    for (int side = 0; side < 2; ++side) {
        if (test.link.hasR3(nullptr, side))
            ++foundByCrossing;

        if (test.link.hasR3(StrandRef(), side))
            ++foundByStrand;
    }

    for (auto c : test.link.crossings())
        for (int side = 0; side < 2; ++side) {
            if (test.link.hasR3(c, side))
                ++foundByCrossing;

            for (int strand = 0; strand < 2; ++strand)
                if (test.link.hasR3(c->strand(strand), side))
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

    verifyR3Count(virtualTrefoil, 0);
    verifyR3Count(kishino, 0);
    verifyR3Count(gpv, 0);
    verifyR3Count(virtualLink2, 0);
    verifyR3Count(virtualLink3, 0);
    verifyR3Count(virtualTrefoilx2, 2);
    verifyR3Count(virtualDisconnected, 0);
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

static void verifyR2UpClassical(Link link, int upperCrossing, int upperStrand,
        int upperSide, int lowerCrossing, int lowerStrand, int lowerSide,
        const char* briefResult) {
    SCOPED_TRACE_CSTRING(briefResult);
    SCOPED_TRACE_NUMERIC(upperCrossing);
    SCOPED_TRACE_NUMERIC(upperStrand);
    SCOPED_TRACE_NUMERIC(upperSide);
    SCOPED_TRACE_NUMERIC(lowerCrossing);
    SCOPED_TRACE_NUMERIC(lowerStrand);
    SCOPED_TRACE_NUMERIC(lowerSide);

    // Every classical R2 is also a virtual R2, so we test both R2 variants.

    StrandRef upper, lower;
    if (upperCrossing >= 0)
        upper = link.crossing(upperCrossing)->strand(upperStrand);
    if (lowerCrossing >= 0)
        lower = link.crossing(lowerCrossing)->strand(lowerStrand);

    Link copy(link, false);
    StrandRef upperCopy = copy.translate(upper);
    StrandRef lowerCopy = copy.translate(lower);

    EXPECT_TRUE(link.r2(upper, upperSide, lower, lowerSide));
    EXPECT_TRUE(isConsistent(link));
    EXPECT_EQ(link.brief(), briefResult);

    EXPECT_TRUE(copy.r2Virtual(upperCopy, upperSide, lowerCopy, lowerSide));
    EXPECT_TRUE(isConsistent(copy));
    EXPECT_EQ(copy.brief(), briefResult);
}

static void verifyR2UpVirtualOnly(Link link, int upperCrossing, int upperStrand,
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

    Link copy(link, false);
    StrandRef upperCopy = copy.translate(upper);
    StrandRef lowerCopy = copy.translate(lower);

    EXPECT_FALSE(copy.r2(upperCopy, upperSide, lowerCopy, lowerSide));
    EXPECT_TRUE(isConsistent(copy));
    EXPECT_EQ(copy, link);

    EXPECT_TRUE(link.r2Virtual(upper, upperSide, lower, lowerSide));
    EXPECT_TRUE(isConsistent(link));
    EXPECT_EQ(link.brief(), briefResult);
}

static void verifyR2UpVirtual(Link link, int crossing, int strand,
        int firstSide, int firstStrand, const char* briefResult) {
    SCOPED_TRACE_CSTRING(briefResult);
    SCOPED_TRACE_NUMERIC(crossing);
    SCOPED_TRACE_NUMERIC(strand);
    SCOPED_TRACE_NUMERIC(firstSide);
    SCOPED_TRACE_NUMERIC(firstStrand);

    StrandRef arc;
    if (crossing >= 0)
        arc = link.crossing(crossing)->strand(strand);

    EXPECT_TRUE(link.r2Virtual(arc, firstSide, firstStrand));
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

static void verifyR1Down(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (size_t i = 0; i < link.size(); ++i) {
        Link alt(link, false);
        if (alt.r1(alt.crossing(i))) {
            EXPECT_TRUE(isConsistent(alt));
            EXPECT_EQ(alt.size(), link.size() - 1);
            EXPECT_EQ(alt.virtualGenus(), link.virtualGenus());
            verifyTopologicallySame(alt, link);
        } else {
            EXPECT_EQ(alt, link);
        }
    }
    {
        Link alt(link, false);
        EXPECT_FALSE(alt.r1(nullptr));
        EXPECT_EQ(alt, link);
    }
}

static void verifyR1Up(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (int side = 0; side <= 1; ++side)
        for (int sign = -1; sign <= 1; sign += 2) {
            for (size_t i = 0; i < link.size(); ++i) {
                for (int strand = 0; strand <= 1; ++strand) {
                    Link alt(link, false);
                    EXPECT_TRUE(alt.r1(alt.crossing(i)->strand(strand),
                        side, sign));
                    EXPECT_TRUE(isConsistent(alt));
                    EXPECT_EQ(alt.size(), link.size() + 1);
                    EXPECT_EQ(alt.virtualGenus(), link.virtualGenus());
                    verifyTopologicallySame(alt, link);
                }
            }
            {
                Link alt(link, false);
                if (alt.r1(StrandRef(), side, sign)) {
                    EXPECT_TRUE(isConsistent(alt));
                    EXPECT_EQ(alt.size(), link.size() + 1);
                    EXPECT_EQ(alt.virtualGenus(), link.virtualGenus());
                    verifyTopologicallySame(alt, link);
                } else {
                    EXPECT_EQ(alt, link);
                }
            }
        }
}

static void verifyR2Down(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (size_t i = 0; i < link.size(); ++i) {
        for (int strand = 0; strand <= 1; ++strand) {
            Link alt(link, false);
            if (alt.r2(alt.crossing(i)->strand(strand))) {
                EXPECT_TRUE(isConsistent(alt));
                EXPECT_NE(alt, link);
                EXPECT_EQ(alt.size(), link.size() - 2);
                EXPECT_LE(alt.virtualGenus(), link.virtualGenus());
                verifyTopologicallySame(alt, link);
            } else {
                EXPECT_EQ(alt, link);
            }
        }

        Link alt(link, false);
        if (alt.r2(alt.crossing(i))) {
            EXPECT_TRUE(isConsistent(alt));
            EXPECT_EQ(alt.size(), link.size() - 2);
            EXPECT_LE(alt.virtualGenus(), link.virtualGenus());
            verifyTopologicallySame(alt, link);
        } else {
            EXPECT_EQ(alt, link);
        }
    }
    {
        Link alt(link, false);
        EXPECT_FALSE(alt.r2(nullptr));
        EXPECT_EQ(alt, link);
    }
    {
        Link alt(link, false);
        EXPECT_FALSE(alt.r2(StrandRef()));
        EXPECT_EQ(alt, link);
    }
}

static void verifyR2Up(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (int uSide = 0; uSide <= 1; ++uSide)
        for (int lSide = 0; lSide <= 1; ++lSide) {
            for (size_t c1 = 0; c1 < link.size(); ++c1) {
                for (int s1 = 0; s1 <= 1; ++s1) {
                    for (size_t c2 = 0; c2 < link.size(); ++c2)
                        for (size_t s2 = 0; s2 <= 1; ++s2) {
                            Link alt(link, false);
                            if (alt.r2(alt.crossing(c1)->strand(s1), uSide,
                                    alt.crossing(c2)->strand(s2), lSide)) {
                                EXPECT_TRUE(isConsistent(alt));
                                EXPECT_EQ(alt.size(), link.size() + 2);
                                EXPECT_EQ(alt.virtualGenus(),
                                    link.virtualGenus());
                                verifyTopologicallySame(alt, link);

                                Link virt(link, false);
                                EXPECT_TRUE(virt.r2Virtual(
                                    virt.crossing(c1)->strand(s1), uSide,
                                    virt.crossing(c2)->strand(s2), lSide));
                                EXPECT_EQ(virt, alt);
                            } else {
                                EXPECT_EQ(alt, link);

                                // Although a classical R2 is not possible,
                                // a virtual R2 should still be possible
                                // unless both strands are the same.
                                if (c1 == c2 && s1 == s2) {
                                    Link virt(link, false);
                                    EXPECT_FALSE(virt.r2Virtual(
                                        virt.crossing(c1)->strand(s1), uSide,
                                        virt.crossing(c2)->strand(s2), lSide));
                                    EXPECT_EQ(virt, link);
                                } else {
                                    Link virt(link, false);
                                    EXPECT_TRUE(virt.r2Virtual(
                                        virt.crossing(c1)->strand(s1), uSide,
                                        virt.crossing(c2)->strand(s2), lSide));
                                    EXPECT_TRUE(isConsistent(virt));
                                    EXPECT_EQ(virt.size(), link.size() + 2);
                                    // This is the only case where the virtual
                                    // genus of the diagram might change.
                                    EXPECT_GE(virt.virtualGenus(),
                                        link.virtualGenus());
                                    verifyTopologicallySame(virt, link);
                                }
                            }
                        }
                    {
                        Link alt(link, false);
                        if (alt.r2(alt.crossing(c1)->strand(s1), uSide,
                                {}, lSide)) {
                            EXPECT_TRUE(isConsistent(alt));
                            EXPECT_EQ(alt.size(), link.size() + 2);
                            EXPECT_EQ(alt.virtualGenus(), link.virtualGenus());
                            verifyTopologicallySame(alt, link);

                            Link virt(link, false);
                            EXPECT_TRUE(virt.r2Virtual(
                                virt.crossing(c1)->strand(s1), uSide,
                                {}, lSide));
                            EXPECT_EQ(virt, alt);
                        } else {
                            EXPECT_EQ(alt, link);

                            Link virt(link, false);
                            EXPECT_FALSE(virt.r2Virtual(
                                virt.crossing(c1)->strand(s1), uSide,
                                {}, lSide));
                            EXPECT_EQ(virt, link);
                        }
                    }
                    {
                        Link alt(link, false);
                        if (alt.r2({}, uSide,
                                alt.crossing(c1)->strand(s1), lSide)) {
                            EXPECT_TRUE(isConsistent(alt));
                            EXPECT_EQ(alt.size(), link.size() + 2);
                            EXPECT_EQ(alt.virtualGenus(), link.virtualGenus());
                            verifyTopologicallySame(alt, link);

                            Link virt(link, false);
                            EXPECT_TRUE(virt.r2Virtual({}, uSide,
                                virt.crossing(c1)->strand(s1), lSide));
                            EXPECT_EQ(virt, alt);
                        } else {
                            EXPECT_EQ(alt, link);

                            Link virt(link, false);
                            EXPECT_FALSE(virt.r2Virtual({}, uSide,
                                virt.crossing(c1)->strand(s1), lSide));
                            EXPECT_EQ(virt, link);
                        }
                    }
                }
            }
            {
                Link alt(link, false);
                if (alt.r2({}, uSide, {}, lSide)) {
                    EXPECT_TRUE(isConsistent(alt));
                    EXPECT_EQ(alt.size(), link.size() + 2);
                    EXPECT_EQ(alt.virtualGenus(), link.virtualGenus());
                    verifyTopologicallySame(alt, link);

                    Link virt(link, false);
                    EXPECT_TRUE(virt.r2Virtual({}, uSide, {}, lSide));
                    EXPECT_EQ(virt, alt);
                } else {
                    EXPECT_EQ(alt, link);

                    Link virt(link, false);
                    EXPECT_FALSE(virt.r2Virtual({}, uSide, {}, lSide));
                    EXPECT_EQ(virt, link);
                }
            }
        }
}

static void verifyR3(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    // Note: there is exactly one scenario in which alt == link (and with
    // identical labellings): the R3 happens on a virtual link with three
    // components, each passing through two crossings and making one side of
    // the R3 triangle.
    for (int side = 0; side <= 1; ++side) {
        for (size_t i = 0; i < link.size(); ++i) {
            for (int strand = 0; strand <= 1; ++strand) {
                Link alt(link, false);
                if (alt.r3(alt.crossing(i)->strand(strand), side)) {
                    EXPECT_TRUE(isConsistent(alt));
                    if (alt == link && link.isConnected()) {
                        // We must have one of the special scenarios above.
                        // We only test this for the connected case (which is
                        // easy).
                        EXPECT_EQ(link.size(), 3);
                        EXPECT_EQ(link.countComponents(), 3);
                    }
                    EXPECT_EQ(alt.size(), link.size());
                    EXPECT_EQ(alt.virtualGenus(), link.virtualGenus());
                    verifyTopologicallySame(alt, link);
                } else {
                    EXPECT_EQ(alt, link);
                }
            }

            Link alt(link, false);
            if (alt.r3(alt.crossing(i), side)) {
                EXPECT_TRUE(isConsistent(alt));
                if (alt == link && link.isConnected()) {
                    // We must have one of the special scenarios above.
                    // We only test this for the connected case (which is easy).
                    EXPECT_EQ(link.size(), 3);
                    EXPECT_EQ(link.countComponents(), 3);
                }
                EXPECT_EQ(alt.size(), link.size());
                EXPECT_EQ(alt.virtualGenus(), link.virtualGenus());
                verifyTopologicallySame(alt, link);
            } else {
                EXPECT_EQ(alt, link);
            }
        }
        {
            Link alt(link, false);
            EXPECT_FALSE(alt.r3(nullptr, side));
            EXPECT_EQ(alt, link);
        }
        {
            Link alt(link, false);
            EXPECT_FALSE(alt.r3(StrandRef(), side));
            EXPECT_EQ(alt, link);
        }
    }
}

TEST_F(LinkTest, reidemeister1Down) {
    testManualCases(verifyR1Down, false /* gordian */);
    runCensusAllVirtual(verifyR1Down);
}

TEST_F(LinkTest, reidemeister1Up) {
    testManualCases(verifyR1Up, false /* gordian */);
    runCensusAllVirtual(verifyR1Up, true /* small */);
}

TEST_F(LinkTest, reidemeister2Down) {
    testManualCases(verifyR2Down, false /* gordian */);
    runCensusAllVirtual(verifyR2Down);
}

TEST_F(LinkTest, reidemeister2Up) {
    testManualCases(verifyR2Up, false /* gordian */);
    runCensusAllVirtual(verifyR2Up, true /* small */);
}

TEST_F(LinkTest, reidemeister3) {
    testManualCases(verifyR3, false /* gordian */);
    runCensusAllVirtual(verifyR3);
}

TEST_F(LinkTest, reidemeisterMisc) {
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

    // A virtual variant of overlapping loops, with a single component and
    // optionally an extra twist:
    {
        Link link = Link::fromData({ 1, -1 }, { 1, 2, -1, -2 });
        verifyR2Down(link, 0, "( )");
        verifyR2Down(link, 0, 0, "( )");
        verifyR2Down(link, 0, 1, "( )");
    }
    {
        Link link = Link::fromData({ -1, 1 }, { -1, 2, 1, -2 });
        verifyR2Down(link, 1, "( )");
        verifyR2Down(link, 1, 0, "( )");
        verifyR2Down(link, 1, 1, "( )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 3, 1, 2, -1, -2, -3 });
        verifyR1Down(link, 2, "+- ( ^0 ^1 _0 _1 )");
        verifyR2Down(link, 0, "+ ( ^0 _0 )");
        verifyR2Down(link, 0, 0, "+ ( ^0 _0 )");
        verifyR2Down(link, 0, 1, "+ ( ^0 _0 )");
    }
    {
        Link link = Link::fromData({ -1, -1, 1 }, { -2, 1, -1, 3, 2, -3 });
        verifyR1Down(link, 0, "-+ ( _0 ^1 ^0 _1 )");
        verifyR2Down(link, 2, "- ( ^0 _0 )");
        verifyR2Down(link, 2, 0, "- ( ^0 _0 )");
        verifyR2Down(link, 2, 1, "- ( ^0 _0 )");
    }
    {
        Link link = Link::fromData({ -1, -1, 1 }, { -1, -3, -2, 3, 2, 1 });
        verifyR1Down(link, 0, "-+ ( _1 _0 ^1 ^0 )");
        verifyR2Down(link, 2, "- ( _0 ^0 )");
        verifyR2Down(link, 2, 0, "- ( _0 ^0 )");
        verifyR2Down(link, 2, 1, "- ( _0 ^0 )");
    }

    // A virtual variant of overlapping loops with an extra crossing, which
    // makes it two components:
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 3, 1, 2 }, { -3, -1, -2 });
        verifyR2Down(link, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 1, "+ ( ^0 ) ( _0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 2, 3, 1 }, { -2, -3, -1 });
        verifyR2Down(link, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 1, "+ ( ^0 ) ( _0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 1, 2, 3 }, { -1, -2, -3 });
        verifyR2Down(link, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 1, "+ ( ^0 ) ( _0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 1, 2, 3 }, { -3, -1, -2 });
        verifyR2Down(link, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 1, "+ ( ^0 ) ( _0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 3, 1, 2 }, { -2, -3, -1 });
        verifyR2Down(link, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 1, "+ ( ^0 ) ( _0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 2, 3, 1 }, { -1, -2, -3 });
        verifyR2Down(link, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 0, "+ ( ^0 ) ( _0 )");
        verifyR2Down(link, 0, 1, "+ ( ^0 ) ( _0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { -3, 1, 2 }, { 3, -2, -1 });
        verifyR2Down(link, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 2, -3, 1 }, { -1, 3, -2 });
        verifyR2Down(link, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 1, 2, -3 }, { -2, -1, 3 });
        verifyR2Down(link, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 1, 2, -3 }, { 3, -2, -1 });
        verifyR2Down(link, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { -3, 1, 2 }, { -1, 3, -2 });
        verifyR2Down(link, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 2, -3, 1 }, { -2, -1, 3 });
        verifyR2Down(link, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { -3, 2, 1 }, { 3, -1, -2 });
        verifyR2Down(link, 1, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 1, -3, 2 }, { -2, 3, -1 });
        verifyR2Down(link, 1, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 2, 1, -3 }, { -1, -2, 3 });
        verifyR2Down(link, 1, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 2, 1, -3 }, { 3, -1, -2 });
        verifyR2Down(link, 1, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { -3, 2, 1 }, { -2, 3, -1 });
        verifyR2Down(link, 1, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 1, "+ ( _0 ) ( ^0 )");
    }
    {
        Link link = Link::fromData({ 1, -1, 1 }, { 1, -3, 2 }, { -1, -2, 3 });
        verifyR2Down(link, 1, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 0, 0, "+ ( _0 ) ( ^0 )");
        verifyR2Down(link, 1, 1, "+ ( _0 ) ( ^0 )");
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

        // Note: for R2 with two different strands, the implementation always
        // adds the two new crossings in the order (+, -).

        verifyR2UpClassical(link, -1, 0, 0, -1, 0, 0, "++---+- "
            "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _6 _5 ) ( )");
        verifyR2UpClassical(link, -1, 0, 0, -1, 0, 1, "++---+- "
            "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _6 _5 ) ( )");
        verifyR2UpClassical(link, -1, 0, 1, -1, 0, 0, "++---+- "
            "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _5 _6 ) ( )");
        verifyR2UpClassical(link, -1, 0, 1, -1, 0, 1, "++---+- "
            "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _5 _6 ) ( )");
        verifyR2UpClassical(link, -1, 0, 0, 4, 0, 0, "++---+- "
            "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 _6 _5 ) ( ) ( )");
        verifyR2UpClassical(link, -1, 0, 1, 4, 0, 0, "++---+- "
            "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 _5 _6 ) ( ) ( )");
        verifyR2UpClassical(link, 4, 0, 0, -1, 0, 0, "++---+- "
            "( _6 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ^5 ^6 ) ( ) ( )");
        verifyR2UpClassical(link, 4, 0, 0, -1, 0, 1, "++---+- "
            "( _6 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ^6 ^5 ) ( ) ( )");
        verifyR2UpClassical(link, -1, 0, 0, 4, 1, 1, "++---+- "
            "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _6 _5 _4 ) ( ) ( )");
        verifyR2UpClassical(link, -1, 0, 1, 4, 1, 1, "++---+- "
            "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 _6 _4 ) ( ) ( )");
        verifyR2UpClassical(link, 4, 1, 1, -1, 0, 0, "++---+- "
            "( _5 _6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 ^6 _4 ) ( ) ( )");
        verifyR2UpClassical(link, 4, 1, 1, -1, 0, 1, "++---+- "
            "( _5 _6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^6 ^5 _4 ) ( ) ( )");

        verifyR2UpClassical(link, 4, 1, 1, 1, 0, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 _5 _6 ^0 _2 ^3 ^4 ^6 ^5 _4 ) ( ) ( )");
        verifyR2UpClassical(link, 1, 0, 1, 4, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^6 ^5 ^0 _2 ^3 ^4 _5 _6 _4 ) ( ) ( )");

        verifyR2UpClassical(link, 2, 0, 0, 1, 1, 1, "++---+- "
            "( ) ( _0 ^1 _6 _5 _3 ^2 _1 ^0 _2 ^6 ^5 ^3 ^4 _4 ) ( ) ( )");
        verifyR2UpClassical(link, 1, 1, 1, 2, 0, 0, "++---+- "
            "( ) ( _0 ^1 ^5 ^6 _3 ^2 _1 ^0 _2 _5 _6 ^3 ^4 _4 ) ( ) ( )");

        verifyR2UpClassical(link, 4, 0, 0, 3, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _6 _5 ^4 _4 ^5 ^6 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 0, 0, 3, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _6 _5 ^4 _4 ^6 ^5 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 0, 1, 3, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _5 _6 ^4 _4 ^5 ^6 ) ( ) ( )");
        verifyR2UpClassical(link, 4, 0, 1, 3, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _5 _6 ^4 _4 ^6 ^5 ) ( ) ( )");
        verifyR2UpClassical(link, 3, 1, 0, 4, 0, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^5 ^6 ^4 _4 _6 _5 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 3, 1, 0, 4, 0, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^6 ^5 ^4 _4 _6 _5 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 3, 1, 1, 4, 0, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^5 ^6 ^4 _4 _5 _6 ) ( ) ( )");
        verifyR2UpClassical(link, 3, 1, 1, 4, 0, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^6 ^5 ^4 _4 _5 _6 ) ( ) ( )");

        verifyR2UpVirtualOnly(link, 4, 1, 0, 2, 0, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 _6 _5 ^3 ^4 ^5 ^6 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 1, 0, 2, 0, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 _6 _5 ^3 ^4 ^6 ^5 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 1, 1, 2, 0, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 _5 _6 ^3 ^4 ^5 ^6 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 1, 1, 2, 0, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 _5 _6 ^3 ^4 ^6 ^5 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 2, 0, 0, 4, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^5 ^6 ^3 ^4 _6 _5 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 2, 0, 0, 4, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^6 ^5 ^3 ^4 _6 _5 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 2, 0, 1, 4, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^5 ^6 ^3 ^4 _5 _6 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 2, 0, 1, 4, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^6 ^5 ^3 ^4 _5 _6 _4 ) ( ) ( )");

        verifyR2UpVirtualOnly(link, 4, 1, 0, 3, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _6 _5 ^4 ^5 ^6 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 1, 0, 3, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _6 _5 ^4 ^6 ^5 _4 ) ( ) ( )");
        verifyR2UpClassical(link, 4, 1, 1, 3, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _5 _6 ^4 ^5 ^6 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 1, 1, 3, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _5 _6 ^4 ^6 ^5 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 3, 1, 0, 4, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^5 ^6 ^4 _6 _5 _4 ) ( ) ( )");
        verifyR2UpClassical(link, 3, 1, 0, 4, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^6 ^5 ^4 _6 _5 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 3, 1, 1, 4, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^5 ^6 ^4 _5 _6 _4 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 3, 1, 1, 4, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^6 ^5 ^4 _5 _6 _4 ) ( ) ( )");

        verifyR2UpVirtualOnly(link, 4, 1, 0, 4, 0, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 ^6 _4 _6 _5 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 1, 0, 4, 0, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^6 ^5 _4 _6 _5 ) ( ) ( )");
        verifyR2UpClassical(link, 4, 1, 1, 4, 0, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 ^6 _4 _5 _6 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 1, 1, 4, 0, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^6 ^5 _4 _5 _6 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 0, 0, 4, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _6 _5 _4 ^5 ^6 ) ( ) ( )");
        verifyR2UpClassical(link, 4, 0, 0, 4, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _6 _5 _4 ^6 ^5 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 0, 1, 4, 1, 0, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 _6 _4 ^5 ^6 ) ( ) ( )");
        verifyR2UpVirtualOnly(link, 4, 0, 1, 4, 1, 1, "++---+- "
            "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 _6 _4 ^6 ^5 ) ( ) ( )");
    }

    // Virtual R2 moves that operate on the same strand:
    {
        Link link = Link(2);
        verifyR2UpVirtual(link, -1, 0, 0, 0, "+- ( _0 _1 ^0 ^1 ) ( )");
        verifyR2UpVirtual(link, -1, 0, 0, 1, "-+ ( ^0 ^1 _0 _1 ) ( )");
        verifyR2UpVirtual(link, -1, 0, 1, 0, "-+ ( _0 _1 ^0 ^1 ) ( )");
        verifyR2UpVirtual(link, -1, 0, 1, 1, "+- ( ^0 ^1 _0 _1 ) ( )");
    }
    {
        Link link = ExampleLink::trefoil();
        verifyR2UpVirtual(link, 0, 1, 0, 0,
            "++++- ( ^0 _3 _4 ^3 ^4 _1 ^2 _0 ^1 _2 )");
        verifyR2UpVirtual(link, 0, 1, 0, 1,
            "+++-+ ( ^0 ^3 ^4 _3 _4 _1 ^2 _0 ^1 _2 )");
        verifyR2UpVirtual(link, 0, 1, 1, 0,
            "+++-+ ( ^0 _3 _4 ^3 ^4 _1 ^2 _0 ^1 _2 )");
        verifyR2UpVirtual(link, 0, 1, 1, 1,
            "++++- ( ^0 ^3 ^4 _3 _4 _1 ^2 _0 ^1 _2 )");
    }
    {
        Link link = Link::fromData({ 1 }, { 1, -1 });
        verifyR2UpVirtual(link, 0, 1, 0, 0, "++- ( ^0 _1 _2 ^1 ^2 _0 )");
        verifyR2UpVirtual(link, 0, 1, 0, 1, "+-+ ( ^0 ^1 ^2 _1 _2 _0 )");
        verifyR2UpVirtual(link, 0, 1, 1, 0, "+-+ ( ^0 _1 _2 ^1 ^2 _0 )");
        verifyR2UpVirtual(link, 0, 1, 1, 1, "++- ( ^0 ^1 ^2 _1 _2 _0 )");
        verifyR2UpVirtual(link, 0, 0, 0, 0, "++- ( ^0 _0 _1 _2 ^1 ^2 )");
        verifyR2UpVirtual(link, 0, 0, 0, 1, "+-+ ( ^0 _0 ^1 ^2 _1 _2 )");
        verifyR2UpVirtual(link, 0, 0, 1, 0, "+-+ ( ^0 _0 _1 _2 ^1 ^2 )");
        verifyR2UpVirtual(link, 0, 0, 1, 1, "++- ( ^0 _0 ^1 ^2 _1 _2 )");
    }
    {
        Link link = Link::fromData({ 1 }, { 1 }, { -1 });
        verifyR2UpVirtual(link, 0, 1, 0, 0, "++- ( ^0 _1 _2 ^1 ^2 ) ( _0 )");
        verifyR2UpVirtual(link, 0, 1, 0, 1, "+-+ ( ^0 ^1 ^2 _1 _2 ) ( _0 )");
        verifyR2UpVirtual(link, 0, 1, 1, 0, "+-+ ( ^0 _1 _2 ^1 ^2 ) ( _0 )");
        verifyR2UpVirtual(link, 0, 1, 1, 1, "++- ( ^0 ^1 ^2 _1 _2 ) ( _0 )");
        verifyR2UpVirtual(link, 0, 0, 0, 0, "++- ( ^0 ) ( _0 _1 _2 ^1 ^2 )");
        verifyR2UpVirtual(link, 0, 0, 0, 1, "+-+ ( ^0 ) ( _0 ^1 ^2 _1 _2 )");
        verifyR2UpVirtual(link, 0, 0, 1, 0, "+-+ ( ^0 ) ( _0 _1 _2 ^1 ^2 )");
        verifyR2UpVirtual(link, 0, 0, 1, 1, "++- ( ^0 ) ( _0 ^1 ^2 _1 _2 )");
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

static void verifyResolveViaJones(const Link& link, const char* name) {
    // Keep this test very fast, since for a link of size n we are computing
    // 3n Jones polynomials, each of which takes exponential time.
    if (link.size() > 10)
        return;

    SCOPED_TRACE_CSTRING(name);

    regina::Laurent<regina::Integer> minusCoeff { 2, { -1 } };
    regina::Laurent<regina::Integer> plusCoeff { -2, { 1 } };
    regina::Laurent<regina::Integer> resolveCoeff { -1, { -1, 0, 1 } };

    for (size_t i = 0; i < link.size(); ++i) {
        SCOPED_TRACE_NUMERIC(i);

        // Verify the skein relation for the Jones polynomial around crossing i.
        Link minus = link;
        Link plus = link;
        Link resolve = link;

        if (link.crossing(i)->sign() > 0)
            minus.change(minus.crossing(i));
        else
            plus.change(plus.crossing(i));
        resolve.resolve(resolve.crossing(i));

        EXPECT_EQ(resolveCoeff * resolve.jones(),
            plusCoeff * plus.jones() + minusCoeff * minus.jones());
    }
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

    testManualCases(verifyResolveViaJones);
}

static void verifyMakeVirtual(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (size_t i = 0; i < link.size(); ++i) {
        Link alt(link, false);
        alt.makeVirtual(alt.crossing(i));

        EXPECT_TRUE(isConsistent(alt));
        EXPECT_EQ(alt.size(), link.size() - 1);
        EXPECT_EQ(alt.countComponents(), link.countComponents());

        if (auto untwist = link.withR1(link.crossing(i))) {
            // This operation just undoes the twist.
            EXPECT_EQ(alt, untwist);
        } else if (link.isClassical()) {
            // For classical diagrams, this operation should break the
            // checkerboard colouring - this means we add a new handle to the
            // surface in which the diagram is embedded.
            EXPECT_FALSE(alt.isClassical());
            EXPECT_EQ(alt.virtualGenus(), 1);
        }
        // I have no idea how we expect the virtual genus to change when the
        // input is already a virtual link diagram.
    }

    Link alt(link, false);
    alt.makeVirtual(nullptr);
    EXPECT_EQ(alt, link);
}

TEST_F(LinkTest, makeVirtual) {
    testManualCases(verifyMakeVirtual);
}

static void verifyGraft(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    size_t nTrivial = link.countTrivialComponents();

    if (nTrivial < 2) {
        Link mod(link, false);
        EXPECT_THROW({ mod.graft({}, {}); }, InvalidArgument);
        EXPECT_EQ(mod, link);
    } else {
        // In theory the trivial components could be reordered in the
        // comparison below, but this doesn't happen in our test suite.
        Link mod(link, false);
        mod.graft({}, {});
        EXPECT_EQ(addTrivialComponents(mod, 1), link);
    }

    for (size_t i = 0; i < 2 * link.size(); ++i) {
        if (nTrivial == 0) {
            Link mod(link, false);
            EXPECT_THROW({ mod.graft(mod.strand(i), {}); }, InvalidArgument);
            EXPECT_EQ(mod, link);
            EXPECT_THROW({ mod.graft({}, mod.strand(i)); }, InvalidArgument);
            EXPECT_EQ(mod, link);
        } else {
            // In theory the trivial components could be reordered in the
            // comparisons below, but this doesn't happen in our test suite.
            Link mod1(link, false);
            mod1.graft(mod1.strand(i), {});
            EXPECT_EQ(addTrivialComponents(mod1, 1), link);

            Link mod2(link, false);
            mod2.graft({}, mod1.strand(i));
            EXPECT_EQ(mod2, mod1);
        }

        {
            Link mod(link, false);
            mod.graft(mod.strand(i), mod.strand(i));
            EXPECT_EQ(addTrivialComponents(link, 1), mod);
        }

        StrandRef comp1 = link.component(link.strand(i));

        for (size_t j = 0; j < 2 * link.size(); ++j) {
            if (j == i)
                continue;

            StrandRef comp2 = link.component(link.strand(j));

            Link mod(link, false);
            mod.graft(mod.strand(i), mod.strand(j));
            EXPECT_TRUE(isConsistent(mod));
            EXPECT_EQ(mod.size(), link.size());
            if (comp1 == comp2)
                EXPECT_EQ(mod.countComponents(), link.countComponents() + 1);
            else
                EXPECT_EQ(mod.countComponents() + 1, link.countComponents());

            // When undoing the graft, we should get back the original link
            // diagram but possibly with different starting points for the
            // link components.
            Link undo(mod, false);
            undo.graft(undo.strand(i), undo.strand(j));
            EXPECT_TRUE(isConsistent(undo));
            EXPECT_EQ(undo.sig(false, false, false),
                link.sig(false, false, false));;
        }
    }
}

TEST_F(LinkTest, graft) {
    testManualCases(verifyGraft, false /* gordian */);
}

static void verifySig(const Link& link, bool reflect, bool reverse,
        bool rotate) {
    SCOPED_TRACE_NUMERIC(reflect);
    SCOPED_TRACE_NUMERIC(reverse);
    SCOPED_TRACE_NUMERIC(rotate);

    std::string sig = link.sig(reflect, reverse, rotate);
    EXPECT_FALSE(sig.empty());

    if (reflect) {
        Link alt(link, false);
        alt.reflect();
        EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);
    }
    if (reverse) {
        Link alt(link, false);
        alt.reverse();
        EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);

        for (size_t i = 1; i < alt.countComponents(); ++i) {
            alt.reverse(alt.component(i));
            EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);
        }
    }
    if (rotate) {
        Link alt(link, false);
        alt.rotate();
        EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);
    }
    if (reflect && rotate) {
        Link alt(link, false);
        alt.reflect();
        alt.rotate();
        EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);
    }
    if (reflect && reverse) {
        Link alt(link, false);
        alt.reflect();
        alt.reverse();
        EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);

        for (size_t i = 1; i < alt.countComponents(); ++i) {
            alt.reverse(alt.component(i));
            EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);
        }
    }
    if (rotate && reverse) {
        Link alt(link, false);
        alt.rotate();
        alt.reverse();
        EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);

        for (size_t i = 1; i < alt.countComponents(); ++i) {
            alt.reverse(alt.component(i));
            EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);
        }
    }
    if (reflect && rotate && reverse) {
        Link alt(link, false);
        alt.reflect();
        alt.rotate();
        alt.reverse();
        EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);

        for (size_t i = 1; i < alt.countComponents(); ++i) {
            alt.reverse(alt.component(i));
            EXPECT_EQ(alt.sig(reflect, reverse, rotate), sig);
        }
    }

    Link recon;
    ASSERT_NO_THROW({ recon = Link::fromSig(sig); });

    EXPECT_EQ(recon.size(), link.size());
    EXPECT_EQ(recon.countComponents(), link.countComponents());
    EXPECT_EQ(recon.virtualGenus(), link.virtualGenus());
    EXPECT_EQ(recon.linking2(), link.linking2());
    if (recon.countComponents() == 1 && link.countComponents() == 1) {
        if (reflect)
            EXPECT_EQ(std::abs(recon.oddWrithe()), std::abs(link.oddWrithe()));
        else
            EXPECT_EQ(recon.oddWrithe(), link.oddWrithe());
    }
    EXPECT_EQ(recon.sig(reflect, reverse, rotate), sig);
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
            EXPECT_EQ(jonesModReflection(recon), jonesModReflection(link));
        } else {
            EXPECT_EQ(recon.jones(), link.jones());
        }
    }

    // Verify the "magic" string constructor.
    EXPECT_NO_THROW({ EXPECT_EQ(Link(sig), recon); });
}

static void verifySig(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    verifySig(link, true, true, true);
    verifySig(link, true, false, true);
    verifySig(link, false, true, true);
    verifySig(link, false, false, true);
    verifySig(link, true, true, false);
    verifySig(link, true, false, false);
    verifySig(link, false, true, false);
    verifySig(link, false, false, false);
}

TEST_F(LinkTest, sig) {
    testManualCases(verifySig);

    // Test signatures that respect / ignore reflection:
    EXPECT_EQ(trefoilRight.link.sig(true, true),  "dabcabcv-");
    EXPECT_EQ(trefoilRight.link.sig(false, true), "dabcabcv-");
    EXPECT_EQ(trefoilLeft.link.sig(true, true),   "dabcabcv-");
    EXPECT_EQ(trefoilLeft.link.sig(false, true) , "dabcabcva");

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
    EXPECT_EQ(asymmetric.sig(true, true),   "gaabcdefbcfedPQ--");
    EXPECT_EQ(asymmetric.sig(true, false),  "gaabcdefdcbefPQ--");
    EXPECT_EQ(asymmetric.sig(false, true),  "gaabcdefbcfedPQaa");
    EXPECT_EQ(asymmetric.sig(false, false), "gaabcdefdcbefPQaa");

    // For the Hopf link, reversing one component is the same as reflection.
    {
        Link hopfNegative = ExampleLink::hopf(); // positive crossings
        hopfNegative.reflect();

        EXPECT_EQ(hopfNegative.sig(true, true), "cabcabjp");
        EXPECT_EQ(hopfNegative.sig(true, false), "cabcabjp");
        EXPECT_EQ(hopfNegative.sig(false, true), "cabcabjp");
        EXPECT_EQ(hopfNegative.sig(false, false), "cabcabja");
    }

    // The virtual trefoil is the same under rotation but not reflection.
    {
        Link link = ExampleLink::virtualTrefoil();
        Link rot = link;
        rot.rotate();
        Link ref = link;
        ref.reflect();
        Link both = rot;
        both.reflect();

        EXPECT_EQ(link.sig(true, true, true), "cababdp");
        EXPECT_EQ(rot.sig(true, true, true), "cababdp");
        EXPECT_EQ(ref.sig(true, true, true), "cababdp");
        EXPECT_EQ(both.sig(true, true, true), "cababdp");

        EXPECT_EQ(link.sig(true, true, false), "cababdp");
        EXPECT_EQ(rot.sig(true, true, false), "cababdp");
        EXPECT_EQ(ref.sig(true, true, false), "cababdp");
        EXPECT_EQ(both.sig(true, true, false), "cababdp");

        EXPECT_EQ(link.sig(false, true, true), "cababdp");
        EXPECT_EQ(rot.sig(false, true, true), "cababdp");
        EXPECT_EQ(ref.sig(false, true, true), "cababda"); // different
        EXPECT_EQ(both.sig(false, true, true), "cababda"); // different

        EXPECT_EQ(link.sig(false, true, false), "cababdp");
        EXPECT_EQ(rot.sig(false, true, false), "cababdp");
        EXPECT_EQ(ref.sig(false, true, false), "cababda"); // different
        EXPECT_EQ(both.sig(false, true, false), "cababda"); // different
    }

    // The GPV virtual knot gives four different sigs under all four
    // reflection/rotation options:
    {
        Link link = ExampleLink::gpv();
        Link rot = link;
        rot.rotate();
        Link ref = link;
        ref.reflect();
        Link both = rot;
        both.reflect();

        EXPECT_EQ(link.sig(true, true, true), "eabacdcdbZa-d");
        EXPECT_EQ(rot.sig(true, true, true), "eabacdcdbZa-d");
        EXPECT_EQ(ref.sig(true, true, true), "eabacdcdbZa-d");
        EXPECT_EQ(both.sig(true, true, true), "eabacdcdbZa-d");

        EXPECT_EQ(link.sig(true, true, false), "eabcbcdadZa-d"); // different
        EXPECT_EQ(rot.sig(true, true, false), "eabacdcdbZa-d");
        EXPECT_EQ(ref.sig(true, true, false), "eabcbcdadZa-d"); // different
        EXPECT_EQ(both.sig(true, true, false), "eabacdcdbZa-d");

        EXPECT_EQ(link.sig(false, true, true), "eabacdcdbZaaa"); // different
        EXPECT_EQ(rot.sig(false, true, true), "eabacdcdbZaaa"); // different
        EXPECT_EQ(ref.sig(false, true, true), "eabacdcdbZa-d");
        EXPECT_EQ(both.sig(false, true, true), "eabacdcdbZa-d");

        // Four different signatures here:
        EXPECT_EQ(link.sig(false, true, false), "eabcbcdadZaaa");
        EXPECT_EQ(rot.sig(false, true, false), "eabacdcdbZaaa");
        EXPECT_EQ(ref.sig(false, true, false), "eabcbcdadZa-d");
        EXPECT_EQ(both.sig(false, true, false), "eabacdcdbZa-d");
    }

    // The Kishino knot is symmetric under both reflection and rotation, if we
    // allow reversal.  If we do not allow reversal, then each of the three
    // reflection/reversal/rotation operations are equivalent to the others.
    {
        Link link = ExampleLink::kishino();
        Link rot = link;
        rot.rotate();
        Link ref = link;
        ref.reflect();
        Link both = rot;
        both.reflect();

        EXPECT_EQ(link.sig(true, true, true), "eabacdcdblbTa");
        EXPECT_EQ(rot.sig(true, true, true), "eabacdcdblbTa");
        EXPECT_EQ(ref.sig(true, true, true), "eabacdcdblbTa");
        EXPECT_EQ(both.sig(true, true, true), "eabacdcdblbTa");

        EXPECT_EQ(link.sig(true, true, false), "eabacdcdblbTa");
        EXPECT_EQ(rot.sig(true, true, false), "eabacdcdblbTa");
        EXPECT_EQ(ref.sig(true, true, false), "eabacdcdblbTa");
        EXPECT_EQ(both.sig(true, true, false), "eabacdcdblbTa");

        EXPECT_EQ(link.sig(false, true, true), "eabacdcdblbTa");
        EXPECT_EQ(rot.sig(false, true, true), "eabacdcdblbTa");
        EXPECT_EQ(ref.sig(false, true, true), "eabacdcdblbTa");
        EXPECT_EQ(both.sig(false, true, true), "eabacdcdblbTa");

        EXPECT_EQ(link.sig(false, true, false), "eabacdcdblbTa");
        EXPECT_EQ(rot.sig(false, true, false), "eabacdcdblbTa");
        EXPECT_EQ(ref.sig(false, true, false), "eabacdcdblbTa");
        EXPECT_EQ(both.sig(false, true, false), "eabacdcdblbTa");

        EXPECT_EQ(link.sig(false, false, false), "eabcbcdadTalb"); // different
        EXPECT_EQ(rot.sig(false, false, false), "eabacdcdblbTa");
        EXPECT_EQ(ref.sig(false, false, false), "eabacdcdblbTa");
        EXPECT_EQ(both.sig(false, false, false), "eabcbcdadTalb"); // different
    }

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

    // Add some hard-coded classical link signatures and virtual knot/link
    // signatures (both of which are new to Regina 7.4), to ensure that nothing
    // changes as we optimise the underlying algorithms in later releases.
    // Unless indicated otherwise, all signatures below were computed using
    // Regina 7.4.
    EXPECT_EQ(empty.link.sig(), "_"); // verified by hand
    EXPECT_EQ(hopf.link.sig(), "cabcabjp"); // verified by hand
    EXPECT_EQ(whitehead.link.sig(), "fabcadefbcedvfpd"); // verified by hand
    EXPECT_EQ(borromean.link.sig(), "gabcdgaecfgbfdeLwto"); // verified by hand
    EXPECT_EQ(trefoil_unknot_overlap.link.sig(), "fabcdeadefbcxb7h");
    EXPECT_EQ(adams6_28.link.sig(), "gabcadefdgbcefvv--"); // verified by hand

    EXPECT_EQ(unlink2_0.link.sig(), "aa"); // verified by hand
    EXPECT_EQ(unlink3_0.link.sig(), "aaa"); // verified by hand
    EXPECT_EQ(unlink2_r2.link.sig(), "cabcabdf");
    EXPECT_EQ(unlink2_r1r1.link.sig(), "baabdbaaba");
    EXPECT_EQ(trefoil_unknot0.link.sig(), "dabcabcv-a");
    EXPECT_EQ(trefoil_unknot1.link.sig(), "dabcabcv-baaba");

    EXPECT_EQ(virtualTrefoil.link.sig(), "cababdp");
    EXPECT_EQ(kishino.link.sig(), "eabacdcdblbTa");
    EXPECT_EQ(gpv.link.sig(), "eabacdcdbZa-d");
    EXPECT_EQ(virtualLink2.link.sig(), "bababd");
    EXPECT_EQ(virtualLink3.link.sig(), "cabcacbjp");
    EXPECT_EQ(virtualTrefoilx2.link.sig(),
        "mabcadefghcijmbkldkijlefghNI8OF4-d");
    EXPECT_EQ(virtualDisconnected.link.sig(), "cabcacbjpcabcabjpcababdp");
}

static void verifyGaussAndDT(const TestCase& test,
        bool testGauss = true, bool testDT = true) {
    SCOPED_TRACE_CSTRING(test.name);
    ASSERT_EQ(test.link.countComponents(), 1);

    if (! test.link.isClassical()) {
        // Verify that Gauss and D-T codes both fail as expected.
        EXPECT_THROW({ test.link.dt(); }, regina::NotImplemented);
        EXPECT_THROW({ test.link.dtData(); }, regina::NotImplemented);

        std::string code = test.link.gauss();
        EXPECT_THROW({ Link::fromGauss(code); }, InvalidArgument);
        EXPECT_THROW({ Link link(code); }, InvalidArgument);

        auto data = test.link.gaussData();
        EXPECT_THROW({ Link::fromGauss(data.begin(), data.end()); },
            InvalidArgument);

        return;
    }

    // For "non-composite-like" knot diagrams, the only possible ambiguity
    // is reflection.  Use the reflection-distinguishing knot signature to
    // tell whether we reflected upon reconstruction.
    std::string targetSig = test.link.sig(false);

    if (testGauss) {
        std::string code = test.link.gauss();
        auto data = test.link.gaussData();

        Link recon;
        ASSERT_NO_THROW({ recon = Link::fromGauss(code); });
        Link reconData;
        ASSERT_NO_THROW({ reconData = Link::fromGauss(
            data.begin(), data.end()); });
        EXPECT_EQ(recon, reconData);

        EXPECT_EQ(recon.size(), test.link.size());
        EXPECT_EQ(recon.countComponents(), test.link.countComponents());
        EXPECT_TRUE(recon.isClassical());

        // Verify the "magic" string constructor.
        EXPECT_NO_THROW({ EXPECT_EQ(Link(code), recon); });

        // If we reflected, undo this for our subsequent tests.
        if (recon.sig(false) != targetSig)
            recon.reflect();

        EXPECT_EQ(recon.sig(false), targetSig);
        if (test.link.size() <= JONES_THRESHOLD)
            EXPECT_EQ(recon.jones(), test.link.jones());
    }

    if (testDT) {
        for (int alpha = 0; alpha <= 1; ++alpha) {
            if (alpha && test.link.size() > 26) {
                EXPECT_THROW({ test.link.dt(alpha); }, regina::NotImplemented);
                continue;
            }

            std::string code = test.link.dt(alpha);
            auto data = test.link.dtData();

            Link recon;
            ASSERT_NO_THROW({ recon = Link::fromDT(code); });
            Link reconData;
            ASSERT_NO_THROW({ reconData = Link::fromDT(
                data.begin(), data.end()); });
            EXPECT_EQ(recon, reconData);

            EXPECT_EQ(recon.size(), test.link.size());
            EXPECT_EQ(recon.countComponents(), test.link.countComponents());
            EXPECT_TRUE(recon.isClassical());

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
            if (recon.sig(false) != targetSig)
                recon.reflect();

            EXPECT_EQ(recon.sig(false), targetSig);
            if (test.link.size() <= JONES_THRESHOLD)
                EXPECT_EQ(recon.jones(), test.link.jones());
        }
    }
}

static void verifyGaussPlanarity(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    if (link.countComponents() != 1) {
        EXPECT_THROW({ link.gauss(); }, regina::NotImplemented);
    } else {
        // Note: a non-classical diagram _could_ have the same Gauss code as a
        // classical diagram (same order of crossings but different crossing
        // signs).
        std::string code = link.gauss();
        SCOPED_TRACE_STDSTRING(code);
        try {
            Link recon = Link::fromGauss(code);
            EXPECT_EQ(recon.gauss(), code);
            EXPECT_TRUE(recon.isClassical());
        } catch (const InvalidArgument&) {
            EXPECT_FALSE(link.isClassical());
        }
    }
}

static void verifyDTPlanarity(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    if (link.countComponents() != 1) {
        EXPECT_THROW({ link.dt(); }, regina::NotImplemented);
    } else if (! link.isClassical()) {
        EXPECT_THROW({ link.dt(); }, regina::NotImplemented);
    } else {
        std::string code = link.dt();
        SCOPED_TRACE_STDSTRING(code);
        EXPECT_NO_THROW({
            Link recon = Link::fromDT(code);
            EXPECT_EQ(recon.dt(), code);
            EXPECT_TRUE(recon.isClassical());
        });
    }
}

static void verifyDTPlanarityPermuting(std::string code) {
    // The input code should be alphabetical and sorted (i.e., "abcd..").
    // Currently this test only supports ≤ 5 crossings.
    ASSERT_LE(code.length(), 5);

    // All of the non-planar D-T codes with ≤ 5 crossings (here we ignore
    // upper/lower strands and always use lower-case letters):
    const std::array<std::string, 7> nonPlanar {
        // These five are relabellings of the same 4-valent graph:
        "bdaec", "bedac", "cedba", "dcaeb", "dceba",
        // These two are relabellings of the same 4-valent graph:
        "bcdea", "deabc"
    };

    do {
        SCOPED_TRACE_STDSTRING(code);
        bool expectPlanar = (std::find(nonPlanar.begin(), nonPlanar.end(), code)
            == nonPlanar.end());
        if (expectPlanar) {
            EXPECT_NO_THROW({
                Link recon = Link::fromDT(code);
                EXPECT_TRUE(recon.isClassical());
                EXPECT_EQ(recon.dt(true), code);
            });
        } else {
            EXPECT_THROW({ Link::fromDT(code); }, InvalidArgument);
        }
    } while (std::next_permutation(code.begin(), code.end()));
}

TEST_F(LinkTest, gaussAndDT) {
    // We test Gauss and D-T codes together, since they both have the same
    // ambiguity with knot composition.

    // The following cases are precisely those single-component classical
    // knots without ambiguous Gauss/D-T codes.
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
    verifyGaussAndDT(trefoil_r1x2, true, true);
    verifyGaussAndDT(figureEight_r1x2, true, true);
    verifyGaussAndDT(rht_rht, true, true);

    // Virtual knots do not play well with Gauss / D-T codes, but we should
    // verify this.
    verifyGaussAndDT(virtualTrefoil);
    verifyGaussAndDT(kishino);
    verifyGaussAndDT(gpv);

    // Verify that Gauss and D-T codes behave as expect wrt planarity.
    runCensusAllVirtual(verifyGaussPlanarity);
    runCensusAllVirtual(verifyDTPlanarity);
    verifyDTPlanarityPermuting("a");
    verifyDTPlanarityPermuting("ab");
    verifyDTPlanarityPermuting("abc");
    verifyDTPlanarityPermuting("abcd");
    verifyDTPlanarityPermuting("abcde");
}

static void verifyOrientedGauss(const Link& link, const char* name) {
    if (link.countComponents() != 1)
        return;

    SCOPED_TRACE_CSTRING(name);

    std::string code = link.orientedGauss();
    auto data = link.orientedGaussData();

    Link recon;
    ASSERT_NO_THROW({ recon = Link::fromOrientedGauss(code); });
    Link reconData;
    ASSERT_NO_THROW({ reconData = Link::fromOrientedGauss(
        data.begin(), data.end()); });

    // Oriented gauss codes reconstruct the labelling precisely.
    EXPECT_EQ(recon, link);
    EXPECT_EQ(recon.orientedGauss(), code);
    EXPECT_EQ(reconData, link);
    EXPECT_EQ(reconData.orientedGaussData(), data);

    // Verify the "magic" string constructor.
    EXPECT_NO_THROW({ EXPECT_EQ(Link(code), recon); });
}

TEST_F(LinkTest, orientedGauss) {
    testManualCases(verifyOrientedGauss);
}

static void verifySignedGauss(const Link& link, const char* name) {
    if (link.countComponents() != 1)
        return;

    SCOPED_TRACE_CSTRING(name);

    std::string code = link.signedGauss();
    auto data = link.signedGaussData();

    Link recon;
    ASSERT_NO_THROW({ recon = Link::fromSignedGauss(code); });
    Link reconData;
    ASSERT_NO_THROW({ reconData = Link::fromSignedGauss(
        data.begin(), data.end()); });

    // Signed gauss codes reconstruct the labelling precisely.
    EXPECT_EQ(recon, link);
    EXPECT_EQ(recon.signedGauss(), code);
    EXPECT_EQ(reconData, link);
    EXPECT_EQ(reconData.signedGaussData(), data);

    // Verify that lower-case signed Gauss codes are handled also.
    std::string lower = code;
    for (char& c : lower)
        c = ::tolower(c);
    if (link.size() == 0)
        EXPECT_EQ(lower, code); // no crossings, so no letters at all
    else
        EXPECT_NE(lower, code);

    Link reconLower;
    ASSERT_NO_THROW({ reconLower = Link::fromSignedGauss(lower); });
    EXPECT_EQ(reconLower, link);
    EXPECT_EQ(reconLower.signedGauss(), code);

    // Verify the "magic" string constructor.
    EXPECT_NO_THROW({ EXPECT_EQ(Link(code), recon); });
}

TEST_F(LinkTest, signedGauss) {
    testManualCases(verifySignedGauss);
}

static void verifyJenkins(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    std::string code = link.jenkins();
    auto data = link.jenkinsData();

    Link recon;
    ASSERT_NO_THROW({ recon = Link::fromJenkins(code); });
    Link reconData;
    ASSERT_NO_THROW({ reconData = Link::fromJenkins(
        data.begin(), data.end()); });

    // Jenkins format reconstructs the labelling precisely.
    EXPECT_EQ(recon, link);
    EXPECT_EQ(recon.jenkins(), code);
    EXPECT_EQ(reconData, link);
    EXPECT_EQ(reconData.jenkinsData(), data);
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
    auto data = link.pdData();

    Link recon;
    ASSERT_NO_THROW({ recon = Link::fromPD(code); });
    Link reconData;
    ASSERT_NO_THROW({ reconData = Link::fromPD(data.begin(), data.end()); });
    EXPECT_EQ(recon, reconData);

    EXPECT_EQ(recon.size(), link.size());
    EXPECT_EQ(recon.countComponents() + lost, link.countComponents());
    EXPECT_EQ(recon.writhe(), link.writhe());
    EXPECT_EQ(recon.linking2(), link.linking2());
    if (recon.countComponents() == 1 && link.countComponents() == 1)
        EXPECT_EQ(recon.oddWrithe(), link.oddWrithe());

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

    EXPECT_THROW({ Link::fromSig(code); }, InvalidArgument);
    EXPECT_THROW({ Link::fromGauss(code); }, InvalidArgument);
    EXPECT_THROW({ Link::fromDT(code); }, InvalidArgument);
    EXPECT_THROW({ Link::fromOrientedGauss(code); }, InvalidArgument);
    EXPECT_THROW({ Link::fromJenkins(code); }, InvalidArgument);
    EXPECT_THROW({ Link::fromPD(code); }, InvalidArgument);

    // Finally, the "magic" constructor:
    EXPECT_THROW({ Link l(code); }, InvalidArgument);
}

static void verifyRewriteClassical(const TestCase& test, int height,
        int threads, bool track, size_t expectCount) {
    SCOPED_TRACE_CSTRING(test.name);
    SCOPED_TRACE_NUMERIC(height);
    SCOPED_TRACE_NUMERIC(threads);

    size_t count = 0;
    auto jones = jonesModReflection(test.link);
    size_t initComp = test.link.countDiagramComponents();

    std::unique_ptr<regina::ProgressTrackerOpen> tracker;
    if (track)
        tracker.reset(new regina::ProgressTrackerOpen());

    bool result = test.link.rewrite(height, threads, tracker.get(),
            [&count, &jones, initComp](const Link& alt) {
        ++count;
        EXPECT_EQ(jonesModReflection(alt), jones);
        EXPECT_GE(alt.countDiagramComponents(), initComp);
        return false;
    });
    if (track)
        EXPECT_TRUE(tracker->isFinished());
    EXPECT_FALSE(result);
    EXPECT_EQ(count, expectCount);
}

static void verifyRewriteVirtual(const TestCase& test, int height,
        int threads, bool track, size_t expectCount) {
    SCOPED_TRACE_CSTRING(test.name);
    SCOPED_TRACE_NUMERIC(height);
    SCOPED_TRACE_NUMERIC(threads);

    size_t count = 0;
    auto jones = jonesModReflection(test.link);
    size_t initComp = test.link.countDiagramComponents();

    std::unique_ptr<regina::ProgressTrackerOpen> tracker;
    if (track)
        tracker.reset(new regina::ProgressTrackerOpen());

    bool result = test.link.rewriteVirtual(height, threads, tracker.get(),
            [&count, &jones, initComp](const Link& alt) {
        ++count;
        EXPECT_EQ(jonesModReflection(alt), jones);
        EXPECT_GE(alt.countDiagramComponents(), initComp);
        return false;
    });
    if (track)
        EXPECT_TRUE(tracker->isFinished());
    EXPECT_FALSE(result);
    EXPECT_EQ(count, expectCount);
}

static void verifyRewrite(const TestCase& test, int height,
        size_t expectClassical, size_t expectVirtual) {
    // Single-threaded, no tracker:
    verifyRewriteClassical(test, height, 1, false, expectClassical);
    verifyRewriteVirtual(test, height, 1, false, expectVirtual);
    // Multi-threaded, with and without tracker:
    verifyRewriteClassical(test, height, 2, false, expectClassical);
    verifyRewriteVirtual(test, height, 2, false, expectVirtual);
    verifyRewriteClassical(test, height, 2, true, expectClassical);
    verifyRewriteVirtual(test, height, 2, true, expectVirtual);
}

TEST_F(LinkTest, rewrite) {
    // The classical rewrite counts below were computed using Regina 6.0 in
    // single-threaded mode (except for the zero-crossing and multiple-component
    // cases, which were computed by hand).  All virtual rewrite counts were
    // computed using Regina 7.4 in single-threaded mode (again except for the
    // zero-crossing and multiple component cases, which were computed by hand).

    verifyRewrite(empty, 0, 1, 1);
    verifyRewrite(empty, 1, 1, 1);
    verifyRewrite(empty, 2, 1, 1);
    verifyRewrite(unknot0, 0, 1, 1);
    verifyRewrite(unknot0, 1, 2, 2);
    verifyRewrite(unknot0, 2, 6, 7);
    verifyRewrite(unknot0, 3, 22, 29);
    verifyRewrite(unknot3, 0, 22, 29);
    verifyRewrite(unknot3, 1, 131, 270);
    verifyRewrite(unknot3, 2, 998, 3585);
    verifyRewrite(unlink2_0, 0, 1, 1);
    verifyRewrite(unlink2_0, 1, 2, 2);
    verifyRewrite(unlink2_0, 2, 8, 9);
    verifyRewrite(unlink2_r2, 0, 9, 10);
    verifyRewrite(unlink2_r2, 1, 32, 40);
    verifyRewrite(unlink3_0, 0, 1, 1);
    verifyRewrite(unlink3_0, 1, 2, 2);
    verifyRewrite(unlink3_0, 2, 8, 9);
    verifyRewrite(figureEight, 0, 1, 1);
    verifyRewrite(figureEight, 1, 8, 8);
    verifyRewrite(figureEight, 2, 137, 172);
    verifyRewrite(figureEight_r1x2, 0, 137, 172);
    verifyRewrite(figureEight, 3, 2401, 4184);
    verifyRewrite(figureEight_r1x2, 1, 2401, 4184);
    // verifyRewrite(figureEight_r1x2, 2, 44985);
    verifyRewrite(rht_lht, 0, 1, 1);
    verifyRewrite(rht_lht, 1, 35, 35);
    verifyRewrite(rht_lht, 2, 1131, 1404);

    // All counts below were computed using Regina 7.4 in single-threaded mode.

    verifyRewrite(trefoil_unknot_overlap, 0, 90, 103);
    verifyRewrite(trefoil_unknot_overlap, 1, 1200, 1818);
    verifyRewrite(virtualTrefoil, 0, 1, 1);
    verifyRewrite(virtualTrefoil, 1, 8, 8);
    verifyRewrite(virtualTrefoil, 2, 111, 120);
    verifyRewrite(virtualTrefoil, 3, 1628, 2028);

    // Regina currently does not merge different connected components of a
    // link diagram when running rewrite().  Verify this:
    {
        Link link = ExampleLink::figureEight();
        link.insertLink(ExampleLink::figureEight());
        EXPECT_FALSE(link.isConnected());

        link.rewrite(2, 1, nullptr, [](const Link& alt) {
            EXPECT_FALSE(alt.isConnected());
            return false;
        });
        link.rewriteVirtual(2, 1, nullptr, [](const Link& alt) {
            EXPECT_FALSE(alt.isConnected());
            return false;
        });
    }
}

static void verifySimplifyExhaustive(const char* sig, int heightNeeded) {
    SCOPED_TRACE_CSTRING(sig);

    for (int threads = 1; threads <= 2; ++threads) {
        SCOPED_TRACE_NUMERIC(threads);

        Link link = Link::fromSig(sig);
        size_t initSize = link.size();
        auto initJones = link.jones();

        for (int height = 0; height < heightNeeded; ++height) {
            SCOPED_TRACE_NUMERIC(height);
            EXPECT_FALSE(link.simplifyExhaustive(height, threads));
            EXPECT_EQ(link.size(), initSize);
            EXPECT_EQ(link.jones(), initJones);
        }

        EXPECT_TRUE(link.simplifyExhaustive(heightNeeded, threads));
        EXPECT_LT(link.size(), initSize);
        EXPECT_EQ(link.jones(), initJones);
    }
}

TEST_F(LinkTest, simplifyExhaustive) {
    // Note: For all of these cases, it does not seem to matter whether or not
    // we allow the virtual R2 move.

    // Classical unknot diagrams:
    verifySimplifyExhaustive("kabcdefghijbefgdcjahixfvbdwGd", 0); // Monster

    // Virtual trefoil:
    verifySimplifyExhaustive("gabacdefcdfbelLDp", 1);
    verifySimplifyExhaustive("gabacdefbfcdetPhc", 1);
    verifySimplifyExhaustive("habacdefgedbcfgJQb1pd", 1);
    verifySimplifyExhaustive("habcdefadgbcgfeBubh9a", 1);
    verifySimplifyExhaustive("habacdefbgfgcdetXchla", 1);
    verifySimplifyExhaustive("habacdefgbgdefcBPapec", 1);
    verifySimplifyExhaustive("habacdefgbcfedgtRafcc", 1);

    // GPV virtual knot:
    verifySimplifyExhaustive("habacdbefgedfgcBkbVVd", 1);
    verifySimplifyExhaustive("iabacdedfgchfgheb7sbx3l", 1);
    verifySimplifyExhaustive("iabacdbefghdhefgcBncVhp", 1);
    verifySimplifyExhaustive("iabacdbefghdfghecBKgVUl", 1);
    verifySimplifyExhaustive("iabacdefghgdefchbBPe37j", 1);
    verifySimplifyExhaustive("iabacbdefghchefgdRLcxVh", 1);
    verifySimplifyExhaustive("iabacdefghchefgdbBXc3Jp", 1);
    verifySimplifyExhaustive("iabacdbcefghdhefgBAeVpo", 1);
    verifySimplifyExhaustive("iabacbdefghchefgdlnk3hp", 1);

    // Virtual diagrams of the Hopf link:
    verifySimplifyExhaustive("eabcdadebclcxd", 2);
    verifySimplifyExhaustive("fabacdebefcdtgFn", 2);
    verifySimplifyExhaustive("fabacdebefcdlkFn", 2);
    verifySimplifyExhaustive("fabcdeabefcdxiVn", 2);
    verifySimplifyExhaustive("fabcabdecfdeNi7n", 2);
}

static void verifyImproveTreewidth(const Link& link, const char* name,
        int bestPossible) {
    SCOPED_TRACE_CSTRING(name);

    auto initJones = link.jones();

    for (int threads = 1; threads <= 2; ++threads) {
        SCOPED_TRACE_NUMERIC(threads);

        Link working(link, false);
        size_t currWidth = regina::TreeDecomposition(working).width();

        while (working.improveTreewidth().first) {
            SCOPED_TRACE_NUMERIC(currWidth);

            size_t newWidth = regina::TreeDecomposition(working).width();
            SCOPED_TRACE_NUMERIC(newWidth);
            EXPECT_LT(newWidth, currWidth);
            currWidth = newWidth;

            EXPECT_EQ(Link(working, false).jones(), initJones);
        }

        EXPECT_EQ(currWidth, bestPossible);
        EXPECT_EQ(Link(working, false).jones(), initJones);
    }
}

TEST_F(LinkTest, improveTreewidth) {
    // All of the target widths here were found with Regina 7.4.

    // Unknot diagrams should be able to get down to an empty loop.
    verifyImproveTreewidth(ExampleLink::monster(), "Monster", 0);

    // We are able to drop the doubled trefoil's width from 5 down to 4.
    // We use both the right and left trefoil to ensure that reflections
    // (which should not occur) are detected.
    verifyImproveTreewidth(ExampleLink::trefoilRight().parallel(2),
        "Right trefoil (2-cabled)", 4);
    verifyImproveTreewidth(ExampleLink::trefoilLeft().parallel(2),
        "Left trefoil (2-cabled)", 4);

    // The following example is the 3-cabled virtual knot 3.2 from
    // Jeremy Green's tables.  We use it because it is virtual, its
    // Jones polynomial detects reflection, and it supports a reduction of
    // width from 8 down to 7.
    verifyImproveTreewidth(Link::fromSignedGauss("O1-O2+U1-O3-U2+U3-").
        parallel(3), "3-cabled virtual knot 3.2", 7);
}

static void verifyClassicalGroup(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    regina::GroupPresentation fromLink = link.group();
    regina::GroupPresentation fromComp = link.complement().group();

    EXPECT_EQ(fromLink.abelianRank(), link.countComponents());

    // Verify (as far as possible) that the two groups are isomorphic.
    // We assume here that both groups have been simplified.
    verifyIsomorphic(fromLink, fromComp);
}

// Use this when we should get the same group when viewing from
// above and below the diagram.
static void verifyGroup(const TestCase& test, const GroupPresentation& expect) {
    SCOPED_TRACE_CSTRING(test.name);

    auto found = test.link.groups();
    verifyIsomorphic(found.first, expect);
    verifyIsomorphic(found.second, expect);

    Link flip(test.link);
    flip.changeAll();
    verifyIsomorphic(flip.group(), expect);
}

// Use this when we should get different groups when viewing from
// above and below the diagram.
static void verifyGroups(const TestCase& test,
        const GroupPresentation& expect, const GroupPresentation& expectFlip) {
    SCOPED_TRACE_CSTRING(test.name);

    auto found = test.link.groups();
    verifyIsomorphic(found.first, expect);
    verifyIsomorphic(found.second, expectFlip);

    Link flip(test.link);
    flip.changeAll();
    verifyIsomorphic(flip.group(), expectFlip);
}

TEST_F(LinkTest, group) {
    // Note: the Gordian unknot is too slow for the test suite.
    testManualCases(verifyClassicalGroup, false /* gordian */,
        false /* virtual */);

    // In the tests below, we are currently missing group presentations for:
    // - conway, kinoshitaTerasaka, gst;
    // - whitehead, borromean, adams6_28;
    // - virtualTrefoilx2.
    // We should hunt down independent confirmations of these and then include
    // them in the test suite.

    verifyGroup(unknot0, { 1 });
    verifyGroup(unknot1, { 1 });
    verifyGroup(unknot3, { 1 });
    verifyGroup(unknotMonster, { 1 });
    // verifyGroup(unknotGordian, { 1 });
    verifyGroup(trefoilLeft, { 2, { "aabbb" }});
    verifyGroup(trefoilRight, { 2, { "aabbb" }});
    verifyGroup(trefoil_r1x2, { 2, { "aabbb" }});
    verifyGroup(trefoil_r1x6, { 2, { "aabbb" }});
    verifyGroup(figureEight, { 2, { "AbaBabABaB" }});
    verifyGroup(figureEight_r1x2, { 2, { "AbaBabABaB" }});

    verifyGroup(rht_rht, { 3, { "abaBAB", "acaCAC" }});
    verifyGroup(rht_lht, { 3, { "abaBAB", "acaCAC" }});

    verifyGroup(unlink2_0, { 2 });
    verifyGroup(unlink3_0, { 3 });
    verifyGroup(unlink2_r2, { 2 });
    verifyGroup(unlink2_r1r1, { 2 });
    verifyGroup(hopf, { 2, { "abAB" }});
    verifyGroup(trefoil_unknot0, { 3, { "aabbb" }});
    verifyGroup(trefoil_unknot1, { 3, { "aabbb" }});
    verifyGroup(trefoil_unknot_overlap, { 3, { "aabbb" }});

    verifyGroup(virtualTrefoil, { 1 });
    verifyGroup(kishino, { 1 });
    verifyGroups(gpv, { 2, { "aabbb" }}, { 1 });
    verifyGroup(virtualLink2, { 2, { "abAB" }});
    verifyGroup(virtualLink3, { 3, { "abAB", "acAC" }});
    verifyGroup(virtualDisconnected, { 6, { "abAB", "acAC", "efEF" }});
}

static void verifyExtendedGroupBasic(const Link& link, const char* name) {
    // For now, some very basic tests.
    SCOPED_TRACE_CSTRING(name);

    auto groups = link.extendedGroups();

    EXPECT_EQ(groups.first.abelianRank(), link.countComponents() + 1);
    EXPECT_EQ(groups.second.abelianRank(), link.countComponents() + 1);

    if (link.isClassical()) {
        // I cannot yet locate a reference stating that the extended groups
        // of a classical link and its mirror image are isomorphic.
        // However, it is certainly true for all of the tests that we run here.
        verifyIsomorphic(groups.first, groups.second,
            true /* fast tests only */);
    }
}

// Use this when we should get the same group when viewing from
// above and below the diagram.
static void verifyExtendedGroupTwoSided(Link link, const char* name,
        const GroupPresentation& expect) {
    SCOPED_TRACE_CSTRING(name);

    auto found = link.extendedGroups();
    verifyIsomorphic(found.first, expect);
    verifyIsomorphic(found.second, expect);

    link.changeAll();
    verifyIsomorphic(link.extendedGroup(), expect);
}

// Use this when we might different groups when viewing from above and below
// the diagram, and we do not actually know what to expect from below.
static void verifyExtendedGroupOneSided(const Link& link, const char* name,
        const GroupPresentation& expect) {
    SCOPED_TRACE_CSTRING(name);

    verifyIsomorphic(link.extendedGroup(), expect);
}

TEST_F(LinkTest, extendedGroup) {
    testManualCases(verifyExtendedGroupBasic, false /* gordian */);

    // Some groups for which we know what the answers should be:

    verifyExtendedGroupTwoSided(unknot0.link, unknot0.name, { 2 });
    verifyExtendedGroupTwoSided(unknot1.link, unknot1.name, { 2 });
    verifyExtendedGroupTwoSided(unknot3.link, unknot3.name, { 2 });
    verifyExtendedGroupTwoSided(unknotMonster.link, unknotMonster.name, { 2 });

    verifyExtendedGroupTwoSided(unlink2_0.link, unlink2_0.name, { 3 });
    verifyExtendedGroupTwoSided(unlink3_0.link, unlink3_0.name, { 4 });
    verifyExtendedGroupTwoSided(unlink2_r2.link, unlink2_r2.name, { 3 });
    verifyExtendedGroupTwoSided(unlink2_r1r1.link, unlink2_r1r1.name, { 3 });

    // Example 2.1 of Boden et al., "Alexander invariants for virtual knots",
    // JKTR 24 (2015) gives a presentation of the virtual knot group for the
    // virtual trefoil.  The extended group is obtained from this presentation
    // by setting q=1.
    verifyExtendedGroupTwoSided(virtualTrefoil.link, kishino.name,
        { 2, { "ABBAbABabbaBab" }});

    // Boden et al. also note that the Kishino knot has trivial virtual knot
    // group, which implies a trivial extended knot group.  Here "trivial"
    // means the same as for the unknot, i.e., free on two generators.
    verifyExtendedGroupTwoSided(kishino.link, kishino.name, { 2 });

    // The next group is example 4.5 of Silver and Williams.
    // Regina's computations suggest that the mirror image has a different
    // group, and so we use a one-sided test only.  (Silver and Williams say
    // nothing about mirror images.)
    verifyExtendedGroupOneSided(
        Link::fromData({-1,+1,-1,-1}, {1,-2,-3,4,-1,3,-4,2}),
        "Silver-Williams ex. 4.5", { 3, { "bacBABabbabCABAC", "abcabCABAC" }});
}

static void verifySmallCells(const Link& link, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    const regina::ModelLinkGraph graph(link);
    const auto& cells = graph.cells();

    // Verify that loops(), monogons(), bigons() and triangles() match what we
    // see from the dual cell decomposition.
    for (auto n : graph.nodes()) {
        SCOPED_TRACE_NUMERIC(n->index());

        int foundLoops = 0;
        int foundMonogons = 0;
        int foundBigons = 0;
        int foundTriangles = 0;

        for (int i = 0; i < 4; ++i) {
            size_t cell = cells.cell(n->arc(i));
            switch (cells.size(cell)) {
                case 1:
                    ++foundLoops;
                    ++foundMonogons;
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

        // Account for any loops at n that do not bound 1-gons:
        if (n->adj(0) == n->arc(2))
            ++foundLoops;
        if (n->adj(1) == n->arc(3))
            ++foundLoops;

        EXPECT_EQ(foundLoops, n->loops());
        EXPECT_EQ(foundMonogons, n->monogons());
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
