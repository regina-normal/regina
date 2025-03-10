
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

#include <cstring>
#include <limits>
#include <string>
#include "maths/integer.h"
#include "utilities/intutils.h"
#include "utilities/tightencoding-impl.h"

#include "testhelper.h"

using regina::Integer;
using regina::LargeInteger;

template <typename T>
static Integer toInteger(T val) {
    // The type T must be a native C++ integer type.

    // This function exists because Integer cannot necessarily
    // convert from long long, and even with conversion from long
    // it only works with signed (not unsigned) arguments, which
    // would exclude the maximum possible unsigned long value.
    if constexpr (sizeof(T) == 1) {
        // We cannot write this to an output stream since it
        // will be treated as a character, not an integer.
        return int(val);
    } else {
        std::ostringstream out;
        out << val;
        return Integer(out.str());
    }
}

template <typename T>
static T fromInteger(const Integer& val) {
    // The type T must be a native C++ integer type.

    // This function exists so that we can convert an Integer to
    // a native C++ integer type, even if that type takes values
    // outside the range of a long (which means Integer::longValue
    // is not good enough).

    // Due to our use of the standard library string-to-integer
    // conversion functions, this currently cannot handle
    // any type T larger than a (signed or unsigned) long long.
    static_assert(sizeof(T) <= sizeof(long long));

    if (sizeof(T) < sizeof(long)) {
        return static_cast<T>(val.longValue());
    } else if (sizeof(T) == sizeof(long)) {
        if constexpr(std::is_unsigned_v<T>)
            return std::stoul(val.stringValue());
        else
            return val.longValue();
    } else if (sizeof(T) <= sizeof(long long)) {
        if constexpr(std::is_unsigned_v<T>)
            return std::stoull(val.stringValue());
        else
            return std::stoll(val.stringValue());
    }
}

template <typename T>
static void verifyUsing(const Integer& val, const std::string& enc) {
    SCOPED_TRACE_TYPE(T);
    SCOPED_TRACE_REGINA(val);

    constexpr static bool arbitraryPrecision =
        regina::IsReginaArbitraryPrecisionInteger<T>::value;

    if constexpr (! arbitraryPrecision) {
        // This is a native C++ integer type, and so could be out of range.
        if (val > toInteger(std::numeric_limits<T>::max()) ||
                val < toInteger(std::numeric_limits<T>::min())) {
            // This integer *should* be out of range.
            EXPECT_THROW({
                regina::tightDecoding<T>(enc);
            }, regina::InvalidArgument);

            EXPECT_THROW({
                std::istringstream input(enc);
                regina::tightDecode<T>(input);
            }, regina::InvalidInput);
        }
        return;
    }

    // This integer should be in range.
    // First fetch it as type T, and in the case of a native C++
    // integer type, make sure it looks like we've done this correctly.
    T native;
    if constexpr (arbitraryPrecision) {
        native = val;
    } else {
        native = fromInteger<T>(val);

        std::ostringstream out;
        if constexpr (sizeof(T) == 1)
            out << int(native);
        else
            out << native;
        ASSERT_EQ(out.str(), val.stringValue());
    }

    // Now we can verify encodings and decodings.
    EXPECT_EQ(regina::tightEncoding(native), enc);

    {
        std::ostringstream out;
        regina::tightEncode(out, native);
        EXPECT_EQ(out.str(), enc);
    }

    EXPECT_NO_THROW({
        T dec = regina::tightDecoding<T>(enc);
        EXPECT_EQ(dec, native);
    });

    EXPECT_NO_THROW({
        std::istringstream input(enc);
        T dec = regina::tightDecode<T>(input);
        EXPECT_EQ(dec, native);
    });

    // Strings being decoded cannot have trailing whitespace.
    EXPECT_THROW({
        regina::tightDecoding<T>(enc + ' ');
    }, regina::InvalidArgument);

    // Streams being decoded should ignore (and not consume) any trailing
    // characters.
    EXPECT_NO_THROW({
        std::istringstream input(enc + "x y z");
        T dec = regina::tightDecode<T>(input);
        EXPECT_EQ(dec, native);

        char c;
        input >> c;
        ASSERT_TRUE(input);
        EXPECT_EQ(c, 'x');
    });
}

static void verifyInteger(Integer val) {
    std::string enc = val.tightEncoding();

    verifyUsing<int8_t>(val, enc);
    verifyUsing<uint8_t>(val, enc);
    verifyUsing<int16_t>(val, enc);
    verifyUsing<uint16_t>(val, enc);
    verifyUsing<int32_t>(val, enc);
    verifyUsing<uint32_t>(val, enc);
    verifyUsing<int64_t>(val, enc);
    verifyUsing<uint64_t>(val, enc);
    verifyUsing<long long>(val, enc);
    verifyUsing<unsigned long long>(val, enc);
    // Leave out 128-bit types for now, since these are not well
    // supported in the standard library with iostreams/typeinfo/etc.
    verifyUsing<Integer>(val, enc);
    verifyUsing<LargeInteger>(val, enc);
}

