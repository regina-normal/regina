
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include <limits>
#include <sstream>
#include "maths/integer.h"
#include "utilities/stringutils.h"

#include "testhelper.h"

using regina::InvalidArgument;

template <regina::CppInteger Native>
static void testParse(Native value) {
    SCOPED_TRACE_INTEGER(value);
    EXPECT_NO_THROW({
        EXPECT_EQ(regina::parse<Native>(regina::toString(value)), value); });
}

template <regina::CppInteger Native>
static void testParse() {
    SCOPED_TRACE_TYPE(Native);

    EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<Native>("0"), 0); });
    EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<Native>("10"), 10); });

    testParse<Native>(std::numeric_limits<Native>::min());
    for (Native i = 1; i <= 10; ++i)
        testParse<Native>(std::numeric_limits<Native>::min() + i);
    testParse<Native>(std::numeric_limits<Native>::min() / 2);
    testParse<Native>(std::numeric_limits<Native>::max() / 2);
    for (Native i = 1; i <= 10; ++i)
        testParse<Native>(std::numeric_limits<Native>::max() - i);
    testParse<Native>(std::numeric_limits<Native>::max());

    if constexpr (regina::SignedCppInteger<Native>) {
        EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<Native>("-128"), -128); });
        EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<Native>("-100"), -100); });
    }
    EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<Native>("100"), 100); });
    EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<Native>("127"), 127); });
    if constexpr (regina::UnsignedCppInteger<Native>) {
        EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<Native>("200"), 200); });
        EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<Native>("255"), 255); });
    }

    if constexpr (regina::SignedCppInteger<Native>) {
        EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<Native>("-1"), -1); });
        EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<Native>("-10"), -10); });
    } else {
        EXPECT_THROW({ regina::parse<Native>("-1"); }, InvalidArgument);
        EXPECT_THROW({ regina::parse<Native>("-10"); }, InvalidArgument);
    }

    // Cases that should fail for all integer types:
    EXPECT_THROW({ regina::parse<Native>(""); }, InvalidArgument);
    EXPECT_THROW({ regina::parse<Native>(" "); }, InvalidArgument);
    EXPECT_THROW({ regina::parse<Native>("-"); }, InvalidArgument);
    EXPECT_THROW({ regina::parse<Native>("x"); }, InvalidArgument);

    // Trailing characters should cause a failure.
    EXPECT_THROW({ regina::parse<Native>("0 "); }, InvalidArgument);
    EXPECT_THROW({ regina::parse<Native>("0x"); }, InvalidArgument);
    EXPECT_THROW({ regina::parse<Native>("10 "); }, InvalidArgument);
    EXPECT_THROW({ regina::parse<Native>("-1 "); }, InvalidArgument);
    EXPECT_THROW({ regina::parse<Native>("-10 "); }, InvalidArgument);

    // What to do about leading whitespace?  Functions like strtol() can
    // handle it, but we claim that any whitespace at all should cause a
    // failure, so test for that.
    {
        EXPECT_THROW({ regina::parse<Native>(" 0"); }, InvalidArgument);
        EXPECT_THROW({ regina::parse<Native>(" 10"); }, InvalidArgument);
        EXPECT_THROW({ regina::parse<Native>(" -1"); }, InvalidArgument);
        EXPECT_THROW({ regina::parse<Native>(" -10"); }, InvalidArgument);
    }

    // Check how parse() behaves in the presence of overflow.
    {
        regina::Integer overflow = std::numeric_limits<Native>::max();
        for (int i = 0; i < 10; ++i) {
            ++overflow;
            SCOPED_TRACE_REGINA(overflow);
            EXPECT_THROW({ regina::parse<Native>(overflow.str()); },
                InvalidArgument);
        }
    }
    {
        regina::Integer overflow = std::numeric_limits<Native>::min();
        for (int i = 0; i < 10; ++i) {
            --overflow;
            SCOPED_TRACE_REGINA(overflow);
            EXPECT_THROW({ regina::parse<Native>(overflow.str()); },
                InvalidArgument);
        }
    }
}

TEST(StringUtilsTest, parse) {
    // Tests for integer types:

    testParse<signed char>();
    testParse<short>();
    testParse<int>();
    testParse<long>();
    testParse<long long>();
    testParse<ssize_t>();

    testParse<unsigned char>();
    testParse<unsigned short>();
    testParse<unsigned int>();
    testParse<unsigned long>();
    testParse<unsigned long long>();
    testParse<size_t>();

    #ifdef INT128_AVAILABLE
    testParse<regina::Int128>();
    testParse<regina::UInt128>();
    #endif

    // Some very basic tests for double:
    EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<double>("2.5"), 2.5); });
    EXPECT_NO_THROW({ EXPECT_EQ(regina::parse<double>("-2.5"), -2.5); });
    EXPECT_THROW({ regina::parse<double>(" 2.5"); }, InvalidArgument);
    EXPECT_THROW({ regina::parse<double>("2.5 "); }, InvalidArgument);
    EXPECT_THROW({ regina::parse<double>(""); }, InvalidArgument);
    EXPECT_THROW({ regina::parse<double>(" "); }, InvalidArgument);

    // TODO: Properly test parse() for bool, double, BoolSet.
}

