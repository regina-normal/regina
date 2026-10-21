
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "utilities/intutils.h"

#include "testhelper.h"

template <regina::UnsignedCppInteger T>
void verifyBitsRequired(T n) {
    static constexpr int totalBits = sizeof(T) * 8;
    static constexpr T highBit = (T(1) << (totalBits - 1));

    int bits = regina::bitsRequired(n);
    if (n <= 1) {
        EXPECT_EQ(bits, 0);
    } else if (n > highBit) {
        EXPECT_EQ(bits, totalBits);
    } else if (n == highBit) {
        EXPECT_EQ(bits, totalBits - 1);
    } else {
        ASSERT_GT(bits, 0);
        ASSERT_LT(bits, totalBits);
        EXPECT_LE(n, T(1) << bits);
        EXPECT_GT(n, T(1) << (bits - 1));
    }
}

template <regina::SignedCppInteger T>
void verifyBitsRequired(T n) {
    if (n < 0) {
        EXPECT_EQ(regina::bitsRequired(n), 0);
    } else {
        verifyBitsRequired<regina::MakeUnsigned<T>>(n);
    }
}

template <regina::UnsignedCppInteger T>
void verifyBitsRequired() {
    for (T i = 0; i <= 20; ++i)
        verifyBitsRequired<T>(i);

    verifyBitsRequired<T>(std::numeric_limits<T>::max());

    for (int shift = 5; shift < sizeof(T); ++shift)
        for (int offset = -2; offset <= 2; ++offset)
            verifyBitsRequired<T>(static_cast<T>(T(1) << shift) + offset);
}

template <regina::SignedCppInteger T>
void verifyBitsRequired() {
    for (T i = -20; i <= 20; ++i)
        verifyBitsRequired<T>(i);

    verifyBitsRequired<T>(std::numeric_limits<T>::min());
    verifyBitsRequired<T>(std::numeric_limits<T>::max());

    for (int shift = 5; shift < sizeof(T) - 1; ++shift)
        for (int offset = -2; offset <= 2; ++offset)
            verifyBitsRequired<T>(static_cast<T>(T(1) << shift) + offset);
}

TEST(IntUtilsTest, bitsRequired) {
    verifyBitsRequired<int8_t>();
    verifyBitsRequired<int>();
    verifyBitsRequired<long>();
    verifyBitsRequired<long long>();
    verifyBitsRequired<ssize_t>();

    verifyBitsRequired<uint8_t>();
    verifyBitsRequired<unsigned>();
    verifyBitsRequired<unsigned long>();
    verifyBitsRequired<unsigned long long>();
    verifyBitsRequired<size_t>();

#ifdef INT128_AVAILABLE
    verifyBitsRequired<regina::Int128>();
    verifyBitsRequired<regina::UInt128>();
#endif
}

template <regina::UnsignedCppInteger T>
void verifyIsqrt(T n) {
    T isqrt;
    ASSERT_NO_THROW({ isqrt = regina::isqrt(n); });

    // Ensure that isqrt^2 will not overflow.
    ASSERT_LT(isqrt, static_cast<T>(T(1) << 4 * sizeof(T)));

    T square = isqrt * isqrt;
    EXPECT_LE(square, n); // isqrt^2 ≤ n
    EXPECT_LE(n - square, isqrt << 1); // n < (isqrt + 1)^2
}

template <regina::SignedCppInteger T>
void verifyIsqrt(T n) {
    if (n < 0) {
        EXPECT_THROW({ regina::isqrt(n); }, regina::NoSolution);
    } else {
        verifyIsqrt<regina::MakeUnsigned<T>>(n);
    }
}

template <regina::UnsignedCppInteger T>
void verifyIsqrt() {
    for (T i = 0; i <= 100; ++i)
        verifyIsqrt<T>(i);

    for (T i = 0; i <= 100; ++i)
        verifyIsqrt<T>(std::numeric_limits<T>::max() - i);

    for (int shift = 5; shift < sizeof(T); ++shift)
        for (int offset = -5; offset <= 5; ++offset)
            verifyIsqrt<T>(static_cast<T>(T(1) << shift) + offset);
}

template <regina::SignedCppInteger T>
void verifyIsqrt() {
    for (T i = -5; i <= 100; ++i)
        verifyIsqrt<T>(i);

    verifyIsqrt<T>(std::numeric_limits<T>::min());

    for (T i = 0; i <= 100; ++i)
        verifyIsqrt<T>(std::numeric_limits<T>::max() - i);

    for (int shift = 5; shift < sizeof(T) - 1; ++shift)
        for (int offset = -5; offset <= 5; ++offset)
            verifyIsqrt<T>(static_cast<T>(T(1) << shift) + offset);
}

TEST(IntUtilsTest, isqrt) {
    verifyIsqrt<int8_t>();
    verifyIsqrt<int>();
    verifyIsqrt<long>();
    verifyIsqrt<long long>();
    verifyIsqrt<ssize_t>();

    verifyIsqrt<uint8_t>();
    verifyIsqrt<unsigned>();
    verifyIsqrt<unsigned long>();
    verifyIsqrt<unsigned long long>();
    verifyIsqrt<size_t>();

#ifdef INT128_AVAILABLE
    verifyIsqrt<regina::Int128>();
    verifyIsqrt<regina::UInt128>();
#endif
}

