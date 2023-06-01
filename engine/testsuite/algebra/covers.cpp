
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

#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "link/examplelink.h"
#include "link/link.h"
#include "snappea/snappeatriangulation.h"
#include "triangulation/example3.h"

#include "gtest/gtest.h"

using regina::AbelianGroup;
using regina::GroupPresentation;
using regina::Link;
using regina::SnapPeaTriangulation;
using regina::Triangulation;

class CoversTest : public testing::Test {
    protected:
        template <int degree>
        std::vector<std::string> viaSnapPea(const Triangulation<3>& tri) {
            std::vector<std::string> covers;

            SnapPeaTriangulation s(tri);
            if (s.isNull()) {
                covers.emplace_back("Null_SnapPea");
                return covers;
            }

            s.enumerateCovers(degree, SnapPeaTriangulation::all_covers,
                    [&](const SnapPeaTriangulation& cover,
                    SnapPeaTriangulation::CoverType type) {
                covers.push_back(cover.homologyFilled().str());
            });
            std::sort(covers.begin(), covers.end());
            return covers;
        }

        template <int degree>
        std::vector<std::string> viaRegina(const Triangulation<3>& tri) {
            std::vector<std::string> covers;
            tri.group().enumerateCovers<degree>([&](
                    GroupPresentation&& g) {
                AbelianGroup ab = g.abelianisation();
                covers.push_back(ab.str());

                // Since we are already computing abelianisations, and since
                // their ranks can differ between covers of the same index,
                // this is a good place to verify abelianRank().
                EXPECT_EQ(ab.rank(), g.abelianRank());
            });
            std::sort(covers.begin(), covers.end());
            return covers;
        }

        template <int maxDegree>
        void compareResults(const Triangulation<3>& tri, const char* name) {
            SCOPED_TRACE(name);

            EXPECT_EQ(viaSnapPea<maxDegree>(tri), viaRegina<maxDegree>(tri));

            if constexpr (maxDegree > 2)
                compareResults<maxDegree - 1>(tri, name);
        }

        template <int degree>
        void verifyResults(const Triangulation<3>& tri,
                const std::vector<std::string>& expected, const char* name) {
            SCOPED_TRACE(name);

            EXPECT_EQ(viaRegina<degree>(tri), expected);
        }

        template <int maxDegree>
        void compareResults(const Link& link, const char* name) {
            compareResults<maxDegree>(link.complement(), name);
        }

        template <int maxDegree>
        void verifyResults(const Link& link,
                const std::vector<std::string>& expected, const char* name) {
            verifyResults<maxDegree>(link.complement(), expected, name);
        }
};

TEST_F(CoversTest, trivial) {
    // No covers:
    compareResults<11>(regina::Example<3>::sphere(), "Sphere");
}

TEST_F(CoversTest, manifolds) {
    // No covers for degrees 2..4 or 7..9:
    compareResults<9>(regina::Example<3>::poincare(),
        "Poincare homology sphere");

    // Cover (which is trivial) only for degree 3:
    compareResults<10>(regina::Example<3>::lens(3, 1), "L(3,1)");

    // Several covers for degree 5 and a few for degrees ≥ 7:
    compareResults<9>(regina::Example<3>::weeks(), "Weeks");

    // Many, many covers for degree 5 (and a bit too slow to put
    // degree 6 in the test suite: takes half a second on my machine):
    Triangulation<3> ws = regina::Example<3>::weberSeifert();
    verifyResults<2>(ws, { }, "Weber-Seifert");
    verifyResults<3>(ws, { }, "Weber-Seifert");
    verifyResults<4>(ws, { }, "Weber-Seifert");
    verifyResults<5>(ws, {
        "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25",
        "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25",
        "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25",
        "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25",
        "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25",
        "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25",
        "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25",
        "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25", "2 Z_5 + 2 Z_25",
        "2 Z_5 + 2 Z_25",
        "4 Z + 2 Z_3",
        "6 Z_5 + Z_25", "6 Z_5 + Z_25",
        "6 Z_5 + Z_25", "6 Z_5 + Z_25",
        "6 Z_5 + Z_25", "6 Z_5 + Z_25",
        "Z_5 + 2 Z_25 + Z_75", "Z_5 + 2 Z_25 + Z_75",
        "Z_5 + 2 Z_25 + Z_75", "Z_5 + 2 Z_25 + Z_75",
        "Z_5 + 2 Z_25 + Z_75", "Z_5 + 2 Z_25 + Z_75" },
        "Weber-Seifert");
}

