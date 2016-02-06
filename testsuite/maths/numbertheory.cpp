
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
#include "maths/numbertheory.h"
#include "testsuite/maths/testmaths.h"

class NumberTheoryTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NumberTheoryTest);

    CPPUNIT_TEST(reducedMod);
    CPPUNIT_TEST(gcd);
    CPPUNIT_TEST(gcdWithCoeffs);
    CPPUNIT_TEST(modularInverse);

    CPPUNIT_TEST_SUITE_END();

    private:
        long lEvenLarge;
            /**< Large even number; twice this fits in a signed long. */
        long lOddLarge;
            /**< Large odd number; twice this fits in a signed long. */
        long lEvenMed;
            /**< Medium even number; this squared fits in a signed long. */
        long lOddMed;
            /**< Medium odd number; this squared fits in a signed long. */
        long lEvenSmall;
            /**< Small even number; this cubed fits in a signed long. */
        long lOddSmall;
            /**< Small odd number; this cubed fits in a signed long. */
        long lEvenTiny;
            /**< Tiny even number; this to the fourth fits in a signed long. */
        long lOddTiny;
            /**< Tiny odd number; this to the fourth fits in a signed long. */
        unsigned long ulEvenLarge;
            /**< Large even number; twice this fits in a signed long. */
        unsigned long ulOddLarge;
            /**< Large odd number; twice this fits in a signed long. */
        unsigned long ulEvenMed;
            /**< Medium even number; this squared fits in a signed long. */
        unsigned long ulOddMed;
            /**< Medium odd number; this squared fits in a signed long. */
        unsigned long ulEvenSmall;
            /**< Small even number; this cubed fits in a signed long. */
        unsigned long ulOddSmall;
            /**< Small odd number; this cubed fits in a signed long. */
        unsigned long ulEvenTiny;
            /**< Tiny even number; this to the fourth fits in a signed long. */
        unsigned long ulOddTiny;
            /**< Tiny odd number; this to the fourth fits in a signed long. */

    public:
        void setUp() {
            ulEvenLarge = lEvenLarge = 1000000000;
            ulOddLarge = lOddLarge = 1000000001;
            ulEvenMed = lEvenMed = 40000;
            ulOddMed = lOddMed = 40001;
            ulEvenSmall = lEvenSmall = 1000;
            ulOddSmall = lOddSmall = 1001;
            ulEvenTiny = lEvenTiny = 200;
            ulOddTiny = lOddTiny = 201;
        }

        void tearDown() {
        }

        static void reducedModSpec(long k, long modBase) {
            long ans = regina::reducedMod(k, modBase);

            std::ostringstream msg;
            msg << "reducedMod(" << k << ", " << modBase
                << ") should not be " << ans << '.';

            CPPUNIT_ASSERT_MESSAGE(msg.str(), 2 * ans <= modBase);
            CPPUNIT_ASSERT_MESSAGE(msg.str(), 2 * ans > -modBase);
            CPPUNIT_ASSERT_MESSAGE(msg.str(), (k - ans) % modBase == 0);
        }

        void reducedMod() {
            // 0 mod n:
            reducedModSpec(0, 1);
            reducedModSpec(0, lEvenLarge);

            // n mod 1:
            reducedModSpec(1, 1);
            reducedModSpec(-1, 1);
            reducedModSpec(lEvenLarge, 1);
            reducedModSpec(-lEvenLarge, 1);

            // Halfway tests:
            reducedModSpec(16, 2);
            reducedModSpec(17, 2);
            reducedModSpec(-16, 2);
            reducedModSpec(-17, 2);
            reducedModSpec(16, 3);
            reducedModSpec(17, 3);
            reducedModSpec(-16, 3);
            reducedModSpec(-17, 3);

            long lEvenMod = lEvenMed * 2;
            long lEvenHalf = (lEvenMed * lEvenMed) - lEvenMed;
            long lOddMod = lOddMed;
            long lOddBelowHalf = (lOddMed * lOddMed) - ((lOddMed + 1) / 2);
            reducedModSpec(lEvenHalf - 1, lEvenMod);
            reducedModSpec(lEvenHalf, lEvenMod);
            reducedModSpec(lEvenHalf + 1, lEvenMod);
            reducedModSpec(-(lEvenHalf - 1), lEvenMod);
            reducedModSpec(-lEvenHalf, lEvenMod);
            reducedModSpec(-(lEvenHalf + 1), lEvenMod);
            reducedModSpec(lOddBelowHalf, lOddMod);
            reducedModSpec(lOddBelowHalf + 1, lOddMod);
            reducedModSpec(-lOddBelowHalf, lOddMod);
            reducedModSpec(-(lOddBelowHalf + 1), lOddMod);

            // Examples from documentation:
            reducedModSpec(4, 10);
            reducedModSpec(6, 10);
        }

        void gcd() {
            // Boundary cases
            CPPUNIT_ASSERT_EQUAL((long)0, regina::gcd(0, 0));
            CPPUNIT_ASSERT_EQUAL(lOddLarge, regina::gcd(0, lOddLarge));
            CPPUNIT_ASSERT_EQUAL(lOddLarge, regina::gcd(0, -lOddLarge));
            CPPUNIT_ASSERT_EQUAL(lOddLarge, regina::gcd(lOddLarge, 0));
            CPPUNIT_ASSERT_EQUAL(lOddLarge, regina::gcd(-lOddLarge, 0));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::gcd(1, lOddLarge));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::gcd(1, -lOddLarge));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::gcd(-1, lOddLarge));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::gcd(-1, -lOddLarge));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::gcd(lOddLarge, 1));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::gcd(-lOddLarge, 1));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::gcd(lOddLarge, -1));
            CPPUNIT_ASSERT_EQUAL((long)1, regina::gcd(-lOddLarge, -1));

            // Equality / multiple of
            CPPUNIT_ASSERT_EQUAL(lEvenMed, regina::gcd(lEvenMed, lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed, regina::gcd(-lEvenMed, lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed, regina::gcd(lEvenMed, -lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed, regina::gcd(-lEvenMed, -lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed,
                regina::gcd(lEvenMed, lEvenMed * lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed,
                regina::gcd(-lEvenMed, lEvenMed * lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed,
                regina::gcd(lEvenMed, -lEvenMed * lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed,
                regina::gcd(-lEvenMed, -lEvenMed * lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed,
                regina::gcd(lEvenMed * lEvenMed, lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed,
                regina::gcd(-lEvenMed * lEvenMed, lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed,
                regina::gcd(lEvenMed * lEvenMed, -lEvenMed));
            CPPUNIT_ASSERT_EQUAL(lEvenMed,
                regina::gcd(-lEvenMed * lEvenMed, -lEvenMed));

            // Miscellaneous cases.
            CPPUNIT_ASSERT_EQUAL((long)12, regina::gcd(96, 324));
            CPPUNIT_ASSERT_EQUAL((long)12, regina::gcd(96, -324));
            CPPUNIT_ASSERT_EQUAL((long)12, regina::gcd(-96, 324));
            CPPUNIT_ASSERT_EQUAL((long)12, regina::gcd(-96, -324));
            CPPUNIT_ASSERT_EQUAL((long)12, regina::gcd(324, 96));
            CPPUNIT_ASSERT_EQUAL((long)12, regina::gcd(324, -96));
            CPPUNIT_ASSERT_EQUAL((long)12, regina::gcd(-324, 96));
            CPPUNIT_ASSERT_EQUAL((long)12, regina::gcd(-324, -96));
            CPPUNIT_ASSERT_EQUAL(lEvenMed, regina::gcd(
                lEvenMed * (lEvenMed - 3),
                lEvenMed * (lEvenMed - 1)));
            CPPUNIT_ASSERT_EQUAL(lEvenMed, regina::gcd(
                lEvenMed * (lEvenMed - 3),
                -lEvenMed * (lEvenMed - 1)));
            CPPUNIT_ASSERT_EQUAL(lEvenMed, regina::gcd(
                -lEvenMed * (lEvenMed - 3),
                lEvenMed * (lEvenMed - 1)));
            CPPUNIT_ASSERT_EQUAL(lEvenMed, regina::gcd(
                -lEvenMed * (lEvenMed - 3),
                -lEvenMed * (lEvenMed - 1)));
            CPPUNIT_ASSERT_EQUAL(lEvenMed * 2, regina::gcd(
                lEvenMed * (lEvenMed - 4),
                lEvenMed * (lEvenMed - 2)));
            CPPUNIT_ASSERT_EQUAL(lEvenMed * 2, regina::gcd(
                lEvenMed * (lEvenMed - 4),
                -lEvenMed * (lEvenMed - 2)));
            CPPUNIT_ASSERT_EQUAL(lEvenMed * 2, regina::gcd(
                -lEvenMed * (lEvenMed - 4),
                lEvenMed * (lEvenMed - 2)));
            CPPUNIT_ASSERT_EQUAL(lEvenMed * 2, regina::gcd(
                -lEvenMed * (lEvenMed - 4),
                -lEvenMed * (lEvenMed - 2)));
        }

        static void gcdWithCoeffsSpec(long a, long b) {
            long u, v;
            long d = regina::gcdWithCoeffs(a, b, u, v);

            std::ostringstream msg;
            msg << "gcdWithCoeffs(" << a << ", " << b
                << ") should not give (d, u, v) = (" << d << ", "
                << u << ", " << v << ").";

            // Common divisor.
            if (d) {
                // Returned gcd is non-zero.
                CPPUNIT_ASSERT_MESSAGE(msg.str(), a % d == 0);
                CPPUNIT_ASSERT_MESSAGE(msg.str(), b % d == 0);
            } else {
                // Returned gcd is zero.
                CPPUNIT_ASSERT_MESSAGE(msg.str(), a == 0 && b == 0);
            }

            // Linear combination condition.
            CPPUNIT_ASSERT_MESSAGE(msg.str(), u * a + v * b == d);

            // Range conditions.
            CPPUNIT_ASSERT_MESSAGE(msg.str(), d >= 0);

            if (a && b) {
                long aMult = (a >= 0 ? a / d : (-a) / d);
                long bMult = (b >= 0 ? b / d : (-b) / d);
                long uSigned = (a >= 0 ? u : -u);
                long vSigned = (b >= 0 ? v : -v);
                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    -aMult < vSigned && vSigned <= 0);
                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    1 <= uSigned && uSigned <= bMult);
            } else {
                if (a == 0 && b != 0)
                    CPPUNIT_ASSERT_MESSAGE(msg.str(), u == 0 &&
                        (b < 0 && v == -1 || b > 0 && v == 1));
                else if (a != 0 && b == 0)
                    CPPUNIT_ASSERT_MESSAGE(msg.str(), v == 0 &&
                        (a < 0 && u == -1 || a > 0 && u == 1));
                else
                    CPPUNIT_ASSERT_MESSAGE(msg.str(), u == 0 && v == 0);
            }
        }

        static void gcdWithCoeffsSpecAllCombs(long a, long b) {
            gcdWithCoeffsSpec(a, b);
            gcdWithCoeffsSpec(a, -b);
            gcdWithCoeffsSpec(-a, b);
            gcdWithCoeffsSpec(-a, -b);

            gcdWithCoeffsSpec(b, a);
            gcdWithCoeffsSpec(b, -a);
            gcdWithCoeffsSpec(-b, a);
            gcdWithCoeffsSpec(-b, -a);
        }

        void gcdWithCoeffs() {
            // Small cases.
            gcdWithCoeffsSpecAllCombs(0, 0);
            gcdWithCoeffsSpecAllCombs(0, 1);
            gcdWithCoeffsSpecAllCombs(0, lEvenMed);
            gcdWithCoeffsSpecAllCombs(1, lEvenMed);

            // Equal / multiple of.
            gcdWithCoeffsSpecAllCombs(lEvenMed, lEvenMed);
            gcdWithCoeffsSpecAllCombs(lEvenSmall, lEvenSmall * lEvenSmall);
            gcdWithCoeffsSpecAllCombs(lEvenSmall, lEvenSmall * (lEvenSmall - 1));

            // Large cases.
            gcdWithCoeffsSpecAllCombs(lEvenSmall * 3, lEvenSmall * lEvenSmall);
            gcdWithCoeffsSpecAllCombs(lEvenSmall * 3,
                lEvenSmall * (lEvenSmall - 1));
            gcdWithCoeffsSpecAllCombs(ulEvenTiny * (ulEvenTiny - 3),
                ulEvenTiny * (ulEvenTiny - 1));
            gcdWithCoeffsSpecAllCombs(ulEvenTiny * (ulEvenTiny - 4),
                ulEvenTiny * (ulEvenTiny - 2));

            // Miscellaneous cases.
            gcdWithCoeffsSpecAllCombs(96, 324);
        }

        static void modularInverseSpec(unsigned long n, unsigned long k) {
            unsigned long ans = regina::modularInverse(n, k);

            std::ostringstream msg;
            msg << "modularInverse(" << n << ", " << k
                << ") should not be " << ans << '.';

            CPPUNIT_ASSERT_MESSAGE(msg.str(), ans >= 0);
            CPPUNIT_ASSERT_MESSAGE(msg.str(), ans < n);
            CPPUNIT_ASSERT_MESSAGE(msg.str(), (ans * (k % n) - 1) % n == 0);
        }

        void modularInverse() {
            // Small cases
            modularInverseSpec(1, 1);
            modularInverseSpec(1, ulEvenMed);
            modularInverseSpec(2, 1);
            modularInverseSpec(2, ulOddMed);

            // Boundary cases
            modularInverseSpec(ulOddMed, 1);
            modularInverseSpec(ulEvenMed, 1);
            modularInverseSpec(ulOddMed, ulOddMed - 1);
            modularInverseSpec(ulEvenMed, ulEvenMed - 1);
            modularInverseSpec(ulOddMed, ulOddMed + 1);
            modularInverseSpec(ulEvenMed, ulEvenMed + 1);

            // All cases for a particular modular base.
            for (unsigned long k = 1; k < ulOddMed; k++) {
                if (regina::gcd(k, ulOddMed) != 1)
                    continue;

                // Element to invert within standard range.
                modularInverseSpec(ulOddMed, k);

                // Element to invert not within standard range.
                modularInverseSpec(ulOddMed, (ulOddMed * (ulOddMed - 1)) + k);
            }
        }
};

void addNumberTheory(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NumberTheoryTest::suite());
}

