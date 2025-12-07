
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
    Integer dest;
    EXPECT_TRUE(regina::valueOf(s.str(), dest));
    EXPECT_EQ(dest, value);
}

template <typename Integer, bool isSigned>
static void testValueOf() {
    SCOPED_TRACE_TYPE(Integer);

    {
        Integer dest;
        EXPECT_TRUE(regina::valueOf("0", dest));
        EXPECT_EQ(dest, 0);
    }
    {
        Integer dest;
        EXPECT_TRUE(regina::valueOf("10", dest));
        EXPECT_EQ(dest, 10);
    }

    testValueOf<Integer>(std::numeric_limits<Integer>::max());
    testValueOf<Integer>(std::numeric_limits<Integer>::min());

    if constexpr (isSigned) {
        {
            Integer dest;
            EXPECT_TRUE(regina::valueOf("-1", dest));
            EXPECT_EQ(dest, -1);
        }
        {
            Integer dest;
            EXPECT_TRUE(regina::valueOf("-10", dest));
            EXPECT_EQ(dest, -10);
        }
    } else {
        {
            Integer dest;
            EXPECT_FALSE(regina::valueOf("-1", dest));
        }
        {
            Integer dest;
            EXPECT_FALSE(regina::valueOf("-10", dest));
        }
    }
}

TEST(StringUtilsTest, valueOf) {
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

    // TODO: Also test valueOf() for bool, double, BoolSet.
}

