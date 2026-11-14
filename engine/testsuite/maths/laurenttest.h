
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
#include "utilities/randutils.h"

#include "testhelper.h"

// An integer that cannot fit into 128 bits, and some small multiples of it.
// We split the digits into chunks to not break syntax highlighting.
#define BIG "5421309874" "5789403215" "6654013103" "5798756432" "1035741817"
#define BIG2 "10842619749" "1578806431" "3308026207" "1597512864" "2071483634"
#define BIG3 "16263929623" "7368209646" "9962039310" "7396269296" "3107225451"
#define BIG4 "21685239498" "3157612862" "6616052414" "3195025728" "4142967268"

// Note: We use std::type_identity_t in several functions here to force a
// non-deduced context.  This means that the compiler will deduce the
// coefficient type from the Laurent polynomial (not the other arguments,
// such as initialiser lists, scalar integers, etc.).

/**
 * Provides common code for testing Laurent polynomials.
 *
 * Test fixtures in each dimension should use LaurentTest as a base class.
 */
class LaurentTest : public testing::Test {
    public:
        // A pack of exponents for a single term of a Laurent polynomial.
        template <int vars> requires (vars > 0 && vars <= 3)
        using Exponents = std::array<long, vars>;

        // An integer that cannot fit into 128 bits.
        inline static const regina::Integer bigInt { BIG };

        // ---------------------------------------------------------------
        // Validation functions
        // ---------------------------------------------------------------

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
        requires (vars > 0 && vars <= 3)
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

