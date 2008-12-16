
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "census/dim4facetpairing.h"
#include "testsuite/census/testcensus.h"

using regina::Dim4FacetPairing;
using regina::Dim4FacetPairingIsoList;
using regina::NBoolSet;

/**
 * Simply increment the given count when a face pairing is found.
 */
void countFacetPairings(const Dim4FacetPairing* pair,
        const Dim4FacetPairingIsoList*, void* count) {
    if (pair)
        (*(unsigned*)count)++;
}

class Dim4FacetPairingTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Dim4FacetPairingTest);

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
            // Figures taken from sequence A129430 in the On-Line
            // Encyclopedia of Integer Sequences, as enumerated by
            // Brendan McKay using the software Nauty.
            unsigned nPairs[] = { 1, 0, 3, 0, 26, 0, 639, 0, 40264 };

            unsigned size;
            for (size = 2; size <= 5; ++size) {
                count = 0;
                Dim4FacetPairing::findAllPairings(size, NBoolSet::sFalse,
                    0, countFacetPairings, &count, false);

                std::ostringstream msg;
                msg << "Facet pairing count for " << size
                    << " pentachora should be " << nPairs[size]
                    << ", not " << count << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(), count == nPairs[size]);
            }
        }
};

void addDim4FacetPairing(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Dim4FacetPairingTest::suite());
}