TEST_F(CoversTest, knots) {
    compareResults<9>(regina::ExampleLink::trefoilRight(), "Trefoil");

    // Each of the following invariants have been verified with SnapPea
    // and/or GAP.  However, SnapPea is slow to compute them for large
    // indices, so in those cases we have done the verification offline
    // and just hard-coded the expected results here.
    //
    // We do not include index 7 tests for link19 or link20, since
    // these definitely slow down the test suite more than we'd like.

    Link conway = regina::ExampleLink::conway();
    compareResults<5>(conway, "Conway knot");
    verifyResults<6>(conway,
        { "2 Z", "2 Z + Z_12", "2 Z + Z_2 + Z_4", "2 Z + Z_3",
          "2 Z + Z_3", "2 Z + Z_3", "2 Z + Z_3", "2 Z + Z_3",
          "2 Z + Z_30", "2 Z + Z_6", "2 Z + Z_6", "3 Z", "3 Z + Z_2",
          "3 Z + Z_2", "Z", "Z + Z_108" },
        "Conway knot");
    verifyResults<7>(conway,
        { "2 Z", "2 Z", "2 Z + Z_2", "3 Z", "3 Z", "3 Z", "3 Z", "3 Z",
          "3 Z", "3 Z", "3 Z", "3 Z + 2 Z_2", "3 Z + 2 Z_2",
          "3 Z + 2 Z_2", "3 Z + 2 Z_2", "3 Z + 2 Z_2", "3 Z + Z_2",
          "3 Z + Z_2", "3 Z + Z_2", "3 Z + Z_2", "3 Z + Z_6", "4 Z",
          "Z", "Z + 2 Z_2 + Z_4 + Z_12", "Z + 2 Z_2 + Z_4 + Z_12",
          "Z + 2 Z_2 + Z_8 + Z_40", "Z + 2 Z_2 + Z_8 + Z_40",
          "Z + Z_139", "Z + Z_1838", "Z + Z_2782" },
        "Conway knot");

    Link link19 = regina::Link::fromKnotSig(
        "tabcadefghdijklmnoipkjplmefqrghbcsonqrsvvvvvvb-VzgZBa");
    compareResults<3>(link19, "19-crossing knot");
    verifyResults<4>(link19,
        { "2 Z + Z_2" , "Z + Z_9 + Z_39411" },
        "19-crossing knot");
    verifyResults<5>(link19,
        { "2 Z", "3 Z", "Z + 2 Z_6691" },
        "19-crossing knot");
    verifyResults<6>(link19,
        { "2 Z + Z_157 + Z_628", "2 Z + Z_2 + Z_12", "2 Z + Z_4379",
          "2 Z + Z_8758", "Z + 2 Z_2 + Z_314 + Z_1375006",
          "Z + 2 Z_4 + Z_1819388", "Z + Z_628 + Z_324048" },
        "19-crossing knot");

    Link link20 = regina::Link::fromKnotSig(
        "uabcdbefgecdhifgjklmnhijopqlkqrsaoprtnmtsRktvvvfFyWJTFl");
    compareResults<3>(link20, "20-crossing knot");
    verifyResults<4>(link20,
        { "2 Z + Z_140", "Z + Z_25 + Z_91675" },
        "20-crossing knot");
    verifyResults<5>(link20,
        { "2 Z", "2 Z + Z_2", "3 Z + 2 Z_2", "3 Z + 2 Z_2",
          "3 Z + Z_2 + Z_6", "3 Z + Z_2 + Z_6", "3 Z + Z_3",
          "4 Z + Z_4", "Z + 2 Z_15061", "Z + Z_6 + Z_8638440" },
        "20-crossing knot");
    verifyResults<6>(link20,
        { "2 Z + 2 Z_12", "2 Z + 2 Z_3", "2 Z + 2 Z_3", "2 Z + 2 Z_3",
          "2 Z + 3 Z_2 + Z_36", "2 Z + 3 Z_2 + Z_36",
          "2 Z + Z_2 + Z_114", "2 Z + Z_2 + Z_12", "2 Z + Z_2 + Z_12",
          "2 Z + Z_2 + Z_132", "2 Z + Z_2 + Z_248 + Z_8680",
          "2 Z + Z_2 + Z_36", "2 Z + Z_2 + Z_4 + Z_8",
          "2 Z + Z_2 + Z_4 + Z_8", "2 Z + Z_3", "2 Z + Z_3 + Z_36",
          "2 Z + Z_4", "2 Z + Z_456", "2 Z + Z_6 + Z_48",
          "2 Z + Z_6 + Z_48", "3 Z", "4 Z", "4 Z + Z_2", "4 Z + Z_2",
          "4 Z + Z_2", "4 Z + Z_2", "4 Z + Z_2", "5 Z", "5 Z",
          "Z + 2 Z_2 + Z_248 + Z_40176",
          "Z + 2 Z_2 + Z_496 + Z_1818832", "Z + Z_56161980" },
        "20-crossing knot");
}

