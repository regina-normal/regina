
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
#include "maths/nmatrixint.h"

#include "triangulation/nexampletriangulation.h"
#include "triangulation/ntriangulation.h"

#include "algebra/ncellulardata.h"
#include "algebra/nmarkedabeliangroup.h"

#include "testsuite/utilities/testutilities.h"

using regina::NLargeInteger;
using regina::NTriangulation;
using regina::NExampleTriangulation;
using regina::NMarkedAbelianGroup;
using regina::NHomMarkedAbelianGroup;
using regina::NCellularData;
using regina::NMatrixInt;

class NCellularDataTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NCellularDataTest);

    CPPUNIT_TEST(basic_tests);
    CPPUNIT_TEST(coordinate_isomorphisms_tests); 
    CPPUNIT_TEST(homology_LES_tests);
    CPPUNIT_TEST(poincare_duality_tests); 
    CPPUNIT_TEST(intersectionform_tests);

    CPPUNIT_TEST_SUITE_END();

    void copyAndDelete(NTriangulation& dest, NTriangulation* source) {
            dest.insertTriangulation(*source);
            delete source; }

    private:
       NTriangulation m2_1, m2_2, m3_9, m4_52, m4_1_2, m4_4_2;
       NTriangulation n1_1, n2_1, n2_1_2, n4_14, n4_9_2, n4_1_2_1;
       NTriangulation closedHypOr, closedHypNor;
       NTriangulation weberSeifert;
       NTriangulation s1s1s1, comp1;

       std::vector< NCellularData* > cdList;
       std::vector< std::string > polyList;

    public:
        void setUp() {     
	  // todo: once we have dim4Triangulation dehydration strings we can add some 4-dimensional triangulations.
          m2_1.insertRehydration("cabbbbaei");          m2_2.insertRehydration("cabbbbapt");
          m3_9.insertRehydration("dagacccfwkn");        m4_52.insertRehydration("ebdbcdddaqhie");
          m4_1_2.insertRehydration("eahbcdddhsssj");    m4_4_2.insertRehydration("ebdbcdddddddx");
          n1_1.insertRehydration("baaaade");            n2_1.insertRehydration("cabbbbabw");
          n2_1_2.insertRehydration("cabbbbcdw");        n4_14.insertRehydration("eahdccddakfhq");
          n4_9_2.insertRehydration("ebdbcdddcemre");    n4_1_2_1.insertRehydration("eahbcdddjxxxj");
          copyAndDelete(closedHypOr,            NExampleTriangulation::smallClosedOrblHyperbolic());
          copyAndDelete(closedHypNor,           NExampleTriangulation::smallClosedNonOrblHyperbolic());
          copyAndDelete(weberSeifert,           NExampleTriangulation::weberSeifert());
	  s1s1s1.insertRehydration("gepaadcefeffnkkanax");
	  comp1.insertRehydration("jgofiaaaceedfhiiifkxkfnbtxe");

	  cdList.resize(17);
	  cdList[0] = new NCellularData( m2_1 );	  cdList[1] = new NCellularData( m2_2 );
	  cdList[2] = new NCellularData( m3_9 );	  cdList[3] = new NCellularData( m4_52 );
	  cdList[4] = new NCellularData( m4_1_2 );	  cdList[5] = new NCellularData( m4_4_2 );
	  cdList[6] = new NCellularData( n1_1 );	  cdList[7] = new NCellularData( n2_1 );
	  cdList[8] = new NCellularData( n2_1_2 );	  cdList[9] = new NCellularData( n4_14 );
	  cdList[10] = new NCellularData( n4_9_2 );	  cdList[11] = new NCellularData( n4_1_2_1 );
	  cdList[12] = new NCellularData( closedHypOr );  cdList[13] = new NCellularData( closedHypNor );
	  cdList[14] = new NCellularData( weberSeifert ); cdList[15] = new NCellularData( s1s1s1 );
	  cdList[16] = new NCellularData( comp1 );

	  polyList.resize(17);
	  polyList[0] = "1+t";       polyList[1] = "1+t";        polyList[2] = "1+t";    polyList[3] = "1+t";
 	  polyList[4] = "1+2t+t^2";  polyList[5] = "1+2t+t^2";   polyList[6] = "1+t";    polyList[7] = "1+t";
	  polyList[8] = "1+t";       polyList[9] = "1+t";        polyList[10] = "1+t";   polyList[11] = "1+2t+t^2";
	  polyList[12] = "1+t^3";    polyList[13] = "1+t";       polyList[14] = "1+t^3"; polyList[15] = "1+3t+3t^2+t^3";
          polyList[16] = "1+t^3";
        }

        void tearDown() {
	 for (unsigned long i=0; i<cdList.size(); i++) delete cdList[i];
	}

	void basic_tests() {  // euler char, poincare poly, cell counts
		for (unsigned long i=0; i<cdList.size(); i++)
		 if (cdList[i]->eulerChar() != 0) CPPUNIT_FAIL("Euler characteristic error.");
		for (unsigned long i=0; i<cdList.size(); i++)
	         if (cdList[i]->poincarePolynomial().toString() != polyList[i]) CPPUNIT_FAIL("Poincare polynomial error.");
	}

	void coordinate_isomorphisms_tests() { 
		for (unsigned long i=0; i<cdList.size(); i++)
		 if (!cdList[i]->coordinateIsomorphismsVerified()) CPPUNIT_FAIL("Coordinate isomorphisms error.");
	}
	void homology_LES_tests() { 
		for (unsigned long i=0; i<cdList.size(); i++)
		 if (!cdList[i]->homologyLESVerified()) CPPUNIT_FAIL("Homology LES error.");
	}
	void poincare_duality_tests() { 
		for (unsigned long i=0; i<cdList.size(); i++)
		 if (!cdList[i]->poincareDualityVerified()) CPPUNIT_FAIL("Poincare Duality error.");
	}
	void intersectionform_tests() {
		for (unsigned long i=0; i<cdList.size(); i++)
		 if (!cdList[i]->intersectionFormsVerified()) CPPUNIT_FAIL("Intersection forms misbehaving.");
	}

};

void addNCellularData(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NCellularDataTest::suite());
}

