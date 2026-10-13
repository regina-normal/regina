
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

#include "gtest/gtest.h"

using regina::FailedPrecondition;
using regina::InvalidArgument;
using regina::SFSFibre;
using regina::SFSPartialReflector;
using regina::SFSpace;

void verifyName(const SFSpace& sfs, const char* expected) {
    EXPECT_EQ( sfs.name(), expected );
}

void verifyReducedStructure( const SFSpace& sfs, bool expectReflectionEqual,
        SFSpace::BundleType bundleType, size_t baseGenus,
        size_t punctures, size_t puncturesTwisted,
        size_t reflectors, size_t reflectorsTwisted,
        long obstruction,
        std::initializer_list<SFSPartialReflector> sortedParRefs,
        std::initializer_list<SFSPartialReflector> sortedParRefsTwisted,
        std::initializer_list<SFSFibre> sortedFibres) {
    SFSpace reduced(sfs);
    reduced.reduce();
    EXPECT_EQ( reduced.bundleType(), bundleType );
    EXPECT_EQ( reduced.baseGenus(), baseGenus );
    EXPECT_EQ( reduced.punctures(), punctures + puncturesTwisted );
    EXPECT_EQ( reduced.punctures(false), punctures );
    EXPECT_EQ( reduced.punctures(true), puncturesTwisted );
    EXPECT_EQ( reduced.reflectors(), reflectors + reflectorsTwisted );
    EXPECT_EQ( reduced.reflectors(false), reflectors );
    EXPECT_EQ( reduced.reflectors(true), reflectorsTwisted );
    EXPECT_EQ( reduced.partialReflectorCount(),
            sortedParRefs.size() + sortedParRefsTwisted.size() );
    EXPECT_EQ( reduced.partialReflectorCount(false), sortedParRefs.size() );
    EXPECT_EQ( reduced.partialReflectorCount(true),
            sortedParRefsTwisted.size() );
    EXPECT_TRUE( std::ranges::equal(
                reduced.partialReflectors(false), sortedParRefs ) );
    EXPECT_TRUE( std::ranges::equal(
                reduced.partialReflectors(true), sortedParRefsTwisted ) );
    EXPECT_EQ( reduced.fibreCount(), sortedFibres.size() );
    EXPECT_TRUE( std::ranges::equal(
                reduced.fibres(), sortedFibres ) );
    EXPECT_EQ( reduced.obstruction(), obstruction );

    // Verify reflection.
    SFSpace mirror(reduced);
    mirror.reflect();
    mirror.reduce(false); // Reduce without undoing the reflection.
    if (expectReflectionEqual) {
        EXPECT_EQ( reduced, mirror );
    } else {
        EXPECT_NE( reduced, mirror );
    }
}

