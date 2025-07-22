
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "triangulation/dim3.h"
#include "triangulation/dim3/homologicaldata.h"
#include "triangulation/example3.h"

#include "testhelper.h"

using regina::Example;
using regina::HomologicalData;
using regina::Triangulation;

struct TestCase {
    HomologicalData data;
    const char* name;
};

/**
 * Thanks to Ryan Budney for supplying the questions and answers for
 * these tests!
 */
class HomologicalDataTest : public testing::Test {
    protected:
        // Closed orientable triangulations:
        TestCase s3 { Example<3>::threeSphere(), "S^3" };
        TestCase s2xs1 { Example<3>::s2xs1(), "S^2 x S^1" };
        TestCase lens3_1 { Example<3>::lens(3, 1), "L(3,1)" };
        TestCase lens4_1 { Example<3>::lens(4, 1), "L(4,1)" };
        TestCase lens7_1 { Example<3>::lens(7, 1), "L(7,1)" };
        TestCase d88xz15 { Example<3>::augTriSolidTorus(2, -1, 2, -1, 11, -30),
            "S^3 / D_88 x Z_15" };
        TestCase poincare { Example<3>::poincare(), "Poincare" };
        TestCase weberSeifert { Example<3>::weberSeifert(), "Weber-Seifert" };
        TestCase torusBundleA { Example<3>::augTriSolidTorus(2, 1, 4, -7, 4, 1),
            "T x I / [ 0,1 | -1,0 ]" };
        TestCase torusBundleB {
            Example<3>::augTriSolidTorus(3, -1, 3, -1, 3, -1),
            "T x I / [ -1,1 | -1,0 ]" };
        TestCase twistedKBxS1 {
            Triangulation<3>::rehydrate("gepaacdfefefknakanx"),
            "KB/n2 x~ S^1" };
        TestCase closedHypA {
            // From the Hodgson-Weeks closed orientable census:
            Triangulation<3>::rehydrate("jgpadaaeffghfiihirmxitdagbj"),
            "Closed Hyp (vol=1.01494161)" };
        TestCase closedHypB {
            // From the Hodgson-Weeks closed orientable census:
            Triangulation<3>::rehydrate("mnnjjcabehfgjijkijllaaqabhoehrtab"),
            "Closed Hyp (vol=2.45402944)" };
        TestCase closedHypC {
            // From Regina's closed orientable census:
            Triangulation<3>::rehydrate("jlncbaabdghfggiiiafxhbccqei"),
            "Closed Hyp (vol=1.26370924)" };

        // Closed non-orientable triangulations:
        TestCase norA {
            Triangulation<3>::rehydrate("jofbdaabccfhgihiiffhofoxohx"),
            "SFS [M_/n2: (2,1)]" };
        TestCase norB {
            Triangulation<3>::rehydrate("gepaacdfefefaaknnkx"),
            "SFS [RP2: (2,1) (2,1)]" };
        TestCase norTorusBundle {
            Triangulation<3>::rehydrate("hipabcedfggfggbgforro"),
            "T x I / [ 2,1 | 1,0 ]" };

        // Ideal triangulations, including several SnapPea census manifolds:
        TestCase gieseking { Example<3>::gieseking(), "Gieseking manifold" };
        TestCase figureEight { Example<3>::figureEight(),
            "Figure eight knot complement" };
        TestCase m003 { Triangulation<3>::rehydrate("cabbbbmlq"),
            "SnapPea m003" };
        TestCase m041 { Triangulation<3>::rehydrate("eahcdbddbokpv"),
            "SnapPea m041" };
        TestCase m045 { Triangulation<3>::rehydrate("eahccdcddbfof"),
            "SnapPea m045" };
        TestCase s028 { Triangulation<3>::rehydrate("gkfacaccdeffffohhhf"),
            "SnapPea s028" };
        TestCase s887 { Triangulation<3>::rehydrate("gknaaacefffennqbnjf"),
            "SnapPea s887" };
        TestCase s955 { Triangulation<3>::rehydrate("gbpaabcfdffefohfxhf"),
            "SnapPea s955" };
        TestCase genusTwoBdry {
            Triangulation<3>::rehydrate("eanadccdnxfno"),
            "Ideal with genus 2 cusp" };

