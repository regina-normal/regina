
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

#include <array>
#include "concepts/core.h"
#include "maths/integer.h"

#include "testhelper.h"

using regina::ArbitraryPrecisionInteger;
using regina::CppInteger;
using regina::SignedCppInteger;
using regina::UnsignedCppInteger;

using regina::IntegerBase;
using regina::Integer;
using regina::LargeInteger;

// Some strings representing very large integers.
// We break these into smaller chunks because the long integer string constants
// break vim's syntax highlighting (for the entire file).
#define HUGE_INTEGER "1236498172" "6394781629" "3784619237" "8649187456" \
    "9283746672"
#define ENORMOUS_INTEGER "1157920892" "3731619542" "3570985008" "6879078532" \
    "6998466564" "0564039457" "5840079131" "29639936"

// A sequence of 256 zero digits.
#define ZEROES \
    "0000000000000000000000000000000000000000000000000000000000000000" \
    "0000000000000000000000000000000000000000000000000000000000000000" \
    "0000000000000000000000000000000000000000000000000000000000000000" \
    "0000000000000000000000000000000000000000000000000000000000000000"

template <typename IntegerType>
static void verifyLarge(const IntegerType& x, const std::string& expect) {
    // Verifies that x uses a large integer representation, and appears
    // self-consistent with a string value equal to expect.
    SCOPED_TRACE_STDSTRING(expect);
    ASSERT_FALSE(expect.empty());

    int sign = (expect[0] == '-' ? -1 : expect[0] == '0' ? 0 : 1);

    EXPECT_FALSE(x.isNative());
    EXPECT_FALSE(x.isInfinite());
    EXPECT_EQ(x.stringValue(), expect);
    {
        std::ostringstream out;
        out << x;
        EXPECT_EQ(out.str(), expect);
    }
    EXPECT_EQ(x.sign(), sign);

    if (sign < 0) {
        EXPECT_LT(x, 0);
        EXPECT_FALSE(x.isZero());
    } else if (sign == 0) {
        EXPECT_EQ(x, 0);
        EXPECT_TRUE(x.isZero());
    } else {
        EXPECT_GT(x, 0);
        EXPECT_FALSE(x.isZero());
    }
}

template <typename IntegerType>
static void verifyNative(const IntegerType& x, long expect) {
    // Verifies that x uses a native integer representation, and appears
    // self-consistent with a long value equal to expect.
    SCOPED_TRACE_NUMERIC(expect);

    int sign = (expect < 0 ? -1 : expect == 0 ? 0 : 1);

    EXPECT_TRUE(x.isNative());
    EXPECT_FALSE(x.isInfinite());
    EXPECT_EQ(x.template unsafeValue<long>(), expect);
    EXPECT_NO_THROW({
        EXPECT_EQ(x.template safeValue<long>(), expect);
    });
    EXPECT_EQ(x.sign(), sign);

    std::string expectStr = std::to_string(expect);
    EXPECT_EQ(x.stringValue(), expectStr);
    {
        std::ostringstream out;
        out << x;
        EXPECT_EQ(out.str(), expectStr);
    }

    if (sign < 0) {
        EXPECT_LT(x, 0);
        EXPECT_FALSE(x.isZero());
    } else if (sign == 0) {
        EXPECT_EQ(x, 0);
        EXPECT_TRUE(x.isZero());
    } else {
        EXPECT_GT(x, 0);
        EXPECT_FALSE(x.isZero());
    }
}

template <typename IntegerType>
static void verifyInfinite(const IntegerType& x) {
    // Verifies that x appears to be self-consistent and equal to infinity.
    static_assert(IntegerType::supportsInfinity);

    EXPECT_FALSE(x.isNative());
    EXPECT_TRUE(x.isInfinite());
    EXPECT_THROW({ x.template safeValue<long>(); }, regina::NoSolution);
    EXPECT_EQ(x.sign(), 1);
    EXPECT_FALSE(x.isZero());
    EXPECT_EQ(x.stringValue(), "inf");
    {
        std::ostringstream out;
        out << x;
        EXPECT_EQ(out.str(), "inf");
    }
}

template <typename IntegerType>
static void verifyIdentical(const IntegerType& x, const IntegerType& y) {
    // Verifies that x and y appear to be equal, _and_ with identical internal
    // representations.
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_REGINA(y);

    EXPECT_EQ(x, y);
    EXPECT_FALSE(x != y);
    EXPECT_EQ(x.isNative(), y.isNative());
    EXPECT_EQ(x.isInfinite(), y.isInfinite());
    if (x.isNative()) {
        EXPECT_EQ(x.template unsafeValue<long>(),
            y.template unsafeValue<long>());
    }
    EXPECT_EQ(x.str(), y.str());
}

template <typename IntegerType>
static void verifyCopyAssign(const IntegerType& x) {
    SCOPED_TRACE_REGINA(x);

    // Verify the copy constructor and self-assignment.
    {
        IntegerType y(x);
        verifyIdentical(y, x);

        y = y;
        verifyIdentical(y, x);
    }

    // Verify assignment to and from x.
    IntegerType z(5);
    verifyNative(z, 5);
    z = x;
    verifyIdentical(z, x);
    z = 7;
    verifyNative(z, 7);
    z = x;
    verifyIdentical(z, x);
    z = HUGE_INTEGER;
    verifyLarge(z, HUGE_INTEGER);
    z = x;
    verifyIdentical(z, x);
    z = "-" HUGE_INTEGER;
    verifyLarge(z, "-" HUGE_INTEGER);
    z = x;
    verifyIdentical(z, x);
    z = 8;
    verifyNative(z, 8);

    // Verify raw GMP assignment.
    z = x;
    verifyIdentical(z, x);
    IntegerType raw(HUGE_INTEGER);
    z.setRaw(raw.rawData());
    verifyLarge(z, HUGE_INTEGER);

    if (! x.isInfinite()) {
        // Note: calling rawData() will force a GMP representation.
        IntegerType large(x);
        large.makeLarge();

        IntegerType y(x);
        {
            IntegerType v(5);
            v.setRaw(y.rawData());
            verifyIdentical(v, large);
        }
        {
            IntegerType v(HUGE_INTEGER);
            v.setRaw(y.rawData());
            verifyIdentical(v, large);
        }

        // Ensure that y is not broken after using its raw data.
        verifyIdentical(y, large);
        EXPECT_EQ(large, x);
    }
}

template <typename IntegerType>
static void verifyCopyAssignLarge(const IntegerType& x,
        const std::string& expect) {
    verifyLarge(x, expect);
    verifyCopyAssign(x);
}

template <typename IntegerType>
static void verifyCopyAssignNative(const IntegerType& x, long expect) {
    verifyNative(x, expect);
    verifyCopyAssign(x);
}

template <typename IntegerType>
static void verifyCopyAssignInfinite(const IntegerType& x) {
    verifyInfinite(x);
    verifyCopyAssign(x);
}

template <typename IntegerType>
class IntegerTest : public testing::Test {
    protected:
        // TODO: Perhaps make these non-static?
        // A set of test cases that we can work through, in increasing order.
        inline static const std::array<IntegerType, 49> cases {
            // Too low for a native long:
            "-" ENORMOUS_INTEGER,
            "-" HUGE_INTEGER "2",
            "-" HUGE_INTEGER "1",
            "-" HUGE_INTEGER "0",
            "-" HUGE_INTEGER,
            -IntegerType(static_cast<unsigned long>(ULONG_MAX)),
            -IntegerType(static_cast<unsigned long>(LONG_MAX) + 3),
            -IntegerType(static_cast<unsigned long>(LONG_MAX) + 2),
            // Fits into a native long:
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
            HUGE_INTEGER "0",
            HUGE_INTEGER "1",
            HUGE_INTEGER "2",
            ENORMOUS_INTEGER
        };

        // A set of test cases that fit inside a C++ long, in increasing order.
        inline static const std::array<long, 33> longCases {
            LONG_MIN, -LONG_MAX, LONG_MIN+2, -32768, -32767,
            -5000, -4999, -4998, -4997, -3000, -1000,
            -5, -4, -3, -2, -1,
            0,
            1, 2, 3, 4, 5,
            1000, 3000, 4997, 4998, 4999, 5000,
            32767, 32768, LONG_MAX-2, LONG_MAX-1, LONG_MAX
        };
};

using IntegerTypes = ::testing::Types<regina::Integer, regina::LargeInteger>;
TYPED_TEST_SUITE(IntegerTest, IntegerTypes);

TYPED_TEST(IntegerTest, constructCopyAssignNative) {
    // Create some out-of-native-range integers using string manipulation.
    std::string sLongMaxInc = std::to_string(LONG_MAX);
    ++sLongMaxInc.back();
    std::string sLongMinDec = std::to_string(LONG_MIN);
    ++sLongMinDec.back();
    std::string sULongMax = std::to_string(ULONG_MAX);

    verifyCopyAssignNative(TypeParam(), 0);
    verifyCopyAssignNative(TypeParam(1), 1);
    verifyCopyAssignNative(TypeParam(2), 2);
    verifyCopyAssignNative(TypeParam(-1), -1);
    verifyCopyAssignNative(TypeParam(-2), -2);
    verifyCopyAssignNative(TypeParam(int(100)), 100);
    verifyCopyAssignNative(TypeParam(int(-32768)), -32768);
    verifyCopyAssignNative(TypeParam(unsigned(65535)), 65535);
    verifyCopyAssignNative(TypeParam(long(2147483647)), 2147483647);
    verifyCopyAssignNative(TypeParam(long(-2147483648)), -2147483648);
    verifyCopyAssignNative(TypeParam(long(LONG_MAX)), LONG_MAX);
    verifyCopyAssignNative(TypeParam(long(LONG_MIN)), LONG_MIN);
    verifyCopyAssignNative(TypeParam((unsigned long)(LONG_MAX)), LONG_MAX);
    verifyCopyAssignLarge(TypeParam(((unsigned long)(LONG_MAX)) + 1),
        sLongMaxInc);
    verifyCopyAssignLarge(-TypeParam(((unsigned long)(LONG_MAX)) + 2),
        sLongMinDec);
    verifyCopyAssignLarge(TypeParam((unsigned long)(ULONG_MAX)), sULongMax);

    TypeParam x;
    verifyCopyAssignNative(x, 0);
    x = 1;
    verifyCopyAssignNative(x, 1);
    x = 2;
    verifyCopyAssignNative(x, 2);
    x = -1;
    verifyCopyAssignNative(x, -1);
    x = -2;
    verifyCopyAssignNative(x, -2);
    x = int(100);
    verifyCopyAssignNative(x, 100);
    x = int(-32768);
    verifyCopyAssignNative(x, -32768);
    x = unsigned(65535);
    verifyCopyAssignNative(x, 65535);
    x = long(2147483647);
    verifyCopyAssignNative(x, 2147483647);
    x = long(-2147483648);
    verifyCopyAssignNative(x, -2147483648);
    x = long(LONG_MAX);
    verifyCopyAssignNative(x, LONG_MAX);
    x = long(LONG_MIN);
    verifyCopyAssignNative(x, LONG_MIN);
    x = (unsigned long)(LONG_MAX);
    verifyCopyAssignNative(x, LONG_MAX);
    x = ((unsigned long)(LONG_MAX)) + 1;
    verifyCopyAssignLarge(x, sLongMaxInc);
    x = ((unsigned long)(LONG_MAX)) + 2;
    x.negate();
    verifyCopyAssignLarge(x, sLongMinDec);
    x = (unsigned long)(ULONG_MAX);
    verifyCopyAssignLarge(x, sULongMax);
}

template <typename IntegerType>
static void verifyNativeFromString(const std::string& s, int base, long value) {
    std::string use[4] = { s, s + " \t\r\n  ", " \t\r\n  " + s,
        " \t\r\n  " + s + " \t\r\n  " };

    for (const std::string& str : use) {
        SCOPED_TRACE_STDSTRING(str);

        {
            IntegerType x(str, base);
            if (base > 0)
                EXPECT_EQ(x.stringValue(base), s);
            verifyCopyAssignNative(x, value);
        }
        {
            IntegerType x(str.c_str(), base);
            if (base > 0)
                EXPECT_EQ(x.stringValue(base), s);
            verifyCopyAssignNative(x, value);
        }

        if (base == 10) {
            {
                IntegerType x(5);
                EXPECT_TRUE(x.isNative());
                x = str;
                EXPECT_EQ(x.str(), s);
                verifyCopyAssignNative(x, value);
            }
            {
                IntegerType x(5);
                EXPECT_TRUE(x.isNative());
                x = str.c_str();
                EXPECT_EQ(x.str(), s);
                verifyCopyAssignNative(x, value);
            }
        }
    }

    // Try strings with errors.
    EXPECT_THROW({
        IntegerType(s + "!", base);
    }, regina::InvalidArgument);
    EXPECT_THROW({
        IntegerType((s + "!").c_str(), base);
    }, regina::InvalidArgument);
}

template <typename IntegerType>
static void verifyLargeFromString(const std::string& s) {
    std::string use[4] = { s, s + " \t\r\n  ", " \t\r\n  " + s,
        " \t\r\n  " + s + " \t\r\n  " };

    for (const std::string& str : use) {
        SCOPED_TRACE_STDSTRING(str);

        {
            IntegerType x(str, 10);
            verifyCopyAssignLarge(x, s);
        }
        {
            IntegerType x(str.c_str(), 10);
            verifyCopyAssignLarge(x, s);
        }
        {
            IntegerType x(5);
            EXPECT_TRUE(x.isNative());
            x = str;
            verifyCopyAssignLarge(x, s);
        }
        {
            IntegerType x(5);
            EXPECT_TRUE(x.isNative());
            x = str.c_str();
            verifyCopyAssignLarge(x, s);
        }
    }

    // Try strings with errors.
    EXPECT_THROW({ IntegerType(s + "!"); }, regina::InvalidArgument);
    EXPECT_THROW({ IntegerType((s + "!").c_str()); }, regina::InvalidArgument);
}

