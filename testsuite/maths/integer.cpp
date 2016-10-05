
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
#include "maths/integer.h"
#include "testsuite/utilities/testutilities.h"

using regina::IntegerBase;
using regina::Integer;
using regina::LargeInteger;

#define ENORMOUS_INTEGER "115792089237316195423570985008687907853269984665640564039457584007913129639936"
#define HUGE_INTEGER "12364981726394781629378461923786491874569283746672"
#define ZERO3 "000"
#define ZERO4 "0000"
#define ZERO7 "0000000"
#define ZERO8 "00000000"
#define ZERO15 "000000000000000"
#define ZERO16 "0000000000000000"
#define ZERO31 ZERO15 ZERO16
#define ZERO32 ZERO16 ZERO16
#define ZERO63 ZERO31 ZERO32
#define ZERO64 ZERO32 ZERO32
#define ZEROES ZERO64 ZERO64 ZERO64 ZERO64
#define F3 "FFF"
#define F4 "FFFF"
#define F7 "FFFFFFF"
#define F8 "FFFFFFFF"
#define F15 "FFFFFFFFFFFFFFF"
#define F16 "FFFFFFFFFFFFFFFF"
#define F31 F15 F16
#define F32 F16 F16
#define F63 F31 F32
#define F64 F32 F32

class IntegerTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(IntegerTest);

    CPPUNIT_TEST(constructAssignCopyNative<Integer>);
    CPPUNIT_TEST(constructAssignCopyNative<LargeInteger>);
    CPPUNIT_TEST(constructAssignCopyString<Integer>);
    CPPUNIT_TEST(constructAssignCopyString<LargeInteger>);
    CPPUNIT_TEST(constructAssignCopyInfinity);
    CPPUNIT_TEST(constructSpecial<Integer>);
    CPPUNIT_TEST(constructSpecial<LargeInteger>);
#ifdef INT128_AVAILABLE
    CPPUNIT_TEST(constructNative128<Integer>);
    CPPUNIT_TEST(constructNative128<LargeInteger>);
    CPPUNIT_TEST(gcdNative128);
