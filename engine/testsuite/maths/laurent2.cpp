
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
#include "maths/laurent2.h"
#include "utilities/stringutils.h"
#include <array>

#include "utilities/tightencodingtest.h"

using regina::Integer;
using regina::Laurent2;

using L = Laurent2<Integer>;
using Term = std::pair<std::pair<long, long>, Integer>;

// An integer that cannot fit into 128 bits.
// We split the digits into chunks to not break syntax highlighting.
#define BIG "5421309874" "5789403215" "6654013103" "5798756432" "1035741817"

class Laurent2Test : public testing::Test {
    protected:
        // An integer that cannot fit into 128 bits.
        const Integer bigInt { BIG };

        const L zero {};
        const L one { {0, 0, 1} };
        const L two { {0, 0, 2} };
        const L minusOne { {0, 0, -1} };
        const L x2 { {2, 0, 1} };
        const L xy { {1, 1, 1} };
        const L xy2 { {1, 2, 1} };
        const L a { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} };
        const L b { {1, 0, 2}, {1, 2, -4}, {0, -1, -2} };

        const std::array<std::reference_wrapper<const L>, 9> cases {
            std::cref(zero), std::cref(one), std::cref(two),
            std::cref(minusOne),
            std::cref(x2), std::cref(xy), std::cref(xy2),
            std::cref(a), std::cref(b) };
};

static void validate(const L& poly) {
    EXPECT_NO_THROW({ poly.validate(); });
}

static void validate(const L& poly,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    // We assume here that coeffs only contains non-zero coefficients, and
    // does not contain any duplicate exponent pairs.

    EXPECT_NO_THROW({ poly.validate(); });
    EXPECT_EQ(poly.countTerms(), coeffs.size());
    if (coeffs.size() == 0) {
        EXPECT_TRUE(poly.isZero());
    } else {
        EXPECT_FALSE(poly.isZero());
        for (const auto& c : coeffs)
            EXPECT_EQ(poly(std::get<0>(c), std::get<1>(c)), std::get<2>(c));
    }
}

static void validateZero(const L& poly) {
    EXPECT_NO_THROW({ poly.validate(); });
    EXPECT_TRUE(poly.isZero());
    EXPECT_EQ(poly.countTerms(), 0);
    EXPECT_EQ(poly.begin(), poly.end());
}

static void validate(const L& poly, const L& expect) {
    if (expect.isZero())
        validateZero(poly);
    else {
        EXPECT_NO_THROW({ poly.validate(); });
        EXPECT_FALSE(poly.isZero());
        EXPECT_EQ(poly.countTerms(), expect.countTerms());

        auto pit = poly.begin();
        auto eit = expect.begin();
        while (pit != poly.end() && eit != expect.end())
            EXPECT_EQ(*pit++, *eit++);
        EXPECT_EQ(pit, poly.end());
        EXPECT_EQ(eit, expect.end());
    }

    EXPECT_EQ(poly, expect);
    EXPECT_FALSE(poly != expect);
}

template <std::regular_invocable<const Term&> TranslateTerm>
requires requires(TranslateTerm trans, const Term t) {
    { trans(t) } -> std::convertible_to<Term>;
}
static void validate(const L& poly, const L& from, TranslateTerm&& translate) {
    // It is assumed that translate preserves the order of terms, and does not
    // create any zero terms.
    if (from.isZero())
        validateZero(poly);
    else {
        EXPECT_NO_THROW({ poly.validate(); });
        EXPECT_FALSE(poly.isZero());
        EXPECT_EQ(poly.countTerms(), from.countTerms());

        auto pit = poly.begin();
        auto fit = from.begin();
        while (pit != poly.end() && fit != from.end())
            EXPECT_EQ(*pit++, translate(*fit++));
        EXPECT_EQ(pit, poly.end());
        EXPECT_EQ(fit, from.end());
    }
}

