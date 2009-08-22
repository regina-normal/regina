
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
#include "maths/nsparsegrid.h"
#include "maths/nlargeinteger.h"
#include "algebra/nmarkedabeliangroup.h"
#include "testsuite/utilities/testutilities.h"

using regina::NLargeInteger;
//using regina::Dim4Triangulation;
//using regina::Dim4ExampleTriangulation;

// todo: build a pre-set collection of test presentations. Maybe get them from the example triangulations?
//       or both...

class NCellularDataTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NCellularDataTest);

    CPPUNIT_TEST(basic_tests);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {}
        void tearDown() {}

	void basic_tests() { // constructors, isIsomorphicTo, isEqualTo, isChainComplex, getRank,
				   // getTorsionRank, getNumberOfInvariantFactors, minNumberOfGenerators,
				   // getInvariantFactor, isTrivial, minNumberCycleGens, writeTextShort,
				   // cycleGen, getRankCC, getM, getN, coefficients
	}

};

void addNCellularData(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NCellularDataTest::suite());
}

