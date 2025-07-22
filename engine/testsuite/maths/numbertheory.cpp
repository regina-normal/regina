
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <limits.h>
#include <numeric> // for std::gcd()
#include "maths/numbertheory.h"

#include "testhelper.h"

// A positive odd integer whose square is a bit less than LONG_MAX:
static constexpr long halfSizeOdd =
    (long(1) << (sizeof(long) * 4 - 4)) * 11 + 5;

// An integer type that we can use to safely multiply longs.
using DoubleSize = regina::IntOfSize<sizeof(long) * 2>::type;

static void verifyReducedMod(long k, long modBase) {
    SCOPED_TRACE_NUMERIC(k);
    SCOPED_TRACE_NUMERIC(modBase);

    long ans = regina::reducedMod(k, modBase);

    // These tests are written a little awkwardly; the reason is to ensure
    // they do the right thing even when pushing up against LONG_MIN / LONG_MAX.

    if (ans >= 0)
        EXPECT_LE(ans, modBase - ans);
    else
        EXPECT_GT(ans, -(modBase + ans));

    if (k >= 0) {
        if (ans >= 0)
            EXPECT_EQ((k - ans) % modBase, 0);
        else
            EXPECT_EQ(((k - modBase) - ans) % modBase, 0);
    } else {
        if (ans >= 0)
            EXPECT_EQ(((k + modBase) - ans) % modBase, 0);
        else
            EXPECT_EQ((k - ans) % modBase, 0);
    }
}

TEST(NumberTheoryTest, reducedMod) {
    // 0 mod n:
    verifyReducedMod(0, 1);
    verifyReducedMod(0, 1000000000);
    verifyReducedMod(0, LONG_MAX);

    // n mod 1:
    verifyReducedMod(1, 1);
    verifyReducedMod(-1, 1);
    verifyReducedMod(1000000000, 1);
    verifyReducedMod(-1000000000, 1);
    verifyReducedMod(LONG_MAX, 1);
    verifyReducedMod(LONG_MIN, 1);

    // Extreme cases:
    verifyReducedMod(LONG_MAX - 1, LONG_MAX);
    verifyReducedMod(LONG_MAX, LONG_MAX - 1);
    verifyReducedMod(LONG_MIN, LONG_MAX);

    // Halfway tests:
    verifyReducedMod(16, 2);
    verifyReducedMod(17, 2);
    verifyReducedMod(-16, 2);
    verifyReducedMod(-17, 2);
    verifyReducedMod(16, 3);
    verifyReducedMod(17, 3);
    verifyReducedMod(-16, 3);
    verifyReducedMod(-17, 3);
    verifyReducedMod(LONG_MAX / 2, LONG_MAX);
    verifyReducedMod(LONG_MAX / 2 + 1, LONG_MAX);
    verifyReducedMod(-(LONG_MAX / 2), LONG_MAX);
    verifyReducedMod(-(LONG_MAX / 2 + 1), LONG_MAX);

    static constexpr long evenMod = 40000 * 2;
    static constexpr long evenHalf = (40000 * 40000) - 40000;
    static constexpr long oddMod = 40001;
    static constexpr long oddBelowHalf = (40001 * 40001) - ((40001 + 1) / 2);
    verifyReducedMod(evenHalf - 1, evenMod);
    verifyReducedMod(evenHalf, evenMod);
    verifyReducedMod(evenHalf + 1, evenMod);
    verifyReducedMod(-(evenHalf - 1), evenMod);
    verifyReducedMod(-evenHalf, evenMod);
    verifyReducedMod(-(evenHalf + 1), evenMod);
    verifyReducedMod(oddBelowHalf, oddMod);
    verifyReducedMod(oddBelowHalf + 1, oddMod);
    verifyReducedMod(-oddBelowHalf, oddMod);
    verifyReducedMod(-(oddBelowHalf + 1), oddMod);

    // Examples from documentation:
    verifyReducedMod(4, 10);
    verifyReducedMod(6, 10);

    // Invalid cases:
    EXPECT_THROW({ regina::reducedMod(0, 0); }, regina::InvalidArgument);
    EXPECT_THROW({ regina::reducedMod(3, 0); }, regina::InvalidArgument);
    EXPECT_THROW({ regina::reducedMod(3, -7); }, regina::InvalidArgument);
    EXPECT_THROW({ regina::reducedMod(3, LONG_MIN); }, regina::InvalidArgument);
    EXPECT_THROW({ regina::reducedMod(LONG_MAX, LONG_MIN); },
        regina::InvalidArgument);
}

