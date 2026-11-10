
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

#include "maths/laurenttest.h"
#include "utilities/tightencodingtest.h"

using regina::Integer;
using regina::Laurent;

using L = Laurent<Integer, 2>;

// An integer that cannot fit into 128 bits.
// We split the digits into chunks to not break syntax highlighting.
#define BIG "5421309874" "5789403215" "6654013103" "5798756432" "1035741817"

class Laurent2Test : public LaurentTest {
    protected:
        // An integer that cannot fit into 128 bits.
        const Integer bigInt { BIG };

        const std::array<const L, 11> cases {
            L{},
            L{{{0, 0}, 1}},
            L{{{0, 0}, 2}},
            L{{{0, 0}, -1}},
            L{{{2, 0}, 1}},
            L{{{1, 1}, 1}},
            L{{{1, 2}, 1}},
            L{{{0, 0}, 1}, {{1, 2}, -2}, {{-2, -1}, 3}},
            L{{{1, 0}, 2}, {{1, 2}, -4}, {{0, -1}, -2}},
            L{{{1, 1}, -1}, {{1, 2}, 1}, {{0, -1}, -2}, {{1, 0}, -2}},
            L{{{1, 2}, bigInt}, {{1, -2}, -bigInt}, {{1, 1}, -1}}
        };

        // Some single-variable polynomials to use with routines such as
        // embed() and diagonalise().
        const std::array<const regina::Laurent<Integer>, 3> sub {
            Laurent<Integer>(),
            Laurent<Integer>(-5),
            Laurent<Integer>(-2, {3,0,2,-1})
        };
};

TEST_F(Laurent2Test, construct) {
    // Here we also test isZero() and countTerms() via the calls to validate()
    // and validateZero(), through the initialiser list tests below.
    validateZero(L());
    validateZero(L({}));
    validate(L{{{0,0},1}}, {{{0,0},1}});
    validate(L{{{0,0},-1}}, {{{0,0},-1}});
    validate(L{{{2,0},3}}, {{{2,0},3}});
    validate(L{{{0,-2},-3}}, {{{0,-2},-3}});
    validate(L{{{1,0},-2}, {{1,1},5}, {{0,1},7}},
        {{{0,1},7}, {{1,0},-2}, {{1,1},5}});
    validate(L{{{0,1},7}, {{1,1},5}, {{1,0},-2}},
        {{{0,1},7}, {{1,0},-2}, {{1,1},5}});
    validate(L{{{0,0},1}, {{0,1},2}, {{0,2},3}, {{-1,0},4}, {{-2,0},5}},
        {{{-2,0},5}, {{-1,0},4}, {{0,0},1}, {{0,1},2}, {{0,2},3}});
    validate(L{{{-2,0},5}, {{-1,0},4}, {{0,2},3}, {{0,1},2}, {{0,0},1}},
        {{{-2,0},5}, {{-1,0},4}, {{0,0},1}, {{0,1},2}, {{0,2},3}});

    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        {
            L x(c);
            validate(x, c);
            L y(std::move(x));
            validate(y, c);
        }

        #if 0
        using Tuple = std::tuple<long, long, Integer>;
        {
            std::vector<Tuple> coeffs;
            for (const auto& t : c)
                coeffs.emplace_back(t.first.first, t.first.second, t.second);
            validate(L(coeffs.begin(), coeffs.end()), c);

            std::reverse(coeffs.begin(), coeffs.end());
            validate(L(coeffs.begin(), coeffs.end()), c);

            std::vector<Tuple> messy;
            for (const auto& t : c)
                messy.emplace_back(t.first.first, t.first.second, 0);
            for (const auto& t : c) // real coefficients in sorted order
                messy.emplace_back(t.first.first, t.first.second, t.second);
            for (const auto& t : c)
                messy.emplace_back(t.first.second, t.first.first, 0);
            for (const auto& t : coeffs) // real coefficients in reverse
                messy.push_back(t);
            messy.emplace_back(1000, -1000, 0);
            validate(L(messy.begin(), messy.end()), c, [](const Term& term) {
                return std::make_pair(term.first, term.second * 2);
            });
        }
        #endif
    }

    {
        Integer zero(0), pos(4), neg(-4), big(bigInt);
        validateZero(L(zero));
        validateZero(L(std::move(zero)));
        validateZero(L(0));
        validate(L(pos), {{{0,0},4}});
        validate(L(std::move(pos)), {{{0,0},4}});
        validate(L(4), {{{0,0},4}});
        validate(L(neg), {{{0,0},-4}});
        validate(L(std::move(neg)), {{{0,0},-4}});
        validate(L(-4), {{{0,0},-4}});
        validate(L(big), {{{0,0},bigInt}});
        validate(L(std::move(big)), {{{0,0},bigInt}});
        // At this point: zero, pos, neg, big are all unusable.
    }

    for (const auto& s : sub) {
        validateZero(L(sub[0], s));
        validateZero(L(s, sub[0]));
    }

    validate(L(sub[1], sub[1]), {{{0,0},25}});
    validate(L(sub[1], sub[2]), {{{0,-2},-15}, {{0,0},-10}, {{0,1},5}});
    validate(L(sub[2], sub[1]), {{{-2,0},-15}, {{0,0},-10}, {{1,0},5}});
    validate(L(sub[2], sub[2]), {
        {{-2,-2},9}, {{-2,0},6}, {{-2,1},-3},
        {{0,-2},6}, {{0,0},4}, {{0,1},-2},
        {{1,-2},-3}, {{1,0},-2}, {{1,1},1}
    });
}

