
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

using regina::NTriangulation;
using regina::NExampleTriangulation;
using regina::NMarkedAbelianGroup;
using regina::NHomMarkedAbelianGroup;
using regina::NCellularData;
using regina::NMatrixInt;
using regina::NLargeInteger;

// todo: at present I'm using NTriangulation and NCellularData to construct chain complexes
//       perhaps later we should avoid using these classes, and have some more direct constructions
//       of useful chain complexes for the purposes of testing NMarkedAbelianGroup and NHomMarkedAbelianGroup.

class NMarkedAbelianGroupTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NMarkedAbelianGroupTest);

    CPPUNIT_TEST(basic_tests);
    CPPUNIT_TEST(element_representation_tests);
    CPPUNIT_TEST(boundary_map_tests);

    CPPUNIT_TEST(hom_basic_tests);
    CPPUNIT_TEST(hom_induced_objects);
    CPPUNIT_TEST(hom_detailed_tests);

    CPPUNIT_TEST_SUITE_END();

    void copyAndDelete(NTriangulation& dest, NTriangulation* source) {
            dest.insertTriangulation(*source);
            delete source; }

    private:
	/* An easy source of chain complexes is standard triangulations via
	 * NCellularData. Maybe we'll cook up a different source some other time? */
        NTriangulation m2_1, m2_2, m3_9, m4_52, m4_1_2, m4_4_2;
        NTriangulation n1_1, n2_1, n2_1_2, n4_14, n4_9_2, n4_1_2_1;
        NTriangulation closedHypOr, closedHypNor;
        NTriangulation weberSeifert;
	NTriangulation s1s1s1, comp1;

	std::vector< NCellularData* > cdList;
        std::vector< std::string > h1List;

	std::vector< std::string > ker_mtr;
	std::vector< std::string > coker_mtr;


    public:
        void setUp() {
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

	  cdList.resize(10);
	  cdList[0] = new NCellularData( m2_1 );	  cdList[1] = new NCellularData( m2_2 );
	  cdList[2] = new NCellularData( m3_9 );	  cdList[3] = new NCellularData( m4_52 );
	  cdList[4] = new NCellularData( m4_1_2 );	  cdList[5] = new NCellularData( m4_4_2 );
	  cdList[6] = new NCellularData( n1_1 );	  cdList[7] = new NCellularData( n2_1 );
	  cdList[8] = new NCellularData( n2_1_2 );	  cdList[9] = new NCellularData( n4_14 );
//	  cdList[10] = new NCellularData( n4_9_2 );	  cdList[11] = new NCellularData( n4_1_2_1 );
//	  cdList[12] = new NCellularData( closedHypOr );  cdList[13] = new NCellularData( closedHypNor );
//	  cdList[14] = new NCellularData( weberSeifert ); cdList[15] = new NCellularData( s1s1s1 );
//	  cdList[16] = new NCellularData( comp1 );

          h1List.resize(17);
          h1List[0] = "Z";	     h1List[1] = "Z + Z_5";  h1List[2] = "Z";         h1List[3] = "Z + 2 Z_3";
          h1List[4] = "2 Z";         h1List[5] = "2 Z";      h1List[6] = "Z";         h1List[7] = "Z + Z_2";
          h1List[8] = "Z + Z_2";     h1List[9] = "Z";        //h1List[10] = "Z + Z_2";  h1List[11] = "2 Z + Z_2";
//          h1List[12] = "2 Z_5";	     h1List[13] = "Z";       h1List[14] = "3 Z_5";    h1List[15] = "3 Z";
//	  h1List[16] = "Z_4 + Z_20";

	  ker_mtr.resize(17);
	  ker_mtr[0] = "Z";          ker_mtr[1] = "Z";       ker_mtr[2] = "Z";        ker_mtr[3] = "Z";
 	  ker_mtr[4] = "2 Z";        ker_mtr[5] = "2 Z";     ker_mtr[6] = "Z";        ker_mtr[7] = "Z";
	  ker_mtr[8] = "Z + Z_2";    ker_mtr[9] = "Z";       //ker_mtr[10] = "Z + Z_2"; ker_mtr[11] = "2 Z + Z_2";
//	  ker_mtr[12] = "0";         ker_mtr[13] = "0";      ker_mtr[14] = "0";       ker_mtr[15] = "0";
//         ker_mtr[16] = "0";

	  coker_mtr.resize(17);
	  coker_mtr[0] = "0";          coker_mtr[1] = "Z_5";     coker_mtr[2] = "0";        coker_mtr[3] = "2 Z_3";
 	  coker_mtr[4] = "0";          coker_mtr[5] = "0";       coker_mtr[6] = "0";        coker_mtr[7] = "Z_2";
	  coker_mtr[8] = "0";          coker_mtr[9] = "Z_9";     //coker_mtr[10] = "0";       coker_mtr[11] = "0";
//	  coker_mtr[12] = "2 Z_5";     coker_mtr[13] = "Z";      coker_mtr[14] = "3 Z_5";   coker_mtr[15] = "3 Z";
//          coker_mtr[16] = "Z_4 + Z_20";

        }

        void tearDown() {
	 for (unsigned long i=0; i<cdList.size(); i++) delete cdList[i];	
	}

	void basic_tests() { 
	 const regina::NMarkedAbelianGroup* test(NULL);
         for (unsigned long i=0; i<cdList.size(); i++)
	  {
	   // precomputed isomorphism tests
           std::stringstream temp;
	   test = cdList[i]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) );
	   test->writeTextShort(temp);
	   if (temp.str() != h1List[i]) CPPUNIT_FAIL("Homology computation error.");
	   temp.str("");
           cdList[i]->unmarkedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) 
            )->writeTextShort(temp);
	   if (h1List[i] != temp.str() ) CPPUNIT_FAIL("Homology computation error (2).");
	  }
	 // relative isomorphism tests, positive results
	 if (!(cdList[0]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->isIsomorphicTo( 
	      (*cdList[9]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) ) ) ) )
	    ) CPPUNIT_FAIL("isIsomorphicTo error.");
	 if (!(cdList[2]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->isIsomorphicTo( 
	      (*cdList[6]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) ) ) ) )
	    ) CPPUNIT_FAIL("isIsomorphicTo error (2).");
	 if (!(cdList[0]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->isIsomorphicTo( 
	      (*cdList[2]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) ) ) ) )
	    ) CPPUNIT_FAIL("isIsomorphicTo error (3).");
