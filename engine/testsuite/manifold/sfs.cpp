
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
#include "manifold/lensspace.h"

#include "gtest/gtest.h"

using regina::SFSFibre;
using regina::SFSpace;

SFSpace buildSFS( SFSpace::Class c, size_t genus,
        size_t punctures, size_t puncturesTwisted,
        size_t reflectors, size_t reflectorsTwisted,
        std::initializer_list<SFSFibre> fibres ) {
    SFSpace sfs( c, genus,
            punctures, puncturesTwisted,
            reflectors, reflectorsTwisted );
    for (const auto& f : fibres) {
        sfs.insertFibre(f);
    }
    return sfs;
}

void verifyLens( const SFSpace& sfs, bool expectedHasValue ) {
    EXPECT_EQ( sfs.isLensSpace().has_value(), expectedHasValue );
}

TEST(SFSTest, lens) {
    // Lens spaces usually have class o1 over the 2-sphere (genus 0), with at
    // most two exceptional fibres.
    SFSpace sfsOverS2 = buildSFS( SFSpace::Class::o1, 0, 0, 0, 0, 0, {} );
    verifyLens( sfsOverS2, true );
    sfsOverS2.insertFibre(5, 2);
    verifyLens( sfsOverS2, true );
    sfsOverS2.insertFibre(3, 1);
    verifyLens( sfsOverS2, true );
    sfsOverS2.insertFibre(5, 1);
    verifyLens( sfsOverS2, false );

    // Verify some cases which are obviously not lens spaces.
    verifyLens(
            buildSFS( SFSpace::Class::o1, 1, 0, 0, 0, 0, {} ),
            false );
    verifyLens(
            buildSFS( SFSpace::Class::bo1, 0, 1, 0, 0, 0, {} ),
            false );
    verifyLens(
            buildSFS( SFSpace::Class::bo1, 0, 0, 1, 0, 0, {} ),
            false );
    verifyLens(
            buildSFS( SFSpace::Class::bo1, 0, 0, 0, 1, 0, {} ),
            false );
    verifyLens(
            buildSFS( SFSpace::Class::bo1, 0, 0, 0, 0, 1, {} ),
            false );
    verifyLens(
            buildSFS( SFSpace::Class::o2, 1, 0, 0, 0, 0, {} ),
            false );
    verifyLens(
            buildSFS( SFSpace::Class::n2, 2, 0, 0, 0, 0, {} ),
            false );
}

void verifyName(const SFSpace& sfs, const char* expected) {
    EXPECT_EQ(sfs.name(), expected);
}

TEST(SFSTest, construct) {
    verifyName(
            buildSFS( SFSpace::Class::o1, 0, 0, 0, 0, 0, {} ),
            "S2 x S1" );
    verifyName(
            buildSFS( SFSpace::Class::o1, 1, 0, 0, 0, 0, {} ),
            "T x S1" );
    verifyName(
            buildSFS( SFSpace::Class::bo1, 0, 1, 0, 0, 0, {} ),
            "D x S1" );

    //TODO Actually construct some SFSpaces.
}
