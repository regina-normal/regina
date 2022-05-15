
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "triangulation/generic/isomorphism.h"
#include "triangulation/facetpairing.h"
#include <cppunit/extensions/HelperMacros.h>

using regina::FacetPairing;
using regina::Isomorphism;

/**
 * Inherited by the test classes for all dimensions.
 */
template <int dim>
class FacetPairingTest : public CppUnit::TestFixture {
    public:
        void setUp() {
        }

        void tearDown() {
        }

        /**
         * Pre: p is in canonical form.
         */
        static void verifyIsCanonical(const FacetPairing<dim>& p) {
            if (! p.isCanonical()) {
                std::ostringstream msg;
                msg << "Pairing not identified as canonical: " << p.str();
                CPPUNIT_FAIL(msg.str());
            }
        }

        /**
         * Pre: p is in canonical form.
         */
        static void verifyMakeCanonical(const FacetPairing<dim>& p,
                const Isomorphism<dim>& iso) {
            FacetPairing<dim> alt = iso(p);
            auto [canon, canonIso] = alt.canonical();

            if (canon != p) {
                std::ostringstream msg;
                msg << "Pairing " << alt.str() << " does not canonise "
                    "to the expected " << p.str() << '.';
                CPPUNIT_FAIL(msg.str());
            }
            if (canonIso(alt) != p) {
                std::ostringstream msg;
                msg << "Pairing " << alt.str() << " does not canonise "
                    "correctly under the returned isomorphism "
                    << canonIso.str() << '.';
                CPPUNIT_FAIL(msg.str());
            }

            if (alt == p) {
                if (! alt.isCanonical()) {
                    std::ostringstream msg;
                    msg << "Pairing " << alt.str() << " canonises to itself "
                        "but is not identified as canonical.";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (alt.isCanonical()) {
                    std::ostringstream msg;
                    msg << "Pairing " << alt.str() << " canonises to "
                        "something different but is identified as canonical.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        /**
         * Pre: p is in canonical form.
         */
        static void verifyMakeCanonical(const FacetPairing<dim>& p) {
            Isomorphism<dim> iso = Isomorphism<dim>::identity(p.size());
            do {
                verifyMakeCanonical(p, iso);
                ++iso;
            } while (! iso.isIdentity());
        }

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
                    verifyIsCanonical(p);
                });
        }

        static void isCanonicalAllBounded(size_t size) {
            FacetPairing<dim>::findAllPairings(size,
                true /* boundary */, -1 /* bdry facets */,
                [](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    verifyIsCanonical(p);
                });
        }

        static void enumerateClosed(size_t size, size_t expectedCount) {
            size_t count = 0;

            FacetPairing<dim>::findAllPairings(size, false, 0,
                [&count](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    ++count;
                });

            if (count != expectedCount) {
                std::ostringstream msg;
                msg << "Facet pairing count for " << size
                    << " simplices (closed) should be " << expectedCount
                    << ", not " << count << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void enumerateBounded(size_t size, size_t expectedCount) {
            size_t count = 0;

            FacetPairing<dim>::findAllPairings(size, true, -1,
                [&count](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    ++count;
                });

            if (count != expectedCount) {
                std::ostringstream msg;
                msg << "Facet pairing count for " << size
                    << " simplices (with boundary) should be " << expectedCount
                    << ", not " << count << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void enumerateBounded(size_t size, size_t bdryFacets,
                size_t expectedCount) {
            size_t count = 0;

            FacetPairing<dim>::findAllPairings(size, true, bdryFacets,
                [&count](const FacetPairing<dim>& p,
                        typename FacetPairing<dim>::IsoList) {
                    ++count;
                });

            if (count != expectedCount) {
                std::ostringstream msg;
                msg << "Facet pairing count for " << size
                    << " simplices (" << bdryFacets
                    << " boundary facets) should be " << expectedCount
                    << ", not " << count << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }
};

