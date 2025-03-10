
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
#include "maths/binom.h"
#include "utilities/bitmask.h"
#include "utilities/bitmanip.h"
#include "utilities/intutils.h"

#include "testhelper.h"

namespace {
    /**
     * We need a way to compute (n choose k) where n could be large but
     * either k or n-k is very small.
     *
     * Our binomMedium() is not helpful since it puts a cap on the size of n.
     */
    int binomEdge(int n, int k) {
        if (k < 0 || k > n)
            return 0;
        if (k == 0 || k == n)
            return 1;
        if (k == 1 || k == n - 1)
            return n;
        if (k == 2 || k == n - 2)
            return (n * (n - 1)) / 2;
        if (k == 3 || k == n - 3)
            return (n * (n - 1) * (n - 2)) / 6;
        throw regina::NotImplemented("The test suite's binomEdge(n,k) is only "
            "implemented for small k or small n-k");
    }
}

TEST(BitmaskTest, assignment) {
    // Try using assignment to initialise a bitmask.
    regina::Bitmask a;
    regina::Bitmask b(2);
    b.set(0, true);
    b.set(1, false);

    a = b;
    EXPECT_TRUE(a.get(0));
    EXPECT_FALSE(a.get(1));

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
    EXPECT_FALSE(b.get(0));
    EXPECT_TRUE(b.get(1));
    EXPECT_FALSE(b.get(2));
    EXPECT_TRUE(b.get(3));
}

TEST(BitmaskTest, sizes) {
    EXPECT_GE(sizeof(regina::Bitmask1<uint8_t>), 1);
    EXPECT_GE(sizeof(regina::Bitmask1<uint16_t>), 2);
    EXPECT_GE(sizeof(regina::Bitmask1<uint32_t>), 4);
    EXPECT_GE(sizeof(regina::Bitmask1<uint64_t>), 8);
#ifdef INT128_AVAILABLE
    EXPECT_GE(sizeof(regina::Bitmask1<regina::IntOfSize<16>::utype>), 16);
#endif
}

template <typename BitmaskType>
static void testFirstLastBit(int length) {
    SCOPED_TRACE_TYPE(BitmaskType);
    SCOPED_TRACE_NUMERIC(length);

    for (int i = 0; i < length; ++i)
        for (int j = i; j < length; ++j) {
            BitmaskType b(length);
            b.set(i, true);
            b.set(j, true);
            EXPECT_EQ(b.firstBit(), i);
            EXPECT_EQ(b.lastBit(), j);
        }

    BitmaskType zero(length);
    EXPECT_EQ(zero.firstBit(), -1);
    EXPECT_EQ(zero.lastBit(), -1);
}

TEST(BitmaskTest, firstLastBit) {
    static constexpr int longBits = 8 * sizeof(unsigned long);

    testFirstLastBit<regina::Bitmask1<uint8_t>>(8);
    testFirstLastBit<regina::Bitmask1<uint16_t>>(16);
    testFirstLastBit<regina::Bitmask1<uint32_t>>(32);
    testFirstLastBit<regina::Bitmask1<uint64_t>>(64);
#ifdef INT128_AVAILABLE
    testFirstLastBit<regina::Bitmask1<regina::IntOfSize<16>::utype>>(128);
#endif
    testFirstLastBit<regina::Bitmask1<unsigned char>>(8);
    testFirstLastBit<regina::Bitmask1<unsigned long>>(longBits);
    testFirstLastBit<regina::Bitmask2<unsigned char, unsigned char>>(16);
    testFirstLastBit<regina::Bitmask2<unsigned char, unsigned long>>
        (8 + longBits);
    testFirstLastBit<regina::Bitmask2<unsigned long, unsigned char>>
        (8 + longBits);
    testFirstLastBit<regina::Bitmask>(128);
}

template <typename BitmaskType>
static void testBits(int length) {
    SCOPED_TRACE_TYPE(BitmaskType);
    SCOPED_TRACE_NUMERIC(length);

    BitmaskType a(length);
    for (int i = 0; i <= length; ++i) {
        EXPECT_EQ(a.bits(), i);
        if (i < length)
            a.set(i, true);
    }

    BitmaskType b(length);
    for (int i = 0; i <= length; ++i) {
        EXPECT_EQ(b.bits(), i);
        if (i < length)
            b.set(length - i - 1, true);
    }
}

TEST(BitmaskTest, bits) {
    static constexpr int longBits = 8 * sizeof(unsigned long);

    testBits<regina::Bitmask1<uint8_t>>(8);
    testBits<regina::Bitmask1<uint16_t>>(16);
    testBits<regina::Bitmask1<uint32_t>>(32);
    testBits<regina::Bitmask1<uint64_t>>(64);
#ifdef INT128_AVAILABLE
    testBits<regina::Bitmask1<regina::IntOfSize<16>::utype>>(128);
#endif
    testBits<regina::Bitmask1<unsigned char>>(8);
    testBits<regina::Bitmask1<unsigned long>>(longBits);
    testBits<regina::Bitmask2<unsigned char, unsigned char>>(16);
    testBits<regina::Bitmask2<unsigned char, unsigned long>>(8 + longBits);
    testBits<regina::Bitmask2<unsigned long, unsigned char>>(8 + longBits);
    testBits<regina::Bitmask>(128);
}

