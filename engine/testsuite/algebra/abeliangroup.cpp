
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

#include "algebra/abeliangroup.h"
#include "maths/matrix.h"

#include "utilities/tightencodingtest.h"

using regina::Integer;
using regina::AbelianGroup;
using regina::MatrixInt;

// These are a pretty limited set of tests.  But they're a start. -ryan

class AbelianGroupTest : public testing::Test,
        public TightEncodingTest<AbelianGroup> {
    protected:
        void verifyGroup(const AbelianGroup& g,
                size_t rank, std::initializer_list<int> torsion) {
            SCOPED_TRACE_REGINA(g);

            EXPECT_EQ(g.rank(), rank);
            EXPECT_EQ(g.countInvariantFactors(), torsion.size());

            auto it = torsion.begin();
            for (size_t i = 0; i < torsion.size(); ++i) {
                SCOPED_TRACE_NUMERIC(i);
                EXPECT_EQ(g.invariantFactor(i), *it);
                ++it;
            }
        }
};

TEST_F(AbelianGroupTest, incrementalBuild) {
    AbelianGroup g;

    g.addRank(); verifyGroup(g, 1, {});
    g.addRank(2); verifyGroup(g, 3, {});
    g.addRank(-3); verifyGroup(g, 0, {});
    g.addRank(); verifyGroup(g, 1, {});

    g.addTorsion(2); verifyGroup(g, 1, { 2 });
    g.addTorsion(3); verifyGroup(g, 1, { 6 });
    g.addTorsion(4); verifyGroup(g, 1, { 2, 12 });
    g.addTorsion(2); verifyGroup(g, 1, { 2, 2, 12 });
    g.addTorsion(5); verifyGroup(g, 1, { 2, 2, 60 });
    g.addTorsion(21); verifyGroup(g, 1, { 2, 6, 420 });
    g.addTorsion(4); verifyGroup(g, 1, { 2, 2, 12, 420 });

    // A "safe" version of self-addition.
    // We will test "real" self-addition later.
    g.addGroup(AbelianGroup(g));
        verifyGroup(g, 2, { 2, 2, 2, 2, 12, 12, 420, 420 });

    // We try both m and its transpose.
    // When using m, there is a redundant row which gives a
    // redundant relation that we can ignore.
    // When using the transpose, there is a redundant column which
    // gives an extra generator (i.e., extra rank).
    MatrixInt m {{7,3,10}, {6,3,9}, {1,0,1}, {5,3,8}};
    g.addGroup(m); verifyGroup(g, 3, { 2, 2, 2, 6, 12, 12, 420, 420 });
    g.addGroup(m.transpose()); verifyGroup(g, 5,
        { 2, 2, 6, 6, 12, 12, 420, 420 });

    // Back to self-addition:
    g.addGroup(g); verifyGroup(g, 10,
        { 2, 2, 2, 2, 6, 6, 6, 6, 12, 12, 12, 12, 420, 420, 420, 420 });
}

TEST_F(AbelianGroupTest, tightEncoding) {
    verifyTightEncoding(AbelianGroup());
    verifyTightEncoding(AbelianGroup(1));
    verifyTightEncoding(AbelianGroup(1000000));
    verifyTightEncoding(AbelianGroup(0, { 2 }));
    verifyTightEncoding(AbelianGroup(1, { 2 }));
    verifyTightEncoding(AbelianGroup(1, { 2, 12 }));
    verifyTightEncoding(AbelianGroup(3, { 2, 2, 12, 420 }));
    verifyTightEncoding(AbelianGroup(0, { 2, 2, 12, 420, 420 }));
    verifyTightEncoding(AbelianGroup(3, { 2, 2, 12, 420, 420 }));
}
