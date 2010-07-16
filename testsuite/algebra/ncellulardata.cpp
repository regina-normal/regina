
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
#include "dim4/dim4triangulation.h"

#include "manifold/nlensspace.h"

#include "algebra/ncellulardata.h"
#include "algebra/nbilinearform.h"
#include "algebra/nmarkedabeliangroup.h"

#include "testsuite/utilities/testutilities.h"
#include "testsuite/testparams.h"

using regina::NLargeInteger;
using regina::NTriangulation;
using regina::Dim4Triangulation;
using regina::NExampleTriangulation;
using regina::NMarkedAbelianGroup;
using regina::NHomMarkedAbelianGroup;
using regina::NCellularData;
using regina::NBilinearForm;
using regina::NMatrixInt;
using regina::NLensSpace;

class NCellularDataTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NCellularDataTest);

    // Poincare Polynomial, Euler Characteristic
    CPPUNIT_TEST(basic_tests); 
    // verify chain complexes are really chain complexes.
    CPPUNIT_TEST(chain_complex_tests); 
    // verify chain maps are really maps of chain complexes.
    CPPUNIT_TEST(chain_maps_tests); 
    // maps between standard, dual and mixed
    CPPUNIT_TEST(coordinate_isomorphisms_tests);  
    // homology long exact sequence
    CPPUNIT_TEST(homology_LES_tests); 
    // poincare duality
    CPPUNIT_TEST(poincare_duality_tests); 
    // intersection forms
    CPPUNIT_TEST(intersectionform_tests); 
    // check lens space classification, torsion linking form
    CPPUNIT_TEST(lensspacehomotopyclassification_tests); 

    CPPUNIT_TEST_SUITE_END();

    void copyAndDelete(NTriangulation& dest, NTriangulation* source) {
            dest.insertTriangulation(*source);
            delete source; }
    void copyAndDelete(Dim4Triangulation& dest, Dim4Triangulation* source) {
            dest.insertTriangulation(*source);
            delete source; }

    private:
       NTriangulation m2_1, m2_2, m3_9, m4_52, m4_1_2, m4_4_2;
       NTriangulation n1_1, n2_1, n2_1_2, n4_14, n4_9_2, n4_1_2_1;
       NTriangulation closedHypOr, closedHypNor;
       NTriangulation weberSeifert;
       NTriangulation s1s1s1, comp1;

       Dim4Triangulation pen, knot1, knot2, knot3, knot4, cp2, PDSxI;

       std::vector< NCellularData* > m3List; // NCellularData of 3-manifolds
       std::vector< NCellularData* > m4List; // NCellularData of 4-manifolds
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

          copyAndDelete(pen,   Dim4Triangulation::fromIsoSig("baa"));
          copyAndDelete(PDSxI, Dim4Triangulation::fromIsoSig("cHkbbbRb3asb")); // Poincare dodecahedral space x interval
          copyAndDelete(knot1, Dim4Triangulation::fromIsoSig("cMkabbb+aAa3blb")); // simplest 2-knot 2-pentachoron
          copyAndDelete(knot2, Dim4Triangulation::fromIsoSig("eLMQcaccddcd1aaa2a4aaa1aca"));  // 4-pentachoron knot
          copyAndDelete(knot3, Dim4Triangulation::fromIsoSig("eLAQcbbbdddd0baa0bhahaDaDa"));  // another 
          copyAndDelete(knot4, Dim4Triangulation::fromIsoSig("gLLAQQccddeffeffaayaNaNaPbzb0aPbIaxa")); // 6-pentachoron knot
          copyAndDelete(cp2,   Dim4Triangulation::fromIsoSig("eAMMcaabccdd+aoa+aAaqbyaca")); // CP^2

	  m3List.resize(17);
	  m3List[0] = new NCellularData( m2_1 );	  m3List[1] = new NCellularData( m2_2 );
	  m3List[2] = new NCellularData( m3_9 );	  m3List[3] = new NCellularData( m4_52 );
	  m3List[4] = new NCellularData( m4_1_2 );	  m3List[5] = new NCellularData( m4_4_2 );
	  m3List[6] = new NCellularData( n1_1 );	  m3List[7] = new NCellularData( n2_1 );
	  m3List[8] = new NCellularData( n2_1_2 );	  m3List[9] = new NCellularData( n4_14 );
	  m3List[10] = new NCellularData( n4_9_2 );	  m3List[11] = new NCellularData( n4_1_2_1 );
	  m3List[12] = new NCellularData( closedHypOr );  m3List[13] = new NCellularData( closedHypNor );
	  m3List[14] = new NCellularData( weberSeifert ); m3List[15] = new NCellularData( s1s1s1 );
	  m3List[16] = new NCellularData( comp1 );

          m4List.resize(7);
          m4List[0] = new NCellularData( pen );         m4List[1] = new NCellularData( PDSxI );
          m4List[2] = new NCellularData( knot1 );       m4List[3] = new NCellularData( knot2 );
          m4List[4] = new NCellularData( knot3 );       m4List[5] = new NCellularData( knot4 );
          m4List[6] = new NCellularData( cp2 ); 

	  polyList.resize(17);
	  polyList[0] = "1+t";       polyList[1] = "1+t";        polyList[2] = "1+t";    polyList[3] = "1+t";
 	  polyList[4] = "1+2t+t^2";  polyList[5] = "1+2t+t^2";   polyList[6] = "1+t";    polyList[7] = "1+t";
	  polyList[8] = "1+t";       polyList[9] = "1+t";        polyList[10] = "1+t";   polyList[11] = "1+2t+t^2";
	  polyList[12] = "1+t^3";    polyList[13] = "1+t";       polyList[14] = "1+t^3"; polyList[15] = "1+3t+3t^2+t^3";
          polyList[16] = "1+t^3";
        }

        void tearDown() {
	 for (unsigned long i=0; i<m3List.size(); i++) delete m3List[i];
         for (unsigned long i=0; i<m4List.size(); i++) delete m4List[i];
	}

	void basic_tests() {  // euler char, poincare poly, cell counts
		for (unsigned long i=0; i<m3List.size(); i++)
		 if (m3List[i]->eulerChar() != 0) CPPUNIT_FAIL("Euler characteristic error.");
		for (unsigned long i=0; i<m3List.size(); i++)
	         if (m3List[i]->poincarePolynomial().toString() != polyList[i]) CPPUNIT_FAIL("Poincare polynomial error.");
	}

        void chain_complex_tests() {	
        	for (unsigned long i=0; i<m3List.size(); i++)
                 if (!m3List[i]->chainComplexesVerified()) CPPUNIT_FAIL("Chain complex error (3).");
         	for (unsigned long i=0; i<m4List.size(); i++)
                 if (!m4List[i]->chainComplexesVerified()) CPPUNIT_FAIL("Chain complex error (4).");
       } 

        void chain_maps_tests() {        	
                for (unsigned long i=0; i<m3List.size(); i++)
                 if (!m3List[i]->chainMapsVerified()) CPPUNIT_FAIL("Chain map error (3).");
                for (unsigned long i=0; i<m4List.size(); i++)
                 if (!m4List[i]->chainMapsVerified()) CPPUNIT_FAIL("Chain map error (4).");
        }

	void coordinate_isomorphisms_tests() { 
		if (! detailedTests()) // this is Ben's magic speed-up flag true iff
			return;        // REGINA_DETAILED_TESTS initialized and non-empty
		for (unsigned long i=0; i<m3List.size(); i++)
		 if (!m3List[i]->coordinateIsomorphismsVerified()) CPPUNIT_FAIL("Coordinate isomorphisms error (3).");
		for (unsigned long i=0; i<m4List.size(); i++)
		 if (!m4List[i]->coordinateIsomorphismsVerified()) CPPUNIT_FAIL("Coordinate isomorphisms error (4).");

	}
	void homology_LES_tests() { 
		for (unsigned long i=0; i<m3List.size(); i++)
		 if (!m3List[i]->homologyLESVerified()) CPPUNIT_FAIL("Homology LES error (3).");
		for (unsigned long i=0; i<m4List.size(); i++)
		 if (!m4List[i]->homologyLESVerified()) CPPUNIT_FAIL("Homology LES error (4).");
	}
	void poincare_duality_tests() { 
		for (unsigned long i=0; i<m3List.size(); i++)
		 if (!m3List[i]->poincareDualityVerified()) CPPUNIT_FAIL("Poincare Duality error (3).");
		for (unsigned long i=0; i<m4List.size(); i++)
		 if (!m4List[i]->poincareDualityVerified()) CPPUNIT_FAIL("Poincare Duality error (4).");
	}
	void intersectionform_tests() {
		for (unsigned long i=0; i<m3List.size(); i++)
		 if (!m3List[i]->intersectionFormsVerified()) CPPUNIT_FAIL("Intersection forms misbehaving (3).");
		for (unsigned long i=0; i<m4List.size(); i++)
		 if (!m4List[i]->intersectionFormsVerified()) CPPUNIT_FAIL("Intersection forms misbehaving (4).");
	}

        void lensspacehomotopyclassification_tests() {
         NLargeInteger maxP(22); 
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