TEST(SFSTest, structure) {
    const SFSpace sphereBundle{};
    const SFSpace torusBundle( SFSpace::BundleType::o1, 1 );
    const SFSpace discBundle( SFSpace::BundleType::o1, 0, 1 );

    {
        SCOPED_TRACE("Common names");

        verifyName( sphereBundle, "S2 x S1" );
        verifyName( torusBundle, "T x S1" );
        verifyName( discBundle, "D x S1" );
    }

    // For the structure tests below, we include some tests which ensure that
    // the constructors correctly convert degenerate parameters, such as:
    // - Removing (1,b) fibres by adjusting the obstruction constant.
    // - Reassigning partial reflectors with zero reflector arcs as punctures.

    {
        SCOPED_TRACE("Basic circle bundles");

        verifyReducedStructure(
                sphereBundle, true /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 0, 0, 0, 0, 0,
                0 /* obstruction */, {}, {}, {} );
        verifyReducedStructure(
                torusBundle, true /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 1, 0, 0, 0, 0,
                0 /* obstruction */, {}, {}, {} );
        verifyReducedStructure(
                discBundle, true /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 0, 1, 0, 0, 0,
                0 /* obstruction */, {}, {}, {} );
    }

    {
        SCOPED_TRACE("With twisted punctures and/or reflector circles");

        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o, 0, 1, 2 ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::o, 0, 1, 2, 0, 0,
                0 /* obstruction */, {}, {}, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o, 0, 1, 2, 3 ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::o, 0, 1, 2, 3, 0,
                0 /* obstruction */, {}, {}, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o, 0, 1, 2, 3, 4 ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::o, 0, 1, 2, 3, 4,
                0 /* obstruction */, {}, {}, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::n, 1, 0, 1, 0, 1 ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::n, 1, 0, 1, 0, 1,
                0 /* obstruction */, {}, {}, {} );
    }

    {
        SCOPED_TRACE("With reflector arcs");

        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o1, 0, 0, 0, 0, 0,
                    { SFSPartialReflector( false, 0 ) } ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 0, 1 /* punctures */, 0, 0, 0,
                0 /* obstruction */, {}, {}, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o1, 0, 0, 0, 0, 0,
                    { SFSPartialReflector( false, 1 ) } ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 0, 0 /* punctures */, 0, 0, 0,
                0 /* obstruction */,
                { SFSPartialReflector( false, 1 ) }, {}, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o,
                    0, 0, 1 /* puncturesTwisted */, 0, 0,
                    { SFSPartialReflector( true, 0 ) } ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::o, 0, 0, 2 /* puncturesTwisted */, 0, 0,
                0 /* obstruction */, {}, {}, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o,
                    0, 0, 1 /* puncturesTwisted */, 0, 0,
                    { SFSPartialReflector( true, 1 ) } ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::o, 0, 0, 1 /* puncturesTwisted */, 0, 0,
                0 /* obstruction */,
                {}, { SFSPartialReflector( true, 1 ) }, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::n,
                    1, 0, 0, 0, 2 /* reflectorsTwisted */,
                    { SFSPartialReflector( false, 2 ),
                    SFSPartialReflector( false, 1 ) } ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::n, 1, 0, 0, 0, 2 /* reflectorsTwisted */,
                0 /* obstruction */, { SFSPartialReflector( false, 1 ),
                SFSPartialReflector( false, 2 ) }, {}, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::n,
                    1, 0, 0, 0, 0 /* reflectorsTwisted */,
                    { SFSPartialReflector( true, 2 ),
                    SFSPartialReflector( true, 1 ) } ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::n, 1, 0, 0, 0, 0 /* reflectorsTwisted */,
                0 /* obstruction */, {}, { SFSPartialReflector( true, 1 ),
                SFSPartialReflector( true, 2 ) }, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::n,
                    1, 0, 0, 0, 1 /* reflectorsTwisted */,
                    { SFSPartialReflector( true, 2 ),
                    SFSPartialReflector( false, 1 ) } ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::n, 1, 0, 0, 0, 1 /* reflectorsTwisted */,
                0 /* obstruction */, { SFSPartialReflector( false, 1 ) },
                { SFSPartialReflector( true, 2 ) }, {} );
    }

    {
        SCOPED_TRACE("Closed with exceptional fibres");

        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o1, 1, 0 /* punctures */,
                    0, 0, 0, {}, { SFSFibre(1, -2) } ),
                false /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 1, 0 /* punctures */,
                0, 0, 0, 2 /* obstruction */, {}, {}, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o1, 1, 0 /* punctures */,
                    0, 0, 0, {}, { SFSFibre(3, 5) } ),
                false /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 1, 0 /* punctures */,
                0, 0, 0, 1 /* obstruction */, {}, {}, { SFSFibre(3, 2) } );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o1, 1, 0 /* punctures */,
                    0, 0, 0, {}, { SFSFibre(4, 1), SFSFibre(3, 5) } ),
                false /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 1, 0 /* punctures */,
                0, 0, 0, 1 /* obstruction */, {}, {},
                { SFSFibre(3, 2), SFSFibre(4, 1) } );
    }

    {
        SCOPED_TRACE("Bounded with exceptional fibres");

        // With non-empty boundary, SFSpace::reduce() now always reduces the
        // obstruction constant to zero.
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o1, 1, 1 /* punctures */,
                    0, 0, 0, {}, { SFSFibre(1, -2) } ),
                true /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 1, 1 /* punctures */,
                0, 0, 0, 0 /* obstruction */, {}, {}, {} );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o1, 1, 1 /* punctures */,
                    0, 0, 0, {}, { SFSFibre(3, 5) } ),
                false /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 1, 1 /* punctures */,
                0, 0, 0, 0 /* obstruction */, {}, {}, { SFSFibre(3, 1) } );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o1, 1, 1 /* punctures */,
                    0, 0, 0, {}, { SFSFibre(4, 1), SFSFibre(3, 5) } ),
                false /* expectReflectionEqual */,
                SFSpace::BundleType::o1, 1, 1 /* punctures */,
                0, 0, 0, 0 /* obstruction */, {}, {},
                { SFSFibre(3, 1), SFSFibre(4, 3) } );
    }

    {
        SCOPED_TRACE("Mismatched bundle type and base genus");

        EXPECT_THROW( SFSpace( SFSpace::BundleType::o2, 0 ),
                FailedPrecondition );
        EXPECT_THROW( SFSpace( SFSpace::BundleType::n, 0 ),
                FailedPrecondition );
        EXPECT_THROW( SFSpace( SFSpace::BundleType::n1, 0 ),
                FailedPrecondition );
        EXPECT_THROW( SFSpace( SFSpace::BundleType::n2, 0 ),
                FailedPrecondition );
        EXPECT_THROW( SFSpace( SFSpace::BundleType::n3, 0 ),
                FailedPrecondition );
        EXPECT_THROW( SFSpace( SFSpace::BundleType::n3, 1 ),
                FailedPrecondition );
        EXPECT_THROW( SFSpace( SFSpace::BundleType::n4, 0 ),
                FailedPrecondition );
        EXPECT_THROW( SFSpace( SFSpace::BundleType::n4, 1 ),
                FailedPrecondition );
        EXPECT_THROW( SFSpace( SFSpace::BundleType::n4, 2 ),
                FailedPrecondition );
    }

    {
        SCOPED_TRACE("Twisted boundary exceptions");

        // Bundle types which should not have twisted boundaries.
        std::initializer_list<SFSpace::BundleType>
            bundleTypesNoTwistedBdry = {
                SFSpace::BundleType::o1,
                SFSpace::BundleType::o2,
                SFSpace::BundleType::n1,
                SFSpace::BundleType::n2,
                SFSpace::BundleType::n3,
                SFSpace::BundleType::n4
            };
        for (auto bundleType : bundleTypesNoTwistedBdry) {
            EXPECT_THROW(
                    SFSpace(bundleType, 3, 0, 2 /* puncturesTwisted */),
                    FailedPrecondition );
            EXPECT_THROW(
                    SFSpace(bundleType, 3, 0, 1 /* puncturesTwisted */,
                        0, 1 /* reflectorsTwisted */),
                    FailedPrecondition );
            EXPECT_THROW(
                    SFSpace(bundleType, 3, 0, 0 /* puncturesTwisted */,
                        0, 2 /* reflectorsTwisted */),
                    FailedPrecondition );
            EXPECT_THROW(
                    SFSpace(bundleType, 3, 0, 0 /* puncturesTwisted */,
                        0, 0 /* reflectorsTwisted */,
                        { SFSPartialReflector(true, 1),
                        SFSPartialReflector(true, 2) }),
                    FailedPrecondition );
            EXPECT_THROW(
                    SFSpace(bundleType, 3, 0, 1 /* puncturesTwisted */,
                        0, 0 /* reflectorsTwisted */,
                        { SFSPartialReflector(true, 1) }),
                    FailedPrecondition );
            EXPECT_THROW(
                    SFSpace(bundleType, 3, 0, 0 /* puncturesTwisted */,
                        0, 1 /* reflectorsTwisted */,
                        { SFSPartialReflector(true, 1) }),
                    FailedPrecondition );
        }

        // Bundle types which should have a positive even number of twisted
        // boundaries.
        std::initializer_list<SFSpace::BundleType>
            bundleTypesWithTwistedBdry = {
                SFSpace::BundleType::o,
                SFSpace::BundleType::n
            };
        for (auto bundleType : bundleTypesWithTwistedBdry) {
            EXPECT_THROW( SFSpace(bundleType, 3), FailedPrecondition );
            EXPECT_THROW(
                    SFSpace(bundleType, 3, 0, 1 /* puncturesTwisted */),
                    FailedPrecondition );
            EXPECT_THROW(
                    SFSpace(bundleType, 3, 0, 0 /* puncturesTwisted */,
                        0, 1 /* reflectorsTwisted */),
                    FailedPrecondition );
            EXPECT_THROW(
                    SFSpace(bundleType, 3, 0, 0 /* puncturesTwisted */,
                        0, 0 /* reflectorsTwisted */,
                        { SFSPartialReflector(true, 1) }),
                    FailedPrecondition );
            EXPECT_THROW(
                    SFSpace(bundleType, 3, 0, 1 /* puncturesTwisted */,
                        0, 1 /* reflectorsTwisted */,
                        { SFSPartialReflector(true, 1) }),
                    FailedPrecondition );
        }
    }
}

