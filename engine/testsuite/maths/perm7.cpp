
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

class Perm7Test : public SmallPermTest<7> {
};

TEST_F(Perm7Test, permCode) {
    SmallPermTest<7>::permCode();
}
TEST_F(Perm7Test, sign) {
    SmallPermTest<7>::sign();
}
TEST_F(Perm7Test, index) {
    SmallPermTest<7>::index();
}
TEST_F(Perm7Test, exhaustive) {
    SmallPermTest<7>::exhaustive();
}
TEST_F(Perm7Test, swaps) {
    SmallPermTest<7>::swaps();
}
TEST_F(Perm7Test, increment) {
    GeneralPermTest<7>::increment();
}
TEST_F(Perm7Test, products) {
    SmallPermTest<7>::products();
}
TEST_F(Perm7Test, cachedProducts) {
    SmallPermTest<7>::cachedProducts();
}
TEST_F(Perm7Test, conjugates) {
    SmallPermTest<7>::conjugates();
}
TEST_F(Perm7Test, cachedConjugates) {
    SmallPermTest<7>::cachedConjugates();
}
TEST_F(Perm7Test, cachedInverse) {
    GeneralPermTest<7>::cachedInverse();
}
TEST_F(Perm7Test, compareWith) {
    SmallPermTest<7>::compareWith();
}
TEST_F(Perm7Test, reverse) {
    SmallPermTest<7>::reverse();
}
TEST_F(Perm7Test, clear) {
    SmallPermTest<7>::clear();
}
TEST_F(Perm7Test, order) {
    SmallPermTest<7>::order();
}
TEST_F(Perm7Test, cachedOrder) {
    SmallPermTest<7>::cachedOrder();
}
TEST_F(Perm7Test, pow) {
    SmallPermTest<7>::pow();
}
TEST_F(Perm7Test, cachedPow) {
    SmallPermTest<7>::cachedPow();
}
TEST_F(Perm7Test, rot) {
    SmallPermTest<7>::rot();
}
TEST_F(Perm7Test, conjugacyMinimal) {
    GeneralPermTest<7>::conjugacyMinimal();
}
TEST_F(Perm7Test, tightEncoding) {
    SmallPermTest<7>::tightEncoding();
}

TEST_F(Perm7Test, aliases) {
    for (Perm<7>::Index i = 0; i < Perm<7>::nPerms; ++i)
        EXPECT_EQ(Perm<7>::S7[i], Perm<7>::Sn[i]);
}
