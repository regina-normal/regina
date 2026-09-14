
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
using regina::polynomialProduct;

using L = Laurent<Integer>;

class LaurentTest : public testing::Test {
    protected:
        // An integer that cannot fit into 128 bits.
        // We split the digits into chunks to not break syntax highlighting.
        const Integer bigInt {
            "5421309874" "5789403215" "6654013103" "5798756432" "1035741817" };

        L zero {};
        L zero2 { 0, {} };
        L zero3 { 2, {} };
        L one { 0, { 1 } };
        L two { 0, { 2 } };
        L minusOne { 0, { -1 } };
        L x2 { 2, { 1 } };
        L x5Inv { -5, { 1 } };
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

        // Some longer polynomials:
        L long1 { 2, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };
        L long2 { 3, { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 } };

        // Some polynomials with more space allocated than they need:
        L paddedZero = g - g;
        L paddedConst = (two + d) - d;
        L paddedPower = (x2 + highish) - highish;
        L paddedPoly = ((c + low + high) - low) - high;

        std::array<std::reference_wrapper<const L>, 31> cases {
            std::cref(zero), std::cref(zero2), std::cref(zero3), std::cref(one),
            std::cref(two), std::cref(minusOne), std::cref(x2),
            std::cref(x5Inv), std::cref(a), std::cref(b), std::cref(c),
            std::cref(d), std::cref(e), std::cref(f), std::cref(g),
            std::cref(low), std::cref(lowish), std::cref(mid),
            std::cref(highish), std::cref(high), std::cref(bigLow),
            std::cref(bigLowish), std::cref(bigMid), std::cref(bigHighish),
            std::cref(bigHigh), std::cref(long1), std::cref(long2),
            std::cref(paddedZero), std::cref(paddedConst),
            std::cref(paddedPower), std::cref(paddedPoly) };

        // TODO: Replace the verify... routines below.

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
};

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
    EXPECT_EQ(poly, 0);

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
static void validate(const L& poly, long minExp, const Container& coeffs) {
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
    EXPECT_EQ(poly, expect);
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

/**
 * Returns a polynomial equal to \a poly, but with more space for
 * coefficients allocated on each end than is necessary.
 */
static L padded(const L& poly) {
    // Note: this still works if poly is the zero polynomial (in which
    // case the first call to set() changes _both_ minExp and maxExp).
    L ans(poly);
    ans.set(ans.minExp() - 3, 1); // changes minExp
    ans.set(ans.maxExp() + 2, 1); // changes maxExp
    ans.set(ans.minExp(), 0);
    ans.set(ans.maxExp(), 0);
    return ans;
}

TEST_F(LaurentTest, construct) {
    validateZero(L());
    validateZero(zero);
    validateZero(zero2);
    validateZero(zero3);
    validateZero(paddedZero);

    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        {
            L x(c);
            validate(x, c);
            validate(L(std::move(x)), c);
        }
        validate(L(c.minExp(), c.begin(), c.end()), c);
        {
            // This test (for iterator ranges with zero padding at either end)
            // also covers the case where the iterator range contains entirely
            // zeroes (this is tested when c is the zero polynomial).
            std::vector<Integer> coeffs(3); // some initial zeroes
            coeffs.insert(coeffs.end(), c.begin(), c.end());
            coeffs.resize(coeffs.size() + 2); // some final zeroes
            EXPECT_FALSE(coeffs.empty());
            EXPECT_EQ(coeffs.front(), 0);
            EXPECT_EQ(coeffs.back(), 0);
            validate(L(c.minExp() - 3, coeffs.begin(), coeffs.end()), c);
        }
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
        {
            L x = padded(c);
            validate(x, c);

            auto alloc = x.allocation();
            EXPECT_GT(alloc.second, 0);
            if (c.isZero()) {
                EXPECT_TRUE(x.isZero());
                // In this case, alloc.first is arbitrary.
            } else {
                EXPECT_LT(alloc.first, x.minExp());
                EXPECT_GT(alloc.first + static_cast<long>(alloc.second),
                    x.maxExp() + 1);
            }
        }
        {
            L x(c);
            if (c.isZero()) {
                L x(c);
                EXPECT_EQ(x.allocation().second, 0);

                x.reserveRange(1000, 999);
                EXPECT_EQ(x.allocation().second, 0);

                // TODO: Finish this: alloc and set for non-trivial ranges
            } else {
                auto alloc = x.allocation();
                EXPECT_EQ(alloc.first, x.minExp());
                EXPECT_EQ(alloc.second, x.maxExp() - x.minExp() + 1);
                const auto initAlloc = alloc;

                x.reserveRange(x.minExp(), x.minExp());
                EXPECT_EQ(x.allocation(), initAlloc);

                x.reserveRange(x.maxExp(), x.maxExp());
                EXPECT_EQ(x.allocation(), initAlloc);

                x.reserveRange((x.minExp() + x.maxExp()) / 2, x.maxExp());
                EXPECT_EQ(x.allocation(), initAlloc);

                x.reserveRange(1000, 999);
                EXPECT_EQ(x.allocation(), initAlloc);

                // TODO: Finish this: alloc and set for non-trivial ranges
            }
        }
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

TEST_F(LaurentTest, moveThenAssign) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            {
                L x(c);
                L y = std::move(x);
                x = d;
                validate(x, d);
                validate(y, c);
            }
            {
                L x(c);
                L y = std::move(x);
                x = 3;
                validate(x, L(3));
                validate(y, c);
            }
            {
                L x(c);
                L y = std::move(x);
                x = bigInt;
                validate(x, L(bigInt));
                validate(y, c);
            }
            {
                L x(c);
                L y = std::move(x);
                Integer i(bigInt);
                x = std::move(i);
                validate(x, L(bigInt));
                validate(y, c);
            }
            // Test move-then-init also.
            {
                L x(c);
                L y = std::move(x);
                x.init();
                validateZero(x);
                validate(y, c);
            }
            {
                L x(c);
                L y = std::move(x);
                x.initExp(-5);
                validate(x, x5Inv);
                validate(y, c);
            }
        }
    }
}

