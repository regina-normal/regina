
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "triangulation/dim2.h"
#include "triangulation/facetpairing.h"
#include "testsuite/census/testcensus.h"

using regina::FacetPairing;
using regina::BoolSet;

/**
 * Simply increment the given count when a face pairing is found.
 */
void countEdgePairings(const FacetPairing<2>* pair,
        const FacetPairing<2>::IsoList*, void* count) {
    if (pair)
        (*(unsigned*)count)++;
}

class FacetPairing2Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(FacetPairing2Test);

    CPPUNIT_TEST(rawCounts);

    CPPUNIT_TEST_SUITE_END();

    private:
        unsigned count;
            /**< Used to hold arbitrary totals. */

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void rawCounts() {
            // Figures taken from the online encyclopedia of integer
            // sequences, #A005967.
            unsigned nPairs[] = { 0, 0, 2, 0, 5, 0, 17, 0, 71, 0, 388,
                0, 2592 };

            unsigned nTri;
            for (nTri = 0; nTri <= 12; nTri++) {
                count = 0;
                FacetPairing<2>::findAllPairings(nTri, BoolSet::sFalse,
                    0, countEdgePairings, &count);

                std::ostringstream msg;
                msg << "Edge pairing count for " << nTri
                    << " triangles should be " << nPairs[nTri]
                    << ", not " << count << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(), count == nPairs[nTri]);
            }
        }
};

void addFacetPairing2(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(FacetPairing2Test::suite());
}

