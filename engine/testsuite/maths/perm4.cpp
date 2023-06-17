
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

#include "triangulation/facenumbering.h"

#include "maths/permtest.h"

using regina::FaceNumbering;
using regina::Perm;

class Perm4Test : public SmallPermTest<4> {
};

TEST_F(Perm4Test, permCode) {
    SmallPermTest<4>::permCode();
}
TEST_F(Perm4Test, sign) {
    SmallPermTest<4>::sign();
}
TEST_F(Perm4Test, index) {
    SmallPermTest<4>::index();
}
TEST_F(Perm4Test, exhaustive) {
    SmallPermTest<4>::exhaustive();
}
TEST_F(Perm4Test, swaps) {
    SmallPermTest<4>::swaps();
}
TEST_F(Perm4Test, increment) {
    GeneralPermTest<4>::increment();
}
TEST_F(Perm4Test, products) {
    SmallPermTest<4>::products();
}
TEST_F(Perm4Test, cachedProducts) {
    SmallPermTest<4>::cachedProducts();
}
TEST_F(Perm4Test, conjugates) {
    SmallPermTest<4>::conjugates();
}
TEST_F(Perm4Test, cachedConjugates) {
    SmallPermTest<4>::cachedConjugates();
}
TEST_F(Perm4Test, cachedInverse) {
    GeneralPermTest<4>::cachedInverse();
}
TEST_F(Perm4Test, compareWith) {
    SmallPermTest<4>::compareWith();
}
TEST_F(Perm4Test, reverse) {
    SmallPermTest<4>::reverse();
}
TEST_F(Perm4Test, clear) {
    SmallPermTest<4>::clear();
}
TEST_F(Perm4Test, order) {
    SmallPermTest<4>::order();
}
TEST_F(Perm4Test, cachedOrder) {
    SmallPermTest<4>::cachedOrder();
}
TEST_F(Perm4Test, pow) {
    SmallPermTest<4>::pow();
}
TEST_F(Perm4Test, cachedPow) {
    SmallPermTest<4>::cachedPow();
}
TEST_F(Perm4Test, rot) {
    SmallPermTest<4>::rot();
}
TEST_F(Perm4Test, conjugacyMinimal) {
    GeneralPermTest<4>::conjugacyMinimal();
}
TEST_F(Perm4Test, tightEncoding) {
    SmallPermTest<4>::tightEncoding();
}

TEST_F(Perm4Test, pairs) {
    for (int idx = 0; idx < 24; ++idx) {
        Perm<4> p4 = Perm<4>::S4[idx];
        Perm<6> p6 = p4.pairs();

        for (int i = 0; i < 4; ++i)
            for (int j = i + 1; j < 4; ++j) {
                // Look at how p4 maps the pair {i,j}.
                int e = FaceNumbering<3,1>::edgeNumber[i][j];
                int f = FaceNumbering<3,1>::edgeNumber[p4[i]][p4[j]];
                EXPECT_EQ(p6[e], f);
            }
    }
}

TEST_F(Perm4Test, databases) {
    for (int i = 0; i < 6; ++i)
        EXPECT_EQ(Perm<4>::S3[i], Perm<4>::extend(regina::Perm<3>::S3[i]));
    for (int i = 0; i < 6; ++i)
        EXPECT_EQ(Perm<4>::orderedS3[i],
            Perm<4>::extend(regina::Perm<3>::orderedS3[i]));
    for (int i = 0; i < 2; ++i)
        EXPECT_EQ(Perm<4>::S2[i], Perm<4>::extend(regina::Perm<3>::S2[i]));
}

TEST_F(Perm4Test, aliases) {
    for (int i = 0; i < 24; ++i)
        EXPECT_EQ(Perm<4>::S4[i], Perm<4>::Sn[i]);

    for (int i = 0; i < 6; ++i)
        EXPECT_EQ(Perm<4>::S3[i], Perm<4>::Sn_1[i]);
}

TEST_F(Perm4Test, S2) {
    for (int i = 0; i < 2; ++i) {
        EXPECT_TRUE(looksEqual(Perm<4>::S2[i],
                Perm<4>::extend(Perm<2>::S2[i])));
        EXPECT_EQ(Perm<2>::S2[i], Perm<2>::contract(Perm<4>::S2[i]));
    }
}

TEST_F(Perm4Test, S3) {
    for (int i = 0; i < 6; ++i) {
        EXPECT_TRUE(looksEqual(Perm<4>::S3[i],
                Perm<4>::extend(Perm<3>::S3[i])));
        EXPECT_TRUE(looksEqual(Perm<4>::Sn_1[i],
                Perm<4>::extend(Perm<3>::S3[i])));
        EXPECT_TRUE(looksEqual(Perm<4>::orderedS3[i],
                Perm<4>::extend(Perm<3>::orderedS3[i])));
        EXPECT_EQ(Perm<3>::S3[i], Perm<3>::contract(Perm<4>::S3[i]));
        EXPECT_EQ(Perm<3>::S3[i], Perm<3>::contract(Perm<4>::Sn_1[i]));
        EXPECT_EQ(Perm<3>::orderedS3[i],
            Perm<3>::contract(Perm<4>::orderedS3[i]));
    }
}