TEST(TightEncodingTest, integer) {
    // Test *all* the one-digit and two-digit cases, plus the
    // three-digit boundary cases on either side.
    for (int i = -4140; i <= 4141; ++i)
        verifyInteger(i);

    // Test the boundaries between the three-digit and general cases.
    verifyInteger(-368640);
    verifyInteger(-368639);
    verifyInteger(368640);
    verifyInteger(368641);

    // Test the boundaries where the base 45 digit wraps around.
    verifyInteger(-368730);
    verifyInteger(-368729);
    verifyInteger(-368685);
    verifyInteger(-368684);
    verifyInteger(368684);
    verifyInteger(368685);
    verifyInteger(368729);
    verifyInteger(368730);

    // Test the boundaries of different native C++ types.
    // Once we cross the 32-bit boundary we could theoretically
    // be out of the range of a long, so we use strings.
    // 16-bit:
    verifyInteger(32767);
    verifyInteger(32768);
    verifyInteger(-32768);
    verifyInteger(-32769);
    verifyInteger(65535);
    verifyInteger(65536);
    // 32-bit:
    verifyInteger("2147483647");
    verifyInteger("2147483648");
    verifyInteger("-2147483648");
    verifyInteger("-2147483649");
    verifyInteger("4294967295");
    verifyInteger("4294967296");
    // 64-bit:
    verifyInteger("9223372036854775807");
    verifyInteger("9223372036854775808");
    verifyInteger("-9223372036854775808");
    verifyInteger("-9223372036854775809");
    verifyInteger("18446744073709551615");
    verifyInteger("18446744073709551616");
    // 128-bit:
    verifyInteger("170141183460469231731687303715884105727");
    verifyInteger("170141183460469231731687303715884105728");
    verifyInteger("-170141183460469231731687303715884105728");
    verifyInteger("-170141183460469231731687303715884105729");
    verifyInteger("340282366920938463463374607431768211455");
    verifyInteger("340282366920938463463374607431768211456");

    // Test something that doesn't fit into *any* native type.
    verifyInteger("10000000000000000000000000000000000000000000");
    verifyInteger("-10000000000000000000000000000000000000000000");
}

template <typename T>
static void verifyInfinityUsing(const std::string& enc) {
    SCOPED_TRACE_TYPE(T);

    if constexpr (! std::is_same_v<T, LargeInteger>) {
        // This type does not support infinity.
        EXPECT_THROW({
            regina::tightDecoding<T>(enc);
        }, regina::InvalidArgument);

        EXPECT_THROW({
            std::istringstream input(enc);
            regina::tightDecode<T>(input);
        }, regina::InvalidInput);
    } else {
        // T is the LargeInteger type, and should support infinity.
        // Verify all methods of encoding and decoding.
        EXPECT_EQ(regina::tightEncoding(LargeInteger::infinity), enc);

        {
            std::ostringstream out;
            regina::tightEncode(out, LargeInteger::infinity);
            EXPECT_EQ(out.str(), enc);
        }

        EXPECT_NO_THROW({
            LargeInteger dec = regina::tightDecoding<LargeInteger>(enc);
            EXPECT_TRUE(dec.isInfinite());
        });

        EXPECT_NO_THROW({
            std::istringstream input(enc);
            LargeInteger dec = regina::tightDecode<LargeInteger>(input);
            EXPECT_TRUE(dec.isInfinite());
        });

        // Strings being decoded cannot have trailing whitespace.
        EXPECT_THROW({
            regina::tightDecoding<LargeInteger>(enc + ' ');
        }, regina::InvalidArgument);

        // Streams being decoded should ignore (and not consume) any trailing
        // characters.
        EXPECT_NO_THROW({
            std::istringstream input(enc + "x y z");
            LargeInteger dec = regina::tightDecode<LargeInteger>(input);
            EXPECT_TRUE(dec.isInfinite());

            char c;
            input >> c;
            ASSERT_TRUE(input);
            EXPECT_EQ(c, 'x');
        });
    }
}

TEST(TightEncodingTest, infinity) {
    std::string enc = LargeInteger::infinity.tightEncoding();

    verifyInfinityUsing<int8_t>(enc);
    verifyInfinityUsing<uint8_t>(enc);
    verifyInfinityUsing<int16_t>(enc);
    verifyInfinityUsing<uint16_t>(enc);
    verifyInfinityUsing<int32_t>(enc);
    verifyInfinityUsing<uint32_t>(enc);
    verifyInfinityUsing<int64_t>(enc);
    verifyInfinityUsing<uint64_t>(enc);
    verifyInfinityUsing<long long>(enc);
    verifyInfinityUsing<unsigned long long>(enc);
    // Leave out 128-bit types for now, since these are not well
    // supported in the standard library with iostreams/typeinfo/etc.
    verifyInfinityUsing<Integer>(enc);
    verifyInfinityUsing<LargeInteger>(enc);
}

