
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

#include "maths/integer.h"
#include "maths/laurent.h"
#include "utilities/stringutils.h"
#include <array>
#include <ranges>

#include "utilities/tightencodingtest.h"

using regina::CoefficientDomain;
using regina::Integer;
using regina::Laurent;

class LaurentTest : public testing::Test {
    protected:
        using L = Laurent<Integer>;

        // An integer that cannot fit into 128 bits.
        // We split the digits into chunks to not break syntax highlighting.
        const Integer bigInt {
            "5421309874" "5789403215" "6654013103" "5798756432" "1035741817" };

        L zero {};
        L zero2 { 0, {} };
        L zero3 { 2, {} };
        L one { 0, { 1 } };
        L two { 0, { 2 } };
        L x2 { 2, { 1 } };
        L a { -1, { 1, -1, 1 } };
        L b { 0, { 1, -1, 1 } };
        L c { 1, { 1, -1, 1 } };
        L d { -2, { -1, 1, -1, 1 } };
        L e { 4, { 2, 4, -2, 2 } };
        L f { 3, { -1, 0, 0, 0, 1 } };
        L g { 20, { 2, -3 } };

        // Several ranges each of which overlap at a single coefficient:
        L low { -7, { 1, -2, 3, -4 } };
        L lowish { -4, { -1, 2, -3, 4 } };
        L mid { -1, { 1, -2, 3 } };
        L highish { 1, { 1, -2, -3, 4 } };
        L high { 4, { -1, 2, 3, -4 } };

        // The same polynomials as before, but this time forcing large integer
        // arithmetic:
        L bigLow { -7, { bigInt, bigInt * -2, bigInt * 3, bigInt * -4 } };
        L bigLowish { -4, { -bigInt, bigInt * 2, bigInt * -3, bigInt * 4 } };
        L bigMid { -1, { bigInt, bigInt * -2, bigInt * 3 } };
        L bigHighish { 1, { bigInt, bigInt * -2, bigInt * -3, bigInt * 4 } };
        L bigHigh { 4, { -bigInt, bigInt * 2, bigInt * 3, bigInt * -4 } };

        // Some polynomials with more space allocated than they need.
        L paddedZero = g - g;
        L paddedConst = (two + d) - d;
        L paddedPower = (x2 + highish) - highish;
        L paddedPoly = ((c + low + high) - low) - high;

        std::array<std::reference_wrapper<const L>, 27> cases {
            std::cref(zero), std::cref(zero2), std::cref(zero3), std::cref(one),
            std::cref(two), std::cref(x2), std::cref(a), std::cref(b),
            std::cref(c), std::cref(d), std::cref(e), std::cref(f),
            std::cref(g), std::cref(low), std::cref(lowish), std::cref(mid),
            std::cref(highish), std::cref(high), std::cref(bigLow),
            std::cref(bigLowish), std::cref(bigMid), std::cref(bigHighish),
            std::cref(bigHigh), std::cref(paddedZero), std::cref(paddedConst),
            std::cref(paddedPower), std::cref(paddedPoly) };

        static void validate(const L& poly) {
            auto alloc = poly.allocation();
            if (alloc.second == 0) {
                EXPECT_EQ(alloc.first, 0);
                EXPECT_TRUE(poly.isZero());
                EXPECT_EQ(poly.minExp(), 0);
                EXPECT_EQ(poly.maxExp(), 0);
                EXPECT_EQ(poly[0], 0);
            } else if (poly.isZero()) {
                // We have a zero polynomial but with memory pre-allocated.
                // In this scenario, alloc.first (the base) is arbitrary.
                EXPECT_EQ(poly.minExp(), 0);
                EXPECT_EQ(poly.maxExp(), 0);
                EXPECT_EQ(poly[0], 0);
            } else {
                EXPECT_LE(alloc.first, poly.minExp());
                EXPECT_LE(poly.minExp(), poly.maxExp());
                EXPECT_LT(poly.maxExp(),
                    alloc.first + static_cast<long>(alloc.second));
                EXPECT_NE(poly[poly.minExp()], 0);
                EXPECT_NE(poly[poly.maxExp()], 0);
            }
        }

