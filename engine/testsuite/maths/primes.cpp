
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

#include "maths/primes.h"

#include "testhelper.h"

using regina::Primes;

TEST(PrimesTest, autoGrow) {
    // We can't test the size exactly, since the size may be
    // larger than expected if other parts of Regina have
    // already been run (in particular, if the test suite is
    // being run for a second time in the same process).

    size_t initSize = Primes::size();
    EXPECT_GE(initSize, 10000); // The initial list of seed primes

    Primes::prime(10005, false); // Do not auto-grow the list of primes
    EXPECT_EQ(Primes::size(), initSize);

    Primes::prime(10005, true); // Auto-grow the list as required.
    EXPECT_GE(Primes::size(), 10006);
}

TEST(PrimesTest, primes) {
    EXPECT_EQ(Primes::prime(0), 2);
    EXPECT_EQ(Primes::prime(1), 3);
    EXPECT_EQ(Primes::prime(2), 5);
    EXPECT_EQ(Primes::prime(3), 7);
    EXPECT_EQ(Primes::prime(4), 11);
    EXPECT_EQ(Primes::prime(9998), 104723);
    EXPECT_EQ(Primes::prime(9999), 104729);
    EXPECT_EQ(Primes::prime(10000), 104743);
    EXPECT_EQ(Primes::prime(10001), 104759);
    EXPECT_EQ(Primes::prime(10002), 104761);
    EXPECT_EQ(Primes::prime(10003), 104773);
    EXPECT_EQ(Primes::prime(10004), 104779);
    EXPECT_EQ(Primes::prime(10005), 104789);
}

static void verifyDecomp(long n, std::initializer_list<long> expect) {
    SCOPED_TRACE_NUMERIC(n);
    auto factors = Primes::primeDecomp(n);
    EXPECT_TRUE(std::equal(factors.begin(), factors.end(),
        expect.begin(), expect.end()));
}

TEST(PrimesTest, primeDecomp) {
    // Special cases.
    verifyDecomp(0, { 0 });
    verifyDecomp(1, { });
    verifyDecomp(-1, { -1 });

    // Prime cases.
    verifyDecomp(2, { 2 });
    verifyDecomp(-2, { -1, 2 });
    verifyDecomp(7, { 7 });
    verifyDecomp(-7, { -1, 7 });

    // Prime power cases.
    verifyDecomp(8, { 2, 2, 2 });
    verifyDecomp(-8, { -1, 2, 2, 2 });
    verifyDecomp(49, { 7, 7 });
    verifyDecomp(-49, { -1, 7, 7 });

    // Composite with distinct primes.
    verifyDecomp(6, { 2, 3 });
    verifyDecomp(-6, { -1, 2, 3 });

    // Composite with repeated primes.
    verifyDecomp(12, { 2, 2, 3 });
    verifyDecomp(-12, { -1, 2, 2, 3 });
    verifyDecomp(225, { 3, 3, 5, 5 });
    verifyDecomp(-225, { -1, 3, 3, 5, 5 });
    verifyDecomp(54, { 2, 3, 3, 3 });
    verifyDecomp(-54, { -1, 2, 3, 3, 3 });
}

static void verifyPowerDecomp(long n, std::initializer_list<
        std::pair<regina::Integer, unsigned long>> expect) {
    SCOPED_TRACE_NUMERIC(n);
    auto powers = Primes::primePowerDecomp(n);
    EXPECT_TRUE(std::equal(powers.begin(), powers.end(),
        expect.begin(), expect.end()));
}

TEST(PrimesTest, primePowerDecomp) {
    // Special cases.
    verifyPowerDecomp(0, { {0,1} });
    verifyPowerDecomp(1, { });
    verifyPowerDecomp(-1, { {-1,1} });

    // Prime cases.
    verifyPowerDecomp(2, { {2,1} });
    verifyPowerDecomp(-2, { {-1,1}, {2,1} });
    verifyPowerDecomp(7, { {7,1} });
    verifyPowerDecomp(-7, { {-1,1}, {7,1} });

    // Prime power cases.
    verifyPowerDecomp(8, { {2,3} });
    verifyPowerDecomp(-8, { {-1,1}, {2,3} });
    verifyPowerDecomp(49, { {7,2} });
    verifyPowerDecomp(-49, { {-1,1}, {7,2} });

    // Composite with distinct primes.
    verifyPowerDecomp(6, { {2,1}, {3,1} });
    verifyPowerDecomp(-6, { {-1,1}, {2,1}, {3,1} });

    // Composite with repeated primes.
    verifyPowerDecomp(12, { {2,2}, {3,1} });
    verifyPowerDecomp(-12, { {-1,1}, {2,2}, {3,1} });
    verifyPowerDecomp(225, { {3,2}, {5,2} });
    verifyPowerDecomp(-225, { {-1,1}, {3,2}, {5,2} });
    verifyPowerDecomp(54, { {2,1}, {3,3} });
    verifyPowerDecomp(-54, { {-1,1}, {2,1}, {3,3} });
}
