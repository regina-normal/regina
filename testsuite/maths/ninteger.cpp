
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

// TODO: Test +, -, * (all lazy && long)
// TODO: Review down to operator /.

// TODO: Test /, divExact, % (all lazy && long); unary -
// TODO: Test rounding direction for all variants of division
// TODO: Test divisionAlg
// TODO: Test +=, -=, *=, /=, divByExact, %= (lazy && long); negate
// TODO: Test raiseToPower, abs, gcd, lcm, gcdWithCoeffs
// TODO: Test legendre, random functions
// TODO: Test setRaw, rawData, tryReduce, makeLarge.

using regina::NIntegerBase;
using regina::NInteger;
using regina::NLargeInteger;

#define ENORMOUS_INTEGER "115792089237316195423570985008687907853269984665640564039457584007913129639936"
#define HUGE_INTEGER "12364981726394781629378461923786491874569283746672"

class NIntegerTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NIntegerTest);

    CPPUNIT_TEST(constructAssignCopyNative<NInteger>);
    CPPUNIT_TEST(constructAssignCopyNative<NLargeInteger>);
    CPPUNIT_TEST(constructAssignCopyString<NInteger>);
    CPPUNIT_TEST(constructAssignCopyString<NLargeInteger>);
    CPPUNIT_TEST(constructAssignCopyInfinity);
    CPPUNIT_TEST(constructSpecial<NInteger>);
    CPPUNIT_TEST(constructSpecial<NLargeInteger>);
    CPPUNIT_TEST(stringValue<NInteger>);
    CPPUNIT_TEST(stringValue<NLargeInteger>);
    CPPUNIT_TEST(swap<NInteger>);
    CPPUNIT_TEST(swap<NLargeInteger>);
    CPPUNIT_TEST(comparisons<NInteger>);
    CPPUNIT_TEST(comparisons<NLargeInteger>);
    CPPUNIT_TEST(incDec<NInteger>);
    CPPUNIT_TEST(incDec<NLargeInteger>);
    CPPUNIT_TEST(plusMinus<NInteger>);
    CPPUNIT_TEST(plusMinus<NLargeInteger>);
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
            static const IntType cases[];
                /**< An increasing sequence of arbitrary precision integers.
                     These are defined at the bottom of this file (since
                     static arrays must be defined out of line). */
            static const unsigned nCases;
                /**< The length of the sequence \a cases. */

            static const long longCases[];
                /**< An increasing sequence of long integers.
                     These are defined at the bottom of this file (since
                     static arrays must be defined out of line). */
            static const long nLongCases;
                /**< The length of the sesquence \a longCases. */

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

        std::string sLongMax;
        std::string sLongMaxInc;
        std::string sLongMin;
        std::string sLongMinDec;
        std::string sULongMax;

        template <typename IntType>
        Data<IntType>& data();

    public:
        template <typename T>
        std::string str(T x) {
            std::ostringstream ans;
            ans << x;
            return ans.str();
        }

        /**
         * Convert the string for a positive integer n into the string
         * for (n+1), or convert the string for a negative integer -n
         * into the string for -(n+1).
         *
         * PRE: The last digit of s is not 9.
         */
        std::string incStrSimple(const std::string& s) {
            std::string ans = s;
            ans[ans.length() - 1]++;
            return ans;
        }

        void setUp() {
            dataL.setUp();
            dataI.setUp();

            zeroL = 0;

            sLongMax = str(LONG_MAX);
            sLongMaxInc = incStrSimple(sLongMax);
            sLongMin = str(LONG_MIN);
            sLongMinDec = incStrSimple(sLongMin);
            sULongMax = str(ULONG_MAX);
        }

        void tearDown() {
        }

        template <typename IntType>
        std::string eltName(int whichSeries, int whichMember) {
            std::ostringstream ans;
            ans << data<IntType>().seriesName[whichSeries]
                << '[' << whichMember << ']';
            return ans.str();
        }

        template <typename IntType>
        void testNative(const IntType& x, const char* name, long value,
                int sign, bool testCopy = true) {
            if (! x.isNative()) {
                std::ostringstream msg;
                msg << name << " is non-native.";
                CPPUNIT_FAIL(msg.str());
            }

            if (x.isInfinite()) {
                std::ostringstream msg;
                msg << name << " is reported as infinite.";
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
            } else if (sign == 0 && (x.longValue() != 0 || ! x.isZero())) {
                std::ostringstream msg;
                msg << name << " is not zero as a long.";
                CPPUNIT_FAIL(msg.str());
            }

            if (testCopy) {
                // Test the copy constructor and copy assignment here
                // also.
                IntType y(x);
                testNative(y, "Native copy", value, sign, false);

                IntType z(5);
                if (! z.isNative())
                    CPPUNIT_FAIL("Hard-coded 5 is not native.");
                z = x;
                testNative(z, "Native = from native", value, sign, false);

                IntType w(HUGE_INTEGER);
                if (w.isNative())
                    CPPUNIT_FAIL("Hard-coded HUGE_INTEGER is native.");
                w = x;
                testNative(w, "Native = from large", value, sign, false);
            }
        }

        template <typename IntType>
        void testLarge(const IntType& x, const char* name,
                const std::string& value, int sign, bool testCopy = true) {
            if (x.isNative()) {
                std::ostringstream msg;
                msg << name << " should be non-native.";
                CPPUNIT_FAIL(msg.str());
            }

            if (x.isInfinite()) {
                std::ostringstream msg;
                msg << name << " is reported as infinite.";
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
                testLarge(y, "Large copy", value, sign, false);

                IntType z(5);
                if (! z.isNative())
                    CPPUNIT_FAIL("Hard-coded 5 is not native.");
                z = x;
                testLarge(z, "Large = from native", value, sign, false);

                IntType w(HUGE_INTEGER);
                if (w.isNative())
                    CPPUNIT_FAIL("Hard-coded HUGE_INTEGER is native.");
                w = x;
                testLarge(w, "Large = from large", value, sign, false);
            }
        }

        template <typename IntType>
        void constructAssignCopyNative() {
            testNative(IntType(), "Default", 0, 0);
            testNative(IntType(int(100)), "Int", 100, 1);
            testNative(IntType(int(-32768)), "Int", -32768, -1);
            testNative(IntType(unsigned(65535)), "UInt", 65535, 1);
            testNative(IntType(long(2147483647)), "Long", 2147483647, 1);
            testNative(IntType(long(-2147483648)), "Long", -2147483648, -1);
            testNative(IntType(long(LONG_MAX)), "Long", LONG_MAX, 1);
            testNative(IntType(long(LONG_MIN)), "Long", LONG_MIN, -1);
            testNative(IntType((unsigned long)(LONG_MAX)), "ULong", LONG_MAX,
                1);
            testLarge(IntType(((unsigned long)(LONG_MAX)) + 1), "ULong",
                sLongMaxInc, 1);
            testLarge(IntType((unsigned long)(ULONG_MAX)), "ULong",
                sULongMax, 1);

            IntType x;

            x = int(100);
            testNative(x, "Int=", 100, 1);
            x = int(-32768);
            testNative(x, "Int=", -32768, -1);
            x = unsigned(65535);
            testNative(x, "UInt=", 65535, 1);
            x = long(2147483647);
            testNative(x, "Long=", 2147483647, 1);
            x = long(-2147483648);
            testNative(x, "Long=", -2147483648, -1);
            x = long(LONG_MAX);
            testNative(x, "Long=", LONG_MAX, 1);
            x = long(LONG_MIN);
            testNative(x, "Long=", LONG_MIN, -1);
            x = (unsigned long)(LONG_MAX);
            testNative(x, "ULong=", LONG_MAX, 1);
            x = ((unsigned long)(LONG_MAX)) + 1;
            testLarge(x, "ULong=", sLongMaxInc, 1);
            x = (unsigned long)(ULONG_MAX);
            testLarge(x, "ULong=", sULongMax, 1);
        }

        template <typename IntType>
        void testStringNative(const std::string& s, int base,
                long value, int sign) {
            bool valid;
            std::string str;
            for (int i = 0; i < 4; ++i) {
                if (i < 2)
                    str = s;
                else {
                    // Try with whitespace prepended.
                    str = " \t\r\n  ";
                    str += s;
                }
                if (i % 2)
                    str += " \t\r\n  "; // and whitespace appended.

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
                    testNative(x, name.str().c_str(), value, sign);
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
                    testNative(x, name.str().c_str(), value, sign);
                }
                if (base == 10) {
                    {
                        std::ostringstream name;
                        name << "C string = \"" << str << "\"";
                        IntType x(5);
                        if (! x.isNative())
                            CPPUNIT_FAIL("Hard-coded 5 is not native.");
                        x = str.c_str();
                        testNative(x, name.str().c_str(), value, sign);
                    }
                    {
                        std::ostringstream name;
                        name << "C++ string = \"" << str << "\"";
                        IntType y(5);
                        if (! y.isNative())
                            CPPUNIT_FAIL("Hard-coded 5 is not native.");
                        y = str;
                        testNative(y, name.str().c_str(), value, sign);
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
        void testStringLarge(const std::string& s, int sign) {
            bool valid;
            std::string str;
            for (int i = 0; i < 4; ++i) {
                if (i < 2)
                    str = s;
                else {
                    // Try with whitespace prepended.
                    str = " \t\r\n  ";
                    str += s;
                }
                if (i % 2)
                    str += " \t\r\n  "; // and whitespace appended.

                {
                    std::ostringstream name;
                    name << "C string \"" << str << "\"";
                    IntType x(str.c_str(), 10, &valid);
                    if (! valid) {
                        std::ostringstream msg;
                        msg << name << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    testLarge(x, name.str().c_str(), s, sign);
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
                    testLarge(x, name.str().c_str(), s, sign);
                }
                {
                    std::ostringstream name;
                    name << "C string = \"" << str << "\"";
                    IntType x(5);
                    if (! x.isNative())
                        CPPUNIT_FAIL("Hard-coded 5 is not native.");
                    x = str.c_str();
                    testLarge(x, name.str().c_str(), s, sign);
                }
                {
                    std::ostringstream name;
                    name << "C++ string = \"" << str << "\"";
                    IntType y(5);
                    if (! y.isNative())
                        CPPUNIT_FAIL("Hard-coded 5 is not native.");
                    y = str;
                    testLarge(y, name.str().c_str(), s, sign);
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
                const std::string& valueBase10, int sign) {
            bool valid;
            std::string str;
            for (int i = 0; i < 4; ++i) {
                if (i < 2)
                    str = s;
                else {
                    // Try with whitespace prepended.
                    str = " \t\r\n  ";
                    str += s;
                }
                if (i % 2)
                    str += " \t\r\n  "; // and whitespace appended.

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
                    testLarge(x, name.str().c_str(), valueBase10, sign);
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
                    testLarge(x, name.str().c_str(), valueBase10, sign);
                }
                if (base == 10) {
                    {
                        std::ostringstream name;
                        name << "C string = \"" << str << "\"";
                        IntType x(5);
                        if (! x.isNative())
                            CPPUNIT_FAIL("Hard-coded 5 is not native.");
                        x = str.c_str();
                        testLarge(x, name.str().c_str(), s, sign);
                    }
                    {
                        std::ostringstream name;
                        name << "C++ string = \"" << str << "\"";
                        IntType y(5);
                        if (! y.isNative())
                            CPPUNIT_FAIL("Hard-coded 5 is not native.");
                        y = str;
                        testLarge(y, name.str().c_str(), s, sign);
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
        void constructAssignCopyString() {
            testStringNative<IntType>(sLongMax, 10, LONG_MAX, 1);
            testStringNative<IntType>(sLongMin, 10, LONG_MIN, -1);
            testStringLarge<IntType>(sLongMaxInc, 1);
            testStringLarge<IntType>(sULongMax, 1);
            testStringLarge<IntType>(HUGE_INTEGER, 1);
            testStringLarge<IntType>("-" HUGE_INTEGER, -1);

            // Test string constructors in different bases.
            testStringNative<IntType>("101", 2, 5, 1);
            testStringNative<IntType>("-101", 2, -5, -1);
            testStringNative<IntType>("121", 3, 16, 1);
            testStringNative<IntType>("-121", 3, -16, -1);
            testStringNative<IntType>("1af", 16, 431, 1);
            testStringNative<IntType>("-1af", 16, -431, -1);
            testStringNative<IntType>("201", 31, 1923, 1);
            testStringNative<IntType>("-201", 31, -1923, -1);
            testStringNative<IntType>("121", 0, 121, 1);
            testStringNative<IntType>("-121", 0, -121, -1);
            testStringNative<IntType>("034", 0, 28, 1);
            testStringNative<IntType>("-034", 0, -28, -1);
            testStringNative<IntType>("0x1af", 0, 431, 1);
            testStringNative<IntType>("-0x1af", 0, -431, -1);
            testStringNative<IntType>("0X1af", 0, 431, 1);
            testStringNative<IntType>("-0X1af", 0, -431, -1);
            testStringLarge<IntType>("1000000000000000000000000000000", 29,
                "74462898441675122902293018227199467668020601", 1);
            testStringLarge<IntType>("-1000000000000000000000000000000", 29,
                "-74462898441675122902293018227199467668020601", -1);
            testStringLarge<IntType>(
                "74462898441675122902293018227199467668020601",
                0, "74462898441675122902293018227199467668020601", 1);
            testStringLarge<IntType>(
                "-74462898441675122902293018227199467668020601",
                0, "-74462898441675122902293018227199467668020601", -1);
            testStringLarge<IntType>(
                "01000000000000000000000000000000000000000000000",
                0, "43556142965880123323311949751266331066368", 1);
            testStringLarge<IntType>(
                "-01000000000000000000000000000000000000000000000",
                0, "-43556142965880123323311949751266331066368", -1);
            testStringLarge<IntType>("0x10000000000000000000000000000000000",
                0, "87112285931760246646623899502532662132736", 1);
            testStringLarge<IntType>("-0x10000000000000000000000000000000000",
                0, "-87112285931760246646623899502532662132736", -1);
            testStringLarge<IntType>("0X10000000000000000000000000000000000",
                0, "87112285931760246646623899502532662132736", 1);
            testStringLarge<IntType>("-0X10000000000000000000000000000000000",
                0, "-87112285931760246646623899502532662132736", -1);
        }

        void testInfinity(const NLargeInteger& x, const char* name,
                bool testCopy = true) {
            if (x.isNative()) {
                std::ostringstream msg;
                msg << name << " is native.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! x.isInfinite()) {
                std::ostringstream msg;
                msg << name << " is reported as non-infinite.";
                CPPUNIT_FAIL(msg.str());
            }

            if (x.stringValue() != "inf") {
                std::ostringstream msg;
                msg << name << " != inf as a string.";
                CPPUNIT_FAIL(msg.str());
            }

            {
                std::ostringstream out;
                out << x;
                if (out.str() != "inf") {
                    std::ostringstream msg;
                    msg << name << " != inf on an ostream.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (x.isZero()) {
                std::ostringstream msg;
                msg << name << " is zero.";
                CPPUNIT_FAIL(msg.str());
            }

            if (testCopy) {
                // Test the copy constructor and copy assignment here
                // also.
                NLargeInteger y(x);
                testInfinity(y, "Native copy", false);

                NLargeInteger z(5);
                if (! z.isNative())
                    CPPUNIT_FAIL("Hard-coded 5 is not native.");
                z = x;
                testInfinity(z, "Native = from native", false);
                z = 7;
                testNative(z, "(5 = inf) = 7)", 7, 1);
                z = x;
                testInfinity(z, "Native = from native", false);
                z = HUGE_INTEGER;
                testLarge(z, "(7 = inf) = HUGE)", HUGE_INTEGER, 1);
                z = x;
                testInfinity(z, "Native = from large", false);
                z = "-"HUGE_INTEGER;
                testLarge(z, "(HUGE = inf) = -HUGE)", "-"HUGE_INTEGER, -1);
                z = x;
                testInfinity(z, "Native = from large", false);
                z = 8;
                testNative(z, "(-HUGE = inf) = 8)", 8, 1);
            }
        }

        void constructAssignCopyInfinity() {
            testInfinity(NLargeInteger::infinity, "Static infinity");

            NLargeInteger x(5);
            if (! x.isNative())
                CPPUNIT_FAIL("Hard-coded 5 is not native.");
            x.makeInfinite();
            testInfinity(x, "5.makeInfinite()");

            NLargeInteger y(HUGE_INTEGER);
            if (y.isNative())
                CPPUNIT_FAIL("Hard-coded HUGE_INTEGER is not native.");
            y.makeInfinite();
            testInfinity(y, "HUGE_INTEGER.makeInfinite()");

            NLargeInteger z(NLargeInteger::infinity);
            if (! z.isInfinite())
                CPPUNIT_FAIL("Hard-coded infinity is not infinite.");
            z.makeInfinite();
            testInfinity(z, "inf.makeInfinite()");
        }

        template <typename IntType>
        void constructSpecial() {
            const Data<IntType>& d(data<IntType>());

            // Make sure that our "special case" data members look correct,
            // so we can use them with confidence throughout this class.
            testNative(d.zero, "Special case 0", 0, 0);
            testNative(d.one, "Special case 1", 1, 1);
            testNative(d.two, "Special case 2", 2, 1);
            testNative(d.negOne, "Special case -1", -1, -1);
            testNative(d.negTwo, "Special case -2", -2, -1);
            testNative(d.longMax, "Special case LONG_MAX", LONG_MAX, 1);
            if ((d.longMax.longValue() + 1) >= zeroL)
                CPPUNIT_FAIL("Special case LONG_MAX does not "
                    "wrap around correctly.");
            testNative(d.longMin, "Special case LONG_MIN", LONG_MIN, -1);
            if ((d.longMin.longValue() - 1) <= zeroL)
                CPPUNIT_FAIL("Special case LONG_MIN does not "
                    "wrap around correctly.");
            testLarge(d.longMaxInc, "Special case LONG_MAX+1", sLongMaxInc, 1);
            if (d.longMaxInc <= LONG_MAX)
                CPPUNIT_FAIL("Special case LONG_MAX+1 has overflowed.");
            testLarge(d.longMinDec, "Special case LONG_MIN-1", sLongMinDec, -1);
            if (d.longMinDec >= LONG_MIN)
                CPPUNIT_FAIL("Special case LONG_MIN-1 has overflowed.");
            testLarge(d.ulongMax, "Special case ULONG_MAX", sULongMax, 1);
            if (d.ulongMax <= LONG_MAX)
                CPPUNIT_FAIL("Special case ULONG_MAX has overflowed.");
            testLarge(d.hugePos, "Special case HUGE", HUGE_INTEGER, 1);
            if (d.hugePos <= LONG_MAX)
                CPPUNIT_FAIL("Special case HUGE has overflowed.");
            testLarge(d.hugeNeg, "Special case -HUGE", "-"HUGE_INTEGER, -1);
            if (d.hugeNeg >= LONG_MIN)
                CPPUNIT_FAIL("Special case -HUGE has overflowed.");
            testLarge(-d.hugeNeg, "-(special case -HUGE)", HUGE_INTEGER, 1);
        }

        template <typename IntType>
        void testStringValue(const IntType& x, int base, const char* value) {
            std::string ans = x.stringValue(base);
            if (ans != value) {
                std::ostringstream msg;
                msg << "Conversion of " << x << " to base " << base
                    << " is " << ans << ", not " << value << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        template <typename IntType>
        void stringValue() {
            // We've already tested stringValue() heavily with the
            // default base of 10.  Here we test other bases.

            testStringValue(IntType(0), 2, "0");
            testStringValue(IntType(0), 3, "0");
            testStringValue(IntType(0), 21, "0");
            testStringValue(IntType(0), 22, "0");
            testStringValue(IntType(0), 36, "0");
            testStringValue(IntType(42), 2, "101010");
            testStringValue(IntType(42), 3, "1120");
            testStringValue(IntType(42), 21, "20");
            testStringValue(IntType(42), 22, "1k");
            testStringValue(IntType(42), 36, "16");
            testStringValue(IntType(71), 36, "1z");
            testStringValue(IntType(-42), 2, "-101010");
            testStringValue(IntType(-42), 3, "-1120");
            testStringValue(IntType(-42), 21, "-20");
            testStringValue(IntType(-42), 22, "-1k");
            testStringValue(IntType(-42), 36, "-16");
            testStringValue(IntType(-71), 36, "-1z");

            // In the following tests, the hard-coded integer is 2^130.
            IntType pos("1361129467683753853853498429727072845824");
            IntType neg("-1361129467683753853853498429727072845824");
            testStringValue(pos, 2,
                "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
            testStringValue(pos, 4,
                "100000000000000000000000000000000000000000000000000000000000000000");
            testStringValue(pos, 16,
                "400000000000000000000000000000000");
            testStringValue(pos, 32,
                "100000000000000000000000000");
            testStringValue(pos, 36,
                "1omfro7zwmumr3umxudzyj6scg");
            testStringValue(neg, 2,
                "-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
            testStringValue(neg, 4,
                "-100000000000000000000000000000000000000000000000000000000000000000");
            testStringValue(neg, 16,
                "-400000000000000000000000000000000");
            testStringValue(neg, 32,
                "-100000000000000000000000000");
            testStringValue(neg, 36,
                "-1omfro7zwmumr3umxudzyj6scg");

            // Test infinity (which must be hard-coded to NLargeInteger).
            for (int i = 2; i <= 36; ++i)
                if (NLargeInteger::infinity.stringValue(i) != "inf") {
                    std::ostringstream msg;
                    msg << "Conversion of infinity to base " << i
                        << " is not \"inf\".";
                    CPPUNIT_FAIL(msg.str());
                }
        }

        template <typename IntType>
        void swap() {
            {
                IntType a(3);
                IntType b(LONG_MIN);
                IntType c(sLongMaxInc);
                IntType d(HUGE_INTEGER);
                IntType e(1000);

                testNative(a, "3", 3, 1);
                testNative(b, "LONG_MIN", LONG_MIN, -1);
                testLarge(c, "LONG_MAX+1", sLongMaxInc, 1);
                testLarge(d, "HUGE", HUGE_INTEGER, 1);
                testNative(e, "1000", 1000, 1);

                std::swap(a, b); // native <-> native

                testNative(b, "3", 3, 1);
                testNative(a, "LONG_MIN", LONG_MIN, -1);

                std::swap(a, c); // native <-> long

                testLarge(a, "LONG_MAX+1", sLongMaxInc, 1);
                testNative(c, "LONG_MIN", LONG_MIN, -1);

                std::swap(a, d); // long <-> long

                testLarge(a, "HUGE", HUGE_INTEGER, 1);
                testLarge(d, "LONG_MAX+1", sLongMaxInc, 1);

                std::swap(a, e); // long <-> native

                testNative(a, "1000", 1000, 1);
                testLarge(e, "HUGE", HUGE_INTEGER, 1);

                std::swap(a, a); // long <-> self

                testNative(a, "1000", 1000, 1);

                std::swap(e, e); // large <-> self

                testLarge(e, "HUGE", HUGE_INTEGER, 1);
            }

            // Tests for infinity are hard-coded to NLargeInteger.
            {
                NLargeInteger a(3);
                NLargeInteger b(LONG_MIN);
                NLargeInteger c(sLongMaxInc);
                NLargeInteger d(HUGE_INTEGER);
                NLargeInteger i(NLargeInteger::infinity);
                NLargeInteger j;
                j.makeInfinite();

                testNative(a, "3", 3, 1);
                testNative(b, "LONG_MIN", LONG_MIN, -1);
                testLarge(c, "LONG_MAX+1", sLongMaxInc, 1);
                testLarge(d, "HUGE", HUGE_INTEGER, 1);
                testInfinity(i, "infinity");
                testInfinity(j, "infinity");

                std::swap(a, i); // native <-> infinity

                testInfinity(a, "infinity");
                testNative(i, "3", 3, 1);

                std::swap(c, a); // long <-> infinity

                testLarge(a, "LONG_MAX+1", sLongMaxInc, 1);
                testInfinity(c, "infinity");

                std::swap(c, d); // infinity <-> long

                testLarge(c, "HUGE", HUGE_INTEGER, 1);
                testInfinity(d, "infinity");

                std::swap(d, b); // infinity <-> native

                testInfinity(b, "infinity");
                testNative(d, "LONG_MIN", LONG_MIN, -1);

                std::swap(b, j); // infinity <-> infinity

                testInfinity(b, "infinity");
                testInfinity(j, "infinity");

                std::swap(b, b); // infinity <-> self

                testInfinity(b, "infinity");
            }
        }

        template <typename IntType>
        void shouldBeLess(const IntType& a, const IntType& b,
                bool reorder = true) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + a.stringValue() + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "== " + b.stringValue() + ".",
                ! (a == b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not != " + b.stringValue() + ".",
                a != b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not < " + b.stringValue() + ".",
                a < b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not <= " + b.stringValue() + ".",
                a <= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "> " + b.stringValue() + ".",
                ! (a > b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + ">= " + b.stringValue() + ".",
                ! (a >= b));

            if (reorder)
                shouldBeGreater(b, a, false);
        }

        template <typename IntType>
        void shouldBeEqual(const IntType& a, const IntType& b,
                bool reorder = true) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + a.stringValue() + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "not == " + b.stringValue() + ".",
                a == b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "!= " + b.stringValue() + ".",
                ! (a != b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "< " + b.stringValue() + ".",
                ! (a < b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not <= " + b.stringValue() + ".",
                a <= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "> " + b.stringValue() + ".",
                ! (a > b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not >= " + b.stringValue() + ".",
                a >= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not str== " +
                b.stringValue() + " (long).",
                a.stringValue() == b.stringValue());

            if (reorder)
                shouldBeEqual(b, a, false);
        }

        template <typename IntType>
        void shouldBeGreater(const IntType& a, const IntType& b,
                bool reorder = true) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + a.stringValue() + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "== " + b.stringValue() + ".",
                ! (a == b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not != " + b.stringValue() + ".",
                a != b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "< " + b.stringValue() + ".",
                ! (a < b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "<= " + b.stringValue() + ".",
                ! (a <= b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not > " + b.stringValue() + ".",
                a > b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not >= " + b.stringValue() + ".",
                a >= b);

            if (reorder)
                shouldBeLess(b, a, false);
        }

        template <typename IntType>
        void shouldBeLess(const IntType& a, long b) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + a.stringValue() + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "== " + str(b) + " (long).",
                ! (a == b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not != " + str(b) + " (long).",
                a != b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not < " + str(b) + " (long).",
                a < b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not <= " + str(b) + " (long).",
                a <= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "> " + str(b) + " (long).",
                ! (a > b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + ">= " + str(b) + " (long).",
                ! (a >= b));
        }

        template <typename IntType>
        void shouldBeEqual(const IntType& a, long b) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + a.stringValue() + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "not == " + str(b) + " (long).",
                a == b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "!= " + str(b) + " (long).",
                ! (a != b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "< " + str(b) + " (long).",
                ! (a < b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not <= " + str(b) + " (long).",
                a <= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "> " + str(b) + " (long).",
                ! (a > b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not >= " + str(b) + " (long).",
                a >= b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not str== " + str(b) + " (long).",
                a.stringValue() == str(b));

            if (b == 0)
                CPPUNIT_ASSERT_MESSAGE(msgBase + "not isZero().", a.isZero());
            else
                CPPUNIT_ASSERT_MESSAGE(msgBase + "isZero().", ! a.isZero());
        }

        template <typename IntType>
        void shouldBeGreater(const IntType& a, long b) {
            std::string msgBase = "Integer ";
            msgBase = msgBase + a.stringValue() + " is ";

            CPPUNIT_ASSERT_MESSAGE(msgBase + "== " + str(b) + " (long).",
                ! (a == b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not != " + str(b) + " (long).",
                a != b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "< " + str(b) + " (long).",
                ! (a < b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "<= " + str(b) + " (long).",
                ! (a <= b));
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not > " + str(b) + " (long).",
                a > b);
            CPPUNIT_ASSERT_MESSAGE(msgBase + "not >= " + str(b) + " (long).",
                a >= b);
        }

        template <typename IntType>
        void comparisons() {
            unsigned a, b;

            const IntType& zero(IntType::zero);
            const IntType& one(IntType::one);
            const Data<IntType>& d(data<IntType>());

            shouldBeLess(zero, 1L);
            shouldBeEqual(zero, 0L);
            shouldBeGreater(zero, -1L);
            shouldBeLess(one, 2L);
            shouldBeEqual(one, 1L);
            shouldBeGreater(one, 0L);
            shouldBeGreater(one, -1L);
            shouldBeLess(zero, one);
            shouldBeEqual(zero, zero);
            shouldBeEqual(one, one);

            for (a = 0; a < d.nCases; ++a) {
                IntType x(d.cases[a]);
                shouldBeEqual(x, x);
                IntType y(d.cases[a]);
                shouldBeEqual(x, y);
                if (x.isNative())
                    shouldBeEqual(x, x.longValue());
            }

            for (a = 0; a < d.nCases; ++a)
                for (b = a + 1; b < d.nCases; ++b) {
                    IntType x(d.cases[a]);
                    IntType y(d.cases[b]);
                    shouldBeLess(x, y);
                    if (x.isNative())
                        shouldBeGreater(y, x.longValue());
                    if (y.isNative())
                        shouldBeLess(x, y.longValue());
                    if (x.isNative() && y.isNative() &&
                            x.longValue() >= y.longValue()) {
                        std::ostringstream msg;
                        msg << "Long values for cases #" << a
                            << " and #" << b << " are not properly ordered.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

            for (a = 0; a < d.nLongCases; ++a) {
                IntType x(d.longCases[a]);
                shouldBeEqual(x, d.longCases[a]);
                x.makeLarge();
                shouldBeEqual(x, d.longCases[a]);
            }

            for (a = 0; a < d.nLongCases; ++a)
                for (b = a + 1; b < d.nLongCases; ++b) {
                    IntType x(d.longCases[a]);
                    shouldBeLess(x, d.longCases[b]);
                    x.makeLarge();
                    shouldBeLess(x, d.longCases[b]);

                    IntType y(d.longCases[b]);
                    shouldBeGreater(y, d.longCases[a]);
                    y.makeLarge();
                    shouldBeGreater(y, d.longCases[a]);
                }

            // Tests for infinity are hard-coded to NLargeInteger.
            const NLargeInteger& infinity(NLargeInteger::infinity);

            for (a = 0; a < dataL.nCases; a++)
                shouldBeGreater(infinity, dataL.cases[a]);
            for (a = 0; a < dataL.nLongCases; a++)
                shouldBeGreater(infinity, dataL.longCases[a]);

            shouldBeEqual(infinity, NLargeInteger::infinity);
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

            for (int a = 0; a < d.nCases; a++)
                testIncDec(d.cases[a]);

            // Tests for infinity are hard-coded to NLargeInteger.
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
        void plusMinus() {
            unsigned a, b;

            const Data<IntType>& d(data<IntType>());

            IntType tmp;
            for (a = 0; a < d.nCases; ++a) {
                for (b = 0; b < d.nCases; ++b) {
                    IntType x(d.cases[a]);
                    IntType y(d.cases[b]);

                    shouldBeEqual((x + y) - y, x);
                    shouldBeEqual((x - y) + y, x);
                    shouldBeEqual((x + y) - x, y);
                    shouldBeEqual((x - y) - x, -y);
                    shouldBeEqual(((x + y) - x) - y, 0L);
                    shouldBeEqual(((x + y) - y) - x, 0L);
                    shouldBeEqual(((x - y) - x) + y, 0L);
                    shouldBeEqual(((x - y) + y) - x, 0L);
                }

                for (b = 0; b < d.nLongCases; ++b) {
                    IntType x(d.cases[a]);
                    long y = d.longCases[b];

                    shouldBeEqual((x + y) - y, x);
                    shouldBeEqual((x - y) + y, x);
                    shouldBeEqual((x + y) - x, y);
                    shouldBeEqual(-((x - y) - x), y); // -y could overflow
                    shouldBeEqual(((x + y) - x) - y, 0L);
                    shouldBeEqual(((x + y) - y) - x, 0L);
                    shouldBeEqual(((x - y) - x) + y, 0L);
                    shouldBeEqual(((x - y) + y) - x, 0L);
                }

                IntType z(d.cases[a]);
                shouldBeEqual(z + 0L, z);
                shouldBeEqual(z - 0L, z);
                shouldBeEqual(0L + z, z);
                shouldBeEqual(z + IntType::zero, z);
                shouldBeEqual(z - IntType::zero, z);
                shouldBeEqual(IntType::zero + z, z);
            }

            // TODO: Tests:
            // {native, large} {+,-} {native, large}
            // {negative, positive} {+,-} {negative, positive}
            // overflow points
            // +, +=, -, -=

            // Tests for infinity are hard-coded to NLargeInteger.
            // TODO: Here down.
            const NLargeInteger& infinity(NLargeInteger::infinity);

            for (a = 0; a < dataL.nCases; a++)
                shouldBeGreater(infinity, dataL.cases[a]);
            for (a = 0; a < dataL.nLongCases; a++)
                shouldBeGreater(infinity, dataL.longCases[a]);

            shouldBeEqual(infinity, NLargeInteger::infinity);
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

// Out-of-line definitions of static arrays.

template <typename IntType>
const IntType NIntegerTest::Data<IntType>::cases[] = {
    // Too low for a native long:
    "-"ENORMOUS_INTEGER,
    "-"HUGE_INTEGER,
    -IntType(static_cast<unsigned long>(ULONG_MAX)),
    -IntType(static_cast<unsigned long>(LONG_MAX + 2)),
    // Fit into a native long:
    LONG_MIN, -LONG_MAX, -32768, -5000, -1000, -3, -2, -1,
    0,
    1, 2, 3, 1000, 5000, 32768, LONG_MAX-1, LONG_MAX,
    // Too large for a native long:
    static_cast<unsigned long>(LONG_MAX + 1),
    static_cast<unsigned long>(ULONG_MAX),
    HUGE_INTEGER,
    ENORMOUS_INTEGER
};

template <typename IntType>
const unsigned NIntegerTest::Data<IntType>::nCases = 25;

template <typename IntType>
const long NIntegerTest::Data<IntType>::longCases[] = {
    LONG_MIN, -LONG_MAX, -32768, -5000, -1000, -3, -2, -1,
    0,
    1, 2, 3, 1000, 5000, 32768, LONG_MAX-1, LONG_MAX
};

template <typename IntType>
const long NIntegerTest::Data<IntType>::nLongCases = 17;

// Boilerplate stuff.

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

