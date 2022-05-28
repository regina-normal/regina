
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
#include "triangulation/dim2.h"
#include "testsuite/dim2/testdim2.h"
#include "testsuite/utilities/tightencodingtest.h"

using regina::Isomorphism;
using regina::Perm;
using regina::Triangulation;

class Isomorphism2Test :
        public CppUnit::TestFixture, public TightEncodingTest<Isomorphism<2>> {
    CPPUNIT_TEST_SUITE(Isomorphism2Test);

    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        void tightEncoding() {
            // Cases where the isomorphism is empty:
            verifyTightEncoding(Isomorphism<2>(0));

            // Exhaustive run through all small isomorphisms (we need
            // both odd and even sizes here since permutations are
            // encoded in pairs):
            {
                Isomorphism<2> iso = Isomorphism<2>::identity(1);
                do {
                    verifyTightEncoding(iso);
                    ++iso;
                } while (! iso.isIdentity());
            }
            {
                Isomorphism<2> iso = Isomorphism<2>::identity(2);
                do {
                    verifyTightEncoding(iso);
                    ++iso;
                } while (! iso.isIdentity());
            }
            {
                Isomorphism<2> iso = Isomorphism<2>::identity(3);
                do {
                    verifyTightEncoding(iso);
                    ++iso;
                } while (! iso.isIdentity());
            }

            // Cases where the isomorphism includes higher-numbered
            // simplex images, and also uninitialised simplex images:
            {
                Isomorphism<2> iso(2);
                iso.simpImage(0) = -1;
                iso.simpImage(1) = 3;
                iso.facetPerm(0) = Perm<3>(1,2,0);
                iso.facetPerm(1) = Perm<3>(2,0,1);
                verifyTightEncoding(iso);
            }
        }
};

void addIsomorphism2(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Isomorphism2Test::suite());
}

