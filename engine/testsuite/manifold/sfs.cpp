
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

bool hasSameFibres( const SFSpace& reference, const SFSpace& compare,
        bool allowReflect=true){
    if ( reference.fibreCount() != compare.fibreCount() ) {
        return false;
    }
    if (allowReflect) {
        if ( hasSameFibres( reference, compare, false ) ) {
            return true;
        }

        // Try the reflection instead.
        SFSpace reflected(compare);
        reflected.reflect();
        return hasSameFibres( reference, reflected, false );
    }
    size_t iFibre;
    for ( iFibre = 0; iFibre < reference.fibreCount(); ++iFibre ) {
        if ( reference.fibre(iFibre) != compare.fibre(iFibre) ) {
            return false;
        }
    }
    return true;
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
    SCOPED_TRACE( reduced.name() );

    ASSERT_NO_THROW( sfs.construct() );
    auto blockedSFS = BlockedSFS::recognise( sfs.construct() );
    ASSERT_TRUE(blockedSFS);
    auto blockedManifold = blockedSFS->manifold();
    ASSERT_TRUE(blockedManifold);
    SFSpace compare = dynamic_cast<SFSpace&>(*blockedManifold);
    compare.reduce();
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

        // For comparing the fibres, we need to allow reflections, because the
        // current implementation of SFSpace::reduce() doesn't fully reduce b_
        // for orientable SFS with non-empty boundary.
        //
        //TODO Improve SFSpace::reduce() to address this. The obstacle to
        //      making such an improvement is that there are tests for
        //      StandardTriangulation (and possibly also other parts of the
        //      codebase) which rely on the current implementation.
        EXPECT_TRUE( hasSameFibres( reduced, compare ) );
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

    {
        SCOPED_TRACE("Orientable SFS over S^2 with >= 3 exceptional fibres");

        verifyStructureOrientable(
                buildSFS( SFSpace::Class::o1, 0, 0, 0, 0, 0,
                    { SFSFibre(2, 1), SFSFibre(5, 1), SFSFibre(5, -2) } ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::o1, 0, 0, 0, 0, 0,
                    { SFSFibre(3, 1), SFSFibre(7, -2), SFSFibre(7, 2) } ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::o1, 0, 0, 0, 0, 0,
                    { SFSFibre(2, 1), SFSFibre(5, 1), SFSFibre(5, -2),
                    SFSFibre(2, 3) } ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::o1, 0, 0, 0, 0, 0,
                    { SFSFibre(3, 1), SFSFibre(7, -2), SFSFibre(7, 2),
                    SFSFibre(7, -3) } ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::o1, 0, 0, 0, 0, 0,
                    { SFSFibre(2, 1), SFSFibre(5, 1), SFSFibre(5, -2),
                    SFSFibre(2, 3), SFSFibre(8, 3) } ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::o1, 0, 0, 0, 0, 0,
                    { SFSFibre(3, 1), SFSFibre(7, -2), SFSFibre(7, 2),
                    SFSFibre(7, -3), SFSFibre(5, -2), SFSFibre(5, 8) } ) );
    }

    {
        SCOPED_TRACE("Orientable circle bundles with Euler number 0");

        verifyStructureOrientable(
                buildSFS( SFSpace::Class::o1, 2, 0, 0, 0, 0, {} ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::bo1, 0, 2, 0, 0, 0, {} ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::bo1, 1, 1, 0, 0, 0, {} ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::n2, 2, 0, 0, 0, 0, {} ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::bn2, 1, 2, 0, 0, 0, {} ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::bn2, 2, 1, 0, 0, 0, {} ) );
    }

    {
        SCOPED_TRACE("Orientable SFS over D^2 with >= 2 exceptional fibres");

        verifyStructureOrientable(
                buildSFS( SFSpace::Class::bo1, 0, 1, 0, 0, 0,
                    { SFSFibre(5, 1), SFSFibre(5, -2) } ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::bo1, 0, 1, 0, 0, 0,
                    { SFSFibre(3, 1), SFSFibre(7, -2), SFSFibre(7, 2) } ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::bo1, 0, 1, 0, 0, 0,
                    { SFSFibre(5, 1), SFSFibre(5, -2), SFSFibre(2, 3) } ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::bo1, 0, 1, 0, 0, 0,
                    { SFSFibre(3, 1), SFSFibre(7, -2), SFSFibre(7, 2),
                    SFSFibre(7, -3) } ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::bo1, 0, 1, 0, 0, 0,
                    { SFSFibre(2, 1), SFSFibre(5, 1), SFSFibre(5, -2),
                    SFSFibre(2, 3), SFSFibre(8, 3) } ) );
        verifyStructureOrientable(
                buildSFS( SFSpace::Class::bo1, 0, 1, 0, 0, 0,
                    { SFSFibre(3, 1), SFSFibre(7, -2), SFSFibre(7, 2),
                    SFSFibre(7, -3), SFSFibre(5, -2), SFSFibre(5, 8) } ) );
    }

    {
        SCOPED_TRACE("Orientable SFS over base neither S^2 nor D^2");

        long signedGenus;
        size_t genus, punctures;
        SFSpace::Class useClass;
        for (signedGenus = -2; signedGenus <= 2; ++signedGenus) {
            for (punctures = 0; punctures <= 2; ++punctures) {
                // Calculate genus and useClass for the base.
                if (signedGenus == 0) {
                    if (punctures < 2) {
                        continue;
                    }
                    genus = 0;
                    useClass = SFSpace::Class::bo1;
                } else if (signedGenus > 0) {
                    genus = signedGenus;
                    if (punctures == 0) {
                        useClass = SFSpace::Class::o1;
                    } else {
                        useClass = SFSpace::Class::bo1;
                    }
                } else {
                    genus = -signedGenus;
                    if (punctures == 0) {
                        useClass = SFSpace::Class::n2;
                    } else {
                        useClass = SFSpace::Class::bn2;
                    }
                }

                // Test with various sets of exceptional fibres.
                if ( signedGenus != -1 or punctures != 0 ) {
                    // To avoid lens spaces, only do this test when the base
                    // isn't a projective plane.
                    verifyStructureOrientable(
                            buildSFS( useClass, genus, punctures, 0, 0, 0,
                                { SFSFibre(3, 1) } ) );
                }
                verifyStructureOrientable(
                        buildSFS( useClass, genus, punctures, 0, 0, 0,
                            { SFSFibre(5, 1), SFSFibre(5, -2) } ) );
                verifyStructureOrientable(
                        buildSFS( useClass, genus, punctures, 0, 0, 0,
                            { SFSFibre(3, 1), SFSFibre(7, -2),
                            SFSFibre(7, 2) } ) );
                verifyStructureOrientable(
                        buildSFS( useClass, genus, punctures, 0, 0, 0,
                            { SFSFibre(3, 1), SFSFibre(7, -2),
                            SFSFibre(7, 2), SFSFibre(7, -3) } ) );
                verifyStructureOrientable(
                        buildSFS( useClass, genus, punctures, 0, 0, 0,
                            { SFSFibre(2, 1), SFSFibre(5, 1),
                            SFSFibre(5, -2), SFSFibre(2, 3),
                            SFSFibre(8, 3) } ) );
                verifyStructureOrientable(
                        buildSFS( useClass, genus, punctures, 0, 0, 0,
                            { SFSFibre(3, 1), SFSFibre(7, -2),
                            SFSFibre(7, 2), SFSFibre(7, -3),
                            SFSFibre(5, -2), SFSFibre(5, 8) } ) );
            }
        }
    }
}
