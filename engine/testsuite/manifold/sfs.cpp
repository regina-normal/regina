
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
using regina::SFSFibre;
using regina::SFSPartialReflector;
using regina::SFSpace;

void verifyName(const SFSpace& sfs, const char* expected) {
    EXPECT_EQ( sfs.name(), expected );
}

void verifyReducedStructure( const SFSpace& sfs, bool verifyReflection,
        SFSpace::BundleType bundleType, size_t baseGenus,
        size_t punctures, size_t puncturesTwisted,
        size_t reflectors, size_t reflectorsTwisted,
        long obstruction,
        std::initializer_list<SFSPartialReflector> sortedParRefs,
        std::initializer_list<SFSPartialReflector> sortedParRefsTwisted,
        std::initializer_list<SFSFibre> sortedFibres) {
    SFSpace reduced(sfs);
    reduced.reduce();
    if (verifyReflection) {
        SFSpace mirror(reduced);
        mirror.reflect();

        // Reduce *without* allowing reflections, and verify that the result
        // is different.
        mirror.reduce(false);
        EXPECT_NE( reduced, mirror );
    }
    EXPECT_EQ( reduced.bundleType(), bundleType );
    EXPECT_EQ( reduced.baseGenus(), baseGenus );
    EXPECT_EQ( reduced.punctures(), punctures + puncturesTwisted );
    EXPECT_EQ( reduced.punctures(false), punctures );
    EXPECT_EQ( reduced.punctures(true), puncturesTwisted );
    EXPECT_EQ( reduced.reflectors(), reflectors + reflectorsTwisted );
    EXPECT_EQ( reduced.reflectors(false), reflectors );
    EXPECT_EQ( reduced.reflectors(true), reflectorsTwisted );
    EXPECT_EQ( reduced.obstruction(), obstruction );
    EXPECT_TRUE( std::ranges::equal(
                reduced.partialReflectors(false), sortedParRefs ) );
    EXPECT_TRUE( std::ranges::equal(
                reduced.partialReflectors(true), sortedParRefsTwisted ) );
    EXPECT_TRUE( std::ranges::equal(
                reduced.fibres(), sortedFibres ) );
}

TEST(SFSTest, structure) {
    {
        SCOPED_TRACE("Common names");

        // Verify some Seifert fibred spaces with common names.
        verifyName( SFSpace(), "S2 x S1" );
        verifyName( SFSpace( SFSpace::BundleType::o1, 1 ), "T x S1" );
        verifyName( SFSpace( SFSpace::BundleType::o1, 0, 1 ), "D x S1" );
    }

    {
        SCOPED_TRACE("Reduced structure");

        // Verify structure of some more general Seifert fibred spaces. This
        // includes some tests which ensure that the constructor(s) correctly
        // convert degenerate parameters, such as:
        // - Removing (1,b) fibres by adjusting the obstruction constant.
        // - Reassigning partial reflectors with zero reflector arcs as punctures.
        //TODO
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o1, 1, 0 /* punctures */,
                    0, 0, 0, {}, { SFSFibre(3, 5) } ),
                true /* verifyReflection */,
                SFSpace::BundleType::o1, 1, 0 /* punctures */,
                0, 0, 0, 1 /* obstruction */, {}, {}, { SFSFibre(3, 2) } );
        verifyReducedStructure(
                SFSpace( SFSpace::BundleType::o1, 1, 1 /* punctures */,
                    0, 0, 0, {}, { SFSFibre(3, 5) } ),
                true /* verifyReflection */,
                SFSpace::BundleType::o1, 1, 1 /* punctures */,
                0, 0, 0, 0 /* obstruction */, {}, {}, { SFSFibre(3, 1) } );
        //TODO
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
