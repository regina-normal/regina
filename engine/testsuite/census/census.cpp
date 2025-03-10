
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "census/census.h"
#include "census/gluingpermsearcher2.h"
#include "census/gluingpermsearcher3.h"
#include "census/gluingpermsearcher4.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

#include "testhelper.h"

using regina::BoolSet;
using regina::Census;
using regina::CensusHit;
using regina::FacetPairing;
using regina::GluingPerms;
using regina::GluingPermSearcher;
using regina::Triangulation;

class CensusTest : public testing::Test {
    protected:
        struct CensusSpec {
            BoolSet orbl_;
            BoolSet finite_;
            regina::Flags<regina::CensusPurge> purge_;
            bool minimal_;

            size_t count_ { 0 };

            CensusSpec(BoolSet orbl, BoolSet finite,
                    regina::Flags<regina::CensusPurge> purge, bool minimal) :
                    orbl_(orbl), finite_(finite), purge_(purge),
                    minimal_(minimal) {}
        };

        template <int dim>
        static void foundPerms(const GluingPerms<dim>& p, CensusSpec* spec) {
            Triangulation<dim> tri = p.triangulate();
            if (tri.isValid() &&
                    spec->orbl_.contains(tri.isOrientable()) &&
                    spec->finite_.contains(! tri.isIdeal())) {
                if constexpr (dim == 2) {
                    if (! (spec->minimal_ && ! tri.isMinimal()))
                        ++spec->count_;
                } else if constexpr (dim == 3) {
                    if (! (spec->minimal_ && tri.simplifyToLocalMinimum(false)))
                        ++spec->count_;
                } else if constexpr (dim == 4) {
                    ++spec->count_;
                }
            }
        }

        template <int dim>
        static void foundPairing(const FacetPairing<dim>& pairing,
            typename FacetPairing<dim>::IsoList autos, CensusSpec* spec) {
            if constexpr (dim == 2) {
                GluingPermSearcher<dim>::findAllPerms(pairing, std::move(autos),
                    ! spec->orbl_.hasFalse(), foundPerms<dim>, spec);
            } else if constexpr (dim == 3) {
                GluingPermSearcher<dim>::findAllPerms(pairing, std::move(autos),
                    ! spec->orbl_.hasFalse(), ! spec->finite_.hasFalse(),
                    spec->purge_, foundPerms<dim>, spec);
            } else if constexpr (dim == 4) {
                GluingPermSearcher<dim>::findAllPerms(pairing, std::move(autos),
                    ! spec->orbl_.hasFalse(), ! spec->finite_.hasFalse(),
                    foundPerms<dim>, spec);
            }
        }

        template <int dim>
        static void rawCountsCompare(int minSize, int maxSize,
                const size_t* realAns,
                BoolSet orientability,
                BoolSet finiteness,
                BoolSet boundary,
                int nBdryFacets,
                regina::Flags<regina::CensusPurge> purge = {},
                bool minimal = false) {
            SCOPED_TRACE_BOOLSET(orientability);
            SCOPED_TRACE_BOOLSET(finiteness);
            SCOPED_TRACE_BOOLSET(boundary);
            SCOPED_TRACE_NUMERIC(nBdryFacets);
            SCOPED_TRACE_NUMERIC(purge);
            SCOPED_TRACE_NUMERIC(minimal);

            // Some census parameters are not supported in some dimensions.
            if constexpr (dim == 2)
                ASSERT_TRUE(finiteness.full());
            if constexpr (dim != 3)
                ASSERT_EQ(purge, regina::CensusPurge::None);
            if constexpr (dim >= 4)
                ASSERT_EQ(minimal, false);

            for (int size = minSize; size <= maxSize; ++size) {
                SCOPED_TRACE_NUMERIC(size);

                CensusSpec spec(orientability, finiteness, purge, minimal);
                FacetPairing<dim>::findAllPairings(size, boundary, nBdryFacets,
                    foundPairing<dim>, std::addressof(spec));
                EXPECT_EQ(spec.count_, realAns[size]);
            }
        }
};

