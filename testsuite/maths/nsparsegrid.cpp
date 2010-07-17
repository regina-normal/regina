
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
#include "testsuite/utilities/testutilities.h"

using regina::NLargeInteger;

class NSparseGridTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NSparseGridTest);

    CPPUNIT_TEST(multiindex_test);
    CPPUNIT_TEST(sparsegrid_test);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

	void multiindex_test() {
	 regina::NMultiIndex i(3);
         i[0]=1; i[1]=5; i[2]=3;
         regina::NMultiIndex j(i);
         if (i!=j) CPPUNIT_FAIL("Copy constructor error.");
         if (!(i==j)) CPPUNIT_FAIL("Copy constructor error (2).");
	 j[2]--;
	 if (i<j) CPPUNIT_FAIL("Inequality error.");
         if (i==j) CPPUNIT_FAIL("Equality error.");
	 i=j;
         if (i!=j) CPPUNIT_FAIL("Assignment error.");
         if (!(i==j)) CPPUNIT_FAIL("Assignment error (2).");
	};

	void sparsegrid_test() {
	 regina::NSparseGridRing< regina::NLargeInteger > g(3); regina::NMultiIndex gi(3);
	 regina::NSparseGridRing< regina::NLargeInteger > m(2); regina::NMultiIndex mi(2);
	 regina::NSparseGridRing< regina::NLargeInteger > v(1); regina::NMultiIndex vi(1);

	 std::stringstream voutss;
	 for (unsigned i=0; i<6; i++)
	  {
	   vi[0]=5*i;
	   v.setEntry( vi, NLargeInteger(1) );
	  }
	 if (v.getGrid().size() != 6) CPPUNIT_FAIL("Linear grid setEntry error.");
	 v.writeTextShort(voutss);
	 std::string vout(voutss.str());
	 if (vout != "[(0), 1], [(5), 1], [(10), 1], [(15), 1], [(20), 1], [(25), 1]")
	  CPPUNIT_FAIL("Vector output error.");

	 for (unsigned i=0; i<6; i++)
	  {
	   vi[0]=5*i;
	   v.incEntry( vi, NLargeInteger(-1) );
	  }
	 if (v.getGrid().size() != 0) CPPUNIT_FAIL("Linear grid incEntry error.");

         for (unsigned i=0; i<7; i++)
	  {
	   mi[0]=2*i % 5; mi[1]=3*i % 11; 
           m.incEntry( mi, NLargeInteger(i + 1) * (i%2==0 ? 1 : -1)  );
          }
        std::stringstream moutss;
	m.writeTextShort(moutss);
        std::string mout(moutss.str());
        if (mout != "[(0, 0), 1], [(0, 4), -6], [(1, 9), -4], [(2, 3), -2], [(2, 7), 7], [(3, 1), 5], [(4, 6), 3]")
	 CPPUNIT_FAIL("Matrix output error.");
        for (unsigned i=0; i<4; i++)
	  {
	   mi[0]=2*i % 5; mi[1]=3*i % 11; 
           m.incEntry( mi, NLargeInteger(i + 1) * (i%2==0 ? -1 : 1)  );
          }
        if (m.getGrid().size() != 3) CPPUNIT_FAIL("Matrix grid incEntry error.");
	// now 3-grid...
	for (unsigned i=0; i<30; i++)
	 {
	  gi[0]=i % 2; gi[1]=i % 3; gi[2]=i % 5;
	  g.setEntry( gi, NLargeInteger( i%3==0 ? -1 : 1 ) );
	 }
	std::stringstream goutss;
	g.writeTextShort(goutss);
	std::string gout(goutss.str());
	if (gout != "[(0, 0, 0), -1], [(0, 0, 1), -1], [(0, 0, 2), -1], [(0, 0, 3), -1], [(0, 0, 4), -1], [(0, 1, 0), 1], [(0, 1, 1), 1], [(0, 1, 2), 1], [(0, 1, 3), 1], [(0, 1, 4), 1], [(0, 2, 0), 1], [(0, 2, 1), 1], [(0, 2, 2), 1], [(0, 2, 3), 1], [(0, 2, 4), 1], [(1, 0, 0), -1], [(1, 0, 1), -1], [(1, 0, 2), -1], [(1, 0, 3), -1], [(1, 0, 4), -1], [(1, 1, 0), 1], [(1, 1, 1), 1], [(1, 1, 2), 1], [(1, 1, 3), 1], [(1, 1, 4), 1], [(1, 2, 0), 1], [(1, 2, 1), 1], [(1, 2, 2), 1], [(1, 2, 3), 1], [(1, 2, 4), 1]")
	 CPPUNIT_FAIL("3-grid setEntry error.");
        }
};

void addNSparseGrid(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NSparseGridTest::suite());
}