TEST_F(LaurentTest, set) {
    // TODO: This test is to be replaced
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

// TODO: [], set()
// TODO: iterators
// TODO: == (Laurent, Integer, int), <=>
// TODO: assign (Laurent, Laurent&&, Integer, Integer&&, int)
// TODO: shift, shifted, shifted &&
// TODO: scaleUp, scaleDown

TEST_F(LaurentTest, negate) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        if (c.isZero()) {
            validateZero(-c);

            L x(c);
            x.negate();
            validateZero(x);
            validateZero(-c);
            validateZero(-std::move(x));
        } else {
            L neg = -c;
            validate(neg);
            EXPECT_FALSE(neg.isZero());
            EXPECT_EQ(neg.minExp(), c.minExp());
            EXPECT_EQ(neg.maxExp(), c.maxExp());
            for (long i = c.minExp(); i <= c.maxExp(); ++i)
                EXPECT_EQ(neg[i], -c[i]);

            {
                L x(c);
                validate(-std::move(x), neg);
            }
            {
                L x(c);
                x.negate();
                validate(x, neg);
            }

            validateZero(c + neg);
        }
    }
}

// TODO: invertX
// TODO: transform (with, without exponents), extract

template <regina::CppInteger Native>
static void verifyNativeProduct(const L& poly, const Integer& scalar,
        const L& product) {
    SCOPED_TRACE_TYPE(Native);

    Native native;
    if (scalar >= std::numeric_limits<Native>::min() &&
            scalar <= std::numeric_limits<Native>::max()) {
        EXPECT_NO_THROW({ native = scalar.safeValue<Native>(); });
    } else {
        EXPECT_THROW({ scalar.safeValue<Native>(); }, regina::IntegerOverflow);
        return;
    }

    validate(poly * native, product);
    validate(L(poly) * native, product);
    validate(native * poly, product);
    validate(native * L(poly), product);
    {
        L x(poly);
        validate(x *= native, product);
    }
}