template <int index>
static void verifyFreeAbelian(int rank) {
    // Every finite index subgroup of a free abelian group of
    // rank n is also a free abelian group of rank n.
    //
    // For rank 1, there is exactly one subgroup up to conjugacy.
    // For ranks 2 and above, we can get the expected number of
    // subgroups up to conjugacy from the OEIS.
    //
    // For index 1, 2, ..., 10:
    //     rank 2 (A000203): 1, 3, 4, 7, 6, 12, 8, 15, 13, 18
    //     rank 3 (A001001): 1, 7, 13, 35, 31, 91, 57, 155, 130, 217
    //     rank 4 (A038991): 1, 15, 40, 155, 156,
    //                       600, 400, 1395, 1210, 2340
    //     rank 5 (A038992): 1, 31, 121, 651, 781,
    //                       3751, 2801, 11811, 11011, 24211
    //     rank 6 (A038993): 1, 63, 364, 2667, 3906,
    //                       22932, 19608, 97155, 99463, 246078
    //     rank 7 (A038994): 1, 127, 1093, 10795, 19531,
    //                       138811, 137257, 788035, 896260, 2480437
    //     rank 8 (A038995): 1, 255, 3280, 43435, 97656,
    //                       836400, 960800, 6347715, 8069620, 24902280
    //     rank 9 (A038996): 1, 511, 9841, 174251, 488281, 5028751,
    //                       6725601, 50955971, 72636421, 249511591
    //
    // We just hard-code the expected results in an array,
    // which is indexed as expected[rank - 1][index - 2].
    static constexpr size_t maxIndex = 10;
    static constexpr size_t maxRank = 6; // enough for our tests
    static constexpr size_t expected[maxRank][maxIndex - 1] = {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 3, 4, 7, 6, 12, 8, 15, 13, 18 },
        { 7, 13, 35, 31, 91, 57, 155, 130, 217 },
        { 15, 40, 155, 156, 600, 400, 1395, 1210, 2340 },
        { 31, 121, 651, 781, 3751, 2801, 11811, 11011, 24211 },
        { 63, 364, 2667, 3906, 22932, 19608, 97155, 99463, 246078 }
    };

    SCOPED_TRACE("index = " + std::to_string(index));
    SCOPED_TRACE("rank = " + std::to_string(rank));

    // Prerequisites for us to actually use these tests:
    ASSERT_GE(index, 2);
    ASSERT_LE(index, maxIndex);
    ASSERT_GE(rank, 1);
    ASSERT_LE(rank, maxRank);

    // Build the group presentation.
    GroupPresentation freeAbelian(rank);
    for (int i = 0; i < rank; ++i)
        for (int j = i + 1; j < rank; ++j) {
            regina::GroupExpression reln;
            reln.addTermLast(i, 1);
            reln.addTermLast(j, 1);
            reln.addTermLast(i, -1);
            reln.addTermLast(j, -1);
            freeAbelian.addRelation(std::move(reln));
        }

    // Now: go compute.
    size_t expectedCount = expected[rank - 1][index - 2];

    size_t nFound = 0;
    size_t ans = freeAbelian.enumerateCovers<index>([&](
            GroupPresentation&& g) {
        g.intelligentSimplify();

        // Of course the group itself should be free abelian, but we
        // call abelianisation() since that is guaranteed to show
        // the correct rank, whereas the presentation on its own
        // could be too messy for Regina to recognise.
        EXPECT_TRUE(g.abelianisation().isFree(rank));

        ++nFound;
    });

    EXPECT_EQ(ans, nFound);
    EXPECT_EQ(ans, expectedCount);
}

