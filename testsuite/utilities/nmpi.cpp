
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "testsuite/utilities/testutilities.h"
#include "utilities/nmpi.h"

using regina::NLargeInteger;

class NLargeIntegerTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NLargeIntegerTest);

    CPPUNIT_TEST(comparisons);

    CPPUNIT_TEST_SUITE_END();

    private:
        NLargeInteger smallPosSeries[6];
            /**< A sequence of positive integers that fit into a long. */
        NLargeInteger smallNegSeries[6];
            /**< A sequence of negative integers that fit into a long. */
        NLargeInteger largePosSeries[6];
            /**< A sequence of positive integers too large for a long. */
        NLargeInteger largeNegSeries[6];
            /**< A sequence of negative integers too large for a long. */

        static const unsigned nSeries = 4;
            /**< The number of sequences described above. */
        static const unsigned seriesLen = 6;
            /**< The number of integers in each of the above sequences. */
        NLargeInteger* series[4];
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

    public:
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
            largePosSeries[5] = "6000000000000000000000000000000"; // 6*10^30
            largeNegSeries[0] = "-1000000000000000"; // 10^15
            largeNegSeries[1] = "-2000000000000000"; // 2*10^15
            largeNegSeries[2] = "-3000000000000000"; // 3*10^15
            largeNegSeries[3] = "-4000000000000000"; // 4*10^15
            largeNegSeries[4] = "-6000000000000000"; // 6*10^15
            largeNegSeries[5] = "-6000000000000000000000000000000"; // 6*10^30

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
        }

        void tearDown() {
        }

        std::string eltName(int whichSeries, int whichMember) {
            std::ostringstream ans;
            ans << seriesName[whichSeries] << '[' << whichMember << ']';
            return ans.str();
        }

        void shouldBeLess(const NLargeInteger& a, const NLargeInteger& b,
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

        void shouldBeEqual(const NLargeInteger& a, const NLargeInteger& b,
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
        }

        void shouldBeGreater(const NLargeInteger& a, const NLargeInteger& b,
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

        void shouldBeLess(const NLargeInteger& a, long b,
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

        void shouldBeEqual(const NLargeInteger& a, long b,
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
        }

        void shouldBeGreater(const NLargeInteger& a, long b,
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

        void comparisons() {
            unsigned a, b, i, j;

            const NLargeInteger& zero(NLargeInteger::zero);
            const NLargeInteger& one(NLargeInteger::one);
            const NLargeInteger& infinity(NLargeInteger::infinity);

            shouldBeLess(zero, one, "zero", "one");
            shouldBeLess(zero, 1L, "zero", "one");
            shouldBeLess(one, infinity, "one", "infinity");
            shouldBeLess(zero, infinity, "zero", "infinity");
            shouldBeGreater(one, zero, "one", "zero");
            shouldBeGreater(one, 0L, "one", "zero");
            shouldBeGreater(infinity, one, "infinity", "one");
            shouldBeGreater(infinity, 1L, "infinity", "one");
            shouldBeGreater(infinity, zero, "infinity", "zero");
            shouldBeGreater(infinity, 0L, "infinity", "zero");
            shouldBeEqual(zero, zero, "zero", "zero");
            shouldBeEqual(zero, 0L, "zero", "zero");
            shouldBeEqual(one, one, "one", "one");
            shouldBeEqual(one, 1L, "one", "one");
            shouldBeEqual(infinity, infinity, "infinity", "infinity");

            // Compare the elements of the series with zero, one and
            // infinity.
            for (a = 0; a < nSeries; a++)
                for (i = 0; i < seriesLen; i++) {
                    shouldBeLess(series[a][i], infinity,
                        eltName(a, i), "infinity");
                    shouldBeGreater(infinity, series[a][i],
                        "infinity", eltName(a, i));
                    if (a == 0 || a == 2) {
                        // Positive series.
                        shouldBeGreater(series[a][i], zero,
                            eltName(a, i), "zero");
                        shouldBeGreater(series[a][i], 0L,
                            eltName(a, i), "zero");
                        shouldBeGreater(series[a][i], one,
                            eltName(a, i), "one");
                        shouldBeGreater(series[a][i], 1L,
                            eltName(a, i), "one");
                        shouldBeLess(zero, series[a][i],
                            "zero", eltName(a, i));
                        shouldBeLess(one, series[a][i],
                            "one", eltName(a, i));
                    } else {
                        // Negative series.
                        shouldBeLess(series[a][i], zero,
                            eltName(a, i), "zero");
                        shouldBeLess(series[a][i], 0L,
                            eltName(a, i), "zero");
                        shouldBeLess(series[a][i], one,
                            eltName(a, i), "one");
                        shouldBeLess(series[a][i], 1L,
                            eltName(a, i), "one");
                        shouldBeGreater(zero, series[a][i],
                            "zero", eltName(a, i));
                        shouldBeGreater(one, series[a][i],
                            "one", eltName(a, i));
                    }
                }

            // Compare all elements of all series in pairs.
            int expected;
            for (a = 0; a < nSeries; a++)
                for (b = 0; b < nSeries; b++)
                    for (i = 0; i < seriesLen; i++)
                        for (j = 0; j < seriesLen; j++) {
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
                                shouldBeLess(series[a][i], series[b][j],
                                    eltName(a, i), eltName(b, j));
                            } else if (expected > 0) {
                                shouldBeGreater(series[a][i], series[b][j],
                                    eltName(a, i), eltName(b, j));
                            } else {
                                shouldBeEqual(series[a][i], series[b][j],
                                    eltName(a, i), eltName(b, j));
                            }

                            // Compare with expected long values as
                            // well, if we have them.
                            if (b < nSmallSeries) {
                                if (expected < 0) {
                                    shouldBeLess(series[a][i],
                                        smallSeriesVal[b][j],
                                        eltName(a, i), eltName(b, j));
                                } else if (expected > 0) {
                                    shouldBeGreater(series[a][i],
                                        smallSeriesVal[b][j],
                                        eltName(a, i), eltName(b, j));
                                } else {
                                    shouldBeEqual(series[a][i],
                                        smallSeriesVal[b][j],
                                        eltName(a, i), eltName(b, j));
                                }
                            }
                        }
        }
};

void addNLargeInteger(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NLargeIntegerTest::suite());
}

