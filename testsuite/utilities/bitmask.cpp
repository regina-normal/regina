
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

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "testsuite/utilities/testutilities.h"
#include "utilities/bitmask.h"

class BitmaskTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(BitmaskTest);

    CPPUNIT_TEST(assignment);
    CPPUNIT_TEST(sizes);
    CPPUNIT_TEST(firstLastBit);
    CPPUNIT_TEST(bits);
    CPPUNIT_TEST(truncate);
    CPPUNIT_TEST(lexOrder);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void assignment() {
            // Try using assignment to initialise a bitmask.
            regina::Bitmask a;
            regina::Bitmask b(2);
            b.set(0, true);
            b.set(1, false);

            a = b;
            if (! (a.get(0) && ! a.get(1)))
                CPPUNIT_FAIL("Bitmask assignment initialisation error.");

            // Just make sure we don't crash here.
            regina::Bitmask c;
            b = c;

            regina::Bitmask d;
            c = d;

            // Try using assignment to resize a bitmask.
            regina::Bitmask e(4);
            e.set(0, false);
            e.set(1, true);
            e.set(2, false);
            e.set(3, true);

            b = e;
            if (! ((! b.get(0)) && b.get(1) && (! b.get(2)) && b.get(3)))
                CPPUNIT_FAIL("Bitmask assignment resizing error.");
        }

        void sizes() {
            if (sizeof(regina::BitmaskLen8) < 1)
                CPPUNIT_FAIL("BitmaskLen8 does not hold 1 byte.");
            if (sizeof(regina::BitmaskLen16) < 2)
                CPPUNIT_FAIL("BitmaskLen16 does not hold 2 bytes.");
            if (sizeof(regina::BitmaskLen32) < 4)
                CPPUNIT_FAIL("BitmaskLen32 does not hold 4 bytes.");
            if (sizeof(regina::BitmaskLen64) < 8)
                CPPUNIT_FAIL("BitmaskLen64 does not hold 8 bytes.");
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
            testFirstLastBit<regina::BitmaskLen8>("len8", 8);
            testFirstLastBit<regina::BitmaskLen16>("len16", 16);
            testFirstLastBit<regina::BitmaskLen32>("len32", 32);
            testFirstLastBit<regina::BitmaskLen64>("len64", 64);
            testFirstLastBit<regina::Bitmask1<unsigned char> >("uchar", 8);
            testFirstLastBit<regina::Bitmask1<unsigned long> >
                ("ulong", 8 * sizeof(unsigned long));
            testFirstLastBit<regina::Bitmask2<unsigned char, unsigned char> >
                ("uchar,uchar", 16);
            testFirstLastBit<regina::Bitmask2<unsigned char, unsigned long> >
                ("uchar,ulong", 8 + 8 * sizeof(unsigned long));
            testFirstLastBit<regina::Bitmask2<unsigned long, unsigned char> >
                ("ulong,uchar", 8 + 8 * sizeof(unsigned long));
            testFirstLastBit<regina::Bitmask>("pieces", 128);
        }

        template <typename BitmaskType>
        void testBits(const char* typeDesc, int length) {
            {
                BitmaskType b(length);
                for (int i = 0; i <= length; ++i) {
                    if (b.bits() != i) {
                        std::ostringstream out;
                        out << "Bitmask using type " << typeDesc
                            << ", len=" << length
                            << " with T at first " << i << " positions"
                            << " reports bits != " << i << ".";
                        CPPUNIT_FAIL(out.str());
                    }
                    if (i < length)
                        b.set(i, true);
                }
            }

            {
                BitmaskType b(length);
                for (int i = 0; i <= length; ++i) {
                    if (b.bits() != i) {
                        std::ostringstream out;
                        out << "Bitmask using type " << typeDesc
                            << ", len=" << length
                            << " with T at last " << i << " positions"
                            << " reports bits != " << i << ".";
                        CPPUNIT_FAIL(out.str());
                    }
                    if (i < length)
                        b.set(length - i - 1, true);
                }
            }
        }

        void bits() {
            testBits<regina::BitmaskLen8>("len8", 8);
            testBits<regina::BitmaskLen16>("len16", 16);
            testBits<regina::BitmaskLen32>("len32", 32);
            testBits<regina::BitmaskLen64>("len64", 64);
            testBits<regina::Bitmask1<unsigned char> >("uchar", 8);
            testBits<regina::Bitmask1<unsigned long> >
                ("ulong", 8 * sizeof(unsigned long));
            testBits<regina::Bitmask2<unsigned char, unsigned char> >
                ("uchar,uchar", 16);
            testBits<regina::Bitmask2<unsigned char, unsigned long> >
                ("uchar,ulong", 8 + 8 * sizeof(unsigned long));
            testBits<regina::Bitmask2<unsigned long, unsigned char> >
                ("ulong,uchar", 8 + 8 * sizeof(unsigned long));
            testBits<regina::Bitmask>("pieces", 128);
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
            testTruncate<regina::BitmaskLen8>("len8", 8);
            testTruncate<regina::BitmaskLen16>("len16", 16);
            testTruncate<regina::BitmaskLen32>("len32", 32);
            testTruncate<regina::BitmaskLen64>("len64", 64);
            testTruncate<regina::Bitmask1<unsigned char> >("uchar", 8);
            testTruncate<regina::Bitmask1<unsigned long> >
                ("ulong", 8 * sizeof(unsigned long));
            testTruncate<regina::Bitmask2<unsigned char, unsigned char> >
                ("uchar,uchar", 16);
            testTruncate<regina::Bitmask2<unsigned char, unsigned long> >
                ("uchar,ulong", 8 + 8 * sizeof(unsigned long));
            testTruncate<regina::Bitmask2<unsigned long, unsigned char> >
                ("ulong,uchar", 8 + 8 * sizeof(unsigned long));
            testTruncate<regina::Bitmask>("pieces", 128);
        }

        template <typename BitmaskType>
        void testLexOrder(const char* typeDesc, int length) {
            BitmaskType b[256];
            int i, j;

            for (i = 0; i < 256; ++i) {
                b[i].reset(length);
                for (j = 0; j < 8; ++j)
                    if (i & (1 << j))
                        b[i].set(j * (length / 8), true);
            }

            for (i = 0; i < 256; ++i) {
                if (b[i].lessThan(b[i])) {
                    std::ostringstream out;
                    out << "Bitmask using type " << typeDesc
                        << ", len=" << length << ": x < x";
                    CPPUNIT_FAIL(out.str());
                }
                if (i > 0 && ! (b[i-1].lessThan(b[i]))) {
                    std::ostringstream out;
                    out << "Bitmask using type " << typeDesc
                        << ", len=" << length
                        << ": lessThan() gives incorrect order";
                    CPPUNIT_FAIL(out.str());
                }
                if (i > 0 && b[i].lessThan(b[i-1])) {
                    std::ostringstream out;
                    out << "Bitmask using type " << typeDesc
                        << ", len=" << length
                        << ": lessThan() gives incorrect order";
                    CPPUNIT_FAIL(out.str());
                }
            }
        }

        void lexOrder() {
            testLexOrder<regina::BitmaskLen8>("len8", 8);
            testLexOrder<regina::BitmaskLen16>("len16", 16);
            testLexOrder<regina::BitmaskLen32>("len32", 32);
            testLexOrder<regina::BitmaskLen64>("len64", 64);
            testLexOrder<regina::Bitmask1<unsigned char> >("uchar", 8);
            testLexOrder<regina::Bitmask1<unsigned long> >
                ("ulong", 8 * sizeof(unsigned long));
            testLexOrder<regina::Bitmask2<unsigned char, unsigned char> >
                ("uchar,uchar", 16);
            testLexOrder<regina::Bitmask2<unsigned char, unsigned long> >
                ("uchar,ulong", 8 + 8 * sizeof(unsigned long));
            testLexOrder<regina::Bitmask2<unsigned long, unsigned char> >
                ("ulong,uchar", 8 + 8 * sizeof(unsigned long));
            testLexOrder<regina::Bitmask>("pieces", 128);
        }
};

void addBitmask(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(BitmaskTest::suite());
}