TEST_F(CoversTest, freeAbelian) {
    // The upper bounds on the ranks below were chosen according to
    // what would finish quickly enough to be part of the test suite.
    for (int rank = 1; rank <= 6; ++rank)
        verifyFreeAbelian<2>(rank);
    for (int rank = 1; rank <= 6; ++rank)
        verifyFreeAbelian<3>(rank);
    for (int rank = 1; rank <= 5; ++rank)
        verifyFreeAbelian<4>(rank);
    for (int rank = 1; rank <= 5; ++rank)
        verifyFreeAbelian<5>(rank);
    for (int rank = 1; rank <= 4; ++rank)
        verifyFreeAbelian<6>(rank);
    for (int rank = 1; rank <= 4; ++rank)
        verifyFreeAbelian<7>(rank);
    for (int rank = 1; rank <= 3; ++rank)
        verifyFreeAbelian<8>(rank);
    for (int rank = 1; rank <= 2; ++rank)
        verifyFreeAbelian<9>(rank);
    for (int rank = 1; rank <= 2; ++rank)
        verifyFreeAbelian<10>(rank);
}

template <int index>
static void verifyFree(int rank) {
    // The Nielsen–Schreier formula says that every subgroup of g is
    // free of rank 1 + index * (rank - 1).

    // The number of conjugacy classes (at least for all our tests)
    // can be found in the OEIS (sequence A057004):
    //
    // For ranks 1, 2, 3, ...:
    //     index 2: 1, 3, 7, 15, 31, 63, 127, 255, 511
    //     index 3: 1, 7, 41, 235, 1361, 7987, 47321, 281995
    //     index 4: 1, 26, 604, 14120, 334576, 7987616, 191318464
    //     index 5: 1, 97, 13753, 1712845, 207009649
    //     index 6: 1, 624, 504243, 371515454
    //     index 7: 1, 4163, 24824785
    //     index 8: 1, 34470

    // The formulae in terms of the rank r:
    //     index 2:           2^r - 1
    //     index 3 (A057009): 6^(r-1) + 3^(r-1) - 2^(r-1)
    //     index 4 (A057010): 24^(r-1) + 8^(r-1) - 6^(r-1)
    //     index 5 (A057011): 120^(r-1) - 24^(r-1) - 12^(r-1) +
    //                        6^(r-1) + 5^(r-1) + 4^(r-1) - 2^(r-1)
    //     index 6 (A057012): OEIS has PARI code online

    // For now we just hard-code the expected results in an array,
    // which is indexed as expected[index - 2][rank - 1].
    // An array value of 0 means the result is beyond the end of
    // our hard-coded list of results for that particular index.
    static constexpr size_t maxIndex = 8;
    static constexpr size_t maxRank = 9;
    static constexpr size_t expected[maxIndex - 1][maxRank] = {
        { 1, 3, 7, 15, 31, 63, 127, 255, 511 },
        { 1, 7, 41, 235, 1361, 7987, 47321, 281995, 0 },
        { 1, 26, 604, 14120, 334576, 7987616, 191318464, 0, 0 },
        { 1, 97, 13753, 1712845, 207009649, 0, 0, 0, 0 },
        { 1, 624, 504243, 371515454, 0, 0, 0, 0, 0 },
        { 1, 4163, 24824785, 0, 0, 0, 0, 0, 0 },
        { 1, 34470, 0, 0, 0, 0, 0, 0, 0 }
    };

    SCOPED_TRACE("index = " + std::to_string(index));
    SCOPED_TRACE("rank = " + std::to_string(rank));

    // Prerequisites for us to actually use these tests:
    ASSERT_GE(index, 2);
    ASSERT_LE(index, maxIndex);
    ASSERT_LE(rank, maxRank);
    if (rank > 0)
        ASSERT_NE(expected[index - 2][rank - 1], 0);

    // Now: go compute.

    size_t expectedRank = (rank > 0 ? (1 + index * (rank - 1)) : 0);
    size_t expectedCount = (rank > 0 ? expected[index - 2][rank - 1] :
        0);

    size_t nFound = 0;
    size_t ans = GroupPresentation(rank).enumerateCovers<index>([&](
            GroupPresentation&& g) {
        g.intelligentSimplify();

        EXPECT_EQ(g.countGenerators(), expectedRank);
        EXPECT_EQ(g.countRelations(), 0);

        ++nFound;
    });

    EXPECT_EQ(ans, nFound);
    EXPECT_EQ(ans, expectedCount);
}

