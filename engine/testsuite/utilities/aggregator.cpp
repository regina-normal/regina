
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

#include <cmath>
#include "utilities/aggregator.h"

#include "testhelper.h"

using regina::MaxAggregator;

TEST(AggregatorTest, max) {
    using Agg = MaxAggregator<float>;

    Agg a;
    EXPECT_TRUE(a.empty());
    EXPECT_THROW({ a.value(); }, regina::NoSolution);
    EXPECT_EQ(a.count(), 0);

    a.aggregate(-2.0);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.value(), -2.0);
    EXPECT_EQ(a.count(), 1);

    a.aggregate(-0.0);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.value(), 0.0);
    EXPECT_EQ(a.count(), 1);

    a.aggregate(+0.0); // equivalent but not equal to -0.0
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.value(), 0.0);
    EXPECT_EQ(a.count(), 2);

#if defined(NAN)
    EXPECT_THROW({ a.aggregate(NAN); }, regina::NoSolution);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.value(), 0.0);
    EXPECT_EQ(a.count(), 2);
#endif

    a.aggregate(-1.0);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.value(), 0.0);
    EXPECT_EQ(a.count(), 2);

    Agg b;
    EXPECT_NE(a, b);
    EXPECT_GT(a, b);
    b.aggregate(0.0);
    EXPECT_NE(a, b);
    EXPECT_GT(a, b);
    b.aggregate(0.0);
    EXPECT_EQ(a, b);
    b.aggregate(0.0);
    EXPECT_NE(a, b);
    EXPECT_LT(a, b);

    b.aggregate(a);
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.value(), 0.0);
    EXPECT_EQ(b.count(), 5);

    b.aggregate(1.0);
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.value(), 1.0);
    EXPECT_EQ(b.count(), 1);

    b.aggregate(a);
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.value(), 1.0);
    EXPECT_EQ(b.count(), 1);

#if defined(NAN)
    Agg c;
    c.aggregate(NAN);
    EXPECT_FALSE(c.empty());
    EXPECT_TRUE(std::isnan(c.value()));
    EXPECT_EQ(c.count(), 1);

    EXPECT_NE(c, b);
    EXPECT_FALSE(c < b);
    EXPECT_FALSE(c > b);
    EXPECT_FALSE(c == b);
    EXPECT_EQ(c <=> b, std::partial_ordering::unordered);

    EXPECT_THROW({ c.aggregate(1.0); }, regina::NoSolution);
#endif
}