static void verifyBoolean(bool value, const std::string& enc) {
    SCOPED_TRACE_NUMERIC(value);
    {
        std::string s = regina::tightEncoding(value);
        EXPECT_EQ(s, enc);
    }
    {
        std::ostringstream out;
        regina::tightEncode(out, value);
        EXPECT_EQ(out.str(), enc);
    }

    EXPECT_NO_THROW({
        bool dec = regina::tightDecoding<bool>(enc);
        EXPECT_EQ(dec, value);
    });

    EXPECT_NO_THROW({
        std::istringstream input(enc);
        bool dec = regina::tightDecode<bool>(input);
        EXPECT_EQ(dec, value);
    });

    // Strings being decoded cannot have trailing whitespace.
    EXPECT_THROW({
        regina::tightDecoding<bool>(enc + ' ');
    }, regina::InvalidArgument);

    // Streams being decoded should ignore (and not consume) any trailing
    // characters.
    EXPECT_NO_THROW({
        std::istringstream input(enc + "x y z");
        bool dec = regina::tightDecode<bool>(input);
        EXPECT_EQ(dec, value);

        char c;
        input >> c;
        ASSERT_TRUE(input);
        EXPECT_EQ(c, 'x');
    });
}

TEST(TightEncodingTest, boolean) {
    verifyBoolean(true, regina::tightEncoding(1));
    verifyBoolean(false, regina::tightEncoding(0));
}

template <typename T>
static void verifyIndex(T val) {
    SCOPED_TRACE_TYPE(T);
    SCOPED_TRACE_NUMERIC(val);

    std::ostringstream out;
    regina::detail::tightEncodeIndex(out, val);
    std::string enc = out.str();

    EXPECT_NO_THROW({
        std::istringstream input(enc);
        T dec = regina::detail::tightDecodeIndex<T>(input);
        EXPECT_EQ(dec, val);
    });

    // Streams being decoded should ignore (and not consume) any trailing
    // characters.
    EXPECT_NO_THROW({
        std::istringstream input(enc + "x y z");
        T dec = regina::detail::tightDecodeIndex<T>(input);
        EXPECT_EQ(dec, val);

        char c;
        input >> c;
        ASSERT_TRUE(input);
        EXPECT_EQ(c, 'x');
    });
}

template <typename T>
static void verifyIndexMax() {
    SCOPED_TRACE_TYPE(T);

    // First test the maximum possible value.
    static constexpr T max = std::numeric_limits<T>::max();
    verifyIndex<T>(max);

    // Now test what happens just beyond the maximum possible value.
    // This should refuse to encode.
    std::ostringstream out;
    regina::detail::tightEncodeIndex(out, max);
    std::string enc = out.str();

    EXPECT_GE(enc.length(), 2);

    // Step up to encode the next integer value.
    // In base 90, the first digit of the encoding should currently be
    // (MAX_VALUE - 737279) mod 90 == (MAX_VALUE + 1) mod 90.
    // In particular, we would expect MAX_VALUE to be odd, and so
    // this first digit should not be 89.
    // This means that we can safely increment the first digit.
    EXPECT_NE(enc[1], 122);
    ++enc[1];

    // Likewise, incrementing the decoding beyond max should refuse to decode.
    EXPECT_THROW({
        std::istringstream input(enc);
        regina::detail::tightDecodeIndex<T>(input);
    }, regina::InvalidInput);
}

TEST(TightEncodingTest, index) {
    // First verify that negative indices do the right thing.
    verifyIndex<ssize_t>(-1);
    EXPECT_THROW({
        std::ostringstream out;
        regina::detail::tightEncodeIndex(out, static_cast<ssize_t>(-2));
    }, regina::InvalidArgument);

    // Test *all* the one-digit and two-digit cases, plus a bit
    // into the three-digit cases.
    for (int i = 0; i <= 10000; ++i) {
        verifyIndex<uint16_t>(i);
        verifyIndex<uint32_t>(i);
        verifyIndex<uint64_t>(i);
        verifyIndex<unsigned long long>(i);
        verifyIndex<size_t>(i);
        verifyIndex<ssize_t>(i);
    }

    // Test the boundaries between the three-digit and general cases.
    // (This is only relevant for 32-bit integers and larger.)
    verifyIndex<uint32_t>(737278);
    verifyIndex<uint32_t>(737279);
    verifyIndex<uint64_t>(737278);
    verifyIndex<uint64_t>(737279);
    verifyIndex<unsigned long long>(737278);
    verifyIndex<unsigned long long>(737279);
    verifyIndex<size_t>(737278);
    verifyIndex<size_t>(737279);
    verifyIndex<ssize_t>(737278);
    verifyIndex<ssize_t>(737279);

    // Test at and beyond the maximum possible values.
    verifyIndexMax<uint16_t>();
    verifyIndexMax<uint32_t>();
    verifyIndexMax<uint64_t>();
    verifyIndexMax<unsigned long long>();
    verifyIndexMax<size_t>();
    verifyIndexMax<ssize_t>();
}
