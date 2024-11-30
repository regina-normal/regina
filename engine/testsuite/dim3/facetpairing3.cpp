
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

#include "triangulation/facetpairing3.h"

#include "generic/facetpairingtest.h"

TEST(FacetPairing3Test, isCanonical) {
    FacetPairingTest<3>::isCanonicalAllClosed(0);
    FacetPairingTest<3>::isCanonicalAllClosed(1);
    FacetPairingTest<3>::isCanonicalAllClosed(2);
    FacetPairingTest<3>::isCanonicalAllBounded(1);
    FacetPairingTest<3>::isCanonicalAllBounded(2);
    FacetPairingTest<3>::isCanonicalAllBounded(3);
}

TEST(FacetPairing3Test, makeCanonical) {
    FacetPairingTest<3>::makeCanonicalAllClosed(0);
    FacetPairingTest<3>::makeCanonicalAllClosed(1);
    FacetPairingTest<3>::makeCanonicalAllClosed(2);
    FacetPairingTest<3>::makeCanonicalAllBounded(1);
    FacetPairingTest<3>::makeCanonicalAllBounded(2);
}

TEST(FacetPairing3Test, rawCountsClosed) {
    // Figures taken from "Face pairing graphs and 3-manifold
    // enumeration", Benjamin A. Burton, J. Knot Theory
    // Ramifications 13 (2004), pp. 1057--1101.
    //
    // See also OEIS sequence #A085549.
    static const size_t nPairs[] = { 0, 1, 2, 4, 10, 28, 97, 359, 1635 };

    for (size_t i = 0; i <= 8; ++i)
        FacetPairingTest<3>::enumerateClosed(i, nPairs[i]);
}

TEST(FacetPairing3Test, rawCountsBounded) {
    // Figures based on enumeration under Regina 4.93.
    static const size_t nBdry[] = { 0, 2, 6, 21, 100, 521, 3234, 22304 };
    static const size_t nBdry2[] = { 0, 1, 3, 8, 30, 118, 548, 2790, 16029 };

    for (size_t i = 0; i <= 8; ++i)
        FacetPairingTest<3>::enumerateBounded(i, 1, 0);

    for (size_t i = 0; i <= 7; ++i)
        FacetPairingTest<3>::enumerateBounded(i, 2, nBdry2[i]);

    for (size_t i = 0; i <= 6; ++i)
        FacetPairingTest<3>::enumerateBounded(i, nBdry[i]);
}

TEST(FacetPairing3Test, tightEncoding) {
    FacetPairingTest<3>::tightEncodingAllClosed(1);
    FacetPairingTest<3>::tightEncodingAllClosed(2);
    FacetPairingTest<3>::tightEncodingAllBounded(1);
    FacetPairingTest<3>::tightEncodingAllBounded(2);
    FacetPairingTest<3>::tightEncodingAllBounded(3);
}

TEST(FacetPairing3Test, badSubgraphs) {
    // Figures taken from "Face pairing graphs and 3-manifold
    // enumeration", Benjamin A. Burton, J. Knot Theory
    // Ramifications 13 (2004), pp. 1057--1101.
    static const size_t nTriple[] = { 0, 0, 1, 1, 3, 8, 29, 109, 497 };
    static const size_t nBroken[] = { 0, 0, 0, 1, 3, 10, 36, 137, 608 };
    static const size_t nHandle[] = { 0, 0, 0, 1, 2, 4, 12, 40, 155 };

    // Figures taken from "Enumeration of non-orientable
    // 3-manifolds using face-pairing graphs and union-find",
    // Benjamin A. Burton, Discrete Comput. Geom. 38 (2007),
    // no. 3, 527--571.
    static const size_t nWedged[] = { 0, 0, 0, 0, 1, 2, 5, 13, 46 };
    static const size_t nStray[] = { 0, 0, 0, 1, 4, 13, 56, 227, 1083 };
    static const size_t nTripleChain[] = { 0, 0, 0, 0, 0, 1, 2, 5, 14 };

    // Figures taken from an initial test run with some manual
    // verification.
    static const size_t nSingleStar[] = { 0, 0, 0, 0, 0, 0, 0, 0, 130 };
    static const size_t nDoubleStar[] = { 0, 0, 0, 0, 0, 0, 16, 88, 615 };
    static const size_t nDoubleSquare[] = { 0, 0, 0, 0, 3, 4, 16, 50, 217 };

    for (size_t nTets = 1; nTets <= 8; nTets++) {
        SCOPED_TRACE_NUMERIC(nTets);

        size_t tripleEdge = 0;
        size_t brokenDoubleEndedChain = 0;
        size_t oneEndedChainWithDoubleHandle = 0;
        size_t wedgedDoubleEndedChain = 0;
        size_t oneEndedChainWithStrayBigon = 0;
        size_t tripleOneEndedChain = 0;
        size_t singleStar = 0;
        size_t doubleStar = 0;
        size_t doubleSquare = 0;

        regina::FacetPairing<3>::findAllPairings(nTets, false, 0,
                [&](const FacetPairing<3>& pair, FacetPairing<3>::IsoList) {
            if (pair.hasMultiEdge<3>())
                ++tripleEdge;
            if (pair.hasBrokenDoubleEndedChain())
                ++brokenDoubleEndedChain;
            if (pair.hasOneEndedChainWithDoubleHandle())
                ++oneEndedChainWithDoubleHandle;
            if (pair.hasWedgedDoubleEndedChain())
                ++wedgedDoubleEndedChain;
            if (pair.hasOneEndedChainWithStrayBigon())
                ++oneEndedChainWithStrayBigon;
            if (pair.hasTripleOneEndedChain())
                ++tripleOneEndedChain;
            if (pair.hasSingleStar())
                ++singleStar;
            if (pair.hasDoubleStar())
                ++doubleStar;
            if (pair.hasDoubleSquare())
                ++doubleSquare;
        });

        EXPECT_EQ(tripleEdge, nTriple[nTets]);
        EXPECT_EQ(brokenDoubleEndedChain, nBroken[nTets]);
        EXPECT_EQ(oneEndedChainWithDoubleHandle, nHandle[nTets]);
        EXPECT_EQ(wedgedDoubleEndedChain, nWedged[nTets]);
        EXPECT_EQ(oneEndedChainWithStrayBigon, nStray[nTets]);
        EXPECT_EQ(tripleOneEndedChain, nTripleChain[nTets]);
        EXPECT_EQ(singleStar, nSingleStar[nTets]);
        EXPECT_EQ(doubleStar, nDoubleStar[nTets]);
        EXPECT_EQ(doubleSquare, nDoubleSquare[nTets]);
    }
}
