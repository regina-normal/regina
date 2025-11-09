
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

#include "manifold/sfs.h"

#include "gtest/gtest.h"

using regina::SFSFibre;
using regina::SFSpace;

//TODO Test new SFS constructions.

void verifyName(SFSpace::Class c, size_t genus,
        size_t punctures, size_t puncturesTwisted,
        size_t reflectors, size_t reflectorsTwisted,
        std::initializer_list<SFSFibre> fibres,
        const char* expected) {
    SFSpace s(c, genus, punctures, puncturesTwisted,
        reflectors, reflectorsTwisted);
    for (const auto& f : fibres)
        s.insertFibre(f);

    EXPECT_EQ(s.name(), expected);
}

TEST(SFSTest, construct) {
    verifyName(SFSpace::Class::o1, 0, 0, 0, 0, 0, {}, "S2 x S1");
    verifyName(SFSpace::Class::o1, 1, 0, 0, 0, 0, {}, "T x S1");
    verifyName(SFSpace::Class::bo1, 0, 1, 0, 0, 0, {}, "D x S1");
}
