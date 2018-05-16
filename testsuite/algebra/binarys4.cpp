
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
#include "algebra/binaryS4.h"
#include "testsuite/algebra/testalgebra.h"

using regina::BinaryS4;

class BinaryS4Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(BinaryS4Test);

    CPPUNIT_TEST(associativity);
    CPPUNIT_TEST(identity);
    CPPUNIT_TEST(inverses);
    CPPUNIT_TEST(homomorphism);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void associativity() {
            BinaryS4 ei, ej, ek;
            for (unsigned long i=0; i<48; i++) {
                ei.setPermCode(i);
                for (unsigned long j=0; j<48; j++) {
                    ej.setPermCode(j);
                    for (unsigned long k=0; k<48; k++) {
                        ek.setPermCode(k);
                        if ( (ei*ej)*ek != ei*(ej*ek) )
                            CPPUNIT_FAIL("BinaryS4 does not satisfy "
                                "associativity axiom.");
                    }
                }
            }
        }

        void identity() {
            BinaryS4 Id, ei;
            for (unsigned long i=0; i<48; i++) {
                ei.setPermCode(i);
                if ( (Id*ei != ei) || (ei*Id != ei) )
                    CPPUNIT_FAIL("BinaryS4 does not have an "
                        "identity element.");
            }
        }

        void inverses() {
            BinaryS4 Id, ei;
            for (unsigned long i=0; i<48; i++) {
                ei.setPermCode(i);
                if ( (ei*ei.inverse() != Id) || (ei.inverse()*ei != Id) )
                    CPPUNIT_FAIL("BinaryS4 does not have inverses.");
            }
        }

        void homomorphism() {
            BinaryS4 ei, ej;
            for (unsigned long i=0; i<48; i++) {
                ei.setPermCode(i);
                for (unsigned long j=0; j<48; j++) {
                    ej.setPermCode(j);
                    if ( (ei*ej).getPerm4() != ei.getPerm4()*ej.getPerm4() )
                        CPPUNIT_FAIL("BinaryS4 does not map "
                            "homomorphically onto S4.");
                }
            }
        }
};

void addBinaryS4(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(BinaryS4Test::suite());
}