TEST(SFSTest, addGenus) {
    SFSpace sfs{};

    // Add handles/crosscaps which preserve orientability of the SFS.
    sfs.addHandle();
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o1 );
    EXPECT_EQ( sfs.baseGenus(), 1 );
    sfs.addCrosscap(true);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::n2 );
    EXPECT_EQ( sfs.baseGenus(), 3 );
    sfs.addCrosscap(true);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::n2 );
    EXPECT_EQ( sfs.baseGenus(), 4 );
    sfs.addHandle();
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::n2 );
    EXPECT_EQ( sfs.baseGenus(), 6 );

    // Now add handles/crosscaps which don't preserve orientability.
    sfs.addCrosscap();
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::n3 );
    EXPECT_EQ( sfs.baseGenus(), 7 );
    sfs.addHandle(true);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::n3 );
    EXPECT_EQ( sfs.baseGenus(), 9 );

    // To be safe, check that adding handles and crosscaps didn't change any
    // of the other properties.
    EXPECT_EQ( sfs.punctures(), 0 );
    EXPECT_EQ( sfs.reflectors(), 0 );
    EXPECT_EQ( sfs.partialReflectorCount(), 0 );
    EXPECT_EQ( sfs.fibreCount(), 0 );
    EXPECT_EQ( sfs.obstruction(), 0 );
}

