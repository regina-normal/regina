
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

#include "maths/integer.h"
#include "maths/laurent2.h"
#include "utilities/stringutils.h"

#include "utilities/tightencodingtest.h"

using regina::Integer;
using regina::Laurent2;

class Laurent2Test : public testing::Test {
    protected:
        Laurent2<Integer> zero {};
        Laurent2<Integer> one { {0, 0, 1} };
        Laurent2<Integer> two { {0, 0, 2} };
        Laurent2<Integer> x2 { {2, 0, 1} };
        Laurent2<Integer> xy { {1, 1, 1} };
        Laurent2<Integer> a { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} };
        Laurent2<Integer> b { {1, 0, 2}, {1, 2, -4}, {0, -1, -2} };

        template <typename T>
        static void verifyEqual(const Laurent2<T>& result,
                std::initializer_list<std::tuple<long, long, T>> coeffs) {
            SCOPED_TRACE_REGINA(result);

            Laurent2<T> expect(coeffs);
            SCOPED_TRACE_REGINA(expect);

            EXPECT_EQ(result, expect);
            EXPECT_FALSE(result != expect);
            EXPECT_EQ(result.str(), expect.str());
            if (coeffs.size() == 0) {
                EXPECT_TRUE(result.isZero());
            } else {
                EXPECT_FALSE(result.isZero());
            }
        }

        template <typename T>
        void verifyPlus(const Laurent2<T>& x, const Laurent2<T>& y,
                std::initializer_list<std::tuple<long, long, T>> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x + y, coeffs);
            verifyEqual((x + zero) + y, coeffs);
            verifyEqual(x + (y + zero), coeffs);
            verifyEqual((x + zero) + (y + zero), coeffs);

            verifyEqual(y + x, coeffs);
            verifyEqual((y + zero) + x, coeffs);
            verifyEqual(y + (x + zero), coeffs);
            verifyEqual((y + zero) + (x + zero), coeffs);

            {
                Laurent2<T> z(x);
                verifyEqual(z += y, coeffs);
            }
            {
                Laurent2<T> z(x);
                verifyEqual(z += (y + zero), coeffs);
            }
            {
                Laurent2<T> z(y);
                verifyEqual(z += x, coeffs);
            }
            {
                Laurent2<T> z(y);
                verifyEqual(z += (x + zero), coeffs);
            }
        }

        template <typename T>
        void verifyMinus(const Laurent2<T>& x, const Laurent2<T>& y,
                std::initializer_list<std::tuple<long, long, T>> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x - y, coeffs);
            verifyEqual((x + zero) - y, coeffs);
            verifyEqual(x - (y + zero), coeffs);
            verifyEqual((x + zero) - (y + zero), coeffs);

            {
                Laurent2<T> z(x);
                verifyEqual(z -= y, coeffs);
            }
            {
                Laurent2<T> z(x);
                verifyEqual(z -= (y + zero), coeffs);
            }

            verifyPlus(x, -y, coeffs);
            {
                Laurent2<T> z(y);
                z.negate();
                verifyPlus(x, z, coeffs);
            }
        }

        template <typename T>
        void verifyMult(const Laurent2<T>& x, const T& y,
                std::initializer_list<std::tuple<long, long, T>> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x * y, coeffs);
            verifyEqual((x + zero) * y, coeffs);
            verifyEqual(y * x, coeffs);
            verifyEqual(y * (x + zero), coeffs);
            {
                Laurent2<T> z(x);
                verifyEqual(z *= y, coeffs);
            }
        }

        template <typename T>
        void verifyDiv(const Laurent2<T>& x, const T& y,
                std::initializer_list<std::tuple<long, long, T>> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x / y, coeffs);
            verifyEqual((x + zero) / y, coeffs);
            {
                Laurent2<T> z(x);
                verifyEqual(z /= y, coeffs);
            }
        }

        template <typename T>
        void verifyMult(const Laurent2<T>& x, const Laurent2<T>& y,
                std::initializer_list<std::tuple<long, long, T>> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x * y, coeffs);
            verifyEqual((x + zero) * y, coeffs);
            verifyEqual(x * (y + zero), coeffs);
            verifyEqual((x + zero) * (y + zero), coeffs);

            verifyEqual(y * x, coeffs);
            verifyEqual((y + zero) * x, coeffs);
            verifyEqual(y * (x + zero), coeffs);
            verifyEqual((y + zero) * (x + zero), coeffs);

            {
                Laurent2<T> z(x);
                verifyEqual(z *= y, coeffs);
            }
            {
                Laurent2<T> z(x);
                verifyEqual(z *= (y + zero), coeffs);
            }
            {
                Laurent2<T> z(y);
                verifyEqual(z *= x, coeffs);
            }
            {
                Laurent2<T> z(y);
                verifyEqual(z *= (x + zero), coeffs);
            }
        }
};

