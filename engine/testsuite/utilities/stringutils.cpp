
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

#include <limits>
#include <sstream>
#include "maths/integer.h"
#include "utilities/stringutils.h"

#include "testhelper.h"

template <regina::CppInteger Native>
static void testValueOf(Native value) {
    SCOPED_TRACE_INTEGER(value);

    Native dest = 3;
    EXPECT_TRUE(regina::valueOf(regina::toString(value), dest));
    EXPECT_EQ(dest, value);
}

template <regina::CppInteger Native>
static void testValueOf() {
    SCOPED_TRACE_TYPE(Native);

    {
        Native dest = 3;
        EXPECT_TRUE(regina::valueOf("0", dest));
        EXPECT_EQ(dest, 0);
    }
    {
        Native dest = 3;
        EXPECT_TRUE(regina::valueOf("10", dest));
        EXPECT_EQ(dest, 10);
    }

    testValueOf<Native>(std::numeric_limits<Native>::min());
    testValueOf<Native>(std::numeric_limits<Native>::min() + 1);
    testValueOf<Native>(std::numeric_limits<Native>::min() / 2);
    testValueOf<Native>(std::numeric_limits<Native>::max() / 2);
    testValueOf<Native>(std::numeric_limits<Native>::max() - 1);
    testValueOf<Native>(std::numeric_limits<Native>::max());

    if constexpr (regina::SignedCppInteger<Native>) {
        testValueOf<Native>(-128);
        testValueOf<Native>(-100);
    }
    testValueOf<Native>(100);
    testValueOf<Native>(127);
    if constexpr (regina::UnsignedCppInteger<Native>) {
        testValueOf<Native>(200);
        testValueOf<Native>(255);
    }

    if constexpr (regina::SignedCppInteger<Native>) {
        {
            Native dest = 3;
            EXPECT_TRUE(regina::valueOf("-1", dest));
            EXPECT_EQ(dest, -1);
        }
        {
            Native dest = 3;
            EXPECT_TRUE(regina::valueOf("-10", dest));
            EXPECT_EQ(dest, -10);
        }
    } else {
        {
            Native dest = 3;
            EXPECT_FALSE(regina::valueOf("-1", dest));
            EXPECT_EQ(dest, 3);
        }
        {
            Native dest = 3;
            EXPECT_FALSE(regina::valueOf("-10", dest));
            EXPECT_EQ(dest, 3);
        }
    }

    // Cases that should fail for all integer types:
    {
        Native dest = 3;
        EXPECT_FALSE(regina::valueOf("", dest));
        EXPECT_FALSE(regina::valueOf(" ", dest));
        EXPECT_FALSE(regina::valueOf("-", dest));
        EXPECT_FALSE(regina::valueOf("x", dest));
        EXPECT_EQ(dest, 3);
    }

    // Trailing characters should cause a failure.
    // The value of dest is theoretically undefined, but in practice valueOf()
    // should do its best with the string that it was given, and we check this
    // here.
    {
        Native dest = 3;
        EXPECT_FALSE(regina::valueOf("0 ", dest));
        EXPECT_EQ(dest, 3);
    }
    {
        Native dest = 3;
        EXPECT_FALSE(regina::valueOf("0x", dest));
        EXPECT_EQ(dest, 3);
    }
    {
        Native dest = 3;
        EXPECT_FALSE(regina::valueOf("10 ", dest));
        EXPECT_EQ(dest, 3);
    }
    if constexpr (regina::SignedCppInteger<Native>) {
        {
            Native dest = 3;
            EXPECT_FALSE(regina::valueOf("-1 ", dest));
            EXPECT_EQ(dest, 3);
        }
        {
            Native dest = 3;
            EXPECT_FALSE(regina::valueOf("-10 ", dest));
            EXPECT_EQ(dest, 3);
        }
    } else {
        {
            Native dest = 3;
            EXPECT_FALSE(regina::valueOf("-1 ", dest));
            EXPECT_EQ(dest, 3);
        }
        {
            Native dest = 3;
            EXPECT_FALSE(regina::valueOf("-10 ", dest));
            EXPECT_EQ(dest, 3);
        }
    }

    // What to do about leading whitespace?  Functions like strtol() can
    // handle it, but we claim that any whitespace at all should cause a
    // failure, so test for that.
    {
        Native dest = 3;
        EXPECT_FALSE(regina::valueOf(" 0", dest));
        EXPECT_FALSE(regina::valueOf(" 10", dest));
        EXPECT_FALSE(regina::valueOf(" -1", dest));
        EXPECT_FALSE(regina::valueOf(" -10", dest));
        EXPECT_EQ(dest, 3);
    }

    // Check how valueOf() behaves in the presence of overflow.
    {
        regina::Integer overflow = std::numeric_limits<Native>::max();
        ++overflow;
        SCOPED_TRACE_REGINA(overflow);
        Native dest = 3;
        EXPECT_FALSE(regina::valueOf(overflow.str(), dest));
        EXPECT_EQ(dest, 3);
    }
    {
        regina::Integer overflow = std::numeric_limits<Native>::min();
        --overflow;
        SCOPED_TRACE_REGINA(overflow);
        Native dest = 3;
        EXPECT_FALSE(regina::valueOf(overflow.str(), dest));
        EXPECT_EQ(dest, 3);
    }
}

TEST(StringUtilsTest, valueOf) {
    // Tests for integer types:

    testValueOf<signed char>();
    testValueOf<short>();
    testValueOf<int>();
    testValueOf<long>();
    testValueOf<long long>();
    testValueOf<ssize_t>();

    testValueOf<unsigned char>();
    testValueOf<unsigned short>();
    testValueOf<unsigned int>();
    testValueOf<unsigned long>();
    testValueOf<unsigned long long>();
    testValueOf<size_t>();

    #ifdef INT128_AVAILABLE
    testValueOf<regina::Int128>();
    testValueOf<regina::UInt128>();
    #endif

    // Some very basic tests for double:

    {
        double dest = 3.0;
        EXPECT_TRUE(regina::valueOf("2.5", dest));
        EXPECT_EQ(dest, 2.5);
    }
    {
        double dest = 3.0;
        EXPECT_TRUE(regina::valueOf("-2.5", dest));
        EXPECT_EQ(dest, -2.5);
    }
    {
        double dest = 3.0;
        EXPECT_FALSE(regina::valueOf(" 2.5", dest));
        EXPECT_FALSE(regina::valueOf("2.5 ", dest));
        EXPECT_EQ(dest, 3.0);
    }

    // TODO: Properly test valueOf() for bool, double, BoolSet.
}