TEST_F(Laurent2Test, embed) {
    validateZero(L::embed<0>(sub[0]));
    validateZero(L::embed<1>(sub[0]));
    validate(L::embed<0>(sub[1]), {{{0,0},-5}});
    validate(L::embed<1>(sub[1]), {{{0,0},-5}});
    validate(L::embed<0>(sub[2]), {{{-2,0},3}, {{0,0},2}, {{1,0},-1}});
    validate(L::embed<1>(sub[2]), {{{0,-2},3}, {{0,0},2}, {{0,1},-1}});
}

TEST_F(Laurent2Test, diagonalise) {
    validateZero(L::diagonalise(sub[0]));
    validate(L::diagonalise(sub[1]), {{{0,0},-5}});
    validate(L::diagonalise(sub[2]), {{{-2,-2},3}, {{0,0},2}, {{1,1},-1}});
}

TEST_F(Laurent2Test, assign) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            {
                L x(c);
                validate(x, c);
                x = d;
                validate(x, d);
            }
            {
                L x(c), y(d);
                validate(x, c);
                validate(y, d);
                x = std::move(y);
                validate(x, d);
            }
        }

        Integer zero(0), pos(4), neg(-4), big(bigInt);
        { L x(c); validate(x, c); x = zero; validateZero(x); }
        { L x(c); validate(x, c); x = std::move(zero); validateZero(x); }
        { L x(c); validate(x, c); x = 0; validateZero(x); }
        { L x(c); validate(x, c); x = pos; validate(x, {{{0,0},4}}); }
        { L x(c); validate(x, c); x = std::move(pos);
            validate(x, {{{0,0},4}}); }
        { L x(c); validate(x, c); x = 4; validate(x, {{{0,0},4}}); }
        { L x(c); validate(x, c); x = neg; validate(x, {{{0,0},-4}}); }
        { L x(c); validate(x, c); x = std::move(neg);
            validate(x, {{{0,0},-4}}); }
        { L x(c); validate(x, c); x = -4; validate(x, {{{0,0},-4}}); }
        { L x(c); validate(x, c); x = big; validate(x, {{{0,0},bigInt}}); }
        { L x(c); validate(x, c); x = std::move(big);
            validate(x, {{{0,0},bigInt}}); }
        // At this point: zero, pos, neg, big are all unusable.
    }
}