template <regina::CppInteger Native>
static void verifyNativeQuotient(const L& poly, const Integer& scalar,
        const std::optional<L>& quotient) {
    SCOPED_TRACE_TYPE(Native);

    Native native;
    if (scalar >= std::numeric_limits<Native>::min() &&
            scalar <= std::numeric_limits<Native>::max()) {
        EXPECT_NO_THROW({ native = scalar.safeValue<Native>(); });
    } else {
        EXPECT_THROW({ scalar.safeValue<Native>(); }, regina::IntegerOverflow);
        return;
    }

    if (quotient) {
        validate(poly / native, *quotient);
        validate(L(poly) / native, *quotient);
        {
            L x(poly);
            validate(x /= native, *quotient);
        }
    } else {
        #if 0 // For now, Laurent division requires a non-zero divisor.
        EXPECT_THROW({ poly / native; }, regina::DivisionByZero);
        EXPECT_THROW({ L(poly) / native; }, regina::DivisionByZero);
        {
            L x(poly);
            EXPECT_THROW({ x /= native; }, regina::DivisionByZero);
        }
        #endif
    }
}

static void verifyScalarMultiplyDivide(const L& poly, const Integer& scalar) {
    SCOPED_TRACE_REGINA(scalar);

    const L product = poly * scalar;
    if (poly.isZero() || scalar == 0) {
        validateZero(product);
    } else {
        validate(product);
        EXPECT_FALSE(product.isZero());
        EXPECT_EQ(product.minExp(), poly.minExp());
        EXPECT_EQ(product.maxExp(), poly.maxExp());
        for (long i = poly.minExp(); i <= poly.maxExp(); ++i)
            EXPECT_EQ(product[i], poly[i] * scalar);
    }
    validate(L(poly) * scalar, product);
    validate(scalar * poly, product);
    validate(scalar * L(poly), product);
    {
        L x(poly);
        validate(x *= scalar, product);
    }

    verifyNativeProduct<int8_t>(poly, scalar, product);
    verifyNativeProduct<int16_t>(poly, scalar, product);
    verifyNativeProduct<int32_t>(poly, scalar, product);
    verifyNativeProduct<int64_t>(poly, scalar, product);
    #ifdef INT128_AVAILABLE
    verifyNativeProduct<regina::Int128>(poly, scalar, product);
    #endif

    if (scalar.sign() >= 0) {
        verifyNativeProduct<uint8_t>(poly, scalar, product);
        verifyNativeProduct<uint16_t>(poly, scalar, product);
        verifyNativeProduct<uint32_t>(poly, scalar, product);
        verifyNativeProduct<uint64_t>(poly, scalar, product);
        #ifdef INT128_AVAILABLE
        verifyNativeProduct<regina::UInt128>(poly, scalar, product);
        #endif
    }

    if (scalar == 0) {
        #if 0 // For now, Laurent division requires a non-zero divisor.
        EXPECT_THROW({ poly / scalar; }, regina::DivisionByZero);
        EXPECT_THROW({ L(poly) / scalar; }, regina::DivisionByZero);
        {
            L x(poly);
            EXPECT_THROW({ x /= scalar; }, regina::DivisionByZero);
        }

        verifyNativeQuotient<int8_t>(poly, scalar, {});
        verifyNativeQuotient<int16_t>(poly, scalar, {});
        verifyNativeQuotient<int32_t>(poly, scalar, {});
        verifyNativeQuotient<int64_t>(poly, scalar, {});
        #ifdef INT128_AVAILABLE
        verifyNativeQuotient<regina::Int128>(poly, scalar, {});
        #endif

        verifyNativeQuotient<uint8_t>(poly, scalar, {});
        verifyNativeQuotient<uint16_t>(poly, scalar, {});
        verifyNativeQuotient<uint32_t>(poly, scalar, {});
        verifyNativeQuotient<uint64_t>(poly, scalar, {});
        #ifdef INT128_AVAILABLE
        verifyNativeQuotient<regina::UInt128>(poly, scalar, {});
        #endif
        #endif
    } else {
        const L quotient = poly / scalar;
        if (poly.isZero()) {
            validateZero(quotient);
        } else {
            validate(quotient);
            if (poly[poly.minExp()].abs() >= scalar.abs()) {
                EXPECT_FALSE(quotient.isZero());
                EXPECT_EQ(quotient.minExp(), poly.minExp());
            } else {
                if (! quotient.isZero())
                    EXPECT_GT(quotient.minExp(), poly.minExp());
            }
            if (poly[poly.maxExp()].abs() >= scalar.abs()) {
                EXPECT_FALSE(quotient.isZero());
                EXPECT_EQ(quotient.maxExp(), poly.maxExp());
            } else {
                if (! quotient.isZero())
                    EXPECT_LT(quotient.maxExp(), poly.maxExp());
            }
            for (long i = poly.minExp(); i <= poly.maxExp(); ++i)
                EXPECT_EQ(quotient[i], poly[i] / scalar);
        }
        validate(L(poly) / scalar, quotient);
        {
            L x(poly);
            validate(x /= scalar, quotient);
        }

        verifyNativeQuotient<int8_t>(poly, scalar, quotient);
        verifyNativeQuotient<int16_t>(poly, scalar, quotient);
        verifyNativeQuotient<int32_t>(poly, scalar, quotient);
        verifyNativeQuotient<int64_t>(poly, scalar, quotient);
        #ifdef INT128_AVAILABLE
        verifyNativeQuotient<regina::Int128>(poly, scalar, quotient);
        #endif

        if (scalar.sign() >= 0) {
            verifyNativeQuotient<uint8_t>(poly, scalar, quotient);
            verifyNativeQuotient<uint16_t>(poly, scalar, quotient);
            verifyNativeQuotient<uint32_t>(poly, scalar, quotient);
            verifyNativeQuotient<uint64_t>(poly, scalar, quotient);
            #ifdef INT128_AVAILABLE
            verifyNativeQuotient<regina::UInt128>(poly, scalar, quotient);
            #endif
        }
    }
}