template <typename IntegerType>
static void verifyLargeFromString(const std::string& s, int base,
        const std::string& valueBase10) {
    std::string use[4] = { s, s + " \t\r\n  ", " \t\r\n  " + s,
        " \t\r\n  " + s + " \t\r\n  " };

    for (const std::string& str : use) {
        SCOPED_TRACE_STDSTRING(str);

        {
            IntegerType x(str, base);
            if (base > 0)
                EXPECT_EQ(x.stringValue(base), s);
            verifyCopyAssignLarge(x, valueBase10);
        }
        {
            IntegerType x(str.c_str(), base);
            if (base > 0)
                EXPECT_EQ(x.stringValue(base), s);
            verifyCopyAssignLarge(x, valueBase10);
        }
        if (base == 10) {
            {
                IntegerType x(5);
                EXPECT_TRUE(x.isNative());
                x = str;
                verifyCopyAssignLarge(x, s);
            }
            {
                IntegerType x(5);
                EXPECT_TRUE(x.isNative());
                x = str.c_str();
                verifyCopyAssignLarge(x, s);
            }
        }
    }

    // Try strings with errors.
    EXPECT_THROW({
        IntegerType(s + "!", base);
    }, regina::InvalidArgument);
    EXPECT_THROW({
        IntegerType((s + "!").c_str(), base);
    }, regina::InvalidArgument);
}

TYPED_TEST(IntegerTest, constructCopyAssignString) {
    // Create some out-of-native-range integers using string manipulation.
    std::string sLongMaxInc = std::to_string(LONG_MAX);
    ++sLongMaxInc.back();
    std::string sLongMinDec = std::to_string(LONG_MIN);
    ++sLongMinDec.back();
    std::string sULongMax = std::to_string(ULONG_MAX);

    verifyNativeFromString<TypeParam>(std::to_string(LONG_MAX), 10, LONG_MAX);
    verifyNativeFromString<TypeParam>(std::to_string(LONG_MIN), 10, LONG_MIN);
    verifyLargeFromString<TypeParam>(sLongMaxInc);
    verifyLargeFromString<TypeParam>(sLongMinDec);
    verifyLargeFromString<TypeParam>(sULongMax);
    verifyLargeFromString<TypeParam>(HUGE_INTEGER);
    verifyLargeFromString<TypeParam>("-" HUGE_INTEGER);

    // Test string constructors in different bases.
    verifyNativeFromString<TypeParam>("101", 2, 5);
    verifyNativeFromString<TypeParam>("-101", 2, -5);
    verifyNativeFromString<TypeParam>("121", 3, 16);
    verifyNativeFromString<TypeParam>("-121", 3, -16);
    verifyNativeFromString<TypeParam>("1af", 16, 431);
    verifyNativeFromString<TypeParam>("-1af", 16, -431);
    verifyNativeFromString<TypeParam>("201", 31, 1923);
    verifyNativeFromString<TypeParam>("-201", 31, -1923);
    verifyNativeFromString<TypeParam>("121", 0, 121);
    verifyNativeFromString<TypeParam>("-121", 0, -121);
    verifyNativeFromString<TypeParam>("034", 0, 28);
    verifyNativeFromString<TypeParam>("-034", 0, -28);
    verifyNativeFromString<TypeParam>("0x1af", 0, 431);
    verifyNativeFromString<TypeParam>("-0x1af", 0, -431);
    verifyNativeFromString<TypeParam>("0X1af", 0, 431);
    verifyNativeFromString<TypeParam>("-0X1af", 0, -431);
    // We split the strings below into chunks so that vim's syntax highlighting
    // can cope.
    verifyLargeFromString<TypeParam>("1" "000000000000000000000000000000", 29,
        "74462898441" "67512290229" "30182271994" "67668020601");
    verifyLargeFromString<TypeParam>("-1" "000000000000000000000000000000", 29,
        "-74462898441" "67512290229" "30182271994" "67668020601");
    verifyLargeFromString<TypeParam>(
        "74462898441" "67512290229" "30182271994" "67668020601",
        0, "74462898441" "67512290229" "30182271994" "67668020601");
    verifyLargeFromString<TypeParam>(
        "-74462898441" "67512290229" "30182271994" "67668020601",
        0, "-74462898441" "67512290229" "30182271994" "67668020601");
    verifyLargeFromString<TypeParam>(
        "01" "000000000000000000000000000000000000000000000",
        0, "4355614296" "5880123323" "3119497512" "66331066368");
    verifyLargeFromString<TypeParam>(
        "-01" "000000000000000000000000000000000000000000000",
        0, "-4355614296" "5880123323" "3119497512" "66331066368");
    verifyLargeFromString<TypeParam>("0x1" "0000000000000000000000000000000000",
        0, "8711228593" "1760246646" "6238995025" "32662132736");
    verifyLargeFromString<TypeParam>("-0x1" "0000000000000000000000000000000000",
        0, "-8711228593" "1760246646" "6238995025" "32662132736");
    verifyLargeFromString<TypeParam>("0X1" "0000000000000000000000000000000000",
        0, "8711228593" "1760246646" "6238995025" "32662132736");
    verifyLargeFromString<TypeParam>("-0X1" "0000000000000000000000000000000000",
        0, "-8711228593" "1760246646" "6238995025" "32662132736");
}

static void verifyInfiniteFromString(const char* str) {
    // Test construction and assignment from the given string, which should
    // be a recognised string representation of infinity.
    SCOPED_TRACE_CSTRING(str);
    std::string cppstr(str);

    {
        LargeInteger x(str);
        verifyInfinite(x);
    }
    {
        LargeInteger x(cppstr);
        verifyInfinite(x);
    }
    {
        LargeInteger x(5);
        EXPECT_TRUE(x.isNative());
        EXPECT_FALSE(x.isInfinite());
        x = str;
        verifyInfinite(x);
    }
    {
        LargeInteger x(5);
        EXPECT_TRUE(x.isNative());
        EXPECT_FALSE(x.isInfinite());
        x = cppstr;
        verifyInfinite(x);
    }
}

TYPED_TEST(IntegerTest, constructCopyAssignInfinity) {
    if constexpr (TypeParam::supportsInfinity) {
        verifyCopyAssignInfinite(TypeParam::infinity);

        // Ensure that makeInfinte() behaves correctly:
        {
            TypeParam x(5);
            EXPECT_TRUE(x.isNative());
            EXPECT_FALSE(x.isInfinite());
            x.makeInfinite();
            verifyCopyAssignInfinite(x);
        }
        {
            TypeParam x(HUGE_INTEGER);
            EXPECT_FALSE(x.isNative());
            EXPECT_FALSE(x.isInfinite());
            x.makeInfinite();
            verifyCopyAssignInfinite(x);
        }
        {
            TypeParam x(LargeInteger::infinity);
            verifyInfinite(x);
            x.makeInfinite();
            verifyCopyAssignInfinite(x);
        }

        // Test construction and assignment from strings:
        verifyInfiniteFromString("inf");
        verifyInfiniteFromString("infinity");
        verifyInfiniteFromString(" \tinf");
        verifyInfiniteFromString(" \tinfinity! ");
        verifyInfiniteFromString("  infimum");
    }
}

TYPED_TEST(IntegerTest, swap) {
    // Create LONG_MAX + 1 using direct string manipulation.
    std::string sLongMaxInc = std::to_string(LONG_MAX);
    ++sLongMaxInc.back();

    {
        TypeParam a(3);
        TypeParam b(LONG_MIN);
        TypeParam c(LONG_MAX); ++c;
        TypeParam d(HUGE_INTEGER);
        TypeParam e(1000);

        verifyNative(a, 3);
        verifyNative(b, LONG_MIN);
        verifyLarge(c, sLongMaxInc);
        verifyLarge(d, HUGE_INTEGER);
        verifyNative(e, 1000);

        std::swap(a, b); // native <-> native

        verifyNative(b, 3);
        verifyNative(a, LONG_MIN);

        std::swap(a, c); // native <-> long

        verifyLarge(a, sLongMaxInc);
        verifyNative(c, LONG_MIN);

        std::swap(a, d); // long <-> long

        verifyLarge(a, HUGE_INTEGER);
        verifyLarge(d, sLongMaxInc);

        std::swap(a, e); // long <-> native

        verifyNative(a, 1000);
        verifyLarge(e, HUGE_INTEGER);

        std::swap(a, a); // long <-> self

        verifyNative(a, 1000);

        std::swap(e, e); // large <-> self

        verifyLarge(e, HUGE_INTEGER);
    }

    if constexpr (TypeParam::supportsInfinity) {
        TypeParam a(3);
        TypeParam b(LONG_MIN);
        TypeParam c(LONG_MAX); ++c;
        TypeParam d(HUGE_INTEGER);
        TypeParam i(TypeParam::infinity);
        TypeParam j; j.makeInfinite();

        verifyNative(a, 3);
        verifyNative(b, LONG_MIN);
        verifyLarge(c, sLongMaxInc);
        verifyLarge(d, HUGE_INTEGER);
        verifyInfinite(i);
        verifyInfinite(j);

        std::swap(a, i); // native <-> infinity

        verifyInfinite(a);
        verifyNative(i, 3);

        std::swap(c, a); // long <-> infinity

        verifyLarge(a, sLongMaxInc);
        verifyInfinite(c);

        std::swap(c, d); // infinity <-> long

        verifyLarge(c, HUGE_INTEGER);
        verifyInfinite(d);

        std::swap(d, b); // infinity <-> native

        verifyInfinite(b);
        verifyNative(d, LONG_MIN);

        std::swap(b, j); // infinity <-> infinity

        verifyInfinite(b);
        verifyInfinite(j);

        std::swap(b, b); // infinity <-> self

        verifyInfinite(b);
    }
}

#ifdef INT128_AVAILABLE
static void verifyEqual128(const regina::NativeInteger<16>& x,
        const regina::NativeInteger<16>& y) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_REGINA(y);

    EXPECT_EQ(x, y);
    EXPECT_EQ(x.nativeValue(), y.nativeValue());
    EXPECT_EQ(x.str(), y.str());
}

template <typename IntegerType>
static void verifyNative128(const regina::NativeInteger<16>& native,
        const char* string) {
    using CppType = regina::IntOfSize<16>::type;

    EXPECT_EQ(native.str(), string);
    EXPECT_EQ(IntegerType(native).str(), string);
    {
        IntegerType assigned = 7;
        EXPECT_NE(assigned.str(), string);
        assigned = native;
        EXPECT_EQ(assigned.str(), string);
    }

    verifyEqual128(native, IntegerType(string).template safeValue<CppType>());
    verifyEqual128(native, IntegerType(string).template unsafeValue<CppType>());
    verifyEqual128(native, IntegerType(native).template safeValue<CppType>());
    verifyEqual128(native, IntegerType(native).template unsafeValue<CppType>());

    // Make sure large-to-native conversion works even for numbers that do not
    // enter the highest order long-sized block.  For most machines this means
    // the integers fit into a single long, so here we force them into a large
    // (GMP) representation regardless.
    IntegerType large(native);
    large.makeLarge();
    verifyEqual128(native, large.template safeValue<CppType>());
    verifyEqual128(native, large.template unsafeValue<CppType>());
}

TYPED_TEST(IntegerTest, constructNative128) {
    // Test conversions involving native types that are larger than long.
    regina::NativeInteger<16> pos62 = 1;
    pos62 *= 1073741824; // 2^30
    pos62 *= 1073741824; // 2^30
    pos62 *= 4;
    regina::NativeInteger<16> neg62 = -pos62;
    regina::NativeInteger<16> pos63 = pos62 * 2;
    regina::NativeInteger<16> neg63 = -pos63;
    regina::NativeInteger<16> pos64 = pos63 * 2;
    regina::NativeInteger<16> neg64 = -pos64;
    regina::NativeInteger<16> pos126 = pos63 * pos63;
    regina::NativeInteger<16> neg126 = -pos126;
    regina::NativeInteger<16> pos127 = pos126 * 2; // Should overflow to -2^127
    regina::NativeInteger<16> neg127 = neg126 * 2;

    regina::NativeInteger<16> pos126_62 = pos126 + pos62;
    regina::NativeInteger<16> pos126_63 = pos126 + pos63;
    regina::NativeInteger<16> neg126_62 = neg126 + neg62;
    regina::NativeInteger<16> neg126_63 = neg126 + neg63;

    regina::NativeInteger<16> maxVal(
        ~(regina::IntOfSize<16>::type(1) << 127));

    // We split the strings below into chunks so that vim's syntax highlighting
    // can cope.
    verifyNative128<TypeParam>(pos62, "4611686018" "427387904");
    verifyNative128<TypeParam>(neg62, "-4611686018" "427387904");
    verifyNative128<TypeParam>(pos63, "9223372036" "854775808");
    verifyNative128<TypeParam>(neg63, "-9223372036" "854775808");
    verifyNative128<TypeParam>(pos64, "1844674407" "3709551616");
    verifyNative128<TypeParam>(neg64, "-1844674407" "3709551616");
    verifyNative128<TypeParam>(pos126,
        "8507059173" "0234615865" "8436518579" "42052864");
    verifyNative128<TypeParam>(neg126,
        "-8507059173" "0234615865" "8436518579" "42052864");
    verifyNative128<TypeParam>(pos126_62,
        "8507059173" "0234615870" "4553378763" "69440768");
    verifyNative128<TypeParam>(neg126_62,
        "-8507059173" "0234615870" "4553378763" "69440768");
    verifyNative128<TypeParam>(pos126_63,
        "8507059173" "0234615875" "0670238947" "96828672");
    verifyNative128<TypeParam>(neg126_63,
        "-8507059173" "0234615875" "0670238947" "96828672");
    // Recall that pos127 overflows.
    verifyNative128<TypeParam>(pos127,
        "-1701411834" "6046923173" "1687303715" "884105728");
    verifyNative128<TypeParam>(neg127,
        "-1701411834" "6046923173" "1687303715" "884105728");
    verifyNative128<TypeParam>(maxVal,
        "1701411834" "6046923173" "1687303715" "884105727");
}
#endif

