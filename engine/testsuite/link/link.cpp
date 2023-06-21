
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

using regina::Crossing;
using regina::ExampleLink;
using regina::Link;
using regina::Triangulation;
using regina::StrandRef;

// A link size beneath which we declare it trivial to compute Jones polynomials:
static constexpr int JONES_THRESHOLD = 20;

static constexpr const char* FIG8_COMPLEMENT_SIG = "cPcbbbiht";

static void EXPECT_CONSISTENCY(const Link& link) {
    for (Crossing* c : link.crossings()) {
        StrandRef lower(c, 0);
        StrandRef upper(c, 1);
        if (lower.next().prev() != lower ||
                upper.next().prev() != upper) {
            ADD_FAILURE() << "Inconsistent next/prev links";
            return;
        }
    }
}

static void EXPECT_TREFOIL_COMPLEMENT(const Triangulation<3>& tri) {
    std::string sig = tri.isoSig();

    // Regina's simplification heuristics have been found to produce these
    // trefoil complements in practice:
    for (const char* s : {
            "cPcbbbadh", "cPcbbbadu",
            "dLQbcbcdlcj", "dLQbcbcdlcn", "dLQabccbrwj", "dLQabccbrwn",
            "eLAkbbcddainqv", "eLAkbcbddducqn", "eLAkbcbdddmcxj" })
        if (sig == s)
            return;

    ADD_FAILURE() << "Does not appear to be trefoil complement: " << sig;
}

static void EXPECT_CENSUS_MANIFOLD(const Triangulation<3>& tri,
        const std::string& name) {
    std::string sig = tri.isoSig();
    std::string altName = name + " : ";

    auto hits = regina::Census::lookup(sig);
    for (const auto& hit : hits)
        if (hit.name() == name ||
                hit.name().substr(0, altName.size()) == altName)
            return;

    ADD_FAILURE() << "Does not appear to be " << name << ": " << sig;
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

        Link p = link.parallel(k, regina::FRAMING_BLACKBOARD);
        EXPECT_EQ(p.countComponents(), k * link.countComponents());
        EXPECT_EQ(p.size(), k * k * link.size());
        EXPECT_EQ(p.writhe(), k * k * writhe);
        EXPECT_EQ(p.linking(), k * k * linking + (k * (k-1) * writheSame) / 2);

        p = link.parallel(k, regina::FRAMING_SEIFERT);
        EXPECT_EQ(p.countComponents(), k * link.countComponents());
        EXPECT_EQ(p.size(), k * k * link.size() + k * (k-1) * absWritheSame);
        EXPECT_EQ(p.writhe(), k * k * writhe - k * (k-1) * writheSame);
        EXPECT_EQ(p.linking(), k * k * linking);
    }
}

TEST_F(LinkTest, parallel) {
    testManualCases(verifyParallel);
}

#if 0
static void testJones(const Link& l, const char* expected, const char* name) {
    // Since we are computing the Jones polynomial multiple times
    // (using different algorithms), we work with clones of l
    // that do not clone any already-computed properties.

    if (l.size() <= 40) {
        // The naive algorithm iterates through 2^n states.
        // If n > 40 then we don't have a chance.
        std::ostringstream s1;
        s1 << Link(l, false).jones(regina::ALG_NAIVE);

        EXPECT_EQ(s1.str(), expected);
    }

    std::ostringstream s2;
    s2 << Link(l, false).jones(regina::ALG_TREEWIDTH);

    EXPECT_EQ(s2.str(), expected);
}

void jones() {
    testJones(empty, "0", "Empty");
    testJones(unknot0, "1", "Unknot (0 crossings)");
    testJones(unknot1, "1", "Unknot (1 crossing)");
    testJones(unknot3, "1", "Unknot (3 crossings)");
    testJones(unknotMonster, "1", "Monster unknot");
    // The Gordian unknot is too large to compute V(link).
    testJones(trefoilLeft, "x^-2 + x^-6 - x^-8", "LH Trefoil");
    testJones(trefoilRight, "-x^8 + x^6 + x^2", "RH Trefoil");
    testJones(trefoil_r1x2, "-x^8 + x^6 + x^2", "Trefoil with 2 R1s");
    testJones(trefoil_r1x6, "-x^8 + x^6 + x^2", "Trefoil with 6 R1s");
    testJones(figureEight, "x^4 - x^2 + 1 - x^-2 + x^-4",
        "Figure eight");
    testJones(figureEight_r1x2, "x^4 - x^2 + 1 - x^-2 + x^-4",
        "Figure eight with 2 R1s");
    testJones(unlink2_0, "-x - x^-1", "Unlink (2 components)");
    testJones(unlink3_0, "x^2 + 2 + x^-2", "Unlink (3 components)");
    testJones(unlink2_r2, "-x - x^-1", "Unlink (2 components via R2)");
    testJones(unlink2_r1r1, "-x - x^-1",
        "Unlink (2 components via R1+R1)");
    testJones(hopf, "-x^5 - x", "Hopf link");
    testJones(whitehead, "-x^3 + x - 2 x^-1 + x^-3 - 2 x^-5 + x^-7",
        "Whitehead link");
    testJones(borromean,
        "-x^6 + 3 x^4 - 2 x^2 + 4 - 2 x^-2 + 3 x^-4 - x^-6",
        "Borromean rings");
    testJones(trefoil_unknot0, "x^9 - x^5 - x^3 - x",
        "Trefoil U unknot (separate)");
    testJones(trefoil_unknot1, "x^9 - x^5 - x^3 - x",
        "Trefoil U unknot (separate + twist)");
    testJones(trefoil_unknot_overlap, "x^9 - x^5 - x^3 - x",
        "Trefoil U unknot (with R2)");
    testJones(rht_rht, "x^16 - 2 x^14 + x^12 - 2 x^10 + 2 x^8 + x^4",
        "RH Trefoil # RH Trefoil");
    testJones(rht_lht, "-x^6 + x^4 - x^2 + 3 - x^-2 + x^-4 - x^-6",
        "RH Trefoil # LH Trefoil");

    // The following polynomials were computed using Regina,
    // using the naive algorithm (ALG_NAIVE).
    // This means that they have not been verified independently;
    // here they are simply being used to ensure that different
    // algorithms give the same results, and also that nothing breaks
    // in a subsequent release.
    testJones(conway, "-x^8 + 2 x^6 - 2 x^4 + 2 x^2 + x^-4 - 2 x^-6 + 2 x^-8 - 2 x^-10 + x^-12",
        "Conway knot");
    testJones(kinoshitaTerasaka, "-x^8 + 2 x^6 - 2 x^4 + 2 x^2 + x^-4 - 2 x^-6 + 2 x^-8 - 2 x^-10 + x^-12",
        "Kinoshita-Terasaka knot");

    // The following polynomials were computed using Regina,
    // via the treewidth-based algorithm (ALG_TREEWIDTH).
    // This is because they are too large for the naive
    // algorithm to handle.
    testJones(gst, "-x^32 + 3 x^30 - 4 x^28 + 3 x^26 - x^24 + x^22 - x^20 + x^18 - x^16 + 2 x^14 - 4 x^12 + 3 x^10 - x^8 - 3 x^6 + 5 x^4 - 5 x^2 + 5 - 3 x^-2 + 3 x^-4 - x^-6 + x^-12 - x^-14",
        "GST");
}

