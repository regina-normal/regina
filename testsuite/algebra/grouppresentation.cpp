
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>

#include "algebra/grouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "utilities/stringutils.h"

#include "testsuite/algebra/testalgebra.h"

using regina::Integer;
using regina::MarkedAbelianGroup;
using regina::HomMarkedAbelianGroup;
using regina::MatrixInt;
using regina::GroupPresentation;
using regina::GroupExpression;
using regina::HomGroupPresentation;

// These are a pretty limited set of tests.  But they're a start. -ryan

class GroupPresentationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(GroupPresentationTest);

    CPPUNIT_TEST(RS_test); // Reidemeister-Schreir
    CPPUNIT_TEST(word_reduction_test); // inversion, multiplication,
                                       // word reduction.
    CPPUNIT_TEST(presValid_test); // validity of presentation.
    // CPPUNIT_TEST(simplify_word_test); // TODO: word simplification in pres.
    // CPPUNIT_TEST(ab_test); // TODO: abelianization tests.
    // CPPUNIT_TEST(nielsen_test); // TODO: nielsen moves
    CPPUNIT_TEST(homalign_test); // homological alignment.

    CPPUNIT_TEST_SUITE_END();

    private:
        GroupPresentation Z_pres, Z6_pres, D8_pres;
        GroupPresentation fig8_pres, CS_pres, CSCS_pres, KSUM_pres, FOX_pres;
        GroupPresentation KPDS_pres, Z2Z3Z8_pres;

        // this list will be allocated to include all the above presentations.
        std::list< GroupPresentation* > presList;

    public:

    void setUp() {
        // the integers
        Z_pres.addGenerator(1);

        // Z modulo 6
        Z6_pres.addGenerator(1);
        Z6_pres.addRelation( new GroupExpression("a^6") );

        // dihedral group of octagon.
        D8_pres.addGenerator(2);
        D8_pres.addRelation( new GroupExpression("a^8") );
        D8_pres.addRelation( new GroupExpression("b^2") );
        D8_pres.addRelation( new GroupExpression("abab") );

        // figure-8 knot exterior.
        fig8_pres.addGenerator(2);
        fig8_pres.addRelation( new GroupExpression("aaBAbabAB") );

        // Bundle over S^1 with fiber S^1 x S^2 # S^1 x S^2.
        KSUM_pres.addGenerator(2);
        KSUM_pres.addRelation( new GroupExpression("a^2b^3") );

        // Fox quick trip example 11.
        FOX_pres.addGenerator(2);
        FOX_pres.addRelation( new GroupExpression("a^2BAb") );

        // Cappell-Shaneson with Alex 1+t-t^3
        CS_pres.addGenerator(2);
        CS_pres.addRelation( new GroupExpression("a^3BA^2b^2") );
        CS_pres.addRelation( new GroupExpression("a^2B^2Ab^3") );

        // Fibers over S^1 with fiber (S^1)^3 # (S^1)^3
        CSCS_pres.addGenerator(2);
        CSCS_pres.addRelation( new GroupExpression("A^2b^2a^4B^3") );
        CSCS_pres.addRelation( new GroupExpression("a^2bA^4ba^2B^2") );

        // Fibers over S^1 with fiber a Poincare Dodecahedral space.
        KPDS_pres.addGenerator(2);
        KPDS_pres.addRelation( new GroupExpression("aBBabbAbb") );
        KPDS_pres.addRelation( new GroupExpression("AbbaabbbaB") );

        // Z_2 + Z_3 + Z_8
        Z2Z3Z8_pres.addGenerator(3);
        Z2Z3Z8_pres.addRelation( new GroupExpression("a^2") );
        Z2Z3Z8_pres.addRelation( new GroupExpression("b^3") );
        Z2Z3Z8_pres.addRelation( new GroupExpression("c^8") );
        Z2Z3Z8_pres.addRelation( new GroupExpression("abAB") );
        Z2Z3Z8_pres.addRelation( new GroupExpression("acAC") );
        Z2Z3Z8_pres.addRelation( new GroupExpression("bcBC") );

        presList.push_back( &Z_pres );    presList.push_back( &Z6_pres );
        presList.push_back( &D8_pres );   presList.push_back( &fig8_pres );
        presList.push_back( &KSUM_pres ); presList.push_back( &FOX_pres );
        presList.push_back( &CS_pres );   presList.push_back( &CSCS_pres );
        presList.push_back( &KPDS_pres ); presList.push_back( &Z2Z3Z8_pres );
    } // end setUp

    void tearDown() {
        // so far nothing.
    }

    void RS_test() {
        for (std::list<GroupPresentation*>::iterator i=presList.begin();
                i!=presList.end(); i++) {
            (*i)->intelligentSimplify();
            // Currently identifyExtensionOverZ() is private, so we
            // cannot call it.  Examine the name from recogniseGroup()
            // instead to see if R-S worked.
            std::string name = (*i)->recogniseGroup();
            if (! regina::startsWith(name, "Z~")) {
                // We know which cases this should fail for.
                // Note that Reidemeister-Schreir should work for Z and KPDS,
                // but their recognised names are Z and (unknown)
                // respectively and so we exclude them here.
                if ( (*i != &Z_pres) &&
                        (*i != &Z6_pres) &&
                        (*i != &D8_pres) &&
                        (*i != &FOX_pres) &&
                        (*i != &KPDS_pres) &&
                        (*i != &Z2Z3Z8_pres) )
                    CPPUNIT_FAIL("Reidemeister-Schreir failure.");
            }
        }
    }; // end RS_test

    void word_reduction_test() { // pure GroupExpression tests here.
        GroupExpression word1("abcABC");
        GroupExpression word2("cbaCBA");
        GroupExpression word3("abccbaBCCBA");

        GroupExpression* word4( word1.inverse() );
        if ( !( (*word4)==word2 ) )
            CPPUNIT_FAIL("GroupExpression::inverse "+word2.str()+
                std::string(" != ")+word4->str());
        delete word4;

        word1.simplify();
        word2.simplify();
        if (word1.countTerms() != 6)
            CPPUNIT_FAIL("GroupExpression::number of terms (1). "+word1.str());
        if (word1.countTerms() != 6)
            CPPUNIT_FAIL("GroupExpression::number of terms (2). "+word1.str());
        word1.simplify(true);
        word2.simplify(true);
        if (word1.countTerms() != 6)
            CPPUNIT_FAIL("GroupExpression::number of terms (3). "+word1.str());
        if (word1.countTerms() != 6)
            CPPUNIT_FAIL("GroupExpression::number of terms (4). "+word1.str());

        word1.addTermsLast(word2);
        word1.simplify();
        if (word1.countTerms() != 0)
            CPPUNIT_FAIL("GroupExpression::inverse (2). "+word1.str());

        if (word3.countTerms() != 11)
            CPPUNIT_FAIL("GroupExpression::number of terms (5). "+word3.str());
        word3.simplify();
        if (word3.countTerms() != 9)
            CPPUNIT_FAIL("GroupExpression::number of terms (6). "+word3.str());
        word3.simplify(true);
        if (word3.countTerms() != 1)
            CPPUNIT_FAIL("GroupExpression::number of terms (7). "+word3.str());
    }
    void presValid_test() {
        for (std::list<GroupPresentation*>::iterator i=presList.begin();
                i!=presList.end(); i++)
            if (!(*i)->isValid())
                CPPUNIT_FAIL("Invalid presentation.");
        GroupPresentation DPRES;
        DPRES.addGenerator(1);
        DPRES.addRelation( new GroupExpression("ab^2aaa") );
        if (DPRES.isValid())
            CPPUNIT_FAIL("DPRES: invalid presentation.");
    }

    /*
    void simplify_word_test() { // this is with respect to a presentation.
        // TODO
    }
    void ab_test() { // abelianize, marked abelianize, and isAbelian
        // TODO
    }
    void nielsen_test() {
        // TODO
    }
    */
    void homalign_test() {
        // ensure homological alignment does what we claim
        for (std::list<GroupPresentation*>::iterator i=presList.begin();
                i!=presList.end(); i++) {
            GroupPresentation tPres( *(*i) );
            tPres.homologicalAlignment();
            std::unique_ptr<MarkedAbelianGroup> mab( tPres.markedAbelianisation() );
            unsigned long N(mab->countInvariantFactors());
            unsigned long M(mab->minNumberOfGenerators());
            /*
             * If the abelianisation of this group has rank N and M
             * invariant factors d0 | d2 | ... | d(M-1),
             * this routine applies Nielsen moves
             * to the presentation to ensure that under the markedAbelianisation
             * routine, generators 0 through M-1 are mapped to generators of the
             * relevant Z_di group.  Similarly, generators M through M+N-1 are
             * mapped to +-1 in the appropriate factor. All further generators
             * will be mapped to zero.
             */
            for (unsigned long j=0; j<tPres.countGenerators(); j++) {
                std::vector<Integer> epsilon( tPres.countGenerators() );
                epsilon[j] = 1;
                std::vector<Integer> temp( mab->snfRep(epsilon) );

                for (unsigned long k=0; k<M; k++) {
                    // case 1: columns of torsion abelianisations
                    if (j<N) {
                        // check in temp if dj | entry(j) for all j != k
                        // and             GCD(dj,entry(j)) == 1  j == k
                        if (k==j) {
                            if (temp[k].gcd( mab->invariantFactor(k) )!=
                                    Integer::one)
                                CPPUNIT_FAIL("GroupPresentation: homologicalAlignment Error 1.");
                        } else if ( k < N ) {
                            if ( temp[k] % mab->invariantFactor(k) !=
                                    Integer::zero )
                                CPPUNIT_FAIL("GroupPresentation: homologicalAlignment Error 2.");
                        } else if ( temp[k] != Integer::zero )
                            CPPUNIT_FAIL("GroupPresentation: homologicalAlignment Error 3.");
                        // dj divides entry(j) for all j!=k,
                        // and GCD(dk,entry(k)==1.
                        continue;
                    }

                    // case 2: column should be delta_ij
                    if (j<M) {  // entry
                        if (k==j) {
                            if (temp[k].abs() != Integer::one)
                                CPPUNIT_FAIL("GroupPresentation: homologicalAlignment Error 4.");
                        } else if (k<N) {
                            if (temp[k] % mab->invariantFactor(k) !=
                                    Integer::zero )
                                CPPUNIT_FAIL("GroupPresentation: homologicalAlignment Error 5.");
                        } else if (temp[k] != Integer::zero)
                            CPPUNIT_FAIL("GroupPresentation: homologicalAlignment Error 6.");
                        continue;
                    }

                    // case 3: column should be zero (modulo d's)
                    if (k<N) {
                        if (temp[k] % mab->invariantFactor(k) !=
                                Integer::zero )
                            CPPUNIT_FAIL("GroupPresentation: homologicalAlignment Error 7.");
                    } else if (temp[k] != Integer::zero)
                        CPPUNIT_FAIL("GroupPresentation: homologicalAlignment Error 8.");
                } // end k loop
            } // end j loop
        } // end i loop
    } // end homalign_test()
};

void addGroupPresentation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(GroupPresentationTest::suite());
}