TYPED_TEST(IntegerTest, stringValue) {
    // We've already tested stringValue() heavily with the default base of 10.
    // Here we test other bases.

    EXPECT_EQ(TypeParam(0).stringValue(2), "0");
    EXPECT_EQ(TypeParam(0).stringValue(3), "0");
    EXPECT_EQ(TypeParam(0).stringValue(21), "0");
    EXPECT_EQ(TypeParam(0).stringValue(22), "0");
    EXPECT_EQ(TypeParam(0).stringValue(36), "0");
    EXPECT_EQ(TypeParam(42).stringValue(2), "101010");
    EXPECT_EQ(TypeParam(42).stringValue(3), "1120");
    EXPECT_EQ(TypeParam(42).stringValue(21), "20");
    EXPECT_EQ(TypeParam(42).stringValue(22), "1k");
    EXPECT_EQ(TypeParam(42).stringValue(36), "16");
    EXPECT_EQ(TypeParam(71).stringValue(36), "1z");
    EXPECT_EQ(TypeParam(-42).stringValue(2), "-101010");
    EXPECT_EQ(TypeParam(-42).stringValue(3), "-1120");
    EXPECT_EQ(TypeParam(-42).stringValue(21), "-20");
    EXPECT_EQ(TypeParam(-42).stringValue(22), "-1k");
    EXPECT_EQ(TypeParam(-42).stringValue(36), "-16");
    EXPECT_EQ(TypeParam(-71).stringValue(36), "-1z");

    // In the following tests, the hard-coded integer is 2^130.
    // As before, we split the strings into chunks so that vim's syntax
    // highlighting can cope.
    TypeParam pos( "13611294676" "83753853853" "49842972707" "2845824");
    TypeParam neg("-13611294676" "83753853853" "49842972707" "2845824");
    EXPECT_EQ(pos.stringValue(2), "1" "000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000");
    EXPECT_EQ(pos.stringValue(4), "1" "000000000000000000000000000000"
        "00000000000000000000000000000000000");
    EXPECT_EQ(pos.stringValue(16), "4" "00000000000000000000000000000000");
    EXPECT_EQ(pos.stringValue(32), "1" "00000000000000000000000000");
    EXPECT_EQ(pos.stringValue(36), "1omfro7zwmumr3umxudzyj6scg");
    EXPECT_EQ(neg.stringValue(2), "-1" "000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000");
    EXPECT_EQ(neg.stringValue(4), "-1" "000000000000000000000000000000"
        "00000000000000000000000000000000000");
    EXPECT_EQ(neg.stringValue(16), "-4" "00000000000000000000000000000000");
    EXPECT_EQ(neg.stringValue(32), "-1" "00000000000000000000000000");
    EXPECT_EQ(neg.stringValue(36), "-1omfro7zwmumr3umxudzyj6scg");

    if constexpr (TypeParam::supportsInfinity)
        for (int i = 2; i <= 36; ++i)
            EXPECT_EQ(TypeParam::infinity.stringValue(i), "inf");
}

template <bool withInfinity>
static void verifyEqual(const IntegerBase<withInfinity>& a,
        const IntegerBase<withInfinity>& b) {
    SCOPED_TRACE_REGINA(a);
    SCOPED_TRACE_REGINA(b);

    EXPECT_EQ(a, b);
    EXPECT_EQ(b, a);
    EXPECT_LE(a, b);
    EXPECT_LE(b, a);
    EXPECT_GE(a, b);
    EXPECT_GE(b, a);

    EXPECT_FALSE(a != b);
    EXPECT_FALSE(b != a);
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(b < a);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(b > a);

    EXPECT_EQ(a.stringValue(), b.stringValue());
}

template <bool withInfinity>
static void verifyEqual(const IntegerBase<withInfinity>& a, long b) {
    SCOPED_TRACE_REGINA(a);
    SCOPED_TRACE_NUMERIC(b);

    EXPECT_EQ(a, b);
    EXPECT_LE(a, b);
    EXPECT_GE(a, b);

    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(a > b);

    EXPECT_EQ(a.stringValue(), std::to_string(b));
    EXPECT_EQ(a.isZero(), b == 0);
}

template <bool withInfinity>
static void verifyLess(const IntegerBase<withInfinity>& a,
        const IntegerBase<withInfinity>& b) {
    SCOPED_TRACE_REGINA(a);
    SCOPED_TRACE_REGINA(b);

    EXPECT_NE(a, b);
    EXPECT_NE(b, a);
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
    EXPECT_LE(a, b);
    EXPECT_GE(b, a);

    EXPECT_FALSE(a == b);
    EXPECT_FALSE(b == a);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(a >= b);
    EXPECT_FALSE(b < a);
    EXPECT_FALSE(b <= a);
}

template <bool withInfinity>
static void verifyLess(const IntegerBase<withInfinity>& a, long b) {
    SCOPED_TRACE_REGINA(a);
    SCOPED_TRACE_NUMERIC(b);

    EXPECT_NE(a, b);
    EXPECT_LT(a, b);
    EXPECT_LE(a, b);

    EXPECT_FALSE(a == b);
    EXPECT_FALSE(a > b);
    EXPECT_FALSE(a >= b);
}

template <bool withInfinity>
static void verifyLess(long a, const IntegerBase<withInfinity>& b) {
    SCOPED_TRACE_NUMERIC(a);
    SCOPED_TRACE_REGINA(b);

    EXPECT_NE(b, a);
    EXPECT_GT(b, a);
    EXPECT_GE(b, a);

    EXPECT_FALSE(b == a);
    EXPECT_FALSE(b < a);
    EXPECT_FALSE(b <= a);
}

TYPED_TEST(IntegerTest, comparisons) {
    for (const auto& x : this->cases) {
        verifyEqual(x, x);

        TypeParam y(x);
        EXPECT_EQ(x.isNative(), y.isNative());
        verifyEqual(x, y);

        y.makeLarge();
        EXPECT_FALSE(y.isNative());
        verifyEqual(x, y);
    }

    for (long x : this->longCases) {
        TypeParam a(x);
        TypeParam b(x);
        TypeParam c(x);
        c.makeLarge();

        EXPECT_TRUE(a.isNative());
        EXPECT_TRUE(b.isNative());
        EXPECT_FALSE(c.isNative());

        verifyEqual(a, a);
        verifyEqual(a, b);
        verifyEqual(a, c);
        verifyEqual(a, x);
        verifyEqual(c, x);

        EXPECT_TRUE(a.isNative());
        EXPECT_TRUE(b.isNative());
        EXPECT_FALSE(c.isNative());

        EXPECT_EQ(a.template unsafeValue<long>(), x);
        EXPECT_EQ(c.template unsafeValue<long>(), x);
        EXPECT_EQ(a.template safeValue<long>(), x);
        EXPECT_EQ(c.template safeValue<long>(), x);

        EXPECT_TRUE(a.isNative());
        EXPECT_FALSE(c.isNative());
    }

    for (size_t x = 0; x < this->cases.size(); ++x)
        for (size_t y = x + 1; y < this->cases.size(); ++y) {
            TypeParam a(this->cases[x]);
            TypeParam b(this->cases[y]);

            verifyLess(a, b);

            if (a.isNative()) {
                verifyLess(a.template unsafeValue<long>(), b);

                TypeParam p(a);
                p.makeLarge();
                EXPECT_FALSE(p.isNative());
                verifyLess(p, b);
            }
            if (b.isNative()) {
                verifyLess(a, b.template unsafeValue<long>());

                TypeParam q(b);
                q.makeLarge();
                EXPECT_FALSE(q.isNative());
                verifyLess(a, q);
            }
        }

    for (size_t x = 0; x < this->longCases.size(); ++x)
        for (size_t y = x + 1; y < this->longCases.size(); ++y) {
            TypeParam a(this->longCases[x]);
            TypeParam b(this->longCases[y]);

            TypeParam p(this->longCases[x]);
            TypeParam q(this->longCases[y]);
            p.makeLarge();
            q.makeLarge();

            EXPECT_TRUE(a.isNative());
            EXPECT_TRUE(b.isNative());
            EXPECT_FALSE(p.isNative());
            EXPECT_FALSE(q.isNative());

            verifyLess(a, b);
            verifyLess(a, q);
            verifyLess(p, b);
            verifyLess(p, q);
            verifyLess(a, this->longCases[y]);
            verifyLess(p, this->longCases[y]);
            verifyLess(this->longCases[x], b);
            verifyLess(this->longCases[x], q);
        }

    if constexpr (TypeParam::supportsInfinity) {
        TypeParam inf(TypeParam::infinity);

        EXPECT_EQ(inf, inf);
        EXPECT_EQ(inf, TypeParam(inf));

        for (const auto& x : this->cases)
            verifyLess(x, inf);

        for (long x : this->longCases) {
            TypeParam a(x);
            TypeParam b(x);
            b.makeLarge();

            EXPECT_TRUE(a.isNative());
            EXPECT_FALSE(b.isNative());

            verifyLess(a, inf);
            verifyLess(b, inf);

            EXPECT_TRUE(a.isNative());
            EXPECT_FALSE(b.isNative());
        }
    }
}

TYPED_TEST(IntegerTest, incDec) {
    for (const auto& x : this->cases) {
        SCOPED_TRACE_REGINA(x);

        TypeParam up = x + 1;
        TypeParam down = x - 1;
        EXPECT_GT(up, x);
        EXPECT_LT(down, x);

        TypeParam i = x;

        EXPECT_EQ(i++, x);
        EXPECT_EQ(i, up);
        EXPECT_EQ(i--, up);
        EXPECT_EQ(i, x);

        EXPECT_EQ(--i, down);
        EXPECT_EQ(i, down);
        EXPECT_EQ(++i, x);
        EXPECT_EQ(i, x);
    }

    if constexpr (TypeParam::supportsInfinity) {
        {
            TypeParam i(TypeParam::infinity);
            EXPECT_EQ(++i, TypeParam::infinity);
            EXPECT_EQ(i, TypeParam::infinity);
        }
        {
            TypeParam i(TypeParam::infinity);
            EXPECT_EQ(i++, TypeParam::infinity);
            EXPECT_EQ(i, TypeParam::infinity);
        }
        {
            TypeParam i(TypeParam::infinity);
            EXPECT_EQ(--i, TypeParam::infinity);
            EXPECT_EQ(i, TypeParam::infinity);
        }
        {
            TypeParam i(TypeParam::infinity);
            EXPECT_EQ(i--, TypeParam::infinity);
            EXPECT_EQ(i, TypeParam::infinity);
        }
    }
}

template <ArbitraryPrecisionInteger IntegerType>
static void verifySumNativeNative(long a, long b, const IntegerType& expect) {
    SCOPED_TRACE_NUMERIC(a);
    SCOPED_TRACE_NUMERIC(b);

    EXPECT_EQ(IntegerType(a) + b, expect);
    EXPECT_EQ(b + IntegerType(a), expect);
    EXPECT_EQ(a + IntegerType(b), expect);
    EXPECT_EQ(IntegerType(b) + a, expect);
    EXPECT_EQ(IntegerType(a) + IntegerType(b), expect);
    EXPECT_EQ(IntegerType(b) + IntegerType(a), expect);
    EXPECT_EQ(expect - a, b);
    EXPECT_EQ(expect - b, a);
    EXPECT_EQ(expect - IntegerType(a), b);
    EXPECT_EQ(expect - IntegerType(b), a);
}

template <ArbitraryPrecisionInteger IntegerType>
static void verifySumLargeNative(const IntegerType& a, long b,
        const IntegerType& expect) {
    SCOPED_TRACE_REGINA(a);
    SCOPED_TRACE_NUMERIC(b);

    EXPECT_EQ(a + b, expect);
    EXPECT_EQ(b + a, expect);
    EXPECT_EQ(a + IntegerType(b), expect);
    EXPECT_EQ(IntegerType(b) + a, expect);
    EXPECT_EQ(expect - a, b);
    EXPECT_EQ(expect - b, a);
    EXPECT_EQ(expect - IntegerType(b), a);
}

template <ArbitraryPrecisionInteger IntegerType>
static void verifySumLargeLarge(const IntegerType& a, const IntegerType& b,
        const IntegerType& expect) {
    SCOPED_TRACE_REGINA(a);
    SCOPED_TRACE_REGINA(b);

    EXPECT_EQ(a + b, expect);
    EXPECT_EQ(b + a, expect);
    EXPECT_EQ(expect - a, b);
    EXPECT_EQ(expect - b, a);
}