static void testHomflyAZ(const Link& l, bool reverse, const char* expected,
        const char* name) {
    // Since we are computing the HOMFLY polynomial multiple times
    // (using different algorithms), we work with clones of l
    // that do not clone any already-computed properties.

    std::ostringstream s1;
    {
        Link use(l, false);
        if (reverse)
            use.reverse();
        s1 << use.homflyAZ(regina::ALG_BACKTRACK);
    }

    EXPECT_EQ(s1.str(), expected);

    std::ostringstream s2;
    {
        Link use(l, false);
        if (reverse)
            use.reverse();
        s2 << use.homflyAZ(regina::ALG_TREEWIDTH);
    }

    EXPECT_EQ(s2.str(), expected);
}

static void testHomflyAZ(const Link& l, const char* expected,
        const char* name) {
    testHomflyAZ(l, false, expected, name);
    testHomflyAZ(l, true, expected, name);
}

static void testHomflyLM(const Link& l, bool reverse, const char* expected,
        const char* name) {
    std::ostringstream s1;
    {
        Link use(l, false);
        if (reverse)
            use.reverse();
        s1 << use.homflyLM(regina::ALG_BACKTRACK);
    }

    EXPECT_EQ(s1.str(), expected);

    std::ostringstream s2;
    {
        Link use(l, false);
        if (reverse)
            use.reverse();
        s2 << use.homflyLM(regina::ALG_TREEWIDTH);
    }

    EXPECT_EQ(s2.str(), expected);
}

static void testHomflyLM(const Link& l, const char* expected,
        const char* name) {
    testHomflyLM(l, false, expected, name);
    testHomflyLM(l, true, expected, name);
}

void homfly() {
    testHomflyAZ(empty, "0", "Empty");
    testHomflyLM(empty, "0", "Empty");

    testHomflyAZ(unknot0, "1", "Unknot (0 crossings)");
    testHomflyLM(unknot0, "1", "Unknot (0 crossings)");
    testHomflyAZ(unknot1, "1", "Unknot (1 crossing)");
    testHomflyLM(unknot1, "1", "Unknot (1 crossing)");
    testHomflyAZ(unknot3, "1", "Unknot (3 crossings)");
    testHomflyLM(unknot3, "1", "Unknot (3 crossings)");
    testHomflyAZ(unknotMonster, "1", "Monster unknot");
    testHomflyLM(unknotMonster, "1", "Monster unknot");
    // The Gordian unknot is surely too large for this.

    testHomflyLM(unlink2_0, "-x y^-1 - x^-1 y^-1",
        "Unlink (2 components)");
    testHomflyLM(unlink3_0, "x^2 y^-2 + 2 y^-2 + x^-2 y^-2",
        "Unlink (3 components)");
    testHomflyLM(unlink2_r2, "-x y^-1 - x^-1 y^-1",
        "Unlink (2 components via R2)");
    testHomflyLM(unlink2_r1r1, "-x y^-1 - x^-1 y^-1",
        "Unlink (2 components via R1+R1)");

    testHomflyLM(trefoilLeft, "-x^4 + x^2 y^2 - 2 x^2",
        "LH Trefoil");
    testHomflyLM(trefoilRight, "x^-2 y^2 - 2 x^-2 - x^-4",
        "RH Trefoil");
    testHomflyAZ(trefoilRight, "x^-2 y^2 + 2 x^-2 - x^-4",
        "RH Trefoil");
    testHomflyLM(trefoil_r1x2, "x^-2 y^2 - 2 x^-2 - x^-4",
        "Trefoil with 2 R1s");
    testHomflyLM(trefoil_r1x6, "x^-2 y^2 - 2 x^-2 - x^-4",
        "Trefoil with 6 R1s");

    testHomflyLM(figureEight, "-x^2 + y^2 - 1 - x^-2", "Figure eight");
    testHomflyLM(figureEight_r1x2, "-x^2 + y^2 - 1 - x^-2",
        "Figure eight with 2 R1s");

    testHomflyLM(hopf, "-x^-1 y + x^-1 y^-1 + x^-3 y^-1", "Hopf link");

    testHomflyLM(trefoil_unknot0,
        "-x^-1 y + 2 x^-1 y^-1 - x^-3 y + 3 x^-3 y^-1 + x^-5 y^-1",
        "Trefoil U unknot (separate)");
    testHomflyLM(trefoil_unknot1,
        "-x^-1 y + 2 x^-1 y^-1 - x^-3 y + 3 x^-3 y^-1 + x^-5 y^-1",
        "Trefoil U unknot (separate + twist)");
    testHomflyLM(trefoil_unknot_overlap,
        "-x^-1 y + 2 x^-1 y^-1 - x^-3 y + 3 x^-3 y^-1 + x^-5 y^-1",
        "Trefoil U unknot (with R2)");

    // This is different from Adams' claim regarding the HOMFLY
    // polynomial of this link.  But... Adams does get his arithmetic
    // wrong elsewhere, and a calculation by hand using the Skein
    // relation agrees with the polynomial below.
    testHomflyLM(adams6_28,
        "x y - x^-1 y^3 + x^-1 y + 2 x^-3 y - x^-3 y^-1 - x^-5 y^-1",
        "Adams Fig. 6.28");

    testHomflyLM(rht_rht,
        "x^-4 y^4 - 4 x^-4 y^2 + 4 x^-4 - 2 x^-6 y^2 + 4 x^-6 + x^-8",
        "RH Trefoil # RH Trefoil");
    testHomflyLM(rht_lht,
        "-x^2 y^2 + 2 x^2 + y^4 - 4 y^2 + 5 - x^-2 y^2 + 2 x^-2",
        "RH Trefoil # LH Trefoil");

    // The following polynomials were computed using Regina,
    // using Kauffman's skein template algorithm (ALG_BACKTRACK).
    // This means that they have not been verified independently;
    // here they are simply being used to ensure that different
    // algorithms give the same results, and also that nothing breaks
    // in a subsequent release.
    testHomflyLM(whitehead,
        "x^3 y - x y^3 + 2 x y - x y^-1 + x^-1 y - x^-1 y^-1",
        "Whitehead link");
    testHomflyLM(borromean,
        "-x^2 y^2 + x^2 y^-2 + y^4 - 2 y^2 + 2 y^-2 - x^-2 y^2 + x^-2 y^-2",
        "Borromean rings");
    testHomflyLM(conway,
        "x^4 y^4 - 3 x^4 y^2 + 2 x^4 - x^2 y^6 + 6 x^2 y^4 - 11 x^2 y^2 + 6 x^2 - y^6 + 6 y^4 - 11 y^2 + 7 + x^-2 y^4 - 3 x^-2 y^2 + 2 x^-2",
        "Conway knot");
    testHomflyLM(kinoshitaTerasaka,
        "x^4 y^4 - 3 x^4 y^2 + 2 x^4 - x^2 y^6 + 6 x^2 y^4 - 11 x^2 y^2 + 6 x^2 - y^6 + 6 y^4 - 11 y^2 + 7 + x^-2 y^4 - 3 x^-2 y^2 + 2 x^-2",
        "Kinoshita-Terasaka knot");

    // On my machine, GST takes around 25 seconds for ALG_TREEWIDTH,
    // and 88 seconds for ALG_NAIVE.  This is probably still a
    // touch too slow to put in the test suite.
    // testHomflyLM(gst,
    //     "-x^4 y^8 + 6 x^4 y^6 - 11 x^4 y^4 + 8 x^4 y^2 - 2 x^4 - x^2 y^12 + 10 x^2 y^10 - 35 x^2 y^8 + 49 x^2 y^6 - 21 x^2 y^4 - 7 x^2 y^2 + 5 x^2 + y^14 - 12 y^12 + 53 y^10 - 102 y^8 + 67 y^6 + 36 y^4 - 63 y^2 + 21 - x^-2 y^16 + 16 x^-2 y^14 - 104 x^-2 y^12 + 355 x^-2 y^10 - 685 x^-2 y^8 + 744 x^-2 y^6 - 422 x^-2 y^4 + 100 x^-2 y^2 - 3 x^-2 + x^-4 y^18 - 18 x^-4 y^16 + 137 x^-4 y^14 - 575 x^-4 y^12 + 1457 x^-4 y^10 - 2296 x^-4 y^8 + 2233 x^-4 y^6 - 1279 x^-4 y^4 + 385 x^-4 y^2 - 45 x^-4 + x^-6 y^18 - 17 x^-6 y^16 + 122 x^-6 y^14 - 484 x^-6 y^12 + 1168 x^-6 y^10 - 1776 x^-6 y^8 + 1698 x^-6 y^6 - 978 x^-6 y^4 + 304 x^-6 y^2 - 38 x^-6 - x^-8 y^16 + 14 x^-8 y^14 - 79 x^-8 y^12 + 233 x^-8 y^10 - 393 x^-8 y^8 + 392 x^-8 y^6 - 228 x^-8 y^4 + 71 x^-8 y^2 - 9 x^-8");

    // TODO: Verify that L # M means * for HOMFLY/Jones

    // TODO: Verify that HOMFLY gives Jones by:
    //   * l = it^-1, m = i(t^-1/2 - t^1/2)
    //   * a = t^-1, z = t^1/2 - t^-1/2
}
#endif

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
    regina::NormalSurfaces vtx(test.link.complement(), regina::NS_STANDARD);
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
            if (comp[0].isSolidTorus())
                EXPECT_TREFOIL_COMPLEMENT(comp[1]);
            else if (comp[1].isSolidTorus())
                EXPECT_TREFOIL_COMPLEMENT(comp[0]);
            else
                ADD_FAILURE() << "Link splits into unexpected components";
            return;
        }
    }

    ADD_FAILURE() << "Link does not split";
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

    EXPECT_EQ(figureEight.link.complement().isoSig(), FIG8_COMPLEMENT_SIG);
    EXPECT_EQ(figureEight_r1x2.link.complement().isoSig(), FIG8_COMPLEMENT_SIG);

    EXPECT_TREFOIL_COMPLEMENT(trefoilLeft.link.complement());
    EXPECT_TREFOIL_COMPLEMENT(trefoilRight.link.complement());
    EXPECT_TREFOIL_COMPLEMENT(trefoil_r1x2.link.complement());
    EXPECT_TREFOIL_COMPLEMENT(trefoil_r1x6.link.complement());

    EXPECT_CENSUS_MANIFOLD(whitehead.link.complement(), "m129");
    EXPECT_CENSUS_MANIFOLD(borromean.link.complement(), "t12067");

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

