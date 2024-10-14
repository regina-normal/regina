
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

#include "angle/anglestructures.h"
#include "surface/normalsurface.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

#include "testhelper.h"
#include "testexhaustive.h"

using regina::AngleStructure;
using regina::AngleStructures;
using regina::Example;
using regina::Tetrahedron;
using regina::Triangulation;

static void verifyCount(const AngleStructures& list, size_t expectedCount,
        bool strict, bool taut) {
    size_t tot = 0;
    for (const AngleStructure& s : list)
        if (s.isStrict() == strict && s.isTaut() == taut)
            ++tot;
    EXPECT_EQ(tot, expectedCount);
}

TEST(AngleStructuresTest, empty) {
    Triangulation<3> tri;

    AngleStructures list(tri);
    EXPECT_EQ(list.size(), 1);
    EXPECT_TRUE(list.spansStrict());
    EXPECT_TRUE(list.spansTaut());
}

TEST(AngleStructuresTest, singleTetrahedron) {
    Triangulation<3> tri = Example<3>::ball();

    AngleStructures list(tri);
    EXPECT_EQ(list.size(), 3);
    EXPECT_TRUE(list.spansStrict());
    EXPECT_TRUE(list.spansTaut());

    verifyCount(list, 3, false /* strict */, true /* taut */);
}

TEST(AngleStructuresTest, gieseking) {
    Triangulation<3> tri = Example<3>::gieseking();

    AngleStructures list(tri);
    EXPECT_EQ(list.size(), 3);
    EXPECT_TRUE(list.spansStrict());
    EXPECT_TRUE(list.spansTaut());

    verifyCount(list, 3, false /* strict */, true /* taut */);
}

TEST(AngleStructuresTest, figureEight) {
    Triangulation<3> tri = Example<3>::figureEight();

    AngleStructures list(tri);
    EXPECT_EQ(list.size(), 5);
    EXPECT_TRUE(list.spansStrict());
    EXPECT_TRUE(list.spansTaut());

    verifyCount(list, 3, false /* strict */, true /* taut */);
    verifyCount(list, 2, false /* strict */, false /* taut */);
}

TEST(AngleStructuresTest, loopC2) {
    Triangulation<3> tri = Example<3>::layeredLoop(2, false);

    AngleStructures list(tri);
    EXPECT_EQ(list.size(), 0);
    EXPECT_FALSE(list.spansStrict());
    EXPECT_FALSE(list.spansTaut());
}

static void verifyTaut(const char* isoSig, size_t nTaut) {
    SCOPED_TRACE_CSTRING(isoSig);
    Triangulation<3> tri = Triangulation<3>::fromIsoSig(isoSig);

    AngleStructures a(tri, true);
    EXPECT_EQ(a.size(), nTaut);

    for (const AngleStructure& s : a) {
        for (size_t j = 0; j < tri.size(); ++j) {
            regina::Rational tot = 0;
            for (int k = 0; k < 3; ++k) {
                regina::Rational tmp = s.angle(j, k);
                if (tmp != 0 && tmp != 1)
                    ADD_FAILURE() << "Bad angle: " << tmp;
                tot += tmp;
            }
            EXPECT_EQ(tot, 1);
        }

        for (auto e : tri.edges()) {
            if (e->isBoundary())
                continue;

            regina::Rational tot = 0;
            for (size_t k = 0; k < e->degree(); ++k) {
                tot += s.angle(
                    e->embedding(k).tetrahedron()->index(),
                    regina::quadSeparating[
                        e->embedding(k).vertices()[0]][
                        e->embedding(k).vertices()[1]]);
            }
            EXPECT_EQ(tot, 2);
        }
    }
}