TEST_F(CensusTest, dim2RawCountsClosed) {
    // All counts taken from an enumeration using Regina 4.94.
    static size_t nAll[] = { 1, 0, 7, 0, 51, 0, 738, 0, 20540, 0, 911677 };
    rawCountsCompare<2>(1, 8, nAll,
        BoolSet(true, true), BoolSet(true, true), false, 0);

    static size_t nOrientable[] = { 1, 0, 3, 0, 11, 0, 73, 0, 838, 0, 15840};
    rawCountsCompare<2>(1, 10, nOrientable,
        true, BoolSet(true, true), false, 0);
}

TEST_F(CensusTest, dim2RawCountsBounded) {
    // All counts taken from an enumeration using Regina 4.94.
    static size_t nAll[] = { 1, 3, 6, 26, 105, 622, 3589, 28031, 202169 };
    rawCountsCompare<2>(1, 7, nAll,
        BoolSet(true, true), BoolSet(true, true), true, -1);

    static size_t nOrientable[] =
        { 1, 2, 4, 11, 41, 155, 750, 3967, 23260, 148885, 992299 };
    rawCountsCompare<2>(1, 8, nOrientable,
        true, BoolSet(true, true), true, -1);
}

TEST_F(CensusTest, dim2RawCountsClosedMinimal) {
    // All counts taken from an enumeration using Regina 4.94.
    static size_t nOrientable[] = { 1, 0, 3 /* sphere + torus */, 0, 0, 0,
        8, 0, 0, 0, 927 };
    rawCountsCompare<2>(1, 10, nOrientable,
        true, BoolSet(true, true), false, 0, regina::CensusPurge::None, true);

    static size_t nNonOrientable[] = { 1, 0, 4 /* PP + KB */, 0, 11, 0,
        144, 0, 3627, 0, 149288 };
    rawCountsCompare<2>(1, 8, nNonOrientable,
        false, BoolSet(true, true), false, 0, regina::CensusPurge::None, true);
}

TEST_F(CensusTest, dim3RawCounts) {
    static size_t nAll[] = { 1, 5, 61, 1581 };
    rawCountsCompare<3>(1, 3, nAll,
        BoolSet(true, true), BoolSet(true, true), false, 0);

    static size_t nOrientable[] = { 1, 4, 35, 454, 13776 };
    rawCountsCompare<3>(1, 3, nOrientable,
        true, BoolSet(true, true), false, 0);
}

TEST_F(CensusTest, dim3RawCountsCompact) {
    static size_t nAll[] = { 1, 4, 17, 81, 577, 5184, 57753 };
    rawCountsCompare<3>(1, 4, nAll,
        BoolSet(true, true), true, false, 0);

    static size_t nOrientable[] = { 1, 4, 16, 76, 532, 4807, 52946 };
    rawCountsCompare<3>(1, 4, nOrientable,
        true, true, false, 0);
}

TEST_F(CensusTest, dim3RawCountsPrimeMinimalOr) {
    static size_t nOrientable[] = { 1, 4, 11, 7, 17, 50 };
    rawCountsCompare<3>(1, 4, nOrientable,
        true, true, false, 0, regina::CensusPurge::NonMinimalPrime, true);
}

TEST_F(CensusTest, dim3RawCountsPrimeMinimalNor) {
    static size_t nNonOrientable[] = { 0, 0, 1, 0, 2, 4 };
    rawCountsCompare<3>(1, 4, nNonOrientable,
        false, true, false, 0,
        regina::CensusPurge::NonMinimalPrime | regina::CensusPurge::P2Reducible,
        true);
}

TEST_F(CensusTest, dim3RawCountsBounded) {
    static size_t nAll[] = { 1, 3, 17, 156, 2308 };
    rawCountsCompare<3>(1, 3, nAll,
        BoolSet(true, true), true, true, -1);

    static size_t nOrientable[] = { 1, 3, 14, 120, 1531 };
    rawCountsCompare<3>(1, 3, nOrientable,
        true, true, true, -1);
}