static void verifyGcdWithCoeffs(long a, long b, long gcd) {
    SCOPED_TRACE_NUMERIC(a);
    SCOPED_TRACE_NUMERIC(b);

    auto [d, u, v] = regina::gcdWithCoeffs(a, b);

    EXPECT_EQ(d, gcd);
    EXPECT_GE(d, 0);
    EXPECT_EQ(static_cast<DoubleSize>(u) * a + static_cast<DoubleSize>(v) * b,
        static_cast<DoubleSize>(d));

    if (a == 0 && b == 0) {
        EXPECT_EQ(d, 0);
        EXPECT_EQ(u, 0);
        EXPECT_EQ(v, 0);
    } else if (a == 0) {
        EXPECT_EQ(d, std::abs(b));
        EXPECT_EQ(u, 0);
        EXPECT_EQ(v, (b < 0 ? -1 : 1));
    } else if (b == 0) {
        EXPECT_EQ(d, std::abs(a));
        EXPECT_EQ(u, (a < 0 ? -1 : 1));
        EXPECT_EQ(v, 0);
    } else {
        ASSERT_NE(d, 0);

        EXPECT_EQ(a % d, 0);
        EXPECT_EQ(b % d, 0);

        long aMult = (a >= 0 ? a / d : (-a) / d);
        long bMult = (b >= 0 ? b / d : (-b) / d);
        long uSigned = (a >= 0 ? u : -u);
        long vSigned = (b >= 0 ? v : -v);
        EXPECT_LT(-aMult, vSigned);
        EXPECT_LE(vSigned, 0);
        EXPECT_LE(1, uSigned);
        EXPECT_LE(uSigned, bMult);
    }

    // While we're here, verify that std::gcd() does the right thing also.
    EXPECT_EQ(std::gcd(a, b), gcd);
}

static void verifyGcdWithCoeffsAllCombs(long a, long b, long gcd) {
    verifyGcdWithCoeffs(a, b, gcd);
    verifyGcdWithCoeffs(a, -b, gcd);
    verifyGcdWithCoeffs(-a, b, gcd);
    verifyGcdWithCoeffs(-a, -b, gcd);

    verifyGcdWithCoeffs(b, a, gcd);
    verifyGcdWithCoeffs(b, -a, gcd);
    verifyGcdWithCoeffs(-b, a, gcd);
    verifyGcdWithCoeffs(-b, -a, gcd);
}

TEST(NumberTheoryTest, gcdWithCoeffs) {
    // All of these cases are designed so that the arguments fit within
    // 32-bit integers.

    // Small cases:
    verifyGcdWithCoeffsAllCombs(0, 0, 0);
    verifyGcdWithCoeffsAllCombs(0, 1, 1);
    verifyGcdWithCoeffsAllCombs(0, 40000, 40000);
    verifyGcdWithCoeffsAllCombs(0, 1000000001, 1000000001);
    verifyGcdWithCoeffsAllCombs(0, LONG_MAX, LONG_MAX);
    verifyGcdWithCoeffsAllCombs(1, 40000, 1);
    verifyGcdWithCoeffsAllCombs(1, 1000000001, 1);
    verifyGcdWithCoeffsAllCombs(1, LONG_MAX, 1);

    // Equal / multiple of:
    verifyGcdWithCoeffsAllCombs(1000, 1000 * 999, 1000);
    verifyGcdWithCoeffsAllCombs(1000, 1000 * 1000, 1000);
    verifyGcdWithCoeffsAllCombs(40000, 40000, 40000);
    verifyGcdWithCoeffsAllCombs(40000, 40000 * 40000, 40000);
    verifyGcdWithCoeffsAllCombs(halfSizeOdd, halfSizeOdd, halfSizeOdd);
    verifyGcdWithCoeffsAllCombs(halfSizeOdd, halfSizeOdd * (halfSizeOdd - 1),
        halfSizeOdd);
    verifyGcdWithCoeffsAllCombs(halfSizeOdd, halfSizeOdd * halfSizeOdd,
        halfSizeOdd);

    // Large cases:
    verifyGcdWithCoeffsAllCombs(200 * 197, 200 * 199, 200);
    verifyGcdWithCoeffsAllCombs(200 * 196, 200 * 198, 200 * 2);
    verifyGcdWithCoeffsAllCombs(1000 * 3, 1000 * 1000, 1000);
    verifyGcdWithCoeffsAllCombs(1000 * 3, 1000 * 999, 1000 * 3);
    verifyGcdWithCoeffsAllCombs(40000 * 39997, 40000 * 39999, 40000);
    verifyGcdWithCoeffsAllCombs(40000 * 39996, 40000 * 39998, 40000 * 2);
    verifyGcdWithCoeffsAllCombs(halfSizeOdd * (halfSizeOdd - 3),
        halfSizeOdd * (halfSizeOdd - 1), halfSizeOdd * 2);
    verifyGcdWithCoeffsAllCombs(halfSizeOdd * (halfSizeOdd - 4),
        halfSizeOdd * (halfSizeOdd - 2), halfSizeOdd);

    // Miscellaneous cases:
    verifyGcdWithCoeffsAllCombs(96, 324, 12);

    // Extreme cases:
    verifyGcdWithCoeffsAllCombs(LONG_MAX / 2, LONG_MAX, 1);
    verifyGcdWithCoeffsAllCombs(LONG_MAX - 1, LONG_MAX, 1);
    verifyGcdWithCoeffsAllCombs(LONG_MAX, LONG_MAX, LONG_MAX);
}

