
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

#include <algorithm>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "testsuite/utilities/testutilities.h"
#include "utilities/nbitmask.h"

class NBitmaskTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NBitmaskTest);

    CPPUNIT_TEST(sizes);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void sizes() {
            if (sizeof(regina::NBitmaskLen8) < 1)
                CPPUNIT_FAIL("NBitmaskLen8 does not hold 1 byte.");
            if (sizeof(regina::NBitmaskLen16) < 2)
                CPPUNIT_FAIL("NBitmaskLen16 does not hold 2 bytes.");
            if (sizeof(regina::NBitmaskLen32) < 4)
                CPPUNIT_FAIL("NBitmaskLen32 does not hold 4 bytes.");
            if (sizeof(regina::NBitmaskLen64) < 8)
                CPPUNIT_FAIL("NBitmaskLen64 does not hold 8 bytes.");
        }
};

void addNBitmask(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NBitmaskTest::suite());
}

