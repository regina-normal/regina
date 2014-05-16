
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include "algebra/ngrouppresentation.h"
#include "algebra/nmarkedabeliangroup.h"

#include "testsuite/utilities/testutilities.h"

using regina::NLargeInteger;
using regina::NMarkedAbelianGroup;
using regina::NHomMarkedAbelianGroup;
using regina::NMatrixInt;
using regina::NGroupPresentation;
using regina::NGroupExpression;
using regina::NHomGroupPresentation;

// These are a pretty limited set of tests.  But they're a start. -ryan

class NGroupPresentationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NGroupPresentationTest);

    CPPUNIT_TEST(RS_test); // Reidemeister-Schreir
    CPPUNIT_TEST(word_reduction_test); // inversion, multiplication
      // word reduction. 
    CPPUNIT_TEST(presValid_test); // validity of presentation.
    CPPUNIT_TEST(simplify_word_test); // word simplification in pres.
    CPPUNIT_TEST(ab_test); // abelianization tests.
    CPPUNIT_TEST(nielsen_test); // nielsen moves
    CPPUNIT_TEST(homalign_test); // homological alignment.

    CPPUNIT_TEST_SUITE_END();
 
    private:
    NGroupPresentation Z_pres, Z6_pres, D8_pres; 
    NGroupPresentation fig8_pres, CS_pres, CSCS_pres, KSUM_pres, FOX_pres;
    NGroupPresentation KPDS_pres, Z2Z3Z8_pres;

    // this list will be allocated to include all the above presentations.
    std::list< NGroupPresentation* > presList;

    public:

    void setUp() {
        // the integers
        Z_pres.addGenerator(1);

        // Z modulo 6
        Z6_pres.addGenerator(1);
        Z6_pres.addRelation( new NGroupExpression("a^6") );

        // dihedral group of octagon.
        D8_pres.addGenerator(2); 
        D8_pres.addRelation( new NGroupExpression("a^8") );
        D8_pres.addRelation( new NGroupExpression("b^2") );
        D8_pres.addRelation( new NGroupExpression("abab") );

        // figure-8 knot exterior.
        fig8_pres.addGenerator(2);
        fig8_pres.addRelation( new NGroupExpression("aaBAbabAB") );

        // Bundle over S^1 with fiber S^1 x S^2 # S^1 x S^2. 
        KSUM_pres.addGenerator(2);
        KSUM_pres.addRelation( new NGroupExpression("a^2b^3") );

        // Fox quick trip example 11. 
        FOX_pres.addGenerator(2);
        FOX_pres.addRelation( new NGroupExpression("a^2BAb") );

        // Cappell-Shaneson with Alex 1+t-t^3
        CS_pres.addGenerator(2);
        CS_pres.addRelation( new NGroupExpression("a^3BA^2b^2") );
        CS_pres.addRelation( new NGroupExpression("a^2B^2Ab^3") );

        // Fibers over S^1 with fiber (S^1)^3 # (S^1)^3
        CSCS_pres.addGenerator(2);
        CSCS_pres.addRelation( new NGroupExpression("A^2b^2a^4B^3") );
        CSCS_pres.addRelation( new NGroupExpression("a^2bA^4ba^2B^2") );

        // Fibers over S^1 with fiber a Poincare Dodecahedral space.
        KPDS_pres.addGenerator(2);
        KPDS_pres.addRelation( new NGroupExpression("aBBabbAbb") );
        KPDS_pres.addRelation( new NGroupExpression("AbbaabbbaB") );

        // Z_2 + Z_3 + Z_8
        Z2Z3Z8_pres.addGenerator(3);
        Z2Z3Z8_pres.addRelation( new NGroupExpression("a^2") );
        Z2Z3Z8_pres.addRelation( new NGroupExpression("b^3") );
        Z2Z3Z8_pres.addRelation( new NGroupExpression("c^8") );
        Z2Z3Z8_pres.addRelation( new NGroupExpression("abAB") );
        Z2Z3Z8_pres.addRelation( new NGroupExpression("acAC") );
        Z2Z3Z8_pres.addRelation( new NGroupExpression("bcBC") );

        presList.push_back( &Z_pres );    presList.push_back( &Z6_pres );
        presList.push_back( &D8_pres );   presList.push_back( &fig8_pres );
        presList.push_back( &KSUM_pres ); presList.push_back( &FOX_pres );
        presList.push_back( &CS_pres );   presList.push_back( &CSCS_pres );
        presList.push_back( &KPDS_pres ); presList.push_back( &Z2Z3Z8_pres );
     } // end setUp

   void tearDown() {
    // TODO - so far nothing.
    }

   void RS_test() {
    for (std::list<NGroupPresentation*>::iterator i=presList.begin(); 
         i!=presList.end(); i++)
     {
      (*i)->intelligentSimplify();      
      std::auto_ptr< NHomGroupPresentation > 
         homPtr( (*i)->identify_extension_over_Z() );
      if (homPtr.get()==NULL) if ( ( *i != &Z6_pres) &&
       (*i != &D8_pres) && (*i != &FOX_pres) && (*i != &Z2Z3Z8_pres) )
         CPPUNIT_FAIL("NGroupPresentation::Reidemeister-Schreir failure.");
     }
   }; // end RS_test

   void word_reduction_test() { // pure NGroupExpression tests here.
     NGroupExpression word1("abcABC");
     NGroupExpression word2("cbaCBA");
     NGroupExpression word3("abccbaBCCBA");

     NGroupExpression* word4( word1.inverse() );
     if ( !( (*word4)==word2 ) ) 
        CPPUNIT_FAIL("NGroupExpression::inverse "+word2.str()+
            std::string(" != ")+word4->str());
     delete word4;

     word1.simplify(); word2.simplify();
     if (word1.getNumberOfTerms() != 6) 
        CPPUNIT_FAIL("NGroupExpression::number of terms (1). "+word1.str());
     if (word1.getNumberOfTerms() != 6) 
        CPPUNIT_FAIL("NGroupExpression::number of terms (2). "+word1.str());
     word1.simplify(true); word2.simplify(true);
     if (word1.getNumberOfTerms() != 6) 
        CPPUNIT_FAIL("NGroupExpression::number of terms (3). "+word1.str());
     if (word1.getNumberOfTerms() != 6) 
        CPPUNIT_FAIL("NGroupExpression::number of terms (4). "+word1.str());

     word1.addTermsLast(word2);
     word1.simplify();
     if (word1.getNumberOfTerms() != 0)
        CPPUNIT_FAIL("NGroupExpression::inverse (2). "+word1.str());

     if (word3.getNumberOfTerms() != 11)
        CPPUNIT_FAIL("NGroupExpression::number of terms (5). "+word3.str());
     word3.simplify();
     if (word3.getNumberOfTerms() != 9)
        CPPUNIT_FAIL("NGroupExpression::number of terms (6). "+word3.str());
     word3.simplify(true);
     if (word3.getNumberOfTerms() != 1)
        CPPUNIT_FAIL("NGroupExpression::number of terms (7). "+word3.str());
    }
   void presValid_test() {
     for (std::list<NGroupPresentation*>::iterator i=presList.begin(); 
          i!=presList.end(); i++)
          if (!(*i)->isValid()) 
            CPPUNIT_FAIL("NGroupPresentation::valid presentation.");
     NGroupPresentation DPRES;
     DPRES.addGenerator(1);
     DPRES.addRelation( new NGroupExpression("ab^2aaa") );
     if (DPRES.isValid()) 
        CPPUNIT_FAIL("NGroupPresentation::invalid presentation.");
    }
   void simplify_word_test() { // this is with respect to a presentation. 
     // TODO
    }
   void ab_test() { // abelianize, marked abelianize, and isAbelian
     // TODO
    }
   void nielsen_test() {
     // TODO
    }
   void homalign_test() { // ensure homological alignment does what we claim
      for (std::list<NGroupPresentation*>::iterator i=presList.begin();
            i!=presList.end(); i++)
        { 
         NGroupPresentation tPres( *(*i) );
         tPres.homologicalAlignment();
         std::auto_ptr<NMarkedAbelianGroup> mab( tPres.markedAbelianisation() );
         unsigned long N(mab->getNumberOfInvariantFactors());
         unsigned long M(mab->minNumberOfGenerators());
        /*
         * If the abelianisation of this group has rank N and M invariant
         * factors d0 | d2 | ... | d(M-1), this routine applies Nielsen moves
         * to the presentation to ensure that under the markedAbelianisation
         * routine, generators 0 through M-1 are mapped to generators of the
         * relevant Z_di group.  Similarly, generators M through M+N-1 are
         * mapped to +-1 in the appropriate factor. All further generators 
         * will be mapped to zero. 
         */
         for (unsigned long j=0; j<tPres.getNumberOfGenerators(); j++)
          {
           std::vector<NLargeInteger> epsilon( tPres.getNumberOfGenerators() );
           epsilon[j] = 1;
           std::vector<NLargeInteger> temp( mab->snfRep(epsilon) );

           for (unsigned long k=0; k<M; k++)
            {
            // case 1: columns of torsion abelianisations
            if (j<N) 
             {
             // check in temp if dj | entry(j) for all j != k
              // and             GCD(dj,entry(j)) == 1  j == k
              if (k==j) { if (temp[k].gcd( mab->getInvariantFactor(k) )!=
                    NLargeInteger::one)
                CPPUNIT_FAIL("NGroupPresentation: homologicalAlignment Error 1."); }
              else if ( k < N ) { if ( temp[k] % mab->getInvariantFactor(k) != 
                    NLargeInteger::zero )
                CPPUNIT_FAIL("NGroupPresentation: homologicalAlignment Error 2."); }
              else if ( temp[k] != NLargeInteger::zero )
                CPPUNIT_FAIL("NGroupPresentation: homologicalAlignment Error 3.");
              // dj divides entry(j) for all j!=k, 
              // and GCD(dk,entry(k)==1.
              continue; 
             } 

            // case 2: column should be delta_ij
            if (j<M) 
             {  // entry 
             if (k==j) { if (temp[k].abs() != NLargeInteger::one) 
                CPPUNIT_FAIL("NGroupPresentation: homologicalAlignment Error 4."); }
              else if (k<N) { if (temp[k] % mab->getInvariantFactor(k) !=
                NLargeInteger::zero )
                CPPUNIT_FAIL("NGroupPresentation: homologicalAlignment Error 5."); }
              else if (temp[k] != NLargeInteger::zero)
                CPPUNIT_FAIL("NGroupPresentation: homologicalAlignment Error 6.");
              continue;
             }

            // case 3: column should be zero (modulo d's)
            if (k<N) { 
             if (temp[k] % mab->getInvariantFactor(k) != NLargeInteger::zero )
              CPPUNIT_FAIL("NGroupPresentation: homologicalAlignment Error 7."); }
            else if (temp[k] != NLargeInteger::zero)
              CPPUNIT_FAIL("NGroupPresentation: homologicalAlignment Error 8.");
           } // end k loop
         } // end j loop
        } // end i loop
    } // end homalign_test()

}; // end NBilinearFormTest

void addNGroupPresentation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NGroupPresentationTest::suite());
}