//	 if (!(cdList[6]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->isIsomorphicTo( 
//	      (*cdList[13]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) ) ) ) )
//	    ) CPPUNIT_FAIL("isIsomorphicTo error (4).");
	 // relative isomorphism tests, negative results
	 if ((cdList[0]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->isIsomorphicTo( 
	      (*cdList[1]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) ) ) ) )
	    ) CPPUNIT_FAIL("!isIsomorphicTo error.");
//	 if ((cdList[1]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->isIsomorphicTo( 
//	      (*cdList[14]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) ) ) ) )
//	    ) CPPUNIT_FAIL("!isIsomorphicTo error (2).");
//	 if ((cdList[14]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->isIsomorphicTo( 
//	      (*cdList[15]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) ) ) ) )
//	    ) CPPUNIT_FAIL("!isIsomorphicTo error (3).");
//	 if ((cdList[15]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->isIsomorphicTo( 
//	      (*cdList[16]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) ) ) ) )
//	    ) CPPUNIT_FAIL("!isIsomorphicTo error (4).");
	 // identity test, negative results
	 if ((cdList[0]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::MIX_coord, 0 ) )->equalTo( 
	      (*cdList[9]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::MIX_coord, 0 ) ) ) ) )
	    ) CPPUNIT_FAIL("equalTo error.");
	 if ((cdList[2]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::MIX_coord, 0 ) )->equalTo( 
	      (*cdList[6]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::MIX_coord, 0 ) ) ) ) )
	    ) CPPUNIT_FAIL("equalTo error (2).");
	 if ((cdList[0]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::STD_coord, 0 ) )->equalTo( 
	      (*cdList[2]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::STD_coord, 0 ) ) ) ) )
	    ) CPPUNIT_FAIL("equalTo error (3).");