TEST_F(CensusTest, dim3RawCountsHypMin) {
    // Enforced: all vertices torus/KB, no low-degree edges.
    static size_t nAll[] = { 1, 1, 7, 31, 224, 1075, 6348 };
    rawCountsCompare<3>(1, 4, nAll,
        BoolSet(true, true), false, false, -1,
        regina::CensusPurge::NonMinimalHyp, false);

    static size_t nOrientable[] = { 1, 0, 3, 14, 113, 590, 3481 };
    rawCountsCompare<3>(1, 5, nOrientable,
        true, false, false, -1, regina::CensusPurge::NonMinimalHyp, false);
}

TEST_F(CensusTest, dim4RawCounts) {
    static size_t nAll[] = { 1, 0, 23, 0, 8656, 0 };
    rawCountsCompare<4>(1, 3, nAll,
        BoolSet(true, true), BoolSet(true, true), false, 0);

    static size_t nOrientable[] = { 1, 0, 15, 0, 4150, 0 };
    rawCountsCompare<4>(1, 3, nOrientable,
        true, BoolSet(true, true), false, 0);
}

TEST_F(CensusTest, dim4RawCountsCompact) {
    static size_t nAll[] = { 1, 0, 10, 0 };
    rawCountsCompare<4>(1, 3, nAll,
        BoolSet(true, true), true, false, 0);

    static size_t nOrientable[] = { 1, 0, 8, 0 };
    rawCountsCompare<4>(1, 3, nOrientable,
        true, true, false, 0);
}

TEST_F(CensusTest, dim4RawCountsBounded) {
    static size_t nAll[] = { 1, 7, 51, 939, 25265 };
    rawCountsCompare<4>(1, 2, nAll,
        BoolSet(true, true), BoolSet(true, true), true, -1);

    static size_t nCompact[] = { 1, 5, 38, 782 };
    rawCountsCompare<4>(1, 2, nCompact,
        BoolSet(true, true), true, true, -1);

    static size_t nOrientable[] = { 1, 4, 27, 457 };
    rawCountsCompare<4>(1, 2, nOrientable,
        true, true, true, -1);
}

static void verifyLookupNone(const char* isoSig) {
    SCOPED_TRACE_CSTRING(isoSig);

    auto hits = Census::lookup(isoSig);
    EXPECT_TRUE(hits.empty());
}

static void verifyLookup(const char* isoSig, const char* name) {
    SCOPED_TRACE_CSTRING(isoSig);

    auto hits = Census::lookup(isoSig);
    ASSERT_EQ(hits.size(), 1);
    EXPECT_EQ(hits.front().name(), name);
}

static void verifyLookup(const char* isoSig, const char* name1,
        const char* name2) {
    SCOPED_TRACE_CSTRING(isoSig);

    auto hits = Census::lookup(isoSig);
    ASSERT_EQ(hits.size(), 2);
    EXPECT_EQ(hits.front().name(), name1);
    EXPECT_EQ(hits.back().name(), name2);
}

TEST_F(CensusTest, lookup) {
    // Make sure that the database library is working, and that
    // we can access all censuses.

    verifyLookupNone("");
    verifyLookupNone("abcdefg");
    verifyLookup("fvPQcdecedekrsnrs",
        "SFS [S2: (2,1) (3,1) (5,-4)] : #1"); // closed or
    verifyLookup("kLLvLQQkcdjgjijhihihsfrovojgng",
        "Hyp_1.28448530 (Z_6) : #12",
        "1.2844853004683544 : m004(6, 1)"); // closed or, closed hyp
    verifyLookup("gvLQQcdefeffdwnplhe",
        "T x I / [ 1,1 | 1,0 ] : #1"); // closed nor
    verifyLookup("cPcbbbiht",
        "m004 : #1", "L104001"); // cusped or, hyp knots & links
    verifyLookup("bkaaid", "m000 : #1"); // cusped nor
    verifyLookup("kLLPLLQkceefejjiijiiiatdmpamxt",
        "L408001", "L410005"); // hyp knots & links, multiple times
}