TEST(SFSTest, addBoundary) {
    SFSpace sfs{};

    // Make sure we throw InvalidArgument (and leave sfs untouched) if we try
    // to add an odd number of twisted boundaries to the base orbifold, or if
    // we try to add a negative number of reflector arcs.
    EXPECT_THROW( sfs.addPuncture(true), InvalidArgument );
    EXPECT_THROW( sfs.addPuncture(true, 3), InvalidArgument );
    EXPECT_THROW( sfs.addReflector(true), InvalidArgument );
    EXPECT_THROW( sfs.addReflector(true, 3), InvalidArgument );
    EXPECT_THROW( sfs.addOrbifoldBoundary(true), InvalidArgument );
    EXPECT_THROW( sfs.addOrbifoldBoundary(true, 3), InvalidArgument );
    EXPECT_THROW( sfs.addOrbifoldBoundary(true, 0, 1), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(true, 0, 0, {1}), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(true, 1, 0, {1, 2}), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(true, 0, 1, {1, 2}), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(true, 1, 1, {1}), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(false, 0, 0, {-1}), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(true, 1, 0, {-1}), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(false, 0, 0, {1, -2}), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(true, 0, 0, {1, -2}), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(false, 1, 0,
                { SFSPartialReflector(true, 1) }), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(true, 1, 0,
                { SFSPartialReflector(false, 1) }), InvalidArgument );
    EXPECT_THROW(
            sfs.addOrbifoldBoundary(true, 1, 1,
                { SFSPartialReflector(true, 1) }), InvalidArgument );
    EXPECT_EQ( sfs, SFSpace() );

    // Now add a bunch of punctures, reflector circles and partial reflectors,
    // and check that things are updated correctly.

    sfs.addPuncture();
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o1 );
    EXPECT_EQ( sfs.punctures(), 1 );
    EXPECT_EQ( sfs.punctures(false), 1 );
    sfs.addPuncture(false);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o1 );
    EXPECT_EQ( sfs.punctures(), 2 );
    EXPECT_EQ( sfs.punctures(false), 2 );
    sfs.addPuncture(false, 2);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o1 );
    EXPECT_EQ( sfs.punctures(), 4 );
    EXPECT_EQ( sfs.punctures(false), 4 );
    sfs.addPuncture(true, 2);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.punctures(), 6 );
    EXPECT_EQ( sfs.punctures(true), 2 );

    sfs.addReflector();
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.reflectors(), 1 );
    EXPECT_EQ( sfs.reflectors(false), 1 );
    sfs.addReflector(false);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.reflectors(), 2 );
    EXPECT_EQ( sfs.reflectors(false), 2 );
    sfs.addReflector(false, 2);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.reflectors(), 4 );
    EXPECT_EQ( sfs.reflectors(false), 4 );
    sfs.addReflector(true, 2);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.reflectors(), 6 );
    EXPECT_EQ( sfs.reflectors(true), 2 );

    sfs.addOrbifoldBoundary();
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.punctures(), 7 );
    EXPECT_EQ( sfs.punctures(false), 5 );
    sfs.addOrbifoldBoundary(false);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.punctures(), 8 );
    EXPECT_EQ( sfs.punctures(false), 6 );
    sfs.addOrbifoldBoundary(false, 2);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.punctures(), 10 );
    EXPECT_EQ( sfs.punctures(false), 8 );
    sfs.addOrbifoldBoundary(false, 0, 1);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.reflectors(), 7 );
    EXPECT_EQ( sfs.reflectors(false), 5 );

    sfs.addOrbifoldBoundary(true, 2);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.punctures(), 12 );
    EXPECT_EQ( sfs.punctures(true), 4 );
    sfs.addOrbifoldBoundary(true, 0, 2);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.reflectors(), 9 );
    EXPECT_EQ( sfs.reflectors(true), 4 );

    sfs.addOrbifoldBoundary(false, 1, 1);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.punctures(), 13 );
    EXPECT_EQ( sfs.punctures(false), 9 );
    EXPECT_EQ( sfs.reflectors(), 10 );
    EXPECT_EQ( sfs.reflectors(false), 6 );
    sfs.addOrbifoldBoundary(true, 1, 1);
    EXPECT_EQ( sfs.bundleType(), SFSpace::BundleType::o );
    EXPECT_EQ( sfs.punctures(), 14 );
    EXPECT_EQ( sfs.punctures(true), 5 );
    EXPECT_EQ( sfs.reflectors(), 11 );
    EXPECT_EQ( sfs.reflectors(true), 5 );

    //TODO Add partial reflectors (using both variants of the routine).
}