TYPED_TEST(IntegerTest, plusMinus) {
    for (const auto& x : this->cases) {
        SCOPED_TRACE_REGINA(x);

        EXPECT_EQ(x + 0L, x);
        EXPECT_EQ(x - 0L, x);
        EXPECT_EQ(0L + x, x);
        EXPECT_EQ(x + TypeParam(), x);
        EXPECT_EQ(x - TypeParam(), x);
        EXPECT_EQ(TypeParam() + x, x);
        EXPECT_EQ(TypeParam() - x, -x);

        for (const auto& y : this->cases) {
            if (y == 0)
                continue;

            SCOPED_TRACE_REGINA(y);

            EXPECT_EQ(x + y, y + x);
            EXPECT_EQ(x - y, -(y - x));

            EXPECT_EQ((x + y) - y, x);
            EXPECT_EQ((x - y) + y, x);
            EXPECT_EQ((x + y) - x, y);
            EXPECT_EQ((x - y) - x, -y);
            EXPECT_EQ(((x + y) - x) - y, 0);
            EXPECT_EQ(((x + y) - y) - x, 0);
            EXPECT_EQ(((x - y) - x) + y, 0);
            EXPECT_EQ(((x - y) + y) - x, 0);

            {
                TypeParam z = x;
                EXPECT_EQ(z += y, x + y);
                EXPECT_EQ(z, x + y);
            }
            {
                TypeParam z = x;
                EXPECT_EQ(z -= y, x - y);
                EXPECT_EQ(z, x - y);
            }

            if (y.sign() > 0) {
                EXPECT_GT(x + y, x);
                EXPECT_LT(x - y, x);
            } else if (y.sign() < 0) {
                EXPECT_LT(x + y, x);
                EXPECT_GT(x - y, x);
            } else {
                EXPECT_EQ(x + y, x);
                EXPECT_EQ(x - y, x);
            }

            if (x.sign() > 0) {
                EXPECT_GT(x + y, y);
                EXPECT_GT(x - y, -y);
            } else if (x.sign() < 0) {
                EXPECT_LT(x + y, y);
                EXPECT_LT(x - y, -y);
            } else {
                EXPECT_EQ(x + y, y);
                EXPECT_EQ(x - y, -y);
            }
        }

        for (long y : this->longCases) {
            if (y == 0)
                continue;

            SCOPED_TRACE_NUMERIC(y);

            EXPECT_EQ(x + y, y + x);

            EXPECT_EQ((x + y) - y, x);
            EXPECT_EQ((x - y) + y, x);
            EXPECT_EQ((x + y) - x, y);
            EXPECT_EQ(-((x - y) - x), y); // -y could overflow
            EXPECT_EQ(((x + y) - x) - y, 0);
            EXPECT_EQ(((x + y) - y) - x, 0);
            EXPECT_EQ(((x - y) - x) + y, 0);
            EXPECT_EQ(((x - y) + y) - x, 0);

            EXPECT_EQ((y + x) - y, x);
            EXPECT_EQ((y + x) - x, y);
            EXPECT_EQ(((y + x) - x) - y, 0);
            EXPECT_EQ(((y + x) - y) - x, 0);

            {
                TypeParam z = x;
                EXPECT_EQ(z += y, x + y);
                EXPECT_EQ(z, x + y);
            }
            {
                TypeParam z = x;
                EXPECT_EQ(z -= y, x - y);
                EXPECT_EQ(z, x - y);
            }

            if (y > 0) {
                EXPECT_GT(x + y, x);
                EXPECT_GT(y + x, x);
                EXPECT_LT(x - y, x);
            } else if (y < 0) {
                EXPECT_LT(x + y, x);
                EXPECT_LT(y + x, x);
                EXPECT_GT(x - y, x);
            } else {
                EXPECT_EQ(x + y, x);
                EXPECT_EQ(y + x, x);
                EXPECT_EQ(x - y, x);
            }

            if (x.sign() > 0) {
                EXPECT_GT(x + y, y);
                EXPECT_LT(-(x - y), y); // -y could overflow
            } else if (x.sign() < 0) {
                EXPECT_LT(x + y, y);
                EXPECT_GT(-(x - y), y); // -y could overflow
            } else {
                EXPECT_EQ(x + y, y);
                EXPECT_EQ(-(x - y), y); // -y could overflow
            }
        }
    }

    // Ad-hoc tests for native {+,-} native:
    verifySumNativeNative<TypeParam>(3, 7, 10);
    verifySumNativeNative<TypeParam>(-3, 7, 4);
    verifySumNativeNative<TypeParam>(3, -7, -4);
    verifySumNativeNative<TypeParam>(-3, -7, -10);

    // Ad-hoc tests for large {+,-} native and native {+,-} large:
    verifySumLargeNative<TypeParam>(ENORMOUS_INTEGER "0", 3,
        ENORMOUS_INTEGER "3");
    verifySumLargeNative<TypeParam>("-" ENORMOUS_INTEGER "10", 3,
        "-" ENORMOUS_INTEGER "07");
    verifySumLargeNative<TypeParam>(ENORMOUS_INTEGER "10", -3,
        ENORMOUS_INTEGER "07");
    verifySumLargeNative<TypeParam>("-" ENORMOUS_INTEGER "0", -3,
        "-" ENORMOUS_INTEGER "3");

    verifySumLargeLarge<TypeParam>(ENORMOUS_INTEGER "10",
        "-" ENORMOUS_INTEGER "07", 3);
    verifySumLargeLarge<TypeParam>("-" ENORMOUS_INTEGER "10",
        ENORMOUS_INTEGER "07", -3);

    // Ad-hoc tests for large {+,-} large:
    verifySumLargeLarge<TypeParam>("3" ZEROES, "7" ZEROES, "10" ZEROES);
    verifySumLargeLarge<TypeParam>("3" ZEROES, "-7" ZEROES, "-4" ZEROES);
    verifySumLargeLarge<TypeParam>("-3" ZEROES, "7" ZEROES, "4" ZEROES);
    verifySumLargeLarge<TypeParam>("-3" ZEROES, "-7" ZEROES, "-10" ZEROES);

    // Test around overflow points:
    TypeParam longMaxInc(LONG_MAX); ++longMaxInc;
    TypeParam longMinDec(LONG_MIN); --longMinDec;

    verifySumNativeNative<TypeParam>(LONG_MAX, 1, longMaxInc);
    verifySumNativeNative<TypeParam>(LONG_MAX, 0, LONG_MAX);
    verifySumNativeNative<TypeParam>(LONG_MIN, -1, longMinDec);
    verifySumNativeNative<TypeParam>(LONG_MIN, 0, LONG_MIN);
    verifySumNativeNative<TypeParam>(LONG_MAX, LONG_MIN, -1);
    verifySumLargeNative<TypeParam>(longMaxInc, -1, LONG_MAX);
    verifySumLargeNative<TypeParam>(longMinDec, 1, LONG_MIN);
    verifySumLargeNative<TypeParam>(longMaxInc, LONG_MIN, 0);
    verifySumLargeNative<TypeParam>(longMaxInc, LONG_MAX, ULONG_MAX);
    verifySumLargeLarge<TypeParam>(longMaxInc, longMinDec, -1);
    EXPECT_EQ(TypeParam(LONG_MAX) + TypeParam(LONG_MAX) + 1,
        TypeParam(ULONG_MAX));

    for (int k : {4, 8, 16, 32, 64, 128, 256}) {
        // The following tests work in base 16.
        SCOPED_TRACE_NUMERIC(k);

        std::string z2k(k, '0');       // The string 00..0, of length k
        std::string z2k_1(k - 1, '0'); // The string 00..0, of length k-1
        std::string f2k(k, 'F');          // The string FF..F, of length k

        verifySumLargeNative(TypeParam(f2k, 16), 1,
            TypeParam("1" + z2k, 16));
        verifySumLargeNative(TypeParam("-" + f2k, 16), -1,
            TypeParam("-1" + z2k, 16));
        verifySumLargeNative(TypeParam("1" + z2k, 16), -1,
            TypeParam(f2k, 16));
        verifySumLargeNative(TypeParam("-1" + z2k, 16), 1,
            TypeParam("-" + f2k, 16));
        verifySumLargeLarge(TypeParam("-" + f2k, 16), TypeParam("1" + z2k, 16),
            TypeParam(1));
        verifySumLargeLarge(TypeParam(f2k, 16), TypeParam("-1" + z2k, 16),
            TypeParam(-1));

        verifySumLargeNative(TypeParam("1" + z2k_1 + "1", 16), -1,
            TypeParam("1" + z2k, 16));
        verifySumLargeNative(TypeParam("-1" + z2k_1 + "1", 16), 1,
            TypeParam("-1" + z2k, 16));
        verifySumLargeNative(TypeParam("1" + z2k, 16), 1,
            TypeParam("1" + z2k_1 + "1", 16));
        verifySumLargeNative(TypeParam("-1" + z2k, 16), -1,
            TypeParam("-1" + z2k_1 + "1", 16));
        verifySumLargeLarge(TypeParam("1" + z2k_1 + "1", 16),
            TypeParam("-1" + z2k, 16),
            TypeParam(1));
        verifySumLargeLarge(TypeParam("-1" + z2k_1 + "1", 16),
            TypeParam("1" + z2k, 16),
            TypeParam(-1));
    }

    if constexpr (TypeParam::supportsInfinity) {
        TypeParam inf(TypeParam::infinity);
        EXPECT_EQ(inf + inf, inf);
        EXPECT_EQ(inf - inf, inf);

        for (const auto& x : this->cases) {
            SCOPED_TRACE_REGINA(x);

            EXPECT_EQ(inf + x, inf);
            EXPECT_EQ(inf - x, inf);
            EXPECT_EQ(x + inf, inf);
            EXPECT_EQ(x - inf, inf);
        }

        for (long x : this->longCases) {
            SCOPED_TRACE_NUMERIC(x);

            EXPECT_EQ(inf + x, inf);
            EXPECT_EQ(inf - x, inf);
            EXPECT_EQ(x + inf, inf);
        }
    }
}

template <ArbitraryPrecisionInteger IntegerType>
static void verifyProductNativeNative(long a, long b,
        const IntegerType& expect) {
    SCOPED_TRACE_NUMERIC(a);
    SCOPED_TRACE_NUMERIC(b);

    EXPECT_EQ(IntegerType(a) * b, expect);
    EXPECT_EQ(b * IntegerType(a), expect);
    EXPECT_EQ(a * IntegerType(b), expect);
    EXPECT_EQ(IntegerType(b) * a, expect);
    EXPECT_EQ(IntegerType(a) * IntegerType(b), expect);
    EXPECT_EQ(IntegerType(b) * IntegerType(a), expect);
}

template <ArbitraryPrecisionInteger IntegerType>
static void verifyProductLargeNative(const IntegerType& a, long b,
        const IntegerType& expect) {
    SCOPED_TRACE_REGINA(a);
    SCOPED_TRACE_NUMERIC(b);

    EXPECT_EQ(a * b, expect);
    EXPECT_EQ(b * a, expect);
    EXPECT_EQ(a * IntegerType(b), expect);
    EXPECT_EQ(IntegerType(b) * a, expect);
}

