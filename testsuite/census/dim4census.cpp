
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
#include "census/dim4census.h"
#include "packet/ncontainer.h"
#include "testsuite/census/testcensus.h"

using regina::Dim4Census;
using regina::NBoolSet;
using regina::NContainer;

class Dim4CensusTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Dim4CensusTest);

    CPPUNIT_TEST(rawCounts);
    CPPUNIT_TEST(rawCountsCompact);
    CPPUNIT_TEST(rawCountsBounded);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void rawCounts() {
            unsigned nAll[] = { 1, 0, 23, 0, 8656, 0 };
            rawCountsCompare(1, 3, nAll, "closed/ideal",
                NBoolSet::sBoth, NBoolSet::sBoth, NBoolSet::sFalse, 0, 0);

            unsigned nOrientable[] = { 1, 0, 15, 0, 4150, 0 };
            rawCountsCompare(1, 3, nOrientable, "closed/ideal",
                NBoolSet::sBoth, NBoolSet::sTrue, NBoolSet::sFalse, 0, 0);
        }

        void rawCountsCompact() {
            unsigned nAll[] = { 1, 0, 10, 0 };
            rawCountsCompare(1, 3, nAll, "closed compact",
                NBoolSet::sTrue, NBoolSet::sBoth, NBoolSet::sFalse, 0, 0);

            unsigned nOrientable[] = { 1, 0, 8 };
            rawCountsCompare(1, 3, nOrientable, "closed compact orbl",
                NBoolSet::sTrue, NBoolSet::sTrue, NBoolSet::sFalse, 0, 0);
        }

        void rawCountsBounded() {
            unsigned nAll[] = { 1, 7, 51, 939, 25265 };
            rawCountsCompare(1, 2, nAll, "bounded",
                NBoolSet::sBoth, NBoolSet::sBoth, NBoolSet::sTrue, -1, 0);

            unsigned nCompact[] = { 1, 5, 38, 782 };
            rawCountsCompare(1, 2, nCompact, "bounded compact",
                NBoolSet::sTrue, NBoolSet::sBoth, NBoolSet::sTrue, -1, 0);

            unsigned nOrientable[] = { 1, 4, 27, 457 };
            rawCountsCompare(1, 2, nOrientable, "bounded compact orbl",
                NBoolSet::sTrue, NBoolSet::sTrue, NBoolSet::sTrue, -1, 0);
        }

        static void rawCountsCompare(unsigned minPent, unsigned maxPent,
                const unsigned* realAns, const char* censusType,
                NBoolSet finiteness, NBoolSet orientability,
                NBoolSet boundary, int nBdryFacets,
                Dim4Census::AcceptTriangulation sieve) {
            NContainer* census;

            for (unsigned nPent = minPent; nPent <= maxPent; nPent++) {
                census = new NContainer();
                Dim4Census::formCensus(census, nPent, finiteness, orientability,
                    boundary, nBdryFacets, sieve);

                std::ostringstream msg;
                msg << "Census count for " << nPent << " pentachora ("
                    << censusType << ") should be " << realAns[nPent]
                    << ", not " << census->getNumberOfChildren() << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    census->getNumberOfChildren() == realAns[nPent]);
                delete census;
            }
        }
};

void addDim4Census(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Dim4CensusTest::suite());
}

