
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "maths/integer.h"
#include "maths/laurent.h"
#include "utilities/stringutils.h"

#include "utilities/tightencodingtest.h"

using regina::Integer;
using regina::Laurent;

class LaurentTest : public testing::Test {
    protected:
        Laurent<Integer> zero {};
        Laurent<Integer> zero2 { 0, {} };
        Laurent<Integer> zero3 { 2, {} };
        Laurent<Integer> one { 0, { 1 } };
        Laurent<Integer> two { 0, { 2 } };
        Laurent<Integer> x2 { 2, { 1 } };
        Laurent<Integer> a { -1, { 1, -1, 1 } };
        Laurent<Integer> b { 0, { 1, -1, 1 } };
        Laurent<Integer> c { 1, { 1, -1, 1 } };
        Laurent<Integer> d { -2, { -1, 1, -1, 1 } };
        Laurent<Integer> e { 4, { 2, 4, -2, 2 } };

        template <typename T>
        static void verifyEqual(const Laurent<T>& result,
                long minExp, std::initializer_list<T> coeffs) {
            Laurent<T> expect(minExp, coeffs);
            EXPECT_EQ(result, expect);
            EXPECT_FALSE(result != expect);
            EXPECT_EQ(result.str(), expect.str());
            if (coeffs.size() == 0) {
                EXPECT_TRUE(result.isZero());
                EXPECT_EQ(result.minExp(), 0);
                EXPECT_EQ(result.maxExp(), 0);
            } else {
                EXPECT_FALSE(result.isZero());
                EXPECT_EQ(result.minExp(), minExp);
                EXPECT_EQ(result.maxExp(), minExp + coeffs.size() - 1);
            }
        }

        template <typename T>
        void verifyPlus(const Laurent<T>& x, const Laurent<T>& y,
                long minExp, std::initializer_list<T> coeffs) {
            verifyEqual(x + y, minExp, coeffs);
            verifyEqual((x + zero) + y, minExp, coeffs);
            verifyEqual(x + (y + zero), minExp, coeffs);
            verifyEqual((x + zero) + (y + zero), minExp, coeffs);

            verifyEqual(y + x, minExp, coeffs);
            verifyEqual((y + zero) + x, minExp, coeffs);
            verifyEqual(y + (x + zero), minExp, coeffs);
            verifyEqual((y + zero) + (x + zero), minExp, coeffs);

            {
                Laurent<T> z(x);
                verifyEqual(z += y, minExp, coeffs);
            }
            {
                Laurent<T> z(x);
                verifyEqual(z += (y + zero), minExp, coeffs);
            }
            {
                Laurent<T> z(y);
                verifyEqual(z += x, minExp, coeffs);
            }
            {
                Laurent<T> z(y);
                verifyEqual(z += (x + zero), minExp, coeffs);
            }
        }

        template <typename T>
        void verifyMinus(const Laurent<T>& x, const Laurent<T>& y,
                long minExp, std::initializer_list<T> coeffs) {
            verifyEqual(x - y, minExp, coeffs);
            verifyEqual((x + zero) - y, minExp, coeffs);
            verifyEqual(x - (y + zero), minExp, coeffs);
            verifyEqual((x + zero) - (y + zero), minExp, coeffs);

            {
                Laurent<T> z(x);
                verifyEqual(z -= y, minExp, coeffs);
            }
            {
                Laurent<T> z(x);
                verifyEqual(z -= (y + zero), minExp, coeffs);
            }

            verifyPlus(x, -y, minExp, coeffs);
            {
                Laurent<T> z(y);
                z.negate();
                verifyPlus(x, z, minExp, coeffs);
            }
        }

        template <typename T>
        void verifyMult(const Laurent<T>& x, const T& y,
                long minExp, std::initializer_list<T> coeffs) {
            verifyEqual(x * y, minExp, coeffs);
            verifyEqual((x + zero) * y, minExp, coeffs);
            verifyEqual(y * x, minExp, coeffs);
            verifyEqual(y * (x + zero), minExp, coeffs);
            {
                Laurent<T> z(x);
                verifyEqual(z *= y, minExp, coeffs);
            }
        }

        template <typename T>
        void verifyDiv(const Laurent<T>& x, const T& y,
                long minExp, std::initializer_list<T> coeffs) {
            verifyEqual(x / y, minExp, coeffs);
            verifyEqual((x + zero) / y, minExp, coeffs);
            {
                Laurent<T> z(x);
                verifyEqual(z /= y, minExp, coeffs);
            }
        }

        template <typename T>
        void verifyMult(const Laurent<T>& x, const Laurent<T>& y,
                long minExp, std::initializer_list<T> coeffs) {
            verifyEqual(x * y, minExp, coeffs);
            verifyEqual((x + zero) * y, minExp, coeffs);
            verifyEqual(x * (y + zero), minExp, coeffs);
            verifyEqual((x + zero) * (y + zero), minExp, coeffs);

            verifyEqual(y * x, minExp, coeffs);
            verifyEqual((y + zero) * x, minExp, coeffs);
            verifyEqual(y * (x + zero), minExp, coeffs);
            verifyEqual((y + zero) * (x + zero), minExp, coeffs);

            {
                Laurent<T> z(x);
                verifyEqual(z *= y, minExp, coeffs);
            }
            {
                Laurent<T> z(x);
                verifyEqual(z *= (y + zero), minExp, coeffs);
            }
            {
                Laurent<T> z(y);
                verifyEqual(z *= x, minExp, coeffs);
            }
            {
                Laurent<T> z(y);
                verifyEqual(z *= (x + zero), minExp, coeffs);
            }
        }
};