        static void validateZero(const L& poly) {
            EXPECT_TRUE(poly.isZero());
            EXPECT_EQ(poly.minExp(), 0);
            EXPECT_EQ(poly.maxExp(), 0);
            EXPECT_EQ(poly[0], 0);

            auto alloc = poly.allocation();
            if (alloc.second == 0)
                EXPECT_EQ(alloc.first, 0);
            // If alloc.second is positive then we have a zero polynomial
            // but with memory pre-allocated, which means alloc.first
            // (the base exponent) is arbitrary.
        }

        static void validate(const L& poly, long minExp,
                std::initializer_list<Integer> coeffs) {
            if (coeffs.size() == 0) { // initializer_list does not have empty()
                EXPECT_TRUE(poly.isZero());
                EXPECT_EQ(poly.minExp(), 0);
                EXPECT_EQ(poly.maxExp(), 0);
                EXPECT_EQ(poly[0], 0);

                auto alloc = poly.allocation();
                if (alloc.second == 0)
                    EXPECT_EQ(alloc.first, 0);
                // If alloc.second is positive then we have a zero polynomial
                // but with memory pre-allocated, which means alloc.first
                // (the base exponent) is arbitrary.
            } else {
                EXPECT_FALSE(poly.isZero());
                EXPECT_EQ(poly.minExp(), minExp);
                EXPECT_EQ(poly.maxExp(), minExp + coeffs.size() - 1);

                auto alloc = poly.allocation();
                EXPECT_LE(alloc.first, minExp);
                EXPECT_GE(alloc.first + static_cast<long>(alloc.second),
                    minExp + static_cast<long>(coeffs.size()));

                EXPECT_NE(poly[poly.minExp()], 0);
                EXPECT_NE(poly[poly.maxExp()], 0);

                auto expect = coeffs.begin();
                for (long e = minExp; expect != coeffs.end(); ++e, ++expect)
                    EXPECT_EQ(poly[e], *expect);
            }
        }

        template <std::ranges::sized_range Container>
        static void validate(const L& poly, long minExp,
                const Container& coeffs) {
            if (coeffs.empty()) {
                EXPECT_TRUE(poly.isZero());
                EXPECT_EQ(poly.minExp(), 0);
                EXPECT_EQ(poly.maxExp(), 0);
                EXPECT_EQ(poly[0], 0);

                auto alloc = poly.allocation();
                if (alloc.second == 0)
                    EXPECT_EQ(alloc.first, 0);
                // If alloc.second is positive then we have a zero polynomial
                // but with memory pre-allocated, which means alloc.first
                // (the base exponent) is arbitrary.
            } else {
                EXPECT_FALSE(poly.isZero());
                EXPECT_EQ(poly.minExp(), minExp);
                EXPECT_EQ(poly.maxExp(), minExp + coeffs.size() - 1);

                auto alloc = poly.allocation();
                EXPECT_LE(alloc.first, minExp);
                EXPECT_GE(alloc.first + alloc.second, minExp + coeffs.size());

                EXPECT_NE(poly[poly.minExp()], 0);
                EXPECT_NE(poly[poly.maxExp()], 0);

                auto expect = coeffs.begin();
                for (long e = minExp; expect != coeffs.end(); ++e, ++expect)
                    EXPECT_EQ(poly[e], *expect);
            }
        }