        template <regina::CoefficientDomain T, int vars,
            std::regular_invocable<const Exponents<vars>&, const T&> Translate>
        requires
            (vars > 0 && vars <= 3) &&
            requires(Translate trans, const Exponents<vars> exp, const T& v) {
                { trans(exp, v) } ->
                    std::convertible_to<std::pair<Exponents<vars>, T>>;
            }
        static void validate(const regina::Laurent<T, vars>& poly,
                const regina::Laurent<T, vars>& from, Translate&& trans) {
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

                // Just run through the non-zero coefficients, which we
                // can access via iterators.
                auto pc = poly.first();
                auto ec = from.first();
                for ( ; pc && ec; ++pc, ++ec) {
                    auto ans = trans(ec.exponents(), *ec);
                    EXPECT_EQ(pc.exponents(), ans.first);
                    EXPECT_EQ(*pc, ans.second);
                }
                EXPECT_FALSE(pc);
                EXPECT_FALSE(ec);
            }
        }

        // ---------------------------------------------------------------
        // Other helper functions
        // ---------------------------------------------------------------

        /**
         * Returns the requested coefficient of the given polynomial, where the
         * exponents are given as an array instead of individual arguments.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static const T& coeff(const regina::Laurent<T, vars>& poly,
                std::type_identity_t<const std::array<long, vars>>& exp) {
            if constexpr (vars == 1)
                return poly[exp[0]];
            else if constexpr (vars == 2)
                return poly(exp[0], exp[1]);
            else // vars == 3
                return poly(exp[0], exp[1], exp[2]);
        }

        /**
         * Sets the given coefficient of the given polynomial, where the
         * exponents are given as an array instead of individual arguments.
         */
        template <regina::CoefficientDomain T, int vars,
            regina::AssignableTo<T&> Value>
        requires (vars > 0 && vars <= 3)
        static void set(regina::Laurent<T, vars>& poly,
                std::type_identity_t<const std::array<long, vars>>& exp,
                Value&& value) {
            if constexpr (vars == 1)
                poly.set(exp[0], std::forward<Value>(value));
            else if constexpr (vars == 2)
                poly.set(exp[0], exp[1], std::forward<Value>(value));
            else // vars == 3
                poly.set(exp[0], exp[1], exp[2], std::forward<Value>(value));
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
         * Returns a polynomial equal to \a poly, but with more space
         * allocated for coefficients at each coordinate position.
         */
        template <regina::CoefficientDomain T>
        static regina::Laurent<T, 2> padded(const regina::Laurent<T, 2>& poly) {
            using L = regina::Laurent<T, 2>;

            L ans(poly);
            if (poly.isZero()) {
                withExponents(poly, { -3, 0, 2 },
                        [&ans](const L&, auto... exp) {
                    ans += L::monomial(exp...);
                });
                withExponents(poly, { -3, 0, 2 },
                        [&ans](const L&, auto... exp) {
                    ans -= L::monomial(exp...);
                });
            } else {
                withExponents(poly, { -3, 0, 2 },
                        [&ans](const L& p, auto... exp) {
                    ans += p.shifted(exp...);
                });
                withExponents(poly, { -3, 0, 2 },
                        [&ans](const L& p, auto... exp) {
                    ans -= p.shifted(exp...);
                });
            }
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

        /**
         * Runs the given function over a comprehensive set of Integer scalars.
         */
        template <std::regular_invocable<const regina::Integer&> Test>
        static void withScalars(Test&& test) {
            for (int i = -3; i <= 3; ++i)
                std::invoke(std::forward<Test>(test), i);

            // Run through boundaries and "midpoints" for {8,16,32,64,128}-bit
            // integers:
            regina::Integer pow2 = 256; // 2^8
            regina::Integer halfPow2 = 128;
            regina::Integer quarterPow2 = 64;
            for (int i = 0; i < 5; ++i) {
                // pow2 = 2^bits = 2^{2^b} for some b
                // halfPow2 = 2^(bits-1)
                // quarterPow2 = 2^(bits-2)
                for (int j = -1; j <= 1; ++j) {
                    std::invoke(std::forward<Test>(test), pow2 + j);
                    std::invoke(std::forward<Test>(test), -pow2 + j);
                    std::invoke(std::forward<Test>(test), halfPow2 + j);
                    std::invoke(std::forward<Test>(test), -halfPow2 + j);
                    std::invoke(std::forward<Test>(test), quarterPow2 + j);
                    std::invoke(std::forward<Test>(test), -quarterPow2 + j);
                }
                // Double the number of bits in pow2:
                quarterPow2 *= pow2;
                halfPow2 *= pow2;
                pow2 *= pow2;
            }

            std::invoke(std::forward<Test>(test), bigInt);
            std::invoke(std::forward<Test>(test), -bigInt);
        }

        /**
         * Runs the given function over all *vars*-tuples of exponents, where
         * each exponent come from the given list.
         *
         * The argument \a test must take a Laurent polynomial followed by
         * precisely *vars* long integers as arguments.  We do not attempt to
         * encode this with a `requires` clause here: it will be messy and this
         * is just the test suite.
         *
         * This is unlike withScalars(), in that it takes a single Laurent
         * polynomial directly (unlike withScalars(), where you use a lambda
         * and capture as many polynomials as you need).  We do it this way
         * so that the implementation knows the value of \a vars.
         */
        template <regina::CoefficientDomain T, int vars, typename Test>
        requires (vars > 0 && vars <= 3)
        static void withExponents(const regina::Laurent<T, vars>& poly,
                std::initializer_list<long> exponents, Test&& test) {
            // Probably we could make this generic by iterating over a
            // std::array<long, vars> and then using std::apply to unpack
            // the array into individual arguments.  But for now we just
            // split into cases according to *vars*, which is easy to code.

            if constexpr (vars == 1) {
                for (long e : exponents)
                    std::invoke(std::forward<Test>(test), poly, e);
            } else if constexpr (vars == 2) {
                for (long e : exponents)
                    for (long f : exponents)
                        std::invoke(std::forward<Test>(test), poly, e, f);
            } else { // vars == 3
                for (long e : exponents)
                    for (long f : exponents)
                        for (long g : exponents)
                            std::invoke(std::forward<Test>(test), poly,
                                e, f, g);
            }
        }

        // ---------------------------------------------------------------
        // Individual tests
        // ---------------------------------------------------------------

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyConstruct(const regina::Laurent<T, vars>& c) {
            using L = regina::Laurent<T, vars>;

            {
                L x(c);
                validate(x, c);
                L y(std::move(x));
                validate(y, c);
            }

            // This code tests the iterator-based constructor, but since
            // the std::initializer_list constructor is implemented using that,
            // it effectively tests the std::initializer_list constructor also.
            if constexpr (vars == 1) {
                auto seq = coefficientSequence(c);
                validate(L(c.minExp(), seq.begin(), seq.end()), c);

                // Test iterator ranges with zero padding at either end.
                // This also covers the case where the range contains entirely
                // zeroes (this is tested when c is the zero polynomial).
                {
                    std::vector<T> coeffs(3); // some initial zeroes
                    coeffs.insert(coeffs.end(), seq.begin(), seq.end());
                    coeffs.resize(coeffs.size() + 2); // some final zeroes
                    EXPECT_FALSE(coeffs.empty());
                    EXPECT_EQ(coeffs.front(), 0);
                    EXPECT_EQ(coeffs.back(), 0);
                    validate(L(c.minExp() - 3, coeffs.begin(), coeffs.end()),
                        c);
                }
            } else {
                using Term = std::pair<std::array<long, vars>, T>;
                {
                    std::vector<Term> terms;
                    for (auto it = c.first(); it; ++it)
                        terms.emplace_back(it.exponents(), *it);
                    validate(L(terms.begin(), terms.end()), c);
                    validate(L(terms.rbegin(), terms.rend()), c);

                    {
                        // Zero terms will be ignored entirely.
                        std::vector<Term> messy;
                        for (size_t i = 1; i < terms.size(); i += 2)
                            messy.emplace_back(terms[i].first, 0);
                        for (size_t i = 0; i < terms.size(); i += 2)
                            messy.push_back(terms[i]);
                        for (size_t i = 0; i < terms.size(); i += 2)
                            messy.emplace_back(terms[i].first, 0);
                        for (size_t i = 1; i < terms.size(); i += 2)
                            messy.push_back(terms[i]);
                        for (size_t i = 1; i < terms.size(); i += 2)
                            messy.emplace_back(terms[i].first, 0);

                        std::array<long, vars> exp;
                        for (int i = 0; i < vars; ++i)
                            exp[i] = (i % 2 == 0 ? 1000 : -1000);
                        messy.push_back(Term(exp, 0));

                        validate(L(messy.begin(), messy.end()), c);
                        validate(L(messy.rbegin(), messy.rend()), c);
                    }

                    for (const auto& t : terms) {
                        std::vector<Term> broken = terms;
                        broken.push_back(t);
                        EXPECT_THROW({ L(broken.begin(), broken.end()); },
                            regina::InvalidArgument);
                    }

                    {
                        using BadTerm =
                            std::pair<std::array<long, vars - 1>, T>;
                        std::vector<BadTerm> bad;

                        validateZero(L(bad.begin(), bad.end()));
                        bad.push_back(BadTerm());
                        EXPECT_THROW({ L(bad.begin(), bad.end()); },
                            regina::InvalidArgument);
                    }

                    {
                        using BadTerm
                            = std::pair<std::array<long, vars + 1>, T>;
                        std::vector<BadTerm> bad;

                        validateZero(L(bad.begin(), bad.end()));
                        bad.push_back(BadTerm());
                        EXPECT_THROW({ L(bad.begin(), bad.end()); },
                            regina::InvalidArgument);
                    }
                }
            }
        }

        template <int vars> requires (vars > 0 && vars <= 3)
        static void verifyConstructBasic() {
            using L = regina::Laurent<regina::Integer, vars>;

            validateZero(L());
            if constexpr (vars == 1) {
                validateZero(L(0, {}));
                validateZero(L(100, {}));
                validateZero(L(-100, {}));
            } else {
                validateZero(L({}));
            }

            regina::Integer zero(0), pos(4), neg(-4), big(bigInt);
            std::array<long, vars> exp;
            std::fill(exp.begin(), exp.end(), 0);

            validateZero(L(zero));
            validateZero(L(std::move(zero)));
            validateZero(L(0));
            validate(L(pos), {{exp, 4}});
            validate(L(std::move(pos)), {{exp, 4}});
            validate(L(4), {{exp, 4}});
            validate(L(neg), {{exp, -4}});
            validate(L(std::move(neg)), {{exp, -4}});
            validate(L(-4), {{exp, -4}});
            validate(L(big), {{exp, bigInt}});
            validate(L(std::move(big)), {{exp, bigInt}});
            // At this point: zero, pos, neg, big are all unusable.
        }

        /**
         * Tests assignment operations involving two polynomials.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyAssign(const regina::Laurent<T, vars>& c,
                const regina::Laurent<T, vars>& d) {
            using L = regina::Laurent<T, vars>;

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
                const auto alloc = y.allocation();
                x = std::move(y);
                validate(x, d);
                EXPECT_EQ(x.allocation(), alloc);
            }
        }

        /**
         * Tests assignment operations involving a single polynomial.
         */
        template <int vars> requires (vars > 0 && vars <= 3)
        static void verifyAssign(
                const regina::Laurent<regina::Integer, vars>& c) {
            using L = regina::Laurent<regina::Integer, vars>;

            regina::Integer zero(0), pos(4), neg(-4), big(bigInt);
            std::array<long, vars> exp;
            std::fill(exp.begin(), exp.end(), 0);

            { L x(c); validate(x, c); x = zero; validateZero(x); }
            { L x(c); validate(x, c); x = std::move(zero); validateZero(x); }
            { L x(c); validate(x, c); x = 0; validateZero(x); }
            { L x(c); validate(x, c); x = pos; validate(x, {{exp, 4}}); }
            { L x(c); validate(x, c); x = std::move(pos);
                validate(x, {{exp, 4}}); }
            { L x(c); validate(x, c); x = 4; validate(x, {{exp, 4}}); }
            { L x(c); validate(x, c); x = neg; validate(x, {{exp, -4}}); }
            { L x(c); validate(x, c); x = std::move(neg);
                validate(x, {{exp, -4}}); }
            { L x(c); validate(x, c); x = -4; validate(x, {{exp, -4}}); }
            { L x(c); validate(x, c); x = big; validate(x, {{exp, bigInt}}); }
            { L x(c); validate(x, c); x = std::move(big);
                validate(x, {{exp, bigInt}}); }
            // At this point: zero, pos, neg, big are all unusable.
        }

        /**
         * Verifies that \a padded does indeed to be a padded version of the
         * given source polynomial (i.e., has more space allocated than it
         * requires).
         *
         * Since we are restricted to public APIs, we can only verify the
         * padding in the first coordinate (i.e., we cannot verify that the
         * inner polynomials spanning the later coordinates are padded also).
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyPadding(const regina::Laurent<T, vars>& padded,
                const regina::Laurent<T, vars>& source) {
            validate(padded, source);

            auto alloc = padded.allocation();
            EXPECT_GT(alloc.second, 0);
            if (source.isZero()) {
                EXPECT_TRUE(padded.isZero());
                // In this case, alloc.first is arbitrary.
            } else {
                EXPECT_LT(alloc.first, source.first().template exp<0>());
                EXPECT_GT(alloc.first + static_cast<long>(alloc.second),
                    source.last().template exp<0>() + 1);
            }
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        void verifyReserveRange(const regina::Laurent<T, vars>& poly,
                long fromExp, long toExp, bool noop = false) {
            SCOPED_TRACE_NUMERIC(fromExp);
            SCOPED_TRACE_NUMERIC(toExp);

            using L = regina::Laurent<T, vars>;

            long minExp = poly.isZero() ? 0 : poly.first().template exp<0>();
            long maxExp = poly.isZero() ? 0 : poly.last().template exp<0>();

            L x(poly); // should set the range precisely from poly.
            const auto initAlloc = x.allocation();
            if (poly.isZero()) {
                EXPECT_EQ(initAlloc.second, 0);
                // initAlloc.first (the base) is arbitrary in this case.
            } else {
                EXPECT_EQ(initAlloc.first, minExp);
                EXPECT_EQ(initAlloc.second, maxExp - minExp + 1);
            }

            x.reserveRange(fromExp, toExp);

            validate(x, poly);
            EXPECT_EQ(x.first().exponents(), poly.first().exponents());
            EXPECT_EQ(x.last().exponents(), poly.last().exponents());

            if (noop) {
                // Nothing should have changed.
                EXPECT_EQ(x.allocation(), initAlloc);
            }

            if (fromExp > toExp) {
                EXPECT_TRUE(noop);
                // The numerical values of (fromExp, toExp) are meaningless,
                // so there is nothing more to test here.
            } else {
                const auto newAlloc = x.allocation();
                EXPECT_GT(newAlloc.second, 0);
                EXPECT_GE(newAlloc.second, initAlloc.second);
                if (! poly.isZero()) {
                    EXPECT_LE(newAlloc.first, minExp);
                    EXPECT_GT(newAlloc.first +
                        static_cast<long>(newAlloc.second), maxExp);
                }
                EXPECT_LE(newAlloc.first, fromExp);
                EXPECT_GT(newAlloc.first + static_cast<long>(newAlloc.second),
                    toExp);

                std::array<long, vars> exp;
                std::fill(exp.begin() + 1, exp.end(), 0);
                for (long i = fromExp; i <= toExp; ++i) {
                    exp[0] = i;
                    set(x, exp, 1);
                    EXPECT_EQ(x.allocation(), newAlloc);
                }
            }
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        void verifyReserveRange(const regina::Laurent<T, vars>& c) {
            if (c.isZero()) {
                verifyReserveRange(c, 1000, 999, true /* no-op */);
                verifyReserveRange(c, -999, -1000, true /* no-op */);
                verifyReserveRange(c, 0, 0);
                verifyReserveRange(c, 100, 100);
                verifyReserveRange(c, -100, -100);
                verifyReserveRange(c, -100, -90);
                verifyReserveRange(c, -10, 10);
                verifyReserveRange(c, 90, 100);
            } else {
                long min = c.first().template exp<0>();
                long max = c.last().template exp<0>();
                long mid = (min + max) / 2;
                long span = max - min;
                verifyReserveRange(c, 1000, 999, true /* no-op */);
                verifyReserveRange(c, -999, -1000, true /* no-op */);
                verifyReserveRange(c, min, min, true /* no-op */);
                verifyReserveRange(c, mid, mid, true /* no-op */);
                verifyReserveRange(c, max, max, true /* no-op */);
                verifyReserveRange(c, min, max, true /* no-op */);
                verifyReserveRange(c, min, mid, true /* no-op */);
                verifyReserveRange(c, mid, max, true /* no-op */);
                verifyReserveRange(c, mid - 1, mid + 1, span >= 2 /* no-op */);
                verifyReserveRange(c, min - 100, min - 100);
                verifyReserveRange(c, min - 1, min - 1);
                verifyReserveRange(c, min - 1, min);
                verifyReserveRange(c, max, max + 1);
                verifyReserveRange(c, max + 1, max + 1);
                verifyReserveRange(c, max + 100, max + 100);
                verifyReserveRange(c, min - 1, max + 1);
                verifyReserveRange(c, min - 100, max + 100);
                verifyReserveRange(c, min - 100, mid);
                verifyReserveRange(c, min - 1, mid);
                verifyReserveRange(c, mid, max + 1);
                verifyReserveRange(c, mid, max + 100);
                verifyReserveRange(c, min - 2 * span, min - span);
                verifyReserveRange(c, max + span, max + 2 * span);
            }
        }

        /**
         * Verifies init(), `init(exp...)`, and `monomial(exp...)`.
         */
        template <regina::CoefficientDomain T, int vars,
            std::convertible_to<long>... Exponents>
        requires (vars > 0 && vars <= 3)
        static void verifyInit(const regina::Laurent<T, vars>& c,
                Exponents... exp) {
            using L = regina::Laurent<T, vars>;

            {
                L x(c);
                x.init();
                validateZero(x);
            }
            {
                L x(c);
                x.initExp(exp...);
                validate(x, {{{exp...},1}});

                // Here is as good a place as any to test monomial().
                validate(L::monomial(exp...), x);
            }
        }

        /**
         * Tests moving out of a polynomial then reassigning a new value, via
         * either the assignment operator or one of our init functions.
         *
         * Here \a junk indicates the initial value to assign to the
         * test polynomial, or is `null` to indicate that the test polynomial
         * should be default constructed.
         */
        template <int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyMoveThenAssign(
                const regina::Laurent<regina::Integer, vars>& c,
                const regina::Laurent<regina::Integer, vars>& d,
                std::type_identity_t<
                    const regina::Laurent<regina::Integer, vars>*> junk) {
            using L = regina::Laurent<regina::Integer, vars>;

            {
                L x(c);
                validate(x, c);
                L y;
                if (junk) {
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
                if (junk) {
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

        /**
         * Tests moving out of a polynomial then reassigning a new value, via
         * either the assignment operator or one of our init functions.
         *
         * Here \a junk indicates the initial value to assign to the
         * test polynomial, or is `null` to indicate that the test polynomial
         * should be default constructed.
         */
        template <int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyMoveThenAssign(
                const regina::Laurent<regina::Integer, vars>& c,
                std::type_identity_t<
                    const regina::Laurent<regina::Integer, vars>*> junk) {
            using L = regina::Laurent<regina::Integer, vars>;

            // The point of passing *junk* at all is to ensure that, when
            // moving x into y, we don't end up with unwanted residue from
            // this junk living within x.
            {
                L x(c);
                validate(x, c);
                L y;
                if (junk) {
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
                if (junk) {
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
                if (junk) {
                    y = *junk;
                    validate(y, *junk);
                } else {
                    validateZero(y);
                }

                y = std::move(x);
                regina::Integer i(bigInt);
                x = std::move(i);
                validate(x, L(bigInt));
                validate(y, c);
            }
            {
                L x(c);
                validate(x, c);
                L y;
                if (junk) {
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
                if (junk) {
                    y = *junk;
                    validate(y, *junk);
                } else {
                    validateZero(y);
                }

                if constexpr (vars == 1) {
                    const L m = L::monomial(-5);
                    y = std::move(x);
                    x.initExp(-5);
                    validate(x, m);
                    validate(y, c);
                } else if constexpr (vars == 2) {
                    const L m = L::monomial(-5, 3);
                    y = std::move(x);
                    x.initExp(-5, 3);
                    validate(x, m);
                    validate(y, c);
                } else { // vars == 3
                    const L m = L::monomial(-5, 3, -2);
                    y = std::move(x);
                    x.initExp(-5, 3, -2);
                    validate(x, m);
                    validate(y, c);
                }
            }
        }

        template <int vars> requires (vars > 0 && vars <= 3)
        static void verifySet(const regina::Laurent<regina::Integer, vars>& c) {
            using L = regina::Laurent<regina::Integer, vars>;

            // Give ourselves random access to the individual non-zero terms.
            std::vector<std::pair<std::array<long, vars>, regina::Integer>>
                terms;
            for (auto it = c.first(); it; ++it)
                terms.emplace_back(it.exponents(), *it);

            // Test setting out-of-range coefficients to zero.
            for (int i = 0; i < vars; ++i) {
                L x(c);
                const auto alloc = x.allocation();
                auto exp = c.first().exponents();
                --exp[i];
                set(x, exp, 0);
                validate(x, c);
                EXPECT_EQ(x.allocation(), alloc);
            }
            for (int i = 0; i < vars; ++i) {
                L x(c);
                const auto alloc = x.allocation();
                auto exp = c.first().exponents();
                exp[i] -= 100;
                set(x, exp, 0);
                validate(x, c);
                EXPECT_EQ(x.allocation(), alloc);
            }
            for (int i = 0; i < vars; ++i) {
                L x(c);
                const auto alloc = x.allocation();
                auto exp = c.last().exponents();
                ++exp[i];
                set(x, exp, 0);
                validate(x, c);
                EXPECT_EQ(x.allocation(), alloc);
            }
            for (int i = 0; i < vars; ++i) {
                L x(c);
                const auto alloc = x.allocation();
                auto exp = c.last().exponents();
                exp[i] += 100;
                set(x, exp, 0);
                validate(x, c);
                EXPECT_EQ(x.allocation(), alloc);
            }

            // Test eliminating all coefficients, one at a time.
            if (! c.isZero()) {
                ASSERT_FALSE(terms.empty());

                for (int pad = 0; pad < 2; ++pad) {
                    // Eliminate from front to back:
                    {
                        L x(c);
                        if (pad)
                            x.reserveRange(c.first().template exp<0>() - 5,
                                c.last().template exp<0>() + 5);
                        for (size_t i = 0; i < terms.size(); ++i) {
                            set(x, terms[i].first, 0);
                            if (i == terms.size() - 1) {
                                validateZero(x);
                            } else {
                                validate(x);
                                EXPECT_NE(x, c);
                                EXPECT_EQ(x.first().exponents(),
                                    terms[i + 1].first);
                                EXPECT_EQ(x.last().exponents(),
                                    terms.back().first);
                            }
                        }
                    }
                    // Eliminate from back to front:
                    {
                        L x(c);
                        if (pad)
                            x.reserveRange(c.first().template exp<0>() - 5,
                                c.last().template exp<0>() + 5);
                        for (ssize_t i = terms.size() - 1; i >= 0; --i) {
                            set(x, terms[i].first, 0);
                            if (i == 0) {
                                validateZero(x);
                            } else {
                                validate(x);
                                EXPECT_NE(x, c);
                                EXPECT_EQ(x.first().exponents(),
                                    terms.front().first);
                                EXPECT_EQ(x.last().exponents(),
                                    terms[i - 1].first);
                            }
                        }
                    }
                    // Eliminate from the inside out:
                    if (terms.size() > 2) {
                        auto mixed = terms;
                        std::swap(mixed.front(), mixed[mixed.size() - 2]);
                        {
                            regina::RandomEngine r;
                            std::shuffle(mixed.begin(), mixed.end() - 2,
                                r.engine());
                        }
                        L x(c);
                        if (pad)
                            x.reserveRange(c.first().template exp<0>() - 5,
                                c.last().template exp<0>() + 5);
                        for (size_t i = 0; i < mixed.size(); ++i) {
                            set(x, mixed[i].first, 0);
                            validate(x);
                            EXPECT_NE(x, c);
                            if (i < mixed.size() - 2) {
                                EXPECT_EQ(x.first().exponents(),
                                    terms.front().first);
                                EXPECT_EQ(x.last().exponents(),
                                    terms.back().first);
                            } else if (i == mixed.size() - 2) {
                                EXPECT_EQ(x.first().exponents(),
                                    terms.back().first);
                                EXPECT_EQ(x.last().exponents(),
                                    terms.back().first);
                            } else {
                                validateZero(x);
                            }
                        }
                    }
                }
            }

            // Test setting in-range coefficients to zero that are already zero:
            if (terms.size() > 1) {
                L x(c);
                for (size_t i = 0; i < terms.size() - 1; ++i) {
                    std::array<long, vars> exp;
                    for (int j = 0; j < vars; ++j)
                        exp[j] =
                            (terms[i].first[j] + terms[i + 1].first[j]) >> 1;
                    // We want exp to be strictly between the two terms we
                    // just examined.  This might not always happen; e.g.,
                    // integer rounding between exponent sequences (0, 1) and
                    // (1, 0) will give (0, 0).  For now just exclude such
                    // scenarios from the test.
                    if (terms[i].first < exp && exp < terms[i + 1].first) {
                        set(x, exp, 0);
                        validate(x, c);
                    }
                }
            }

            // Verify that the move variant of set() works correctly.
            //
            // The raw data we obtain below are of type __mpz_struct (since
            // mpz_t is the address of an array of type __mpz_struct[1]).
            // We wish to compare the contents of these structs (which contain
            // some size/sign data, plus a pointer to the allocated GMP limbs
            // which is how we tell the data was moved and not copied).
            //
            // Since __mpz_struct does not have a comparison operator and we
            // do not want to rely on internal GMP implementation details
            // (i.e., the names of individual __mpz_struct data members),
            // we use memcmp() for our comparisons.
            L x(c);
            regina::Integer v(bigInt);
            const auto raw = *v.rawData(); // deep copy of initial __mpz_struct

            // Start at any term of the polynomial.
            std::array<long, vars> exp;
            std::fill(exp.begin(), exp.end(), 0);

            set(x, exp, v);
            EXPECT_EQ(coeff(x, exp), bigInt);
            EXPECT_NE(memcmp(coeff(x, exp).rawData(), &raw, sizeof(raw)), 0);

            // Move to any different term of the polynomial.
            ++exp[0];

            set(x, exp, std::move(v));
            EXPECT_EQ(coeff(x, exp), bigInt);
            EXPECT_EQ(memcmp(coeff(x, exp).rawData(), &raw, sizeof(raw)), 0);
        }

        /**
         * Tests iteration, and also coefficient access via operator ()/[]
         * (using LaurentTest::coeff() as a helper function to manage the
         * variable number of arguments).
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyIteration(const regina::Laurent<T, vars>& c) {
            // Here is as good a time as any to test operator()/[] also.

            using L = regina::Laurent<T, vars>;
            const regina::LaurentCoefficient<T, vars> end{};

            if (c.isZero()) {
                EXPECT_FALSE(c.first());
                EXPECT_FALSE(c.last());
                EXPECT_EQ(c.first(), end);
                EXPECT_EQ(c.last(), end);

                withExponents(c, { -100, -1, 0, 1, 100 },
                        [](const L& poly, long exp0, auto... exp) {
                    if constexpr (vars == 1)
                        EXPECT_EQ(poly[exp0], 0);
                    else
                        EXPECT_EQ(poly(exp0, exp...), 0);
                });
            } else {
                ASSERT_TRUE(c.first());
                ASSERT_TRUE(c.last());

                std::array<long, vars> exp;

                // Forward iteration:
                {
                    auto it = c.first();
                    if constexpr (vars == 1)
                        EXPECT_EQ(it.template exp<0>(), c.minExp());

                    exp = it.exponents();
                    for (int i = 0; i < vars; ++i) {
                        EXPECT_NE(coeff(c, exp), 0);
                        --exp[i];
                        EXPECT_EQ(coeff(c, exp), 0);
                        ++exp[i];
                    }

                    std::optional<regina::LaurentCoefficient<T, vars>> prev;
                    for ( ; it; prev = it++) {
                        EXPECT_NE(it, end);
                        EXPECT_NE(*it, 0);
                        if (prev.has_value()) {
                            // Manually verify the order of iteration:
                            EXPECT_LT(prev->exponents(), it.exponents());
                        }
                        EXPECT_EQ(coeff(c, it.exponents()), *it);
                        EXPECT_EQ(it.constantTerm(),
                            std::ranges::all_of(it.exponents(),
                                [](long e) { return e == 0; }));
                    }
                    EXPECT_EQ(it, end);

                    ASSERT_TRUE(prev);
                    ASSERT_TRUE(*prev);
                    EXPECT_EQ(*prev, c.last());
                    if constexpr (vars == 1)
                        EXPECT_EQ(prev->template exp<0>(), c.maxExp());

                    exp = prev->exponents();
                    for (int i = 0; i < vars; ++i) {
                        EXPECT_NE(coeff(c, exp), 0);
                        ++exp[i];
                        EXPECT_EQ(coeff(c, exp), 0);
                        exp[i] += 99;
                        EXPECT_EQ(coeff(c, exp), 0);
                        exp[i] -= 100;
                    }
                }

                // Backward iteration:
                {
                    auto it = c.last();
                    if constexpr (vars == 1)
                        EXPECT_EQ(it.template exp<0>(), c.maxExp());

                    exp = it.exponents();
                    for (int i = 0; i < vars; ++i) {
                        EXPECT_NE(coeff(c, exp), 0);
                        ++exp[i];
                        EXPECT_EQ(coeff(c, exp), 0);
                        --exp[i];
                    }

                    std::optional<regina::LaurentCoefficient<T, vars>> prev;
                    for ( ; it; prev = it--) {
                        EXPECT_NE(it, end);
                        EXPECT_NE(*it, 0);
                        if (prev.has_value()) {
                            // Manually verify the order of iteration:
                            EXPECT_GT(prev->exponents(), it.exponents());
                        }
                        EXPECT_EQ(coeff(c, it.exponents()), *it);
                        EXPECT_EQ(it.constantTerm(),
                            std::ranges::all_of(it.exponents(),
                                [](long e) { return e == 0; }));
                    }
                    EXPECT_EQ(it, end);

                    ASSERT_TRUE(prev);
                    ASSERT_TRUE(*prev);
                    EXPECT_EQ(*prev, c.first());
                    if constexpr (vars == 1)
                        EXPECT_EQ(prev->template exp<0>(), c.minExp());

                    exp = prev->exponents();
                    for (int i = 0; i < vars; ++i) {
                        EXPECT_NE(coeff(c, exp), 0);
                        --exp[i];
                        EXPECT_EQ(coeff(c, exp), 0);
                        exp[i] -= 99;
                        EXPECT_EQ(coeff(c, exp), 0);
                        exp[i] += 100;
                    }
                }
            }
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyComparison(const regina::Laurent<T, vars>& c,
                const regina::Laurent<T, vars>& d,
                const regina::Laurent<T, vars>& e) {
            using L = regina::Laurent<regina::Integer, vars>;

            // Test transitivity of comparisions:
            const auto c_vs_d = c <=> d;
            if (c_vs_d == std::strong_ordering::equal)
                EXPECT_EQ(c <=> e, d <=> e);
            else if ((d <=> e) != (0 <=> c_vs_d))
                EXPECT_EQ(c <=> e, c_vs_d);
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyComparison(const regina::Laurent<T, vars>& c,
                const regina::Laurent<T, vars>& d,
                bool shouldBeEqual) {
            using L = regina::Laurent<regina::Integer, vars>;

            if (shouldBeEqual) {
                EXPECT_TRUE(c == d);
                EXPECT_FALSE(c != d);
                EXPECT_EQ(c <=> d, std::strong_ordering::equal);
            } else {
                EXPECT_FALSE(c == d);
                EXPECT_TRUE(c != d);
                const auto c_vs_d = c <=> d;
                EXPECT_NE(c_vs_d, std::strong_ordering::equal);
                EXPECT_EQ(d <=> c, 0 <=> c_vs_d);
            }
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyComparison(const regina::Laurent<T, vars>& c) {
            using L = regina::Laurent<regina::Integer, vars>;

            EXPECT_TRUE(c == c);
            EXPECT_TRUE(c == L(c));
            EXPECT_FALSE(c != c);
            EXPECT_FALSE(c != L(c));
            EXPECT_EQ(c <=> c, std::strong_ordering::equal);
            EXPECT_EQ(c <=> L(c), std::strong_ordering::equal);
        }

        template <int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyComparison(
                const regina::Laurent<regina::Integer, vars>& c,
                const regina::Integer& value,
                bool shouldBeEqual) {
            EXPECT_EQ(c == value, shouldBeEqual);
            EXPECT_EQ(c != value, ! shouldBeEqual);
        }

        template <int vars, regina::CppInteger Native>
        requires (vars > 0 && vars <= 3)
        static void verifyComparison(
                const regina::Laurent<regina::Integer, vars>& c,
                Native value, bool shouldBeEqual) {
            EXPECT_EQ(c == value, shouldBeEqual);
            EXPECT_EQ(c != value, ! shouldBeEqual);
            verifyComparison(c, regina::Integer(value), shouldBeEqual);
        }

        template <int vars> requires (vars > 0 && vars <= 3)
        static void verifyComparisonBasic() {
            using L = regina::Laurent<regina::Integer, vars>;

            EXPECT_EQ(L(), 0);
            EXPECT_EQ(L(), regina::Integer());
            EXPECT_NE(L(), 2);
            EXPECT_NE(L(), regina::Integer(2));
            EXPECT_NE(L(), bigInt);
            EXPECT_NE(L(2), 0);
            EXPECT_NE(L(2), regina::Integer());
            EXPECT_EQ(L(2), 2);
            EXPECT_EQ(L(2), regina::Integer(2));
            EXPECT_NE(L(2), bigInt);
            EXPECT_NE(L(bigInt), 0);
            EXPECT_NE(L(bigInt), regina::Integer());
            EXPECT_NE(L(bigInt), 2);
            EXPECT_NE(L(bigInt), regina::Integer(2));
            EXPECT_EQ(L(bigInt), bigInt);

            std::array<long, vars> exp;
            std::fill(exp.begin(), exp.end(), 0);
            {
                L m = std::apply([](auto... e) {
                    return L::monomial(e...);
                }, exp);
                EXPECT_NE(m, 0);
                EXPECT_NE(m, regina::Integer(0));
                EXPECT_EQ(m, 1);
                EXPECT_EQ(m, regina::Integer(1));
            }
            for (int i = 0; i < vars; ++i) {
                exp[i] = 2;
                {
                    L m = std::apply([](auto... e) {
                        return L::monomial(e...);
                    }, exp);
                    EXPECT_NE(m, 2);
                    EXPECT_NE(m, regina::Integer(2));
                }
                exp[i] = 0;
                {
                    L m = std::apply([](auto... e) {
                        return L::monomial(e...);
                    }, exp);
                    EXPECT_EQ(m, 1);
                }
            }
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyShift(const regina::Laurent<T, vars>& c) {
            using L = regina::Laurent<T, vars>;

            withExponents(c, { -100, -5, 0, 5, 100 },
                    [](const L& poly, auto... exp) {
                const Exponents<vars> shifts{exp...};
                L s = poly.shifted(exp...);
                validate(s, poly, [shifts](Exponents<vars> e, const T& v) {
                    for (int i = 0; i < vars; ++i)
                        e[i] += shifts[i];
                    return std::make_pair(e, v);
                });
                validate(L(poly).shifted(exp...), s);
                {
                    L x(poly);
                    auto alloc = x.allocation();
                    x.shift(exp...);
                    validate(x, s);
                    if (poly.isZero()) {
                        EXPECT_EQ(x.allocation(), alloc);
                    } else {
                        EXPECT_EQ(x.allocation().first,
                            alloc.first + shifts[0]);
                        EXPECT_EQ(x.allocation().second, alloc.second);
                    }
                }
            });

            // The zero shift is already tested above, but verify explicitly
            // that it matches the original polynomial:
            if constexpr (vars == 1)
                validate(c.shifted(0), c);
            else if constexpr (vars == 2)
                validate(c.shifted(0, 0), c);
            else // vars == 3
                validate(c.shifted(0, 0, 0), c);
        }

        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyNegate(const regina::Laurent<T, vars>& c) {
            using L = regina::Laurent<T, vars>;

            L neg = -c;
            validate(neg, c, [](const Exponents<vars>& e, const T& v) {
                return std::make_pair(e, -v);
            });
            validate(-L(c), neg);
            {
                L x(c);
                x.negate();
                validate(x, neg);
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

        template <regina::CppInteger Native,
            regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyNativeProduct(const regina::Laurent<T, vars>& poly,
                const T& scalar, const regina::Laurent<T, vars>& expect) {
            using L = regina::Laurent<T, vars>;

            SCOPED_TRACE_TYPE(Native);

            Native native;
            if (scalar >= std::numeric_limits<Native>::min() &&
                    scalar <= std::numeric_limits<Native>::max()) {
                EXPECT_NO_THROW(
                    { native = scalar.template safeValue<Native>(); });
            } else {
                EXPECT_THROW({ scalar.template safeValue<Native>(); },
                    regina::IntegerOverflow);
                return;
            }

            validate(poly * native, expect);
            validate(L(poly) * native, expect);
            validate(native * poly, expect);
            validate(native * L(poly), expect);
            {
                L x(poly);
                validate(x *= native, expect);
            }
        }

        template <regina::CppInteger Native, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyNativeQuotient(
                const regina::Laurent<regina::Integer, vars>& poly,
                const regina::Integer& scalar,
                const regina::Laurent<regina::Integer, vars>& expect) {
            // It is assumed that scalar != 0 (which is currently a
            // requirement of scalar division for Laurent polynomials).
            using L = regina::Laurent<regina::Integer, vars>;

            SCOPED_TRACE_TYPE(Native);

            Native native;
            if (scalar >= std::numeric_limits<Native>::min() &&
                    scalar <= std::numeric_limits<Native>::max()) {
                EXPECT_NO_THROW(
                    { native = scalar.template safeValue<Native>(); });
            } else {
                EXPECT_THROW({ scalar.template safeValue<Native>(); },
                    regina::IntegerOverflow);
                return;
            }

            validate(poly / native, expect);
            validate(L(poly) / native, expect);
            {
                L x(poly);
                validate(x /= native, expect);
            }
        }

        template <int vars> requires (vars > 0 && vars <= 3)
        static void verifyScalarMultiplyDivide(
                const regina::Laurent<regina::Integer, vars>& poly,
                const regina::Integer& scalar) {
            using L = regina::Laurent<regina::Integer, vars>;

            SCOPED_TRACE_REGINA(scalar);

            const L product = poly * scalar;
            if (scalar == 0) {
                validateZero(product);
            } else {
                validate(product, poly, [&scalar](const Exponents<vars>& e,
                        const regina::Integer& v) {
                    return std::make_pair(e, v * scalar);
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

            // For now, scalar division for Laurent polynomials requires a
            // non-zero divisor.
            if (scalar != 0) {
                const L quotient = poly / scalar;
                if (poly.isZero()) {
                    validateZero(quotient);
                } else {
                    validate(quotient);
                    if constexpr (vars == 1) {
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
                    } else {
                        auto pit = poly.first();
                        auto qit = quotient.first();
                        while (pit || qit) {
                            if (pit && qit &&
                                    pit.exponents() == qit.exponents()) {
                                auto q = *pit++ / scalar;
                                EXPECT_NE(q, 0);
                                EXPECT_EQ(*qit++, q);
                            } else if (pit) {
                                // We might have a coefficient which, when
                                // divided by our scalar using integer division,
                                // becomes zero.
                                EXPECT_GT(*pit, -scalar.abs());
                                EXPECT_LT(*pit, scalar.abs());
                                EXPECT_EQ(*pit++ / scalar, 0);
                            } else {
                                // We have a deeper mismatch between terms that
                                // we do not know how to recover from.
                                ADD_FAILURE();
                                break;
                            }
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
                    verifyNativeQuotient<regina::UInt128>(poly, scalar,
                        quotient);
                    #endif
                }
            }
        }

        template <int vars> requires (vars > 0 && vars <= 3)
        static void verifyScalarMultiplyDivide(
                const regina::Laurent<regina::Integer, vars>& poly) {
            withScalars([&poly](const regina::Integer& scalar) {
                verifyScalarMultiplyDivide(poly, scalar);
            });

            // Test expected arithmetic properties:
            validateZero(poly * 0);
            validate(poly * 1, poly);
            validate(poly * -1, -poly);
            validate(poly * 2, poly + poly);

            if (! poly.isZero()) {
                {
                    const auto& t = poly.first();
                    verifyScalarMultiplyDivide(poly, *t);

                    regina::Laurent<regina::Integer, vars> p0 = poly / *t;
                    const auto& t0 = p0.first();
                    EXPECT_EQ(t0.exponents(), t.exponents());
                    EXPECT_EQ(*t0, 1);
                }
                {
                    const auto& t = poly.last();
                    verifyScalarMultiplyDivide(poly, *t);

                    regina::Laurent<regina::Integer, vars> p0 = poly / *t;
                    const auto& t0 = p0.last();
                    EXPECT_EQ(t0.exponents(), t.exponents());
                    EXPECT_EQ(*t0, 1);
                }
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
                validate(diff, d, [](const Exponents<vars>& e, const T& v) {
                    return std::make_pair(e, -v);
                });
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
            validate(dbl, c, [](const Exponents<vars>& e, const T& v) {
                return std::make_pair(e, v * 2);
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

            // Test expected arithmetic properties:
            static const regina::Laurent<T, vars> zero;
            validate(c + zero, c);
            validate(zero + c, c);
            validate(c - zero, c);
            validate(zero - c, -c);
            validateZero(c + -c);
        }

        /**
         * Tests product operations involving two polynomials.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyMultiply(const regina::Laurent<T, vars>& c,
                const regina::Laurent<T, vars>& d) {
            using L = regina::Laurent<T, vars>;

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

                if constexpr (vars == 1) {
                    EXPECT_EQ(product.minExp(), c.minExp() + d.minExp());
                    EXPECT_EQ(product.maxExp(), c.maxExp() + d.maxExp());
                    for (long i = product.minExp(); i <= product.maxExp();
                            ++i) {
                        long cExp = std::max(c.minExp(), i - d.maxExp());
                        long dExp = i - cExp;
                        T coeff;
                        while (cExp <= c.maxExp() && dExp >= d.minExp())
                            coeff += (c[cExp++] * d[dExp--]);
                        EXPECT_EQ(product[i], coeff);
                    }
                } else {
                    // We've already tested addition and scalar multiplication,
                    // so we can attempt to reimplement multiplication in terms
                    // of these.
                    L expect;
                    for (auto it = d.first(); it; ++it)
                        std::apply([&](auto... exp) {
                            expect += (c * *it).shifted(exp...);
                        }, it.exponents());
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

                // Test expected arithmetic properties:
                validate(c * d, d * c);

                if constexpr (vars == 1) {
                    // Verify that our different multiplication algorithms
                    // give the same results:
                    using PPA = regina::PolynomialProductAlgorithm;
                    using Array = regina::FixedArray<T>;

                    auto cSeq = coefficientSequence(c);
                    auto dSeq = coefficientSequence(d);

                    auto def = polynomialProduct<PPA::Default>(
                        Array(cSeq.begin(), cSeq.end()),
                        Array(dSeq.begin(), dSeq.end()));
                    auto classic = polynomialProduct<PPA::Classic>(
                        Array(cSeq.begin(), cSeq.end()),
                        Array(dSeq.begin(), dSeq.end()));
                    EXPECT_EQ(def, classic);

                    long cSpan = c.maxExp() - c.minExp();
                    long dSpan = d.maxExp() - d.minExp();
                    if (cSpan >= 1 && dSpan >= 1 &&
                            (2 * cSpan <= dSpan || 2 * dSpan <= cSpan)) {
                        EXPECT_THROW({
                            polynomialProduct<PPA::Karatsuba>(
                                Array(cSeq.begin(), cSeq.end()),
                                Array(dSeq.begin(), dSeq.end()));
                        }, regina::InvalidArgument);
                    } else {
                        EXPECT_NO_THROW({
                            auto karatsuba = polynomialProduct<PPA::Karatsuba>(
                                Array(cSeq.begin(), cSeq.end()),
                                Array(dSeq.begin(), dSeq.end()));
                            EXPECT_EQ(def, karatsuba);
                        });
                    }
                }
            }
        }

        /**
         * Tests product operations involving a single polynomial.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyMultiply(const regina::Laurent<T, vars>& c) {
            using L = regina::Laurent<T, vars>;

            // Test operations upon one's self:
            const L copy(c);
            const L square = c * copy;
            if (c.isZero()) {
                validateZero(square);
            } else {
                EXPECT_FALSE(square.isZero());
                if constexpr (vars == 1) {
                    validate(square);
                    EXPECT_EQ(square.minExp(), 2 * c.minExp());
                    EXPECT_EQ(square.maxExp(), 2 * c.maxExp());
                    for (long i = square.minExp(); i <= square.maxExp(); ++i) {
                        long lhsExp = std::max(c.minExp(), i - c.maxExp());
                        long rhsExp = i - lhsExp;
                        T coeff;
                        while (lhsExp <= c.maxExp() && rhsExp >= c.minExp())
                            coeff += (c[lhsExp++] * c[rhsExp--]);
                        EXPECT_EQ(square[i], coeff);
                    }
                } else {
                    // As above, reimplement multiplication in terms of
                    // addition and scalar multiplication.
                    L expect;
                    for (auto it = c.first(); it; ++it)
                        std::apply([&](auto... exp) {
                            expect += (c * *it).shifted(exp...);
                        }, it.exponents());
                    validate(square, expect);
                }
            }
            validate(c * c, square);
            {
                L x(c);
                validate(x *= x, square);
            }

            // More expected arithmetic properties:
            validate(c * L(1), c);
            validate(L(1) * c, c);
            validate(c * L(2), c + c);
            validate(L(2) * c, c + c);
            validate(c * L(-1), -c);
            validate(L(-1) * c, -c);

            withExponents(c, { -2, -1, 0, 1, 2 },
                    [](const L& poly, auto... exp) {
                validate(poly * L::monomial(exp...), poly.shifted(exp...));
                validate(L::monomial(exp...) * poly, poly.shifted(exp...));
            });
        }

        /**
         * Tests scalar addProduct() operations involving two polynomials.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyScalarAddProduct(const regina::Laurent<T, vars>& c,
                const regina::Laurent<T, vars>& d) {
            using L = regina::Laurent<T, vars>;

            withScalars([&c, &d](const regina::Integer& scalar) {
                const L expect = c + d * scalar;
                {
                    L x(c);
                    x.addProduct(d, scalar);
                    validate(x, expect);
                }
                {
                    L x(c);
                    x.addProduct(L(d), scalar);
                    validate(x, expect);
                }
            });
        }

        /**
         * Tests scalar addProduct() operations involving a single polynomial.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyScalarAddProduct(const regina::Laurent<T, vars>& c) {
            using L = regina::Laurent<T, vars>;

            // Here we just test operations upon one's self.
            withScalars([&c](const regina::Integer& scalar) {
                const L expect = c + c * scalar;
                {
                    L x(c);
                    x.addProduct(x, scalar);
                    validate(x, expect);
                }
            });
        }

        /**
         * Tests polynomial addProduct() operations involving three polynomials.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyPolynomialAddProduct(
                const regina::Laurent<T, vars>& c,
                const regina::Laurent<T, vars>& d,
                const regina::Laurent<T, vars>& e) {
            using L = regina::Laurent<T, vars>;

            const L expect = c + d * e;
            {
                L x(c);
                x.addProduct(d, e);
                validate(x, expect);
            }
            {
                L x(c);
                x.addProduct(L(d), L(e));
                validate(x, expect);
            }
        }

        /**
         * Tests polynomial addProduct() operations involving two polynomials.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyPolynomialAddProduct(
                const regina::Laurent<T, vars>& c,
                const regina::Laurent<T, vars>& d) {
            using L = regina::Laurent<T, vars>;

            // Test operations where two of the three operands are the same
            // object.
            {
                const L expect = c + d * d;
                L x(c);
                x.addProduct(d, d);
                validate(x, expect);
            }
            {
                const L expect = c * (d + L(1)); // c + c * d
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
        }

        /**
         * Tests polynomial addProduct() operations involving a single
         * polynomial.
         */
        template <regina::CoefficientDomain T, int vars>
        requires (vars > 0 && vars <= 3)
        static void verifyPolynomialAddProduct(
                const regina::Laurent<T, vars>& c) {
            using L = regina::Laurent<T, vars>;

            // Test operations where all three operands are the same object.
            {
                const L expect = c * (c + L(1)); // c + c * c
                L x(c);
                x.addProduct(x, x);
                validate(x, expect);
            }
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

