
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "maths/nlargeinteger.h"
#include "maths/nlazyinteger.h"
#include "testsuite/utilities/testutilities.h"

using regina::NLazyInteger;

#define HUGE_INTEGER "12364981726394781629378461923786491874569283746672"

class NLazyIntegerTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NLazyIntegerTest);

    CPPUNIT_TEST(constructAssignCopyNative);
    CPPUNIT_TEST(constructAssignCopyString);
    CPPUNIT_TEST(constructSpecial);
    CPPUNIT_TEST(comparisons);
    CPPUNIT_TEST(incDec);
    CPPUNIT_TEST(gcd);
    CPPUNIT_TEST(lcm);

    CPPUNIT_TEST_SUITE_END();

    private:
        NLazyInteger smallPosSeries[6];
            /**< A sequence of positive integers that fit into a long. */
        NLazyInteger smallNegSeries[6];
            /**< A sequence of negative integers that fit into a long. */
        NLazyInteger largePosSeries[6];
            /**< A sequence of positive integers too large for a long. */
        NLazyInteger largeNegSeries[6];
            /**< A sequence of negative integers too large for a long. */

        static const unsigned nSeries = 4;
            /**< The number of sequences described above. */
        static const unsigned seriesLen = 6;
            /**< The number of integers in each of the above sequences. */
        NLazyInteger* series[4];
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

        NLazyInteger zero;
            /**< Special case (native): 0 */
        NLazyInteger one;
            /**< Special case (native): 1 */
        NLazyInteger two;
            /**< Special case (native): 2 */
        NLazyInteger negOne;
            /**< Special case (native): -1 */
        NLazyInteger negTwo;
            /**< Special case (native): -2 */
        NLazyInteger longMax;
            /**< Special case (native): LONG_MAX */
        NLazyInteger longMin;
            /**< Special case (native): LONG_MIN */
        NLazyInteger longMaxInc;
            /**< Special case (large): LONG_MAX + 1 */
        NLazyInteger longMinDec;
            /**< Special case (large): LONG_MIN - 1 */
        NLazyInteger ulongMax;
            /**< Special case (large): ULONG_MAX */
        NLazyInteger hugePos;
            /**< Special case (large): a huge positive integer */
        NLazyInteger hugeNeg;
            /**< Special case (large): a huge negative integer */

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
            hugePos =  HUGE_INTEGER;
            hugeNeg = "-" HUGE_INTEGER;
        }

        void tearDown() {
        }

        template <typename T>
        std::string str(T x) {
            std::ostringstream ans;
            ans << x;
            return ans.str();
        }

        void testNative(const NLazyInteger& x, const char* name, long value,
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

            if (sign < 0 && (x.longValue() >= 0 || x.isZero())) {
                std::ostringstream msg;
                msg << name << " is not negative as a long.";
                CPPUNIT_FAIL(msg.str());
            } else if (sign > 0 && (x.longValue() <= 0 || x.isZero())) {
                std::ostringstream msg;
                msg << name << " is not positive as a long.";
                CPPUNIT_FAIL(msg.str());
            } else if (sign == 0 && (x.longValue() != 0 || ! x.isZero())) {
                std::ostringstream msg;
                msg << name << " is not zero as a long.";
                CPPUNIT_FAIL(msg.str());
            }

            if (testCopy) {
                // Test the copy constructor and copy assignment here also.
                NLazyInteger y(x);
                testNative(y, "Native copy", value, sign);

                NLazyInteger z(5);
                z = x;
                testNative(z, "Native = from native", value, sign);

                NLazyInteger w(HUGE_INTEGER);
                w = x;
                testNative(w, "Native = from large", value, sign);
            }
        }

        void testLarge(const NLazyInteger& x, const char* name,
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
                // Test the copy constructor and copy assignment here also.
                NLazyInteger y(x);
                testLarge(y, "Large copy", value, sign);

                NLazyInteger z(5);
                z = x;
                testLarge(z, "Large = from native", value, sign);

                NLazyInteger w(HUGE_INTEGER);
                w = x;
                testLarge(w, "Large = from large", value, sign);
            }
        }

        void constructAssignCopyNative() {
            testNative(NLazyInteger(), "Default", 0, 0, true);
            testNative(NLazyInteger(int(100)), "Int", 100, 1, true);
            testNative(NLazyInteger(int(-32768)), "Int", -32768, -1, true);
            testNative(NLazyInteger(unsigned(65535)), "UInt", 65535, 1, true);
            testNative(NLazyInteger(long(2147483647)), "Long", 2147483647,
                1, true);
            testNative(NLazyInteger(long(-2147483648)), "Long", -2147483648,
                -1, true);
            testNative(NLazyInteger(long(LONG_MAX)), "Long", LONG_MAX,
                1, true);
            testNative(NLazyInteger(long(LONG_MIN)), "Long", LONG_MIN,
                -1, true);
            testNative(NLazyInteger((unsigned long)(LONG_MAX)), "ULong",
                LONG_MAX, 1, true);
            testLarge(NLazyInteger((unsigned long)(LONG_MAX) + 1), "ULong",
                str((unsigned long)(LONG_MAX) + 1), 1, true);
            testLarge(NLazyInteger((unsigned long)(ULONG_MAX)), "ULong",
                str((unsigned long)(ULONG_MAX)), 1, true);

            NLazyInteger x;

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
                    NLazyInteger x(str.c_str(), base, &valid);
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
                    NLazyInteger x(str, base, &valid);
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
                        NLazyInteger x(5);
                        x = str.c_str();
                        testNative(x, name.str().c_str(), value, sign,
                            testCopy);
                    }
                    {
                        std::ostringstream name;
                        name << "C++ string = \"" << str << "\"";
                        NLazyInteger y(5);
                        y = str;
                        testNative(y, name.str().c_str(), value, sign,
                            testCopy);
                    }
                }
            }

            // Try strings with errors.
            {
                str = s + "!";
                NLazyInteger x(str.c_str(), base, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            {
                str = s + "!";
                NLazyInteger x(str, base, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C++ string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

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
                    NLazyInteger x(str.c_str(), 10, &valid);
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
                    NLazyInteger x(str, 10, &valid);
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
                    NLazyInteger x(5);
                    x = str.c_str();
                    testLarge(x, name.str().c_str(), s, sign, testCopy);
                }
                {
                    std::ostringstream name;
                    name << "C++ string = \"" << str << "\"";
                    NLazyInteger y(5);
                    y = str;
                    testLarge(y, name.str().c_str(), s, sign, testCopy);
                }
            }

            // Try strings with errors.
            {
                str = s + "!";
                NLazyInteger x(str.c_str(), 10, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            {
                str = s + "!";
                NLazyInteger x(str, 10, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C++ string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

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
                    NLazyInteger x(str.c_str(), base, &valid);
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
                    NLazyInteger x(str, base, &valid);
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
                NLazyInteger x(str.c_str(), base, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            {
                str = s + "!";
                NLazyInteger x(str, base, &valid);
                if (valid) {
                    std::ostringstream msg;
                    msg << "C++ string \"" << str << "\" should be invalid.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void constructAssignCopyString() {
            testStringNative(str(long(LONG_MAX)), 10, LONG_MAX, 1, true);
            testStringNative(str(long(LONG_MIN)), 10, LONG_MIN, -1, true);
            testStringLarge(str((unsigned long)(LONG_MAX + 1)), 1, true);
            testStringLarge(str((unsigned long)(ULONG_MAX)), 1, true);
            testStringLarge(HUGE_INTEGER, 1, true);
            testStringLarge("-" HUGE_INTEGER, -1, true);

            // Test string constructors in different bases.
            testStringNative("101", 2, 5, 1, true);
            testStringNative("-101", 2, -5, -1, true);
            testStringNative("121", 3, 16, 1, true);
            testStringNative("-121", 3, -16, -1, true);
            testStringNative("1af", 16, 431, 1, true);
            testStringNative("-1af", 16, -431, -1, true);
            testStringNative("201", 31, 1923, 1, true);
            testStringNative("-201", 31, -1923, -1, true);
            testStringNative("121", 0, 121, 1, true);
            testStringNative("-121", 0, -121, -1, true);
            testStringNative("034", 0, 28, 1, true);
            testStringNative("-034", 0, -28, -1, true);
            testStringNative("0x1af", 0, 431, 1, true);
            testStringNative("-0x1af", 0, -431, -1, true);
            testStringLarge("1000000000000000000000000000000", 29,
                "74462898441675122902293018227199467668020601", 1, true);
            testStringLarge("-1000000000000000000000000000000", 29,
                "-74462898441675122902293018227199467668020601", -1, true);
            testStringLarge("74462898441675122902293018227199467668020601",
                0, "74462898441675122902293018227199467668020601", 1, true);
            testStringLarge("-74462898441675122902293018227199467668020601",
                0, "-74462898441675122902293018227199467668020601", -1, true);
            testStringLarge("01000000000000000000000000000000000000000000000",
                0, "43556142965880123323311949751266331066368", 1, true);
            testStringLarge("-01000000000000000000000000000000000000000000000",
                0, "-43556142965880123323311949751266331066368", -1, true);
            testStringLarge("0x10000000000000000000000000000000000",
                0, "87112285931760246646623899502532662132736", 1, true);
            testStringLarge("-0x10000000000000000000000000000000000",
                0, "-87112285931760246646623899502532662132736", -1, true);
        }

        void constructSpecial() {
            // Make sure that our "special case" data members look correct,
            // so we can use them with confidence throughout this class.
            if (! (zero.isNative() && zero.longValue() == 0)) {
                CPPUNIT_FAIL("Special case 0 is not initialised correctly.");
            }
            if (! (one.isNative() && one.longValue() == 1)) {
                CPPUNIT_FAIL("Special case 1 is not initialised correctly.");
            }
            if (! (two.isNative() && two.longValue() == 2)) {
                CPPUNIT_FAIL("Special case 2 is not initialised correctly.");
            }
            if (! (negOne.isNative() && negOne.longValue() == -1)) {
                CPPUNIT_FAIL("Special case -1 is not initialised correctly.");
            }
            if (! (negTwo.isNative() && negTwo.longValue() == -2)) {
                CPPUNIT_FAIL("Special case -2 is not initialised correctly.");
            }
            if (! (longMax.isNative() && longMax.longValue() == LONG_MAX &&
                    longMax.longValue() > 0 && (longMax.longValue() + 1) < 0)) {
                CPPUNIT_FAIL("Special case LONG_MAX is not "
                    "initialised correctly.");
            }
            if (! (longMin.isNative() && longMin.longValue() == LONG_MIN &&
                    longMin.longValue() < 0 && (longMin.longValue() - 1) > 0)) {
                CPPUNIT_FAIL("Special case LONG_MIN is not "
                    "initialised correctly.");
            }
            if (longMaxInc.isNative() || longMaxInc <= LONG_MAX ||
                    longMaxInc.stringValue() !=
                    (regina::NLargeInteger(LONG_MAX) + 1).stringValue()) {
                CPPUNIT_FAIL("Special case LONG_MAX+1 is not "
                    "initialised correctly.");
            }
            if (longMinDec.isNative() || longMinDec >= LONG_MIN ||
                    longMinDec.stringValue() !=
                    (- regina::NLargeInteger(LONG_MAX) - 2).stringValue()) {
                CPPUNIT_FAIL("Special case LONG_MIN-1 is not "
                    "initialised correctly.");
            }
            if (ulongMax.isNative() || ulongMax <= LONG_MAX ||
                    ulongMax.stringValue() !=
                    (regina::NLargeInteger(LONG_MAX) * 2 + 1).stringValue()) {
                CPPUNIT_FAIL("Special case ULONG_MAX is not "
                    "initialised correctly.");
            }
            if (hugePos.isNative() || hugePos <= LONG_MAX ||
                    hugePos.stringValue() != HUGE_INTEGER) {
                CPPUNIT_FAIL("Special case HUGE_INTEGER is not "
                    "initialised correctly.");
            }
            if (hugeNeg.isNative() || hugeNeg >= LONG_MIN ||
                    hugeNeg.stringValue() != "-" HUGE_INTEGER) {
                CPPUNIT_FAIL("Special case -HUGE_INTEGER is not "
                    "initialised correctly.");
            }
            {
                NLazyInteger x(hugeNeg);
                x.negate();
                if (x.stringValue() != HUGE_INTEGER) {
                    CPPUNIT_FAIL("Special case -HUGE_INTEGER does not "
                        "negate correctly.");
                }
            }
        }

        // TODO: Test swap()
        // TODO: Test comparisons (lazy && long)
        // TODO: Test ++, -- (both versions)
        // TODO: Test +, -, *, /, divExact, % (lazy && long); unary -
        // TODO: Test +=, -=, *=, /=, divByExact, %= (lazy && long); negate
        // TODO: Test abs, gcd, lcm
        // TODO: Test tryReduce, makeLarge.

        std::string eltName(int whichSeries, int whichMember) {
            std::ostringstream ans;
            ans << seriesName[whichSeries] << '[' << whichMember << ']';
            return ans.str();
        }

        void shouldBeLess(const NLazyInteger& a, const NLazyInteger& b,
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

        void shouldBeEqual(const NLazyInteger& a, const NLazyInteger& b,
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
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not str== " + bName + ".",
                a.stringValue() == b.stringValue());
        }

        void shouldBeGreater(const NLazyInteger& a, const NLazyInteger& b,
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

        void shouldBeLess(const NLazyInteger& a, long b,
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

        void shouldBeEqual(const NLazyInteger& a, long b,
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

        void shouldBeGreater(const NLazyInteger& a, long b,
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

            const NLazyInteger& zero(NLazyInteger::zero);
            const NLazyInteger& one(NLazyInteger::one);

            shouldBeLess(zero, one, "zero", "one");
            shouldBeLess(zero, 1L, "zero", "one");
            shouldBeGreater(one, zero, "one", "zero");
            shouldBeGreater(one, 0L, "one", "zero");
            shouldBeEqual(zero, zero, "zero", "zero");
            shouldBeEqual(zero, 0L, "zero", "zero");
            shouldBeEqual(one, one, "one", "one");
            shouldBeEqual(one, 1L, "one", "one");

            // Compare the elements of the series with zero and one.
            for (a = 0; a < nSeries; a++)
                for (i = 0; i < seriesLen; i++) {
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

        void testIncDec(const NLazyInteger& x) {
            NLazyInteger i(x);
            NLazyInteger orig(x);
            NLazyInteger up(x + 1);
            NLazyInteger down(x - 1);

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

        void incDec() {
            for (int a = 0; a < nSeries; a++)
                for (int i = 0; i < seriesLen; i++)
                    testIncDec(series[a][i]);

            testIncDec(zero);
            testIncDec(one);
            testIncDec(two);
            testIncDec(negOne);
            testIncDec(negTwo);
            testIncDec(longMax);
            testIncDec(longMin);
            testIncDec(longMaxInc);
            testIncDec(longMinDec);
            testIncDec(ulongMax);
            testIncDec(hugePos);
            testIncDec(hugeNeg);
        }

        void gcd() {
            // For now, at least make sure we treat zero correctly.
            CPPUNIT_ASSERT_MESSAGE("gcd(0,x) incorrect.",
                NLazyInteger::zero.gcd(10) == 10);
            CPPUNIT_ASSERT_MESSAGE("gcd(x,0) incorrect.",
                NLazyInteger(10).gcd(NLazyInteger::zero) == 10);
            CPPUNIT_ASSERT_MESSAGE("gcd(0,0) incorrect.",
                NLazyInteger::zero.gcd(NLazyInteger::zero) == 0);
        }

        void lcm() {
            // For now, at least make sure we treat zero correctly.
            CPPUNIT_ASSERT_MESSAGE("lcm(0,x) incorrect.",
                NLazyInteger::zero.lcm(10) == 0);
            CPPUNIT_ASSERT_MESSAGE("lcm(0,-x) incorrect.",
                NLazyInteger::zero.lcm(-10) == 0);
            CPPUNIT_ASSERT_MESSAGE("lcm(x,0) incorrect.",
                NLazyInteger(10).lcm(NLazyInteger::zero) == 0);
            CPPUNIT_ASSERT_MESSAGE("lcm(-x,0) incorrect.",
                NLazyInteger(-10).lcm(NLazyInteger::zero) == 0);
            CPPUNIT_ASSERT_MESSAGE("lcm(0,0) incorrect.",
                NLazyInteger::zero.lcm(NLazyInteger::zero) == 0);
        }
};

void addNLazyInteger(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NLazyIntegerTest::suite());
}