TEST(AngleStructuresTest, taut) {
    // The following taut angle structure counts were computed
    // using Regina 4.95, via the old double description method.

    // The trefoil:
    verifyTaut("cPcbbbadu", 1);

    // Some small hyperbolic knots:
    verifyTaut("cPcbbbiht", 3);
    verifyTaut("dLQbcccdero", 4);
    verifyTaut("gLLMQccefeffdfeqldg", 15);
    verifyTaut("fLLQcbcdeeemgopdp", 7);
    verifyTaut("eLPkbcddddcwjb", 4);

    // Some larger hyperbolic knots:
    verifyTaut("qLvALPzLMQMkbfefhhijmonmnoppppapmggfmgxjgjpeeo", 80);
    verifyTaut("qLLLLAzzPPQkcefehikilmnpmnoppiitdsvivjvvukvunb", 189);
    verifyTaut("qLLLLPzPwPQkcefehjkjljopppnooiitdvqiepdtidxfmi", 127);
    verifyTaut("qLLzPvMzPAQkccdghhjjmlonnnppphgggcbagbvdatdasb", 206);
    verifyTaut("rLLLMzwwPAMQccdfegihimlnompqqpqiceakanuandeuoamom", 145);
    verifyTaut("rLvLvvQPAPQQccfimimliokqlqpqoppdejdvdaglcelsgsfgj", 97);
    verifyTaut("vLLvLvMzAzMAQQQcehlnkilrqppootssututuiiiaiicimgggooabfaalll", 440);
    verifyTaut("uLLvLLvMALQMQQcceihoijpsqnorqorsttrtiimriwfiiksokjovllxoj", 440);
    verifyTaut("sLLvLLvLQAQQQceihimlqqmronpoprpriimrwlmmilifskbvlga", 396);
    verifyTaut("qLvLLvPQLQQkbefhnlnkmolkpmoppmtmohhhuaautvbbkb", 62);

    // Examples from Jonathan:
    verifyTaut("hLvQAkcdcfeeggqjjqhnqj", 0);
    verifyTaut("oLLLAAwzPQcbedgfhfilknmnnmxxnxhxjxxkxqaxw", 0);
    verifyTaut("vvLLAvQvMwwQMQQcdheglkjpopsnstqsrutuuwrawwxhwxhclrmhaqwrrrr", 0);
    verifyTaut("CLwvvwQAzAPwQLwLQzQkaciklmhjmonrqptspvuwxyzzyBAABjghqqhxxgaaaahaajaahaharwqqkn", 0);
    verifyTaut("CLvzMzwQwwvzzQPQPPAkccfigkmiljkovqxustywyxzxAyABBqjqwfqqoaajvujaljladbrxwxwvxw", 4);

    verifyTaut("JLwvvvwwLzwLQAQQPwvQLQQQcackhnsqpxtoyuwtzyrvBywGAzHIEDFHIGFGIngqxgabgaabqhqxhqxrarrabkbgcnnnkrrww", 0);
    verifyTaut("JLwvvvwwLzwLMQQQPMzMPMQPcackhnsqpxovutwAzyrvByxuAEzCGDEHFGIHIjgqxgabgabqhxqxhqxrarrjwajwrwrqrrxjj", 0);
    verifyTaut("JLwvvwMwzMLPzAAvMQLwQAQQcacikmhmqounrtpsyxvDwBCEFEFAHCGHGHFIIjghqhxgababhqxrabwabhahajoslfhahawgj", 0);
    verifyTaut("JLvzMzwQwwvPAPwQwAPvPAMQcccfigkmiljkoutuwvutxtyzACBDEGFGFIHHIqjqwfqqoaajvqhlaallalaaaaaaqqajshhrr", 4);
    verifyTaut("JLLLAAwzLLAwQwvvwMAQAAQMcbedgfhfilnnnpoqrstvCxEBDzFAFEGEFHHIIxxnxhxjxxxaxgvcxxafenatpkatbwqrrqfqr", 0);
}

static void verifyTautVsAllDetail(const Triangulation<3>& t) {
    AngleStructures all(t, false);
    AngleStructures taut(t, true);

    EXPECT_FALSE(all.isTautOnly());
    EXPECT_TRUE(taut.isTautOnly());

    size_t found = 0;
    for (const AngleStructure& a : all)
        if (a.isTaut())
            ++found;

    for (const AngleStructure& a : taut)
        EXPECT_TRUE(a.isTaut());

    EXPECT_EQ(found, taut.size());
}

static void verifyTautVsAll(const char* dehydration) {
    SCOPED_TRACE_CSTRING(dehydration);

    Triangulation<3> tri = Triangulation<3>::rehydrate(dehydration);
    ASSERT_FALSE(tri.isEmpty());

    verifyTautVsAllDetail(tri);
}

static void verifyTautVsAll(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    verifyTautVsAllDetail(tri);
}

TEST(AngleStructuresTest, tautVsAll) {
    verifyTautVsAll("baaaade"); // m000
    verifyTautVsAll("dadbcccaqrb"); // m010
    verifyTautVsAll("hbnajbcdeefgghvfeevho"); // v1000
    verifyTautVsAll("hepacdefegfggcurmsktu"); // y500

    verifyTautVsAll(Triangulation<3>(), "Empty triangulation");
    verifyTautVsAll(Example<3>::ball(), "One-tetrahedron ball");
}

