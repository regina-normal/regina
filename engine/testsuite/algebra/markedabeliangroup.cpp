
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

#include "algebra/markedabeliangroup.h"
#include "maths/matrix.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

#include "testhelper.h"

using regina::Integer;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Triangulation;
using regina::VectorInt;

class MarkedAbelianGroupTest : public testing::Test {
    protected:
        static void verifyNonCycle(const MarkedAbelianGroup& g,
                const VectorInt& v) {
            SCOPED_TRACE_REGINA(v);

            EXPECT_FALSE(g.isCycle(v));
            EXPECT_FALSE(g.isBoundary(v));
            EXPECT_NE(g.cycleProjection(v), v);
            EXPECT_THROW({
                g.snfRep(v);
            }, regina::InvalidArgument);
            EXPECT_THROW({
                g.asBoundary(v);
            }, regina::InvalidArgument);
        }

        static void verifyCycleNonBoundary(const MarkedAbelianGroup& g,
                const VectorInt& v) {
            SCOPED_TRACE_REGINA(v);

            EXPECT_TRUE(g.isCycle(v));
            EXPECT_FALSE(g.isBoundary(v));
            EXPECT_EQ(g.cycleProjection(v), v);
            EXPECT_THROW({
                g.asBoundary(v);
            }, regina::InvalidArgument);
        }

        static void verifyCycleNonBoundary(const MarkedAbelianGroup& g,
                const VectorInt& v, const VectorInt& snf) {
            verifyCycleNonBoundary(g, v);

            SCOPED_TRACE_REGINA(v);
            EXPECT_EQ(g.snfRep(v), snf);
        }

        static void verifyBoundary(const MarkedAbelianGroup& g,
                const VectorInt& v, const VectorInt& bdry) {
            EXPECT_TRUE(g.isCycle(v));
            EXPECT_TRUE(g.isBoundary(v));
            EXPECT_EQ(g.cycleProjection(v), v);
            EXPECT_TRUE(g.snfRep(v).isZero());
            EXPECT_EQ(g.asBoundary(v), bdry);
        }
};

TEST_F(MarkedAbelianGroupTest, lst) {
    Triangulation<3> t = regina::Example<3>::lst(3, 5);

    MarkedAbelianGroup g = t.markedHomology();
    EXPECT_TRUE(g.isZ());

    // Ensure that the degree 1 edge is where it should be.
    regina::Edge<3>* e = t.simplex(0)->edge(0);
    ASSERT_TRUE(e->isBoundary());
    ASSERT_EQ(e->degree(), 1);

    VectorInt snf = g.snfRep(VectorInt::unit(
        t.countEdges(), e->index()));
    EXPECT_EQ(snf.size(), 1);
    EXPECT_EQ(snf[0].abs(), 8);
}

