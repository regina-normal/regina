
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
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

    CPPUNIT_TEST(equality);

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

        void equality() {
            unsigned a, b, i, j;

            const NLargeInteger& zero(NLargeInteger::zero);
            const NLargeInteger& one(NLargeInteger::one);
            const NLargeInteger& infinity(NLargeInteger::infinity);

            CPPUNIT_ASSERT_MESSAGE("Constant zero does not == itself.",
                zero == zero);
            CPPUNIT_ASSERT_MESSAGE("Constant zero does not == 0.",
                zero == 0L);
            CPPUNIT_ASSERT_MESSAGE("Constant one does not == itself.",
                one == one);
            CPPUNIT_ASSERT_MESSAGE("Constant one does not == 1.",
                one == 1L);
            CPPUNIT_ASSERT_MESSAGE("Constant infinity does not == itself.",
                infinity == infinity);
            CPPUNIT_ASSERT_MESSAGE("Constant zero != itself.",
                ! (zero != zero));
            CPPUNIT_ASSERT_MESSAGE("Constant zero != 0.",
                ! (zero != 0L));
            CPPUNIT_ASSERT_MESSAGE("Constant one != itself.",
                ! (one != one));
            CPPUNIT_ASSERT_MESSAGE("Constant one != 1.",
                ! (one != 1L));
            CPPUNIT_ASSERT_MESSAGE("Constant infinity != itself.",
                ! (infinity != infinity));
            CPPUNIT_ASSERT_MESSAGE("Constant infinity == zero.",
                ! (infinity == zero || zero == infinity));
            CPPUNIT_ASSERT_MESSAGE("Constant infinity == 0.",
                ! (infinity == 0L));
            CPPUNIT_ASSERT_MESSAGE("Constant infinity == one.",
                ! (infinity == one || one == infinity));
            CPPUNIT_ASSERT_MESSAGE("Constant infinity == 1.",
                ! (infinity == 1L));
            CPPUNIT_ASSERT_MESSAGE("Constant infinity does not != zero.",
                infinity != zero && zero != infinity);
            CPPUNIT_ASSERT_MESSAGE("Constant infinity does not != 0.",
                infinity != 0L);
            CPPUNIT_ASSERT_MESSAGE("Constant infinity does not != one.",
                infinity != one && one != infinity);
            CPPUNIT_ASSERT_MESSAGE("Constant infinity does not != 1.",
                infinity != 1L);

            for (a = 0; a < nSeries; a++) {
                for (i = 0; i < seriesLen; i++) {
                    if (! (series[a][i] == series[a][i])) {
                        std::string msg = "Element ";
                        msg += eltName(a, i);
                        msg += " does not == itself.";
                        CPPUNIT_FAIL(msg);
                    }
                    if (series[a][i] != series[a][i]) {
                        std::string msg = "Element ";
                        msg += eltName(a, i);
                        msg += " != itself.";
                        CPPUNIT_FAIL(msg);
                    }

                    if (series[a][i] == zero || zero == series[a][i]) {
                        std::string msg = "Element ";
                        msg += eltName(a, i);
                        msg += " == zero.";
                        CPPUNIT_FAIL(msg);
                    }
                    if (! (series[a][i] != zero && zero != series[a][i])) {
                        std::string msg = "Element ";
                        msg += eltName(a, i);
                        msg += " does not != zero.";
                        CPPUNIT_FAIL(msg);
                    }

                    if (series[a][i] == one || one == series[a][i]) {
                        std::string msg = "Element ";
                        msg += eltName(a, i);
                        msg += " == one.";
                        CPPUNIT_FAIL(msg);
                    }
                    if (! (series[a][i] != one && one != series[a][i])) {
                        std::string msg = "Element ";
                        msg += eltName(a, i);
                        msg += " does not != one.";
                        CPPUNIT_FAIL(msg);
                    }

                    if (series[a][i] == infinity || infinity == series[a][i]) {
                        std::string msg = "Element ";
                        msg += eltName(a, i);
                        msg += " == infinity.";
                        CPPUNIT_FAIL(msg);
                    }
                    if (! (series[a][i] != infinity &&
                            infinity != series[a][i])) {
                        std::string msg = "Element ";
                        msg += eltName(a, i);
                        msg += " does not != infinity.";
                        CPPUNIT_FAIL(msg);
                    }

                    if (a < nSmallSeries) {
                        if (! (series[a][i] == smallSeriesVal[a][i])) {
                            std::string msg = "Element ";
                            msg += eltName(a, i);
                            msg += " does not == its declared long value.";
                            CPPUNIT_FAIL(msg);
                        }
                        if (series[a][i] != smallSeriesVal[a][i]) {
                            std::string msg = "Element ";
                            msg += eltName(a, i);
                            msg += " != its declared long value.";
                            CPPUNIT_FAIL(msg);
                        }
                    }
                }
            }

            for (a = 0; a < nSeries; a++)
                for (b = 0; b < nSeries; b++)
                    for (i = 0; i < seriesLen; i++)
                        for (j = 0; j < seriesLen; j++) {
                            if (a == b && i == j)
                                continue;
                            if (series[a][i] == series[b][j]) {
                                std::string msg = "Elements ";
                                msg += eltName(a, i);
                                msg += " and ";
                                msg += eltName(b, j);
                                msg += " == each other.";
                                CPPUNIT_FAIL(msg);
                            }
                            if (! (series[a][i] != series[b][j])) {
                                std::string msg = "Elements ";
                                msg += eltName(a, i);
                                msg += " and ";
                                msg += eltName(b, j);
                                msg += " do not != each other.";
                                CPPUNIT_FAIL(msg);
                            }
                            if (b < nSmallSeries) {
                                if (series[a][i] == smallSeriesVal[b][j]) {
                                    std::string msg = "Element ";
                                    msg += eltName(a, i);
                                    msg += " == the declared long value of ";
                                    msg += eltName(b, j);
                                    msg += ".";
                                    CPPUNIT_FAIL(msg);
                                }
                                if (! (series[a][i] != smallSeriesVal[b][j])) {
                                    std::string msg = "Element ";
                                    msg += eltName(a, i);
                                    msg += " does not != the declared "
                                        "long value of ";
                                    msg += eltName(b, j);
                                    msg += ".";
                                    CPPUNIT_FAIL(msg);
                                }
                            }
                        }
        }
};

void addNLargeInteger(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NLargeIntegerTest::suite());
}