//	 if ((cdList[6]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::STD_coord, 0 ) )->equalTo( 
//	      (*cdList[13]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::STD_coord, 0 ) ) ) ) )
//	    ) CPPUNIT_FAIL("equalTo error (4).");
	 // CC tests, positive results
	 for (unsigned long i=0; i<cdList.size(); i++)
	  {
	  if (!cdList[i]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::STD_coord, 0 ) )->isChainComplex())
		CPPUNIT_FAIL("CC error.");
	  if (!cdList[i]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->isChainComplex())
		CPPUNIT_FAIL("CC error (2).");
	  if (cdList[i]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->getRank() !=
              cdList[i]->unmarkedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->getRank() )
		CPPUNIT_FAIL("getRank() error.");
	  if (cdList[i]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->
		getNumberOfInvariantFactors() !=
              cdList[i]->unmarkedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->
		getNumberOfInvariantFactors() )
		CPPUNIT_FAIL("getNumberOfInvariantFactors() error.");
	  if (cdList[i]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) )->isTrivial() )
		CPPUNIT_FAIL("isTrivial() error.");
	  }
	// add some coefficients tests...
	}

	void element_representation_tests() { // compatibility of ccRep and snfRep, getFreeRep, getTorsionRep, 
		const regina::NMarkedAbelianGroup* test(NULL);
	        // Z coeff
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  test = cdList[i]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) );
                  for (unsigned long j=0; j<test->minNumberOfGenerators(); j++)
		   {
	            std::vector< NLargeInteger > ccVec( test->ccRep(j) );
		    std::vector< NLargeInteger > snfVec( test->snfRep( ccVec ) );
		    for (unsigned long k=0; k<snfVec.size(); k++)
			{
			if (snfVec[k] != ( k==j ? NLargeInteger::one : NLargeInteger::zero ) )
				CPPUNIT_FAIL("snfRep(ccRep[j]) != ej error.");
	  		}
		   }
		 }
		// Z_4 coeff
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  test = cdList[i]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 4 ) );
                  for (unsigned long j=0; j<test->minNumberOfGenerators(); j++)
		   {
	            std::vector< NLargeInteger > ccVec( test->ccRep(j) );
		    std::vector< NLargeInteger > snfVec( test->snfRep( ccVec ) );
		    for (unsigned long k=0; k<snfVec.size(); k++)
			{
			if (snfVec[k] != ( k==j ? NLargeInteger::one : NLargeInteger::zero ) )
				CPPUNIT_FAIL("snfRep(ccRep[j]) != ej error, Z_4 coeff.");
	  		}
		   }
		 }
		// Z_10 coeff
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  test = cdList[i]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 10 ) );
                  for (unsigned long j=0; j<test->minNumberOfGenerators(); j++)
		   {
	            std::vector< NLargeInteger > ccVec( test->ccRep(j) );
		    std::vector< NLargeInteger > snfVec( test->snfRep( ccVec ) );
		    for (unsigned long k=0; k<snfVec.size(); k++)
			{
			if (snfVec[k] != ( k==j ? NLargeInteger::one : NLargeInteger::zero ) )
				CPPUNIT_FAIL("snfRep(ccRep[j]) != ej error, Z_10 coeff.");
	  		}
		   }
		 }

	}

	void boundary_map_tests() { // isCycle, boundaryMap, writeAsBoundary, etc...
		const regina::NMarkedAbelianGroup* test(NULL);
	        // Z coeff
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  test = cdList[i]->markedGroup( NCellularData::GroupLocator( 1, NCellularData::coVariant, NCellularData::DUAL_coord, 0 ) );
                  for (unsigned long j=0; j<test->minNumberCycleGens(); j++)
		   {
	            std::vector< NLargeInteger > ccVec( test->cycleGen(j) );
		    if (!test->isCycle( ccVec )) CPPUNIT_FAIL("cycleGens - isCycle() error.");
		   }
                  for (unsigned long j=0; j<test->getNumberOfInvariantFactors(); j++)
		   {
	            std::vector< NLargeInteger > ccVec( test->getTorsionRep(j) );
		    if (!test->isCycle( ccVec )) CPPUNIT_FAIL("torsionRep - isCycle() error.");
		    if (test->isBoundary( ccVec )) CPPUNIT_FAIL("torsionRep - isBoundary() error.");
		    std::vector< NLargeInteger > bVec( test->boundaryMap( ccVec ) );
		    for (unsigned long k=0; k<bVec.size(); k++) if (bVec[k] != NLargeInteger::zero)
			CPPUNIT_FAIL("boundaryMap != 0 error.");
		    for (unsigned long k=0; k<ccVec.size(); k++) ccVec[k] *= test->getInvariantFactor(j);
		    if (!test->isBoundary(ccVec)) CPPUNIT_FAIL("I.F.(j)*ccVec !isBoundary error.");
	            const NMatrixInt N( test->getN() );
		    bVec = test->writeAsBoundary(ccVec);
		    NLargeInteger ck;
	            for (unsigned long k=0; k<ccVec.size(); k++)
			{
			 ck = NLargeInteger::zero;
			 for (unsigned long m=0; m<N.columns(); m++)
				ck += N.entry(k,m) * bVec[m];
			 if (ck != ccVec[k]) CPPUNIT_FAIL("writeAsBoundary() error.");
			}
		   }

		 }

	}

	// NHomMarkedAbelianGroup tests.
	void hom_basic_tests() { // isCycleMap, isEpic, isMonic, isIsomorphism, isZero.
		const regina::NHomMarkedAbelianGroup* test(NULL);
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  NCellularData::GroupLocator ga( 1, NCellularData::coVariant, NCellularData::STD_coord, 0 );
		  NCellularData::GroupLocator gb( 1, NCellularData::coVariant, NCellularData::MIX_coord, 0 );
		  NCellularData::HomLocator hom( ga, gb );
		  test = cdList[i]->homGroup( hom );
		  if (!test->isCycleMap()) CPPUNIT_FAIL("isCycleMap() error.");
		  if (!test->isMonic()) CPPUNIT_FAIL("isMonic() error.");
		  if (!test->isEpic()) CPPUNIT_FAIL("isEpic() error.");
		  if (!test->isIsomorphism()) CPPUNIT_FAIL("isIsomorphism() error.");
		  if (test->isIdentity()) CPPUNIT_FAIL("isIdentity() error.");
		  if (test->isZero()) CPPUNIT_FAIL("isZero() error.");
	  	 }