static void verifyModularInverse(long n, long k) {
    SCOPED_TRACE_NUMERIC(n);
    SCOPED_TRACE_NUMERIC(k);

    long ans = regina::modularInverse(n, k);

    EXPECT_GE(ans, 0);
    EXPECT_LT(ans, n);
    EXPECT_EQ((static_cast<DoubleSize>(ans) * (k % n) - 1) % n, 0);
}

static void verifyModularInverseAllCombs(long n, long k) {
    verifyModularInverse(n, k);
    verifyModularInverse(n, -k);
}

static void verifyModularInverseExhaustive(long n) {
    SCOPED_TRACE_NUMERIC(n);

    for (long k = 1; k < n; ++k) {
        if (std::gcd(k, n) != 1)
            continue;
        SCOPED_TRACE_NUMERIC(k);

        // Element to invert within standard range.
        long ans = regina::modularInverse(n, k);
        EXPECT_GE(ans, 0);
        EXPECT_LT(ans, n);
        EXPECT_EQ((static_cast<DoubleSize>(ans) * k - 1) % n, 0);

        // Element to invert not within standard range.
        DoubleSize large = static_cast<DoubleSize>(n) * (n - 1) + k;
        if (large <= LONG_MAX)
            EXPECT_EQ(regina::modularInverse(n, static_cast<long>(large)), ans);
    }
}

TEST(NumberTheoryTest, modularInverse) {
    // Small cases:
    verifyModularInverseAllCombs(1, 0);
    verifyModularInverseAllCombs(1, 1);
    verifyModularInverseAllCombs(1, 40000);
    verifyModularInverseAllCombs(2, 1);
    verifyModularInverseAllCombs(2, 40001);

    // Boundary cases:
    verifyModularInverseAllCombs(40000, 1);
    verifyModularInverseAllCombs(40000, 39999);
    verifyModularInverseAllCombs(40000, 40001);
    verifyModularInverseAllCombs(40001, 1);
    verifyModularInverseAllCombs(40001, 40000);
    verifyModularInverseAllCombs(40001, 40002);

    // All cases for a particular modular base:
    verifyModularInverseExhaustive(40000);
    verifyModularInverseExhaustive(40001);

    // Extreme cases:
    verifyModularInverseAllCombs(2, LONG_MAX);
    verifyModularInverseAllCombs(LONG_MAX - 2, LONG_MAX);
    verifyModularInverseAllCombs(LONG_MAX / 2, LONG_MAX);

    // Invalid cases:
    EXPECT_THROW({ regina::modularInverse(0, 0); }, regina::InvalidArgument);
    EXPECT_THROW({ regina::modularInverse(0, 1); }, regina::InvalidArgument);
    EXPECT_THROW({ regina::modularInverse(0, 2); }, regina::InvalidArgument);
    EXPECT_THROW({ regina::modularInverse(2, 0); }, regina::InvalidArgument);
    EXPECT_THROW({ regina::modularInverse(2, 2); }, regina::InvalidArgument);
    EXPECT_THROW({ regina::modularInverse(2, 10); }, regina::InvalidArgument);
    EXPECT_THROW({ regina::modularInverse(101 * 7, 101 * 5); },
        regina::InvalidArgument);
    EXPECT_THROW({ regina::modularInverse(101 * 5, 101 * 7); },
        regina::InvalidArgument);
}
