
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
#include "maths/integer.h"
#include "utilities/aggregator.h"

#include "testhelper.h"

using regina::Integer;
using regina::MaxCountAggregator;
using regina::SetAggregator;

namespace {
    /**
     * Casts any pointer to `const void*`.  This is necessary to avoid a linker
     * error on some platforms (e.g., macOS + Xcode), where it seems that
     * EXPECT_EQ and EXPECT_NE would otherwise try to call a custom output
     * stream operator for GMP pointers.
     */
    template <typename T>
    const void* ptr(T* p) { return static_cast<const void*>(p); }
}

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

TEST(AggregatorTest, setCopyMove) {
    // Here we test some of the move operations with SetAggregator.
    // For this we use the Integer type, where Integer::rawData() gives us
    // a mechanism to detect moves vs copies.
    Integer one(1), two(2), three(3), four(4);
    auto p1 = ptr(one.rawData());
    auto p2 = ptr(two.rawData());
    auto p3 = ptr(three.rawData());
    EXPECT_NE(p1, nullptr);
    EXPECT_NE(p2, nullptr);
    EXPECT_NE(p3, nullptr);

    SetAggregator<Integer> a;
    a += one;
    a += two;
    EXPECT_EQ(a.result().size(), 2);
    EXPECT_NE(ptr(a.result().begin()->rawData()), p1);

    SetAggregator<Integer> b;
    b += std::move(one);
    b += std::move(two);
    EXPECT_EQ(b.result().size(), 2);
    EXPECT_EQ(ptr(b.result().begin()->rawData()), p1);

    SetAggregator<Integer> c;
    c += 2; // two has already been moved from
    c += three;
    c += b;
    EXPECT_EQ(c.result().size(), 3);
    EXPECT_NE(ptr(c.result().begin()->rawData()), p1);
    EXPECT_NE(ptr(c.result().rbegin()->rawData()), p3);

    SetAggregator<Integer> d;
    d += 2; // two has already been moved from
    d += std::move(three);
    d += std::move(b);
    EXPECT_EQ(d.result().size(), 3);
    EXPECT_EQ(ptr(d.result().begin()->rawData()), p1);
    EXPECT_EQ(ptr(d.result().rbegin()->rawData()), p3);

    std::set<Integer> e = d.result();
    EXPECT_EQ(e.size(), 3);
    EXPECT_NE(ptr(e.begin()->rawData()), p1);
    EXPECT_NE(ptr(e.rbegin()->rawData()), p3);

    std::set<Integer> f = std::move(d).result();
    EXPECT_EQ(f.size(), 3);
    EXPECT_EQ(ptr(f.begin()->rawData()), p1);
    EXPECT_EQ(ptr(f.rbegin()->rawData()), p3);
}