TEST_F(LaurentTest, scalarMultiplyDivide) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        for (int i = -3; i <= 3; ++i)
            verifyScalarMultiplyDivide(c, i);

        // Run through boundaries and "midpoints" for {8,16,32,64,128}-bit ints:
        Integer pow2 = 256; // 2^8
        Integer halfPow2 = 128;
        Integer quarterPow2 = 64;
        for (int i = 0; i < 5; ++i) {
            // pow2 = 2^bits = 2^{2^b} for some b
            // halfPow2 = 2^(bits-1)
            // quarterPow2 = 2^(bits-2)
            for (int j = -1; j <= 1; ++j) {
                verifyScalarMultiplyDivide(c, pow2 + j);
                verifyScalarMultiplyDivide(c, -pow2 + j);
                verifyScalarMultiplyDivide(c, halfPow2 + j);
                verifyScalarMultiplyDivide(c, -halfPow2 + j);
                verifyScalarMultiplyDivide(c, quarterPow2 + j);
                verifyScalarMultiplyDivide(c, -quarterPow2 + j);
            }
            // Double the number of bits in pow2:
            quarterPow2 *= pow2;
            halfPow2 *= pow2;
            pow2 *= pow2;
        }

        verifyScalarMultiplyDivide(c, bigInt);
        verifyScalarMultiplyDivide(c, -bigInt);

        // Test expected arithmetic properties:
        validateZero(c * 0);
        validate(c * 1, c);
        validate(c * -1, -c);
        validate(c * 2, c + c);

        if (! c.isZero()) {
            verifyScalarMultiplyDivide(c, c[c.minExp()]);
            verifyScalarMultiplyDivide(c, c[c.maxExp()]);

            L c0 = c / c[c.minExp()];
            EXPECT_EQ(c0.minExp(), c.minExp());
            EXPECT_EQ(c0[c0.minExp()], 1);

            L c1 = c / c[c.maxExp()];
            EXPECT_EQ(c1.maxExp(), c.maxExp());
            EXPECT_EQ(c1[c1.maxExp()], 1);
        }
    }
}

