
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
#include "subcomplex/blockedsfs.h"

#include "gtest/gtest.h"

using regina::SFSFibre;
using regina::SFSpace;
using regina::BlockedSFS;

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

void verifyStructureOrientable(const SFSpace& sfs) {
    // This test is designed for orientable SFS only. We therefore assume the
    // following:
    //  (1) sfs.baseClass() is one of o1, n2, bo1 or bn2.
    //  (2) There are no twisted punctures.
    //  (3) There are no reflectors at all (whether twisted or untwisted).
    //
    // This test also makes a couple of other important assumptions:
    //  (a) The implementation of sfs.construct() uses a blocked construction,
    //      which means that we can recover a Seifert fibration from the
    //      combinatorial structure of the triangulation. Any reasonable
    //      implementation would probably do this for all but a handful of
    //      special examples.
    //  (b) The given sfs is not one of the special cases which admits multiple
    //      non-isomorphic Seifert fibrations. This means that we can verify
    //      that the constructed triangulation has the correct homeomorphism
    //      type by simply comparing Seifert fibrations up to isomorphism.
    // The upshot is that it should be safe to use this test as long as the
    // given sfs is sufficiently generic.
    SFSpace reduced(sfs);
    reduced.reduce();
    auto blockedSFS = BlockedSFS::recognise( sfs.construct() );
    EXPECT_TRUE(blockedSFS);
    auto blockedManifold = blockedSFS->manifold();
    EXPECT_TRUE(blockedManifold);
    SFSpace compare = dynamic_cast<SFSpace&>(*blockedManifold);
    if ( reduced.baseClass() == SFSpace::Class::o1 or
            reduced.baseClass() == SFSpace::Class::n2 ) {
        // Closed orientable SFS, so the isomorphism class is determined not
        // only by the base surface and the (reduced) fibres, but also by the
        // obstruction constant.
        EXPECT_EQ( reduced, compare );
    } else {
        // Following the above, we assume sfs.baseClass() is either bo1 or bn2.
        //
        // The obstruction constant has no bearing on the isomorphism type, so
        // we just compare the base surface and the (reduced) fibres.
        EXPECT_EQ( reduced.baseClass(), compare.baseClass() );
        EXPECT_EQ( reduced.baseGenus(), compare.baseGenus() );
        EXPECT_EQ( reduced.punctures(), compare.punctures() );
        EXPECT_EQ( reduced.fibreCount(), compare.fibreCount() );
        unsigned long iFibre;
        for ( iFibre = 0; iFibre < reduced.fibreCount(); ++iFibre ) {
            EXPECT_EQ( reduced.fibre(iFibre), compare.fibre(iFibre) );
        }
    }
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

    //TODO Construct some "generic" orientable SFS that we can test with
    //      verifyStructureOrientable().
    //      --- Fibred over S^2 with >= 3 exceptional fibres.
    //      --- Orientable circle bundles with Euler number 0.
    //      --- Some examples covered by the most generic construction.
}