TEST_F(Laurent2Test, set) {
    Laurent2<Integer> x { {0, 0, 1}, {1, -1, 2} };

    verifyEqual<Integer>(x, { {0, 0, 1}, {1, -1, 2} });
    x.set(-1, 1, 3);
    verifyEqual<Integer>(x, { {0, 0, 1}, {1, -1, 2}, {-1, 1, 3} });
    x.set(0, 0, 0);
    verifyEqual<Integer>(x, { {1, -1, 2}, {-1, 1, 3} });
    x.set(1, -1, 0);
    verifyEqual<Integer>(x, { {-1, 1, 3} });
    x.set(1, -1, 0);
    verifyEqual<Integer>(x, { {-1, 1, 3} });
    x.set(-1, 1, 0);
    verifyEqual<Integer>(x, {});
    x.set(0, 0, 0);
    verifyEqual<Integer>(x, {});
    x.set(-1, -1, 1);
    verifyEqual<Integer>(x, { {-1, -1, 1} });
}

TEST_F(Laurent2Test, arithmetic) {
    verifyEqual<Integer>(zero, {});

    verifyEqual<Integer>(-zero, {});
    verifyEqual<Integer>(-a, { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });

    verifyPlus<Integer>(zero, zero, {});
    verifyPlus<Integer>(one, two, { {0, 0, 3} });
    verifyPlus<Integer>(a, zero,
        { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} });
    verifyPlus<Integer>(a, one,
        { {0, 0, 2}, {1, 2, -2}, {-2, -1, 3} });
    verifyPlus<Integer>(b, one,
        { {0, 0, 1}, {1, 0, 2}, {1, 2, -4}, {0, -1, -2} });
    verifyPlus<Integer>(a, b,
        { {0, 0, 1}, {1, 0, 2}, {1, 2, -6}, {0, -1, -2}, {-2, -1, 3} });

    verifyMinus<Integer>(zero, zero, {});
    verifyMinus<Integer>(one, two, { {0, 0, -1} });
    verifyMinus<Integer>(a, zero,
        { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} });
    verifyMinus<Integer>(zero, a,
        { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });
    verifyMinus<Integer>(a, one,
        { {1, 2, -2}, {-2, -1, 3} });
    verifyMinus<Integer>(a, a + a,
        { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });
    verifyMinus<Integer>(a, b,
        { {0, 0, 1}, {1, 0, -2}, {1, 2, 2}, {0, -1, 2}, {-2, -1, 3} });
    verifyMinus<Integer>(2 * a, b,
        { {0, 0, 2}, {1, 0, -2}, {0, -1, 2}, {-2, -1, 6} });
    verifyMinus<Integer>(a, a, {});

    verifyMult<Integer>(zero, 0, {});
    verifyMult<Integer>(zero, 1, {});
    verifyMult<Integer>(zero, 2, {});
    verifyMult<Integer>(a, 0, {});
    verifyMult<Integer>(a, 1, { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} });
    verifyMult<Integer>(a, -1, { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });
    verifyMult<Integer>(a, 2, { {0, 0, 2}, {1, 2, -4}, {-2, -1, 6} });

    verifyDiv<Integer>(zero, 1, {});
    verifyDiv<Integer>(zero, 2, {});
    verifyDiv<Integer>(a, 1, { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} });
    verifyDiv<Integer>(a, -1, { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });
    verifyDiv<Integer>(b, 2, { {1, 0, 1}, {1, 2, -2}, {0, -1, -1} });

    verifyMult<Integer>(zero, zero, {});
    verifyMult<Integer>(zero, a, {});
    verifyMult<Integer>({ {1, 0, 1}, {0, -1, 1} },
                        { {1, 0, 1}, {0, -1, -1} },
                        { {2, 0, 1}, {0, -2, -1} });
    verifyMult<Integer>(a, xy, { {1, 1, 1}, {2, 3, -2}, {-1, 0, 3} });

    {
        Laurent2<Integer> x(x2);
        verifyEqual<Integer>(x + x, { {2, 0, 2} });
        verifyEqual<Integer>(x += x, { {2, 0, 2} });
    }
    {
        Laurent2<Integer> x(x2);
        verifyEqual<Integer>(x - x, {});
        verifyEqual<Integer>(x -= x, {});
    }
    {
        Laurent2<Integer> x { {0, -1, 1}, {0, 1, 1} };
        verifyEqual<Integer>(x * x,
            { {0, -2, 1}, {0, 0, 2}, {0, 2, 1} });
        verifyEqual<Integer>(x *= x,
            { {0, -2, 1}, {0, 0, 2}, {0, 2, 1} });
    }
}

TEST_F(Laurent2Test, ringConstants) {
    // Verify that the RingTraits constants looks correct.
    EXPECT_EQ(regina::RingTraits<Laurent2<Integer>>::zero.str(), "0");
    EXPECT_EQ(regina::RingTraits<Laurent2<Integer>>::one.str(), "1");
}

TEST_F(Laurent2Test, tightEncoding) {
    TightEncodingTest<Laurent2<Integer>>::verifyTightEncoding(zero);
    TightEncodingTest<Laurent2<Integer>>::verifyTightEncoding(one);
    TightEncodingTest<Laurent2<Integer>>::verifyTightEncoding(two);
    TightEncodingTest<Laurent2<Integer>>::verifyTightEncoding(x2);
    TightEncodingTest<Laurent2<Integer>>::verifyTightEncoding(xy);
    TightEncodingTest<Laurent2<Integer>>::verifyTightEncoding(a);
    TightEncodingTest<Laurent2<Integer>>::verifyTightEncoding(b);
}