        // Triangulations with real boundary:
        TestCase lst3_4_7 { Example<3>::lst(3, 4), "LST(3,4,7)" };
};

static void verifyHomologyConsistency(TestCase& test) {
    SCOPED_TRACE_CSTRING(test.name);

    for (int i = 0; i <= 3; ++i)
        EXPECT_EQ(test.data.homology(i).unmarked(),
            test.data.dualHomology(i).unmarked());

    EXPECT_EQ(test.data.triangulation().homology(),
        test.data.dualHomology(1).unmarked());
}

TEST_F(HomologicalDataTest, consistency) {
    verifyHomologyConsistency(s3);
    verifyHomologyConsistency(s2xs1);
    verifyHomologyConsistency(lens3_1);
    verifyHomologyConsistency(lens4_1);
    verifyHomologyConsistency(lens7_1);
    verifyHomologyConsistency(d88xz15);
    verifyHomologyConsistency(poincare);
    verifyHomologyConsistency(weberSeifert);
    verifyHomologyConsistency(torusBundleA);
    verifyHomologyConsistency(torusBundleB);
    verifyHomologyConsistency(twistedKBxS1);
    verifyHomologyConsistency(closedHypA);
    verifyHomologyConsistency(closedHypB);
    verifyHomologyConsistency(closedHypC);

    verifyHomologyConsistency(norA);
    verifyHomologyConsistency(norB);
    verifyHomologyConsistency(norTorusBundle);

    verifyHomologyConsistency(gieseking);
    verifyHomologyConsistency(figureEight);
    verifyHomologyConsistency(m003);
    verifyHomologyConsistency(m041);
    verifyHomologyConsistency(m045);
    verifyHomologyConsistency(s028);
    verifyHomologyConsistency(s887);
    verifyHomologyConsistency(s955);
    verifyHomologyConsistency(genusTwoBdry);

    verifyHomologyConsistency(lst3_4_7);
}

static void verifyStandardCells(TestCase& test,
        const std::array<size_t, 4>& expect) {
    SCOPED_TRACE_CSTRING(test.name);

    for (int i = 0; i < 4; ++i)
        EXPECT_EQ(test.data.countStandardCells(i), expect[i]);
}

TEST_F(HomologicalDataTest, standardCells) {
    verifyStandardCells(lens7_1, { 1, 5, 8, 4 });
    verifyStandardCells(d88xz15, { 1, 10, 18, 9 });
    verifyStandardCells(weberSeifert, { 1, 24, 46, 23 });
    verifyStandardCells(closedHypA, { 1, 10, 18, 9 });
    verifyStandardCells(closedHypB, { 1, 13, 24, 12 });
    verifyStandardCells(norA, { 1, 10, 18, 9 });
    verifyStandardCells(s028, { 12, 42, 36, 6 });
    verifyStandardCells(s955, { 12, 42, 36, 6 });
    verifyStandardCells(lst3_4_7, { 1, 5, 7, 3 });
}

static void verifyDualCells(TestCase& test,
        const std::array<size_t, 4>& expect) {
    SCOPED_TRACE_CSTRING(test.name);

    for (int i = 0; i < 4; ++i)
        EXPECT_EQ(test.data.countDualCells(i), expect[i]);
}