#if 0
static void verifyR1Down(const Link& l, int crossing,
        const char* briefResult, const std::string& name) {
    Link clone(l, false);

    ASSERT_TRUE(clone.r1(clone.crossing(crossing)));

    EXPECT_CONSISTENCY(clone);
    EXPECT_EQ(clone.brief(), briefResult);
}

static void verifyR1Up(const Link& l, int crossing, int strand,
        int side, int sign, const char* briefResult,
        const std::string& name) {
    Link clone(l, false);

    StrandRef s;
    if (crossing >= 0)
        s = clone.crossing(crossing)->strand(strand);

    ASSERT_TRUE(clone.r1(s, side, sign));

    EXPECT_CONSISTENCY(clone);
    EXPECT_EQ(clone.brief(), briefResult);
}

static void verifyR2Down(const Link& l, int crossing,
        const char* briefResult, const std::string& name) {
    Link clone(l, false);

    ASSERT_TRUE(clone.r2(clone.crossing(crossing)));

    EXPECT_CONSISTENCY(clone);
    EXPECT_EQ(clone.brief(), briefResult);
}

static void verifyR2Down(const Link& l, int crossing, int strand,
        const char* briefResult, const std::string& name) {
    Link clone(l, false);

    ASSERT_TRUE(clone.r2(clone.crossing(crossing)->strand(strand)));

    EXPECT_CONSISTENCY(clone);
    EXPECT_EQ(clone.brief(), briefResult);
}

void verifyR2Up(const Link& l, int upperCrossing, int upperStrand,
        int upperSide, int lowerCrossing, int lowerStrand,
        int lowerSide, const char* briefResult,
        const std::string& name) {
    Link clone(l, false);

    StrandRef upper, lower;
    if (upperCrossing >= 0)
        upper = clone.crossing(upperCrossing)->strand(upperStrand);
    if (lowerCrossing >= 0)
        lower = clone.crossing(lowerCrossing)->strand(lowerStrand);

    ASSERT_TRUE(clone.r2(upper, upperSide, lower, lowerSide));

    EXPECT_CONSISTENCY(clone);
    EXPECT_EQ(clone.brief(), briefResult);
}

static void verifyR3(const Link& l, int crossing, int side,
        const char* briefResult, const std::string& name) {
    Link clone(l, false);

    ASSERT_TRUE(clone.r3(clone.crossing(crossing), side));

    EXPECT_CONSISTENCY(clone);
    EXPECT_EQ(clone.brief(), briefResult);
}

static void verifyR3(const Link& l, int crossing, int strand, int side,
        const char* briefResult, const std::string& name) {
    Link clone(l, false);

    ASSERT_TRUE(clone.r3(clone.crossing(crossing)->strand(strand), side));

    EXPECT_CONSISTENCY(clone);
    EXPECT_EQ(clone.brief(), briefResult);
}

