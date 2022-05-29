
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "testsuite/dim2/testdim2.h"

using regina::FacetPairing;

class FacetPairing2Test : public FacetPairingTest<2> {
    CPPUNIT_TEST_SUITE(FacetPairing2Test);

    CPPUNIT_TEST(isCanonical);
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(rawCountsClosed);
    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();

    public:
        void isCanonical() {
            FacetPairingTest<2>::isCanonicalAllClosed(0);
            FacetPairingTest<2>::isCanonicalAllClosed(2);
            FacetPairingTest<2>::isCanonicalAllClosed(4);
            FacetPairingTest<2>::isCanonicalAllBounded(1);
            FacetPairingTest<2>::isCanonicalAllBounded(2);
            FacetPairingTest<2>::isCanonicalAllBounded(3);
            FacetPairingTest<2>::isCanonicalAllBounded(4);
        }

        void makeCanonical() {
            FacetPairingTest<2>::makeCanonicalAllClosed(0);
            FacetPairingTest<2>::makeCanonicalAllClosed(2);
            FacetPairingTest<2>::makeCanonicalAllClosed(4);
            FacetPairingTest<2>::makeCanonicalAllBounded(1);
            FacetPairingTest<2>::makeCanonicalAllBounded(2);
            FacetPairingTest<2>::makeCanonicalAllBounded(3);
            FacetPairingTest<2>::makeCanonicalAllBounded(4);
        }

        void rawCountsClosed() {
            // Figures taken from the online encyclopedia of integer
            // sequences, #A005967.
            size_t nPairs[] = { 0, 0, 2, 0, 5, 0, 17, 0, 71, 0, 388, 0, 2592 };

            for (size_t i = 0; i <= 12; ++i)
                FacetPairingTest<2>::enumerateClosed(i, nPairs[i]);
        }

        void tightEncoding() {
            FacetPairingTest<2>::tightEncodingAllClosed(2);
            FacetPairingTest<2>::tightEncodingAllClosed(4);
            FacetPairingTest<2>::tightEncodingAllBounded(1);
            FacetPairingTest<2>::tightEncodingAllBounded(2);
            FacetPairingTest<2>::tightEncodingAllBounded(3);
            FacetPairingTest<2>::tightEncodingAllBounded(4);
        }
};

void addFacetPairing2(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(FacetPairing2Test::suite());
}