TEST_F(Laurent2Test, init) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        {
            L x(c);
            x.init();
            validateZero(x);
        }
        {
            L x(c);
            x.initExp(-2, 3);
            validate(x, {{{-2,3},1}});

            // Here is as good a place as any to test monomial().
            validate(L::monomial(-2, 3), x);
        }
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
                validate(x, {{{1,2},1}});
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
    // We rely on C++ standard data structures for our coefficient map, and so
    // there is not really any specialised code to test here other than
    // ensuring that the map remains free of zero coefficients.

    L x{{{0,0},1}, {{1,-1},2}};

    validate(x, {{{0,0},1}, {{1,-1},2}});
    x.set(-1, 1, 3);
    validate(x, {{{-1,1},3}, {{0,0},1}, {{1,-1},2}});
    x.set(0, 0, 0);
    validate(x, {{{-1,1},3}, {{1,-1},2}});
    x.set(1, -1, 0);
    validate(x, {{{-1,1},3} });
    x.set(1, -1, 0);
    validate(x, {{{-1,1},3} });
    x.set(-1, 1, 0);
    validate(x, {});
    x.set(0, 0, 0);
    validate(x, {});
    x.set(-1, -1, 1);
    validate(x, {{{-1,-1},1}});
}

TEST_F(Laurent2Test, iterators) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        // Here is as good a time as any to test operator() also.
        if (c.isZero()) {
            EXPECT_FALSE(c.first());
            EXPECT_FALSE(c.last());

            for (int i : { -100, -1, 0, 1, 100 })
                for (int j : { -100, -1, 0, 1, 100 })
                    EXPECT_EQ(c(i, j), 0);
        } else {
            ASSERT_TRUE(c.first());
            ASSERT_TRUE(c.last());

            // Forward iteration:
            {
                auto it = c.first();
                EXPECT_EQ(c(it.exp<0>() - 1, it.exp<1>()), 0);
                EXPECT_EQ(c(it.exp<0>(), it.exp<1>() - 1), 0);

                std::optional<regina::LaurentCoefficient<Integer, 2>> prev;
                for ( ; it; prev = it++) {
                    if (prev.has_value()) {
                        // Manually verify the order of iteration:
                        EXPECT_LE(prev->exp<0>(), it.exp<0>());
                        if (prev->exp<0>() == it.exp<0>())
                            EXPECT_LT(prev->exp<1>(), it.exp<1>());
                    }
                    EXPECT_EQ(c(it.exp<0>(), it.exp<1>()), *it);
                }

                ASSERT_TRUE(prev);
                ASSERT_TRUE(*prev);
                EXPECT_EQ(*prev, c.last());
                EXPECT_EQ(c(prev->exp<0>() + 1, prev->exp<1>()), 0);
                EXPECT_EQ(c(prev->exp<0>(), prev->exp<1>() + 1), 0);
            }

            // Backward iteration:
            {
                auto it = c.last();
                EXPECT_EQ(c(it.exp<0>() + 1, it.exp<1>()), 0);
                EXPECT_EQ(c(it.exp<0>(), it.exp<1>() + 1), 0);

                std::optional<regina::LaurentCoefficient<Integer, 2>> prev;
                for ( ; it; prev = it--) {
                    if (prev.has_value()) {
                        // Manually verify the order of iteration:
                        EXPECT_GE(prev->exp<0>(), it.exp<0>());
                        if (prev->exp<0>() == it.exp<0>())
                            EXPECT_GT(prev->exp<1>(), it.exp<1>());
                    }
                    EXPECT_EQ(c(it.exp<0>(), it.exp<1>()), *it);
                }

                ASSERT_TRUE(prev);
                ASSERT_TRUE(*prev);
                EXPECT_EQ(*prev, c.first());
                EXPECT_EQ(c(prev->exp<0>() - 1, prev->exp<1>()), 0);
                EXPECT_EQ(c(prev->exp<0>(), prev->exp<1>() - 1), 0);
            }
        }
    }
}