TYPED_TEST(IntegerTest, multiply) {
    for (const auto& x : this->cases) {
        SCOPED_TRACE_REGINA(x);

        verifyProductLargeNative(x, 2, x + x);
        verifyProductLargeNative(x, 1, x);
        verifyProductLargeNative(x, 0, TypeParam());
        verifyProductLargeNative(x, -1, -x);
        verifyProductLargeNative(x, -2, -x - x);

        for (const auto& y : this->cases) {
            if (y == 0)
                continue;

            SCOPED_TRACE_REGINA(y);

            // Test the commutative law.
            EXPECT_EQ(x * y, y * x);

            // Test the distributive law.
            EXPECT_EQ(x * (y + 1), (x * y) + x);
            EXPECT_EQ(x * (y - 1), (x * y) - x);
            EXPECT_EQ(x * (y + TypeParam(HUGE_INTEGER)),
                (x * y) + (x * TypeParam(HUGE_INTEGER)));
            EXPECT_EQ(x * (y - TypeParam(HUGE_INTEGER)),
                (x * y) - (x * TypeParam(HUGE_INTEGER)));

            // Other simple arithmetic tests.
            EXPECT_EQ(x * (-y), -(x * y));
            EXPECT_EQ((-x) * (-y), x * y);
            EXPECT_EQ(x * (-y) + x * y, 0);

            // Test that *= behaves as it should.
            {
                TypeParam z = x;
                EXPECT_EQ(z *= y, x * y);
                EXPECT_EQ(z, x * y);
            }

            // Test signs and ordering.
            if (x.sign() > 0 && y.sign() > 0) {
                EXPECT_GT(x * y, 0);
                EXPECT_GE(x * y, x);
                EXPECT_GE(x * y, y);
            } else if (x.sign() > 0 && y.sign() < 0) {
                EXPECT_LT(x * y, 0);
                EXPECT_LE(x * y, -x);
                EXPECT_LE(x * y, y);
            } else if (x.sign() < 0 && y.sign() > 0) {
                EXPECT_LT(x * y, 0);
                EXPECT_LE(x * y, x);
                EXPECT_LE(x * y, -y);
            } else if (x.sign() < 0 && y.sign() < 0) {
                EXPECT_GT(x * y, 0);
                EXPECT_GE(x * y, -x);
                EXPECT_GE(x * y, -y);
            }
        }

        for (long y : this->longCases) {
            if (y == 0)
                continue;

            SCOPED_TRACE_NUMERIC(y);

            // Test the commutative law.
            EXPECT_EQ(x * y, y * x);

            // Test the distributive law.
            EXPECT_EQ(x * (TypeParam(y) + 1), (x * y) + x);
            EXPECT_EQ(x * (TypeParam(y) - 1), (x * y) - x);
            EXPECT_EQ(x * (y + TypeParam(HUGE_INTEGER)),
                (x * y) + (x * TypeParam(HUGE_INTEGER)));
            EXPECT_EQ(x * (y + TypeParam("-" HUGE_INTEGER)),
                (x * y) - (x * TypeParam(HUGE_INTEGER)));

            // Other simple arithmetic tests.
            if (y != LONG_MIN) {
                // For y == LONG_MIN, this test requires large integer
                // representation which means we are already testing it above
                // when we loop through this->cases.
                EXPECT_EQ(x * (-y), -(x * y));
                EXPECT_EQ((-x) * (-y), x * y);
                EXPECT_EQ(x * (-y) + x * y, 0);
            }

            // Test that *= behaves as it should.
            {
                TypeParam z = x;
                EXPECT_EQ(z *= y, x * y);
                EXPECT_EQ(z, x * y);
            }

            // Test signs and ordering.
            if (x.sign() > 0 && y > 0) {
                EXPECT_GT(x * y, 0);
                EXPECT_GE(x * y, x);
                EXPECT_GE(x * y, y);
            } else if (x.sign() > 0 && y < 0) {
                EXPECT_LT(x * y, 0);
                EXPECT_LE(x * y, -x);
                EXPECT_LE(x * y, y);
            } else if (x.sign() < 0 && y > 0) {
                EXPECT_LT(x * y, 0);
                EXPECT_LE(x * y, x);
                EXPECT_LE(x * y, -y);
            } else if (x.sign() < 0 && y < 0) {
                EXPECT_GT(x * y, 0);
                EXPECT_GE(x * y, -x);
                EXPECT_GT(x * y, -(y + 1)); // Note: -y could overflow.
            }
        }
    }

    // Ad-hoc tests for native * native:
    verifyProductNativeNative<TypeParam>(3, 7, 21);
    verifyProductNativeNative<TypeParam>(3, -7, -21);
    verifyProductNativeNative<TypeParam>(-3, 7, -21);
    verifyProductNativeNative<TypeParam>(-3, -7, 21);

    // Ad-hoc tests for large * native and native * large:
    verifyProductLargeNative<TypeParam>(ENORMOUS_INTEGER, 100,
        ENORMOUS_INTEGER "00");
    verifyProductLargeNative<TypeParam>(ENORMOUS_INTEGER, -100,
        "-" ENORMOUS_INTEGER "00");
    verifyProductLargeNative<TypeParam>("-" ENORMOUS_INTEGER, -100,
        ENORMOUS_INTEGER "00");
    verifyProductLargeNative<TypeParam>("-" ENORMOUS_INTEGER, 100,
        "-" ENORMOUS_INTEGER "00");

    // Ad-hoc tests for large * large:
    EXPECT_EQ(TypeParam("3" ZEROES) * TypeParam("7" ZEROES),
        TypeParam("21" ZEROES ZEROES));
    EXPECT_EQ(TypeParam("3" ZEROES) * TypeParam("-7" ZEROES),
        TypeParam("-21" ZEROES ZEROES));
    EXPECT_EQ(TypeParam("-3" ZEROES) * TypeParam("7" ZEROES),
        TypeParam("-21" ZEROES ZEROES));
    EXPECT_EQ(TypeParam("-3" ZEROES) * TypeParam("-7" ZEROES),
        TypeParam("21" ZEROES ZEROES));

    // Test around overflow points:
    for (int k : {4, 8, 16, 32, 64, 128, 256}) {
        // The following tests work in base 16.
        SCOPED_TRACE_NUMERIC(k);

        std::string z2k(k, '0');       // The string 00..0, of length k
        std::string z2k_1(k - 1, '0'); // The string 00..0, of length k-1

        verifyProductLargeNative(TypeParam("-1" + z2k, 16), -1,
            TypeParam("1" + z2k, 16));
        verifyProductLargeNative(TypeParam("1" + z2k, 16), -1,
            TypeParam("-1" + z2k, 16));
        verifyProductLargeNative(TypeParam("-8" + z2k_1, 16), -2,
            TypeParam("1" + z2k, 16));
        verifyProductLargeNative(TypeParam("-4" + z2k_1, 16), 4,
            TypeParam("-1" + z2k, 16));
        verifyProductLargeNative(TypeParam("2" + z2k_1, 16), -8,
            TypeParam("-1" + z2k, 16));
        verifyProductLargeNative(TypeParam("1" + z2k_1, 16), 16,
            TypeParam("1" + z2k, 16));

        EXPECT_EQ(TypeParam("1" + z2k, 16) * TypeParam("1" + z2k, 16),
            TypeParam("1" + z2k + z2k, 16));
        EXPECT_EQ(TypeParam("-1" + z2k, 16) * TypeParam("1" + z2k, 16),
            TypeParam("-1" + z2k + z2k, 16));
        EXPECT_EQ(TypeParam("1" + z2k, 16) * TypeParam("-1" + z2k, 16),
            TypeParam("-1" + z2k + z2k, 16));
        EXPECT_EQ(TypeParam("-1" + z2k, 16) * TypeParam("-1" + z2k, 16),
            TypeParam("1" + z2k + z2k, 16));
    }

    TypeParam longMaxInc(LONG_MAX); ++longMaxInc;
    TypeParam longMinDec(LONG_MIN); --longMinDec;

    verifyProductNativeNative<TypeParam>(LONG_MIN, -1, longMaxInc);
    verifyProductLargeNative(longMaxInc, -1, TypeParam(LONG_MIN));

    verifyProductNativeNative<TypeParam>((LONG_MAX-1)/3, 3, LONG_MAX-1);
    verifyProductNativeNative<TypeParam>((LONG_MAX-1)/3, -3, LONG_MIN+2);
    verifyProductNativeNative<TypeParam>(-(LONG_MAX-1)/3, 3, LONG_MIN+2);
    verifyProductNativeNative<TypeParam>(-(LONG_MAX-1)/3, -3, LONG_MAX-1);

    verifyProductNativeNative<TypeParam>((LONG_MAX-1)/3 + 1, 3,
        (unsigned long)(LONG_MAX) + 2);
    verifyProductNativeNative<TypeParam>((LONG_MAX-1)/3 + 1, -3, longMinDec);
    verifyProductNativeNative<TypeParam>(-(LONG_MAX-1)/3 - 1, 3, longMinDec);
    verifyProductNativeNative<TypeParam>(-(LONG_MAX-1)/3 - 1, -3,
        (unsigned long)(LONG_MAX) + 2);

    if constexpr (TypeParam::supportsInfinity) {
        TypeParam inf(TypeParam::infinity);
        EXPECT_EQ(inf * inf, inf);

        for (const auto& x : this->cases) {
            SCOPED_TRACE_REGINA(x);

            EXPECT_EQ(inf * x, inf);
            EXPECT_EQ(x * inf, inf);
        }

        for (long x : this->longCases) {
            SCOPED_TRACE_NUMERIC(x);

            EXPECT_EQ(inf * x, inf);
            EXPECT_EQ(x * inf, inf);
        }
    }
}

TYPED_TEST(IntegerTest, divide) {
    // Rounding should always be towards zero.

    for (const auto& x : this->cases) {
        SCOPED_TRACE_REGINA(x);

        if (x != 0) {
            EXPECT_EQ(TypeParam() / x, 0);
            EXPECT_EQ(x / x, 1);
            EXPECT_EQ(x / -x, -1);
            EXPECT_EQ((x + x) / x, 2);
            EXPECT_EQ((x + x) / -x, -2);

            EXPECT_EQ(TypeParam().divExact(x), 0);
            EXPECT_EQ(x.divExact(x), 1);
            EXPECT_EQ(x.divExact(-x), -1);
            EXPECT_EQ((x + x).divExact(x), 2);
            EXPECT_EQ((x + x).divExact(-x), -2);
        }

        for (const auto& y : this->cases) {
            if (y == 0)
                continue;

            SCOPED_TRACE_REGINA(y);

            TypeParam ans = x / y;

            if (ans == 0) {
                EXPECT_LT(x.abs(), y.abs());
            } else {
                // Check that we got the correct result, by multiplying again
                // and ensuring that the difference is in the correct range.
                TypeParam recover = ans * y;
                EXPECT_NE(recover, 0);
                if (recover > 0) {
                    EXPECT_LE(recover, x);
                    EXPECT_LT(x, recover + y.abs());
                } else {
                    EXPECT_LT(recover - y.abs(), x);
                    EXPECT_LE(x, recover);
                }
            }

            EXPECT_EQ((x * y) / y, x);
            EXPECT_EQ((x * y).divExact(y), x);
            EXPECT_EQ(x / (-y), -ans);
            EXPECT_EQ((-x) / y, -ans);
            EXPECT_EQ((-x) / (-y), ans);

            {
                TypeParam z = x;
                z /= y;
                EXPECT_EQ(z, ans);
            }
            {
                TypeParam z = x * y;
                EXPECT_EQ(z.divByExact(y), x);
                EXPECT_EQ(z, x);
            }
        }

        for (long y : this->longCases) {
            if (y == 0)
                continue;

            SCOPED_TRACE_NUMERIC(y);

            TypeParam ans = x / y;

            // Always work with -|y| instead of |y|, since |LONG_MIN| will
            // overflow a native long.
            if (ans == 0) {
                // |x| < |y|.
                EXPECT_GT(-x.abs(), y >= 0 ? -y : y);
                continue;
            } else {
                // Check that we got the correct result, by multiplying again
                // and ensuring that the difference is in the correct range.
                TypeParam recover = ans * y;
                EXPECT_NE(recover, 0);
                if (recover > 0) {
                    // recover ≤ x < recover + |y|
                    EXPECT_LE(recover, x);
                    EXPECT_LT(x, recover - (y >= 0 ? -y : y));
                } else {
                    // recover - |y| < x ≤ recover
                    EXPECT_LT(recover, x - (y >= 0 ? -y : y));
                    EXPECT_LE(x, recover);
                }
            }

            EXPECT_EQ((x * y) / y, x);
            EXPECT_EQ((x * y).divExact(y), x);
            EXPECT_EQ((-x) / y, -ans);
            if (y == LONG_MIN) {
                // -LONG_MIN will overflow.
                EXPECT_EQ(x / (-TypeParam(y)), -ans);
                EXPECT_EQ((-x) / (-TypeParam(y)), ans);
            } else {
                EXPECT_EQ(x / (-y), -ans);
                EXPECT_EQ((-x) / (-y), ans);
            }

            {
                TypeParam z = x;
                z /= y;
                EXPECT_EQ(z, ans);
            }
            {
                TypeParam z = x * y;
                EXPECT_EQ(z.divByExact(y), x);
                EXPECT_EQ(z, x);
            }
        }
    }

    // Test around overflow points:

    TypeParam zero;
    TypeParam longMax(LONG_MAX);
    TypeParam longMin(LONG_MIN);
    TypeParam longMaxInc(LONG_MAX); ++longMaxInc;
    TypeParam longMinDec(LONG_MIN); --longMinDec;

    EXPECT_EQ(longMax / 1, LONG_MAX);
    EXPECT_EQ(longMax / -1, -LONG_MAX);
    EXPECT_EQ(longMin / 1, LONG_MIN);
    EXPECT_EQ(longMin / -1, longMaxInc);
    EXPECT_EQ(longMax / longMin, 0);
    EXPECT_EQ((-longMax) / longMin, 0);
    EXPECT_EQ(longMin / longMax, -1);
    EXPECT_EQ(longMin / (-longMax), 1);
    EXPECT_EQ(zero / longMax, 0);
    EXPECT_EQ(zero / longMin, 0);
    EXPECT_EQ(longMax / LONG_MIN, 0);
    EXPECT_EQ((-longMax) / LONG_MIN, 0);
    EXPECT_EQ(longMin / LONG_MAX, -1);
    EXPECT_EQ(longMin / (-LONG_MAX), 1);
    EXPECT_EQ(zero / LONG_MAX, 0);
    EXPECT_EQ(zero / LONG_MIN, 0);

    EXPECT_EQ(longMaxInc / 1, longMaxInc);
    EXPECT_EQ(longMaxInc / -1, LONG_MIN);
    EXPECT_EQ(longMinDec / 1, longMinDec);
    EXPECT_EQ(longMinDec / -1, longMaxInc + 1);
    EXPECT_EQ(longMaxInc / longMax, 1);
    EXPECT_EQ(longMaxInc / longMin, -1);
    EXPECT_EQ(longMaxInc / -longMax, -1);
    EXPECT_EQ(longMaxInc / -longMin, 1);
    EXPECT_EQ(longMinDec / longMax, -1);
    EXPECT_EQ(longMinDec / longMin, 1);
    EXPECT_EQ(longMinDec / -longMax, 1);
    EXPECT_EQ(longMinDec / -longMin, -1);
    EXPECT_EQ(longMaxInc / LONG_MAX, 1);
    EXPECT_EQ(longMaxInc / LONG_MIN, -1);
    EXPECT_EQ(longMaxInc / -LONG_MAX, -1);
    EXPECT_EQ(longMinDec / LONG_MAX, -1);
    EXPECT_EQ(longMinDec / LONG_MIN, 1);
    EXPECT_EQ(longMinDec / -LONG_MAX, 1);

    EXPECT_EQ(longMax.divExact(1), LONG_MAX);
    EXPECT_EQ(longMax.divExact(-1), -LONG_MAX);
    EXPECT_EQ(longMin.divExact(1), LONG_MIN);
    EXPECT_EQ(longMin.divExact(-1), longMaxInc);
    EXPECT_EQ(zero.divExact(longMax), 0);
    EXPECT_EQ(zero.divExact(longMin), 0);
    EXPECT_EQ(zero.divExact(LONG_MAX), 0);
    EXPECT_EQ(zero.divExact(LONG_MIN), 0);

    EXPECT_EQ(longMaxInc.divExact(1), longMaxInc);
    EXPECT_EQ(longMaxInc.divExact(-1), LONG_MIN);
    EXPECT_EQ(longMinDec.divExact(1), longMinDec);
    EXPECT_EQ(longMinDec.divExact(-1), longMaxInc + 1);
    EXPECT_EQ(longMaxInc.divExact(longMin), -1);
    EXPECT_EQ(longMaxInc.divExact(-longMin), 1);
    EXPECT_EQ(longMaxInc.divExact(LONG_MIN), -1);

    if constexpr (TypeParam::supportsInfinity) {
        TypeParam inf(TypeParam::infinity);
        EXPECT_EQ(inf / inf, inf);

        for (const auto& x : this->cases) {
            SCOPED_TRACE_REGINA(x);

            EXPECT_EQ(inf / x, inf);
            EXPECT_EQ(x / inf, 0);
            EXPECT_EQ(x / TypeParam(), inf);
            EXPECT_EQ(x / 0L, inf);
        }

        for (long x : this->longCases) {
            SCOPED_TRACE_NUMERIC(x);

            EXPECT_EQ(inf / x, inf);
        }
    }
}

