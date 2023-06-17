
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

class Perm3Test : public SmallPermTest<3> {
};

TEST_F(Perm3Test, permCode) {
    SmallPermTest<3>::permCode();
}
TEST_F(Perm3Test, sign) {
    SmallPermTest<3>::sign();
}
TEST_F(Perm3Test, index) {
    SmallPermTest<3>::index();
}
TEST_F(Perm3Test, exhaustive) {
    SmallPermTest<3>::exhaustive();
}
TEST_F(Perm3Test, swaps) {
    SmallPermTest<3>::swaps();
}
TEST_F(Perm3Test, increment) {
    GeneralPermTest<3>::increment();
}
TEST_F(Perm3Test, products) {
    SmallPermTest<3>::products();
}
TEST_F(Perm3Test, cachedProducts) {
    SmallPermTest<3>::cachedProducts();
}
TEST_F(Perm3Test, conjugates) {
    SmallPermTest<3>::conjugates();
}
TEST_F(Perm3Test, cachedConjugates) {
    SmallPermTest<3>::cachedConjugates();
}
TEST_F(Perm3Test, cachedInverse) {
    GeneralPermTest<3>::cachedInverse();
}
TEST_F(Perm3Test, compareWith) {
    SmallPermTest<3>::compareWith();
}
TEST_F(Perm3Test, reverse) {
    SmallPermTest<3>::reverse();
}
TEST_F(Perm3Test, clear) {
    SmallPermTest<3>::clear();
}
TEST_F(Perm3Test, order) {
    SmallPermTest<3>::order();
}
TEST_F(Perm3Test, cachedOrder) {
    SmallPermTest<3>::cachedOrder();
}
TEST_F(Perm3Test, pow) {
    SmallPermTest<3>::pow();
}
TEST_F(Perm3Test, cachedPow) {
    SmallPermTest<3>::cachedPow();
}
TEST_F(Perm3Test, rot) {
    SmallPermTest<3>::rot();
}
TEST_F(Perm3Test, conjugacyMinimal) {
    GeneralPermTest<3>::conjugacyMinimal();
}
TEST_F(Perm3Test, tightEncoding) {
    SmallPermTest<3>::tightEncoding();
}

TEST_F(Perm3Test, productsViaPerm4) {
    // An indirect test, using Perm<4> to verify.
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b) {
            if (b == a)
                continue;
            int c = 3 - a - b;
            Perm<3> x(a, b, c);

            for (int d = 0; d < 3; ++d)
                for (int e = 0; e < 3; ++e) {
                    if (e == d)
                        continue;
                    int f = 3 - d - e;
                    Perm<3> y(d, e, f);

                    auto product3 = x * y;
                    auto product4 = Perm<4>(a, b, c, 3) * Perm<4>(d, e, f, 3);

                    EXPECT_EQ(product3[0], product4[0]);
                    EXPECT_EQ(product3[1], product4[1]);
                    EXPECT_EQ(product3[2], product4[2]);
                }
        }
}

TEST_F(Perm3Test, aliases) {
    for (int i = 0; i < 6; ++i)
        EXPECT_EQ(Perm<3>::S3[i], Perm<3>::Sn[i]);

    for (int i = 0; i < 2; ++i)
        EXPECT_EQ(Perm<3>::S2[i], Perm<3>::Sn_1[i]);
}

TEST_F(Perm3Test, S2) {
    for (int i = 0; i < 2; ++i) {
        EXPECT_TRUE(looksEqual(Perm<3>::S2[i],
                Perm<3>::extend(Perm<2>::S2[i])));
        EXPECT_TRUE(looksEqual(Perm<3>::Sn_1[i],
                Perm<3>::extend(Perm<2>::S2[i])));
        EXPECT_EQ(Perm<2>::S2[i], Perm<2>::contract(Perm<3>::S2[i]));
        EXPECT_EQ(Perm<2>::S2[i], Perm<2>::contract(Perm<3>::Sn_1[i]));
    }
}
