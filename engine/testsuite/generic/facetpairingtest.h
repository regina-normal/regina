
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

#include "triangulation/generic/isomorphism.h"
#include "triangulation/facetpairing.h"

#include "utilities/tightencodingtest.h"

using regina::FacetPairing;
using regina::Isomorphism;

/**
 * Implements several tests for facet pairings in dimension \a dim.
 *
 * Test suites can call these functions directly.  There is no need (or
 * benefit) for using inheritance of test fixture classes, other than the
 * minor convenience of not having to type out the template parameters for
 * FacetPairingTest every time it is used.
 */
template <int dim> requires (regina::supportedDim(dim))
class FacetPairingTest {
    protected:
        /**
         * Pre: p is in canonical form.
         */
        static void verifyMakeCanonical(const FacetPairing<dim>& p) {
            SCOPED_TRACE_REGINA(p);

            size_t nAutos = p.findAutomorphisms().size();
            Isomorphism<dim> iso = Isomorphism<dim>::identity(p.size());
            do {
                // Note: this trace (iso) causes a non-trivial performace hit.
                SCOPED_TRACE_REGINA(iso);

                FacetPairing<dim> alt = iso(p);
                auto [canon, canonIso] = alt.canonicalAll();

                EXPECT_EQ(canon, p);
                EXPECT_EQ(canonIso.size(), nAutos);
                EXPECT_EQ(canonIso.front()(alt), p);
                EXPECT_EQ(alt.isCanonical(), alt == p);

                ++iso;
            } while (! iso.isIdentity());
        }

    public:
        static void makeCanonicalAllClosed(size_t size) {
            FacetPairing<dim>::findAllPairings(size,
                false /* boundary */, 0 /* bdry facets */,
                [](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    verifyMakeCanonical(p);
                });
        }

        static void makeCanonicalAllBounded(size_t size) {
            FacetPairing<dim>::findAllPairings(size,
                true /* boundary */, -1 /* bdry facets */,
                [](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    verifyMakeCanonical(p);
                });
        }

        static void isCanonicalAllClosed(size_t size) {
            FacetPairing<dim>::findAllPairings(size,
                false /* boundary */, 0 /* bdry facets */,
                [](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    SCOPED_TRACE_REGINA(p);
                    EXPECT_TRUE(p.isCanonical());
                });
        }

        static void isCanonicalAllBounded(size_t size) {
            FacetPairing<dim>::findAllPairings(size,
                true /* boundary */, -1 /* bdry facets */,
                [](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    SCOPED_TRACE_REGINA(p);
                    EXPECT_TRUE(p.isCanonical());
                });
        }

        static void tightEncodingAllClosed(size_t size) {
            FacetPairing<dim>::findAllPairings(size,
                false /* boundary */, 0 /* bdry facets */,
                [](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    TightEncodingTest<FacetPairing<dim>>::
                        verifyTightEncoding(p);
                });
        }

        static void tightEncodingAllBounded(size_t size) {
            FacetPairing<dim>::findAllPairings(size,
                true /* boundary */, -1 /* bdry facets */,
                [](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    TightEncodingTest<FacetPairing<dim>>::
                        verifyTightEncoding(p);
                });
        }

        static void enumerateClosed(size_t size, size_t expectedCount) {
            SCOPED_TRACE_NUMERIC(size);

            size_t count = 0;
            FacetPairing<dim>::findAllPairings(size, false, 0,
                [&count](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    ++count;
                });
            EXPECT_EQ(count, expectedCount);
        }

        static void enumerateBounded(size_t size, size_t expectedCount) {
            SCOPED_TRACE_NUMERIC(size);

            size_t count = 0;
            FacetPairing<dim>::findAllPairings(size, true, -1,
                [&count](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    ++count;
                });
            EXPECT_EQ(count, expectedCount);
        }

        static void enumerateBounded(size_t size, size_t bdryFacets,
                size_t expectedCount) {
            SCOPED_TRACE_NUMERIC(size);
            SCOPED_TRACE_NUMERIC(bdryFacets);

            size_t count = 0;
            FacetPairing<dim>::findAllPairings(size, true, bdryFacets,
                [&count](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    ++count;
                });
            EXPECT_EQ(count, expectedCount);
        }
};