TEST_F(Laurent2Test, moveThenAssign) {
    // Here we test both move-then-assign and move-then-init.
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        // Here *junk indicates the initial value of y.  The point of this is
        // to ensure that, when moving x into y, we don't end up with unwanted
        // residue from this junk living within x.
        //
        // We also allow junk to be cases.end() which means that y should be
        // default constructed (hence the awkward structure of this loop).
        auto junk = cases.begin();
        while (true) {
            {
                L x(c);
                validate(x, c);
                L y;
                if (junk != cases.end()) {
                    y = *junk;
                    validate(y, *junk);
                } else {
                    validateZero(y);
                }

                y = std::move(x);
                x = 3;
                validate(x, L(3));
                validate(y, c);
            }
            {
                L x(c);
                validate(x, c);
                L y;
                if (junk != cases.end()) {
                    y = *junk;
                    validate(y, *junk);
                } else {
                    validateZero(y);
                }

                y = std::move(x);
                x = bigInt;
                validate(x, L(bigInt));
                validate(y, c);
            }
            {
                L x(c);
                validate(x, c);
                L y;
                if (junk != cases.end()) {
                    y = *junk;
                    validate(y, *junk);
                } else {
                    validateZero(y);
                }

                y = std::move(x);
                Integer i(bigInt);
                x = std::move(i);
                validate(x, L(bigInt));
                validate(y, c);
            }
            {
                L x(c);
                validate(x, c);
                L y;
                if (junk != cases.end()) {
                    y = *junk;
                    validate(y, *junk);
                } else {
                    validateZero(y);
                }

                y = std::move(x);
                x.init();
                validateZero(x);
                validate(y, c);
            }
            {
                L x(c);
                validate(x, c);
                L y;
                if (junk != cases.end()) {
                    y = *junk;
                    validate(y, *junk);
                } else {
                    validateZero(y);
                }

                y = std::move(x);
                x.initExp(1, 2);
                validate(x, xy2);
                validate(y, c);
            }

            for (const L& d : cases) {
                SCOPED_TRACE_REGINA(d);
                {
                    L x(c);
                    validate(x, c);
                    L y;
                    if (junk != cases.end()) {
                        y = *junk;
                        validate(y, *junk);
                    } else {
                        validateZero(y);
                    }

                    y = std::move(x);
                    x = d;
                    validate(x, d);
                    validate(y, c);
                }
                {
                    L x(c);
                    validate(x, c);
                    L y;
                    if (junk != cases.end()) {
                        y = *junk;
                        validate(y, *junk);
                    } else {
                        validateZero(y);
                    }

                    y = std::move(x);
                    x = L(d);
                    validate(x, d);
                    validate(y, c);
                }
            }

            if (junk == cases.end())
                break;
            ++junk;
        }
    }
}

TEST_F(Laurent2Test, set) {
    L x { {0, 0, 1}, {1, -1, 2} };

    validate(x, { {0, 0, 1}, {1, -1, 2} });
    x.set(-1, 1, 3);
    validate(x, { {0, 0, 1}, {1, -1, 2}, {-1, 1, 3} });
    x.set(0, 0, 0);
    validate(x, { {1, -1, 2}, {-1, 1, 3} });
    x.set(1, -1, 0);
    validate(x, { {-1, 1, 3} });
    x.set(1, -1, 0);
    validate(x, { {-1, 1, 3} });
    x.set(-1, 1, 0);
    validate(x, {});
    x.set(0, 0, 0);
    validate(x, {});
    x.set(-1, -1, 1);
    validate(x, { {-1, -1, 1} });
}

