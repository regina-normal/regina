
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "algebra/homgrouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "utilities/stringutils.h"

#include "testhelper.h"

using regina::Integer;
using regina::MarkedAbelianGroup;
using regina::HomMarkedAbelianGroup;
using regina::MatrixInt;
using regina::GroupPresentation;
using regina::GroupExpression;
using regina::HomGroupPresentation;

// These are a pretty limited set of tests.  But they're a start. -ryan

class GroupPresentationTest : public testing::Test {
    protected:
        GroupPresentation Z_pres, Z6_pres, D8_pres;
        GroupPresentation fig8_pres, CS_pres, CSCS_pres, KSUM_pres, FOX_pres;
        GroupPresentation KPDS_pres, Z2Z3Z8_pres;

        // this list will be allocated to include all the above presentations.
        // TODO: adjust this to store groups by value, not by pointer.
        std::list< GroupPresentation* > presList;

        GroupPresentationTest() {
            // the integers
            Z_pres.addGenerator(1);

            // Z modulo 6
            Z6_pres.addGenerator(1);
            Z6_pres.addRelation("a^6");

            // dihedral group of octagon.
            D8_pres.addGenerator(2);
            D8_pres.addRelation("a^8");
            D8_pres.addRelation("b^2");
            D8_pres.addRelation("abab");

            // figure-8 knot exterior.
            fig8_pres.addGenerator(2);
            fig8_pres.addRelation("aaBAbabAB");

            // Bundle over S^1 with fiber S^1 x S^2 # S^1 x S^2.
            KSUM_pres.addGenerator(2);
            KSUM_pres.addRelation("a^2b^3");

            // Fox quick trip example 11.
            FOX_pres.addGenerator(2);
            FOX_pres.addRelation("a^2BAb");

            // Cappell-Shaneson with Alex 1+t-t^3
            CS_pres.addGenerator(2);
            CS_pres.addRelation("a^3BA^2b^2");
            CS_pres.addRelation("a^2B^2Ab^3");

            // Fibers over S^1 with fiber (S^1)^3 # (S^1)^3
            CSCS_pres.addGenerator(2);
            CSCS_pres.addRelation("A^2b^2a^4B^3");
            CSCS_pres.addRelation("a^2bA^4ba^2B^2");

            // Fibers over S^1 with fiber a Poincare Dodecahedral space.
            KPDS_pres.addGenerator(2);
            KPDS_pres.addRelation("aBBabbAbb");
            KPDS_pres.addRelation("AbbaabbbaB");

            // Z_2 + Z_3 + Z_8
            Z2Z3Z8_pres.addGenerator(3);
            Z2Z3Z8_pres.addRelation("a^2");
            Z2Z3Z8_pres.addRelation("b^3");
            Z2Z3Z8_pres.addRelation("c^8");
            Z2Z3Z8_pres.addRelation("abAB");
            Z2Z3Z8_pres.addRelation("acAC");
            Z2Z3Z8_pres.addRelation("bcBC");

            presList.push_back(&Z_pres);    presList.push_back(&Z6_pres);
            presList.push_back(&D8_pres);   presList.push_back(&fig8_pres);
            presList.push_back(&KSUM_pres); presList.push_back(&FOX_pres);
            presList.push_back(&CS_pres);   presList.push_back(&CSCS_pres);
            presList.push_back(&KPDS_pres); presList.push_back(&Z2Z3Z8_pres);
        }
};

TEST_F(GroupPresentationTest, reidemeisterSchreir) {
    for (GroupPresentation* g : presList) {
        SCOPED_TRACE("g = " + g->str());

        g->simplify();
        // Currently identifyExtensionOverZ() is private, so we
        // cannot call it.  Examine the name from recogniseGroup()
        // instead to see if R-S worked.
        std::string name = g->recogniseGroup();
        if (! regina::startsWith(name, "Z~")) {
            // We know which cases this should fail for.
            // Note that Reidemeister-Schreir should work for Z and KPDS,
            // but their recognised names are Z and (unknown)
            // respectively and so we exclude them here.
            EXPECT_TRUE(
                (g == &Z_pres) ||
                (g == &Z6_pres) ||
                (g == &D8_pres) ||
                (g == &FOX_pres) ||
                (g == &KPDS_pres) ||
                (g == &Z2Z3Z8_pres));
        }
    }
};

