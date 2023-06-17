
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

class Perm2Test : public SmallPermTest<2> {
};

TEST_F(Perm2Test, permCode) {
    SmallPermTest<2>::permCode();
}
TEST_F(Perm2Test, sign) {
    SmallPermTest<2>::sign();
}
TEST_F(Perm2Test, index) {
    SmallPermTest<2>::index();
}
TEST_F(Perm2Test, exhaustive) {
    SmallPermTest<2>::exhaustive();
}
TEST_F(Perm2Test, swaps) {
    SmallPermTest<2>::swaps();
}
TEST_F(Perm2Test, increment) {
    GeneralPermTest<2>::increment();
}
TEST_F(Perm2Test, products) {
    SmallPermTest<2>::products();
}
TEST_F(Perm2Test, cachedProducts) {
    SmallPermTest<2>::cachedProducts();
}
TEST_F(Perm2Test, conjugates) {
    SmallPermTest<2>::conjugates();
}
TEST_F(Perm2Test, cachedConjugates) {
    SmallPermTest<2>::cachedConjugates();
}
TEST_F(Perm2Test, cachedInverse) {
    GeneralPermTest<2>::cachedInverse();
}
TEST_F(Perm2Test, compareWith) {
    SmallPermTest<2>::compareWith();
}
TEST_F(Perm2Test, reverse) {
    SmallPermTest<2>::reverse();
}
TEST_F(Perm2Test, clear) {
    SmallPermTest<2>::clear();
}
TEST_F(Perm2Test, order) {
    SmallPermTest<2>::order();
}
TEST_F(Perm2Test, cachedOrder) {
    SmallPermTest<2>::cachedOrder();
}
TEST_F(Perm2Test, pow) {
    SmallPermTest<2>::pow();
}
TEST_F(Perm2Test, cachedPow) {
    SmallPermTest<2>::cachedPow();
}
TEST_F(Perm2Test, rot) {
    SmallPermTest<2>::rot();
}
TEST_F(Perm2Test, conjugacyMinimal) {
    GeneralPermTest<2>::conjugacyMinimal();
}
TEST_F(Perm2Test, tightEncoding) {
    SmallPermTest<2>::tightEncoding();
}

TEST_F(Perm2Test, productsViaPerm4) {
    // An indirect test, using Perm<4> to verify.
    for (int a = 0; a < 2; ++a) {
        auto x = Perm<2>::S2[a]; // 0 -> a, 1 -> 1-a.

        for (int d = 0; d < 2; ++d) {
            auto y = Perm<2>::S2[d]; // 0 -> d, 1 -> 1-d.

            auto product2 = x * y;
            auto product4 = Perm<4>(a, 1 - a, 2, 3) * Perm<4>(d, 1 - d, 2, 3);

            EXPECT_EQ(product2[0], product4[0]);
            EXPECT_EQ(product2[1], product4[1]);
        }
    }
}

TEST_F(Perm2Test, aliases) {
    for (int i = 0; i < 2; ++i)
        EXPECT_EQ(Perm<2>::S2[i], Perm<2>::Sn[i]);

    EXPECT_EQ(Perm<2>::S1[0], Perm<2>::Sn_1[0]);
}