TYPED_TEST(IntegerTest, mod) {
    // Infinity is not supported.
    // x % 0 is not supported.
    // If result is non-zero then sign(x % y) = sign(x).

    for (const auto& x : this->cases) {
        SCOPED_TRACE_REGINA(x);

        EXPECT_EQ(x % 1, 0);
        if (x != 0) {
            EXPECT_EQ(TypeParam() % x, 0);
            EXPECT_EQ(TypeParam() % x, 0);
            EXPECT_EQ(x % x, 0);
            EXPECT_EQ((-x) % x, 0);
            EXPECT_EQ((x + x) % x, 0);
            EXPECT_EQ((-(x + x)) % x, 0);
        }

        for (const auto& y : this->cases) {
            if (y == 0)
                continue;

            SCOPED_TRACE_REGINA(y);

            TypeParam ans = x % y;

            // Ensure that ans is within range.
            if (ans < 0) {
                EXPECT_LT(x, 0);
                EXPECT_GT(ans, -y.abs());
            } else if (ans > 0) {
                EXPECT_GT(x, 0);
                EXPECT_LT(ans, y.abs());
            }

            // Ensure that y | (x - ans).
            TypeParam q = (x - ans) / y;
            EXPECT_EQ(q * y + ans, x);

            TypeParam z = x;
            EXPECT_EQ(z %= y, ans);
            EXPECT_EQ(z, ans);

            // Verify divExact() if we can.
            if (ans == 0)
                EXPECT_EQ(x.divExact(y) * y, x);
        }

        for (long y : this->longCases) {
            if (y == 0)
                continue;

            SCOPED_TRACE_NUMERIC(y);

            TypeParam ans = x % y;

            // Ensure that ans is within range.
            if (ans < 0) {
                EXPECT_LT(x, 0);
                EXPECT_GT(ans, y < 0 ? y : -y);
            } else if (ans > 0) {
                EXPECT_GT(x, 0);
                // Careful here: -LONG_MIN will overflow.
                // Work in the negatives instead.
                EXPECT_GT(-ans, y < 0 ? y : -y);
            }

            // Ensure that y | (x - ans).
            TypeParam q = (x - ans) / y;
            EXPECT_EQ(q * y + ans, x);

            TypeParam z = x;
            EXPECT_EQ(z %= y, ans);
            EXPECT_EQ(z, ans);

            // Verify divExact() if we can.
            if (ans == 0)
                EXPECT_EQ(x.divExact(y) * y, x);
        }
    }

    // Test around overflow points:

    TypeParam zero;
    TypeParam longMax(LONG_MAX);
    TypeParam longMin(LONG_MIN);
    TypeParam longMaxInc(LONG_MAX); ++longMaxInc;
    TypeParam longMinDec(LONG_MIN); --longMinDec;

    EXPECT_EQ(longMax % 1, 0);
    EXPECT_EQ(longMax % -1, 0);
    EXPECT_EQ(longMin % 1, 0);
    EXPECT_EQ(longMin % -1, 0);
    EXPECT_EQ(longMax % longMin, longMax);
    EXPECT_EQ((-longMax) % longMin, -longMax);
    EXPECT_EQ(longMin % longMax, -1);
    EXPECT_EQ(longMin % (-longMax), -1);
    EXPECT_EQ(zero % longMax, 0);
    EXPECT_EQ(zero % longMin, 0);
    EXPECT_EQ(longMax % LONG_MIN, LONG_MAX);
    EXPECT_EQ((-longMax) % LONG_MIN, -LONG_MAX);
    EXPECT_EQ(longMin % LONG_MAX, -1);
    EXPECT_EQ(longMin % (-LONG_MAX), -1);
    EXPECT_EQ(zero % LONG_MAX, 0);
    EXPECT_EQ(zero % LONG_MIN, 0);

    EXPECT_EQ(longMaxInc % 1, 0);
    EXPECT_EQ(longMaxInc % -1, 0);
    EXPECT_EQ(longMinDec % 1, 0);
    EXPECT_EQ(longMinDec % -1, 0);
    EXPECT_EQ(longMaxInc % longMax, 1);
    EXPECT_EQ(longMaxInc % longMin, 0);
    EXPECT_EQ(longMaxInc % -longMax, 1);
    EXPECT_EQ(longMaxInc % -longMin, 0);
    EXPECT_EQ(longMinDec % longMax, -2);
    EXPECT_EQ(longMinDec % longMin, -1);
    EXPECT_EQ(longMinDec % -longMax, -2);
    EXPECT_EQ(longMinDec % -longMin, -1);
    EXPECT_EQ(longMaxInc % LONG_MAX, 1);
    EXPECT_EQ(longMaxInc % LONG_MIN, 0);
    EXPECT_EQ(longMaxInc % -LONG_MAX, 1);
    EXPECT_EQ(longMinDec % LONG_MAX, -2);
    EXPECT_EQ(longMinDec % LONG_MIN, -1);
    EXPECT_EQ(longMinDec % -LONG_MAX, -2);
}

TYPED_TEST(IntegerTest, negate) {
    for (const auto& x : this->cases) {
        SCOPED_TRACE_REGINA(x);

        EXPECT_EQ(x + (-x), 0);
        EXPECT_EQ((-x) + x, 0);
        EXPECT_EQ(-(-x), x);
        EXPECT_EQ(-x, x * (-1));

        // Verify the results using string representations.
        if (x.sign() == 0)
            EXPECT_EQ((-x).stringValue(), "0");
        else if (x.sign() > 0)
            EXPECT_EQ((-x).stringValue(), "-" + x.stringValue());
        else
            EXPECT_EQ("-" + (-x).stringValue(), x.stringValue());

        TypeParam z(x);
        z.negate();
        EXPECT_EQ(z, -x);
    }

    if constexpr (TypeParam::supportsInfinity) {
        TypeParam i(TypeParam::infinity);
        i.negate();
        EXPECT_EQ(i, TypeParam::infinity);
    }
}

TYPED_TEST(IntegerTest, abs) {
    for (const auto& x : this->cases) {
        SCOPED_TRACE_REGINA(x);

        std::string str = x.stringValue();
        ASSERT_FALSE(str.empty());

        TypeParam result = TypeParam(x).abs();

        // Verify the results using string representations.
        if (x.sign() == 0) {
            EXPECT_EQ(x, 0);
            EXPECT_EQ(result, 0);

            EXPECT_EQ(str, "0");
            EXPECT_EQ(result.str(), "0");
        } else if (x.sign() > 0) {
            EXPECT_EQ(x.sign(), 1);
            EXPECT_EQ(result, x);

            EXPECT_NE(str[0], '-');
            EXPECT_EQ(result.stringValue(), str);
        } else {
            EXPECT_EQ(x.sign(), -1);
            EXPECT_EQ(result, -x);

            std::string orig = x.stringValue();
            std::string abs = result.stringValue();

            EXPECT_EQ(str[0], '-');
            EXPECT_EQ("-" + result.stringValue(), str);
        }
    }

    if constexpr (TypeParam::supportsInfinity)
        EXPECT_EQ(TypeParam(TypeParam::infinity).abs(), TypeParam::infinity);
}

TYPED_TEST(IntegerTest, divisionAlg) {
    // The list of cases includes cases at the overflow points.
    // Just run through all (n, d) pairs.
    for (const auto& n : this->cases) {
        SCOPED_TRACE_REGINA(n);
        for (const auto& divisor : this->cases) {
            SCOPED_TRACE_REGINA(divisor);

            auto [q, r] = TypeParam(n).divisionAlg(divisor);
            EXPECT_EQ(q * divisor + r, n);
            if (divisor == 0) {
                EXPECT_EQ(q, 0);
                EXPECT_EQ(r, n);
            } else {
                EXPECT_GE(r, 0);
                EXPECT_LT(r, divisor.abs());
            }
        }
    }
}

TYPED_TEST(IntegerTest, gcdLcm) {
    // The list of cases includes cases at the overflow points.
    // Just run through all pairs.
    for (const auto& x : this->cases) {
        SCOPED_TRACE_REGINA(x);
        for (const auto& y : this->cases) {
            SCOPED_TRACE_REGINA(y);

            TypeParam u, v;
            TypeParam g = x.gcdWithCoeffs(y, u, v);
            EXPECT_EQ(g, x.gcd(y));
            EXPECT_EQ(g, y.gcd(x));

            if (x == 0 && y == 0) {
                EXPECT_EQ(g, 0);
                EXPECT_EQ(u, 0);
                EXPECT_EQ(v, 0);
            } else if (x == 0 && y > 0) {
                EXPECT_EQ(g, y);
                EXPECT_EQ(u, 0);
                EXPECT_EQ(v, 1);
            } else if (x == 0 && y < 0) {
                EXPECT_EQ(g, -y);
                EXPECT_EQ(u, 0);
                EXPECT_EQ(v, -1);
            } else if (y == 0 && x > 0) {
                EXPECT_EQ(g, x);
                EXPECT_EQ(u, 1);
                EXPECT_EQ(v, 0);
            } else if (y == 0 && x < 0) {
                EXPECT_EQ(g, -x);
                EXPECT_EQ(u, -1);
                EXPECT_EQ(v, 0);
            } else {
                // The following tests are enough to ensure that we have the
                // right gcd.
                EXPECT_GT(g, 0);
                EXPECT_EQ(x % g, 0);
                EXPECT_EQ(y % g, 0);
                EXPECT_EQ((x / g) * g, x);
                EXPECT_EQ((y / g) * g, y);
                EXPECT_EQ(u * x + v * y, g);

                // These next tests ensure that u and v are correct.
                EXPECT_LT(-x.abs(), g * v * y.sign());
                EXPECT_LT(v * y.sign(), 1);
            }

            // Make sure the LCM is correct.
            // Note that we make no guarantees about the sign of the LCM.
            TypeParam l = x.lcm(y);
            EXPECT_EQ(l, y.lcm(x));
            EXPECT_EQ((g * l).abs(), (x * y).abs());

            // Verify that in-place gcd/lcm operations behave correctly also.
            TypeParam p(x);
            p.gcdWith(y);
            EXPECT_EQ(p, g);

            TypeParam q(x);
            q.lcmWith(y);
            EXPECT_EQ(q, l);
        }
    }

#ifdef INT128_AVAILABLE
    // Long ago I ran into an issue on arm64 with 128-bit integers, where
    // trivial shifts such as (3 << 0) were giving incorrect answers.
    // See commit 8b5c562ad3595e3db8213c4151888c9de26fd57c for the patch.
    // A side-effect was that Regina computed gcd(3,3) == 55340232221128654851
    // on this platform.  Verify that we are _not_ seeing these issues now.

    regina::IntOfSize<16>::type n = 3;
    int shift = 0;
    n <<= shift;
    EXPECT_EQ(n, 3);

    regina::NativeInteger<16> a, b;
    a = b = 3;
    a.gcdWith(b);
    EXPECT_EQ(a, 3);
#endif
}

TYPED_TEST(IntegerTest, raiseToPower) {
    for (const auto& x : this->cases) {
        SCOPED_TRACE_REGINA(x);

        TypeParam ans = 1;
        for (int exp = 0; exp < 64; ++exp) {
            SCOPED_TRACE_NUMERIC(exp);

            TypeParam pow = x;
            pow.raiseToPower(exp);
            EXPECT_EQ(ans, pow);
            ans *= x;
        }
    }

    if constexpr (TypeParam::supportsInfinity) {
        for (int exp = 0; exp < 5; ++exp) {
            SCOPED_TRACE_NUMERIC(exp);

            TypeParam pow = TypeParam::infinity;
            pow.raiseToPower(exp);
            if (exp == 0)
                EXPECT_EQ(pow, 1);
            else
                EXPECT_EQ(pow, TypeParam::infinity);
        }
    }
}

