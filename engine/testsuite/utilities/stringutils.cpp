
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
#include "utilities/stringutils.h"

#include "testhelper.h"

template <typename Integer>
static void testValueOf(Integer value) {
    SCOPED_TRACE_NUMERIC(value);

    std::ostringstream s;
    if constexpr (sizeof(Integer) <= sizeof(char)) {
        // I believe that (+value) promotes char to an int,
        // which means it will be written numerically.
        s << (+value);
    } else {
        s << value;
    }
    Integer dest = 3;
    EXPECT_TRUE(regina::valueOf(s.str(), dest));
    EXPECT_EQ(dest, value);
}

template <typename Integer, bool isSigned>
requires regina::is_cpp_integer_v<Integer>
static void testValueOf() {
    SCOPED_TRACE_TYPE(Integer);

    {
        Integer dest = 3;
        EXPECT_TRUE(regina::valueOf("0", dest));
        EXPECT_EQ(dest, 0);
    }
    {
        Integer dest = 3;
        EXPECT_TRUE(regina::valueOf("10", dest));
        EXPECT_EQ(dest, 10);
    }

    testValueOf<Integer>(std::numeric_limits<Integer>::max());
    testValueOf<Integer>(std::numeric_limits<Integer>::min());

    if constexpr (isSigned) {
        {
            Integer dest = 3;
            EXPECT_TRUE(regina::valueOf("-1", dest));
            EXPECT_EQ(dest, -1);
        }
        {
            Integer dest = 3;
            EXPECT_TRUE(regina::valueOf("-10", dest));
            EXPECT_EQ(dest, -10);
        }
    } else {
        {
            Integer dest = 3;
            EXPECT_FALSE(regina::valueOf("-1", dest));
            EXPECT_EQ(dest, 3); // this is not guaranteed by valueOf()
        }
        {
            Integer dest = 3;
            EXPECT_FALSE(regina::valueOf("-10", dest));
            EXPECT_EQ(dest, 3); // this is not guaranteed by valueOf()
        }
    }

    // Cases that should fail for all integer types:
    {
        Integer dest = 3;
        EXPECT_FALSE(regina::valueOf("", dest));
        EXPECT_FALSE(regina::valueOf(" ", dest));
        EXPECT_FALSE(regina::valueOf("-", dest));
        EXPECT_FALSE(regina::valueOf("x", dest));
    }

    // Trailing characters should cause a failure, but (as promised) the
    // routine should convert the string as best it can.
    {
        Integer dest = 3;
        EXPECT_FALSE(regina::valueOf("0 ", dest));
        EXPECT_EQ(dest, 0);
    }
    {
        Integer dest = 3;
        EXPECT_FALSE(regina::valueOf("0x", dest));
        EXPECT_EQ(dest, 0);
    }
    {
        Integer dest = 3;
        EXPECT_FALSE(regina::valueOf("10 ", dest));
        EXPECT_EQ(dest, 10);
    }
    if constexpr (isSigned) {
        {
            Integer dest = 3;
            EXPECT_FALSE(regina::valueOf("-1 ", dest));
            EXPECT_EQ(dest, -1);
        }
        {
            Integer dest = 3;
            EXPECT_FALSE(regina::valueOf("-10 ", dest));
            EXPECT_EQ(dest, -10);
        }
    } else {
        {
            Integer dest = 3;
            EXPECT_FALSE(regina::valueOf("-1 ", dest));
            EXPECT_EQ(dest, 3); // this is not guaranteed by valueOf()
        }
        {
            Integer dest = 3;
            EXPECT_FALSE(regina::valueOf("-10 ", dest));
            EXPECT_EQ(dest, 3); // this is not guaranteed by valueOf()
        }
    }

    // What to do about leading whitespace?  Functions like strtol() can
    // handle it, but we claim that any whitespace at all should cause a
    // failure, so test for that.
    {
        Integer dest = 3;
        EXPECT_FALSE(regina::valueOf(" 0", dest));
        EXPECT_FALSE(regina::valueOf(" 10", dest));
        EXPECT_FALSE(regina::valueOf(" -1", dest));
        EXPECT_FALSE(regina::valueOf(" -10", dest));
        EXPECT_EQ(dest, 3); // this is not guaranteed by valueOf()
    }
}

TEST(StringUtilsTest, valueOf) {
    // Tests for integer types:

    testValueOf<signed char, true>();
    testValueOf<short, true>();
    testValueOf<int, true>();
    testValueOf<long, true>();
    testValueOf<long long, true>();
    testValueOf<ssize_t, true>();

    testValueOf<unsigned char, false>();
    testValueOf<unsigned short, false>();
    testValueOf<unsigned int, false>();
    testValueOf<unsigned long, false>();
    testValueOf<unsigned long long, false>();
    testValueOf<size_t, false>();

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
    }

    // TODO: Properly test valueOf() for bool, double, BoolSet.
}

