
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

#include "manifold/nlensspace.h"

#include "algebra/ncellulardata.h"
#include "algebra/nbilinearform.h"
#include "algebra/nmarkedabeliangroup.h"

#include "testsuite/utilities/testutilities.h"

using regina::NLargeInteger;
using regina::NTriangulation;
using regina::NExampleTriangulation;
using regina::NMarkedAbelianGroup;
using regina::NHomMarkedAbelianGroup;
using regina::NCellularData;
using regina::NBilinearForm;
using regina::NMatrixInt;
using regina::NLensSpace;

class NCellularDataTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NCellularDataTest);

    CPPUNIT_TEST(basic_tests);
    CPPUNIT_TEST(coordinate_isomorphisms_tests); 
    CPPUNIT_TEST(homology_LES_tests);
    CPPUNIT_TEST(poincare_duality_tests); 
    CPPUNIT_TEST(intersectionform_tests);
    CPPUNIT_TEST(lensspacehomotopyclassification_tests);

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

        void lensspacehomotopyclassification_tests() {
         NLargeInteger maxP(22); maxP.seedRandomGenerator();
         // produce some random lens spaces, compute intersection form and check it is [\pm r^2 q/p] \in Q/Z
         // p will be anywhere from 2 to 19, q will be chosen to be coprime, how? just choose randomly 0 < q < p, 
         // until you find something coprime, I suppose. 
         maxP = maxP - 2;
         bool testpassed(true);
         std::ostringstream msg;
         NTriangulation* lens(NULL);
         const NBilinearForm* tif(NULL);
         for (unsigned long i=0; i<8; i++) // 3 random lens spaces to test.
          { 
           NLargeInteger p( maxP.randomBoundedByThis() + 2 ); // should be at least 2
           NLargeInteger q( p.randomBoundedByThis() );
           while ( q.gcd(p) != 1 ) q = p.randomBoundedByThis();
           lens = new NTriangulation(*NLensSpace(p.longValue(),q.longValue()).construct());
           // we have our random triangulation, lets test it. 
           NCellularData ncd( *lens );
           NCellularData::GroupLocator h1L( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 );
           NCellularData::FormLocator tifL( NCellularData::torsionlinkingForm, h1L, h1L );
           tif = new NBilinearForm( *ncd.bilinearForm(tifL) );
           // have the form, if it evaluates to a/p we need to make sure a/p = +- r^2 q/p mod 1 for some r == 1,2,...,p-1
           bool itestpassed(false);
           std::vector<NLargeInteger> idV(1, NLargeInteger::one);
           std::vector<NLargeInteger> eval(tif->evalCC(idV, idV));
           NLargeInteger A(eval[0]); unsigned long P( p.longValue() );
          for (unsigned long r=1; r < P; r++)
            {
             NLargeInteger temp(q); 
             temp *= (r*r); 
             if ( (temp - A) % p == 0 ) itestpassed=true;
             if ( (temp + A) % p == 0 ) itestpassed=true; 
            }
           // if failed, append a message for output
           if (itestpassed==false) { msg << "L("<<p<<","<<q<<") "; testpassed=false; }
           // tidy up.
           delete tif;
           delete lens;
          } // for i loop
         if (!testpassed) CPPUNIT_FAIL("Lens space(s): "+msg.str()+"failed TLF test.");
	} // lensspacehomotopyclassification_tests()

};

void addNCellularData(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NCellularDataTest::suite());
}

