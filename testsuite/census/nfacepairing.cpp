
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
#include "census/nfacepairing.h"
#include "testsuite/census/testcensus.h"

using regina::NFacePairing;
using regina::NFacePairingIsoList;
using regina::NBoolSet;

/**
 * Simply increment the given count when a face pairing is found.
 */
void countFacePairings(const NFacePairing* pair, const NFacePairingIsoList*,
        void* count) {
    if (pair)
        (*(unsigned*)count)++;
}

/**
 * Increment counts for face pairing graphs with interesting properties.
 */
namespace {
    struct BadGraphs {
        unsigned tripleEdge;
        unsigned brokenDoubleEndedChain;
        unsigned oneEndedChainWithDoubleHandle;
        unsigned wedgedDoubleEndedChain;
        unsigned oneEndedChainWithStrayBigon;
        unsigned tripleOneEndedChain;
        unsigned singleStar;
        unsigned doubleStar;
        unsigned doubleSquare;

        BadGraphs() : tripleEdge(0), brokenDoubleEndedChain(0),
                      oneEndedChainWithDoubleHandle(0),
                      wedgedDoubleEndedChain(0),
                      oneEndedChainWithStrayBigon(0), tripleOneEndedChain(0),
                      singleStar(0), doubleStar(0), doubleSquare(0) {
        }
    };
}

void countBadGraphs(const NFacePairing* pair, const NFacePairingIsoList*,
        void* badGraphsRaw) {
    if (pair) {
        BadGraphs* badGraphs = static_cast<BadGraphs*>(badGraphsRaw);

        if (pair->hasTripleEdge())
            badGraphs->tripleEdge++;
        if (pair->hasBrokenDoubleEndedChain())
            badGraphs->brokenDoubleEndedChain++;
        if (pair->hasOneEndedChainWithDoubleHandle())
            badGraphs->oneEndedChainWithDoubleHandle++;
        if (pair->hasWedgedDoubleEndedChain())
            badGraphs->wedgedDoubleEndedChain++;
        if (pair->hasOneEndedChainWithStrayBigon())
            badGraphs->oneEndedChainWithStrayBigon++;
        if (pair->hasTripleOneEndedChain())
            badGraphs->tripleOneEndedChain++;
        if (pair->hasSingleStar())
            badGraphs->singleStar++;
        if (pair->hasDoubleStar())
            badGraphs->doubleStar++;
        if (pair->hasDoubleSquare())
            badGraphs->doubleSquare++;
    }
}

class NFacePairingTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NFacePairingTest);

    CPPUNIT_TEST(rawCounts);
    CPPUNIT_TEST(badSubgraphs);

    CPPUNIT_TEST_SUITE_END();

    private:
        unsigned count;
            /**< Used to hold arbitrary totals. */

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void rawCounts() {
            // Figures taken from "Face pairing graphs and 3-manifold
            // enumeration", Benjamin A. Burton, J. Knot Theory
            // Ramifications 13 (2004), pp. 1057--1101.
            unsigned nPairs[] = { 1, 1, 2, 4, 10, 28, 97, 359, 1635 };

            unsigned nTets;
            for (nTets = 1; nTets <= 8; nTets++) {
                count = 0;
                NFacePairing::findAllPairings(nTets, NBoolSet::sFalse,
                    0, countFacePairings, &count, false);

                std::ostringstream msg;
                msg << "Face pairing count for " << nTets
                    << " tetrahedra should be " << nPairs[nTets]
                    << ", not " << count << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(), count == nPairs[nTets]);
            }
        }

        void badSubgraphs() {
            // Figures taken from "Face pairing graphs and 3-manifold
            // enumeration", Benjamin A. Burton, J. Knot Theory
            // Ramifications 13 (2004), pp. 1057--1101.
            unsigned nTriple[] = { 0, 0, 1, 1, 3, 8, 29, 109, 497 };
            unsigned nBroken[] = { 0, 0, 0, 1, 3, 10, 36, 137, 608 };
            unsigned nHandle[] = { 0, 0, 0, 1, 2, 4, 12, 40, 155 };

            // Figures taken from "Enumeration of non-orientable
            // 3-manifolds using face pairing graphs and union-find",
            // Benjamin A. Burton, preprint, math.GT/0604584.
            unsigned nWedged[] = { 0, 0, 0, 0, 1, 2, 5, 13, 46 };
            unsigned nStray[] = { 0, 0, 0, 1, 4, 13, 56, 227, 1083 };
            unsigned nTripleChain[] = { 0, 0, 0, 0, 0, 1, 2, 5, 14 };

            // Figures taken from an initial test run with some manual
            // verification.
            unsigned nSingleStar[] = { 0, 0, 0, 0, 0, 0, 0, 0, 130 };
            unsigned nDoubleStar[] = { 0, 0, 0, 0, 0, 0, 16, 88, 615 };
            unsigned nDoubleSquare[] = { 0, 0, 0, 0, 3, 4, 16, 50, 217 };

            unsigned nTets;
            for (nTets = 1; nTets <= 8; nTets++) {
                BadGraphs bad;
                NFacePairing::findAllPairings(nTets, NBoolSet::sFalse,
                    0, countBadGraphs, &bad, false);

                if (bad.tripleEdge != nTriple[nTets]) {
                    std::ostringstream msg;
                    msg << "Triple edge count for " << nTets
                        << " tetrahedra should be " << nTriple[nTets]
                        << ", not " << bad.tripleEdge << '.';
                    CPPUNIT_FAIL(msg.str());
                }
                if (bad.brokenDoubleEndedChain != nBroken[nTets]) {
                    std::ostringstream msg;
                    msg << "Broken double-ended chain count for " << nTets
                        << " tetrahedra should be " << nBroken[nTets]
                        << ", not " << bad.brokenDoubleEndedChain << '.';
                    CPPUNIT_FAIL(msg.str());
                }
                if (bad.oneEndedChainWithDoubleHandle != nHandle[nTets]) {
                    std::ostringstream msg;
                    msg << "One-ended chain with double handle count for "
                        << nTets << " tetrahedra should be " << nHandle[nTets]
                        << ", not " << bad.oneEndedChainWithDoubleHandle << '.';
                    CPPUNIT_FAIL(msg.str());
                }
                if (bad.wedgedDoubleEndedChain != nWedged[nTets]) {
                    std::ostringstream msg;
                    msg << "Wedged double-ended chain count for "
                        << nTets << " tetrahedra should be " << nWedged[nTets]
                        << ", not " << bad.wedgedDoubleEndedChain << '.';
                    CPPUNIT_FAIL(msg.str());
                }
                if (bad.oneEndedChainWithStrayBigon != nStray[nTets]) {
                    std::ostringstream msg;
                    msg << "One-ended chain with stray bigon count for "
                        << nTets << " tetrahedra should be " << nStray[nTets]
                        << ", not " << bad.oneEndedChainWithStrayBigon << '.';
                    CPPUNIT_FAIL(msg.str());
                }
                if (bad.tripleOneEndedChain != nTripleChain[nTets]) {
                    std::ostringstream msg;
                    msg << "Triple one-ended chain count for " << nTets
                        << " tetrahedra should be " << nTripleChain[nTets]
                        << ", not " << bad.tripleOneEndedChain << '.';
                    CPPUNIT_FAIL(msg.str());
                }
                if (bad.singleStar != nSingleStar[nTets]) {
                    std::ostringstream msg;
                    msg << "Single star count for " << nTets
                        << " tetrahedra should be " << nSingleStar[nTets]
                        << ", not " << bad.singleStar << '.';
                    CPPUNIT_FAIL(msg.str());
                }
                if (bad.doubleStar != nDoubleStar[nTets]) {
                    std::ostringstream msg;
                    msg << "Double star count for " << nTets
                        << " tetrahedra should be " << nDoubleStar[nTets]
                        << ", not " << bad.doubleStar << '.';
                    CPPUNIT_FAIL(msg.str());
                }
                if (bad.doubleSquare != nDoubleSquare[nTets]) {
                    std::ostringstream msg;
                    msg << "Double-edged square count for " << nTets
                        << " tetrahedra should be " << nDoubleSquare[nTets]
                        << ", not " << bad.doubleSquare << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }
};

void addNFacePairing(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NFacePairingTest::suite());
}

