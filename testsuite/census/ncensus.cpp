
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
#include "census/ncensus.h"
#include "packet/ncontainer.h"
#include "testsuite/census/testcensus.h"

using regina::AcceptTriangulation;
using regina::NBoolSet;
using regina::NCensus;
using regina::NContainer;

class NCensusTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NCensusTest);

    CPPUNIT_TEST(rawCounts);
    CPPUNIT_TEST(rawCountsPrimeMinimalOr);
    CPPUNIT_TEST(rawCountsPrimeMinimalNor);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void rawCounts() {
            unsigned nAll[] = { 1, 5, 61, 1581 };
            rawCountsCompare(1, 3, nAll, "closed",
                NBoolSet::sBoth, NBoolSet::sBoth, NBoolSet::sFalse, 0, 0, 0);

            unsigned nOrientable[] = { 1, 4, 35, 454, 13776 };
            rawCountsCompare(1, 3, nOrientable, "closed orbl",
                NBoolSet::sBoth, NBoolSet::sTrue, NBoolSet::sFalse, 0, 0, 0);
        }

        void rawCountsPrimeMinimalOr() {
            unsigned nOrientable[] = { 1, 4, 12, 7, 17, 50 };
            rawCountsCompare(1, 4, nOrientable, "closed orbl prime minimal",
                NBoolSet::sTrue, NBoolSet::sTrue, NBoolSet::sFalse, 0,
                NCensus::PURGE_NON_MINIMAL_PRIME, NCensus::mightBeMinimal);
        }

        void rawCountsPrimeMinimalNor() {
            unsigned nNonOrientable[] = { 0, 0, 1, 0, 2, 4 };
            rawCountsCompare(1, 4, nNonOrientable,
                "closed non-orbl prime minimal P2-irreducible",
                NBoolSet::sTrue, NBoolSet::sFalse, NBoolSet::sFalse, 0,
                NCensus::PURGE_NON_MINIMAL_PRIME |
                NCensus::PURGE_P2_REDUCIBLE, NCensus::mightBeMinimal);
        }

        static void rawCountsCompare(unsigned minTets, unsigned maxTets,
                const unsigned* realAns, const char* censusType,
                NBoolSet finiteness, NBoolSet orientability,
                NBoolSet boundary, int nBdryFaces, int whichPurge,
                AcceptTriangulation sieve) {
            NContainer* census;

            for (unsigned nTets = minTets; nTets <= maxTets; nTets++) {
                census = new NContainer();
                NCensus::formCensus(census, nTets, finiteness, orientability,
                    boundary, nBdryFaces, whichPurge, sieve);

                std::ostringstream msg;
                msg << "Census count for " << nTets << " tetrahedra ("
                    << censusType << ") should be " << realAns[nTets]
                    << ", not " << census->getNumberOfChildren() << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    census->getNumberOfChildren() == realAns[nTets]);
                delete census;
            }
        }
};

void addNCensus(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NCensusTest::suite());
}