TEST_F(HomologicalDataTest, dualCells) {
    verifyDualCells(lens7_1, { 4, 8, 5, 1 });
    verifyDualCells(d88xz15, { 9, 18, 10, 1 });
    verifyDualCells(weberSeifert, { 23, 46, 24, 1 });
    verifyDualCells(closedHypA, { 9, 18, 10, 1 });
    verifyDualCells(closedHypB, { 12, 24, 13, 1 });
    verifyDualCells(norA, { 9, 18, 10, 1 });
    verifyDualCells(s028, { 6, 12, 6, 0 });
    verifyDualCells(s955, { 6, 12, 6, 0 });
    verifyDualCells(lst3_4_7, { 3, 5, 2, 0 });
}

TEST_F(HomologicalDataTest, boundaryMapH1) {
    EXPECT_EQ(lens7_1.data.bdryHomologyMap(1).summary(), "Zero map");
    EXPECT_EQ(d88xz15.data.bdryHomologyMap(1).summary(), "Zero map");
    EXPECT_EQ(weberSeifert.data.bdryHomologyMap(1).summary(), "Zero map");
    EXPECT_EQ(closedHypA.data.bdryHomologyMap(1).summary(), "Zero map");
    EXPECT_EQ(closedHypB.data.bdryHomologyMap(1).summary(), "Zero map");
    EXPECT_EQ(norA.data.bdryHomologyMap(1).summary(), "Zero map");

    EXPECT_EQ(s028.data.bdryHomologyMap(1).summary(),
        "Map (kernel Z | cokernel Z_16 | image Z)");
    EXPECT_EQ(s955.data.bdryHomologyMap(1).summary(),
        "Map (kernel Z | cokernel Z_20 | image Z + Z_2)");
    EXPECT_EQ(lst3_4_7.data.bdryHomologyMap(1).summary(),
        "Epic (kernel Z)");
}

TEST_F(HomologicalDataTest, torsionRankVector) {
    EXPECT_EQ(lens7_1.data.torsionRankVectorString(), "7(1)");
    EXPECT_EQ(d88xz15.data.torsionRankVectorString(), "2(0 0 1) 3(1) 5(1)");
    EXPECT_EQ(weberSeifert.data.torsionRankVectorString(), "5(3)");
    EXPECT_EQ(closedHypA.data.torsionRankVectorString(), "2(1) 3(2)");
    EXPECT_EQ(closedHypB.data.torsionRankVectorString(), "7(2)");
    EXPECT_EQ(norA.data.torsionRankVectorString(), "2(1 1)");
    EXPECT_EQ(s028.data.torsionRankVectorString(), "2(0 0 0 1)");
    EXPECT_EQ(s955.data.torsionRankVectorString(), "2(0 1) 5(1)");
    EXPECT_EQ(lst3_4_7.data.torsionRankVectorString(), "no torsion");
}

TEST_F(HomologicalDataTest, torsionSigmaVector) {
    EXPECT_EQ(lens7_1.data.torsionSigmaVectorString(), "no 2-torsion");
    EXPECT_EQ(d88xz15.data.torsionSigmaVectorString(), "7 7 inf");
    EXPECT_EQ(weberSeifert.data.torsionSigmaVectorString(), "no 2-torsion");
    EXPECT_EQ(closedHypA.data.torsionSigmaVectorString(), "inf");
    EXPECT_EQ(closedHypB.data.torsionSigmaVectorString(), "no 2-torsion");
    EXPECT_EQ(norA.data.torsionSigmaVectorString(),
        "manifold is non-orientable");
    EXPECT_EQ(s028.data.torsionSigmaVectorString(), "7 7 7 inf");
    EXPECT_EQ(s955.data.torsionSigmaVectorString(), "inf 0");
    EXPECT_EQ(lst3_4_7.data.torsionSigmaVectorString(), "no 2-torsion");
}

