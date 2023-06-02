
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

#include "census/gluingpermsearcher4.h"
#include "triangulation/dim4.h"

#include "testhelper.h"

using regina::FacetPairing;
using regina::GluingPerms;
using regina::GluingPermSearcher;
using regina::Triangulation;
using regina::BoolSet;

class CensusTest : public testing::Test {
    protected:
        struct CensusSpec {
            BoolSet finite_;
            BoolSet orbl_;
            bool minimal_;

            size_t count_ { 0 };

            CensusSpec(BoolSet finite, BoolSet orbl, bool minimal) :
                finite_(finite), orbl_(orbl), minimal_(minimal) {}
        };

        template <int dim>
        static void foundPerms(const GluingPerms<dim>& p, CensusSpec* spec) {
            Triangulation<dim> tri = p.triangulate();
            if (tri.isValid() &&
                    (! (spec->orbl_ == true && ! tri.isOrientable())) &&
                    (! (spec->orbl_ == false && tri.isOrientable())) &&
                    (! (spec->finite_ == true && tri.isIdeal())) &&
                    (! (spec->finite_ == false && ! tri.isIdeal())))
                ++spec->count_;
        }

        template <int dim>
        static void foundPairing(const FacetPairing<dim>& pairing,
            typename FacetPairing<dim>::IsoList autos, CensusSpec* spec) {
            GluingPermSearcher<dim>::findAllPerms(pairing, std::move(autos),
                ! spec->orbl_.hasFalse(), ! spec->finite_.hasFalse(),
                foundPerms<dim>, spec);
        }

        template <int dim>
        static void rawCountsCompare(int minSize, int maxSize,
                const size_t* realAns,
                BoolSet finiteness, BoolSet orientability,
                BoolSet boundary, int nBdryFacets, bool minimal) {
            SCOPED_TRACE_BOOLSET(finiteness);
            SCOPED_TRACE_BOOLSET(orientability);
            SCOPED_TRACE_BOOLSET(boundary);
            SCOPED_TRACE_NUMERIC(nBdryFacets);
            SCOPED_TRACE_NUMERIC(minimal);

            if (dim == 4)
                ASSERT_EQ(minimal, false); // minimality not supported in 4-D

            for (int size = minSize; size <= maxSize; ++size) {
                SCOPED_TRACE_NUMERIC(size);

                CensusSpec spec(finiteness, orientability, minimal);
                FacetPairing<dim>::findAllPairings(size, boundary, nBdryFacets,
                    foundPairing<dim>, std::addressof(spec));
                EXPECT_EQ(spec.count_, realAns[size]);
            }
        }
};

TEST_F(CensusTest, dim4RawCounts) {
    static size_t nAll[] = { 1, 0, 23, 0, 8656, 0 };
    rawCountsCompare<4>(1, 3, nAll,
        BoolSet(true, true), BoolSet(true, true), false, 0, false);

    static size_t nOrientable[] = { 1, 0, 15, 0, 4150, 0 };
    rawCountsCompare<4>(1, 3, nOrientable,
        BoolSet(true, true), true, false, 0, false);
}

TEST_F(CensusTest, dim4RawCountsCompact) {
    static size_t nAll[] = { 1, 0, 10, 0 };
    rawCountsCompare<4>(1, 3, nAll,
        true, BoolSet(true, true), false, 0, false);

    static size_t nOrientable[] = { 1, 0, 8, 0 };
    rawCountsCompare<4>(1, 3, nOrientable,
        true, true, false, 0, false);
}

TEST_F(CensusTest, dim4RawCountsBounded) {
    static size_t nAll[] = { 1, 7, 51, 939, 25265 };
    rawCountsCompare<4>(1, 2, nAll,
        BoolSet(true, true), BoolSet(true, true), true, -1, false);

    static size_t nCompact[] = { 1, 5, 38, 782 };
    rawCountsCompare<4>(1, 2, nCompact,
        true, BoolSet(true, true), true, -1, false);

    static size_t nOrientable[] = { 1, 4, 27, 457 };
    rawCountsCompare<4>(1, 2, nOrientable,
        true, true, true, -1, false);
}
