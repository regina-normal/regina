
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

#include "testhelper.h"

/**
 * Provides common code for testing Laurent polynomials.
 *
 * Test fixtures in each dimension should use LaurentTest as a base class.
 */
class LaurentTest : public testing::Test {
    public:
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void validate(const regina::Laurent<T, vars>& poly) {
            EXPECT_NO_THROW({ poly.validate(); });
        }

        template <regina::CoefficientDomain T>
        static void validateZero(const T& coeff) {
            if constexpr (regina::CanValidate<T>) {
                coeff.validate();
            } else {
                // Sanity check on the validation concept:
                static_assert(! std::is_same_v<T, regina::Integer>);
            }
            EXPECT_EQ(coeff, 0);
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void validateZero(const regina::Laurent<T, vars>& poly) {
            validate(poly);
            EXPECT_TRUE(poly.isZero());
            if constexpr (vars == 1) {
                EXPECT_EQ(poly.minExp(), 0);
                EXPECT_EQ(poly.maxExp(), 0);
                validateZero(poly[0]);
            } else if constexpr (vars == 2) {
                validateZero(poly(0, 0));
            } else { // vars == 3
                validateZero(poly(0, 0, 0));
            }
            EXPECT_EQ(poly, 0);

            auto alloc = poly.allocation();
            if (alloc.second == 0)
                EXPECT_EQ(alloc.first, 0);
            // If alloc.second is positive then we have a zero polynomial
            // but with memory pre-allocated, which means alloc.first
            // (the base exponent) is arbitrary.

            const regina::Laurent<T, vars> zero;
            EXPECT_EQ(poly, zero);
            EXPECT_FALSE(poly != zero);

            EXPECT_FALSE(poly.first());
            EXPECT_FALSE(poly.last());
        }

        // We use std::type_identity_t in several functions below to force a
        // non-deduced context: this means that the compiler will deduce the
        // coefficient type from the Laurent polynomial, not the initialiser
        // list.

        template <regina::CoefficientDomain T>
        static void validate(const regina::Laurent<T>& poly, long minExp,
                std::initializer_list<std::type_identity_t<T>> coeffs) {
            if (coeffs.size() == 0) { // initializer_list does not have empty()
                validateZero(poly);
            } else {
                validate(poly);
                EXPECT_FALSE(poly.isZero());
                EXPECT_EQ(poly.minExp(), minExp);
                EXPECT_EQ(poly.maxExp(), minExp + coeffs.size() - 1);

                auto alloc = poly.allocation();
                EXPECT_LE(alloc.first, minExp);
                EXPECT_GE(alloc.first + static_cast<long>(alloc.second),
                    minExp + static_cast<long>(coeffs.size()));

                ASSERT_TRUE(poly.first());
                ASSERT_TRUE(poly.last());
                EXPECT_NE(*poly.first(), 0);
                EXPECT_NE(*poly.last(), 0);

                EXPECT_NE(poly[poly.minExp()], 0);
                EXPECT_NE(poly[poly.maxExp()], 0);

                auto expect = coeffs.begin();
                for (long e = minExp; expect != coeffs.end(); ++e, ++expect)
                    EXPECT_EQ(poly[e], *expect);
            }

            const regina::Laurent<T> cmp(minExp, coeffs);
            EXPECT_EQ(poly, cmp);
            EXPECT_FALSE(poly != cmp);
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 1 && vars <= 3)
        static void validate(const regina::Laurent<T, vars>& poly,
                std::initializer_list<std::pair<std::array<long, vars>, T>>
                coeffs) {
            // We assume here that coeffs only contains non-zero coefficients,
            // in order of their exponent sequences which must be distinct.

            if (coeffs.size() == 0) { // initializer_list does not have empty()
                validateZero(poly);
            } else {
                // Note: validate() already verifies the internal array
                // allocation against the polynomial's own min/max exponents.
                validate(poly);

                EXPECT_FALSE(poly.isZero());

                ASSERT_TRUE(poly.first());
                ASSERT_TRUE(poly.last());
                EXPECT_NE(*poly.first(), 0);
                EXPECT_NE(*poly.last(), 0);

                auto pc = poly.first();
                auto ec = coeffs.begin();
                for ( ; pc && ec != coeffs.end(); ++pc, ++ec) {
                    EXPECT_EQ(pc.exponents(), ec->first);
                    EXPECT_EQ(*pc, ec->second);
                }
                EXPECT_FALSE(pc);
                EXPECT_EQ(ec, coeffs.end());
            }
        }

        template <regina::CoefficientDomain T,
            std::ranges::sized_range Container>
        static void validate(const regina::Laurent<T>& poly, long minExp,
                const Container& coeffs) {
            if (coeffs.empty()) {
                validateZero(poly);
            } else {
                validate(poly);
                EXPECT_FALSE(poly.isZero());
                EXPECT_EQ(poly.minExp(), minExp);
                EXPECT_EQ(poly.maxExp(), minExp + coeffs.size() - 1);

                auto alloc = poly.allocation();
                EXPECT_LE(alloc.first, minExp);
                EXPECT_GE(alloc.first + alloc.second, minExp + coeffs.size());

                ASSERT_TRUE(poly.first());
                ASSERT_TRUE(poly.last());
                EXPECT_NE(*poly.first(), 0);
                EXPECT_NE(*poly.last(), 0);

                EXPECT_NE(poly[poly.minExp()], 0);
                EXPECT_NE(poly[poly.maxExp()], 0);

                auto expect = coeffs.begin();
                for (long e = minExp; expect != coeffs.end(); ++e, ++expect)
                    EXPECT_EQ(poly[e], *expect);
            }

            const regina::Laurent<T> cmp(minExp, coeffs.begin(), coeffs.end());
            EXPECT_EQ(poly, cmp);
            EXPECT_FALSE(poly != cmp);
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void validate(const regina::Laurent<T, vars>& poly,
                const regina::Laurent<T, vars>& expect) {
            if (expect.isZero()) {
                validateZero(poly);
            } else {
                // Note: validate() already verifies the internal array
                // allocation against the polynomial's own min/max exponents.
                validate(poly);

                EXPECT_FALSE(poly.isZero());
                if constexpr (vars == 1) {
                    EXPECT_EQ(poly.minExp(), expect.minExp());
                    EXPECT_EQ(poly.maxExp(), expect.maxExp());
                }

                ASSERT_TRUE(poly.first());
                ASSERT_TRUE(poly.last());
                EXPECT_NE(*poly.first(), 0);
                EXPECT_NE(*poly.last(), 0);

                if constexpr (vars == 1) {
                    EXPECT_NE(poly[poly.minExp()], 0);
                    EXPECT_NE(poly[poly.maxExp()], 0);
                }

                auto pc = poly.first();
                auto ec = expect.first();
                for ( ; pc && ec; ++pc, ++ec) {
                    EXPECT_EQ(pc.exponents(), ec.exponents());
                    EXPECT_EQ(*pc, *ec);
                }
                EXPECT_FALSE(pc);
                EXPECT_FALSE(ec);
            }

            EXPECT_EQ(poly, expect);
            EXPECT_FALSE(poly != expect);
        }

        template <regina::CoefficientDomain T,
            std::regular_invocable<long, long, const T&> TranslateTerm>
        requires requires(TranslateTerm trans, long x, long y, const T& c) {
            { trans(x, y, c) } ->
                std::convertible_to<std::tuple<long, long, T>>;
        }
        static void validate(const regina::Laurent<T, 2>& poly,
                const regina::Laurent<T, 2>& from, TranslateTerm&& trans) {
            // It is assumed that translate preserves the order of terms,
            // and does not create any zero terms.
            if (from.isZero()) {
                validateZero(poly);
            } else {
                // Note: validate() already verifies the internal array
                // allocation against the polynomial's own min/max exponents.
                validate(poly);

                EXPECT_FALSE(poly.isZero());

                ASSERT_TRUE(poly.first());
                ASSERT_TRUE(poly.last());
                EXPECT_NE(*poly.first(), 0);
                EXPECT_NE(*poly.last(), 0);

                auto pc = poly.first();
                auto ec = from.first();
                for ( ; pc && ec; ++pc, ++ec) {
                    auto ans = trans(ec.template exp<0>(), ec.template exp<1>(),
                        *ec);
                    EXPECT_EQ(pc.template exp<0>(), std::get<0>(ans));
                    EXPECT_EQ(pc.template exp<1>(), std::get<1>(ans));
                    EXPECT_EQ(*pc, std::get<2>(ans));
                }
                EXPECT_FALSE(pc);
                EXPECT_FALSE(ec);
            }
        }

        /**
         * Returns a polynomial equal to \a poly, but with more space for
         * coefficients allocated on each end than is necessary.
         */
        template <regina::CoefficientDomain T>
        static regina::Laurent<T> padded(const regina::Laurent<T>& poly) {
            // Note: this still works if poly is the zero polynomial (in which
            // case the first call to set() changes _both_ minExp and maxExp).
            regina::Laurent<T> ans(poly);
            ans.set(ans.minExp() - 3, 1); // changes minExp
            ans.set(ans.maxExp() + 2, 1); // changes maxExp
            ans.set(ans.minExp(), 0);
            ans.set(ans.maxExp(), 0);
            return ans;
        }

        /**
         * Returns the coefficients of \a poly in order from minimum to maximum
         * exponent, including any intermediate zero coefficients.
         */
        template <regina::CoefficientDomain T>
        static regina::FixedArray<T> coefficientSequence(
                const regina::Laurent<T>& poly) {
            if (poly.isZero()) {
                return {};
            } else {
                regina::FixedArray<T> ans(poly.maxExp() - poly.minExp() + 1);
                long exp = poly.minExp();
                size_t idx = 0;
                while (exp <= poly.maxExp())
                    ans[idx++] = poly[exp++];
                return ans;
            }
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyTransform(const regina::Laurent<T, vars>& c) {
            {
                // A transformation that preserves minExp/maxExp:
                regina::Laurent<T, vars> x(c);
                x.transform([&c](T& v, auto...) {
                    EXPECT_FALSE(c.isZero());
                    EXPECT_NE(v, 0);
                    v *= 2;
                });
                validate(x, c * 2);
            }
            {
                // A transformation that could change minExp/maxExp or even make
                // the entire polynomial zero:
                regina::Laurent<T, vars> x(c);
                x.transform([&c](T& v, auto...) {
                    EXPECT_FALSE(c.isZero());
                    EXPECT_NE(v, 0);
                    v /= 2;
                });
                validate(x, c / 2);
            }
            {
                // A transformation that uses the exponents:
                regina::Laurent<T, vars> x(c), y;
                x.transform([&c, &y](T& v, long exp0, auto... exp) {
                    EXPECT_FALSE(c.isZero());
                    if constexpr (vars == 1) {
                        EXPECT_GE(exp0, c.minExp());
                        EXPECT_LE(exp0, c.maxExp());
                    }
                    EXPECT_NE(v, 0);

                    if constexpr (sizeof...(exp) == vars - 1) {
                        // This sizeof test should always be true.  However,
                        // we need this code to compile even when it's false,
                        // since the compiler will be trying our lambda against
                        // several other function signatures to choose the right
                        // overload of transform().
                        y.set(exp0, exp..., v);
                    }
                    v /= 2;
                });
                validate(x, c / 2);
                validate(y, c);
            }
        }

        /**
         * Tests addition/subtraction operations involving two polynomials.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyAddSubtract(const regina::Laurent<T, vars>& c,
                const regina::Laurent<T, vars>& d) {
            using L = regina::Laurent<T, vars>;

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
                if constexpr (vars == 1) {
                    if (c.minExp() == d.minExp())
                        EXPECT_GE(sum.minExp(), c.minExp());
                    else
                        EXPECT_EQ(sum.minExp(),
                            std::min(c.minExp(), d.minExp()));

                    if (c.maxExp() == d.maxExp())
                        EXPECT_LE(sum.maxExp(), c.maxExp());
                    else
                        EXPECT_EQ(sum.maxExp(),
                            std::max(c.maxExp(), d.maxExp()));

                    for (long i = std::min(c.minExp(), d.minExp());
                            i <= std::max(c.maxExp(), d.maxExp()); ++i)
                        EXPECT_EQ(sum[i], c[i] + d[i]);
                } else {
                    auto sit = sum.first();
                    auto cit = c.first();
                    auto dit = d.first();
                    while (sit || cit || dit) {
                        if (cit && dit && cit.exponents() == dit.exponents()) {
                            if (sit && sit.exponents() == cit.exponents()) {
                                EXPECT_EQ(*sit++, *cit++ + *dit++);
                            } else {
                                EXPECT_EQ(*cit++, -*dit++);
                            }
                        } else if (sit && cit &&
                                sit.exponents() == cit.exponents()) {
                            EXPECT_EQ(*sit++, *cit++);
                        } else if (sit && dit &&
                                sit.exponents() == dit.exponents()) {
                            EXPECT_EQ(*sit++, *dit++);
                        } else {
                            // We have a lone term that appears on one side but
                            // not the other.
                            ADD_FAILURE();
                            break; // we don't know which iterator to advance
                        }
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
                if constexpr (vars == 1) {
                    validate(diff);
                    EXPECT_FALSE(diff.isZero()); // since d ≠ 0
                    EXPECT_EQ(diff.minExp(), d.minExp());
                    EXPECT_EQ(diff.maxExp(), d.maxExp());
                    for (long i = d.minExp(); i <= d.maxExp(); ++i)
                        EXPECT_EQ(diff[i], - d[i]);
                } else {
                    validate(diff, d, [](long x, long y, const T& v) {
                        return std::make_tuple(x, y, -v);
                    });
                }
            } else {
                validate(diff);

                // Verify the individual terms of the difference.
                if constexpr (vars == 1) {
                    if (c.minExp() == d.minExp())
                        EXPECT_GE(diff.minExp(), c.minExp());
                    else
                        EXPECT_EQ(diff.minExp(),
                            std::min(c.minExp(), d.minExp()));

                    if (c.maxExp() == d.maxExp())
                        EXPECT_LE(diff.maxExp(), c.maxExp());
                    else
                        EXPECT_EQ(diff.maxExp(), std::max(c.maxExp(),
                            d.maxExp()));

                    for (long i = std::min(c.minExp(), d.minExp());
                            i <= std::max(c.maxExp(), d.maxExp()); ++i)
                        EXPECT_EQ(diff[i], c[i] - d[i]);
                } else {
                    // Verify the individual terms of the difference.
                    auto sit = diff.first();
                    auto cit = c.first();
                    auto dit = d.first();
                    while (sit || cit || dit) {
                        if (cit && dit && cit.exponents() == dit.exponents()) {
                            if (sit && sit.exponents() == cit.exponents()) {
                                EXPECT_EQ(*sit++, *cit++ - *dit++);
                            } else {
                                EXPECT_EQ(*cit++, *dit++);
                            }
                        } else if (sit && cit &&
                                sit.exponents() == cit.exponents()) {
                            EXPECT_EQ(*sit++, *cit++);
                        } else if (sit && dit &&
                                sit.exponents() == dit.exponents()) {
                            EXPECT_EQ(*sit++, -*dit++);
                        } else {
                            // We have a lone term that appears on one side but
                            // not the other.
                            ADD_FAILURE();
                            break; // we don't know which iterator to advance
                        }
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
            validate((c + d) - d, c);
            validate((c + d) - c, d);
            validate((c - d) + d, c);
            validate(-(c - d) + c, d);
            validate(-((c - d) - c), d);
            validate(c + (-d), diff);
            validate(c - (-d), sum);
            validate(d + c, sum);
            validate(-(d - c), diff);
        }

        /**
         * Tests addition/subtraction operations involving a single polynomial.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyAddSubtract(const regina::Laurent<T, vars>& c) {
            using L = regina::Laurent<T, vars>;

            // Test operations upon one's self:
            const L dbl = c * 2;
            if (c.isZero()) {
                validateZero(dbl);
            } else {
                if constexpr (vars == 1) {
                    validate(dbl);
                    EXPECT_FALSE(dbl.isZero());
                    EXPECT_EQ(dbl.minExp(), c.minExp());
                    EXPECT_EQ(dbl.maxExp(), c.maxExp());
                    for (long i = c.minExp(); i <= c.maxExp(); ++i)
                        EXPECT_EQ(dbl[i], c[i] * 2);
                } else {
                    validate(dbl, c, [](long x, long y, const T& v) {
                        return std::make_tuple(x, y, v * 2);
                    });
                }
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

            // Test expected arithmetic properties:
            static const regina::Laurent<T, vars> zero;
            validate(c + zero, c);
            validate(zero + c, c);
            validate(c - zero, c);
            validate(zero - c, -c);
            validateZero(c + -c);
        }

        template <regina::Ring T>
        static void verifyRingConstants() {
            // Verify that the RingTraits constants looks correct.
            // Here T should be some class Laurent<T, vars>.
            EXPECT_EQ(regina::RingTraits<T>::zero.str(), "0");
            EXPECT_EQ(regina::RingTraits<T>::one.str(), "1");
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyStr(const regina::Laurent<T, vars>& poly,
                const char* expect) {
            EXPECT_EQ(poly.str(), expect);
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyUtf8(const regina::Laurent<T, vars>& poly,
                const char8_t* expect) {
            EXPECT_EQ(poly.utf8(),
                std::string(reinterpret_cast<const char*>(expect)));
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifySwap(const regina::Laurent<T, vars>& c,
                const regina::Laurent<T, vars>& d) {
            {
                regina::Laurent<T, vars> x(c);
                regina::Laurent<T, vars> y(d);
                x.swap(y);
                validate(x, d);
                validate(y, c);
            }
            {
                regina::Laurent<T, vars> x(c);
                regina::Laurent<T, vars> y(d);
                swap(x, y);
                validate(x, d);
                validate(y, c);
            }
        }
};