TEST_F(MarkedAbelianGroupTest, kleinBottle) {
    Triangulation<2> t = Triangulation<2>::fromGluings(4, {
        { 0, 0, 1, {} },
        { 0, 1, 3, {0,2} },
        { 0, 2, 3, {} },
        { 1, 1, 2, {} },
        { 1, 2, 2, {0,1} },
        { 2, 0, 3, {} },
    });

    MatrixInt b1 = t.boundaryMap<1>();
    MatrixInt b2 = t.boundaryMap<2>();

    EXPECT_EQ(b1, MatrixInt({{-1,0,1,0,1,1}, {1,0,-1,0,-1,-1}}));
    EXPECT_EQ(b2, MatrixInt({{1,0,0,1}, {-1,0,0,1}, {1,1,0,0},
            {0,1,-1,0}, {0,-1,-1,0}, {0,0,1,1}}));

    // Make these assertions, since the dimensions need to be correct for the
    // subsequent tests to be valid.
    MarkedAbelianGroup g = t.markedHomology();
    ASSERT_EQ(g.str(), "Z + Z_2 (Z^4 -> Z^6 -> Z^2)");
    ASSERT_EQ(g.rank(), 1);
    ASSERT_EQ(g.torsionRank(2), 1);
    ASSERT_EQ(g.snfRank(), 2);
    ASSERT_EQ(g.ccRank(), 6);
    ASSERT_EQ(g.cycleRank(), 5);

    EXPECT_EQ(g.freeRep(0), VectorInt({1,0,0,0,0,1}));
    EXPECT_EQ(g.torsionRep(0), VectorInt({1,0,0,0,1,0}));
    EXPECT_EQ(g.ccRep(0), g.torsionRep(0));
    EXPECT_EQ(g.ccRep(1), g.freeRep(0));
    EXPECT_EQ(g.ccRep({1,-1}), g.torsionRep(0) - g.freeRep(0));

    // It seems cycleProjection() is adjusting the 0th chain
    // complex coordinate to produce a cycle, though this
    // particular choice of adjustment is not guaranteed.
    for (int i = 0; i < 6; ++i) {
        SCOPED_TRACE_NAMED_NUMERIC("edge", i);

        auto p = g.cycleProjection(i);
        if (i == 1 || i == 3) {
            // This edge is a cycle.
            EXPECT_EQ(p, VectorInt::unit(6, i));
            EXPECT_TRUE(g.isCycle(VectorInt::unit(6, i)));
        } else if (i == 0) {
            // This edge is not a cycle.
            EXPECT_TRUE(p.isZero());
            verifyNonCycle(g, VectorInt::unit(6, i));
        } else {
            // This edge is not a cycle.
            EXPECT_EQ(p, VectorInt::unit(6, i) + VectorInt::unit(6, 0));
            verifyNonCycle(g, VectorInt::unit(6, i));
        }
        EXPECT_TRUE(g.isCycle(p));
    }
    EXPECT_EQ(g.cycleProjection({3,-1,0,0,2,-4}), VectorInt({-2,-1,0,0,2,-4}));

    // Examine some chains that are not cycles:
    verifyNonCycle(g, {1,0,0,0,0,0});
    verifyNonCycle(g, {1,0,0,0,-1,0});

    // Examine some cycles that are not boundaries:
    verifyCycleNonBoundary(g, {1,0,0,0,1,0}, {1,0});
    verifyCycleNonBoundary(g, {3,0,0,0,3,0}, {1,0});
    verifyCycleNonBoundary(g, {1,0,0,0,0,1}, {0,1});
    verifyCycleNonBoundary(g, {0,1,0,0,0,0}, {0,-1});
    verifyCycleNonBoundary(g, {0,3,0,0,0,0}, {0,-3});
    verifyCycleNonBoundary(g, {1,1,1,0,0,0}, {0,-2});

    // Examine some cycles that are boundaries:
    verifyBoundary(g, {0,0,0,0,0,0}, {0,0,0,0});
    verifyBoundary(g, {2,0,0,0,2,0}, {1,-1,-1,1}); // 2x Z_2 gen
    verifyBoundary(g, {4,0,0,0,4,0}, {2,-2,-2,2}); // 4x Z_2 gen
    verifyBoundary(g, {1,-1,1,0,0,0}, {1,0,0,0}); // Bdry of T0
    verifyBoundary(g, {0,0,1,0,-2,1}, {0,1,1,0}); // Bdry of T1 u T2

    for (unsigned long i = 0; i < g.cycleRank(); ++i)
        verifyCycleNonBoundary(g, g.cycleGen(i));

    MarkedAbelianGroup tor = g.torsionSubgroup();
    EXPECT_EQ(tor.unmarked().str(), "Z_2");

    regina::HomMarkedAbelianGroup hom = g.torsionInclusion();
    MatrixInt m = hom.reducedMatrix();
    EXPECT_EQ(m.rows(), 2);
    EXPECT_EQ(m.columns(), 1);
    EXPECT_EQ(m.entry(0, 0), 1);
    EXPECT_EQ(m.entry(1, 0), 0);
}
