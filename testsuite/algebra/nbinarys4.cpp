
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
#include "algebra/nbinaryS4.h"
#include "testsuite/algebra/testalgebra.h"

using regina::NBinaryS4;

class NBinaryS4Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NBinaryS4Test);

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
            for (unsigned long i=0; i<48; i++)
                for (unsigned long j=0; j<48; j++)
                    for (unsigned long k=0; k<48; k++) {
                        NBinaryS4 ei(i), ej(j), ek(k);
                        if ( (ei*ej)*ek != ei*(ej*ek) )
                            CPPUNIT_FAIL("NBinaryS4 does not satisfy "
                                "associativity axiom.");
                    }
        }

        void identity() {
            for (unsigned long i=0; i<48; i++) {
                NBinaryS4 Id, ei(i);
                if ( (Id*ei != ei) || (ei*Id != ei) )
                    CPPUNIT_FAIL("NBinaryS4 does not have an "
                        "identity element.");
            }
        }

        void inverses() {
            for (unsigned long i=0; i<48; i++) {
                NBinaryS4 Id, ei(i);
                if ( (ei*ei.inverse() != Id) || (ei.inverse()*ei != Id) )
                    CPPUNIT_FAIL("NBinaryS4 does not have inverses.");
            }
        }

        void homomorphism() {
            for (unsigned long i=0; i<48; i++)
                for (unsigned long j=0; j<48; j++) {
                    NBinaryS4 ei( i ), ej( j );
                    if ( (ei*ej).getNPerm4() != ei.getNPerm4()*ej.getNPerm4() )
                        CPPUNIT_FAIL("NBinaryS4 does not map "
                            "homomorphically onto S4.");
                }
        }
};

void addNBinaryS4(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NBinaryS4Test::suite());
}

