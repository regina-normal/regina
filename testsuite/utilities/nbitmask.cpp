
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
    CPPUNIT_TEST(firstLastBit);
    CPPUNIT_TEST(truncate);

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

        template <typename BitmaskType>
        void testFirstLastBit(const char* typeDesc, int length) {
            int i, j;
            for (i = 0; i < length; ++i)
                for (j = i; j < length; ++j) {
                    BitmaskType b(length);
                    b.set(i, true);
                    b.set(j, true);
                    if (b.firstBit() != i) {
                        std::ostringstream out;
                        out << "Bitmask using type " << typeDesc
                            << ", len=" << length << " with T at positions "
                            << i << ',' << j
                            << " reports firstBit != " << i << ".";
                        CPPUNIT_FAIL(out.str());
                    }
                    if (b.lastBit() != j) {
                        std::ostringstream out;
                        out << "Bitmask using type " << typeDesc
                            << ", len=" << length << " with T at positions "
                            << i << ',' << j
                            << " reports lastBit != " << j << ".";
                        CPPUNIT_FAIL(out.str());
                    }
                }

            BitmaskType zero(length);
            if (zero.firstBit() != -1) {
                std::ostringstream out;
                out << "Zero bitmask using type " << typeDesc <<
                    ", len=" << length << " reports firstBit != -1.";
                CPPUNIT_FAIL(out.str());
            }
            if (zero.lastBit() != -1) {
                std::ostringstream out;
                out << "Zero bitmask using type " << typeDesc <<
                    ", len=" << length << " reports lastBit != -1.";
                CPPUNIT_FAIL(out.str());
            }
        }

        void firstLastBit() {
            testFirstLastBit<regina::NBitmaskLen8>("len8", 8);
            testFirstLastBit<regina::NBitmaskLen16>("len16", 16);
            testFirstLastBit<regina::NBitmaskLen32>("len32", 32);
            testFirstLastBit<regina::NBitmaskLen64>("len64", 64);
            testFirstLastBit<regina::NBitmask1<unsigned char> >("uchar", 8);
            testFirstLastBit<regina::NBitmask1<unsigned long> >
                ("ulong", 8 * sizeof(unsigned long));
            testFirstLastBit<regina::NBitmask2<unsigned char, unsigned char> >
                ("uchar,uchar", 16);
            testFirstLastBit<regina::NBitmask2<unsigned char, unsigned long> >
                ("uchar,ulong", 8 + 8 * sizeof(unsigned long));
            testFirstLastBit<regina::NBitmask2<unsigned long, unsigned char> >
                ("ulong,uchar", 8 + 8 * sizeof(unsigned long));
            testFirstLastBit<regina::NBitmask>("pieces", 128);
        }

        template <typename BitmaskType>
        void testTruncate(const char* typeDesc, int length) {
            int i;
            {
                BitmaskType b(length);
                b.flip();
                b.truncate(0);

                if (b.firstBit() != -1 || b.lastBit() != -1 || b.bits() != 0) {
                    std::ostringstream out;
                    out << "Bitmask using type " << typeDesc
                        << ", len=" << length << " truncated to 0 bits "
                        << "does not appear to be the zero bitmask.";
                    CPPUNIT_FAIL(out.str());
                }
            }
            for (i = 1; i <= length; ++i) {
                BitmaskType b(length);
                b.flip();
                b.truncate(i);
                if (b.firstBit() != 0 || b.lastBit() != i-1 || b.bits() != i) {
                    std::ostringstream out;
                    out << "Bitmask using type " << typeDesc
                        << ", len=" << length << " truncated to " << i
                        << " bits does not appear correct.";
                    CPPUNIT_FAIL(out.str());
                }
            }
        }

        void truncate() {
            testTruncate<regina::NBitmaskLen8>("len8", 8);
            testTruncate<regina::NBitmaskLen16>("len16", 16);
            testTruncate<regina::NBitmaskLen32>("len32", 32);
            testTruncate<regina::NBitmaskLen64>("len64", 64);
            testTruncate<regina::NBitmask1<unsigned char> >("uchar", 8);
            testTruncate<regina::NBitmask1<unsigned long> >
                ("ulong", 8 * sizeof(unsigned long));
            testTruncate<regina::NBitmask2<unsigned char, unsigned char> >
                ("uchar,uchar", 16);
            testTruncate<regina::NBitmask2<unsigned char, unsigned long> >
                ("uchar,ulong", 8 + 8 * sizeof(unsigned long));
            testTruncate<regina::NBitmask2<unsigned long, unsigned char> >
                ("ulong,uchar", 8 + 8 * sizeof(unsigned long));
            testTruncate<regina::NBitmask>("pieces", 128);
        }
};

void addNBitmask(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NBitmaskTest::suite());
}