#endif
    CPPUNIT_TEST(stringValue<Integer>);
    CPPUNIT_TEST(stringValue<LargeInteger>);
    CPPUNIT_TEST(swap<Integer>);
    CPPUNIT_TEST(swap<LargeInteger>);
    CPPUNIT_TEST(comparisons<Integer>);
    CPPUNIT_TEST(comparisons<LargeInteger>);
    CPPUNIT_TEST(incDec<Integer>);
    CPPUNIT_TEST(incDec<LargeInteger>);
    CPPUNIT_TEST(plusMinus<Integer>);
    CPPUNIT_TEST(plusMinus<LargeInteger>);
    CPPUNIT_TEST(multiply<Integer>);
    CPPUNIT_TEST(multiply<LargeInteger>);
    CPPUNIT_TEST(divide<Integer>);
    CPPUNIT_TEST(divide<LargeInteger>);
    CPPUNIT_TEST(mod<Integer>);
    CPPUNIT_TEST(mod<LargeInteger>);
    CPPUNIT_TEST(negate<Integer>);
    CPPUNIT_TEST(negate<LargeInteger>);
    CPPUNIT_TEST(abs<Integer>);
    CPPUNIT_TEST(abs<LargeInteger>);
    CPPUNIT_TEST(divisionAlg<Integer>);
    CPPUNIT_TEST(divisionAlg<LargeInteger>);
    CPPUNIT_TEST(gcdLcm<Integer>);
    CPPUNIT_TEST(gcdLcm<LargeInteger>);
    CPPUNIT_TEST(raiseToPower<Integer>);
    CPPUNIT_TEST(raiseToPower<LargeInteger>);
    CPPUNIT_TEST(tryReduce<Integer>);
    CPPUNIT_TEST(tryReduce<LargeInteger>);
    CPPUNIT_TEST(makeLarge<Integer>);
    CPPUNIT_TEST(makeLarge<LargeInteger>);
    CPPUNIT_TEST(nativeVsLarge<Integer>);
    CPPUNIT_TEST(nativeVsLarge<LargeInteger>);

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
        static const size_t nZero; // The size of f2k[], zero2k[], zero2kdec[].
        static const std::string f2k[5]; // 2^k Fs, for a range of k.
        static const std::string zero2k[5]; // 2^k zeroes, for a range of k.
        static const std::string zero2kdec[5]; // One less zero than zero2k[i].

        Data<LargeInteger> dataL;
        Data<Integer> dataI;

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

            if (x.sign() != sign) {
                std::ostringstream msg;
                msg << name << " has the wrong sign.";
                CPPUNIT_FAIL(msg.str());
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

                // Test raw GMP assignment.
                IntType v(5);
                v.setRaw(y.rawData());
                shouldBeEqual(v, y);
                testLarge(y, "Raw GMP assignment src", str(value), sign, false);
                testLarge(v, "Raw GMP assignment dest", str(value), sign,
                    false);

                IntType u(HUGE_INTEGER);
                u.setRaw(y.rawData());
                shouldBeEqual(u, y);
                testLarge(y, "Raw GMP assignment src", str(value), sign, false);
                testLarge(u, "Raw GMP assignment dest", str(value), sign,
                    false);
            }
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

            if (x.sign() != sign) {
                std::ostringstream msg;
                msg << name << " has the wrong sign.";
                CPPUNIT_FAIL(msg.str());
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

                // Test raw GMP assignment.
                IntType v(5);
                v.setRaw(y.rawData());
                shouldBeEqual(v, y);
                testLarge(y, "Raw GMP assignment src", str(value), sign, false);
                testLarge(v, "Raw GMP assignment dest", str(value), sign,
                    false);

                IntType u(HUGE_INTEGER);
                u.setRaw(y.rawData());
                shouldBeEqual(u, y);
                testLarge(y, "Raw GMP assignment src", str(value), sign, false);
                testLarge(u, "Raw GMP assignment dest", str(value), sign,
                    false);
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
                        msg << name.str() << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (base > 0 && x.stringValue(base) != s) {
                        std::ostringstream msg;
                        msg << name.str() << " has incorrect stringValue(base).";
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
                        msg << name.str() << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (base > 0 && x.stringValue(base) != s) {
                        std::ostringstream msg;
                        msg << name.str() << " has incorrect stringValue(base).";
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
                        msg << name.str() << " is not valid.";
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
                        msg << name.str() << " is not valid.";
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
                        msg << name.str() << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (base > 0 && x.stringValue(base) != s) {
                        std::ostringstream msg;
                        msg << name.str() << " has incorrect stringValue(base).";
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
                        msg << name.str() << " is not valid.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (base > 0 && x.stringValue(base) != s) {
                        std::ostringstream msg;
                        msg << name.str() << " has incorrect stringValue(base).";
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

        void testInfinity(const LargeInteger& x, const char* name,
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

            if (x.sign() != 1) {
                std::ostringstream msg;
                msg << name << " has the wrong sign.";
                CPPUNIT_FAIL(msg.str());
            }

            if (testCopy) {
                // Test the copy constructor and copy assignment here
                // also.
                LargeInteger y(x);
                testInfinity(y, "Native copy", false);

                LargeInteger z(5);
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
                z = "-" HUGE_INTEGER;
                testLarge(z, "(HUGE = inf) = -HUGE)", "-" HUGE_INTEGER, -1);
                z = x;
                testInfinity(z, "Native = from large", false);
                z = 8;
                testNative(z, "(-HUGE = inf) = 8)", 8, 1);

                // Test raw GMP assignment.
                z = x;
                testInfinity(z, "Native = from native", false);
                LargeInteger raw(HUGE_INTEGER);
                z.setRaw(raw.rawData());
                testLarge(z, "inf.setRaw()", HUGE_INTEGER, 1, false);
            }
        }

        void constructAssignCopyInfinity() {
            testInfinity(LargeInteger::infinity, "Static infinity");

            LargeInteger x(5);
            if (! x.isNative())
                CPPUNIT_FAIL("Hard-coded 5 is not native.");
            x.makeInfinite();
            testInfinity(x, "5.makeInfinite()");

            LargeInteger y(HUGE_INTEGER);
            if (y.isNative())
                CPPUNIT_FAIL("Hard-coded HUGE_INTEGER is not native.");
            y.makeInfinite();
            testInfinity(y, "HUGE_INTEGER.makeInfinite()");

            LargeInteger z(LargeInteger::infinity);
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
            testLarge(d.hugeNeg, "Special case -HUGE", "-" HUGE_INTEGER, -1);
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

#ifdef INT128_AVAILABLE
        template <typename IntType>
        void test128Value(const regina::NativeInteger<16>& x,
                const regina::NativeInteger<16>& y) {
            if (x != y) {
                std::ostringstream msg;
                msg << "128-bit integers " << x << " vs " << y
                    << " give different wrapped values.";
                CPPUNIT_FAIL(msg.str());
            }
            if (x.nativeValue() != y.nativeValue()) {
                std::ostringstream msg;
                msg << "128-bit integers " << x << " vs " << y
                    << " give different native values.";
                CPPUNIT_FAIL(msg.str());
            }
            if (str(x) != str(y)) {
                std::ostringstream msg;
                msg << "128-bit integers " << x << " vs " << y
                    << " give different string values.";
                CPPUNIT_FAIL(msg.str());
            }
        }


        template <typename IntType>
        void testNative128(const regina::NativeInteger<16>& native,
                const char* string) {
            if (str(native) != string) {
                std::ostringstream msg;
                msg << "128-bit integer " << native <<
                    " has incorrect string representation.";
                CPPUNIT_FAIL(msg.str());
            }

            testStringValue(IntType(native), 10, string);

            test128Value<IntType>(native, regina::NativeInteger<16>(
                IntType(string)));
            test128Value<IntType>(native, regina::NativeInteger<16>(
                IntType(string).template nativeValue<16>()));
            test128Value<IntType>(native, regina::NativeInteger<16>(
                IntType(native).template nativeValue<16>()));
        }

        template <typename IntType>
        void constructNative128() {
            // Test conversion from native types that are larger than long.
            regina::NativeInteger<16> pos62 = 1;
            regina::NativeInteger<16> pos63 = 1;
            regina::NativeInteger<16> pos64 = 1;
            regina::NativeInteger<16> pos126 = 1;
            regina::NativeInteger<16> neg62 = 1;
            regina::NativeInteger<16> neg63 = 1;
            regina::NativeInteger<16> neg64 = 1;
            regina::NativeInteger<16> neg126 = 1;
            regina::NativeInteger<16> pos127 = 1;
            regina::NativeInteger<16> neg127 = 1;
            regina::NativeInteger<16> maxVal(
                ~(regina::IntOfSize<16>::type(1) << 127));
            pos62 *= 1073741824; // 2^30
            pos62 *= 1073741824; // 2^30
            pos62 *= 4;
            neg62 = -pos62;
            pos63 = pos62 * 2;
            neg63 = -pos63;
            pos64 = pos63 * 2;
            neg64 = -pos64;
            pos126 = pos63 * pos63;
            neg126 = -pos126;
            pos127 = pos126 * 2; // Should overflow to -2^127
            neg127 = neg126 * 2;

            testNative128<IntType>(pos62, "4611686018427387904");
            testNative128<IntType>(neg62, "-4611686018427387904");
            testNative128<IntType>(pos63, "9223372036854775808");
            testNative128<IntType>(neg63, "-9223372036854775808");
            testNative128<IntType>(pos64, "18446744073709551616");
            testNative128<IntType>(neg64, "-18446744073709551616");
            testNative128<IntType>(pos126,
                "85070591730234615865843651857942052864");
            testNative128<IntType>(neg126,
                "-85070591730234615865843651857942052864");
            // Recall that pos127 overflows.
            testNative128<IntType>(pos127,
                "-170141183460469231731687303715884105728");
            testNative128<IntType>(neg127,
                "-170141183460469231731687303715884105728");
            testNative128<IntType>(maxVal,
                "170141183460469231731687303715884105727");
        }

        void gcdNative128() {
            // On arm64 with 128-bit integers, the implementation of
            // gcdWith() in Regina 4.96 gives gcd(3,3) = 55340232221128654851.
            // Ouch.

            regina::IntOfSize<16>::type n = 3;
            int shift = 0;
            n <<= shift;
            if (n != 3) {
                std::ostringstream msg;
                msg << "128-bit (3 <<= 0) gives "
                    << (long)(n >> 64) << ":" << (long)(n) << ".";
                CPPUNIT_FAIL(msg.str());
            }

            regina::NativeInteger<16> a, b;
            a = b = 3;
            a.gcdWith(b);
            if (a != 3) {
                std::ostringstream msg;
                msg << "128-bit gcd gives gcd(3,3) = " << a << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }
#endif

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

            // Test infinity (which must be hard-coded to LargeInteger).
            for (int i = 2; i <= 36; ++i)
                if (LargeInteger::infinity.stringValue(i) != "inf") {
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

            // Tests for infinity are hard-coded to LargeInteger.
            {
                LargeInteger a(3);
                LargeInteger b(LONG_MIN);
                LargeInteger c(sLongMaxInc);
                LargeInteger d(HUGE_INTEGER);
                LargeInteger i(LargeInteger::infinity);
                LargeInteger j;
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

            // Tests for infinity are hard-coded to LargeInteger.
            const LargeInteger& infinity(LargeInteger::infinity);

            for (a = 0; a < dataL.nCases; a++)
                shouldBeGreater(infinity, dataL.cases[a]);
            for (a = 0; a < dataL.nLongCases; a++)
                shouldBeGreater(infinity, dataL.longCases[a]);

            shouldBeEqual(infinity, LargeInteger::infinity);
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

            // Tests for infinity are hard-coded to LargeInteger.
            {
                LargeInteger i(LargeInteger::infinity);
                if (++i != LargeInteger::infinity)
                    CPPUNIT_FAIL("++inf does not return inf.");
                if (i != LargeInteger::infinity)
                    CPPUNIT_FAIL("++inf does not result in inf.");
            }
            {
                LargeInteger i(LargeInteger::infinity);
                if (i++ != LargeInteger::infinity)
                    CPPUNIT_FAIL("inf++ does not return inf.");
                if (i != LargeInteger::infinity)
                    CPPUNIT_FAIL("inf++ does not result in inf.");
            }
            {
                LargeInteger i(LargeInteger::infinity);
                if (--i != LargeInteger::infinity)
                    CPPUNIT_FAIL("--inf does not return inf.");
                if (i != LargeInteger::infinity)
                    CPPUNIT_FAIL("--inf does not result in inf.");
            }
            {
                LargeInteger i(LargeInteger::infinity);
                if (i-- != LargeInteger::infinity)
                    CPPUNIT_FAIL("inf-- does not return inf.");
                if (i != LargeInteger::infinity)
                    CPPUNIT_FAIL("inf-- does not result in inf.");
            }
        }

        template <typename IntType>
        void testPlusMinus(long a, long b, const IntType& ans) {
            shouldBeEqual(IntType(a) + long(b), ans);
            shouldBeEqual(IntType(b) + long(a), ans);
            shouldBeEqual(long(a) + IntType(b), ans);
            shouldBeEqual(long(b) + IntType(a), ans);
            shouldBeEqual(IntType(a) + IntType(b), ans);
            shouldBeEqual(IntType(b) + IntType(a), ans);
            shouldBeEqual(ans - long(a), b);
            shouldBeEqual(ans - long(b), a);
            shouldBeEqual(ans - IntType(a), b);
            shouldBeEqual(ans - IntType(b), a);
        }

        template <typename IntType>
        void testPlusMinus(const IntType& a, long b, const IntType& ans) {
            shouldBeEqual(a + long(b), ans);
            shouldBeEqual(long(b) + a, ans);
            shouldBeEqual(a + IntType(b), ans);
            shouldBeEqual(IntType(b) + a, ans);
            shouldBeEqual(ans - long(b), a);
            shouldBeEqual(ans - a, IntType(b));
            shouldBeEqual(ans - IntType(b), a);
        }

        template <typename IntType>
        void testPlusMinus(const IntType& a, const IntType& b,
                const IntType& ans) {
            shouldBeEqual(a + b, ans);
            shouldBeEqual(b + a, ans);
            shouldBeEqual(ans - b, a);
            shouldBeEqual(ans - a, b);
        }

        template <typename IntType>
        void plusMinus() {
            unsigned a, b;

            const Data<IntType>& d(data<IntType>());

            for (a = 0; a < d.nCases; ++a) {
                for (b = 0; b < d.nCases; ++b) {
                    IntType x(d.cases[a]);
                    IntType y(d.cases[b]);

                    shouldBeEqual(x + y, y + x);
                    shouldBeEqual(x - y, -(y - x));

                    shouldBeEqual((x + y) - y, x);
                    shouldBeEqual((x - y) + y, x);
                    shouldBeEqual((x + y) - x, y);
                    shouldBeEqual((x - y) - x, -y);
                    shouldBeEqual(((x + y) - x) - y, 0L);
                    shouldBeEqual(((x + y) - y) - x, 0L);
                    shouldBeEqual(((x - y) - x) + y, 0L);
                    shouldBeEqual(((x - y) + y) - x, 0L);

                    IntType p(x);
                    shouldBeEqual(p += y, x + y);
                    IntType q(x);
                    shouldBeEqual(q -= y, x - y);

                    if (y.sign() > 0) {
                        shouldBeGreater(x + y, x);
                        shouldBeLess(x - y, x);
                    } else if (y.sign() < 0) {
                        shouldBeLess(x + y, x);
                        shouldBeGreater(x - y, x);
                    } else {
                        shouldBeEqual(x + y, x);
                        shouldBeEqual(x - y, x);
                    }

                    if (x.sign() > 0) {
                        shouldBeGreater(x + y, y);
                        shouldBeGreater(x - y, -y);
                    } else if (x.sign() < 0) {
                        shouldBeLess(x + y, y);
                        shouldBeLess(x - y, -y);
                    } else {
                        shouldBeEqual(x + y, y);
                        shouldBeEqual(x - y, -y);
                    }
                }

                for (b = 0; b < d.nLongCases; ++b) {
                    IntType x(d.cases[a]);
                    long y = d.longCases[b];

                    shouldBeEqual(x + y, y + x);

                    shouldBeEqual((x + y) - y, x);
                    shouldBeEqual((x - y) + y, x);
                    shouldBeEqual((x + y) - x, y);
                    shouldBeEqual(-((x - y) - x), y); // -y could overflow
                    shouldBeEqual(((x + y) - x) - y, 0L);
                    shouldBeEqual(((x + y) - y) - x, 0L);
                    shouldBeEqual(((x - y) - x) + y, 0L);
                    shouldBeEqual(((x - y) + y) - x, 0L);

                    shouldBeEqual((y + x) - y, x);
                    shouldBeEqual((y + x) - x, y);
                    shouldBeEqual(((y + x) - x) - y, 0L);
                    shouldBeEqual(((y + x) - y) - x, 0L);

                    IntType p(x);
                    shouldBeEqual(p += y, x + y);
                    IntType q(x);
                    shouldBeEqual(q -= y, x - y);

                    if (y > 0) {
                        shouldBeGreater(x + y, x);
                        shouldBeGreater(y + x, x);
                        shouldBeLess(x - y, x);
                    } else if (y < 0) {
                        shouldBeLess(x + y, x);
                        shouldBeLess(y + x, x);
                        shouldBeGreater(x - y, x);
                    } else {
                        shouldBeEqual(x + y, x);
                        shouldBeEqual(y + x, x);
                        shouldBeEqual(x - y, x);
                    }

                    if (x.sign() > 0) {
                        shouldBeGreater(x + y, y);
                        shouldBeLess(-(x - y), y); // -y could overflow
                    } else if (x.sign() < 0) {
                        shouldBeLess(x + y, y);
                        shouldBeGreater(-(x - y), y); // -y could overflow
                    } else {
                        shouldBeEqual(x + y, y);
                        shouldBeEqual(-(x - y), y); // -y could overflow
                    }
                }

                IntType z(d.cases[a]);
                shouldBeEqual(z + 0L, z);
                shouldBeEqual(z - 0L, z);
                shouldBeEqual(0L + z, z);
                shouldBeEqual(z + IntType::zero, z);
                shouldBeEqual(z - IntType::zero, z);
                shouldBeEqual(IntType::zero + z, z);
            }

            // Ad-hoc tests for native {+,-} native:
            testPlusMinus(long(3), long(7), IntType(10));
            testPlusMinus(long(-3), long(7), IntType(4));
            testPlusMinus(long(3), long(-7), IntType(-4));
            testPlusMinus(long(-3), long(-7), IntType(-10));

            // Ad-hoc tests for large {+,-} native and native {+,-} large:
            testPlusMinus(IntType(ENORMOUS_INTEGER "0"), long(3),
                IntType(ENORMOUS_INTEGER "3"));
            testPlusMinus(IntType("-" ENORMOUS_INTEGER "10"), long(3),
                IntType("-" ENORMOUS_INTEGER "07"));
            testPlusMinus(IntType(ENORMOUS_INTEGER "10"), long(-3),
                IntType(ENORMOUS_INTEGER "07"));
            testPlusMinus(IntType("-" ENORMOUS_INTEGER "0"), long(-3),
                IntType("-" ENORMOUS_INTEGER "3"));

            testPlusMinus(IntType(ENORMOUS_INTEGER "10"),
                IntType("-" ENORMOUS_INTEGER "07"), IntType(3));
            testPlusMinus(IntType("-" ENORMOUS_INTEGER "10"),
                IntType(ENORMOUS_INTEGER "07"), IntType(-3));

            // Ad-hoc tests for large {+,-} large:
            testPlusMinus(IntType("3" ZEROES), IntType("7" ZEROES),
                IntType("10" ZEROES));
            testPlusMinus(IntType("3" ZEROES), IntType("-7" ZEROES),
                IntType("-4" ZEROES));
            testPlusMinus(IntType("-3" ZEROES), IntType("7" ZEROES),
                IntType("4" ZEROES));
            testPlusMinus(IntType("-3" ZEROES), IntType("-7" ZEROES),
                IntType("-10" ZEROES));

            // Test around overflow points:
            testPlusMinus(long(LONG_MAX), long(1), d.longMaxInc);
            testPlusMinus(long(LONG_MAX), long(0), d.longMax);
            testPlusMinus(long(LONG_MIN), long(-1), d.longMinDec);
            testPlusMinus(long(LONG_MIN), long(0), d.longMin);
            testPlusMinus(long(LONG_MAX), long(LONG_MIN), IntType(-1));
            testPlusMinus(d.longMaxInc, long(-1), d.longMax);
            testPlusMinus(d.longMinDec, long(1), d.longMin);
            testPlusMinus(d.longMaxInc, long(LONG_MIN), IntType(0));
            testPlusMinus(d.longMaxInc, long(LONG_MAX), d.ulongMax);
            testPlusMinus(d.longMaxInc, d.longMinDec, IntType(-1));
            shouldBeEqual(d.longMax + d.longMax + IntType(1), d.ulongMax);

            size_t i;
            for (i = 0; i < nZero; ++i) {
                testPlusMinus(
                    IntType(f2k[i], 16), long(1),
                    IntType(std::string("1") + zero2k[i], 16));
                testPlusMinus(
                    IntType(std::string("-") + f2k[i], 16), long(-1),
                    IntType(std::string("-1") + zero2k[i], 16));
                testPlusMinus(
                    IntType(std::string("1") + zero2k[i], 16), long(-1),
                    IntType(f2k[i], 16));
                testPlusMinus(
                    IntType(std::string("-1") + zero2k[i], 16), long(1),
                    IntType(std::string("-") + f2k[i], 16));
                testPlusMinus(
                    IntType(std::string("-") + f2k[i], 16),
                    IntType(std::string("1") + zero2k[i], 16),
                    IntType(1));
                testPlusMinus(
                    IntType(f2k[i], 16),
                    IntType(std::string("-1") + zero2k[i], 16),
                    IntType(-1));

                testPlusMinus(
                    IntType(std::string("1") + zero2kdec[i] + "1", 16),
                    long(-1),
                    IntType(std::string("1") + zero2k[i], 16));
                testPlusMinus(
                    IntType(std::string("-1") + zero2kdec[i] + "1", 16),
                    long(1),
                    IntType(std::string("-1") + zero2k[i], 16));
                testPlusMinus(
                    IntType(std::string("1") + zero2k[i], 16),
                    long(1),
                    IntType(std::string("1") + zero2kdec[i] + "1", 16));
                testPlusMinus(
                    IntType(std::string("-1") + zero2k[i], 16),
                    long(-1),
                    IntType(std::string("-1") + zero2kdec[i] + "1", 16));
                testPlusMinus(
                    IntType(std::string("1") + zero2kdec[i] + "1", 16),
                    IntType(std::string("-1") + zero2k[i], 16),
                    IntType(1));
                testPlusMinus(
                    IntType(std::string("-1") + zero2kdec[i] + "1", 16),
                    IntType(std::string("1") + zero2k[i], 16),
                    IntType(-1));
            }

            // Tests for infinity are hard-coded to LargeInteger.
            const LargeInteger& infinity(LargeInteger::infinity);

            shouldBeEqual(infinity + infinity, infinity);
            shouldBeEqual(infinity - infinity, infinity);
            for (a = 0; a < dataL.nCases; a++) {
                shouldBeEqual(dataL.cases[a] + infinity, infinity);
                shouldBeEqual(dataL.cases[a] - infinity, infinity);
                shouldBeEqual(infinity + dataL.cases[a], infinity);
                shouldBeEqual(infinity - dataL.cases[a], infinity);
            }
            for (a = 0; a < dataL.nLongCases; a++) {
                shouldBeEqual(dataL.longCases[a] + infinity, infinity);
                shouldBeEqual(infinity + dataL.longCases[a], infinity);
                shouldBeEqual(infinity - dataL.longCases[a], infinity);
            }
        }

        template <typename IntType>
        void testMultiply(const IntType& a, long b, const IntType& ans) {
            shouldBeEqual(a * long(b), ans);
            shouldBeEqual(long(b) * a, ans);
            shouldBeEqual(a * IntType(b), ans);
            shouldBeEqual(IntType(b) * a, ans);
        }

        template <typename IntType>
        void testMultiply(const IntType& a, long b, long ans) {
            shouldBeEqual(a * long(b), ans);
            shouldBeEqual(long(b) * a, ans);
            shouldBeEqual(a * IntType(b), ans);
            shouldBeEqual(IntType(b) * a, ans);
        }

        template <typename IntType>
        void multiply() {
            unsigned a, b;

            const Data<IntType>& d(data<IntType>());

            for (a = 0; a < d.nCases; ++a) {
                for (b = 0; b < d.nCases; ++b) {
                    IntType x(d.cases[a]);
                    IntType y(d.cases[b]);

                    // Test the commutative law.
                    shouldBeEqual(x * y, y * x);

                    // Test the distributive law.
                    shouldBeEqual(x * (y + 1), (x * y) + x);
                    shouldBeEqual(x * (y - 1), (x * y) - x);
                    shouldBeEqual(x * (y + IntType(HUGE_INTEGER)),
                        (x * y) + (x * IntType(HUGE_INTEGER)));
                    shouldBeEqual(x * (y - IntType(HUGE_INTEGER)),
                        (x * y) - (x * IntType(HUGE_INTEGER)));

                    // Other simple arithmetic tests.
                    shouldBeEqual(x * (-y), -(x * y));
                    shouldBeEqual((-x) * (-y), x * y);
                    shouldBeEqual(x * (-y) + x * y, 0L);

                    // Test that *= behaves as it should.
                    {
                        IntType p(x);
                        shouldBeEqual(p *= y, x * y);
                    }

                    // Test signs and ordering.
                    if (x.sign() > 0 && y.sign() > 0) {
                        shouldBeGreater(x * y, 0);
                        shouldBeGreater(x * y, x - 1);
                        shouldBeGreater(x * y, y - 1);
                    } else if (x.sign() > 0 && y.sign() < 0) {
                        shouldBeLess(x * y, 0);
                        shouldBeLess(x * y, (-x) + 1);
                        shouldBeLess(x * y, y + 1);
                    } else if (x.sign() < 0 && y.sign() > 0) {
                        shouldBeLess(x * y, 0);
                        shouldBeLess(x * y, x + 1);
                        shouldBeLess(x * y, (-y) + 1);
                    } else if (x.sign() < 0 && y.sign() < 0) {
                        shouldBeGreater(x * y, 0);
                        shouldBeGreater(x * y, (-x) - 1);
                        shouldBeGreater(x * y, (-y) - 1);
                    }
                }

                for (b = 0; b < d.nLongCases; ++b) {
                    IntType x(d.cases[a]);
                    long y = d.longCases[b];

                    // Test the commutative law.
                    shouldBeEqual(x * y, y * x);

                    // Test the distributive law.
                    shouldBeEqual(x * (IntType(y) + 1), (x * y) + x);
                    shouldBeEqual(x * (IntType(y) - 1), (x * y) - x);
                    shouldBeEqual(x * (y + IntType(HUGE_INTEGER)),
                        (x * y) + (x * IntType(HUGE_INTEGER)));
                    shouldBeEqual(x * (y + IntType("-" HUGE_INTEGER)),
                        (x * y) - (x * IntType(HUGE_INTEGER)));

                    // Other simple arithmetic tests.
                    if (y == LONG_MIN) {
                        shouldBeEqual(x * d.longMaxInc, -(x * y));
                        shouldBeEqual((-x) * d.longMaxInc, x * y);
                        shouldBeEqual(x * d.longMaxInc + x * y, 0L);
                    } else {
                        shouldBeEqual(x * (-y), -(x * y));
                        shouldBeEqual((-x) * (-y), x * y);
                        shouldBeEqual(x * (-y) + x * y, 0L);
                    }

                    // Test that *= behaves as it should.
                    {
                        IntType p(x);
                        shouldBeEqual(p *= y, x * y);
                    }

                    // Test signs and ordering.
                    if (x.sign() > 0 && y > 0) {
                        shouldBeGreater(x * y, 0);
                        shouldBeGreater(x * y, x - 1);
                        shouldBeGreater(x * y, y - 1);
                    } else if (x.sign() > 0 && y < 0) {
                        shouldBeLess(x * y, 0);
                        shouldBeLess(x * y, (-x) + 1);
                        shouldBeLess(x * y, y + 1);
                    } else if (x.sign() < 0 && y > 0) {
                        shouldBeLess(x * y, 0);
                        shouldBeLess(x * y, x + 1);
                        shouldBeLess(x * y, (-y) + 1);
                    } else if (x.sign() < 0 && y < 0) {
                        shouldBeGreater(x * y, 0);
                        shouldBeGreater(x * y, (-x) - 1);
                        shouldBeGreater(x * y, (-(y + 1)));
                    }
                }

                IntType z(d.cases[a]);
                testMultiply(z, 2, z + z);
                testMultiply(z, 1, z);
                testMultiply(z, 0, 0L);
                testMultiply(z, -1, -z);
                testMultiply(z, -2, -z - z);
            }

            // Ad-hoc tests for native * native:
            testMultiply(IntType(3), long(7), 21);
            testMultiply(IntType(3), long(-7), -21);
            testMultiply(IntType(-3), long(7), -21);
            testMultiply(IntType(-3), long(-7), 21);

            // Ad-hoc tests for large * native and native * large:
            testMultiply(IntType(ENORMOUS_INTEGER), long(100),
                IntType(ENORMOUS_INTEGER "00"));
            testMultiply(IntType("-" ENORMOUS_INTEGER), long(100),
                IntType("-" ENORMOUS_INTEGER "00"));
            testMultiply(IntType(ENORMOUS_INTEGER), long(-100),
                IntType("-" ENORMOUS_INTEGER "00"));
            testMultiply(IntType("-" ENORMOUS_INTEGER), long(-100),
                IntType(ENORMOUS_INTEGER "00"));

            // Ad-hoc tests for large * large:
            shouldBeEqual(IntType("3" ZEROES) * IntType("7" ZEROES),
                IntType("21" ZEROES ZEROES));
            shouldBeEqual(IntType("3" ZEROES) * IntType("-7" ZEROES),
                IntType("-21" ZEROES ZEROES));
            shouldBeEqual(IntType("-3" ZEROES) * IntType("7" ZEROES),
                IntType("-21" ZEROES ZEROES));
            shouldBeEqual(IntType("-3" ZEROES) * IntType("-7" ZEROES),
                IntType("21" ZEROES ZEROES));

            // Test around overflow points:
            size_t i;
            for (i = 0; i < nZero; ++i) {
                testMultiply(
                    IntType(std::string("-1") + zero2k[i], 16), long(-1),
                    IntType(std::string("1") + zero2k[i], 16));
                testMultiply(
                    IntType(std::string("1") + zero2k[i], 16), long(-1),
                    IntType(std::string("-1") + zero2k[i], 16));
                testMultiply(
                    IntType(std::string("-8") + zero2kdec[i], 16), long(-2),
                    IntType(std::string("1") + zero2k[i], 16));
                testMultiply(
                    IntType(std::string("-4") + zero2kdec[i], 16), long(4),
                    IntType(std::string("-1") + zero2k[i], 16));
                testMultiply(
                    IntType(std::string("2") + zero2kdec[i], 16), long(-8),
                    IntType(std::string("-1") + zero2k[i], 16));
                testMultiply(
                    IntType(std::string("1") + zero2kdec[i], 16), long(16),
                    IntType(std::string("1") + zero2k[i], 16));

                if (i == nZero - 1)
                    continue;

                shouldBeEqual(
                    IntType(std::string("1") + zero2k[i], 16) *
                    IntType(std::string("1") + zero2k[i], 16),
                    IntType(std::string("1") + zero2k[i + 1], 16));
                shouldBeEqual(
                    IntType(std::string("-1") + zero2k[i], 16) *
                    IntType(std::string("1") + zero2k[i], 16),
                    IntType(std::string("-1") + zero2k[i + 1], 16));
                shouldBeEqual(
                    IntType(std::string("1") + zero2k[i], 16) *
                    IntType(std::string("-1") + zero2k[i], 16),
                    IntType(std::string("-1") + zero2k[i + 1], 16));
                shouldBeEqual(
                    IntType(std::string("-1") + zero2k[i], 16) *
                    IntType(std::string("-1") + zero2k[i], 16),
                    IntType(std::string("1") + zero2k[i + 1], 16));
            }

            testMultiply(d.longMin, long(-1), d.longMaxInc);
            testMultiply(d.longMaxInc, long(-1), d.longMin);

            testMultiply(IntType((LONG_MAX-1)/3), long(3), LONG_MAX-1);
            testMultiply(IntType((LONG_MAX-1)/3), long(-3), LONG_MIN+2);
            testMultiply(IntType(-(LONG_MAX-1)/3), long(3), LONG_MIN+2);
            testMultiply(IntType(-(LONG_MAX-1)/3), long(-3), LONG_MAX-1);
            testMultiply(IntType(3), long((LONG_MAX-1)/3), LONG_MAX-1);
            testMultiply(IntType(-3), long((LONG_MAX-1)/3), LONG_MIN+2);
            testMultiply(IntType(3), long(-(LONG_MAX-1)/3), LONG_MIN+2);
            testMultiply(IntType(-3), long(-(LONG_MAX-1)/3), LONG_MAX-1);

            testMultiply(IntType((LONG_MAX-1)/3 + 1), long(3),
                IntType(str(static_cast<unsigned long>(LONG_MAX) + 2)));
            testMultiply(IntType((LONG_MAX-1)/3 + 1), long(-3), d.longMinDec);
            testMultiply(IntType(-(LONG_MAX-1)/3 - 1), long(3), d.longMinDec);
            testMultiply(IntType(-(LONG_MAX-1)/3 - 1), long(-3),
                IntType(str(static_cast<unsigned long>(LONG_MAX) + 2)));
            testMultiply(IntType(3), long((LONG_MAX-1)/3 + 1),
                IntType(str(static_cast<unsigned long>(LONG_MAX) + 2)));
            testMultiply(IntType(-3), long((LONG_MAX-1)/3 + 1), d.longMinDec);
            testMultiply(IntType(3), long(-(LONG_MAX-1)/3 - 1), d.longMinDec);
            testMultiply(IntType(-3), long(-(LONG_MAX-1)/3 - 1),
                IntType(str(static_cast<unsigned long>(LONG_MAX) + 2)));

            // Tests for infinity are hard-coded to LargeInteger.
            const LargeInteger& infinity(LargeInteger::infinity);

            shouldBeEqual(infinity * infinity, infinity);
            for (a = 0; a < dataL.nCases; a++) {
                shouldBeEqual(dataL.cases[a] * infinity, infinity);
                shouldBeEqual(infinity * dataL.cases[a], infinity);
            }
            for (a = 0; a < dataL.nLongCases; a++) {
                shouldBeEqual(dataL.longCases[a] * infinity, infinity);
                shouldBeEqual(infinity * dataL.longCases[a], infinity);
            }
        }

        template <typename IntType>
        void divide() {
            unsigned a, b;

            const Data<IntType>& d(data<IntType>());

            // Rounding should always be towards zero.

            for (a = 0; a < d.nCases; ++a) {
                for (b = 0; b < d.nCases; ++b) {
                    IntType x(d.cases[a]);
                    IntType y(d.cases[b]);

                    // We test divide by zero later.
                    if (y == 0)
                        continue;

                    IntType ans = x / y;

                    if (ans == 0) {
                        // |x| < |y|.
                        shouldBeLess(x.abs(), y.abs());
                    } else {
                        // Here ans != 0 and y != 0.

                        // Check that we got the correct result, by
                        // multiplying again and ensuring that the
                        // difference is in the correct range.
                        IntType recover = ans * y;
                        if (recover > 0) {
                            // recover <= x < recover + abs(y)
                            shouldBeLess(recover, x + 1);
                            shouldBeLess(x, recover + y.abs());
                        } else {
                            // recover - abs(y) < x <= recover
                            shouldBeLess(recover, x + y.abs());
                            shouldBeLess(x, recover + 1);
                        }
                    }

                    shouldBeEqual((x * y) / y, x);
                    shouldBeEqual(x / (-y), -ans);
                    shouldBeEqual((-x) / y, -ans);
                    shouldBeEqual((-x) / (-y), ans);

                    IntType p(x);
                    shouldBeEqual(p /= y, x / y);

                    // Now test divExact().
                    IntType product = x * y;
                    shouldBeEqual(product.divExact(y), x);

                    IntType q(product);
                    shouldBeEqual(q.divByExact(y), x);
                }

                for (b = 0; b < d.nLongCases; ++b) {
                    IntType x(d.cases[a]);
                    long y = d.longCases[b];

                    // We test divide by zero later.
                    if (y == 0)
                        continue;

                    IntType ans = x / y;

                    // Always prefer -abs(y) instead of abs(y), since
                    // abs(LONG_MIN) will overflow.
                    if (ans == 0) {
                        // |x| < |y|.
                        shouldBeGreater(-x.abs(), y >= 0 ? -y : y);
                        continue;
                    } else {
                        // Here ans != 0 and y != 0.

                        // Check that we got the correct result, by
                        // multiplying again and ensuring that the
                        // difference is in the correct range.
                        IntType recover = ans * y;
                        if (recover > 0) {
                            // recover <= x < recover + abs(y)
                            shouldBeLess(recover, x + 1);
                            shouldBeLess(x, recover - (y >= 0 ? -y : y));
                        } else {
                            // recover - abs(y) < x <= recover
                            shouldBeLess(recover, x - (y >= 0 ? -y : y));
                            shouldBeLess(x, recover + 1);
                        }
                    }

                    shouldBeEqual((x * y) / y, x);
                    shouldBeEqual((-x) / y, -ans);
                    if (y == LONG_MIN) {
                        // -LONG_MIN will overflow.
                        shouldBeEqual(x / (-IntType(y)), -ans);
                        shouldBeEqual((-x) / (-IntType(y)), ans);
                    } else {
                        shouldBeEqual(x / (-y), -ans);
                        shouldBeEqual((-x) / (-y), ans);
                    }

                    IntType p(x);
                    shouldBeEqual(p /= y, x / y);

                    // Now test divExact().
                    IntType product = x * y;
                    shouldBeEqual(product.divExact(y), x);
                    shouldBeEqual(product, x * y);

                    IntType q(product);
                    shouldBeEqual(q.divByExact(y), x);
                    shouldBeEqual(q, x);

                    shouldBeEqual(product.divExact(y), product / y);
                }

                IntType z(d.cases[a]);
                if (z != 0) {
                    shouldBeEqual(IntType::zero / z, 0L);
                    shouldBeEqual(z / z, 1);
                    shouldBeEqual(z / -z, -1);
                    shouldBeEqual((z + z) / z, 2);
                    shouldBeEqual((z + z) / -z, -2);
                    shouldBeEqual(IntType::zero.divExact(z), 0L);
                    shouldBeEqual(z.divExact(z), 1);
                    shouldBeEqual(z.divExact(-z), -1);
                    shouldBeEqual((z + z).divExact(z), 2);
                    shouldBeEqual((z + z).divExact(-z), -2);
                }
            }

            // Test around overflow points:
            shouldBeEqual(d.longMax / 1, LONG_MAX);
            shouldBeEqual(d.longMax / -1, -LONG_MAX);
            shouldBeEqual(d.longMin / 1, LONG_MIN);
            shouldBeEqual(d.longMin / -1, d.longMaxInc);
            shouldBeEqual(d.longMax / d.longMin, 0L);
            shouldBeEqual((-d.longMax) / d.longMin, 0L);
            shouldBeEqual(d.longMin / d.longMax, -1);
            shouldBeEqual(d.longMin / (-d.longMax), 1);
            shouldBeEqual(d.zero / d.longMax, 0);
            shouldBeEqual(d.zero / d.longMin, 0);
            shouldBeEqual(d.longMax / LONG_MIN, 0);
            shouldBeEqual((-d.longMax) / LONG_MIN, 0);
            shouldBeEqual(d.longMin / LONG_MAX, -1);
            shouldBeEqual(d.longMin / (-LONG_MAX), 1);
            shouldBeEqual(d.zero / LONG_MAX, 0);
            shouldBeEqual(d.zero / LONG_MIN, 0);

            shouldBeEqual(d.longMaxInc / 1, d.longMaxInc);
            shouldBeEqual(d.longMaxInc / -1, LONG_MIN);
            shouldBeEqual(d.longMinDec / 1, d.longMinDec);
            shouldBeEqual(d.longMinDec / -1, d.longMaxInc + 1);
            shouldBeEqual(d.longMaxInc / d.longMax, 1);
            shouldBeEqual(d.longMaxInc / d.longMin, -1);
            shouldBeEqual(d.longMaxInc / -d.longMax, -1);
            shouldBeEqual(d.longMaxInc / -d.longMin, 1);
            shouldBeEqual(d.longMinDec / d.longMax, -1);
            shouldBeEqual(d.longMinDec / d.longMin, 1);
            shouldBeEqual(d.longMinDec / -d.longMax, 1);
            shouldBeEqual(d.longMinDec / -d.longMin, -1);
            shouldBeEqual(d.longMaxInc / LONG_MAX, 1);
            shouldBeEqual(d.longMaxInc / LONG_MIN, -1);
            shouldBeEqual(d.longMaxInc / -LONG_MAX, -1);
            shouldBeEqual(d.longMinDec / LONG_MAX, -1);
            shouldBeEqual(d.longMinDec / LONG_MIN, 1);
            shouldBeEqual(d.longMinDec / -LONG_MAX, 1);

            shouldBeEqual(d.longMax.divExact(1), LONG_MAX);
            shouldBeEqual(d.longMax.divExact(-1), -LONG_MAX);
            shouldBeEqual(d.longMin.divExact(1), LONG_MIN);
            shouldBeEqual(d.longMin.divExact(-1), d.longMaxInc);
            shouldBeEqual(d.zero.divExact(d.longMax), 0);
            shouldBeEqual(d.zero.divExact(d.longMin), 0);
            shouldBeEqual(d.zero.divExact(LONG_MAX), 0);
            shouldBeEqual(d.zero.divExact(LONG_MIN), 0);

            shouldBeEqual(d.longMaxInc.divExact(1), d.longMaxInc);
            shouldBeEqual(d.longMaxInc.divExact(-1), LONG_MIN);
            shouldBeEqual(d.longMinDec.divExact(1), d.longMinDec);
            shouldBeEqual(d.longMinDec.divExact(-1), d.longMaxInc + 1);
            shouldBeEqual(d.longMaxInc.divExact(d.longMin), -1);
            shouldBeEqual(d.longMaxInc.divExact(-d.longMin), 1);
            shouldBeEqual(d.longMaxInc.divExact(LONG_MIN), -1);

            // Tests for infinity are hard-coded to LargeInteger.
            const LargeInteger& infinity(LargeInteger::infinity);

            shouldBeEqual(infinity / infinity, infinity);
            for (a = 0; a < dataL.nCases; a++) {
                shouldBeEqual(infinity / dataL.cases[a], infinity);
                shouldBeEqual(dataL.cases[a] / infinity, 0L);
                shouldBeEqual(dataL.cases[a] / LargeInteger::zero, infinity);
                shouldBeEqual(dataL.cases[a] / 0L, infinity);
            }
            for (a = 0; a < dataL.nLongCases; a++) {
                shouldBeEqual(infinity / dataL.longCases[a], infinity);
            }
        }

        template <typename IntType>
        void mod() {
            unsigned a, b;

            const Data<IntType>& d(data<IntType>());

            // Infinity is not supported.
            // x % 0 is not supported.
            // If result is non-zero then sign(x & y) = sign(x).

            for (a = 0; a < d.nCases; ++a) {
                for (b = 0; b < d.nCases; ++b) {
                    IntType x(d.cases[a]);
                    IntType y(d.cases[b]);

                    if (y == 0)
                        continue;

                    IntType ans = x % y;

                    // Ensure that ans is within range.
                    if (ans < 0) {
                        shouldBeLess(x, 0);
                        shouldBeGreater(ans, -y.abs());
                    } else if (ans > 0) {
                        shouldBeGreater(x, 0);
                        shouldBeLess(ans, y.abs());
                    }

                    // Ensure that y | (x - ans).
                    IntType q = (x - ans) / y;
                    shouldBeEqual(q * y + ans, x);

                    IntType p(x);
                    shouldBeEqual(p %= y, ans);

                    // Verify divExact() if we can.
                    if (ans == 0)
                        shouldBeEqual(x.divExact(y) * y, x);
                }

                for (b = 0; b < d.nLongCases; ++b) {
                    IntType x(d.cases[a]);
                    long y = d.longCases[b];

                    if (y == 0)
                        continue;

                    IntType ans = x % y;

                    // Ensure that ans is within range.
                    if (ans < 0) {
                        shouldBeLess(x, 0);
                        shouldBeGreater(ans, y < 0 ? y : -y);
                    } else if (ans > 0) {
                        shouldBeGreater(x, 0);
                        shouldBeGreater(-ans, y < 0 ? y : -y); // abs(y) could
                                                               // overflow.
                    }

                    // Ensure that y | (x - ans).
                    IntType q = (x - ans) / y;
                    shouldBeEqual(q * y + ans, x);

                    IntType p(x);
                    shouldBeEqual(p %= y, ans);

                    // Verify divExact() if we can.
                    if (ans == 0)
                        shouldBeEqual(x.divExact(y) * y, x);
                }

                IntType z(d.cases[a]);
                shouldBeEqual(z % 1, 0);
                if (z != 0) {
                    shouldBeEqual(IntType::zero % z, 0L);
                    shouldBeEqual(IntType::zero % z, 0L);
                    shouldBeEqual(z % z, 0L);
                    shouldBeEqual((-z) % z, 0L);
                    shouldBeEqual((z + z) % z, 0L);
                    shouldBeEqual((-(z + z)) % z, 0L);
                }
            }

            // Test around overflow points:
            shouldBeEqual(d.longMax % 1, 0);
            shouldBeEqual(d.longMax % -1, 0);
            shouldBeEqual(d.longMin % 1, 0);
            shouldBeEqual(d.longMin % -1, 0);
            shouldBeEqual(d.longMax % d.longMin, d.longMax);
            shouldBeEqual((-d.longMax) % d.longMin, -d.longMax);
            shouldBeEqual(d.longMin % d.longMax, -1);
            shouldBeEqual(d.longMin % (-d.longMax), -1);
            shouldBeEqual(d.zero % d.longMax, 0);
            shouldBeEqual(d.zero % d.longMin, 0);
            shouldBeEqual(d.longMax % LONG_MIN, LONG_MAX);
            shouldBeEqual((-d.longMax) % LONG_MIN, -LONG_MAX);
            shouldBeEqual(d.longMin % LONG_MAX, -1);
            shouldBeEqual(d.longMin % (-LONG_MAX), -1);
            shouldBeEqual(d.zero % LONG_MAX, 0);
            shouldBeEqual(d.zero % LONG_MIN, 0);

            shouldBeEqual(d.longMaxInc % 1, 0);
            shouldBeEqual(d.longMaxInc % -1, 0);
            shouldBeEqual(d.longMinDec % 1, 0);
            shouldBeEqual(d.longMinDec % -1, 0);
            shouldBeEqual(d.longMaxInc % d.longMax, 1);
            shouldBeEqual(d.longMaxInc % d.longMin, 0);
            shouldBeEqual(d.longMaxInc % -d.longMax, 1);
            shouldBeEqual(d.longMaxInc % -d.longMin, 0);
            shouldBeEqual(d.longMinDec % d.longMax, -2);
            shouldBeEqual(d.longMinDec % d.longMin, -1);
            shouldBeEqual(d.longMinDec % -d.longMax, -2);
            shouldBeEqual(d.longMinDec % -d.longMin, -1);
            shouldBeEqual(d.longMaxInc % LONG_MAX, 1);
            shouldBeEqual(d.longMaxInc % LONG_MIN, 0);
            shouldBeEqual(d.longMaxInc % -LONG_MAX, 1);
            shouldBeEqual(d.longMinDec % LONG_MAX, -2);
            shouldBeEqual(d.longMinDec % LONG_MIN, -1);
            shouldBeEqual(d.longMinDec % -LONG_MAX, -2);
        }

        template <typename IntType>
        void testNegate(const IntType& x) {
            IntType y(x);

            shouldBeEqual(x + (-y), 0L);
            shouldBeEqual((-y) + x, 0L);
            shouldBeEqual(-(-y), x);
            shouldBeEqual(-y, x * (-1));

            std::string orig = x.stringValue();
            std::string neg = (-y).stringValue();
            if (x.sign() > 0) {
                orig.insert(orig.begin(), '-');
                if (orig != neg) {
                    std::ostringstream msg;
                    msg << "Negation gives wrong string for " << x << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            } else if (x.sign() < 0) {
                neg.insert(neg.begin(), '-');
                if (orig != neg) {
                    std::ostringstream msg;
                    msg << "Negation gives wrong string for " << x << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (orig != neg) {
                    std::ostringstream msg;
                    msg << "Negation gives wrong string for " << x << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }

            IntType z(x);
            z.negate();
            shouldBeEqual(z, -x);
        }

        template <typename IntType>
        void negate() {
            const Data<IntType>& d(data<IntType>());

            testNegate(d.zero);
            testNegate(d.one);
            testNegate(d.two);
            testNegate(d.negOne);
            testNegate(d.negTwo);
            testNegate(d.longMax);
            testNegate(d.longMin);
            testNegate(d.longMaxInc);
            testNegate(d.longMinDec);
            testNegate(d.ulongMax);
            testNegate(d.hugePos);
            testNegate(d.hugeNeg);

            for (int a = 0; a < d.nCases; a++)
                testNegate(d.cases[a]);

            // Tests for infinity are hard-coded to LargeInteger.
            LargeInteger i(LargeInteger::infinity);
            shouldBeEqual(-i, LargeInteger::infinity);
            i.negate();
            shouldBeEqual(i, LargeInteger::infinity);
        }

        template <typename IntType>
        void testAbs(const IntType& x) {
            IntType y(x);
            IntType z = y.abs();

            std::string orig = x.stringValue();
            std::string abs = z.stringValue();

            shouldBeEqual(z, x * x.sign());
            shouldBeEqual(z, x >= 0 ? x : -x);

            if (orig.length() > 0 && orig[0] == '-') {
                abs.insert(abs.begin(), '-');
                if (abs != orig) {
                    std::ostringstream msg;
                    msg << "Absolute value gives wrong string for " << x << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (abs != orig) {
                    std::ostringstream msg;
                    msg << "Absolute value gives wrong string for " << x << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        template <typename IntType>
        void abs() {
            const Data<IntType>& d(data<IntType>());

            testAbs(d.zero);
            testAbs(d.one);
            testAbs(d.two);
            testAbs(d.negOne);
            testAbs(d.negTwo);
            testAbs(d.longMax);
            testAbs(d.longMin);
            testAbs(d.longMaxInc);
            testAbs(d.longMinDec);
            testAbs(d.ulongMax);
            testAbs(d.hugePos);
            testAbs(d.hugeNeg);

            for (int a = 0; a < d.nCases; a++)
                testAbs(d.cases[a]);

            // Tests for infinity are hard-coded to LargeInteger.
            LargeInteger i(LargeInteger::infinity);
            shouldBeEqual(i.abs(), LargeInteger::infinity);
        }

        template <typename IntType>
        void testDivisionAlg(const IntType& n, const IntType& divisor) {
            IntType q, r;
            q = IntType(n).divisionAlg(divisor, r);

            shouldBeEqual(q * divisor + r, n);
            if (divisor == 0) {
                shouldBeEqual(q, 0L);
                shouldBeEqual(r, n);
            } else {
                shouldBeGreater(r, -1);
                shouldBeLess(r, divisor >= 0 ? divisor : -divisor);
            }
        }

        template <typename IntType>
        void divisionAlg() {
            const Data<IntType>& d(data<IntType>());

            // The list of cases includes cases at the overflow points.
            // Just run through all n,d pairs.
            int a, b;
            for (a = 0; a < d.nCases; ++a)
                for (b = a + 1; b < d.nCases; ++b)
                    testDivisionAlg(d.cases[a], d.cases[b]);
        }

        template <typename IntType>
        void testGcdLcm(const IntType& x, const IntType& y) {
            IntType g = x.gcd(y);
            IntType g2 = y.gcd(x);
            shouldBeEqual(g, g2);

            IntType l = x.lcm(y);
            IntType l2 = y.lcm(x);
            shouldBeEqual(l, l2);

            IntType u, v;
            IntType g3 = x.gcdWithCoeffs(y, u, v);
            shouldBeEqual(g, g3);

            if (x == 0 && y == 0) {
                shouldBeEqual(g, 0L);
                shouldBeEqual(u, 0L);
                shouldBeEqual(v, 0L);
            } else if (x == 0 && y > 0) {
                shouldBeEqual(g, y);
                shouldBeEqual(u, 0L);
                shouldBeEqual(v, 1);
            } else if (x == 0 && y < 0) {
                shouldBeEqual(g, -y);
                shouldBeEqual(u, 0L);
                shouldBeEqual(v, -1);
            } else if (y == 0 && x > 0) {
                shouldBeEqual(g, x);
                shouldBeEqual(u, 1);
                shouldBeEqual(v, 0L);
            } else if (y == 0 && x < 0) {
                shouldBeEqual(g, -x);
                shouldBeEqual(u, -1);
                shouldBeEqual(v, 0L);
            } else {
                // The following tests are enough to ensure that we have the
                // right gcd.
                shouldBeGreater(g, 0);
                shouldBeEqual(x % g, 0L);
                shouldBeEqual(y % g, 0L);
                shouldBeEqual((x / g) * g, x);
                shouldBeEqual((y / g) * g, y);
                shouldBeEqual(u * x + v * y, g);

                // These next tests ensure that u and v are correct.
                shouldBeLess(-x.abs(), g * v * y.sign());
                shouldBeLess(v * y.sign(), 1);
            }

            // Make sure the LCM is correct.
            // Note that we make no guarantees about the sign of the LCM.
            shouldBeEqual((g * l).abs(), (x * y).abs());

            IntType p(x);
            p.gcdWith(y);
            shouldBeEqual(p, x.gcd(y));

            IntType q(x);
            q.lcmWith(y);
            shouldBeEqual(q, x.lcm(y));
        }

        template <typename IntType>
        void gcdLcm() {
            const Data<IntType>& d(data<IntType>());

            // The list of cases includes cases at the overflow points.
            // Just run through all pairs.
            int a, b;
            for (a = 0; a < d.nCases; ++a)
                for (b = a + 1; b < d.nCases; ++b)
                    testGcdLcm(d.cases[a], d.cases[b]);
        }

        template <typename IntType>
        void raiseToPower() {
            const Data<IntType>& d(data<IntType>());

            int a;
            unsigned exp;
            for (a = 0; a < d.nCases; ++a) {
                IntType ans(1);
                IntType base(d.cases[a]);
                for (exp = 0; exp < 32; ++exp) {
                    IntType pow(base);
                    pow.raiseToPower(exp);
                    shouldBeEqual(ans, pow);
                    ans *= base;
                }
            }

            for (exp = 0; exp < 5; ++exp) {
                LargeInteger pow(LargeInteger::infinity);
                pow.raiseToPower(exp);
                if (exp == 0)
                    shouldBeEqual(pow, 1);
                else
                    shouldBeEqual(pow, LargeInteger::infinity);
            }
        }

        template <typename IntType>
        void tryReduce() {
            const Data<IntType>& d(data<IntType>());

            for (int a = 0; a < d.nCases; ++a) {
                IntType x(d.cases[a]);
                x.tryReduce();
                shouldBeEqual(x, d.cases[a]);

                IntType y(d.cases[a]);
                y += IntType(ENORMOUS_INTEGER);
                y += IntType(HUGE_INTEGER);
                y -= IntType(ENORMOUS_INTEGER);
                y -= IntType(HUGE_INTEGER);
                y.tryReduce();
                shouldBeEqual(y, d.cases[a]);

                IntType z(d.cases[a]);
                z.makeLarge();
                if (z.isNative())
                    CPPUNIT_FAIL("Integer is still native after makeLarge().");
                z.tryReduce();

                if (d.cases[a] <= LONG_MAX && d.cases[a] >= LONG_MIN) {
                    if (! (x.isNative() && y.isNative() && z.isNative()))
                        CPPUNIT_FAIL("Integers are not native after "
                            "tryReduce() even though they are within range.");
                } else {
                    if (x.isNative() || y.isNative() || z.isNative())
                        CPPUNIT_FAIL("Integers become native after "
                            "tryReduce() even though they are out of range.");
                }
            }
        }

        template <typename IntType>
        void makeLarge() {
            const Data<IntType>& d(data<IntType>());

            for (int a = 0; a < d.nCases; ++a) {
                IntType x(d.cases[a]);
                x.makeLarge();
                shouldBeEqual(x, d.cases[a]);
                if (x.isNative())
                    CPPUNIT_FAIL("Integer is still native after makeLarge().");
                shouldBeEqual(x, d.cases[a]);
            }
        }

        template <typename IntType>
        void nativeVsLarge() {
            unsigned a, b;
            int lx, ly, op;

            const Data<IntType>& d(data<IntType>());

            // For native integers, try variants in which they are
            // stored as large GMP integers.

            // Unary operations:
            for (a = 0; a < d.nCases; ++a) {
                IntType xOrig(d.cases[a]);
                xOrig.tryReduce();
                if (! xOrig.isNative())
                    continue;

                for (op = 0; op < 2; ++op) {
                    IntType x(xOrig);

                    IntType x2(xOrig);
                    x2.makeLarge();

                    switch (op) {
                        case 0:
                            shouldBeEqual(-x, -x2); break;
                        case 1:
                            shouldBeEqual(x.abs(), x2.abs()); break;
                    }
                }
            }

            // Binary operations:
            for (a = 0; a < d.nCases; ++a) {
                IntType xOrig(d.cases[a]);
                xOrig.tryReduce();
                for (b = 0; b < d.nCases; ++b) {
                    IntType yOrig(d.cases[b]);
                    yOrig.tryReduce();
                    for (lx = 0; lx < (xOrig.isNative() ? 2 : 1); ++lx) {
                        for (ly = 0; ly < (yOrig.isNative() ? 2 : 1); ++ly) {
                            if (lx == 0 && ly == 0)
                                continue;

                            for (op = 0; op < (yOrig == 0 ? 12 : 15); ++op) {
                                IntType x(xOrig);
                                IntType y(yOrig);

                                IntType x2(xOrig);
                                IntType y2(yOrig);
                                if (lx == 1)
                                    x2.makeLarge();
                                if (ly == 1)
                                    y2.makeLarge();

                                switch (op) {
                                    case 0:
                                        shouldBeEqual(x + y, x2 + y2); break;
                                    case 1:
                                        shouldBeEqual(x - y, x2 - y2); break;
                                    case 2:
                                        shouldBeEqual(x * y, x2 * y2); break;
                                    case 3:
                                        shouldBeEqual(x.gcd(y),
                                                      x2.gcd(y2)); break;
                                    case 4:
                                        {
                                            IntType g, u, v, g2, u2, v2;
                                            g = x.gcdWithCoeffs(y, u, v);
                                            g2 = x2.gcdWithCoeffs(y2, u2, v2);
                                            shouldBeEqual(g, g2);
                                            shouldBeEqual(u, u2);
                                            shouldBeEqual(v, v2);
                                        }
                                        break;
                                    case 5:
                                        shouldBeEqual(x.lcm(y),
                                                      x2.lcm(y2)); break;
                                    case 6:
                                        if ((x < y) != (x2 < y2))
                                            CPPUNIT_FAIL("Inconsistent <.");
                                        break;
                                    case 7:
                                        if ((x <= y) != (x2 <= y2))
                                            CPPUNIT_FAIL("Inconsistent <=.");
                                        break;
                                    case 8:
                                        if ((x > y) != (x2 > y2))
                                            CPPUNIT_FAIL("Inconsistent >.");
                                        break;
                                    case 9:
                                        if ((x >= y) != (x2 >= y2))
                                            CPPUNIT_FAIL("Inconsistent >=.");
                                        break;
                                    case 10:
                                        if ((x == y) != (x2 == y2))
                                            CPPUNIT_FAIL("Inconsistent ==.");
                                        break;
                                    case 11:
                                        if ((x != y) != (x2 != y2))
                                            CPPUNIT_FAIL("Inconsistent !=.");
                                        break;

                                    // Operations that require RHS != 0:
                                    case 12:
                                        shouldBeEqual(x / y, x2 / y2); break;
                                    case 13:
                                        shouldBeEqual(x % y, x2 % y2); break;
                                    case 14:
                                        {
                                            IntType q, r, q2, r2;
                                            q = x.divisionAlg(y, r);
                                            q2 = x2.divisionAlg(y2, r2);
                                            shouldBeEqual(q, q2);
                                            shouldBeEqual(r, r2);
                                        }
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }
};

// Out-of-line definitions of static arrays.

template <typename IntType>
const IntType IntegerTest::Data<IntType>::cases[] = {
    // Too low for a native long:
    "-" ENORMOUS_INTEGER,
    "-" HUGE_INTEGER"2",
    "-" HUGE_INTEGER"1",
    "-" HUGE_INTEGER"0",
    "-" HUGE_INTEGER,
    -IntType(static_cast<unsigned long>(ULONG_MAX)),
    -IntType(static_cast<unsigned long>(LONG_MAX) + 3),
    -IntType(static_cast<unsigned long>(LONG_MAX) + 2),
    // Fit into a native long:
    LONG_MIN, -LONG_MAX, LONG_MIN+2, -32768, -32767,
    -5000, -4999, -4998, -4997, -3000, -1000,
    -5, -4, -3, -2, -1,
    0,
    1, 2, 3, 4, 5,
    1000, 3000, 4997, 4998, 4999, 5000,
    32767, 32768, LONG_MAX-2, LONG_MAX-1, LONG_MAX,
    // Too large for a native long:
    static_cast<unsigned long>(LONG_MAX) + 1,
    static_cast<unsigned long>(LONG_MAX) + 2,
    static_cast<unsigned long>(ULONG_MAX),
    HUGE_INTEGER,
    HUGE_INTEGER"0",
    HUGE_INTEGER"1",
    HUGE_INTEGER"2",
    ENORMOUS_INTEGER
};

template <typename IntType>
const unsigned IntegerTest::Data<IntType>::nCases = 49;

template <typename IntType>
const long IntegerTest::Data<IntType>::longCases[] = {
    LONG_MIN, -LONG_MAX, LONG_MIN+2, -32768, -32767,
    -5000, -4999, -4998, -4997, -3000, -1000,
    -5, -4, -3, -2, -1,
    0,
    1, 2, 3, 4, 5,
    1000, 3000, 4997, 4998, 4999, 5000,
    32767, 32768, LONG_MAX-2, LONG_MAX-1, LONG_MAX
};

template <typename IntType>
const long IntegerTest::Data<IntType>::nLongCases = 33;

const size_t IntegerTest::nZero = 5;
const std::string IntegerTest::f2k[5] = {
    F4, F8, F16, F32, F64 };
const std::string IntegerTest::zero2k[5] = {
    ZERO4, ZERO8, ZERO16, ZERO32, ZERO64 };
const std::string IntegerTest::zero2kdec[5] = {
    ZERO3, ZERO7, ZERO15, ZERO31, ZERO63 };

// Boilerplate stuff.

void addInteger(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(IntegerTest::suite());
}

template <>
inline IntegerTest::Data<LargeInteger>& IntegerTest::data<LargeInteger>() {
    return dataL;
}

template <>
inline IntegerTest::Data<Integer>& IntegerTest::data<Integer>() {
    return dataI;
}