TEST_F(LaurentTest, addSubtract) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);

            const L sum = c + d;
            if (sum.isZero()) {
                validateZero(sum);
                validate(-c, d);
            } else if (c.isZero()) {
                validate(sum, d);
            } else if (d.isZero()) {
                validate(sum, c);
            } else {
                validate(sum);
                if (c.minExp() == d.minExp())
                    EXPECT_GE(sum.minExp(), c.minExp());
                else
                    EXPECT_EQ(sum.minExp(), std::min(c.minExp(), d.minExp()));
                if (c.maxExp() == d.maxExp())
                    EXPECT_LE(sum.maxExp(), c.maxExp());
                else
                    EXPECT_EQ(sum.maxExp(), std::max(c.maxExp(), d.maxExp()));
                for (long i = std::min(c.minExp(), d.minExp());
                        i <= std::max(c.maxExp(), d.maxExp()); ++i)
                    EXPECT_EQ(sum[i], c[i] + d[i]);
            }
            validate(c + L(d), sum);
            validate(L(c) + d, sum);
            validate(L(c) + L(d), sum);
            {
                L x(c);
                validate(x += d, sum);
            }
            {
                L x(c);
                validate(x += L(d), sum);
            }

            const L diff = c - d;
            if (diff.isZero()) {
                validateZero(diff);
                validate(c, d);
            } else if (d.isZero()) {
                validate(diff, c);
            } else if (c.isZero()) {
                validate(diff);
                EXPECT_EQ(diff.minExp(), d.minExp());
                EXPECT_EQ(diff.maxExp(), d.maxExp());
                for (long i = d.minExp(); i <= d.maxExp(); ++i)
                    EXPECT_EQ(diff[i], - d[i]);
            } else {
                validate(diff);
                if (c.minExp() == d.minExp())
                    EXPECT_GE(diff.minExp(), c.minExp());
                else
                    EXPECT_EQ(diff.minExp(), std::min(c.minExp(), d.minExp()));
                if (c.maxExp() == d.maxExp())
                    EXPECT_LE(diff.maxExp(), c.maxExp());
                else
                    EXPECT_EQ(diff.maxExp(), std::max(c.maxExp(), d.maxExp()));
                for (long i = std::min(c.minExp(), d.minExp());
                        i <= std::max(c.maxExp(), d.maxExp()); ++i)
                    EXPECT_EQ(diff[i], c[i] - d[i]);
            }
            validate(c - L(d), diff);
            validate(L(c) - d, diff);
            validate(L(c) - L(d), diff);
            {
                L x(c);
                validate(x -= d, diff);
            }
            {
                L x(c);
                validate(x -= L(d), diff);
            }

            // Test expected arithmetic properties:
            validate((c + d) - d, c);
            validate((c + d) - c, d);
            validate(c - d, -(d - c));
            validate((c - d) + d, c);
            validate(-((c - d) - c), d);
            validate(-(c - d) + c, d);
            validate(c + (-d), c - d);
            validate(c - (-d), c + d);
        }

        // Test operations upon one's self:
        const L dbl = c * 2;
        if (c.isZero()) {
            validateZero(dbl);
        } else {
            validate(dbl);
            EXPECT_FALSE(dbl.isZero());
            EXPECT_EQ(dbl.minExp(), c.minExp());
            EXPECT_EQ(dbl.maxExp(), c.maxExp());
            for (long i = c.minExp(); i <= c.maxExp(); ++i)
                EXPECT_EQ(dbl[i], c[i] * 2);
        }
        validate(c + c, dbl);
        {
            L x(c);
            validate(x += x, dbl);
        }
        validateZero(c - c);
        {
            L x(c);
            validateZero(x -= x);
        }

        // More expected arithmetic properties:
        validate(c + zero, c);
        validate(zero + c, c);
        validate(c - zero, c);
        validate(zero - c, -c);
        validateZero(c + -c);
    }
}