TYPED_TEST(IntegerTest, tryReduce) {
    // Note: there used to be a corresponding makeLarge test, but we have
    // removed this since the comparisons test already verifies everything
    // that the old makeLarge test used to check.

    // For this test we want extensive equality testing, and so we borrow
    // verifyEqual() from the comparisons test.
    for (const auto& x : this->cases) {
        SCOPED_TRACE_REGINA(x);
        bool supportsNative = (x <= LONG_MAX && x >= LONG_MIN);

        {
            TypeParam y = x;
            y.tryReduce();
            EXPECT_EQ(y.isNative(), supportsNative);
            verifyEqual(x, y);
            if (supportsNative)
                EXPECT_EQ(x, y.template safeValue<long>());
            else
                EXPECT_THROW({ y.template safeValue<long>(); },
                    regina::NoSolution);
        }
        {
            TypeParam y = x;
            y += TypeParam(ENORMOUS_INTEGER);
            y += TypeParam(HUGE_INTEGER);
            y -= TypeParam(ENORMOUS_INTEGER);
            y -= TypeParam(HUGE_INTEGER);
            y.tryReduce();
            EXPECT_EQ(y.isNative(), supportsNative);
            verifyEqual(x, y);
            if (supportsNative)
                EXPECT_EQ(x, y.template safeValue<long>());
            else
                EXPECT_THROW({ y.template safeValue<long>(); },
                    regina::NoSolution);
        }
        {
            TypeParam y = x;
            y.makeLarge();
            EXPECT_FALSE(y.isNative());
            y.tryReduce();
            EXPECT_EQ(y.isNative(), supportsNative);
            verifyEqual(x, y);
            if (supportsNative)
                EXPECT_EQ(x, y.template safeValue<long>());
            else
                EXPECT_THROW({ y.template safeValue<long>(); },
                    regina::NoSolution);
        }
    }
}

TYPED_TEST(IntegerTest, nativeVsLarge) {
    // For integers that can fit within a native long, compare their
    // mathematical behaviour under native vs large storage methods.

    // Unary operations:
    for (auto a : this->cases) {
        SCOPED_TRACE_REGINA(a);
        a.tryReduce();
        if (! a.isNative())
            continue;

        for (int op = 0; op < 2; ++op) {
            TypeParam x(a);

            TypeParam x2(a);
            x2.makeLarge();

            EXPECT_TRUE(x.isNative());
            EXPECT_FALSE(x2.isNative());

            switch (op) {
                case 0:
                    EXPECT_EQ(-x, -x2); break;
                case 1:
                    EXPECT_EQ(x.abs(), x2.abs()); break;
            }
        }
    }

    // Binary operations:
    for (auto a : this->cases) {
        SCOPED_TRACE_REGINA(a);
        a.tryReduce();

        for (auto b : this->cases) {
            SCOPED_TRACE_REGINA(b);
            b.tryReduce();

            // aRep and bRep are (0, 1) for (large, native) storage methods.
            for (int aRep = 0; aRep < (a.isNative() ? 2 : 1); ++aRep) {
                for (int bRep = 0; bRep < (b.isNative() ? 2 : 1); ++bRep) {
                    if (aRep == 0 && bRep == 0)
                        continue;

                    for (int op = 0; op < (b == 0 ? 12 : 15); ++op) {
                        TypeParam x(a);
                        TypeParam y(b);

                        TypeParam x2(a);
                        if (aRep == 0)
                            x2.makeLarge();
                        TypeParam y2(b);
                        if (bRep == 0)
                            y2.makeLarge();

                        EXPECT_EQ(x2.isNative(), aRep == 1);
                        EXPECT_EQ(y2.isNative(), bRep == 1);

                        switch (op) {
                            case 0:
                                EXPECT_EQ(x + y, x2 + y2); break;
                            case 1:
                                EXPECT_EQ(x - y, x2 - y2); break;
                            case 2:
                                EXPECT_EQ(x * y, x2 * y2); break;
                            case 3:
                                EXPECT_EQ(x.gcd(y), x2.gcd(y2)); break;
                            case 4:
                                {
                                    TypeParam g, u, v, g2, u2, v2;
                                    g = x.gcdWithCoeffs(y, u, v);
                                    g2 = x2.gcdWithCoeffs(y2, u2, v2);
                                    EXPECT_EQ(g, g2);
                                    EXPECT_EQ(u, u2);
                                    EXPECT_EQ(v, v2);
                                }
                                break;
                            case 5:
                                EXPECT_EQ(x.lcm(y), x2.lcm(y2)); break;
                            case 6:
                                EXPECT_EQ(x < y, x2 < y2); break;
                            case 7:
                                EXPECT_EQ(x <= y, x2 <= y2); break;
                            case 8:
                                EXPECT_EQ(x > y, x2 > y2); break;
                            case 9:
                                EXPECT_EQ(x >= y, x2 >= y2); break;
                            case 10:
                                EXPECT_EQ(x == y, x2 == y2); break;
                            case 11:
                                EXPECT_EQ(x != y, x2 != y2); break;

                            // Operations that require RHS != 0:
                            case 12:
                                EXPECT_EQ(x / y, x2 / y2); break;
                            case 13:
                                EXPECT_EQ(x % y, x2 % y2); break;
                            case 14:
                                {
                                    auto [q, r] = x.divisionAlg(y);
                                    auto [q2, r2] = x2.divisionAlg(y2);
                                    EXPECT_EQ(q, q2);
                                    EXPECT_EQ(r, r2);
                                }
                                break;
                        }
                    }
                }
            }
        }
    }
}

template <ArbitraryPrecisionInteger IntegerType, CppInteger Native>
static void verifyCppInteger(Native native) {
    // Test construction, assignment and various conversions to/from the given
    // native C++ integer.

    using ReginaNative = regina::NativeInteger<sizeof(Native)>;

    // We cannot use SCOPED_TRACE_NUMERIC, since this does not support 128-bit
    // integers.  Use SCOPED_TRACE_STDSTRING once we have the string later on.

    std::string str;
    if constexpr (sizeof(Native) < 16) {
        str = std::to_string(native);
    } else {
        // At present, 128-bit integers do not support std::to_string or
        // std::ostream::operator >> on some platforms.  We need another way
        // to extract a string.
        if (native == 0) {
            str = "0";
        } else if (native > 0) {
            for (Native x = native ; x != 0; x /= 10)
                str += char('0' + (x % 10));
            std::reverse(str.begin(), str.end());
        } else {
            for (Native x = native ; x != 0; x /= 10) {
                Native digit = x % 10;
                if (digit > 0)
                    digit -= 10;
                str += char('0' - digit);
            }
            str += '-';
            std::reverse(str.begin(), str.end());
        }
    }

    SCOPED_TRACE_STDSTRING(str);

    // Construction from Native:
    IntegerType large(native);
    EXPECT_EQ(large.str(), str);
    EXPECT_EQ(large, native);
    {
        Native extracted;
        EXPECT_NO_THROW({ extracted = large.template safeValue<Native>(); });
        EXPECT_EQ(extracted, native);
    }

    // Assignment from Native:
    {
        IntegerType assigned = 1;
        EXPECT_EQ(assigned.str(), "1");
        assigned = native;
        EXPECT_EQ(assigned.str(), str);
        EXPECT_EQ(assigned, native);
        EXPECT_EQ(assigned, large);

        Native extracted;
        EXPECT_NO_THROW({ extracted = assigned.template safeValue<Native>(); });
        EXPECT_EQ(extracted, native);
    }
    {
        IntegerType assigned = HUGE_INTEGER;
        EXPECT_EQ(assigned.str(), HUGE_INTEGER);
        assigned = native;
        EXPECT_EQ(assigned.str(), str);
        EXPECT_EQ(assigned, native);
        EXPECT_EQ(assigned, large);

        Native extracted;
        EXPECT_NO_THROW({ extracted = assigned.template safeValue<Native>(); });
        EXPECT_EQ(extracted, native);
    }

    // Construction and assignment from regina's NativeInteger class:
    if constexpr (regina::is_signed_cpp_integer_v<Native>) {
        ReginaNative n(native);
        EXPECT_EQ(n.str(), str);
        EXPECT_EQ(n, native);

        {
            IntegerType a(n);
            EXPECT_EQ(a.str(), str);
            EXPECT_EQ(a, native);
            EXPECT_EQ(a, n);

            Native extracted;
            EXPECT_NO_THROW({ extracted = a.template safeValue<Native>(); });
            EXPECT_EQ(extracted, native);
        }
        {
            IntegerType b = 7;
            EXPECT_EQ(b.str(), "7");
            b = n;
            EXPECT_EQ(b.str(), str);
            EXPECT_EQ(b, native);
            EXPECT_EQ(b, n);

            Native extracted;
            EXPECT_NO_THROW({ extracted = b.template safeValue<Native>(); });
            EXPECT_EQ(extracted, native);
        }
    }

    // Comparisons:
    {
        static constexpr Native minNative = std::numeric_limits<Native>::min();
        static constexpr Native maxNative = std::numeric_limits<Native>::max();

        EXPECT_EQ(large, native);
        EXPECT_NE(large, static_cast<Native>(native + 1));
        if (native != maxNative) {
            EXPECT_LT(large, static_cast<Native>(native + 1));
            EXPECT_GT(static_cast<Native>(native + 1), large);
        }
        EXPECT_NE(large, static_cast<Native>(native - 1));
        if (native != minNative) {
            EXPECT_GT(large, static_cast<Native>(native - 1));
            EXPECT_LT(static_cast<Native>(native - 1), large);
        }
        if (native > 0) {
            EXPECT_NE(large, static_cast<Native>(native / 2));
            EXPECT_GT(large, static_cast<Native>(native / 2));
        } else if (native < 0) {
            EXPECT_NE(large, static_cast<Native>(native / 2));
            EXPECT_LT(large, static_cast<Native>(native / 2));
        }

        EXPECT_NE(large, static_cast<Native>(native ^ Native(1)));
        if (native % 2 == 0)
            EXPECT_LT(large, static_cast<Native>(native ^ Native(1)));
        else
            EXPECT_GT(large, static_cast<Native>(native ^ Native(1)));

        static constexpr Native firstBit =
            static_cast<Native>(Native(1) << (sizeof(Native) * 8 - 1));
        if constexpr (regina::is_signed_cpp_integer_v<Native>) {
            EXPECT_NE(large, static_cast<Native>(native ^ firstBit));
            if (native >= 0)
                EXPECT_GT(large, static_cast<Native>(native ^ firstBit));
            else
                EXPECT_LT(large, static_cast<Native>(native ^ firstBit));

            if (native != 0 && native != firstBit) {
                EXPECT_NE(large, static_cast<Native>(-native));
                if (native > 0)
                    EXPECT_GT(large, static_cast<Native>(-native));
                else
                    EXPECT_LT(large, static_cast<Native>(-native));
            }
        } else {
            EXPECT_NE(large, static_cast<Native>(native ^ firstBit));
            if (native & firstBit)
                EXPECT_GT(large, static_cast<Native>(native ^ firstBit));
            else
                EXPECT_LT(large, static_cast<Native>(native ^ firstBit));
        }

        // Do this all again, but with regina::NativeInteger (which is always
        // a signed type).
        if constexpr (regina::is_signed_cpp_integer_v<Native>) {
            EXPECT_EQ(large, ReginaNative(native));
            EXPECT_NE(large, ReginaNative(native + 1));
            if (native != maxNative) {
                EXPECT_LT(large, ReginaNative(native + 1));
                EXPECT_GT(ReginaNative(native + 1), large);
            }
            EXPECT_NE(large, ReginaNative(native - 1));
            if (native != minNative) {
                EXPECT_GT(large, ReginaNative(native - 1));
                EXPECT_LT(ReginaNative(native - 1), large);
            }
            if (native > 0) {
                EXPECT_NE(large, ReginaNative(native / 2));
                EXPECT_GT(large, ReginaNative(native / 2));
            } else if (native < 0) {
                EXPECT_NE(large, ReginaNative(native / 2));
                EXPECT_LT(large, ReginaNative(native / 2));
            }

            EXPECT_NE(large, ReginaNative(native ^ Native(1)));
            if (native % 2 == 0)
                EXPECT_LT(large, ReginaNative(native ^ Native(1)));
            else
                EXPECT_GT(large, ReginaNative(native ^ Native(1)));

            EXPECT_NE(large, ReginaNative(native ^ firstBit));
            if (native >= 0)
                EXPECT_GT(large, ReginaNative(native ^ firstBit));
            else
                EXPECT_LT(large, ReginaNative(native ^ firstBit));

            if (native != 0 && native != firstBit) {
                EXPECT_NE(large, ReginaNative(-native));
                if (native > 0)
                    EXPECT_GT(large, ReginaNative(-native));
                else
                    EXPECT_LT(large, ReginaNative(-native));
            }
        }
    }
}

template <ArbitraryPrecisionInteger IntegerType, UnsignedCppInteger Native>
static void verifyCppIntegerType() {
    SCOPED_TRACE_TYPE(Native);

    static constexpr Native maxNative = std::numeric_limits<Native>::max();

    verifyCppInteger<IntegerType, Native>(0);
    verifyCppInteger<IntegerType, Native>(1);
    verifyCppInteger<IntegerType, Native>(2);
    verifyCppInteger<IntegerType, Native>((maxNative / 2) - 1);
    verifyCppInteger<IntegerType, Native>(maxNative / 2);
    verifyCppInteger<IntegerType, Native>((maxNative / 2) + 1);
    verifyCppInteger<IntegerType, Native>(maxNative - 2);
    verifyCppInteger<IntegerType, Native>(maxNative - 1);
    verifyCppInteger<IntegerType, Native>(maxNative);

    if constexpr (sizeof(Native) > sizeof(int)) {
        Native sMax = INT_MAX;
        Native uMax = UINT_MAX;
        verifyCppInteger<IntegerType, Native>(sMax - 1);
        verifyCppInteger<IntegerType, Native>(sMax);
        verifyCppInteger<IntegerType, Native>(sMax + 1);
        verifyCppInteger<IntegerType, Native>(uMax - 1);
        verifyCppInteger<IntegerType, Native>(uMax);
        verifyCppInteger<IntegerType, Native>(uMax + 1);
    }
    if constexpr (sizeof(Native) > sizeof(long)) {
        Native sMax = LONG_MAX;
        Native uMax = ULONG_MAX;
        verifyCppInteger<IntegerType, Native>(sMax - 1);
        verifyCppInteger<IntegerType, Native>(sMax);
        verifyCppInteger<IntegerType, Native>(sMax + 1);
        verifyCppInteger<IntegerType, Native>(uMax - 1);
        verifyCppInteger<IntegerType, Native>(uMax);
        verifyCppInteger<IntegerType, Native>(uMax + 1);
    }
    if constexpr (sizeof(Native) > sizeof(long long)) {
        Native sMax = LLONG_MAX;
        Native uMax = ULLONG_MAX;
        verifyCppInteger<IntegerType, Native>(sMax - 1);
        verifyCppInteger<IntegerType, Native>(sMax);
        verifyCppInteger<IntegerType, Native>(sMax + 1);
        verifyCppInteger<IntegerType, Native>(uMax - 1);
        verifyCppInteger<IntegerType, Native>(uMax);
        verifyCppInteger<IntegerType, Native>(uMax + 1);
    }
}

