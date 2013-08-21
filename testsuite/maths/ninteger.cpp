
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/ninteger.h"
#include "testsuite/utilities/testutilities.h"

using regina::NIntegerBase;
using regina::NInteger;
using regina::NLargeInteger;

#define HUGE_INTEGER "12364981726394781629378461923786491874569283746672"

class NIntegerTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NIntegerTest);

    CPPUNIT_TEST(constructAssignCopyNative<NInteger>);
    CPPUNIT_TEST(constructAssignCopyNative<NLargeInteger>);
    CPPUNIT_TEST(constructAssignCopyString<NInteger>);
    CPPUNIT_TEST(constructAssignCopyString<NLargeInteger>);
    CPPUNIT_TEST(constructSpecial<NInteger>);
    CPPUNIT_TEST(constructSpecial<NLargeInteger>);
    CPPUNIT_TEST(comparisons<NInteger>);
    CPPUNIT_TEST(comparisons<NLargeInteger>);
    CPPUNIT_TEST(comparisonsInfinity);
    CPPUNIT_TEST(incDec<NInteger>);
    CPPUNIT_TEST(incDec<NLargeInteger>);
    CPPUNIT_TEST(incDecInfinity);
    CPPUNIT_TEST(divisionAlg<NInteger>);
    CPPUNIT_TEST(divisionAlg<NLargeInteger>);
    CPPUNIT_TEST(gcd<NInteger>);
    CPPUNIT_TEST(gcd<NLargeInteger>);
    CPPUNIT_TEST(lcm<NInteger>);
    CPPUNIT_TEST(lcm<NLargeInteger>);

    CPPUNIT_TEST_SUITE_END();

    private:
        template <typename IntType>
        struct Data {
            IntType smallPosSeries[6];
                /**< A sequence of positive integers that fit into a long. */
            IntType smallNegSeries[6];
                /**< A sequence of negative integers that fit into a long. */
            IntType largePosSeries[6];
                /**< A sequence of positive integers too large for a long. */
            IntType largeNegSeries[6];
                /**< A sequence of negative integers too large for a long. */

            static const unsigned nSeries = 4;
                /**< The number of sequences described above. */
            static const unsigned seriesLen = 6;
                /**< The number of integers in each of the above sequences. */
            IntType* series[4];
                /**< A list of the individual large integer sequences. */
            std::string seriesName[4];
                /**< The names of the individual large integer sequences. */

            static const unsigned nSmallSeries = 2;
                /**< The number of sequences above that fit into longs. */
            long smallPosSeriesVal[6];
                /**< The long values found in the smallPosSeries sequence. */
            long smallNegSeriesVal[6];
                /**< The long values found in the smallNegSeries sequence. */
            long* smallSeriesVal[2];
                /**< A list of the long value arrays defined above. */

            IntType zero;
                /**< Special case (native): 0 */
            IntType one;
                /**< Special case (native): 1 */
            IntType two;
                /**< Special case (native): 2 */
            IntType negOne;
                /**< Special case (native): -1 */
            IntType negTwo;
                /**< Special case (native): -2 */
            IntType longMax;
                /**< Special case (native): LONG_MAX */
            IntType longMin;
                /**< Special case (native): LONG_MIN */
            IntType longMaxInc;
                /**< Special case (large): LONG_MAX + 1 */
            IntType longMinDec;
                /**< Special case (large): LONG_MIN - 1 */
            IntType ulongMax;
                /**< Special case (large): ULONG_MAX */
            IntType hugePos;
                /**< Special case (large): a huge positive integer */
            IntType hugeNeg;
                /**< Special case (large): a huge negative integer */

            void setUp() {
                smallPosSeries[0] = 1000;
                smallPosSeries[1] = 2000;
                smallPosSeries[2] = 3000;
                smallPosSeries[3] = 4000;
                smallPosSeries[4] = 6000;
                smallPosSeries[5] = 6000000;
                smallNegSeries[0] = -1000;
                smallNegSeries[1] = -2000;
                smallNegSeries[2] = -3000;
                smallNegSeries[3] = -4000;
                smallNegSeries[4] = -6000;
                smallNegSeries[5] = -6000000;
                largePosSeries[0] = "1000000000000000"; // 10^15
                largePosSeries[1] = "2000000000000000"; // 2*10^15
                largePosSeries[2] = "3000000000000000"; // 3*10^15
                largePosSeries[3] = "4000000000000000"; // 4*10^15
                largePosSeries[4] = "6000000000000000"; // 6*10^15
                largePosSeries[5] = "6000000000000000000000000000000";// 6*10^30
                largeNegSeries[0] = "-1000000000000000"; // 10^15
                largeNegSeries[1] = "-2000000000000000"; // 2*10^15
                largeNegSeries[2] = "-3000000000000000"; // 3*10^15
                largeNegSeries[3] = "-4000000000000000"; // 4*10^15
                largeNegSeries[4] = "-6000000000000000"; // 6*10^15
                largeNegSeries[5] = "-6000000000000000000000000000000";//6*10^30

                series[0] = smallPosSeries;
                series[1] = smallNegSeries;
                series[2] = largePosSeries;
                series[3] = largeNegSeries;
                seriesName[0] = "smallPosSeries";
                seriesName[1] = "smallNegSeries";
                seriesName[2] = "largePosSeries";
                seriesName[3] = "largeNegSeries";

                smallPosSeriesVal[0] = 1000;
                smallPosSeriesVal[1] = 2000;
                smallPosSeriesVal[2] = 3000;
                smallPosSeriesVal[3] = 4000;
                smallPosSeriesVal[4] = 6000;
                smallPosSeriesVal[5] = 6000000;
                smallNegSeriesVal[0] = -1000;
                smallNegSeriesVal[1] = -2000;
                smallNegSeriesVal[2] = -3000;
                smallNegSeriesVal[3] = -4000;
                smallNegSeriesVal[4] = -6000;
                smallNegSeriesVal[5] = -6000000;

                smallSeriesVal[0] = smallPosSeriesVal;
                smallSeriesVal[1] = smallNegSeriesVal;

                zero = 0;
                one = 1;
                two = 2;
                negOne = -1;
                negTwo = -2;
                longMax = LONG_MAX;
                longMin = LONG_MIN;
                longMaxInc = LONG_MAX; ++longMaxInc;
                longMinDec = LONG_MIN; --longMinDec;
                ulongMax = (unsigned long)(ULONG_MAX);
                hugePos = HUGE_INTEGER;
                hugeNeg = "-" HUGE_INTEGER;
            }
        };

    private:
        Data<NLargeInteger> dataL;
        Data<NInteger> dataI;

        long zeroL;
            /** We need this so we can test things like (LONG_MAX + 1 < 0)
                without compiler optimisations breaking the results. */

        template <typename IntType>
        Data<IntType>& data();

    public:
        void setUp() {
            dataL.setUp();
            dataI.setUp();

            zeroL = 0;
        }

        void tearDown() {
        }

        template <typename T>
        std::string str(T x) {
            std::ostringstream ans;
            ans << x;
            return ans.str();
        }

        // TODO: Test swap()
        // TODO: Test comparisons (lazy && long)
        // TODO: Test ++, -- (both versions)
        // TODO: Test +, -, *, /, divExact, % (lazy && long); unary -
        // TODO: Test +=, -=, *=, /=, divByExact, %= (lazy && long); negate
        // TODO: Test abs, gcd, lcm
        // TODO: Test tryReduce, makeLarge.

        template <typename IntType>
        std::string eltName(int whichSeries, int whichMember) {
            std::ostringstream ans;
            ans << data<IntType>().seriesName[whichSeries]
                << '[' << whichMember << ']';
            return ans.str();
        }

        template <typename IntType>
        void shouldBeLess(const IntType& a, const IntType& b,
                const std::string& aName, const std::string& bName) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + aName + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "== " + bName + ".",
                ! (a == b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not != " + bName + ".",
                a != b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not < " + bName + ".",
                a < b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not <= " + bName + ".",
                a <= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "> " + bName + ".",
                ! (a > b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + ">= " + bName + ".",
                ! (a >= b));
        }

        template <typename IntType>
        void shouldBeEqual(const IntType& a, const IntType& b,
                const std::string& aName, const std::string& bName) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + aName + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "not == " + bName + ".",
                a == b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "!= " + bName + ".",
                ! (a != b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "< " + bName + ".",
                ! (a < b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not <= " + bName + ".",
                a <= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "> " + bName + ".",
                ! (a > b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not >= " + bName + ".",
                a >= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not str== " + bName + " (long).",
                a.stringValue() == b.stringValue());
        }

        template <typename IntType>
        void shouldBeGreater(const IntType& a, const IntType& b,
                const std::string& aName, const std::string& bName) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + aName + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "== " + bName + ".",
                ! (a == b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not != " + bName + ".",
                a != b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "< " + bName + ".",
                ! (a < b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "<= " + bName + ".",
                ! (a <= b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not > " + bName + ".",
                a > b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not >= " + bName + ".",
                a >= b);
        }

        template <typename IntType>
        void shouldBeLess(const IntType& a, long b,
                const std::string& aName, const std::string& bName) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + aName + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "== " + bName + " (long).",
                ! (a == b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not != " + bName + " (long).",
                a != b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not < " + bName + " (long).",
                a < b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not <= " + bName + " (long).",
                a <= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "> " + bName + " (long).",
                ! (a > b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + ">= " + bName + " (long).",
                ! (a >= b));
        }

        template <typename IntType>
        void shouldBeEqual(const IntType& a, long b,
                const std::string& aName, const std::string& bName) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + aName + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "not == " + bName + " (long).",
                a == b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "!= " + bName + " (long).",
                ! (a != b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "< " + bName + " (long).",
                ! (a < b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not <= " + bName + " (long).",
                a <= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "> " + bName + " (long).",
                ! (a > b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not >= " + bName + " (long).",
                a >= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not str== " + bName + " (long).",
                a.stringValue() == str(b));
        }

        template <typename IntType>
        void shouldBeGreater(const IntType& a, long b,
                const std::string& aName, const std::string& bName) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + aName + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "== " + bName + " (long).",
                ! (a == b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not != " + bName + " (long).",
                a != b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "< " + bName + " (long).",
                ! (a < b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "<= " + bName + " (long).",
                ! (a <= b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not > " + bName + " (long).",
                a > b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not >= " + bName + " (long).",
                a >= b);
        }

        void comparisonsInfinity() {
            const NLargeInteger& infinity(NLargeInteger::infinity);

            shouldBeLess(NLargeInteger::one, infinity, "one", "infinity");
            shouldBeLess(NLargeInteger::zero, infinity, "zero", "infinity");
            shouldBeGreater(infinity, NLargeInteger::one, "infinity", "one");
            shouldBeGreater(infinity, 1L, "infinity", "one");
            shouldBeGreater(infinity, NLargeInteger::zero, "infinity", "zero");
            shouldBeGreater(infinity, 0L, "infinity", "zero");
            shouldBeEqual(infinity, infinity, "infinity", "infinity");

            unsigned a, i;
            for (a = 0; a < dataL.nSeries; a++)
                for (i = 0; i < dataL.seriesLen; i++) {
                    shouldBeLess(dataL.series[a][i], infinity,
                        eltName<NLargeInteger>(a, i), "infinity");
                    shouldBeGreater(infinity, dataL.series[a][i],
                        "infinity", eltName<NLargeInteger>(a, i));
                }
        }

        template <typename IntType>
        void comparisons() {
            unsigned a, b, i, j;

            const IntType& zero(IntType::zero);
            const IntType& one(IntType::one);
            const Data<IntType>& d(data<IntType>());

            shouldBeLess(zero, one, "zero", "one");
            shouldBeLess(zero, 1L, "zero", "one");
            shouldBeGreater(one, zero, "one", "zero");
            shouldBeGreater(one, 0L, "one", "zero");
            shouldBeEqual(zero, zero, "zero", "zero");
            shouldBeEqual(zero, 0L, "zero", "zero");
            shouldBeEqual(one, one, "one", "one");
            shouldBeEqual(one, 1L, "one", "one");

            // Compare the elements of the series with zero and one.
            for (a = 0; a < d.nSeries; a++)
                for (i = 0; i < d.seriesLen; i++) {
                    if (a == 0 || a == 2) {
                        // Positive series.
                        shouldBeGreater(d.series[a][i], zero,
                            eltName<IntType>(a, i), "zero");
                        shouldBeGreater(d.series[a][i], 0L,
                            eltName<IntType>(a, i), "zero");
                        shouldBeGreater(d.series[a][i], one,
                            eltName<IntType>(a, i), "one");
                        shouldBeGreater(d.series[a][i], 1L,
                            eltName<IntType>(a, i), "one");
                        shouldBeLess(zero, d.series[a][i],
                            "zero", eltName<IntType>(a, i));
                        shouldBeLess(one, d.series[a][i],
                            "one", eltName<IntType>(a, i));
                    } else {
                        // Negative series.
                        shouldBeLess(d.series[a][i], zero,
                            eltName<IntType>(a, i), "zero");
                        shouldBeLess(d.series[a][i], 0L,
                            eltName<IntType>(a, i), "zero");
                        shouldBeLess(d.series[a][i], one,
                            eltName<IntType>(a, i), "one");
                        shouldBeLess(d.series[a][i], 1L,
                            eltName<IntType>(a, i), "one");
                        shouldBeGreater(zero, d.series[a][i],
                            "zero", eltName<IntType>(a, i));
                        shouldBeGreater(one, d.series[a][i],
                            "one", eltName<IntType>(a, i));
                    }
                }

            // Compare all elements of all series in pairs.
            int expected;
            for (a = 0; a < d.nSeries; a++)
                for (b = 0; b < d.nSeries; b++)
                    for (i = 0; i < d.seriesLen; i++)
                        for (j = 0; j < d.seriesLen; j++) {
                            // What should the result of the comparison be?
                            if (a == b && i == j)
                                expected = 0;
                            else if (a % 2 == 0 && b % 2 == 1)
                                expected = 1;
                            else if (a % 2 == 1 && b % 2 == 0)
                                expected = -1;
                            else if (a % 2 == 0) {
                                // a and b both positive series.
                                if (a < b)
                                    expected = -1;
                                else if (a > b)
                                    expected = 1;
                                else if (i < j)
                                    expected = -1;
                                else
                                    expected = 1;
                            } else {
                                // a and b both negative series.
                                if (a < b)
                                    expected = 1;
                                else if (a > b)
                                    expected = -1;
                                else if (i < j)
                                    expected = 1;
                                else
                                    expected = -1;
                            }

                            // Compare the elements of the series directly.
                            if (expected < 0) {
                                shouldBeLess(d.series[a][i], d.series[b][j],
                                    eltName<IntType>(a, i),
                                    eltName<IntType>(b, j));
                            } else if (expected > 0) {
                                shouldBeGreater(d.series[a][i], d.series[b][j],
                                    eltName<IntType>(a, i),
                                    eltName<IntType>(b, j));
                            } else {
                                shouldBeEqual(d.series[a][i], d.series[b][j],
                                    eltName<IntType>(a, i),
                                    eltName<IntType>(b, j));
                            }

                            // Compare with expected long values as
                            // well, if we have them.
                            if (b < d.nSmallSeries) {
                                if (expected < 0) {
                                    shouldBeLess(d.series[a][i],
                                        d.smallSeriesVal[b][j],
                                        eltName<IntType>(a, i),
                                        eltName<IntType>(b, j));
                                } else if (expected > 0) {
                                    shouldBeGreater(d.series[a][i],
                                        d.smallSeriesVal[b][j],
                                        eltName<IntType>(a, i),
                                        eltName<IntType>(b, j));
                                } else {
                                    shouldBeEqual(d.series[a][i],
                                        d.smallSeriesVal[b][j],
                                        eltName<IntType>(a, i),
                                        eltName<IntType>(b, j));
                                }
                            }
                        }
        }

        template <typename IntType>
        void testIncDec(const IntType& x) {
            IntType i(x);
            IntType orig(x);
            IntType up(x + 1);
            IntType down(x - 1);

            if (i++ != orig)
                CPPUNIT_FAIL("i++ does not return original value.");
            if (i != up)
                CPPUNIT_FAIL("i++ does not increment properly.");
            if (i-- != up)
                CPPUNIT_FAIL("i-- does not return original value.");
            if (i != orig)
                CPPUNIT_FAIL("i-- does not decrement properly.");

            if (--i != down)
                CPPUNIT_FAIL("--i does not return final value.");
            if (i != down)
                CPPUNIT_FAIL("--i does not decrement properly.");
            if (++i != orig)
                CPPUNIT_FAIL("++i does not return final value.");
            if (i != orig)
                CPPUNIT_FAIL("++i does not increment properly.");
        }

        template <typename IntType>
        void incDec() {
            const Data<IntType>& d(data<IntType>());

            testIncDec(d.zero);
            testIncDec(d.one);
            testIncDec(d.two);
            testIncDec(d.negOne);
            testIncDec(d.negTwo);
            testIncDec(d.longMax);
            testIncDec(d.longMin);
            testIncDec(d.longMaxInc);
            testIncDec(d.longMinDec);
            testIncDec(d.ulongMax);
            testIncDec(d.hugePos);
            testIncDec(d.hugeNeg);

            for (int a = 0; a < d.nSeries; a++)
                for (int i = 0; i < d.seriesLen; i++)
                    testIncDec(d.series[a][i]);
        }

        void incDecInfinity() {
            {
                NLargeInteger i(NLargeInteger::infinity);
                if (++i != NLargeInteger::infinity)
                    CPPUNIT_FAIL("++inf does not return inf.");
                if (i != NLargeInteger::infinity)
                    CPPUNIT_FAIL("++inf does not result in inf.");
            }
            {
                NLargeInteger i(NLargeInteger::infinity);
                if (i++ != NLargeInteger::infinity)
                    CPPUNIT_FAIL("inf++ does not return inf.");
                if (i != NLargeInteger::infinity)
                    CPPUNIT_FAIL("inf++ does not result in inf.");
            }
            {
                NLargeInteger i(NLargeInteger::infinity);
                if (--i != NLargeInteger::infinity)
                    CPPUNIT_FAIL("--inf does not return inf.");
                if (i != NLargeInteger::infinity)
                    CPPUNIT_FAIL("--inf does not result in inf.");
            }
            {
                NLargeInteger i(NLargeInteger::infinity);
                if (i-- != NLargeInteger::infinity)
                    CPPUNIT_FAIL("inf-- does not return inf.");
                if (i != NLargeInteger::infinity)
                    CPPUNIT_FAIL("inf-- does not result in inf.");
            }
        }

        template <typename IntType>
        void testNative(const IntType& x, const char* name, long value,
                int sign, bool testCopy = false) {
            if (! x.isNative()) {
                std::ostringstream msg;
                msg << name << " is non-native.";
                CPPUNIT_FAIL(msg.str());
            }

            if (x.longValue() != value) {
                std::ostringstream msg;
                msg << name << " != " << value << " as a long.";
                CPPUNIT_FAIL(msg.str());
            }

            if (x.stringValue() != str(value)) {
                std::ostringstream msg;
                msg << name << " != " << value << " as a string.";
                CPPUNIT_FAIL(msg.str());
            }

            {
                std::ostringstream out;
                out << x;
                if (out.str() != str(value)) {
                    std::ostringstream msg;
                    msg << name << " != " << value << " on an ostream.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (sign < 0 && (x.longValue() >= zeroL || x.isZero())) {
                std::ostringstream msg;
                msg << name << " is not negative as a long.";
                CPPUNIT_FAIL(msg.str());
            } else if (sign > 0 && (x.longValue() <= zeroL || x.isZero())) {
                std::ostringstream msg;
                msg << name << " is not positive as a long.";
                CPPUNIT_FAIL(msg.str());
            } else if (sign == 0 && (x.longValue() != 0 || !  x.isZero())) {
                std::ostringstream msg;
                msg << name << " is not zero as a long.";
                CPPUNIT_FAIL(msg.str());
            }

            if (testCopy) {
                // Test the copy constructor and copy assignment here
                // also.
                IntType y(x);
                testNative(y, "Native copy", value, sign);

                IntType z(5);
                z = x;
                testNative(z, "Native = from native", value, sign);

                IntType w(HUGE_INTEGER);
                w = x;
                testNative(w, "Native = from large", value, sign);
            }
        }

        template <typename IntType>
        void testLarge(const IntType& x, const char* name,
                const std::string& value, int sign, bool testCopy = false) {
            if (x.isNative()) {
                std::ostringstream msg;
                msg << name << " should be non-native.";
                CPPUNIT_FAIL(msg.str());
            }

            if (x.stringValue() != value) {
                std::ostringstream msg;
                msg << name << " != " << value << " as a string.";
                CPPUNIT_FAIL(msg.str());
            }

            {
                std::ostringstream out;
                out << x;
                if (out.str() != value) {
                    std::ostringstream msg;
                    msg << name << " != " << value << " on an ostream.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (sign < 0 && (x >= 0 || x.stringValue()[0] != '-' ||
                    x.isZero())) {
                std::ostringstream msg;
                msg << name << " is not negative as a long.";
                CPPUNIT_FAIL(msg.str());
            } else if (sign > 0 && (x <= 0 || x.stringValue()[0] < '1' ||
                    x.stringValue()[0] > '9' || x.isZero())) {
                std::ostringstream msg;
                msg << name << " is not positive as a long.";
                CPPUNIT_FAIL(msg.str());
            } else if (sign == 0 && (x != 0 || x.stringValue() != "0" ||
                    ! x.isZero())) {
                std::ostringstream msg;
                msg << name << " is not zero as a long.";
                CPPUNIT_FAIL(msg.str());
            }

            if (testCopy) {
                // Test the copy constructor and copy assignment here
                // also.
                IntType y(x);
                testLarge(y, "Large copy", value, sign);

                IntType z(5);
                z = x;
                testLarge(z, "Large = from native", value, sign);

                IntType w(HUGE_INTEGER);
                w = x;
                testLarge(w, "Large = from large", value, sign);
            }
        }

        template <typename IntType>
        void constructAssignCopyNative() {
            testNative(IntType(), "Default", 0, 0, true);
            testNative(IntType(int(100)), "Int", 100, 1, true);
            testNative(IntType(int(-32768)), "Int", -32768, -1, true);
            testNative(IntType(unsigned(65535)), "UInt", 65535, 1, true);
            testNative(IntType(long(2147483647)), "Long", 2147483647,
                1, true);
            testNative(IntType(long(-2147483648)), "Long", -2147483648,
                -1, true);
            testNative(IntType(long(LONG_MAX)), "Long", LONG_MAX,
                1, true);
            testNative(IntType(long(LONG_MIN)), "Long", LONG_MIN,
                -1, true);
            testNative(IntType((unsigned long)(LONG_MAX)), "ULong",
                LONG_MAX, 1, true);
            testLarge(IntType((unsigned long)(LONG_MAX) + 1), "ULong",
                str((unsigned long)(LONG_MAX) + 1), 1, true);
            testLarge(IntType((unsigned long)(ULONG_MAX)), "ULong",
                str((unsigned long)(ULONG_MAX)), 1, true);

            IntType x;

            x = int(100);
            testNative(x, "Int=", 100, 1, true);
            x = int(-32768);
            testNative(x, "Int=", -32768, -1, true);
            x = unsigned(65535);
            testNative(x, "UInt=", 65535, 1, true);
            x = long(2147483647);
            testNative(x, "Long=", 2147483647, 1, true);
            x = long(-2147483648);
            testNative(x, "Long=", -2147483648, -1, true);
            x = long(LONG_MAX);
            testNative(x, "Long=", LONG_MAX, 1, true);
            x = long(LONG_MIN);
            testNative(x, "Long=", LONG_MIN, -1, true);
            x = (unsigned long)(LONG_MAX);
            testNative(x, "ULong=", LONG_MAX, 1, true);
            x = (unsigned long)(LONG_MAX) + 1;
            testLarge(x, "ULong=", str((unsigned long)(LONG_MAX) + 1), 1, true);
            x = (unsigned long)(ULONG_MAX);
            testLarge(x, "ULong=", str((unsigned long)(ULONG_MAX)), 1, true);
        }

        template <typename IntType>
        void testStringNative(const std::string& s, int base,
                long value, int sign, bool testCopy = false) {
            bool valid;
            std::string str;
            for (int i = 0; i < 2; ++i) {
                if (i == 0)
                    str = s;
                else {
                    // Try with whitespace prepended.
                    str = " \t\r\n  ";
                    str += s;
                }

                {
                    std::ostringstream name;
                    name << "C string \"" << str << "\"";
                    IntType x(str.c_str(), base, &valid);
                    if (! valid) {
                        std::ostringstream msg;
                        msg << name << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (base > 0 && x.stringValue(base) != s) {
                        std::ostringstream msg;
                        msg << name << " has incorrect stringValue(base).";
                        CPPUNIT_FAIL(msg.str());
                    }
                    testNative(x, name.str().c_str(), value, sign, testCopy);
                }
                {
                    std::ostringstream name;
                    name << "C++ string \"" << str << "\"";
                    IntType x(str, base, &valid);
                    if (! valid) {
                        std::ostringstream msg;
                        msg << name << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (base > 0 && x.stringValue(base) != s) {
                        std::ostringstream msg;
                        msg << name << " has incorrect stringValue(base).";
                        CPPUNIT_FAIL(msg.str());
                    }
                    testNative(x, name.str().c_str(), value, sign, testCopy);
                }
                if (base == 10) {
                    {
                        std::ostringstream name;
                        name << "C string = \"" << str << "\"";
                        IntType x(5);
                        x = str.c_str();
                        testNative(x, name.str().c_str(), value, sign,
                            testCopy);
                    }
                    {
                        std::ostringstream name;
                        name << "C++ string = \"" << str << "\"";
                        IntType y(5);
                        y = str;
                        testNative(y, name.str().c_str(), value, sign,
                            testCopy);
                    }
                }
            }

            // Try strings with errors.
            {
                str = s + "!";
                IntType x(str.c_str(), base, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            {
                str = s + "!";
                IntType x(str, base, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C++ string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        template <typename IntType>
        void testStringLarge(const std::string& s, int sign,
                bool testCopy = false) {
            bool valid;
            std::string str;
            for (int i = 0; i < 2; ++i) {
                if (i == 0)
                    str = s;
                else {
                    // Try with whitespace prepended.
                    str = " \t\r\n  ";
                    str += s;
                }

                {
                    std::ostringstream name;
                    name << "C string \"" << str << "\"";
                    IntType x(str.c_str(), 10, &valid);
                    if (! valid) {
                        std::ostringstream msg;
                        msg << name << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    testLarge(x, name.str().c_str(), s, sign, testCopy);
                }
                {
                    std::ostringstream name;
                    name << "C++ string \"" << str << "\"";
                    IntType x(str, 10, &valid);
                    if (! valid) {
                        std::ostringstream msg;
                        msg << name << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    testLarge(x, name.str().c_str(), s, sign, testCopy);
                }
                {
                    std::ostringstream name;
                    name << "C string = \"" << str << "\"";
                    IntType x(5);
                    x = str.c_str();
                    testLarge(x, name.str().c_str(), s, sign, testCopy);
                }
                {
                    std::ostringstream name;
                    name << "C++ string = \"" << str << "\"";
                    IntType y(5);
                    y = str;
                    testLarge(y, name.str().c_str(), s, sign, testCopy);
                }
            }

            // Try strings with errors.
            {
                str = s + "!";
                IntType x(str.c_str(), 10, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            {
                str = s + "!";
                IntType x(str, 10, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C++ string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        template <typename IntType>
        void testStringLarge(const std::string& s, int base,
                const std::string& valueBase10, int sign,
                bool testCopy = false) {
            bool valid;
            std::string str;
            for (int i = 0; i < 2; ++i) {
                if (i == 0)
                    str = s;
                else {
                    // Try with whitespace prepended.
                    str = " \t\r\n  ";
                    str += s;
                }

                {
                    std::ostringstream name;
                    name << "C string \"" << str << "\"";
                    IntType x(str.c_str(), base, &valid);
                    if (! valid) {
                        std::ostringstream msg;
                        msg << name << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (base > 0 && x.stringValue(base) != s) {
                        std::ostringstream msg;
                        msg << name << " has incorrect stringValue(base).";
                        CPPUNIT_FAIL(msg.str());
                    }
                    testLarge(x, name.str().c_str(), valueBase10, sign,
                        testCopy);
                }
                {
                    std::ostringstream name;
                    name << "C++ string \"" << str << "\"";
                    IntType x(str, base, &valid);
                    if (! valid) {
                        std::ostringstream msg;
                        msg << name << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (base > 0 && x.stringValue(base) != s) {
                        std::ostringstream msg;
                        msg << name << " has incorrect stringValue(base).";
                        CPPUNIT_FAIL(msg.str());
                    }
                    testLarge(x, name.str().c_str(), valueBase10, sign,
                        testCopy);
                }
            }

            // Try strings with errors.
            {
                str = s + "!";
                IntType x(str.c_str(), base, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            {
                str = s + "!";
                IntType x(str, base, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C++ string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        template <typename IntType>
        void constructAssignCopyString() {
            testStringNative<IntType>(str(long(LONG_MAX)), 10, LONG_MAX, 1,
                true);
            testStringNative<IntType>(str(long(LONG_MIN)), 10, LONG_MIN, -1,
                true);
            testStringLarge<IntType>(str((unsigned long)(LONG_MAX + 1)), 1,
                true);
            testStringLarge<IntType>(str((unsigned long)(ULONG_MAX)), 1, true);
            testStringLarge<IntType>(HUGE_INTEGER, 1, true);
            testStringLarge<IntType>("-" HUGE_INTEGER, -1, true);

            // Test string constructors in different bases.
            testStringNative<IntType>("101", 2, 5, 1, true);
            testStringNative<IntType>("-101", 2, -5, -1, true);
            testStringNative<IntType>("121", 3, 16, 1, true);
            testStringNative<IntType>("-121", 3, -16, -1, true);
            testStringNative<IntType>("1af", 16, 431, 1, true);
            testStringNative<IntType>("-1af", 16, -431, -1, true);
            testStringNative<IntType>("201", 31, 1923, 1, true);
            testStringNative<IntType>("-201", 31, -1923, -1, true);
            testStringNative<IntType>("121", 0, 121, 1, true);
            testStringNative<IntType>("-121", 0, -121, -1, true);
            testStringNative<IntType>("034", 0, 28, 1, true);
            testStringNative<IntType>("-034", 0, -28, -1, true);
            testStringNative<IntType>("0x1af", 0, 431, 1, true);
            testStringNative<IntType>("-0x1af", 0, -431, -1, true);
            testStringLarge<IntType>("1000000000000000000000000000000", 29,
                "74462898441675122902293018227199467668020601", 1, true);
            testStringLarge<IntType>("-1000000000000000000000000000000", 29,
                "-74462898441675122902293018227199467668020601", -1, true);
            testStringLarge<IntType>(
                "74462898441675122902293018227199467668020601",
                0, "74462898441675122902293018227199467668020601", 1, true);
            testStringLarge<IntType>(
                "-74462898441675122902293018227199467668020601",
                0, "-74462898441675122902293018227199467668020601", -1, true);
            testStringLarge<IntType>(
                "01000000000000000000000000000000000000000000000",
                0, "43556142965880123323311949751266331066368", 1, true);
            testStringLarge<IntType>(
                "-01000000000000000000000000000000000000000000000",
                0, "-43556142965880123323311949751266331066368", -1, true);
            testStringLarge<IntType>("0x10000000000000000000000000000000000",
                0, "87112285931760246646623899502532662132736", 1, true);
            testStringLarge<IntType>("-0x10000000000000000000000000000000000",
                0, "-87112285931760246646623899502532662132736", -1, true);
        }

        template <typename IntType>
        void constructSpecial() {
            const Data<IntType>& d(data<IntType>());

            // Make sure that our "special case" data members look
            // correct,
            // so we can use them with confidence throughout this class.
            if (! (d.zero.isNative() && d.zero.longValue() == 0)) {
                CPPUNIT_FAIL("Special case 0 is not initialised correctly.");
            }
            if (! (d.one.isNative() && d.one.longValue() == 1)) {
                CPPUNIT_FAIL("Special case 1 is not initialised correctly.");
            }
            if (! (d.two.isNative() && d.two.longValue() == 2)) {
                CPPUNIT_FAIL("Special case 2 is not initialised correctly.");
            }
            if (! (d.negOne.isNative() && d.negOne.longValue() == -1)) {
                CPPUNIT_FAIL("Special case -1 is not initialised correctly.");
            }
            if (! (d.negTwo.isNative() && d.negTwo.longValue() == -2)) {
                CPPUNIT_FAIL("Special case -2 is not initialised correctly.");
            }
            if (! (d.longMax.isNative() && d.longMax.longValue() == LONG_MAX &&
                    d.longMax.longValue() > zeroL &&
                    (d.longMax.longValue() + 1) < zeroL)) {
                CPPUNIT_FAIL("Special case LONG_MAX is not "
                    "initialised correctly.");
            }
            if (! (d.longMin.isNative() && d.longMin.longValue() == LONG_MIN &&
                    d.longMin.longValue() < zeroL &&
                    (d.longMin.longValue() - 1) > zeroL)) {
                CPPUNIT_FAIL("Special case LONG_MIN is not "
                    "initialised correctly.");
            }
            if (d.longMaxInc.isNative() || d.longMaxInc <= LONG_MAX ||
                    d.longMaxInc.stringValue() !=
                    (regina::NLargeInteger(LONG_MAX) + 1).stringValue()) {
                CPPUNIT_FAIL("Special case LONG_MAX+1 is not "
                    "initialised correctly.");
            }
            if (d.longMinDec.isNative() || d.longMinDec >= LONG_MIN ||
                    d.longMinDec.stringValue() !=
                    (- regina::NLargeInteger(LONG_MAX) - 2).stringValue()) {
                CPPUNIT_FAIL("Special case LONG_MIN-1 is not "
                    "initialised correctly.");
            }
            if (d.ulongMax.isNative() || d.ulongMax <= LONG_MAX ||
                    d.ulongMax.stringValue() !=
                    (regina::NLargeInteger(LONG_MAX) * 2 + 1).stringValue()) {
                CPPUNIT_FAIL("Special case ULONG_MAX is not "
                    "initialised correctly.");
            }
            if (d.hugePos.isNative() || d.hugePos <= LONG_MAX ||
                    d.hugePos.stringValue() != HUGE_INTEGER) {
                CPPUNIT_FAIL("Special case HUGE_INTEGER is not "
                    "initialised correctly.");
            }
            if (d.hugeNeg.isNative() || d.hugeNeg >= LONG_MIN ||
                    d.hugeNeg.stringValue() != "-" HUGE_INTEGER) {
                CPPUNIT_FAIL("Special case -HUGE_INTEGER is not "
                    "initialised correctly.");
            }
            {
                IntType x(d.hugeNeg);
                x.negate();
                if (x.stringValue() != HUGE_INTEGER) {
                    CPPUNIT_FAIL("Special case -HUGE_INTEGER does not "
                        "negate correctly.");
                }
            }
        }

        template <typename IntType>
        void checkDivisionAlg(int n, int divisor, int quotient,
                int remainder) {
            IntType q, r;
            q = IntType(n).divisionAlg(IntType(divisor), r);

            if (q != quotient) {
                std::ostringstream msg;
                msg << "Division algorithm (n = " << n
                    << ", d = " << divisor << ") gives quotient "
                    << q << ", not " << quotient << ".";
                CPPUNIT_FAIL(msg.str());
            }
            if (r != remainder) {
                std::ostringstream msg;
                msg << "Division algorithm (n = " << n
                    << ", d = " << divisor << ") gives remainder "
                    << r << ", not " << remainder << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        template <typename IntType>
        void divisionAlg() {
            // Check all possible zero/positive/negative combinations.
            checkDivisionAlg<IntType>(0, 0, 0, 0);
            checkDivisionAlg<IntType>(0, 3, 0, 0);
            checkDivisionAlg<IntType>(0, -3, 0, 0);
            checkDivisionAlg<IntType>(10, 0, 0, 10);
            checkDivisionAlg<IntType>(-10, 0, 0, -10);

            checkDivisionAlg<IntType>(10, 3, 3, 1);
            checkDivisionAlg<IntType>(-10, 3, -4, 2);
            checkDivisionAlg<IntType>(10, -3, -3, 1);
            checkDivisionAlg<IntType>(-10, -3, 4, 2);

            checkDivisionAlg<IntType>(12, 3, 4, 0);
            checkDivisionAlg<IntType>(-12, 3, -4, 0);
            checkDivisionAlg<IntType>(12, -3, -4, 0);
            checkDivisionAlg<IntType>(-12, -3, 4, 0);

            checkDivisionAlg<IntType>(1, 3, 0, 1);
            checkDivisionAlg<IntType>(1, -3, 0, 1);
            checkDivisionAlg<IntType>(-1, 3, -1, 2);
            checkDivisionAlg<IntType>(-1, -3, 1, 2);
        }

        template <typename IntType>
        void gcd() {
            // For now, at least make sure we treat zero correctly.
            CPPUNIT_ASSERT_MESSAGE("gcd(0,x) incorrect.",
                IntType::zero.gcd(10) == 10);
            CPPUNIT_ASSERT_MESSAGE("gcd(x,0) incorrect.",
                IntType(10).gcd(IntType::zero) == 10);
            CPPUNIT_ASSERT_MESSAGE("gcd(0,0) incorrect.",
                IntType::zero.gcd(IntType::zero) == 0);
        }

        template <typename IntType>
        void lcm() {
            // For now, at least make sure we treat zero correctly.
            CPPUNIT_ASSERT_MESSAGE("lcm(0,x) incorrect.",
                IntType::zero.lcm(10) == 0);
            CPPUNIT_ASSERT_MESSAGE("lcm(0,-x) incorrect.",
                IntType::zero.lcm(-10) == 0);
            CPPUNIT_ASSERT_MESSAGE("lcm(x,0) incorrect.",
                IntType(10).lcm(IntType::zero) == 0);
            CPPUNIT_ASSERT_MESSAGE("lcm(-x,0) incorrect.",
                IntType(-10).lcm(IntType::zero) == 0);
            CPPUNIT_ASSERT_MESSAGE("lcm(0,0) incorrect.",
                IntType::zero.lcm(IntType::zero) == 0);
        }
};

void addNInteger(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NIntegerTest::suite());
}

template <>
inline NIntegerTest::Data<NLargeInteger>& NIntegerTest::data<NLargeInteger>() {
    return dataL;
}

template <>
inline NIntegerTest::Data<NInteger>& NIntegerTest::data<NInteger>() {
    return dataI;
}