TEST_F(LaurentTest, multiply) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);

            if (c.isZero() || d.isZero()) {
                validateZero(c * d);
                validateZero(c * L(d));
                validateZero(L(c) * d);
                validateZero(L(c) * L(d));
                {
                    L x(c);
                    validateZero(x *= d);
                }
                {
                    L x(c);
                    validateZero(x *= L(d));
                }
            } else {
                const L product = c * d;
                validate(product);
                EXPECT_FALSE(product.isZero());
                EXPECT_EQ(product.minExp(), c.minExp() + d.minExp());
                EXPECT_EQ(product.maxExp(), c.maxExp() + d.maxExp());
                for (long i = product.minExp(); i <= product.maxExp(); ++i) {
                    long cExp = std::max(c.minExp(), i - d.maxExp());
                    long dExp = i - cExp;
                    Integer coeff;
                    while (cExp <= c.maxExp() && dExp >= d.minExp())
                        coeff.addProduct(c[cExp++], d[dExp--]);
                    EXPECT_EQ(product[i], coeff);
                }

                validate(c * L(d), product);
                validate(L(c) * d, product);
                validate(L(c) * L(d), product);
                {
                    L x(c);
                    validate(x *= d, product);
                }
                {
                    L x(c);
                    validate(x *= L(d), product);
                }

                {
                    L x(c);
                    x.addProduct(d, d);
                    validate(x, c + d * d);
                }
                {
                    // Compute the expected result of c -> c + c * d:
                    const L expect = c * (d + L(1));
                    {
                        L x(c);
                        x.addProduct(x, d);
                        validate(x, expect);
                    }
                    {
                        L x(c);
                        x.addProduct(d, x);
                        validate(x, expect);
                    }
                }

                // Test expected arithmetic properties:
                validate(c * d, d * c);

                // Verify that our different multiplication algorithms
                // give the same results:
                using PPA = regina::PolynomialProductAlgorithm;
                using Array = regina::FixedArray<Integer>;

                auto def = polynomialProduct<PPA::Default>(
                    Array(c.begin(), c.end()),
                    Array(d.begin(), d.end()));
                auto classic = polynomialProduct<PPA::Classic>(
                    Array(c.begin(), c.end()),
                    Array(d.begin(), d.end()));
                EXPECT_EQ(def, classic);

                long cSpan = c.maxExp() - c.minExp();
                long dSpan = d.maxExp() - d.minExp();
                if (cSpan >= 1 && dSpan >= 1 &&
                        (2 * cSpan <= dSpan || 2 * dSpan <= cSpan)) {
                    EXPECT_THROW({
                        polynomialProduct<PPA::Karatsuba>(
                            Array(c.begin(), c.end()),
                            Array(d.begin(), d.end()));
                    }, regina::InvalidArgument);
                } else {
                    EXPECT_NO_THROW({
                        auto karatsuba = polynomialProduct<PPA::Karatsuba>(
                            Array(c.begin(), c.end()),
                            Array(d.begin(), d.end()));
                        EXPECT_EQ(def, karatsuba);
                    });
                }
            }

            for (const L& e : cases) {
                SCOPED_TRACE_REGINA(e);
                {
                    L x(c);
                    x.addProduct(d, e);
                    validate(x, c + d * e);
                }
                {
                    L x(c);
                    x.addProduct(L(d), L(e));
                    validate(x, c + d * e);
                }
            }
        }

        // Test operations upon one's self:
        const L copy(c);
        const L square = c * copy;
        if (c.isZero()) {
            validateZero(square);
        } else {
            validate(square);
            EXPECT_FALSE(square.isZero());
            EXPECT_EQ(square.minExp(), 2 * c.minExp());
            EXPECT_EQ(square.maxExp(), 2 * c.maxExp());
            for (long i = square.minExp(); i <= square.maxExp(); ++i) {
                long lhsExp = std::max(c.minExp(), i - c.maxExp());
                long rhsExp = i - lhsExp;
                Integer coeff;
                while (lhsExp <= c.maxExp() && rhsExp >= c.minExp())
                    coeff.addProduct(c[lhsExp++], c[rhsExp--]);
                EXPECT_EQ(square[i], coeff);
            }
        }
        validate(c * c, square);
        {
            L x(c);
            validate(x *= x, square);
        }

        {
            L x(c);
            x.addProduct(x, x);
            validate(x, c + square);
        }

        // More expected arithmetic properties:
        validate(c * one, c);
        validate(one * c, c);
        validate(c * minusOne, -c);
        validate(minusOne * c, -c);
        validate(c * x2, c.shifted(2));
        validate(x2 * c, c.shifted(2));
        validate(c * x5Inv, c.shifted(-5));
        validate(x5Inv * c, c.shifted(-5));
    }
}

// TODO: str, utf8

TEST_F(LaurentTest, ringConstants) {
    // Verify that the RingTraits constants looks correct.
    EXPECT_EQ(regina::RingTraits<L>::zero.str(), "0");
    EXPECT_EQ(regina::RingTraits<L>::one.str(), "1");
}

TEST_F(LaurentTest, tightEncoding) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        TightEncodingTest<L>::verifyTightEncoding(c);
    }
}

TEST_F(LaurentTest, swap) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            {
                L x(c);
                L y(d);
                x.swap(y);
                validate(x, d);
                validate(y, c);
            }
            {
                L x(c);
                L y(d);
                swap(x, y);
                validate(x, d);
                validate(y, c);
            }
        }
    }
}

