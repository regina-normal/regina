
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
#include "testsuite/census/testcensus.h"
#include "triangulation/dim3.h"
#include "triangulation/facetpairing3.h"

using regina::FacetPairing;
using regina::BoolSet;

/**
 * Simply increment the given count when a face pairing is found.
 */
void countFacePairings(const FacetPairing<3>* pair, const FacetPairing<3>::IsoList*,
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

void countBadGraphs(const FacetPairing<3>* pair, const FacetPairing<3>::IsoList*,
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

class FacetPairing3Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(FacetPairing3Test);

    CPPUNIT_TEST(rawCountsClosed);
    CPPUNIT_TEST(rawCountsBounded);
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

        void rawCountsClosed() {
            // Figures taken from "Face pairing graphs and 3-manifold
            // enumeration", Benjamin A. Burton, J. Knot Theory
            // Ramifications 13 (2004), pp. 1057--1101.
            //
            // See also sequence A085549 from the On-Line Encyclopedia
            // of Integer Sequences.
            unsigned nPairs[] = { 0, 1, 2, 4, 10, 28, 97, 359, 1635 };

            unsigned nTets;
            for (nTets = 0; nTets <= 8; nTets++) {
                count = 0;
                FacetPairing<3>::findAllPairings(nTets, BoolSet::sFalse,
                    0, countFacePairings, &count);

                if (count != nPairs[nTets]) {
                    std::ostringstream msg;
                    msg << "Face pairing count for " << nTets
                        << " tetrahedra (closed) should be " << nPairs[nTets]
                        << ", not " << count << '.';

                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void rawCountsBounded() {
            // Figures based on enumeration under Regina 4.93.
            unsigned nBdry[] = { 0, 2, 6, 21, 100, 521, 3234, 22304 };
            unsigned nBdry2[] = { 0, 1, 3, 8, 30, 118, 548, 2790, 16029 };

            unsigned nTets;

            for (nTets = 0; nTets <= 8; nTets++) {
                count = 0;
                FacetPairing<3>::findAllPairings(nTets, BoolSet::sTrue,
                    1, countFacePairings, &count);

                if (count != 0) {
                    std::ostringstream msg;
                    msg << "Face pairing count for " << nTets
                        << " tetrahedra (1 bdry face) should be "
                        << 0 << ", not " << count << '.';

                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (nTets = 0; nTets <= 7; nTets++) {
                count = 0;
                FacetPairing<3>::findAllPairings(nTets, BoolSet::sTrue,
                    2, countFacePairings, &count);

                if (count != nBdry2[nTets]) {
                    std::ostringstream msg;
                    msg << "Face pairing count for " << nTets
                        << " tetrahedra (2 bdry faces) should be "
                        << nBdry2[nTets] << ", not " << count << '.';

                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (nTets = 0; nTets <= 6; nTets++) {
                count = 0;
                FacetPairing<3>::findAllPairings(nTets, BoolSet::sTrue,
                    -1, countFacePairings, &count);

                if (count != nBdry[nTets]) {
                    std::ostringstream msg;
                    msg << "Face pairing count for " << nTets
                        << " tetrahedra (any bdry faces) should be "
                        << nBdry[nTets] << ", not " << count << '.';

                    CPPUNIT_FAIL(msg.str());
                }
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
            // 3-manifolds using face-pairing graphs and union-find",
            // Benjamin A. Burton, Discrete Comput. Geom. 38 (2007),
            // no. 3, 527--571.
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
                FacetPairing<3>::findAllPairings(nTets, BoolSet::sFalse,
                    0, countBadGraphs, &bad);

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

void addFacetPairing3(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(FacetPairing3Test::suite());
}

