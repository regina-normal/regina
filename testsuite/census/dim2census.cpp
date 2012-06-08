
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "census/dim2census.h"
#include "packet/ncontainer.h"
#include "testsuite/census/testcensus.h"

using regina::NBoolSet;
using regina::Dim2Census;
using regina::NContainer;

class Dim2CensusTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Dim2CensusTest);

    CPPUNIT_TEST(rawCountsClosed);
    CPPUNIT_TEST(rawCountsBounded);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void rawCountsClosed() {
            unsigned nAll[] = { 1, 0, 7, 0, 51, 0, 738, 0, 20540, 0, 911677 };
            rawCountsCompare(1, 8, nAll, "closed",
                NBoolSet::sBoth, NBoolSet::sFalse, 0, 0);

            unsigned nOrientable[] = { 1, 0, 3, 0, 11, 0, 73, 0, 838, 0, 15840};
            rawCountsCompare(1, 10, nOrientable, "closed orbl",
                NBoolSet::sTrue, NBoolSet::sFalse, 0, 0);
        }

        void rawCountsBounded() {
            unsigned nAll[] = { 1, 3, 6, 26, 105, 622, 3589, 28031, 202169 };
            rawCountsCompare(1, 7, nAll, "bounded",
                NBoolSet::sBoth, NBoolSet::sTrue, -1, 0);

            unsigned nOrientable[] =
                { 1, 2, 4, 11, 41, 155, 750, 3967, 23260, 148885, 992299 };
            rawCountsCompare(1, 8, nOrientable, "bounded orbl",
                NBoolSet::sTrue, NBoolSet::sTrue, -1, 0);
        }

        static void rawCountsCompare(unsigned minTris, unsigned maxTris,
                const unsigned* realAns, const char* censusType,
                NBoolSet orientability, NBoolSet boundary, int nBdryFaces,
                Dim2Census::AcceptTriangulation sieve) {
            NContainer* census;

            for (unsigned nTris = minTris; nTris <= maxTris; nTris++) {
                census = new NContainer();
                Dim2Census::formCensus(census, nTris, orientability,
                    boundary, nBdryFaces, sieve);

                std::ostringstream msg;
                msg << "Census count for " << nTris << " tetrahedra ("
                    << censusType << ") should be " << realAns[nTris]
                    << ", not " << census->getNumberOfChildren() << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    census->getNumberOfChildren() == realAns[nTris]);
                delete census;
            }
        }
};

void addDim2Census(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Dim2CensusTest::suite());
}