TEST_F(CoversTest, free) {
    // The upper bounds on the ranks below were chosen according to
    // what would finish quickly enough to be part of the test suite.
    for (int rank = 0; rank <= 9; ++rank)
        verifyFree<2>(rank);
    for (int rank = 0; rank <= 6; ++rank)
        verifyFree<3>(rank);
    for (int rank = 0; rank <= 4; ++rank)
        verifyFree<4>(rank);
    for (int rank = 0; rank <= 3; ++rank)
        verifyFree<5>(rank);
    for (int rank = 0; rank <= 2; ++rank)
        verifyFree<6>(rank);
    for (int rank = 0; rank <= 2; ++rank)
        verifyFree<7>(rank);
    for (int rank = 0; rank <= 1; ++rank)
        verifyFree<8>(rank);
}

template <int index>
static void verifyCyclic(long order) {
    // If index divides order then we should have exactly one
    // result, which must be Z_{order/index}.
    // Otherwise we should have no results at all.

    SCOPED_TRACE("index = " + std::to_string(index));
    SCOPED_TRACE("order = " + std::to_string(order));

    long expectedOrder = (order % index == 0 ? order / index : 0);
    size_t nFound = 0;

    GroupPresentation src(1);
    src.addRelation(regina::GroupExpression(0, order));
    size_t ans = src.enumerateCovers<index>([&](GroupPresentation&& g) {
        g.intelligentSimplify();

        if (expectedOrder == 1) {
            EXPECT_EQ(g.countGenerators(), 0);
        } else {
            EXPECT_EQ(g.countGenerators(), 1);
            EXPECT_EQ(g.countRelations(), 1);
            EXPECT_EQ(std::abs(g.relation(0).terms().front().exponent),
                expectedOrder);
        }

        ++nFound;
    });

    if (order % index == 0) {
        EXPECT_EQ(ans, nFound);
        EXPECT_EQ(ans, 1);
    } else {
        EXPECT_EQ(ans, 0);
    }
}

TEST_F(CoversTest, cyclic) {
    for (int order = 1; order <= 15; ++order)
        verifyCyclic<2>(order);
    for (int order = 1; order <= 15; ++order)
        verifyCyclic<3>(order);
    for (int order = 1; order <= 15; ++order)
        verifyCyclic<4>(order);
    for (int order = 1; order <= 15; ++order)
        verifyCyclic<5>(order);
    for (int order = 1; order <= 15; ++order)
        verifyCyclic<6>(order);
    for (int order = 1; order <= 15; ++order)
        verifyCyclic<7>(order);
    for (int order = 1; order <= 15; ++order)
        verifyCyclic<8>(order);
    for (int order = 1; order <= 15; ++order)
        verifyCyclic<9>(order);
    for (int order = 1; order <= 15; ++order)
        verifyCyclic<10>(order);
    for (int order = 1; order <= 15; ++order)
        verifyCyclic<11>(order);
}

