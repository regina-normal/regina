
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include <cppunit/extensions/HelperMacros.h>
#include "testsuite/utilities/testutilities.h"
#include "maths/integer.h"
#include "utilities/intutils.h"
#include "utilities/tightencoding-impl.h"

using regina::Integer;
using regina::LargeInteger;

class TightEncodingTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(TightEncodingTest);

    CPPUNIT_TEST(integer);
    CPPUNIT_TEST(infinity);
    CPPUNIT_TEST(boolean);

    CPPUNIT_TEST_SUITE_END();

    private:

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

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
        static std::string typeDesc() {
            if constexpr (std::is_same_v<T, Integer>) {
                return "regina::Integer";
            } else if constexpr (std::is_same_v<T, LargeInteger>) {
                return "regina::LargeInteger";
            } else {
                static_assert(std::is_integral_v<T>);

                // This function exists because some standard library
                // implementations do not provide typeid() for 128-bit types
                // even when the compiler supports them.
                std::ostringstream out;
                if constexpr (std::is_unsigned_v<T>)
                    out << "unsigned ";
                else
                    out << "signed ";
                out << (8 * sizeof(T)) << "-bit int";
                return out.str();
            }
        }

        template <typename T>
        static void verifyUsing(const Integer& val, const std::string& enc) {
            constexpr static bool arbitraryPrecision =
                regina::IsReginaArbitraryPrecisionInteger<T>::value;

            if constexpr (! arbitraryPrecision) {
                // This is a native C++ integer type, and so could be
                // out of range.
                if (val > toInteger(std::numeric_limits<T>::max()) ||
                        val < toInteger(std::numeric_limits<T>::min())) {
                    // This integer *should* be out of range.
                    try {
                        regina::tightDecoding<T>(enc);

                        std::ostringstream msg;
                        msg << "The tight encoding for " << val
                            << " using " << typeDesc<T>()
                            << " decodes as a string, "
                            "even though it should be out of range.";
                        CPPUNIT_FAIL(msg.str());
                    } catch (const regina::InvalidArgument&) {
                    }

                    try {
                        std::istringstream input(enc);
                        regina::tightDecoding<T>(input);

                        std::ostringstream msg;
                        msg << "The tight encoding for " << val
                            << " using " << typeDesc<T>()
                            << " decodes as an input stream, "
                            "even though it should be out of range.";
                        CPPUNIT_FAIL(msg.str());
                    } catch (const regina::InvalidInput&) {
                    }
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
                {
                    std::ostringstream out;
                    if constexpr (sizeof(T) == 1)
                        out << int(native);
                    else
                        out << native;
                    if (out.str() != val.stringValue()) {
                        std::ostringstream msg;
                        msg << "Could not convert " << val
                            << " to " << typeDesc<T>() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }

            // Now we can verify encodings and decodings.
            if (regina::tightEncoding(native) != enc) {
                std::ostringstream msg;
                msg << "The tight encoding as a string for "
                    << val << " using " << typeDesc<T>()
                    << " is not consistent with the encoding "
                    "using regina::Integer.";
                CPPUNIT_FAIL(msg.str());
            }
            {
                std::ostringstream out;
                regina::tightEncode(out, native);
                if (out.str() != enc) {
                    std::ostringstream msg;
                    msg << "The tight encoding as an output stream for "
                        << val << " using " << typeDesc<T>()
                        << " is not consistent with the encoding "
                        "using regina::Integer.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            try {
                T dec = regina::tightDecoding<T>(enc);
                if (dec != native) {
                    std::ostringstream msg;
                    msg << "The tight encoding for " << val
                        << " using " << typeDesc<T>()
                        << " does not decode as a string "
                        "to the same value.";
                    CPPUNIT_FAIL(msg.str());
                }
            } catch (const regina::InvalidArgument&) {
                std::ostringstream msg;
                msg << "The tight encoding for " << val
                    << " using " << typeDesc<T>()
                    << " does not decode as a string at all.";
                CPPUNIT_FAIL(msg.str());
            }

            try {
                std::istringstream input(enc);
                T dec = regina::tightDecoding<T>(input);
                if (dec != native) {
                    std::ostringstream msg;
                    msg << "The tight encoding for " << val
                        << " using " << typeDesc<T>()
                        << " does not decode as an input stream "
                        "to the same value.";
                    CPPUNIT_FAIL(msg.str());
                }
            } catch (const regina::InvalidInput&) {
                std::ostringstream msg;
                msg << "The tight encoding for " << val
                    << " using " << typeDesc<T>()
                    << " does not decode as an input stream at all.";
                CPPUNIT_FAIL(msg.str());
            }

            try {
                regina::tightDecoding<T>(enc + ' ');

                std::ostringstream msg;
                msg << "The tight encoding for " << val
                    << " using " << typeDesc<T>()
                    << " decodes as a string with trailing whitespace "
                    "(which it should not).";
                CPPUNIT_FAIL(msg.str());
            } catch (const regina::InvalidArgument&) {
            }

            try {
                std::istringstream input(enc + "x y z");
                T dec = regina::tightDecoding<T>(input);
                if (dec != native) {
                    std::ostringstream msg;
                    msg << "The tight encoding for " << val
                        << " using " << typeDesc<T>()
                        << " does not decode as an input stream "
                        "with trailing characters to the same value.";
                    CPPUNIT_FAIL(msg.str());
                }
                char c;
                input >> c;
                if ((! input) || (c != 'x')) {
                    std::ostringstream msg;
                    msg << "The tight encoding for " << val
                        << " using " << typeDesc<T>()
                        << " consumes trailing characters when "
                        "decoding as an input stream.";
                    CPPUNIT_FAIL(msg.str());
                }
            } catch (const regina::InvalidInput&) {
                std::ostringstream msg;
                msg << "The tight encoding for " << val
                    << " using " << typeDesc<T>()
                    << " does not decode as an input stream "
                    "with trailing characters at all.";
                CPPUNIT_FAIL(msg.str());
            }
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

        void integer() {
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
            if constexpr (! std::is_same_v<T, LargeInteger>) {
                // This type does not support infinity.
                try {
                    regina::tightDecoding<T>(enc);

                    std::ostringstream msg;
                    msg << "The tight encoding for infinity using "
                        << typeDesc<T>() << " decodes as a string, "
                        "even though it should be unsupported.";
                    CPPUNIT_FAIL(msg.str());
                } catch (const regina::InvalidArgument&) {
                }

                try {
                    std::istringstream input(enc);
                    regina::tightDecoding<T>(input);

                    std::ostringstream msg;
                    msg << "The tight encoding for infinity using "
                        << typeDesc<T>() << " decodes as an input stream, "
                        "even though it should be unsupported.";
                    CPPUNIT_FAIL(msg.str());
                } catch (const regina::InvalidInput&) {
                }

                return;
            } else {
                // T is the LargeInteger type, and should support infinity.
                // Verify all methods of encoding and decoding.
                if (regina::tightEncoding(LargeInteger::infinity) != enc) {
                    std::ostringstream msg;
                    msg << "The tight encoding as a string for infinity "
                        "is not consistent with the precomputed value.";
                    CPPUNIT_FAIL(msg.str());
                }
                {
                    std::ostringstream out;
                    regina::tightEncode(out, LargeInteger::infinity);
                    if (out.str() != enc) {
                        std::ostringstream msg;
                        msg << "The tight encoding as an output stream for "
                            "infinity is not consistent with the precomputed "
                            "value.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                try {
                    LargeInteger dec = regina::tightDecoding<LargeInteger>(enc);
                    if (! dec.isInfinite()) {
                        std::ostringstream msg;
                        msg << "The tight encoding for infinity "
                            "does not decode as a string to the same value.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } catch (const regina::InvalidArgument&) {
                    std::ostringstream msg;
                    msg << "The tight encoding for infinity "
                        "does not decode as a string at all.";
                    CPPUNIT_FAIL(msg.str());
                }

                try {
                    std::istringstream input(enc);
                    LargeInteger dec = regina::tightDecoding<LargeInteger>(
                        input);
                    if (! dec.isInfinite()) {
                        std::ostringstream msg;
                        msg << "The tight encoding for infinity "
                            "does not decode as an input stream "
                            "to the same value.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } catch (const regina::InvalidInput&) {
                    std::ostringstream msg;
                    msg << "The tight encoding for infinity "
                        "does not decode as an input stream at all.";
                    CPPUNIT_FAIL(msg.str());
                }

                try {
                    regina::tightDecoding<LargeInteger>(enc + ' ');

                    std::ostringstream msg;
                    msg << "The tight encoding for infinity "
                        "decodes as a string with trailing whitespace "
                        "(which it should not).";
                    CPPUNIT_FAIL(msg.str());
                } catch (const regina::InvalidArgument&) {
                }

                try {
                    std::istringstream input(enc + "x y z");
                    LargeInteger dec = regina::tightDecoding<LargeInteger>(
                        input);
                    if (! dec.isInfinite()) {
                        std::ostringstream msg;
                        msg << "The tight encoding for infinity "
                            "does not decode as an input stream "
                            "with trailing characters to the same value.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    char c;
                    input >> c;
                    if ((! input) || (c != 'x')) {
                        std::ostringstream msg;
                        msg << "The tight encoding for infinity "
                            "consumes trailing characters when "
                            "decoding as an input stream.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } catch (const regina::InvalidInput&) {
                    std::ostringstream msg;
                    msg << "The tight encoding for infinity "
                        "does not decode as an input stream "
                        "with trailing characters at all.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void infinity() {
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
            {
                std::string s = regina::tightEncoding(value);
                if (s != enc) {
                    std::ostringstream msg;
                    msg << "The tight encoding as an output stream for "
                        << value << " is not consistent with the "
                        "corresponding integer encoding.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            {
                std::ostringstream out;
                regina::tightEncode(out, value);
                if (out.str() != enc) {
                    std::ostringstream msg;
                    msg << "The tight encoding as an output stream for "
                        << value << " is not consistent with the "
                        "corresponding integer encoding.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            try {
                bool dec = regina::tightDecoding<bool>(enc);
                if (dec != value) {
                    std::ostringstream msg;
                    msg << "The tight encoding for " << value
                        << " does not decode as a string to the same value.";
                    CPPUNIT_FAIL(msg.str());
                }
            } catch (const regina::InvalidArgument&) {
                std::ostringstream msg;
                msg << "The tight encoding for " << value
                    << " does not decode as a string at all.";
                CPPUNIT_FAIL(msg.str());
            }

            try {
                std::istringstream input(enc);
                bool dec = regina::tightDecoding<bool>(input);
                if (dec != value) {
                    std::ostringstream msg;
                    msg << "The tight encoding for " << value
                        << " does not decode as an input stream "
                        "to the same value.";
                    CPPUNIT_FAIL(msg.str());
                }
            } catch (const regina::InvalidInput&) {
                std::ostringstream msg;
                msg << "The tight encoding for " << value
                    << " does not decode as an input stream at all.";
                CPPUNIT_FAIL(msg.str());
            }

            try {
                regina::tightDecoding<bool>(enc + ' ');

                std::ostringstream msg;
                msg << "The tight encoding for " << value
                    << " decodes as a string with trailing whitespace "
                    "(which it should not).";
                CPPUNIT_FAIL(msg.str());
            } catch (const regina::InvalidArgument&) {
            }

            try {
                std::istringstream input(enc + "x y z");
                bool dec = regina::tightDecoding<bool>(input);
                if (dec != value) {
                    std::ostringstream msg;
                    msg << "The tight encoding for " << value
                        << " does not decode as an input stream "
                        "with trailing characters to the same value.";
                    CPPUNIT_FAIL(msg.str());
                }
                char c;
                input >> c;
                if ((! input) || (c != 'x')) {
                    std::ostringstream msg;
                    msg << "The tight encoding for " << value
                        << " consumes trailing characters when "
                        "decoding as an input stream.";
                    CPPUNIT_FAIL(msg.str());
                }
            } catch (const regina::InvalidInput&) {
                std::ostringstream msg;
                msg << "The tight encoding for " << value
                    << " does not decode as an input stream "
                    "with trailing characters at all.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void boolean() {
            verifyBoolean(true, regina::tightEncoding(1));
            verifyBoolean(false, regina::tightEncoding(0));
        }
};

void addTightEncoding(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(TightEncodingTest::suite());
}

