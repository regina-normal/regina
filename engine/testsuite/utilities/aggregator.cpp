
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

#include <cmath>
#include "utilities/aggregator.h"

#include "testhelper.h"

using regina::MaxCountAggregator;

TEST(AggregatorTest, maxCount) {
    using Agg = MaxCountAggregator<float>;

    Agg a;
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.result().second, 0);

    a += -2.0;
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.result().first, -2.0);
    EXPECT_EQ(a.result().second, 1);

    a += -0.0;
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.result().first, 0.0);
    EXPECT_EQ(a.result().second, 1);

    a += +0.0; // equivalent but not equal to -0.0
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.result().first, 0.0);
    EXPECT_EQ(a.result().second, 2);

#if defined(NAN)
    EXPECT_THROW({ a += NAN; }, regina::NoSolution);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.result().first, 0.0);
    EXPECT_EQ(a.result().second, 2);
#endif

    a += -1.0;
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.result().first, 0.0);
    EXPECT_EQ(a.result().second, 2);

    Agg b;
    EXPECT_NE(a, b);
    EXPECT_GT(a, b);
    b += 0.0;
    EXPECT_NE(a, b);
    EXPECT_GT(a, b);
    b += 0.0;
    EXPECT_EQ(a, b);
    b += 0.0;
    EXPECT_NE(a, b);
    EXPECT_LT(a, b);

    b += a;
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.result().first, 0.0);
    EXPECT_EQ(b.result().second, 5);

    b += 1.0;
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.result().first, 1.0);
    EXPECT_EQ(b.result().second, 1);

    b += a;
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.result().first, 1.0);
    EXPECT_EQ(b.result().second, 1);

#if defined(NAN)
    Agg c;
    c += NAN;
    EXPECT_FALSE(c.empty());
    EXPECT_TRUE(std::isnan(c.result().first));
    EXPECT_EQ(c.result().second, 1);

    EXPECT_NE(c, b);
    EXPECT_FALSE(c < b);
    EXPECT_FALSE(c > b);
    EXPECT_FALSE(c == b);
    EXPECT_EQ(c <=> b, std::partial_ordering::unordered);

    EXPECT_THROW({ c += 1.0; }, regina::NoSolution);
#endif
}

