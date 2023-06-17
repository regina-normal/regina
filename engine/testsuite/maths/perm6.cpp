
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

class Perm6Test : public SmallPermTest<6> {
};

TEST_F(Perm6Test, permCode) {
    SmallPermTest<6>::permCode();
}
TEST_F(Perm6Test, sign) {
    SmallPermTest<6>::sign();
}
TEST_F(Perm6Test, index) {
    SmallPermTest<6>::index();
}
TEST_F(Perm6Test, exhaustive) {
    SmallPermTest<6>::exhaustive();
}
TEST_F(Perm6Test, swaps) {
    SmallPermTest<6>::swaps();
}
TEST_F(Perm6Test, increment) {
    GeneralPermTest<6>::increment();
}
TEST_F(Perm6Test, products) {
    SmallPermTest<6>::products();
}
TEST_F(Perm6Test, cachedProducts) {
    SmallPermTest<6>::cachedProducts();
}
TEST_F(Perm6Test, conjugates) {
    SmallPermTest<6>::conjugates();
}
TEST_F(Perm6Test, cachedConjugates) {
    SmallPermTest<6>::cachedConjugates();
}
TEST_F(Perm6Test, cachedInverse) {
    GeneralPermTest<6>::cachedInverse();
}
TEST_F(Perm6Test, compareWith) {
    SmallPermTest<6>::compareWith();
}
TEST_F(Perm6Test, reverse) {
    SmallPermTest<6>::reverse();
}
TEST_F(Perm6Test, clear) {
    SmallPermTest<6>::clear();
}
TEST_F(Perm6Test, order) {
    SmallPermTest<6>::order();
}
TEST_F(Perm6Test, cachedOrder) {
    SmallPermTest<6>::cachedOrder();
}
TEST_F(Perm6Test, pow) {
    SmallPermTest<6>::pow();
}
TEST_F(Perm6Test, cachedPow) {
    SmallPermTest<6>::cachedPow();
}
TEST_F(Perm6Test, rot) {
    SmallPermTest<6>::rot();
}
TEST_F(Perm6Test, conjugacyMinimal) {
    GeneralPermTest<6>::conjugacyMinimal();
}
TEST_F(Perm6Test, tightEncoding) {
    SmallPermTest<6>::tightEncoding();
}

TEST_F(Perm6Test, aliases) {
    for (Perm<6>::Index i = 0; i < Perm<6>::nPerms; ++i)
        EXPECT_EQ(Perm<6>::S6[i], Perm<6>::Sn[i]);
}