TEST_F(HomologicalDataTest, torsionLegendreSymbolVector) {
    EXPECT_EQ(lens7_1.data.torsionLegendreSymbolVectorString(), "7(1)");
    EXPECT_EQ(d88xz15.data.torsionLegendreSymbolVectorString(), "3(1) 5(1)");
    EXPECT_EQ(weberSeifert.data.torsionLegendreSymbolVectorString(), "5(1)");
    EXPECT_EQ(closedHypA.data.torsionLegendreSymbolVectorString(), "3(1)");
    EXPECT_EQ(closedHypB.data.torsionLegendreSymbolVectorString(), "7(-1)");
    EXPECT_EQ(norA.data.torsionLegendreSymbolVectorString(),
        "manifold is non-orientable");
    EXPECT_EQ(s028.data.torsionLegendreSymbolVectorString(),
        "no odd p-torsion");
    EXPECT_EQ(s955.data.torsionLegendreSymbolVectorString(), "5(-1)");
    EXPECT_EQ(lst3_4_7.data.torsionLegendreSymbolVectorString(),
        "no odd p-torsion");
}

TEST_F(HomologicalDataTest, embeddabilityComment) {
    EXPECT_EQ(s2xs1.data.embeddabilityComment(), "No information.");
    EXPECT_EQ(poincare.data.embeddabilityComment(),
        "Manifold is a homology 3-sphere.");
    EXPECT_EQ(weberSeifert.data.embeddabilityComment(),
        "Does not embed in homology 4-sphere.  "
        "Manifold is a rational homology sphere.");
    EXPECT_EQ(lens3_1.data.embeddabilityComment(),
        "Does not embed in homology 4-sphere.  "
        "Manifold is a rational homology sphere.");
    EXPECT_EQ(lens4_1.data.embeddabilityComment(),
        "This manifold, once-punctured, does not embed in a "
        "homology 4-sphere.  Manifold is a rational homology sphere.");
    EXPECT_EQ(closedHypC.data.embeddabilityComment(),
        "The torsion linking form is of hyperbolic type.  "
        "Manifold is a rational homology sphere.");
    EXPECT_EQ(torusBundleA.data.embeddabilityComment(),
        "This manifold, once-punctured, does not embed in a "
        "homology 4-sphere.");
    EXPECT_EQ(torusBundleB.data.embeddabilityComment(),
        "Does not embed in homology 4-sphere.");
    EXPECT_EQ(twistedKBxS1.data.embeddabilityComment(),
        "The torsion linking form is of hyperbolic type.");
    EXPECT_EQ(norB.data.embeddabilityComment(),
        "Orientation cover has hyperbolic torsion linking form.");
    EXPECT_EQ(norTorusBundle.data.embeddabilityComment(),
        "Does not embed in homology 4-sphere.");
    EXPECT_EQ(gieseking.data.embeddabilityComment(),
        "Orientation cover satisfies KK 2-torsion condition.");
    EXPECT_EQ(figureEight.data.embeddabilityComment(),
        "Embeds in a homology 3-sphere as a knot complement.");
    EXPECT_EQ(m003.data.embeddabilityComment(),
        "Embeds in rational homology 3-sphere.  KK 2-tor condition satisfied.");
    EXPECT_EQ(m041.data.embeddabilityComment(),
        "Does not embed in homology 4-sphere.");
    EXPECT_EQ(m045.data.embeddabilityComment(),
        "Embeds in rational homology 3-sphere but not homology 4-sphere.");
    EXPECT_EQ(s887.data.embeddabilityComment(),
        "Embeds in a homology 3-sphere as a link complement.");
    EXPECT_EQ(genusTwoBdry.data.embeddabilityComment(),
        "Embeds in a homology 3-sphere as a graph complement.");

    // The comment for S^3 should depend upon whether or not we have run
    // three-sphere recognition.
    {
        Triangulation<3> tri = Example<3>::threeSphere();
        EXPECT_EQ(HomologicalData(tri).embeddabilityComment(),
            "Manifold is a homology 3-sphere.");
        tri.isSphere();
        EXPECT_EQ(HomologicalData(tri).embeddabilityComment(),
            "This manifold is S^3.");
    }
}