template <typename BitmaskType>
static void testTruncate(int length) {
    SCOPED_TRACE_TYPE(BitmaskType);
    SCOPED_TRACE_NUMERIC(length);

    BitmaskType a(length);
    a.flip();
    a.truncate(0);
    EXPECT_EQ(a.firstBit(), -1);
    EXPECT_EQ(a.lastBit(), -1);
    EXPECT_EQ(a.bits(), 0);

    for (int i = 1; i <= length; ++i) {
        BitmaskType b(length);
        b.flip();
        b.truncate(i);
        EXPECT_EQ(b.firstBit(), 0);
        EXPECT_EQ(b.lastBit(), i - 1);
        EXPECT_EQ(b.bits(), i);
    }
}

TEST(BitmaskTest, truncate) {
    static constexpr int longBits = 8 * sizeof(unsigned long);

    testTruncate<regina::Bitmask1<uint8_t>>(8);
    testTruncate<regina::Bitmask1<uint16_t>>(16);
    testTruncate<regina::Bitmask1<uint32_t>>(32);
    testTruncate<regina::Bitmask1<uint64_t>>(64);
#ifdef INT128_AVAILABLE
    testTruncate<regina::Bitmask1<regina::IntOfSize<16>::utype>>(128);
#endif
    testTruncate<regina::Bitmask1<unsigned char>>(8);
    testTruncate<regina::Bitmask1<unsigned long>>(longBits);
    testTruncate<regina::Bitmask2<unsigned char, unsigned char>>(16);
    testTruncate<regina::Bitmask2<unsigned char, unsigned long>>(8 + longBits);
    testTruncate<regina::Bitmask2<unsigned long, unsigned char>>(8 + longBits);
    testTruncate<regina::Bitmask>(128);
}

template <typename BitmaskType>
static void testLexOrder(int length) {
    SCOPED_TRACE_TYPE(BitmaskType);
    SCOPED_TRACE_NUMERIC(length);

    BitmaskType b[256];
    int i, j;

    for (i = 0; i < 256; ++i) {
        b[i].reset(length);
        for (j = 0; j < 8; ++j)
            if (i & (1 << j))
                b[i].set(j * (length / 8), true);
    }

    for (i = 0; i < 256; ++i) {
        SCOPED_TRACE(b[i]);

        // Note: Bitmask uses lessThan() not <, since ≤ tests for subsets.
        EXPECT_FALSE(b[i].lessThan(b[i]));
        if (i > 0) {
            EXPECT_TRUE(b[i - 1].lessThan(b[i]));
            EXPECT_FALSE(b[i].lessThan(b[i - 1]));
        }
    }
}

TEST(BitmaskTest, lexOrder) {
    static constexpr int longBits = 8 * sizeof(unsigned long);

    testLexOrder<regina::Bitmask1<uint8_t>>(8);
    testLexOrder<regina::Bitmask1<uint16_t>>(16);
    testLexOrder<regina::Bitmask1<uint32_t>>(32);
    testLexOrder<regina::Bitmask1<uint64_t>>(64);
#ifdef INT128_AVAILABLE
    testLexOrder<regina::Bitmask1<regina::IntOfSize<16>::utype>>(128);
#endif
    testLexOrder<regina::Bitmask1<unsigned char>>(8);
    testLexOrder<regina::Bitmask1<unsigned long>>(longBits);
    testLexOrder<regina::Bitmask2<unsigned char, unsigned char>>(16);
    testLexOrder<regina::Bitmask2<unsigned char, unsigned long>>(8 + longBits);
    testLexOrder<regina::Bitmask2<unsigned long, unsigned char>>(8 + longBits);
    testLexOrder<regina::Bitmask>(128);
}

template <typename T, int k>
static void verifyNextPermutationFor() {
    SCOPED_TRACE_TYPE(T);
    SCOPED_TRACE_NUMERIC(k);

    size_t count = 0;
    for (T i = (T(1) << k) - 1; i != 0;
            i = regina::BitManipulator<T>::nextPermutation(i)) {
        EXPECT_EQ(regina::BitManipulator<T>::bits(i), k);
        if (k == 1) {
            EXPECT_EQ(regina::BitManipulator<T>::firstBit(i), count);
            EXPECT_EQ(regina::BitManipulator<T>::lastBit(i), count);
        } else {
            unsigned last = regina::BitManipulator<T>::lastBit(i);
            EXPECT_GE(last, k - 1);
            if (last == k - 1) {
                EXPECT_EQ(count, 0);
            } else {
                EXPECT_GE(count, binomEdge(last, k));
                EXPECT_LT(count, binomEdge(last + 1, k));
            }
        }
        ++count;
    }

    EXPECT_EQ(count, binomEdge(sizeof(T) * 8, k));
}

template <typename T>
static void verifyNextPermutation() {
    SCOPED_TRACE_TYPE(T);

    EXPECT_EQ(regina::BitManipulator<T>::nextPermutation(0), 0);
    EXPECT_EQ(regina::BitManipulator<T>::nextPermutation(-1), 0);
    verifyNextPermutationFor<T, 1>();
    verifyNextPermutationFor<T, 2>();
    verifyNextPermutationFor<T, 3>();
    verifyNextPermutationFor<T, sizeof(T) * 8 - 2>();
    verifyNextPermutationFor<T, sizeof(T) * 8 - 1>();
}

TEST(BitmaskTest, nextPermutation) {
    verifyNextPermutation<unsigned char>();
    verifyNextPermutation<unsigned int>();
    verifyNextPermutation<unsigned long>();
    verifyNextPermutation<regina::IntOfSize<1>::utype>();
    verifyNextPermutation<regina::IntOfSize<8>::utype>();
#ifdef INT128_AVAILABLE
    verifyNextPermutation<regina::IntOfSize<16>::utype>();
#endif
}