static void verifyTreeVsDD(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    bool hasStrict = tri.hasStrictAngleStructure();

    AngleStructures all(tri, false);
    AngleStructures tautTree(tri, true);
    AngleStructures tautDD(tri, true, regina::AngleAlg::DD);

    EXPECT_FALSE(all.isTautOnly());
    EXPECT_TRUE(tautTree.isTautOnly());
    EXPECT_TRUE(tautDD.isTautOnly());
    EXPECT_EQ(tautTree.algorithm(), regina::AngleAlg::Tree);
    EXPECT_EQ(tautDD.algorithm(), regina::AngleAlg::DD);

    EXPECT_EQ(all.spansTaut(), tautTree.spansTaut());
    EXPECT_EQ(all.spansTaut(), tautDD.spansTaut());
    EXPECT_EQ(all.spansStrict(), hasStrict);

    // Verify that tautTree == tautDD, modulo reordering.
    EXPECT_EQ(tautTree.size(), tautDD.size());
    EXPECT_EQ(tautTree, tautDD);

    // Verify that tautTree == { taut members of all }, modulo reordering.
    EXPECT_LE(tautTree.size(), all.size());
    tautTree.sort(std::less<AngleStructure>());
    all.sort([](const AngleStructure& a, const AngleStructure& b) {
        // Like the usual a < b, but always put taut angle structures first.
        if (a.isTaut() && ! b.isTaut())
            return true;
        else if (b.isTaut() && ! a.isTaut())
            return false;
        else
            return a < b;
    });
    auto tautIt = tautTree.begin();
    auto allIt = all.begin();
    for ( ; tautIt != tautTree.end(); ++tautIt, ++allIt) {
        ASSERT_NE(allIt, all.end());
        EXPECT_TRUE(allIt->isTaut());
        EXPECT_EQ(*tautIt, *allIt);
    }
    if (allIt != all.end())
        EXPECT_FALSE(allIt->isTaut());
}

TEST(AngleStructuresTest, tautStrictTreeVsDD) {
    runCensusAllIdeal(verifyTreeVsDD);
    runCensusAllClosed(verifyTreeVsDD); // Should be no solns.
    runCensusAllBounded(verifyTreeVsDD); // May have partial solns.
}

static void verifyGeneralAngleStructure(const Triangulation<3>& tri,
        const char* name) {
    SCOPED_TRACE_CSTRING(name);

    bool exists = tri.hasGeneralAngleStructure();

    if (tri.isValid() && ! tri.hasBoundaryTriangles()) {
        // A generalised angle structure exists iff every vertex
        // link is a torus or Klein bottle.
        for (const auto v : tri.vertices())
            if (v->linkEulerChar() != 0) {
                // There should be no generalised angle structure.
                // There is nothing more to test beyond this.
                EXPECT_FALSE(exists);
                return;
            }

        // We *should* have a generalised angle structure.
        EXPECT_TRUE(exists);
    }

    if (exists) {
        regina::MatrixInt m = regina::makeAngleEquations(tri);
        const regina::VectorInt& vec = tri.generalAngleStructure().vector();
        ASSERT_EQ(vec.size(), m.columns());
        EXPECT_TRUE((m * vec).isZero());
    }
}

TEST(AngleStructuresTest, generalAngleStructure) {
    runCensusAllIdeal(verifyGeneralAngleStructure);
    runCensusAllClosed(verifyGeneralAngleStructure);
    runCensusAllBounded(verifyGeneralAngleStructure);
}

TEST(AngleStructuresTest, copyMove) {
    // The main point of this test is to ensure that the move
    // operations are *actually* move operations and not copies.
    //
    // We assume here that std::vector's move operations
    // preserve the addresses of the underlying objects.
    // I don't think this is required by the standard, but I'm
    // also not aware of any implementation that doesn't do this,
    // and I can't think of a better (and still non-intrusive)
    // way to ensure that the move was a "real" move.

    const AngleStructures a(Example<3>::whitehead());
    ASSERT_NE(a.size(), 0);

    AngleStructures a1(a);
    EXPECT_EQ(a1.detail(), a.detail());
    EXPECT_NE(a.size(), 0);

    AngleStructures a2(std::move(a1));
    EXPECT_EQ(a2.detail(), a.detail());
    // The std::vector move constructor promises to empty the source.
    EXPECT_EQ(a1.size(), 0);

    AngleStructures a3(Example<3>::trefoil());
    a3 = a;
    EXPECT_EQ(a3.detail(), a.detail());
    EXPECT_NE(a.size(), 0);

    AngleStructures a4(Example<3>::trefoil());
    a4 = std::move(a3);
    EXPECT_EQ(a4.detail(), a.detail());
    /*
    // I'm not sure how to verify that the source was moved from
    // and not copied, in a way that does not make assumptions
    // about the standard library implementation...
    EXPECT_EQ(a3.size(), 0);
    */
}