TEST_F(Laurent2Test, comparison) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        EXPECT_TRUE(c == c);
        EXPECT_TRUE(c == L(c));
        EXPECT_FALSE(c != c);
        EXPECT_FALSE(c != L(c));
        EXPECT_EQ(c <=> c, std::strong_ordering::equal);
        EXPECT_EQ(c <=> L(c), std::strong_ordering::equal);

        for (const L& d : cases) {
            if (std::addressof(c) == std::addressof(d))
                continue;

            SCOPED_TRACE_REGINA(d);

            auto c_vs_d = c <=> d;
            EXPECT_FALSE(c == d);
            EXPECT_TRUE(c != d);
            EXPECT_NE(c_vs_d, std::strong_ordering::equal);
            EXPECT_EQ(d <=> c, 0 <=> c_vs_d);

            // Test transitivity of comparisions:
            for (const L& e : cases) {
                if (std::addressof(e) == std::addressof(c) ||
                        std::addressof(e) == std::addressof(d))
                    continue;
                if ((d <=> e) != (0 <=> c_vs_d))
                    EXPECT_EQ(c <=> e, c_vs_d);
            }
        }

        EXPECT_EQ(L(), 0);
        EXPECT_EQ(L(), Integer());
        EXPECT_NE(L(), 2);
        EXPECT_NE(L(), Integer(2));
        EXPECT_NE(L(), bigInt);
        EXPECT_NE(L(2), 0);
        EXPECT_NE(L(2), Integer());
        EXPECT_EQ(L(2), 2);
        EXPECT_EQ(L(2), Integer(2));
        EXPECT_NE(L(2), bigInt);
        EXPECT_NE(L(bigInt), 0);
        EXPECT_NE(L(bigInt), Integer());
        EXPECT_NE(L(bigInt), 2);
        EXPECT_NE(L(bigInt), Integer(2));
        EXPECT_EQ(L(bigInt), bigInt);
        EXPECT_NE(L::monomial(0, 0), 0);
        EXPECT_NE(L::monomial(0, 0), Integer(0));
        EXPECT_EQ(L::monomial(0, 0), 1);
        EXPECT_EQ(L::monomial(0, 0), Integer(1));
        EXPECT_NE(L::monomial(2, 0), 2);
        EXPECT_NE(L::monomial(0, 2), 2);
        EXPECT_NE(L::monomial(2, 0), Integer(2));
        EXPECT_NE(L::monomial(0, 2), Integer(2));
    }
}

TEST_F(Laurent2Test, shift) {
    static constexpr int shifts[5] = { -100, -5, 0, 5, 100 };
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (int xShift : shifts) {
            SCOPED_TRACE_NUMERIC(xShift);

            for (int yShift : shifts) {
                SCOPED_TRACE_NUMERIC(yShift);

                L s = c.shifted(xShift, yShift);
                validate(s, c, [xShift, yShift](long x, long y,
                        const Integer& v) {
                    return std::make_tuple(x + xShift, y + yShift, v);
                });
                validate(L(c).shifted(xShift, yShift), s);
                {
                    L x(c);
                    x.shift(xShift, yShift);
                    validate(x, s);
                }
            }
        }
    }
}

TEST_F(Laurent2Test, negate) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        L neg = -c;
        validate(neg, c, [](long x, long y, const Integer& c) {
            return std::make_tuple(x, y, -c);
        });
        validate(-L(c), neg);
        {
            L x(c);
            x.negate();
            validate(x, neg);
        }
    }
}

TEST_F(Laurent2Test, invert) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        L invX(c), invY(c);
        invX.invert<0>();
        invY.invert<1>();

        validate(invX);
        validate(invY);

        using Term = std::tuple<long, long, Integer>;
        {
            std::vector<Term> expect;
            for (auto it = c.first(); it; ++it)
                expect.emplace_back(-it.exp<0>(), it.exp<1>(), *it);
            std::sort(expect.begin(), expect.end());

            std::vector<Term> found;
            for (auto it = invX.first(); it; ++it)
                found.emplace_back(it.exp<0>(), it.exp<1>(), *it);

            EXPECT_EQ(found, expect);
        }
        {
            std::vector<Term> expect;
            for (auto it = c.first(); it; ++it)
                expect.emplace_back(it.exp<0>(), -it.exp<1>(), *it);
            std::sort(expect.begin(), expect.end());

            std::vector<Term> found;
            for (auto it = invY.first(); it; ++it)
                found.emplace_back(it.exp<0>(), it.exp<1>(), *it);

            EXPECT_EQ(found, expect);
        }

        invX.invert<1>();
        invY.invert<0>();

        validate(invX);
        validate(invY);
        EXPECT_EQ(invX, invY);
    }
}

