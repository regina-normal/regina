
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "triangulation/facetpairing.h"

#include "testsuite/exhaustive.h"
#include "testsuite/generic/facetpairingtest.h"
#include "testsuite/dim4/testdim4.h"

using regina::FacetPairing;

class FacetPairing4Test : public FacetPairingTest<4> {
    CPPUNIT_TEST_SUITE(FacetPairing4Test);

    CPPUNIT_TEST(isCanonical);
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(rawCountsClosed);
    CPPUNIT_TEST(rawCountsBounded);
    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();

    public:
        void isCanonical() {
            FacetPairingTest<4>::isCanonicalAllClosed(0);
            FacetPairingTest<4>::isCanonicalAllClosed(2);
            FacetPairingTest<4>::isCanonicalAllClosed(4);
            FacetPairingTest<4>::isCanonicalAllBounded(1);
            FacetPairingTest<4>::isCanonicalAllBounded(2);
            FacetPairingTest<4>::isCanonicalAllBounded(3);
            FacetPairingTest<4>::isCanonicalAllBounded(4);
        }

        void makeCanonical() {
            FacetPairingTest<4>::makeCanonicalAllClosed(0);
            // Already too slow just for n=2. :/
            // FacetPairingTest<4>::makeCanonicalAllClosed(2);
            FacetPairingTest<4>::makeCanonicalAllBounded(1);
            // FacetPairingTest<4>::makeCanonicalAllBounded(2);
        }

        void rawCountsClosed() {
            // Figures taken from sequence A129430 in the On-Line
            // Encyclopedia of Integer Sequences, as enumerated by
            // Brendan McKay using the software Nauty.
            unsigned nPairs[] = { 0, 0, 3, 0, 26, 0, 639, 0, 40264 };

            for (size_t i = 0; i <= 5; ++i)
                FacetPairingTest<4>::enumerateClosed(i, nPairs[i]);
        }

        void rawCountsBounded() {
            // Figures based on enumeration under the 4-manifolds branch
            // at the time of the Regina 4.93 release.
            unsigned nBdry[] = { 0, 3, 11, 61, 473, 4487 };
            unsigned nBdry1[] = { 0, 1, 0, 10, 0, 284, 0, 17761 };
            unsigned nBdry2[] = { 0, 0, 4, 0, 91, 0, 4665 };

            for (size_t i = 0; i <= 6; ++i)
                FacetPairingTest<4>::enumerateBounded(i, 1, nBdry1[i]);

            for (size_t i = 0; i <= 5; ++i)
                FacetPairingTest<4>::enumerateBounded(i, 2, nBdry2[i]);

            for (size_t i = 0; i <= 4; ++i)
                FacetPairingTest<4>::enumerateBounded(i, nBdry[i]);
        }

        void tightEncoding() {
            FacetPairingTest<4>::tightEncodingAllClosed(2);
            FacetPairingTest<4>::tightEncodingAllClosed(4);
            FacetPairingTest<4>::tightEncodingAllBounded(1);
            FacetPairingTest<4>::tightEncodingAllBounded(2);
            FacetPairingTest<4>::tightEncodingAllBounded(3);
            FacetPairingTest<4>::tightEncodingAllBounded(4);
        }
};

void addFacetPairing4(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(FacetPairing4Test::suite());
}