void r123Perform() {
    Link l;
    std::string label;

    l = Link::fromData({ -1 }, { 1, -1 });
    label = "One twist";
    verifyR1Down(l, 0, "( )", label);

    l = Link::fromData({ 1, -1 }, { -1, 1, 2, -2 });
    label = "Two twists (a)";
    verifyR1Down(l, 0, "- ( ^0 _0 )", label);
    verifyR2Down(l, 0, "( )", label);
    verifyR2Down(l, 0, 1, "( )", label);
    verifyR2Down(l, 1, 0, "( )", label);

    l = Link::fromData({ 1, -1 }, { 1, 2, -2, -1 });
    label = "Two twists (b)";
    verifyR1Down(l, 0, "- ( ^0 _0 )", label);
    verifyR2Down(l, 0, "( )", label);
    verifyR2Down(l, 0, 1, "( )", label);
    verifyR2Down(l, 1, 0, "( )", label);

    l = Link::fromData({ 1, -1 }, { 2, -2, -1, 1 });
    label = "Two twists (c)";
    verifyR1Down(l, 0, "- ( ^0 _0 )", label);
    verifyR2Down(l, 0, "( )", label);
    verifyR2Down(l, 0, 1, "( )", label);
    verifyR2Down(l, 1, 0, "( )", label);

    l = Link::fromData({ 1, -1 }, { -2, -1, 1, 2 });
    label = "Two twists (d)";
    verifyR1Down(l, 0, "- ( _0 ^0 )", label);
    verifyR2Down(l, 0, "( )", label);
    verifyR2Down(l, 0, 1, "( )", label);
    verifyR2Down(l, 1, 0, "( )", label);

    l = Link::fromData({ 1, -1 }, { 1, 2 }, { -2, -1 });
    label = "Overlapping loops";
    verifyR2Down(l, 0, "( ) ( )", label);
    verifyR2Down(l, 1, "( ) ( )", label);
    verifyR2Down(l, 0, 0, "( ) ( )", label);
    verifyR2Down(l, 0, 1, "( ) ( )", label);
    verifyR2Down(l, 1, 0, "( ) ( )", label);
    verifyR2Down(l, 1, 1, "( ) ( )", label);

    l = Link::fromData({ -1, 1, -1 }, { -1, 1, 3, 2 }, { -2, -3 });
    label = "Loop overlapping twist (a)";
    verifyR2Down(l, 2, "- ( _0 ^0 ) ( )", label);
    verifyR2Down(l, 2, 1, "- ( _0 ^0 ) ( )", label);
    verifyR2Down(l, 1, 0, "- ( _0 ^0 ) ( )", label);
    verifyR2Down(l, 2, 0, "- ( _0 ^0 ) ( )", label);
    verifyR3(l, 0, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )", label);
    verifyR3(l, 0, 1, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )", label);
    verifyR3(l, 1, 1, 0, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )", label);
    verifyR3(l, 1, 0, 1, "-+- ( _0 ^1 ^2 ^0 ) ( _1 _2 )", label);

    l = Link::fromData({ -1, 1, -1 }, { 3, 2, -1, 1 }, { -2, -3 });
    label = "Loop overlapping twist (b)";
    verifyR2Down(l, 2, "- ( _0 ^0 ) ( )", label);
    verifyR2Down(l, 2, 1, "- ( _0 ^0 ) ( )", label);
    verifyR2Down(l, 1, 0, "- ( _0 ^0 ) ( )", label);
    verifyR2Down(l, 2, 0, "- ( _0 ^0 ) ( )", label);
    verifyR3(l, 0, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )", label);
    verifyR3(l, 0, 1, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )", label);
    verifyR3(l, 1, 1, 0, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )", label);
    verifyR3(l, 1, 0, 1, "-+- ( ^2 ^0 _0 ^1 ) ( _1 _2 )", label);

    l = Link::fromData({ -1, 1, -1 }, { 2, -1, 1, 3 }, { -2, -3 });
    label = "Loop overlapping twist (c)";
    verifyR2Down(l, 2, "- ( _0 ^0 ) ( )", label);
    verifyR2Down(l, 2, 1, "- ( _0 ^0 ) ( )", label);
    verifyR2Down(l, 1, 0, "- ( _0 ^0 ) ( )", label);
    verifyR2Down(l, 2, 0, "- ( _0 ^0 ) ( )", label);
    verifyR3(l, 0, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )", label);
    verifyR3(l, 0, 1, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )", label);
    verifyR3(l, 1, 1, 0, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )", label);
    verifyR3(l, 1, 0, 1, "-+- ( ^1 ^2 ^0 _0 ) ( _1 _2 )", label);

    l = Link::fromData({ 1, -1, 1 }, { 1, -1, -3, -2 }, { 2, 3 });
    label = "Loop overlapping twist (d)";
    verifyR2Down(l, 1, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 2, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 2, 0, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 1, 1, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 2, 1, "+ ( ^0 _0 ) ( )", label);
    verifyR3(l, 1, 1, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )", label);
    verifyR3(l, 0, 0, 0, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )", label);
    verifyR3(l, 1, 0, 0, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )", label);
    verifyR3(l, 1, 1, 1, "+-+ ( ^0 _1 _2 _0 ) ( ^1 ^2 )", label);

    l = Link::fromData({ 1, -1, 1 }, { -3, -2, 1, -1 }, { 2, 3 });
    label = "Loop overlapping twist (e)";
    verifyR2Down(l, 1, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 2, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 2, 0, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 1, 1, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 2, 1, "+ ( ^0 _0 ) ( )", label);
    verifyR3(l, 1, 1, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )", label);
    verifyR3(l, 0, 0, 0, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )", label);
    verifyR3(l, 1, 0, 0, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )", label);
    verifyR3(l, 1, 1, 1, "+-+ ( _2 _0 ^0 _1 ) ( ^1 ^2 )", label);

    l = Link::fromData({ 1, -1, 1 }, { -2, 1, -1, -3 }, { 2, 3 });
    label = "Loop overlapping twist (f)";
    verifyR2Down(l, 1, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 2, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 2, 0, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 1, 1, "+ ( ^0 _0 ) ( )", label);
    verifyR2Down(l, 2, 1, "+ ( ^0 _0 ) ( )", label);
    verifyR3(l, 1, 1, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )", label);
    verifyR3(l, 0, 0, 0, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )", label);
    verifyR3(l, 1, 0, 0, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )", label);
    verifyR3(l, 1, 1, 1, "+-+ ( _1 _2 _0 ^0 ) ( ^1 ^2 )", label);

    l = Link::fromData({ 1, -1, 1, -1, 1, -1 }, { 5, 4 },
        { 6, -6, -5, -3, 1, -1, -2, 2, 3, -4 });
    label = "Three triangles (a)";
    verifyR3(l, 4, 1,
        "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )", label);
    verifyR3(l, 4, 1, 1,
        "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )", label);
    verifyR3(l, 5, 0, 1,
        "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )", label);
    verifyR3(l, 3, 0, 1,
        "+-+-+- ( ^4 ^3 ) ( ^5 _3 _4 _5 _2 ^0 _0 _1 ^1 ^2 )", label);
    verifyR3(l, 4, 0,
        "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )", label);
    verifyR3(l, 4, 1, 0,
        "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )", label);
    verifyR3(l, 4, 0, 1,
        "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )", label);
    verifyR3(l, 2, 1, 1,
        "+-+-+- ( ^4 ^3 ) ( ^5 _5 _2 _4 ^0 _0 _1 ^1 _3 ^2 )", label);
    verifyR3(l, 1, 0,
        "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )", label);
    verifyR3(l, 1, 1, 0,
        "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )", label);
    verifyR3(l, 2, 0, 0,
        "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )", label);
    verifyR3(l, 0, 0, 0,
        "+-+-+- ( ^4 ^3 ) ( ^5 _5 _4 ^0 _2 _1 _0 ^2 ^1 _3 )", label);

    l = Link::fromData({ -1, +1, -1, +1, -1, +1 }, { -5, -4 },
        { -6, 6, 5, 3, -1, 1, 2, -2, -3, 4 });
    label = "Three triangles (b)";
    verifyR3(l, 5, 1,
        "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )", label);
    verifyR3(l, 4, 0, 1,
        "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )", label);
    verifyR3(l, 5, 1, 1,
        "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )", label);
    verifyR3(l, 3, 1, 1,
        "-+-+-+ ( _4 _3 ) ( _5 ^3 ^4 ^5 ^2 _0 ^0 ^1 _1 _2 )", label);
    verifyR3(l, 4, 1,
        "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )", label);
    verifyR3(l, 4, 0, 0,
        "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )", label);
    verifyR3(l, 4, 1, 1,
        "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )", label);
    verifyR3(l, 2, 0, 1,
        "-+-+-+ ( _4 _3 ) ( _5 ^5 ^2 ^4 _0 ^0 ^1 _1 ^3 _2 )", label);
    verifyR3(l, 0, 0,
        "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )", label);
    verifyR3(l, 1, 0, 0,
        "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )", label);
    verifyR3(l, 2, 1, 0,
        "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )", label);
    verifyR3(l, 0, 1, 0,
        "-+-+-+ ( _4 _3 ) ( _5 ^5 ^4 _0 ^2 ^1 ^0 _2 _1 ^3 )", label);

    l = Link::fromData({ 1, 1, -1 }, { 1, -2, -3, -1, 2, 3 });
    label = "Bad trefoil (a)";
    verifyR2Down(l, 2, "+ ( _0 ^0 )", label);
    verifyR2Down(l, 2, 1, "+ ( _0 ^0 )", label);
    verifyR2Down(l, 2, 0, "+ ( _0 ^0 )", label);
    verifyR3(l, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )", label);
    verifyR3(l, 1, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )", label);
    verifyR3(l, 2, 0, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )", label);
    verifyR3(l, 0, 1, 1, "++- ( ^0 _0 _2 ^2 ^1 _1 )", label);

    l = Link::fromData({ 1, 1, -1 }, { -3, -1, 2, 3, 1, -2 });
    label = "Bad trefoil (b)";
    verifyR2Down(l, 2, "+ ( ^0 _0 )", label);
    verifyR2Down(l, 2, 1, "+ ( ^0 _0 )", label);
    verifyR2Down(l, 2, 0, "+ ( ^0 _0 )", label);
    verifyR3(l, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )", label);
    verifyR3(l, 1, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )", label);
    verifyR3(l, 2, 0, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )", label);
    verifyR3(l, 0, 1, 1, "++- ( _2 ^2 ^1 _1 ^0 _0 )", label);

    l = Link::fromData({ -1, -1, 1 }, { 1, -2, -3, -1, 2, 3 });
    label = "Bad trefoil (c)";
    verifyR2Down(l, 2, "- ( _0 ^0 )", label);
    verifyR2Down(l, 2, 1, "- ( _0 ^0 )", label);
    verifyR2Down(l, 2, 0, "- ( _0 ^0 )", label);
    verifyR3(l, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )", label);
    verifyR3(l, 1, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )", label);
    verifyR3(l, 2, 0, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )", label);
    verifyR3(l, 0, 1, 0, "--+ ( ^0 _0 _2 ^2 ^1 _1 )", label);

    l = Link::fromData({ -1, -1, 1 }, { -3, -1, 2, 3, 1, -2 });
    label = "Bad trefoil (d)";
    verifyR2Down(l, 2, "- ( ^0 _0 )", label);
    verifyR2Down(l, 2, 1, "- ( ^0 _0 )", label);
    verifyR2Down(l, 2, 0, "- ( ^0 _0 )", label);
    verifyR3(l, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )", label);
    verifyR3(l, 1, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )", label);
    verifyR3(l, 2, 0, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )", label);
    verifyR3(l, 0, 1, 0, "--+ ( _2 ^2 ^1 _1 ^0 _0 )", label);

    l = Link::fromData({ -1, 1, 1, -1}, { 1, 2, -4, -3, -2, -1, 3, 4 });
    label = "Bad figure eight (a)";
    verifyR2Down(l, 0, "+- ( _1 _0 ^0 ^1 )", label);
    verifyR2Down(l, 0, 1, "+- ( _1 _0 ^0 ^1 )", label);
    verifyR2Down(l, 1, 0, "+- ( _1 _0 ^0 ^1 )", label);
    verifyR2Down(l, 2, "-+ ( ^0 ^1 _1 _0 )", label);
    verifyR2Down(l, 2, 1, "-+ ( ^0 ^1 _1 _0 )", label);
    verifyR2Down(l, 3, 0, "-+ ( ^0 ^1 _1 _0 )", label);
    verifyR3(l, 0, 1, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )", label);
    verifyR3(l, 0, 1, 1, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )", label);
    verifyR3(l, 0, 0, 0, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )", label);
    verifyR3(l, 2, 0, 0, "-++- ( ^0 _3 _1 _2 ^2 _0 ^3 ^1 )", label);

    l = Link::fromData({ -1, 1, 1, -1}, { 2, -4, -3, -2, -1, 3, 4, 1 });
    label = "Bad figure eight (b)";
    verifyR2Down(l, 0, "+- ( _1 _0 ^0 ^1 )", label);
    verifyR2Down(l, 0, 1, "+- ( _1 _0 ^0 ^1 )", label);
    verifyR2Down(l, 1, 0, "+- ( _1 _0 ^0 ^1 )", label);

    l = Link::fromData({ -1, 1, 1, -1}, { -2, -1, 3, 4, 1, 2, -4, -3 });
    label = "Bad figure eight (c)";
    verifyR2Down(l, 0, "+- ( ^0 ^1 _1 _0 )", label);
    verifyR2Down(l, 0, 1, "+- ( ^0 ^1 _1 _0 )", label);
    verifyR2Down(l, 1, 0, "+- ( ^0 ^1 _1 _0 )", label);

    l = Link::fromData({ -1, 1, 1, -1}, { -1, 3, 4, 1, 2, -4, -3, -2 });
    label = "Bad figure eight (d)";
    verifyR2Down(l, 0, "+- ( ^0 ^1 _1 _0 )", label);
    verifyR2Down(l, 0, 1, "+- ( ^0 ^1 _1 _0 )", label);
    verifyR2Down(l, 1, 0, "+- ( ^0 ^1 _1 _0 )", label);

    l = Link::fromData({ 1, -1, -1, 1}, { -1, -2, 4, 3, 2, 1, -3, -4 });
    label = "Bad figure eight (e)";
    verifyR2Down(l, 1, "-+ ( ^1 ^0 _0 _1 )", label);
    verifyR2Down(l, 0, 0, "-+ ( ^1 ^0 _0 _1 )", label);
    verifyR2Down(l, 1, 1, "-+ ( ^1 ^0 _0 _1 )", label);
    verifyR2Down(l, 3, "+- ( _0 _1 ^1 ^0 )", label);
    verifyR2Down(l, 2, 0, "+- ( _0 _1 ^1 ^0 )", label);
    verifyR2Down(l, 3, 1, "+- ( _0 _1 ^1 ^0 )", label);
    verifyR3(l, 2, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )", label);
    verifyR3(l, 0, 0, 1, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )", label);
    verifyR3(l, 0, 1, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )", label);
    verifyR3(l, 2, 1, 0, "+--+ ( _0 ^3 ^1 ^2 _2 ^0 _3 _1 )", label);

    l = Link::fromData({ 1, -1, -1, 1}, { -2, 4, 3, 2, 1, -3, -4, -1 });
    label = "Bad figure eight (f)";
    verifyR2Down(l, 1, "-+ ( ^1 ^0 _0 _1 )", label);
    verifyR2Down(l, 0, 0, "-+ ( ^1 ^0 _0 _1 )", label);
    verifyR2Down(l, 1, 1, "-+ ( ^1 ^0 _0 _1 )", label);

    l = Link::fromData({ 1, -1, -1, 1}, { 2, 1, -3, -4, -1, -2, 4, 3 });
    label = "Bad figure eight (g)";
    verifyR2Down(l, 1, "-+ ( _0 _1 ^1 ^0 )", label);
    verifyR2Down(l, 0, 0, "-+ ( _0 _1 ^1 ^0 )", label);
    verifyR2Down(l, 1, 1, "-+ ( _0 _1 ^1 ^0 )", label);

    l = Link::fromData({ 1, -1, -1, 1}, { 1, -3, -4, -1, -2, 4, 3, 2 });
    label = "Bad figure eight (h)";
    verifyR2Down(l, 1, "-+ ( _0 _1 ^1 ^0 )", label);
    verifyR2Down(l, 0, 0, "-+ ( _0 _1 ^1 ^0 )", label);
    verifyR2Down(l, 1, 1, "-+ ( _0 _1 ^1 ^0 )", label);

    l = Link::fromData({ -1, 1, -1, -1, -1 },
        { -2, -3, 4, -5, 1, 2, 3, -4, 5, -1 });
    label = "Excessive trefoil (a)";
    verifyR2Down(l, 0, "--- ( _0 ^1 _2 ^0 _1 ^2 )", label);
    verifyR2Down(l, 0, 0, "--- ( _0 ^1 _2 ^0 _1 ^2 )", label);
    verifyR2Down(l, 0, 1, "--- ( _0 ^1 _2 ^0 _1 ^2 )", label);
    verifyR2Down(l, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 )", label);
    verifyR2Down(l, 1, 0, "--- ( ^1 _2 ^0 _1 ^2 _0 )", label);
    verifyR2Down(l, 1, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 )", label);

    l = Link::fromData({ 1, -1, 1, 1, 1 },
        { 2, 3, -4, 5, -1, -2, -3, 4, -5, 1 });
    label = "Excessive trefoil (b)";
    verifyR2Down(l, 0, "+++ ( ^0 _1 ^2 _0 ^1 _2 )", label);
    verifyR2Down(l, 0, 0, "+++ ( ^0 _1 ^2 _0 ^1 _2 )", label);
    verifyR2Down(l, 0, 1, "+++ ( ^0 _1 ^2 _0 ^1 _2 )", label);
    verifyR2Down(l, 1, "+++ ( _1 ^2 _0 ^1 _2 ^0 )", label);
    verifyR2Down(l, 1, 0, "+++ ( _1 ^2 _0 ^1 _2 ^0 )", label);
    verifyR2Down(l, 1, 1, "+++ ( _1 ^2 _0 ^1 _2 ^0 )", label);

    l = Link::fromData({ 1, -1, 1, -1, -1, 1, -1, -1 }, { -3, -4 },
        { 1, -1, -2, 7, 6, 5, 4, 3, -5, -6, -7, 8, -8, 2 });
    label = "Dangling twists (a)";
    verifyR1Down(l, 0,
        "-+--+-- ( _1 _2 ) ( _0 ^5 ^4 ^3 ^2 ^1 _3 _4 _5 ^6 _6 ^0 )",
        label);
    verifyR1Down(l, 7,
        "+-+--+- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^4 ^3 ^2 _4 _5 _6 ^1 )",
        label);
    verifyR2Down(l, 1,
        "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )", label);
    verifyR2Down(l, 1, 1,
        "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )", label);
    verifyR2Down(l, 0, 0,
        "+--+-- ( _0 _1 ) ( ^4 ^3 ^2 ^1 ^0 _2 _3 _4 ^5 _5 )", label);
    verifyR2Down(l, 6,
        "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
    verifyR2Down(l, 6, 1,
        "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
    verifyR2Down(l, 5, 0,
        "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
    verifyR2Down(l, 5,
        "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
    verifyR2Down(l, 5, 1,
        "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
    verifyR2Down(l, 4, 0,
        "+-+--- ( _2 _3 ) ( ^0 _0 _1 ^4 ^3 ^2 _4 ^5 _5 ^1 )", label);
    verifyR2Down(l, 3,
        "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )", label);
    verifyR2Down(l, 3, 1,
        "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )", label);
    verifyR2Down(l, 2, 0,
        "+--+-- ( ) ( ^0 _0 _1 ^4 ^3 ^2 _2 _3 _4 ^5 _5 ^1 )", label);
    verifyR3(l, 4, 0,
        "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )",
        label);
    verifyR3(l, 4, 1, 0,
        "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )",
        label);
    verifyR3(l, 2, 0, 1,
        "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )",
        label);
    verifyR3(l, 2, 1, 0,
        "+-+--+-- ( _2 _3 ) ( ^0 _0 _1 ^6 ^5 ^3 ^4 _4 ^2 _5 _6 ^7 _7 ^1 )",
        label);

    l = Link::fromData({ 1, -1, 1, -1, -1, 1, -1, -1 }, { -3, -4 },
        { 6, 5, 4, 3, -5, -6, -7, 8, -8, 2, 1, -1, -2, 7 });
    label = "Dangling twists (b)";
    verifyR2Down(l, 6,
        "+-+--- ( _2 _3 ) ( ^4 ^3 ^2 _4 ^5 _5 ^1 ^0 _0 _1 )", label);
    verifyR2Down(l, 5,
        "+-+--- ( _2 _3 ) ( ^3 ^2 _4 ^5 _5 ^1 ^0 _0 _1 ^4 )", label);

    l = Link::fromData({ -1, 1, -1, 1, 1, -1, 1, 1 }, { 3, 4 },
        { -1, 1, 2, -7, -6, -5, -4, -3, 5, 6, 7, -8, 8, -2 });
    label = "Dangling twists (c)";
    verifyR1Down(l, 0,
        "+-++-++ ( ^1 ^2 ) ( ^0 _5 _4 _3 _2 _1 ^3 ^4 ^5 _6 ^6 _0 )",
        label);
    verifyR1Down(l, 7,
        "-+-++-+ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _4 _3 _2 ^4 ^5 ^6 _1 )",
        label);
    verifyR2Down(l, 0,
        "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )", label);
    verifyR2Down(l, 0, 1,
        "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )", label);
    verifyR2Down(l, 1, 0,
        "-++-++ ( ^0 ^1 ) ( _4 _3 _2 _1 _0 ^2 ^3 ^4 _5 ^5 )", label);
    verifyR2Down(l, 5,
        "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
    verifyR2Down(l, 5, 1,
        "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
    verifyR2Down(l, 6, 0,
        "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
    verifyR2Down(l, 4,
        "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
    verifyR2Down(l, 4, 1,
        "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
    verifyR2Down(l, 5, 0,
        "-+-+++ ( ^2 ^3 ) ( _0 ^0 ^1 _4 _3 _2 ^4 _5 ^5 _1 )", label);
    verifyR2Down(l, 2,
        "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )", label);
    verifyR2Down(l, 2, 1,
        "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )", label);
    verifyR2Down(l, 3, 0,
        "-++-++ ( ) ( _0 ^0 ^1 _4 _3 _2 ^2 ^3 ^4 _5 ^5 _1 )", label);
    verifyR3(l, 2, 1,
        "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )",
        label);
    verifyR3(l, 2, 1, 1,
        "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )",
        label);
    verifyR3(l, 2, 0, 0,
        "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )",
        label);
    verifyR3(l, 4, 0, 0,
        "-+-++-++ ( ^2 ^3 ) ( _0 ^0 ^1 _6 _5 _3 _4 ^4 _2 ^5 ^6 _7 ^7 _1 )",
        label);

    l = Link::fromData({ -1, 1, -1, 1, 1, -1, 1, 1 }, { 3, 4 },
        { -6, -5, -4, -3, 5, 6, 7, -8, 8, -2, -1, 1, 2, -7 });
    label = "Dangling twists (d)";
    verifyR2Down(l, 5,
        "-+-+++ ( ^2 ^3 ) ( _4 _3 _2 ^4 _5 ^5 _1 _0 ^0 ^1 )", label);
    verifyR2Down(l, 4,
        "-+-+++ ( ^2 ^3 ) ( _3 _2 ^4 _5 ^5 _1 _0 ^0 ^1 _4 )", label);

    l = Link::fromData({ 1, 1, -1, -1, -1 },
        { 0 }, { -1, 2, -4, 3, -2, 1, -3, 4, 5, -5 }, { 0 }, { 0 });
    label = "Figure eight with twist and three unknots";
    verifyR1Up(l, -1, 0, 0, -1,
        "++---- ( ^5 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, -1, 0, 0, 1,
        "++---+ ( _5 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, -1, 0, 1, -1,
        "++---- ( _5 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, -1, 0, 1, 1,
        "++---+ ( ^5 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, 4, 1, 0, -1,
        "++---- ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 _5 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, 4, 1, 0, 1,
        "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 ^5 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, 4, 1, 1, -1,
        "++---- ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 ^5 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, 4, 1, 1, 1,
        "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 _5 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, 1, 0, 0, -1,
        "++---- ( ) ( _0 ^1 _3 ^2 _1 ^5 _5 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, 1, 0, 0, 1,
        "++---+ ( ) ( _0 ^1 _3 ^2 _1 _5 ^5 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, 1, 0, 1, -1,
        "++---- ( ) ( _0 ^1 _3 ^2 _1 _5 ^5 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
        label);
    verifyR1Up(l, 1, 0, 1, 1,
        "++---+ ( ) ( _0 ^1 _3 ^2 _1 ^5 _5 ^0 _2 ^3 ^4 _4 ) ( ) ( )",
        label);
    // Note: for R2, the implementation always adds the two new
    // crossings in the order (+, -).
    verifyR2Up(l, -1, 0, 0, -1, 0, 0, "++---+- "
        "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _6 _5 ) ( )",
        label);
    verifyR2Up(l, -1, 0, 0, -1, 0, 1, "++---+- "
        "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _6 _5 ) ( )",
        label);
    verifyR2Up(l, -1, 0, 1, -1, 0, 0, "++---+- "
        "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _5 _6 ) ( )",
        label);
    verifyR2Up(l, -1, 0, 1, -1, 0, 1, "++---+- "
        "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ) ( _5 _6 ) ( )",
        label);
    verifyR2Up(l, -1, 0, 0, 4, 0, 0, "++---+- "
        "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 _6 _5 ) ( ) ( )",
        label);
    verifyR2Up(l, -1, 0, 1, 4, 0, 0, "++---+- "
        "( ^5 ^6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 _5 _6 ) ( ) ( )",
        label);
    verifyR2Up(l, 4, 0, 0, -1, 0, 0, "++---+- "
        "( _6 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ^5 ^6 ) ( ) ( )",
        label);
    verifyR2Up(l, 4, 0, 0, -1, 0, 1, "++---+- "
        "( _6 _5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _4 ^6 ^5 ) ( ) ( )",
        label);
    verifyR2Up(l, -1, 0, 0, 4, 1, 1, "++---+- "
        "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _6 _5 _4 ) ( ) ( )",
        label);
    verifyR2Up(l, -1, 0, 1, 4, 1, 1, "++---+- "
        "( ^6 ^5 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 _5 _6 _4 ) ( ) ( )",
        label);
    verifyR2Up(l, 4, 1, 1, -1, 0, 0, "++---+- "
        "( _5 _6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^5 ^6 _4 ) ( ) ( )",
        label);
    verifyR2Up(l, 4, 1, 1, -1, 0, 1, "++---+- "
        "( _5 _6 ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^4 ^6 ^5 _4 ) ( ) ( )",
        label);
    verifyR2Up(l, 4, 0, 0, 3, 1, 0, "++---+- "
        "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 _6 _5 ^4 _4 ^5 ^6 ) ( ) ( )",
        label);
    verifyR2Up(l, 3, 1, 0, 4, 0, 0, "++---+- "
        "( ) ( _0 ^1 _3 ^2 _1 ^0 _2 ^3 ^5 ^6 ^4 _4 _6 _5 ) ( ) ( )",
        label);
    verifyR2Up(l, 4, 1, 1, 1, 0, 1, "++---+- "
        "( ) ( _0 ^1 _3 ^2 _1 _5 _6 ^0 _2 ^3 ^4 ^6 ^5 _4 ) ( ) ( )",
        label);
    verifyR2Up(l, 1, 0, 1, 4, 1, 1, "++---+- "
        "( ) ( _0 ^1 _3 ^2 _1 ^6 ^5 ^0 _2 ^3 ^4 _5 _6 _4 ) ( ) ( )",
        label);
    verifyR2Up(l, 2, 0, 0, 1, 1, 1, "++---+- "
        "( ) ( _0 ^1 _6 _5 _3 ^2 _1 ^0 _2 ^6 ^5 ^3 ^4 _4 ) ( ) ( )",
        label);
    verifyR2Up(l, 1, 1, 1, 2, 0, 0, "++---+- "
        "( ) ( _0 ^1 ^5 ^6 _3 ^2 _1 ^0 _2 _5 _6 ^3 ^4 _4 ) ( ) ( )",
        label);
}

static void verifyResolve(const Link& l, int crossing,
        const char* briefResult, const char* name) {
    Link clone(l, false);

    clone.resolve(clone.crossing(crossing));

    EXPECT_CONSISTENCY(clone);
    EXPECT_EQ(clone.brief(), briefResult);
}

void resolve() {
    Link l;

    l = Link::fromData({ +1 }, { 1, -1 });
    verifyResolve(l, 0, "( ) ( )", "One twist (a)");

    l = Link::fromData({ +1 }, { -1, 1 });
    verifyResolve(l, 0, "( ) ( )", "One twist (b)");

    l = Link::fromData({ -1 }, { 1, -1 });
    verifyResolve(l, 0, "( ) ( )", "One twist (c)");

    l = Link::fromData({ -1 }, { -1, 1 });
    verifyResolve(l, 0, "( ) ( )", "One twist (d)");

    l = Link::fromData({ -1, +1, -1, -1 },
        { 4, -1, 2, -2, 3, -4, 1, -3 });
    verifyResolve(l, 1, "--- ( ^2 _0 ^1 _2 ^0 _1 ) ( )",
        "Trefoil with + twist (a)");

    l = Link::fromData({ -1, +1, -1, -1 },
        { 2, -2, 3, -4, 1, -3, 4, -1 });
    verifyResolve(l, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 ) ( )",
        "Trefoil with + twist (b)");

    l = Link::fromData({ -1, +1, -1, -1 },
        { -2, 3, -4, 1, -3, 4, -1, 2 });
    verifyResolve(l, 1, "--- ( ^1 _2 ^0 _1 ^2 _0 ) ( )",
        "Trefoil with + twist (c)");

    l = Link::fromData({ +1, -1, +1, +1 },
        { 4, -1, -2, 2, 3, -4, 1, -3 });
    verifyResolve(l, 1, "+++ ( ^2 _0 ^1 _2 ^0 _1 ) ( )",
        "Trefoil with - twist (a)");

    l = Link::fromData({ +1, -1, +1, +1 },
        { -2, 2, 3, -4, 1, -3, 4, -1 });
    verifyResolve(l, 1, "+++ ( ^1 _2 ^0 _1 ^2 _0 ) ( )",
        "Trefoil with - twist (b)");

    l = Link::fromData({ +1, -1, +1, +1 },
        { 2, 3, -4, 1, -3, 4, -1, -2 });
    verifyResolve(l, 1, "+++ ( ^1 _2 ^0 _1 ^2 _0 ) ( )",
        "Trefoil with - twist (c)");

    l = Link::fromData({ +1, +1, -1, -1 },
        { 3, -1, 2, -3, 4, -2, 1, -4 });
    verifyResolve(l, 2, "++- ( _0 ^1 ) ( ^2 _1 ^0 _2 )",
        "Figure eight (a)");

    l = Link::fromData({ +1, +1, -1, -1 },
        { -3, 4, -2, 1, -4, 3, -1, 2 });
    verifyResolve(l, 2, "++- ( ^2 _1 ^0 _2 ) ( _0 ^1 )",
        "Figure eight (b)");

    l = Link::fromData({ +1, +1, -1, -1 },
        { 2, -3, 4, -2, 1, -4, 3, -1 });
    verifyResolve(l, 2, "++- ( ^1 _0 ) ( ^2 _1 ^0 _2 )",
        "Figure eight (c)");

    l = Link::fromData({ +1, +1, -1, -1 },
        { 4, -2, 1, -4, 3, -1, 2, -3 });
    verifyResolve(l, 2, "++- ( ^2 _1 ^0 _2 ) ( _0 ^1 )",
        "Figure eight (d)");

    l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
        { 2, -5, 6, -2, 1, 3, -4, -6, 5, -1 }, { -3, 4 });
    verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )",
        "Figure eight with link (a)");

    l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
        { 2, -5, 6, -2, 1, 3, -4, -6, 5, -1 }, { 4, -3 });
    verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )",
        "Figure eight with link (b)");

    l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
        { 3, -4, -6, 5, -1, 2, -5, 6, -2, 1 }, { -3, 4 });
    verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )",
        "Figure eight with link (c)");

    l = Link::fromData({ +1, +1, +1, +1, -1, -1 },
        { 3, -4, -6, 5, -1, 2, -5, 6, -2, 1 }, { 4, -3 });
    verifyResolve(l, 2, "+++-- ( ^2 _2 _4 ^3 _0 ^1 _3 ^4 _1 ^0 )",
        "Figure eight with link (d)");
}
#endif

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
    }
    if (reflect && reverse) {
        Link alt(link, false);
        alt.reflect();
        alt.reverse();
        EXPECT_EQ(alt.knotSig(reflect, reverse), sig);
    }

    Link recon;
    ASSERT_NO_THROW({ recon = Link::fromKnotSig(sig); });

    EXPECT_EQ(recon.size(), link.size());
    EXPECT_EQ(recon.countComponents(), link.countComponents());
    EXPECT_EQ(recon.knotSig(reflect, reverse), sig);
    if (link.size() <= JONES_THRESHOLD && ! reflect)
        EXPECT_EQ(recon.jones(), link.jones());

    // Verify the "magic" string constructor.
    EXPECT_NO_THROW({ EXPECT_EQ(Link(sig), recon); });
}

static void verifyKnotSig(const Link& link, const char* name) {
    if (link.countComponents() != 1)
        return;

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

    // A link where all four reflection/reversal options give different sigs:
    Link asymmetric = Link::fromOrientedGauss(
        "-<6 +>3 -<5 +>2 -<4 -<1 +>1 +>5 -<3 +>6 -<2 +>4");
    EXPECT_EQ(asymmetric.knotSig(true, true),   "gaabcdefbcfedPQ--");
    EXPECT_EQ(asymmetric.knotSig(true, false),  "gaabcdefdcbefPQ--");
    EXPECT_EQ(asymmetric.knotSig(false, true),  "gaabcdefbcfedPQaa");
    EXPECT_EQ(asymmetric.knotSig(false, false), "gaabcdefdcbefPQaa");
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
    // (except for the unknot0 cases, which were computed by hand).

    verifyRewrite(unknot0, 0, 1);
    verifyRewrite(unknot0, 1, 2);
    verifyRewrite(unknot0, 2, 6);
    verifyRewrite(unknot0, 3, 22);
    verifyRewrite(unknot3, 0, 22);
    verifyRewrite(unknot3, 1, 131);
    verifyRewrite(unknot3, 2, 998);
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