TEST_F(LaurentTest, set) {
    Laurent<Integer> x { -1, { 1, 2, 1 } };

    verifyEqual<Integer>(x, -1, {1, 2, 1});
    x.set(0, 3);
    verifyEqual<Integer>(x, -1, {1, 3, 1});
    x.set(1, 0);
    verifyEqual<Integer>(x, -1, {1, 3});
    x.set(0, 0);
    verifyEqual<Integer>(x, -1, {1});
    x.set(1, 0);
    verifyEqual<Integer>(x, -1, {1});
    x.set(-1, 0);
    verifyEqual<Integer>(x, 0, {});
    x.set(-1, 0);
    verifyEqual<Integer>(x, 0, {});
    x.set(2, 1);
    verifyEqual<Integer>(x, 2, {1});

    Laurent<Integer> y = { -1, { 1, 2, 1 } };
    y.set(-1, 0);
    verifyEqual<Integer>(y, 0, {2, 1});
    y.set(0, 0);
    verifyEqual<Integer>(y, 1, {1});
    y.set(-2, 3);
    verifyEqual<Integer>(y, -2, {3, 0, 0, 1});
    y.set(-2, 0);
    verifyEqual<Integer>(y, 1, {1});
    y.set(1, 0);
    verifyEqual<Integer>(y, 0, {});
}

TEST_F(LaurentTest, arithmetic) {
    verifyEqual<Integer>(zero, 0, {});
    verifyEqual<Integer>(zero2, 0, {});
    verifyEqual<Integer>(zero3, 0, {});

    verifyEqual<Integer>(-zero, 0, {});
    verifyEqual<Integer>(-a, -1, {-1, 1, -1});
    verifyEqual<Integer>(-c, 1, {-1, 1, -1});

    verifyPlus<Integer>(zero, zero, 0, {});
    verifyPlus<Integer>(one, two, 0, {3});
    verifyPlus<Integer>(a, zero, -1, {1, -1, 1});
    verifyPlus<Integer>(a, one, -1, {1, 0, 1});
    verifyPlus<Integer>(b, one, 0, {2, -1, 1});
    verifyPlus<Integer>(b, x2, 0, {1, -1, 2});
    verifyPlus<Integer>(a, b, -1, {1, 0, 0, 1});
    verifyPlus<Integer>(a, a + a, -1, {3, -3, 3});
    verifyPlus<Integer>(a, e, -1, {1, -1, 1, 0, 0, 2, 4, -2, 2});
    verifyPlus<Integer>(b, d, -2, {-1, 1, 0, 0, 1});
    verifyPlus<Integer>(c, d, -2, {-1, 1, -1, 2, -1, 1});

    verifyMinus<Integer>(zero, zero, 0, {});
    verifyMinus<Integer>(one, two, 0, {-1});
    verifyMinus<Integer>(a, zero, -1, {1, -1, 1});
    verifyMinus<Integer>(zero, a, -1, {-1, 1, -1});
    verifyMinus<Integer>(b, one, 1, {-1, 1});
    verifyMinus<Integer>(b, x2, 0, {1, -1});
    verifyMinus<Integer>(a, b, -1, {1, -2, 2, -1});
    verifyMinus<Integer>(b, a, -1, {-1, 2, -2, 1});
    verifyMinus<Integer>(a, a + a, -1, {-1, 1, -1});
    verifyMinus<Integer>(a, e, -1, {1, -1, 1, 0, 0, -2, -4, 2, -2});
    verifyMinus<Integer>(e, a, -1, {-1, 1, -1, 0, 0, 2, 4, -2, 2});
    verifyMinus<Integer>(b, d, -2, {1, -1, 2, -2, 1});
    verifyMinus<Integer>(d, b, -2, {-1, 1, -2, 2, -1});
    verifyMinus<Integer>(a, a, 0, {});

    verifyMult<Integer>(zero, 0, 0, {});
    verifyMult<Integer>(zero, 1, 0, {});
    verifyMult<Integer>(zero, 2, 0, {});
    verifyMult<Integer>(a, 0, 0, {});
    verifyMult<Integer>(a, 1, -1, {1, -1, 1});
    verifyMult<Integer>(a, -1, -1, {-1, 1, -1});
    verifyMult<Integer>(a, 2, -1, {2, -2, 2});

    verifyDiv<Integer>(zero, 1, 0, {});
    verifyDiv<Integer>(zero, 2, 0, {});
    verifyDiv<Integer>(a, 1, -1, {1, -1, 1});
    verifyDiv<Integer>(a, -1, -1, {-1, 1, -1});
    verifyDiv<Integer>(e, 2, 4, {1, 2, -1, 1});

    verifyMult<Integer>(zero, zero, 0, {});
    verifyMult<Integer>(zero, a, 0, {});
    verifyMult<Integer>(a, b, -1, {1, -2, 3, -2, 1});

    {
        Laurent<Integer> x(a);
        verifyEqual<Integer>(x + x, -1, {2, -2, 2});
        verifyEqual<Integer>(x += x, -1, {2, -2, 2});
    }
    {
        Laurent<Integer> x(a);
        verifyEqual<Integer>(x - x, 0, {});
        verifyEqual<Integer>(x -= x, 0, {});
    }
    {
        Laurent<Integer> x(-1, {1, 0, 1});
        verifyEqual<Integer>(x * x, -2, {1, 0, 2, 0, 1});
        verifyEqual<Integer>(x *= x, -2, {1, 0, 2, 0, 1});
    }
}

TEST_F(LaurentTest, tightEncoding) {
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(zero);
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(zero2);
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(zero3);
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(one);
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(two);
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(x2);
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(a);
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(b);
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(c);
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(d);
    TightEncodingTest<Laurent<Integer>>::verifyTightEncoding(e);
}