        static void validate(const L& poly, const L& expect) {
            if (expect.isZero()) {
                EXPECT_TRUE(poly.isZero());
                EXPECT_EQ(poly.minExp(), 0);
                EXPECT_EQ(poly.maxExp(), 0);
                EXPECT_EQ(poly[0], 0);

                auto alloc = poly.allocation();
                if (alloc.second == 0)
                    EXPECT_EQ(alloc.first, 0);
                // If alloc.second is positive then we have a zero polynomial
                // but with memory pre-allocated, which means alloc.first
                // (the base exponent) is arbitrary.
            } else {
                EXPECT_FALSE(poly.isZero());
                EXPECT_EQ(poly.minExp(), expect.minExp());
                EXPECT_EQ(poly.maxExp(), expect.maxExp());

                auto alloc = poly.allocation();
                EXPECT_LE(alloc.first, poly.minExp());
                EXPECT_GT(alloc.first + static_cast<long>(alloc.second),
                    poly.maxExp());

                EXPECT_NE(poly[poly.minExp()], 0);
                EXPECT_NE(poly[poly.maxExp()], 0);

                for (long e = expect.minExp(); e != expect.maxExp(); ++e)
                    EXPECT_EQ(poly[e], expect[e]);
            }

        }

        static L padded(const L& poly) {
            L ans(poly);
            ans.set(ans.minExp() - 3, 1);
            ans.set(ans.maxExp() + 2, 1);
            ans.set(ans.minExp(), 0);
            ans.set(ans.maxExp(), 0);
            return ans;
        }

        // TODO: HERE

