
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "census/nfacepairing.h"
#include "testsuite/census/testcensus.h"

using regina::NFacePairing;
using regina::NFacePairingIsoList;
using regina::NBoolSet;

/**
 * Simply increment the given count when a face pairing is found.
 */
void countFacePairings(const NFacePairing* pair, const NFacePairingIsoList*,
        void* count) {
    if (pair)
        (*(unsigned*)count)++;
}

class NFacePairingTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NFacePairingTest);

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
            unsigned nPairs[] = { 1, 1, 2, 4, 10, 28, 97, 359 };

            unsigned nTets;
            for (nTets = 1; nTets <= 7; nTets++) {
                count = 0;
                NFacePairing::findAllPairings(nTets, NBoolSet::sFalse,
                    0, countFacePairings, &count, false);
                
                std::ostringstream msg;
                msg << "Face pairing count for " << nTets
                    << " tetrahedra should be " << nPairs[nTets]
                    << ", not " << count << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(), count == nPairs[nTets]);
            }
        }
};

void addNFacePairing(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NFacePairingTest::suite());
}

