
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

#include "maths/permtest.h"

using regina::Perm;

class Perm5Test : public SmallPermTest<5> {
};

TEST_F(Perm5Test, permCode) {
    SmallPermTest<5>::permCode();
}
TEST_F(Perm5Test, sign) {
    SmallPermTest<5>::sign();
}
TEST_F(Perm5Test, index) {
    SmallPermTest<5>::index();
}
TEST_F(Perm5Test, exhaustive) {
    SmallPermTest<5>::exhaustive();
}
TEST_F(Perm5Test, swaps) {
    SmallPermTest<5>::swaps();
}
TEST_F(Perm5Test, increment) {
    GeneralPermTest<5>::increment();
}
TEST_F(Perm5Test, products) {
    SmallPermTest<5>::products();
}
TEST_F(Perm5Test, cachedProducts) {
    SmallPermTest<5>::cachedProducts();
}
TEST_F(Perm5Test, conjugates) {
    SmallPermTest<5>::conjugates();
}
TEST_F(Perm5Test, cachedConjugates) {
    SmallPermTest<5>::cachedConjugates();
}
TEST_F(Perm5Test, cachedInverse) {
    GeneralPermTest<5>::cachedInverse();
}
TEST_F(Perm5Test, compareWith) {
    SmallPermTest<5>::compareWith();
}
TEST_F(Perm5Test, reverse) {
    SmallPermTest<5>::reverse();
}
TEST_F(Perm5Test, clear) {
    SmallPermTest<5>::clear();
}
TEST_F(Perm5Test, order) {
    SmallPermTest<5>::order();
}
TEST_F(Perm5Test, cachedOrder) {
    SmallPermTest<5>::cachedOrder();
}
TEST_F(Perm5Test, pow) {
    SmallPermTest<5>::pow();
}
TEST_F(Perm5Test, cachedPow) {
    SmallPermTest<5>::cachedPow();
}
TEST_F(Perm5Test, rot) {
    SmallPermTest<5>::rot();
}
TEST_F(Perm5Test, conjugacyMinimal) {
    GeneralPermTest<5>::conjugacyMinimal();
}
TEST_F(Perm5Test, tightEncoding) {
    SmallPermTest<5>::tightEncoding();
}

TEST_F(Perm5Test, databases) {
    for (int i = 0; i < 24; ++i)
        EXPECT_EQ(Perm<5>::S4[i], Perm<5>::extend(regina::Perm<4>::S4[i]));
    for (int i = 0; i < 24; ++i)
        EXPECT_EQ(Perm<5>::orderedS4[i],
            Perm<5>::extend(regina::Perm<4>::orderedS4[i]));
    for (int i = 0; i < 6; ++i)
        EXPECT_EQ(Perm<5>::S3[i], Perm<5>::extend(regina::Perm<4>::S3[i]));
    for (int i = 0; i < 6; ++i)
        EXPECT_EQ(Perm<5>::orderedS3[i],
            Perm<5>::extend(regina::Perm<4>::orderedS3[i]));
    for (int i = 0; i < 2; ++i)
        EXPECT_EQ(Perm<5>::S2[i], Perm<5>::extend(regina::Perm<4>::S2[i]));
}

TEST_F(Perm5Test, aliases) {
    for (int i = 0; i < 120; ++i)
        EXPECT_EQ(Perm<5>::S5[i], Perm<5>::Sn[i]);

    for (int i = 0; i < 24; ++i)
        EXPECT_EQ(Perm<5>::S4[i], Perm<5>::Sn_1[i]);
}

TEST_F(Perm5Test, S2) {
    for (int i = 0; i < 2; ++i) {
        EXPECT_TRUE(looksEqual(Perm<5>::S2[i],
                Perm<5>::extend(Perm<2>::S2[i])));
        EXPECT_EQ(Perm<2>::S2[i], Perm<2>::contract(Perm<5>::S2[i]));
    }
}

TEST_F(Perm5Test, S3) {
    for (int i = 0; i < 6; ++i) {
        EXPECT_TRUE(looksEqual(Perm<5>::S3[i],
                Perm<5>::extend(Perm<3>::S3[i])));
        EXPECT_TRUE(looksEqual(Perm<5>::orderedS3[i],
                Perm<5>::extend(Perm<3>::orderedS3[i])));
        EXPECT_EQ(Perm<3>::S3[i], Perm<3>::contract(Perm<5>::S3[i]));
        EXPECT_EQ(Perm<3>::orderedS3[i],
            Perm<3>::contract(Perm<5>::orderedS3[i]));
    }
}

TEST_F(Perm5Test, S4) {
    for (int i = 0; i < 24; ++i) {
        EXPECT_TRUE(looksEqual(Perm<5>::S4[i],
                Perm<5>::extend(Perm<4>::S4[i])));
        EXPECT_TRUE(looksEqual(Perm<5>::Sn_1[i],
                Perm<5>::extend(Perm<4>::S4[i])));
        EXPECT_TRUE(looksEqual(Perm<5>::orderedS4[i],
                Perm<5>::extend(Perm<4>::orderedS4[i])));
        EXPECT_EQ(Perm<4>::S4[i], Perm<4>::contract(Perm<5>::S4[i]));
        EXPECT_EQ(Perm<4>::S4[i], Perm<4>::contract(Perm<5>::Sn_1[i]));
        EXPECT_EQ(Perm<4>::orderedS4[i],
            Perm<4>::contract(Perm<5>::orderedS4[i]));
    }
}