        template <CoefficientDomain T>
        static void verifyEqual(const Laurent<T>& result,
                long minExp, std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(result);

            Laurent<T> expect(minExp, coeffs);
            SCOPED_TRACE_REGINA(expect);

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

        template <CoefficientDomain T>
        void verifyPlus(const Laurent<T>& x, const Laurent<T>& y,
                long minExp, std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

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

        template <CoefficientDomain T>
        void verifyMinus(const Laurent<T>& x, const Laurent<T>& y,
                long minExp, std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

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

        template <CoefficientDomain T>
        void verifyMult(const Laurent<T>& x, const T& y,
                long minExp, std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x * y, minExp, coeffs);
            verifyEqual((x + zero) * y, minExp, coeffs);
            verifyEqual(y * x, minExp, coeffs);
            verifyEqual(y * (x + zero), minExp, coeffs);
            {
                Laurent<T> z(x);
                verifyEqual(z *= y, minExp, coeffs);
            }
        }

        template <CoefficientDomain T, regina::CppInteger U>
        void verifyMult(const Laurent<T>& x, U y,
                long minExp, std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_INTEGER(y);

            verifyEqual(x * y, minExp, coeffs);
            verifyEqual((x + zero) * y, minExp, coeffs);
            verifyEqual(y * x, minExp, coeffs);
            verifyEqual(y * (x + zero), minExp, coeffs);
            {
                Laurent<T> z(x);
                verifyEqual(z *= y, minExp, coeffs);
            }

            verifyMult(x, T(y), minExp, coeffs);
        }

        template <CoefficientDomain T>
        void verifyDiv(const Laurent<T>& x, const T& y,
                long minExp, std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x / y, minExp, coeffs);
            verifyEqual((x + zero) / y, minExp, coeffs);
            {
                Laurent<T> z(x);
                verifyEqual(z /= y, minExp, coeffs);
            }
        }

        template <CoefficientDomain T>
        void verifyMult(const Laurent<T>& x, const Laurent<T>& y,
                long minExp, std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

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

        template <CoefficientDomain T>
        void verifyMultAllAlgorithms(const Laurent<T>& x, const Laurent<T>& y,
                long minExp, std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            using PPA = regina::PolynomialProductAlgorithm;
            using Array = regina::FixedArray<T>;

            Array expect(coeffs);
            EXPECT_NO_THROW({
                EXPECT_EQ(regina::polynomialProduct<PPA::Default>(
                    Array(x.begin(), x.end()), Array(y.begin(), y.end())),
                    coeffs);
                EXPECT_EQ(regina::polynomialProduct<PPA::Classic>(
                    Array(x.begin(), x.end()), Array(y.begin(), y.end())),
                    coeffs);
                EXPECT_EQ(regina::polynomialProduct<PPA::Karatsuba>(
                    Array(x.begin(), x.end()), Array(y.begin(), y.end())),
                    coeffs);
                EXPECT_EQ(regina::polynomialProduct<PPA::Default>(
                    Array(y.begin(), y.end()), Array(x.begin(), x.end())),
                    coeffs);
                EXPECT_EQ(regina::polynomialProduct<PPA::Classic>(
                    Array(y.begin(), y.end()), Array(x.begin(), x.end())),
                    coeffs);
                EXPECT_EQ(regina::polynomialProduct<PPA::Karatsuba>(
                    Array(y.begin(), y.end()), Array(x.begin(), x.end())),
                    coeffs);
            });
        }
};

TEST_F(LaurentTest, construct) {
    validateZero(L());
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        L x(c);
        validate(x, c);
        validate(L(std::move(x)), c);
        validate(L(c.minExp(), c.begin(), c.end()), c);
        // TODO: Iterator sequences with zeroes, inc. all zeroes
    }
    {
        Integer zero(0), pos(4), neg(-4), big(bigInt);
        validateZero(L(zero));
        validateZero(L(std::move(zero)));
        validateZero(L(0));
        validate(L(pos), 0, { 4 });
        validate(L(std::move(pos)), 0, { 4 });
        validate(L(4), 0, { 4 });
        validate(L(neg), 0, { -4 });
        validate(L(std::move(neg)), 0, { -4 });
        validate(L(-4), 0, { -4 });
        std::array<Integer, 1> bigConst { big };
        validate(L(big), 0, bigConst);
        validate(L(std::move(big)), 0, bigConst);
    }
}

TEST_F(LaurentTest, padding) {
    {
        auto alloc = paddedZero.allocation();
        EXPECT_GT(alloc.second, 0);
    }
    {
        // paddedConst: exponent range [0]
        auto alloc = paddedConst.allocation();
        EXPECT_LT(alloc.first, 0);
        EXPECT_GT(alloc.first + static_cast<long>(alloc.second), 1);
    }
    {
        // paddedPower: exponent range [2]
        auto alloc = paddedPower.allocation();
        EXPECT_LT(alloc.first, 2);
        EXPECT_GT(alloc.first + static_cast<long>(alloc.second), 3);
    }
    {
        // paddedPoly: exponent range [1..3]
        auto alloc = paddedPoly.allocation();
        EXPECT_LT(alloc.first, 1);
        EXPECT_GT(alloc.first + static_cast<long>(alloc.second), 4);
    }
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        L x = padded(c);
        validate(x, c);

        auto alloc = x.allocation();
        EXPECT_GT(alloc.second, 0);
        EXPECT_LT(alloc.first, x.minExp());
        EXPECT_GT(alloc.first + static_cast<long>(alloc.second),
            x.maxExp() + 1);
    }
}

TEST_F(LaurentTest, init) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        {
            L x(c);
            x.init();
            validateZero(x);
        }
        {
            L x(c);
            x.initExp(-2);
            validate(x, -2, { 1 });
        }
    }
}

// TODO: HERE

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
    x.set(-1, 3);
    verifyEqual<Integer>(x, -1, {3});
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

    verifyMultAllAlgorithms<Integer>(
        Laurent<Integer>(2, {1,1,1,1,1,1,1,1,1,1,1,1,1}),
        Laurent<Integer>(3, {-1,0,0,0,0,0,0,0,0,0,1}),
        5, {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,1,1,1,1,1,1,1,1,1,1});

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

TEST_F(LaurentTest, ringConstants) {
    // Verify that the RingTraits constants looks correct.
    EXPECT_EQ(regina::RingTraits<Laurent<Integer>>::zero.str(), "0");
    EXPECT_EQ(regina::RingTraits<Laurent<Integer>>::one.str(), "1");
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