template <ArbitraryPrecisionInteger IntegerType, SignedCppInteger Native>
static void verifyCppIntegerType() {
    SCOPED_TRACE_TYPE(Native);

    static constexpr Native minNative = std::numeric_limits<Native>::min();
    static constexpr Native maxNative = std::numeric_limits<Native>::max();

    verifyCppInteger<IntegerType, Native>(minNative);
    verifyCppInteger<IntegerType, Native>(minNative + 1);
    verifyCppInteger<IntegerType, Native>(minNative + 2);
    verifyCppInteger<IntegerType, Native>((minNative / 2) - 1);
    verifyCppInteger<IntegerType, Native>(minNative / 2);
    verifyCppInteger<IntegerType, Native>((minNative / 2) + 1);
    verifyCppInteger<IntegerType, Native>(-2);
    verifyCppInteger<IntegerType, Native>(-1);
    verifyCppInteger<IntegerType, Native>(0);
    verifyCppInteger<IntegerType, Native>(1);
    verifyCppInteger<IntegerType, Native>(2);
    verifyCppInteger<IntegerType, Native>((maxNative / 2) - 1);
    verifyCppInteger<IntegerType, Native>(maxNative / 2);
    verifyCppInteger<IntegerType, Native>((maxNative / 2) + 1);
    verifyCppInteger<IntegerType, Native>(maxNative - 2);
    verifyCppInteger<IntegerType, Native>(maxNative - 1);
    verifyCppInteger<IntegerType, Native>(maxNative);

    if constexpr (sizeof(Native) > sizeof(int)) {
        Native sMin = INT_MIN;
        Native sMax = INT_MAX;
        Native uMax = UINT_MAX;
        verifyCppInteger<IntegerType, Native>(-uMax - 1);
        verifyCppInteger<IntegerType, Native>(-uMax);
        verifyCppInteger<IntegerType, Native>(-uMax + 1);
        verifyCppInteger<IntegerType, Native>(sMin - 1);
        verifyCppInteger<IntegerType, Native>(sMin);
        verifyCppInteger<IntegerType, Native>(sMin + 1);
        verifyCppInteger<IntegerType, Native>(sMax - 1);
        verifyCppInteger<IntegerType, Native>(sMax);
        verifyCppInteger<IntegerType, Native>(sMax + 1);
        verifyCppInteger<IntegerType, Native>(uMax - 1);
        verifyCppInteger<IntegerType, Native>(uMax);
        verifyCppInteger<IntegerType, Native>(uMax + 1);
    }
    if constexpr (sizeof(Native) > sizeof(long)) {
        Native sMin = LONG_MIN;
        Native sMax = LONG_MAX;
        Native uMax = ULONG_MAX;
        verifyCppInteger<IntegerType, Native>(-uMax - 1);
        verifyCppInteger<IntegerType, Native>(-uMax);
        verifyCppInteger<IntegerType, Native>(-uMax + 1);
        verifyCppInteger<IntegerType, Native>(sMin - 1);
        verifyCppInteger<IntegerType, Native>(sMin);
        verifyCppInteger<IntegerType, Native>(sMin + 1);
        verifyCppInteger<IntegerType, Native>(sMax - 1);
        verifyCppInteger<IntegerType, Native>(sMax);
        verifyCppInteger<IntegerType, Native>(sMax + 1);
        verifyCppInteger<IntegerType, Native>(uMax - 1);
        verifyCppInteger<IntegerType, Native>(uMax);
        verifyCppInteger<IntegerType, Native>(uMax + 1);
    }
    if constexpr (sizeof(Native) > sizeof(long long)) {
        Native sMin = LLONG_MIN;
        Native sMax = LLONG_MAX;
        Native uMax = ULLONG_MAX;
        verifyCppInteger<IntegerType, Native>(-uMax - 1);
        verifyCppInteger<IntegerType, Native>(-uMax);
        verifyCppInteger<IntegerType, Native>(-uMax + 1);
        verifyCppInteger<IntegerType, Native>(sMin - 1);
        verifyCppInteger<IntegerType, Native>(sMin);
        verifyCppInteger<IntegerType, Native>(sMin + 1);
        verifyCppInteger<IntegerType, Native>(sMax - 1);
        verifyCppInteger<IntegerType, Native>(sMax);
        verifyCppInteger<IntegerType, Native>(sMax + 1);
        verifyCppInteger<IntegerType, Native>(uMax - 1);
        verifyCppInteger<IntegerType, Native>(uMax);
        verifyCppInteger<IntegerType, Native>(uMax + 1);
    }
}

TYPED_TEST(IntegerTest, cppIntegerTypes) {
    verifyCppIntegerType<TypeParam, unsigned char>();
    verifyCppIntegerType<TypeParam, unsigned short>();
    verifyCppIntegerType<TypeParam, unsigned>();
    verifyCppIntegerType<TypeParam, unsigned long>();
    verifyCppIntegerType<TypeParam, unsigned long long>();
    verifyCppIntegerType<TypeParam, size_t>();

    verifyCppIntegerType<TypeParam, signed char>();
    verifyCppIntegerType<TypeParam, short>();
    verifyCppIntegerType<TypeParam, int>();
    verifyCppIntegerType<TypeParam, long>();
    verifyCppIntegerType<TypeParam, long long>();
    verifyCppIntegerType<TypeParam, ssize_t>();

#ifdef INT128_AVAILABLE
    verifyCppIntegerType<TypeParam, regina::IntOfSize<16>::utype>();
    verifyCppIntegerType<TypeParam, regina::IntOfSize<16>::type>();
#endif
}

template <ArbitraryPrecisionInteger IntegerType, CppInteger Native>
static void verifySafeValueSuccess(IntegerType value, Native expected) {
    SCOPED_TRACE_REGINA(value);

    {
        Native computed;
        EXPECT_NO_THROW({ computed = value.template safeValue<Native>(); });
        EXPECT_EQ(computed, expected);
        EXPECT_EQ(value.template unsafeValue<Native>(), expected);
    }

    if (value.isNative()) {
        value.makeLarge();
        EXPECT_FALSE(value.isNative());
    } else {
        value.tryReduce(); // might or might not be possible
    }

    {
        Native computed;
        EXPECT_NO_THROW({ computed = value.template safeValue<Native>(); });
        EXPECT_EQ(computed, expected);
        EXPECT_EQ(value.template unsafeValue<Native>(), expected);
    }
}

template <ArbitraryPrecisionInteger IntegerType, CppInteger Native>
static void verifySafeValueFailure(IntegerType value) {
    SCOPED_TRACE_REGINA(value);

    EXPECT_THROW({ value.template safeValue<Native>(); }, regina::NoSolution);

    if (value.isNative()) {
        value.makeLarge();
        EXPECT_FALSE(value.isNative());
    } else {
        value.tryReduce(); // might or might not be possible
    }

    EXPECT_THROW({ value.template safeValue<Native>(); }, regina::NoSolution);
}

template <ArbitraryPrecisionInteger IntegerType, UnsignedCppInteger Native>
static void verifySafeValue() {
    SCOPED_TRACE_TYPE(Native);

    static constexpr Native maxNative = std::numeric_limits<Native>::max();

    // Compute the same value directly in Regina's large integer type.
    // It should be (11...1) in binary.
    IntegerType maxRegina;
    for (int i = 0; i < 8 * sizeof(Native); ++i) {
        maxRegina *= 2;
        ++maxRegina;
    }
    EXPECT_EQ(maxRegina, maxNative);

    verifySafeValueFailure<IntegerType, Native>(-maxRegina - 1);
    verifySafeValueFailure<IntegerType, Native>(-maxRegina);
    verifySafeValueFailure<IntegerType, Native>(-maxRegina + 1);
    verifySafeValueFailure<IntegerType, Native>(-(maxRegina / 2) - 1);
    verifySafeValueFailure<IntegerType, Native>(-(maxRegina / 2));
    verifySafeValueFailure<IntegerType, Native>(-(maxRegina / 2) + 1);
    verifySafeValueFailure<IntegerType, Native>(-1);
    verifySafeValueSuccess<IntegerType, Native>(0, 0);
    verifySafeValueSuccess<IntegerType, Native>(1, 1);
    verifySafeValueSuccess<IntegerType, Native>((maxRegina / 2) - 1,
        (maxNative / 2) - 1);
    verifySafeValueSuccess<IntegerType, Native>(maxRegina / 2, maxNative / 2);
    verifySafeValueSuccess<IntegerType, Native>((maxRegina / 2) + 1,
        (maxNative / 2) + 1);
    verifySafeValueSuccess<IntegerType, Native>(maxRegina - 2, maxNative - 2);
    verifySafeValueSuccess<IntegerType, Native>(maxRegina - 1, maxNative - 1);
    verifySafeValueSuccess<IntegerType, Native>(maxRegina, maxNative);
    verifySafeValueFailure<IntegerType, Native>(maxRegina + 1);
    verifySafeValueFailure<IntegerType, Native>(maxRegina + 2);
    verifySafeValueFailure<IntegerType, Native>(maxRegina * 2);
}

template <ArbitraryPrecisionInteger IntegerType, SignedCppInteger Native>
static void verifySafeValue() {
    SCOPED_TRACE_TYPE(Native);

    static constexpr Native minNative = std::numeric_limits<Native>::min();
    static constexpr Native maxNative = std::numeric_limits<Native>::max();

    // Compute the same values directly in Regina's large integer type.
    // They should be (-100...0) and (+11...1) in binary.
    IntegerType minRegina = -1;
    for (int i = 1; i < 8 * sizeof(Native); ++i) {
        minRegina *= 2;
    }
    IntegerType maxRegina;
    for (int i = 1; i < 8 * sizeof(Native); ++i) {
        maxRegina *= 2;
        ++maxRegina;
    }
    EXPECT_EQ(minRegina, minNative);
    EXPECT_EQ(maxRegina, maxNative);

    verifySafeValueFailure<IntegerType, Native>(minRegina * 2);
    verifySafeValueFailure<IntegerType, Native>(minRegina - 2);
    verifySafeValueFailure<IntegerType, Native>(minRegina - 1);
    verifySafeValueSuccess<IntegerType, Native>(minRegina, minNative);
    verifySafeValueSuccess<IntegerType, Native>(minRegina + 1, minNative + 1);
    verifySafeValueSuccess<IntegerType, Native>(minRegina + 2, minNative + 2);
    verifySafeValueSuccess<IntegerType, Native>((minRegina / 2) - 1,
        (minNative / 2) - 1);
    verifySafeValueSuccess<IntegerType, Native>(minRegina / 2, minNative / 2);
    verifySafeValueSuccess<IntegerType, Native>((minRegina / 2) + 1,
        (minNative / 2) + 1);
    verifySafeValueSuccess<IntegerType, Native>(-1, -1);
    verifySafeValueSuccess<IntegerType, Native>(0, 0);
    verifySafeValueSuccess<IntegerType, Native>(1, 1);
    verifySafeValueSuccess<IntegerType, Native>((maxRegina / 2) - 1,
        (maxNative / 2) - 1);
    verifySafeValueSuccess<IntegerType, Native>(maxRegina / 2, maxNative / 2);
    verifySafeValueSuccess<IntegerType, Native>((maxRegina / 2) + 1,
        (maxNative / 2) + 1);
    verifySafeValueSuccess<IntegerType, Native>(maxRegina - 2, maxNative - 2);
    verifySafeValueSuccess<IntegerType, Native>(maxRegina - 1, maxNative - 1);
    verifySafeValueSuccess<IntegerType, Native>(maxRegina, maxNative);
    verifySafeValueFailure<IntegerType, Native>(maxRegina + 1);
    verifySafeValueFailure<IntegerType, Native>(maxRegina + 2);
    verifySafeValueFailure<IntegerType, Native>(maxRegina * 2);

    if constexpr (IntegerType::supportsInfinity)
        verifySafeValueFailure<IntegerType, Native>(IntegerType::infinity);
}

TYPED_TEST(IntegerTest, safeValue) {
    verifySafeValue<TypeParam, unsigned char>();
    verifySafeValue<TypeParam, unsigned short>();
    verifySafeValue<TypeParam, unsigned>();
    verifySafeValue<TypeParam, unsigned long>();
    verifySafeValue<TypeParam, unsigned long long>();
    verifySafeValue<TypeParam, size_t>();

    verifySafeValue<TypeParam, signed char>();
    verifySafeValue<TypeParam, short>();
    verifySafeValue<TypeParam, int>();
    verifySafeValue<TypeParam, long>();
    verifySafeValue<TypeParam, long long>();
    verifySafeValue<TypeParam, ssize_t>();

#ifdef INT128_AVAILABLE
    verifySafeValue<TypeParam, regina::IntOfSize<16>::utype>();
    verifySafeValue<TypeParam, regina::IntOfSize<16>::type>();
#endif
}

