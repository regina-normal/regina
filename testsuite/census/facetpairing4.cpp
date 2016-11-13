
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
#include "triangulation/dim4.h"
#include "triangulation/facetpairing.h"
#include "testsuite/census/testcensus.h"

using regina::FacetPairing;
using regina::BoolSet;

/**
 * Simply increment the given count when a face pairing is found.
 */
void countFacetPairings(const FacetPairing<4>* pair,
        const FacetPairing<4>::IsoList*, void* count) {
    if (pair)
        (*(unsigned*)count)++;
}

class FacetPairing4Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(FacetPairing4Test);

    CPPUNIT_TEST(rawCountsClosed);
    CPPUNIT_TEST(rawCountsBounded);

    CPPUNIT_TEST_SUITE_END();

    private:
        unsigned count;
            /**< Used to hold arbitrary totals. */

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void rawCountsClosed() {
            // Figures taken from sequence A129430 in the On-Line
            // Encyclopedia of Integer Sequences, as enumerated by
            // Brendan McKay using the software Nauty.
            unsigned nPairs[] = { 0, 0, 3, 0, 26, 0, 639, 0, 40264 };

            unsigned size;
            for (size = 0; size <= 5; ++size) {
                count = 0;
                FacetPairing<4>::findAllPairings(size, BoolSet::sFalse,
                    0, countFacetPairings, &count);

                if (count != nPairs[size]) {
                    std::ostringstream msg;
                    msg << "Facet pairing count for " << size
                        << " pentachora (closed) should be "
                        << nPairs[size] << ", not " << count << '.';

                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void rawCountsBounded() {
            // Figures based on enumeration under the 4-manifolds branch
            // at the time of the Regina 4.93 release.
            unsigned nBdry[] = { 0, 3, 11, 61, 473, 4487 };
            unsigned nBdry1[] = { 0, 1, 0, 10, 0, 284, 0, 17761 };
            unsigned nBdry2[] = { 0, 0, 4, 0, 91, 0, 4665 };

            unsigned size;

            for (size = 0; size <= 6; ++size) {
                count = 0;
                FacetPairing<4>::findAllPairings(size, BoolSet::sTrue,
                    1, countFacetPairings, &count);

                if (count != nBdry1[size]) {
                    std::ostringstream msg;
                    msg << "Facet pairing count for " << size
                        << " pentachora (1 bdry face) should be "
                        << nBdry1[size] << ", not " << count << '.';

                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (size = 0; size <= 5; ++size) {
                count = 0;
                FacetPairing<4>::findAllPairings(size, BoolSet::sTrue,
                    2, countFacetPairings, &count);

                if (count != nBdry2[size]) {
                    std::ostringstream msg;
                    msg << "Facet pairing count for " << size
                        << " pentachora (2 bdry faces) should be "
                        << nBdry2[size] << ", not " << count << '.';

                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (size = 0; size <= 4; ++size) {
                count = 0;
                FacetPairing<4>::findAllPairings(size, BoolSet::sTrue,
                    -1, countFacetPairings, &count);

                if (count != nBdry[size]) {
                    std::ostringstream msg;
                    msg << "Facet pairing count for " << size
                        << " pentachora (closed) should be "
                        << nBdry[size] << ", not " << count << '.';

                    CPPUNIT_FAIL(msg.str());
                }
            }
        }
};

void addFacetPairing4(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(FacetPairing4Test::suite());
}