namespace {
    // TODO: Putting verifyNativeProduct and verifyNativeQuotient inside an
    // anonymous namespace is slowing down the tests.  I'd love to know why.
    template <regina::CppInteger Native>
    void verifyNativeProduct(const L& poly, const Integer& scalar,
            const L& product) {
        SCOPED_TRACE_TYPE(Native);

        Native native;
        if (scalar >= std::numeric_limits<Native>::min() &&
                scalar <= std::numeric_limits<Native>::max()) {
            EXPECT_NO_THROW({ native = scalar.safeValue<Native>(); });
        } else {
            EXPECT_THROW({ scalar.safeValue<Native>(); },
                regina::IntegerOverflow);
            return;
        }

        LaurentTest::validate(poly * native, product);
        LaurentTest::validate(L(poly) * native, product);
        LaurentTest::validate(native * poly, product);
        LaurentTest::validate(native * L(poly), product);
        {
            L x(poly);
            LaurentTest::validate(x *= native, product);
        }
    }

    template <regina::CppInteger Native>
    void verifyNativeQuotient(const L& poly, const Integer& scalar,
            const L& quotient) {
        SCOPED_TRACE_TYPE(Native);

        Native native;
        if (scalar >= std::numeric_limits<Native>::min() &&
                scalar <= std::numeric_limits<Native>::max()) {
            EXPECT_NO_THROW({ native = scalar.safeValue<Native>(); });
        } else {
            EXPECT_THROW({ scalar.safeValue<Native>(); },
                regina::IntegerOverflow);
            return;
        }

        LaurentTest::validate(poly / native, quotient);
        LaurentTest::validate(L(poly) / native, quotient);
        {
            L x(poly);
            LaurentTest::validate(x /= native, quotient);
        }
    }