TEST_F(Laurent2Test, negate) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        L neg = -c;
        validate(neg, c, [](const Term& term) {
            return std::make_pair(term.first, -term.second);
        });
        validate(-L(c), neg);
        {
            L x(c);
            x.negate();
            validate(x, neg);
        }
    }
}

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
        const L& quotient) {
    SCOPED_TRACE_TYPE(Native);

    Native native;
    if (scalar >= std::numeric_limits<Native>::min() &&
            scalar <= std::numeric_limits<Native>::max()) {
        EXPECT_NO_THROW({ native = scalar.safeValue<Native>(); });
    } else {
        EXPECT_THROW({ scalar.safeValue<Native>(); }, regina::IntegerOverflow);
        return;
    }

    validate(poly / native, quotient);
    validate(L(poly) / native, quotient);
    {
        L x(poly);
        validate(x /= native, quotient);
    }
}

static void verifyScalarMultiplyDivide(const L& poly, const Integer& scalar) {
    SCOPED_TRACE_REGINA(scalar);

    const L product = poly * scalar;
    if (scalar == 0) {
        validateZero(product);
    } else {
        validate(product, poly, [&scalar](const Term& term) {
            return std::make_pair(term.first, term.second * scalar);
        });
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

    // For now, Laurent2 division requires a non-zero divisor.
    if (scalar != 0) {
        const L quotient = poly / scalar;
        if (poly.isZero()) {
            validateZero(quotient);
        } else {
            validate(quotient);
            EXPECT_LE(quotient.countTerms(), poly.countTerms());

            auto pit = poly.begin();
            auto qit = quotient.begin();
            while (pit != poly.end() || qit != quotient.end()) {
                if (pit != poly.end() && qit != quotient.end() &&
                        pit->first == qit->first) {
                    auto q = (pit++)->second / scalar;
                    EXPECT_NE(q, 0);
                    EXPECT_EQ((qit++)->second, q);
                } else if (pit != poly.end()) {
                    // We might have a coefficient which, when divided by our
                    // scalar using integer division, becomes zero.
                    EXPECT_GT(pit->second, -scalar.abs());
                    EXPECT_LT(pit->second, scalar.abs());
                    EXPECT_EQ((pit++)->second / scalar, 0);
                } else {
                    // We have a deeper mismatch between terms that we do not
                    // know how to recover from.
                    ADD_FAILURE();
                    break;
                }
            }
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

TEST_F(Laurent2Test, scalarMultiplyDivide) {
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
            verifyScalarMultiplyDivide(c, c.begin()->second);

            L c0 = c / c.begin()->second;
            EXPECT_EQ(c0.begin()->first, c.begin()->first);
            EXPECT_EQ(c0.begin()->second, 1);
        }
    }
}

TEST_F(Laurent2Test, addSubtract) {
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
                EXPECT_LE(sum.countTerms(), c.countTerms() + d.countTerms());

                // Verify the individual terms of the sum.
                auto sit = sum.begin();
                auto cit = c.begin();
                auto dit = d.begin();
                while (sit != sum.end() || cit != c.end() || dit != d.end()) {
                    if (cit != c.end() && dit != d.end() &&
                            cit->first == dit->first) {
                        if (sit != sum.end() && sit->first == cit->first) {
                            EXPECT_EQ((sit++)->second,
                                (cit++)->second + (dit++)->second);
                        } else {
                            EXPECT_EQ((cit++)->second, -(dit++)->second);
                        }
                    } else if (sit != sum.end() && cit != c.end() &&
                            sit->first == cit->first) {
                        EXPECT_EQ((sit++)->second, (cit++)->second);
                    } else if (sit != sum.end() && dit != d.end() &&
                            sit->first == dit->first) {
                        EXPECT_EQ((sit++)->second, (dit++)->second);
                    } else {
                        // We have a lone term that appears on one side but
                        // not the other.
                        ADD_FAILURE();
                        break; // since we don't know which iterator to advance
                    }
                }
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
                validate(diff, d, [](const Term& term) {
                    return std::make_pair(term.first, -term.second);
                });
            } else {
                validate(diff);
                EXPECT_LE(diff.countTerms(), c.countTerms() + d.countTerms());

                // Verify the individual terms of the difference.
                auto sit = diff.begin();
                auto cit = c.begin();
                auto dit = d.begin();
                while (sit != diff.end() || cit != c.end() || dit != d.end()) {
                    if (cit != c.end() && dit != d.end() &&
                            cit->first == dit->first) {
                        if (sit != diff.end() && sit->first == cit->first) {
                            EXPECT_EQ((sit++)->second,
                                (cit++)->second - (dit++)->second);
                        } else {
                            EXPECT_EQ((cit++)->second, (dit++)->second);
                        }
                    } else if (sit != diff.end() && cit != c.end() &&
                            sit->first == cit->first) {
                        EXPECT_EQ((sit++)->second, (cit++)->second);
                    } else if (sit != diff.end() && dit != d.end() &&
                            sit->first == dit->first) {
                        EXPECT_EQ((sit++)->second, -(dit++)->second);
                    } else {
                        // We have a lone term that appears on one side but
                        // not the other.
                        ADD_FAILURE();
                        break; // since we don't know which iterator to advance
                    }
                }
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
            validate(d + c, sum);
            validate(-(d - c), diff);
            validate((c + d) - d, c);
            validate((c + d) - c, d);
            validate((c - d) + d, c);
            validate(-((c - d) - c), d);
            validate(-(c - d) + c, d);
            validate(c + (-d), diff);
            validate(c - (-d), sum);
        }

        // Test operations upon one's self:
        const L dbl = c * 2;
        validate(dbl, c, [](const Term& term) {
            return std::make_pair(term.first, term.second * 2);
        });
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

TEST_F(Laurent2Test, multiply) {
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
                EXPECT_GE(product.countTerms(), c.countTerms());
                EXPECT_GE(product.countTerms(), d.countTerms());

                // We will verify the individual coefficients with the help of
                // Laurent<Laurent<Integer>>, whose own multiplications are
                // tested by explicit formula.
                {
                    regina::Laurent<regina::Laurent<Integer>> cLL;
                    for (const auto& term : c) {
                        long x = term.first.first;
                        cLL.set(x, cLL[x] + regina::Laurent<Integer>(
                            term.second).shifted(term.first.second));
                    }

                    regina::Laurent<regina::Laurent<Integer>> dLL;
                    for (const auto& term : d) {
                        long x = term.first.first;
                        dLL.set(x, dLL[x] + regina::Laurent<Integer>(
                            term.second).shifted(term.first.second));
                    }

                    validate(product, L(cLL * dLL));
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
            EXPECT_GE(square.countTerms(), c.countTerms());

            // We will verify the individual coefficients with the help of
            // Laurent<Laurent<Integer>>, whose own multiplications are
            // tested by explicit formula.
            {
                regina::Laurent<regina::Laurent<Integer>> cLL;
                for (const auto& term : c) {
                    long x = term.first.first;
                    cLL.set(x, cLL[x] + regina::Laurent<Integer>(term.second).
                        shifted(term.first.second));
                }

                validate(square, L(cLL * cLL));
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
        validate(c * two, c + c);
        validate(two * c, c + c);
        validate(c * minusOne, -c);
        validate(minusOne * c, -c);
        validate(c * x2, c.shifted(2, 0));
        validate(x2 * c, c.shifted(2, 0));
        validate(c * xy, c.shifted(1, 1));
        validate(xy * c, c.shifted(1, 1));
        validate(c * xy2, c.shifted(1, 2));
        validate(xy2 * c, c.shifted(1, 2));
    }
}

TEST_F(Laurent2Test, ringConstants) {
    // Verify that the RingTraits constants looks correct.
    EXPECT_EQ(regina::RingTraits<L>::zero.str(), "0");
    EXPECT_EQ(regina::RingTraits<L>::one.str(), "1");
}

TEST_F(Laurent2Test, tightEncoding) {
    for (const L& c : cases) {
        TightEncodingTest<L>::verifyTightEncoding(c);
    }
}

TEST_F(Laurent2Test, swap) {
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