// these mod-p computations are a little time consuming so we'll leave these kinds of tests to be part of
//  the ncellulardata tests
		for (unsigned long i=6; i<9; i++) // restricted to <13 to avoid some big triangulations
		 {
		  NCellularData::GroupLocator ga( 1, NCellularData::coVariant, NCellularData::STD_coord, 10 );
		  NCellularData::GroupLocator gb( 1, NCellularData::coVariant, NCellularData::MIX_coord, 10 );
		  NCellularData::HomLocator hom( ga, gb );
		  test = cdList[i]->homGroup( hom );
		  if (!test->isCycleMap()) CPPUNIT_FAIL("isCycleMap() error (2).");
		  if (!test->isMonic()) CPPUNIT_FAIL("isMonic() error (2).");
		  if (!test->isEpic()) CPPUNIT_FAIL("isEpic() error (2).");
		  if (!test->isIsomorphism()) CPPUNIT_FAIL("isIsomorphism() error (2).");
		  if (test->isIdentity()) CPPUNIT_FAIL("isIdentity() error (2).");
		  if (test->isZero()) CPPUNIT_FAIL("isZero() error (2).");
	  	 }
		}

	void hom_induced_objects() { // getKernel, getCoKernel, getImage,getDomain, getRange, getDefiningMatrix, getReducedMatrix
		const regina::NHomMarkedAbelianGroup* test(NULL);
                std::stringstream temp;
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  NCellularData::GroupLocator ga( 1, NCellularData::coVariant, NCellularData::STD_coord, 0 );
		  NCellularData::GroupLocator gb( 1, NCellularData::coVariant, NCellularData::STD_REL_BDRY_coord, 0 );
		  NCellularData::HomLocator hom( ga, gb );
		  test = cdList[i]->homGroup( hom );
		  temp.str("");
		  test->getKernel().writeTextShort(temp);
		  if (temp.str() != ker_mtr[i]) CPPUNIT_FAIL("getKernel() error.");
	  	 }
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  NCellularData::GroupLocator ga( 1, NCellularData::coVariant, NCellularData::STD_BDRY_coord, 0 );
		  NCellularData::GroupLocator gb( 1, NCellularData::coVariant, NCellularData::STD_coord, 0 );
		  NCellularData::HomLocator hom( ga, gb );
		  test = cdList[i]->homGroup( hom );
		  temp.str("");
		  test->getCokernel().writeTextShort(temp);
		  if (temp.str() != coker_mtr[i]) CPPUNIT_FAIL("getCokernel() error.");
	  	 }
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  NCellularData::GroupLocator ga( 1, NCellularData::coVariant, NCellularData::STD_BDRY_coord, 0 );
		  NCellularData::GroupLocator gb( 1, NCellularData::coVariant, NCellularData::STD_coord, 0 );
		  NCellularData::HomLocator hom( ga, gb );
		  test = cdList[i]->homGroup( hom );
		  temp.str("");
		  test->getImage().writeTextShort(temp); 
		  if (temp.str() != ker_mtr[i]) CPPUNIT_FAIL("getImage() error.");
	  	 }
		// todo: explicit CC initializations to allow for some getDefiningMatrix and getReducedMatrix tests.
		}

	void hom_detailed_tests() { // writeTextShort, evalCC, evalSNF, inverseHom, operator*, isIdentity, writeReducedMatrix
		const regina::NHomMarkedAbelianGroup* test1(NULL);
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  NCellularData::GroupLocator ga( 1, NCellularData::coVariant, NCellularData::STD_coord, 0 );
		  NCellularData::GroupLocator gb( 1, NCellularData::coVariant, NCellularData::MIX_coord, 0 );
		  NCellularData::HomLocator hom( ga, gb );
		  test1 = cdList[i]->homGroup( hom );
		  NHomMarkedAbelianGroup test2( test1->inverseHom() );
		  NHomMarkedAbelianGroup test3( (*test1)*test2 );
		  NHomMarkedAbelianGroup test4( test2*(*test1) );
		  if (!test3.isIdentity()) CPPUNIT_FAIL("right inverse error.");
		  if (!test4.isIdentity()) CPPUNIT_FAIL("left inverse error.");
	  	 }
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  NCellularData::GroupLocator ga( 1, NCellularData::coVariant, NCellularData::STD_coord, 10 );
		  NCellularData::GroupLocator gb( 1, NCellularData::coVariant, NCellularData::MIX_coord, 10 );
		  NCellularData::HomLocator hom( ga, gb );
		  test1 = cdList[i]->homGroup( hom );
		  NHomMarkedAbelianGroup test2( test1->inverseHom() );
		  NHomMarkedAbelianGroup test3( (*test1)*test2 );
		  NHomMarkedAbelianGroup test4( test2*(*test1) );
		  if (!test3.isIdentity()) CPPUNIT_FAIL("right inverse error (2).");
		  if (!test4.isIdentity()) CPPUNIT_FAIL("left inverse error (2).");
	  	 }
		// evalCC and evalSNF tests...
		// first test: check for a commutative diagram with the homomorphism in CC and SNF coordinates.
	        //   test on generators in snf coordinates, I suppose... shortened a little for speed considerations
		for (unsigned long i=0; i<cdList.size(); i++)
		 {
		  NCellularData::GroupLocator ga( 1, NCellularData::coVariant, NCellularData::STD_coord, 10 );
		  NCellularData::GroupLocator gb( 1, NCellularData::coVariant, NCellularData::STD_REL_BDRY_coord, 10 );
		  NCellularData::HomLocator hom( ga, gb );
		  test1 = cdList[i]->homGroup( hom );
		  for (unsigned long j=0; j<test1->getDomain().minNumberOfGenerators(); j++)
		   {
		    std::vector< NLargeInteger > domCCj( test1->getDomain().ccRep(j) );
		    std::vector< NLargeInteger > domSNFj( test1->getDomain().snfRep( domCCj ) );
		    std::vector< NLargeInteger > ranCCj( test1->evalCC( domCCj ) );
		    std::vector< NLargeInteger > ranSNFj( test1->evalSNF( domSNFj ) );
		    std::vector< NLargeInteger > CD( test1->getRange().snfRep( ranCCj ) );
		    if ( CD != ranSNFj ) CPPUNIT_FAIL("evalSNF / evalCC error.");
		   }
		 }
		// writeReducedMatrix tests...
		}
};

void addNMarkedAbelianGroup(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NMarkedAbelianGroupTest::suite());
}