TEST_F(GroupPresentationTest, wordReduction) {
    // Test inversion, multiplication, word reduction.
    GroupExpression word1("abcABC");
    GroupExpression word2("cbaCBA");
    GroupExpression word3("abccbaBCCBA");

    GroupExpression word4 = word1.inverse();
    EXPECT_EQ(word4, word2);

    word1.simplify();
    word2.simplify();
    EXPECT_EQ(word1.countTerms(), 6);
    EXPECT_EQ(word2.countTerms(), 6);
    word1.simplify(true);
    word2.simplify(true);
    EXPECT_EQ(word1.countTerms(), 6);
    EXPECT_EQ(word2.countTerms(), 6);

    word1.addTermsLast(word2);
    word1.simplify();
    EXPECT_EQ(word1.countTerms(), 0);

    // The constructor will automatically merge cc, CC -> c^2, C^-2.
    EXPECT_EQ(word3.countTerms(), 9);
    word3.simplify();
    EXPECT_EQ(word3.countTerms(), 9);
    word3.simplify(true);
    EXPECT_EQ(word3.countTerms(), 1);
}

TEST_F(GroupPresentationTest, validity) {
    for (GroupPresentation* g : presList) {
        SCOPED_TRACE("g = " + g->str());
        EXPECT_TRUE(g->isValid());
    }

    GroupPresentation DPRES;
    DPRES.addGenerator(1);
    DPRES.addRelation( "ab^2aaa" );
    EXPECT_FALSE(DPRES.isValid());
}

TEST_F(GroupPresentationTest, homologicalAlignment) {
    // ensure homological alignment does what we claim
    for (const GroupPresentation* g : presList) {
        SCOPED_TRACE("g = " + g->str());

        GroupPresentation tPres(*g);
        tPres.homologicalAlignment();
        MarkedAbelianGroup mab = tPres.markedAbelianisation();
        size_t N = mab.countInvariantFactors();
        size_t M = mab.snfRank();
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
        for (size_t j=0; j<tPres.countGenerators(); j++) {
            SCOPED_TRACE_NAMED_NUMERIC("gen", j);

            regina::Vector<Integer> temp = mab.snfRep(
                regina::Vector<Integer>::unit(tPres.countGenerators(), j));

            for (size_t k=0; k<M; k++) {
                SCOPED_TRACE_NAMED_NUMERIC("col", k);

                // case 1: columns of torsion abelianisations
                if (j<N) {
                    // check in temp if dj | entry(j) for all j != k
                    // and             GCD(dj,entry(j)) == 1  j == k
                    if (k==j)
                        EXPECT_EQ(temp[k].gcd( mab.invariantFactor(k) ), 1);
                    else if ( k < N )
                        EXPECT_EQ(temp[k] % mab.invariantFactor(k), 0);
                    else
                        EXPECT_EQ(temp[k], 0);

                    // dj divides entry(j) for all j!=k,
                    // and GCD(dk,entry(k)==1.
                    continue;
                }

                // case 2: column should be delta_ij
                if (j<M) {  // entry
                    if (k==j)
                        EXPECT_EQ(temp[k].abs(), 1);
                    else if (k<N)
                        EXPECT_EQ(temp[k] % mab.invariantFactor(k), 0);
                    else
                        EXPECT_EQ(temp[k], 0);
                    continue;
                }

                // case 3: column should be zero (modulo d's)
                if (k<N)
                    EXPECT_EQ(temp[k] % mab.invariantFactor(k), 0);
                else
                    EXPECT_EQ(temp[k], 0);
            }
        }
    }
}

TEST_F(GroupPresentationTest, simplifyHomomorphism) {
    // This test was added in regina 7.3 to address an issue where
    // the high-level HomGroupPresentation simplification routines
    // were incorrectly conjugating images/preimages of generators.

    GroupPresentation D(2);
    GroupPresentation G(4, { "aaaaacBCB" });

    HomGroupPresentation H(D, G,
        { GroupExpression("daaaaaD"), GroupExpression("dbcbCD") });
    HomGroupPresentation H2(H);

    // In regina 7.2 and earlier, this incorrectly conjugated the image of
    // the first generator, resulting in the incorrectly simplified image
    // bcbC instead of the correct image daaaaaD.
    H2.simplify();

    EXPECT_EQ(H, H2);
}