    void verifyScalarMultiplyDivide(const L& poly, const Integer& scalar) {
        SCOPED_TRACE_REGINA(scalar);

        const L product = poly * scalar;
        if (scalar == 0) {
            LaurentTest::validateZero(product);
        } else {
            LaurentTest::validate(product, poly, [&scalar](long x, long y,
                    const Integer& c) {
                return std::make_tuple(x, y, c * scalar);
            });
        }
        LaurentTest::validate(L(poly) * scalar, product);
        LaurentTest::validate(scalar * poly, product);
        LaurentTest::validate(scalar * L(poly), product);
        {
            L x(poly);
            LaurentTest::validate(x *= scalar, product);
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
                LaurentTest::validateZero(quotient);
            } else {
                LaurentTest::validate(quotient);
                auto pit = poly.first();
                auto qit = quotient.first();
                while (pit || qit) {
                    if (pit && qit && pit.exp<0>() == qit.exp<0>() &&
                            pit.exp<1>() == qit.exp<1>()) {
                        auto q = *pit++ / scalar;
                        EXPECT_NE(q, 0);
                        EXPECT_EQ(*qit++, q);
                    } else if (pit) {
                        // We might have a coefficient which, when divided by
                        // our scalar using integer division, becomes zero.
                        EXPECT_GT(*pit, -scalar.abs());
                        EXPECT_LT(*pit, scalar.abs());
                        EXPECT_EQ(*pit++ / scalar, 0);
                    } else {
                        // We have a deeper mismatch between terms that we
                        // do not know how to recover from.
                        ADD_FAILURE();
                        break;
                    }
                }
            }
            LaurentTest::validate(L(poly) / scalar, quotient);
            {
                L x(poly);
                LaurentTest::validate(x /= scalar, quotient);
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
} // anonymous namespace

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
            const auto& f = c.first();
            verifyScalarMultiplyDivide(c, *f);

            L c0 = c / *f;
            const auto& f0 = c0.first();
            EXPECT_EQ(f0.exp<0>(), f.exp<0>());
            EXPECT_EQ(f0.exp<1>(), f.exp<1>());
            EXPECT_EQ(*f0, 1);
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

                // Verify the individual terms of the sum.
                auto sit = sum.first();
                auto cit = c.first();
                auto dit = d.first();
                while (sit || cit || dit) {
                    if (cit && dit && cit.exp<0>() == dit.exp<0>() &&
                            cit.exp<1>() == dit.exp<1>()) {
                        if (sit && sit.exp<0>() == cit.exp<0>() &&
                                sit.exp<1>() == cit.exp<1>()) {
                            EXPECT_EQ(*sit++, *cit++ + *dit++);
                        } else {
                            EXPECT_EQ(*cit++, -*dit++);
                        }
                    } else if (sit && cit && sit.exp<0>() == cit.exp<0>() &&
                            sit.exp<1>() == cit.exp<1>()) {
                        EXPECT_EQ(*sit++, *cit++);
                    } else if (sit && dit && sit.exp<0>() == dit.exp<0>() &&
                            sit.exp<1>() == dit.exp<1>()) {
                        EXPECT_EQ(*sit++, *dit++);
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
                validate(diff, d, [](long x, long y, const Integer& c) {
                    return std::make_tuple(x, y, -c);
                });
            } else {
                validate(diff);

                // Verify the individual terms of the difference.
                auto sit = diff.first();
                auto cit = c.first();
                auto dit = d.first();
                while (sit || cit || dit) {
                    if (cit && dit && cit.exp<0>() == dit.exp<0>() &&
                            cit.exp<1>() == dit.exp<1>()) {
                        if (sit && sit.exp<0>() == cit.exp<0>() &&
                                sit.exp<1>() == cit.exp<1>()) {
                            EXPECT_EQ(*sit++, *cit++ - *dit++);
                        } else {
                            EXPECT_EQ(*cit++, *dit++);
                        }
                    } else if (sit && cit && sit.exp<0>() == cit.exp<0>() &&
                            sit.exp<1>() == cit.exp<1>()) {
                        EXPECT_EQ(*sit++, *cit++);
                    } else if (sit && dit && sit.exp<0>() == dit.exp<0>() &&
                            sit.exp<1>() == dit.exp<1>()) {
                        EXPECT_EQ(*sit++, -*dit++);
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
        validate(dbl, c, [](long x, long y, const Integer& v) {
            return std::make_tuple(x, y, v * 2);
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
        const L zero;
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

                // We have already tested addition and scalar multiplication,
                // so we can attempt to reimplement multiplication in terms
                // of these.
                {
                    L expect;
                    for (auto it = d.first(); it; ++it)
                        expect += (c * *it).shifted(it.exp<0>(), it.exp<1>());
                    validate(product, expect);
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

            // As before, reimplement multiplication in terms of addition and
            // scalar multiplication.
            {
                L expect;
                for (auto it = c.first(); it; ++it)
                    expect += (c * *it).shifted(it.exp<0>(), it.exp<1>());
                validate(square, expect);
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
        validate(c * L(1), c);
        validate(L(1) * c, c);
        validate(c * L(2), c + c);
        validate(L(2) * c, c + c);
        validate(c * L(-1), -c);
        validate(L(-1) * c, -c);
        for (int xExp = -2; xExp <= 2; ++xExp)
            for (int yExp = -2; yExp <= 2; ++yExp) {
                validate(c * L::monomial(xExp, yExp), c.shifted(xExp, yExp));
                validate(L::monomial(xExp, yExp) * c, c.shifted(xExp, yExp));
            }
    }
}

TEST_F(Laurent2Test, ringConstants) {
    // Verify that the RingTraits constants looks correct.
    EXPECT_EQ(regina::RingTraits<L>::zero.str(), "0");
    EXPECT_EQ(regina::RingTraits<L>::one.str(), "1");
}

TEST_F(Laurent2Test, str) {
    verifyStr(L(), "0");
    verifyStr(L(2), "2");
    verifyStr(L(-1), "-1");
    verifyStr(L::monomial(1, 0), "x");
    verifyStr(L::monomial(2, 0), "x^2");
    verifyStr(L::monomial(-1, 0), "x^-1");
    verifyStr(L::monomial(0, 1), "y");
    verifyStr(L::monomial(0, 2), "y^2");
    verifyStr(L::monomial(0, -1), "y^-1");
    verifyStr(L::monomial(1, -1), "x y^-1");
    verifyStr(L::monomial(-1, 1), "x^-1 y");
    verifyStr(L::monomial(2, -2), "x^2 y^-2");
    verifyStr(L::monomial(-2, 2), "x^-2 y^2");
    verifyStr(L{{{-2,3},-1}}, "-x^-2 y^3");
    verifyStr(L{{{-2,3},5}}, "5 x^-2 y^3");
    verifyStr(L{{{-2,3},-6}}, "-6 x^-2 y^3");
    verifyStr(L{{{0,0},1}, {{1,2},-2}, {{-2,-1},3}},
        "-2 x y^2 + 1 + 3 x^-2 y^-1");
    verifyStr(L{{{1,0},2}, {{1,2},-4}, {{0,-1},-2}},
        "-4 x y^2 + 2 x - 2 y^-1");
    verifyStr(L{{{0,0},3}, {{2,1},3}, {{2,-1},-2}, {{1,-1},5}},
        "3 x^2 y - 2 x^2 y^-1 + 5 x y^-1 + 3");
    verifyStr(L{{{0,0},3}, {{-2,1},3}, {{-2,-1},-2}, {{-1,-1},5}},
        "3 + 5 x^-1 y^-1 + 3 x^-2 y - 2 x^-2 y^-1");
    verifyStr(L{{{0,0},-3}, {{-2,1},3}, {{-2,-1},-2}, {{-1,-1},5}},
        "-3 + 5 x^-1 y^-1 + 3 x^-2 y - 2 x^-2 y^-1");
}

TEST_F(Laurent2Test, utf8) {
    // Here we directly use unicode superscripts and minus signs in the C++
    // source code.
    verifyUtf8(L(), u8"0");
    verifyUtf8(L(2), u8"2");
    verifyUtf8(L(-1), u8"−1");
    verifyUtf8(L::monomial(1, 0), u8"x");
    verifyUtf8(L::monomial(2, 0), u8"x²");
    verifyUtf8(L::monomial(-1, 0), u8"x⁻¹");
    verifyUtf8(L::monomial(0, 1), u8"y");
    verifyUtf8(L::monomial(0, 2), u8"y²");
    verifyUtf8(L::monomial(0, -1), u8"y⁻¹");
    verifyUtf8(L::monomial(1, -1), u8"x y⁻¹");
    verifyUtf8(L::monomial(-1, 1), u8"x⁻¹ y");
    verifyUtf8(L::monomial(2, -2), u8"x² y⁻²");
    verifyUtf8(L::monomial(-2, 2), u8"x⁻² y²");
    verifyUtf8(L{{{-2,3},-1}}, u8"−x⁻² y³");
    verifyUtf8(L{{{-2,3},5}}, u8"5 x⁻² y³");
    verifyUtf8(L{{{-2,3},-6}}, u8"−6 x⁻² y³");
    verifyUtf8(L{{{0,0},1}, {{1,2},-2}, {{-2,-1},3}},
        u8"−2 x y² + 1 + 3 x⁻² y⁻¹");
    verifyUtf8(L{{{1,0},2}, {{1,2},-4}, {{0,-1},-2}},
        u8"−4 x y² + 2 x − 2 y⁻¹");
    verifyUtf8(L{{{0,0},3}, {{2,1},3}, {{2,-1},-2}, {{1,-1},5}},
        u8"3 x² y − 2 x² y⁻¹ + 5 x y⁻¹ + 3");
    verifyUtf8(L{{{0,0},3}, {{-2,1},3}, {{-2,-1},-2}, {{-1,-1},5}},
        u8"3 + 5 x⁻¹ y⁻¹ + 3 x⁻² y − 2 x⁻² y⁻¹");
    verifyUtf8(L{{{0,0},-3}, {{-2,1},3}, {{-2,-1},-2}, {{-1,-1},5}},
        u8"−3 + 5 x⁻¹ y⁻¹ + 3 x⁻² y − 2 x⁻² y⁻¹");
}

TEST_F(Laurent2Test, tightEncoding) {
    for (const L& c : cases) {
        TightEncodingTest<L>::verifyTightEncoding(c);
    }

    // Test a handful of cases manually, to ensure the format does not change
    // between releases.  (The following tight encodings were all computed
    // using Regina 7.4.1.)
    EXPECT_EQ(L().tightEncoding(), "M");
    {
        L p{{{2,0},3}};
        EXPECT_EQ(p.tightEncoding(), "POMM");
    }
    {
        L p{{{-1,2},5}};
        EXPECT_EQ(p.tightEncoding(), "RLOM");
    }
    {
        L p{{{0,0},1}, {{1,2},-2}, {{-2,-1},3}};
        EXPECT_EQ(p.tightEncoding(), "PKLNMMKNOM");
    }
    {
        L p{{{1,1},-1}, {{1,3},1}, {{0,-1},-2}, {{1,-1},-2}};
        EXPECT_EQ(p.tightEncoding(), "KMLKNLLNNNNPM");
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

