
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "testsuite/maths/testmaths.h"
#include "maths/primes.h"

using regina::Integer;
using regina::Primes;

class PrimesTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(PrimesTest);

    CPPUNIT_TEST(autoGrow);
    CPPUNIT_TEST(primes);
    CPPUNIT_TEST(primeDecomp);
    CPPUNIT_TEST(primePowerDecomp);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void testSize(long size, const char* list) {
            long ans = Primes::size();

            // We can't test the size exactly, since the size may be
            // larger than expected if other parts of Regina have
            // already been run (in particular, if the test suite is
            // being run for a second time in the same process).

            if (ans < size) {
                std::ostringstream msg;
                msg << list << " should have size >= " << size
                    << ", not " << ans << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }

        void autoGrow() {
            testSize(10000, "The initial list of seed primes");
            Primes::prime(10005, false);
            testSize(10000, "The unexpanded list of primes");
            Primes::prime(10005, true);
            testSize(10006, "The expanded list of primes");
        }

        void testPrime(long which, long prime) {
            Integer ans = Primes::prime(which);
            if (ans != prime) {
                std::ostringstream msg;
                msg << "Prime #" << which << " should be "
                    << prime << ", not " << ans << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }

        void primes() {
            testPrime(0, 2);
            testPrime(1, 3);
            testPrime(2, 5);
            testPrime(3, 7);
            testPrime(4, 11);
            testPrime(9998, 104723);
            testPrime(9999, 104729);
            testPrime(10000, 104743);
            testPrime(10001, 104759);
            testPrime(10002, 104761);
            testPrime(10003, 104773);
            testPrime(10004, 104779);
            testPrime(10005, 104789);
        }

        void testDecomp(long n, long p0 = -2, long p1 = -2, long p2 = -2,
                long p3 = -2, long p4 = -2) {
            unsigned expectedLen;
            if (p0 < -1)
                expectedLen = 0;
            else if (p1 < -1)
                expectedLen = 1;
            else if (p2 < -1)
                expectedLen = 2;
            else if (p3 < -1)
                expectedLen = 3;
            else if (p4 < -1)
                expectedLen = 4;
            else
                expectedLen = 5;

            std::vector<Integer> factors = Primes::primeDecomp(n);

            if (factors.size() != expectedLen) {
                std::ostringstream msg;
                msg << "Integer " << n << " should have " << expectedLen
                    << " factor(s), not " << factors.size() << '.';
                CPPUNIT_FAIL(msg.str());
            }

            int expectedFactor[5];
            expectedFactor[0] = p0;
            expectedFactor[1] = p1;
            expectedFactor[2] = p2;
            expectedFactor[3] = p3;
            expectedFactor[4] = p4;

            for (unsigned i = 0; i < expectedLen; i++) {
                if (factors[i] != expectedFactor[i]) {
                    std::ostringstream msg;
                    msg << "Factor #" << i << " of " << n
                        << " should be " << expectedFactor[i]
                        << ", not " << factors[i] << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void primeDecomp() {
            // Special cases.
            testDecomp(0, 0);
            testDecomp(1);
            testDecomp(-1, -1);

            // Prime cases.
            testDecomp(2, 2);
            testDecomp(-2, -1, 2);
            testDecomp(7, 7);
            testDecomp(-7, -1, 7);

            // Prime power cases.
            testDecomp(8, 2, 2, 2);
            testDecomp(-8, -1, 2, 2, 2);
            testDecomp(49, 7, 7);
            testDecomp(-49, -1, 7, 7);

            // Composite with distinct primes.
            testDecomp(6, 2, 3);
            testDecomp(-6, -1, 2, 3);

            // Composite with repeated primes.
            testDecomp(12, 2, 2, 3);
            testDecomp(-12, -1, 2, 2, 3);
            testDecomp(225, 3, 3, 5, 5);
            testDecomp(-225, -1, 3, 3, 5, 5);
            testDecomp(54, 2, 3, 3, 3);
            testDecomp(-54, -1, 2, 3, 3, 3);
        }

        void testPowerDecomp(long n, long p0 = -2, long e0 = 0,
                long p1 = -2, long e1 = 0, long p2 = -2, long e2 = 0) {
            unsigned expectedLen;
            if (p0 < -1)
                expectedLen = 0;
            else if (p1 < -1)
                expectedLen = 1;
            else if (p2 < -1)
                expectedLen = 2;
            else
                expectedLen = 3;

            std::vector<std::pair<Integer, unsigned long> > powers =
                Primes::primePowerDecomp(n);

            if (powers.size() != expectedLen) {
                std::ostringstream msg;
                msg << "Integer " << n << " should have " << expectedLen
                    << " prime power(s), not " << powers.size() << '.';
                CPPUNIT_FAIL(msg.str());
            }

            int expectedPrime[3];
            unsigned expectedExp[3];
            expectedPrime[0] = p0; expectedExp[0] = e0;
            expectedPrime[1] = p1; expectedExp[1] = e1;
            expectedPrime[2] = p2; expectedExp[2] = e2;

            for (unsigned i = 0; i < expectedLen; i++) {
                if (powers[i].first != expectedPrime[i]) {
                    std::ostringstream msg;
                    msg << "Prime power #" << i << " of " << n
                        << " should have base " << expectedPrime[i]
                        << ", not " << powers[i].first << '.';
                    CPPUNIT_FAIL(msg.str());
                }
                if (powers[i].second != expectedExp[i]) {
                    std::ostringstream msg;
                    msg << "Prime power #" << i << " of " << n
                        << " should have exponent " << expectedExp[i]
                        << ", not " << powers[i].second << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void primePowerDecomp() {
            // Special cases.
            testPowerDecomp(0, 0, 1);
            testPowerDecomp(1);
            testPowerDecomp(-1, -1, 1);

            // Prime cases.
            testPowerDecomp(2, 2, 1);
            testPowerDecomp(-2, -1, 1, 2, 1);
            testPowerDecomp(7, 7, 1);
            testPowerDecomp(-7, -1, 1, 7, 1);

            // Prime power cases.
            testPowerDecomp(8, 2, 3);
            testPowerDecomp(-8, -1, 1, 2, 3);
            testPowerDecomp(49, 7, 2);
            testPowerDecomp(-49, -1, 1, 7, 2);

            // Composite with distinct primes.
            testPowerDecomp(6, 2, 1, 3, 1);
            testPowerDecomp(-6, -1, 1, 2, 1, 3, 1);

            // Composite with repeated primes.
            testPowerDecomp(12, 2, 2, 3, 1);
            testPowerDecomp(-12, -1, 1, 2, 2, 3, 1);
            testPowerDecomp(225, 3, 2, 5, 2);
            testPowerDecomp(-225, -1, 1, 3, 2, 5, 2);
            testPowerDecomp(54, 2, 1, 3, 3);
            testPowerDecomp(-54, -1, 1, 2, 1, 3, 3);
        }
};

void addPrimes(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(PrimesTest::suite());
}

