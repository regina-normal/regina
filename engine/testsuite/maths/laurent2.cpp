
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
#include "maths/laurent2.h"
#include "utilities/stringutils.h"
#include <array>

#include "utilities/tightencodingtest.h"

using regina::Integer;
using regina::Laurent2;

using L = Laurent2<Integer>;
using Term = std::pair<std::pair<long, long>, Integer>;

class Laurent2Test : public testing::Test {
    protected:
        const L zero {};
        const L one { {0, 0, 1} };
        const L two { {0, 0, 2} };
        const L x2 { {2, 0, 1} };
        const L xy { {1, 1, 1} };
        const L a { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} };
        const L b { {1, 0, 2}, {1, 2, -4}, {0, -1, -2} };

        const std::array<std::reference_wrapper<const L>, 7> cases {
            std::cref(zero), std::cref(one), std::cref(two),
            std::cref(x2), std::cref(xy), std::cref(a), std::cref(b) };
};

void validate(const L& poly) {
    EXPECT_NO_THROW({ poly.validate(); });
}

void validate(const L& poly,
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

void validateZero(const L& poly) {
    EXPECT_NO_THROW({ poly.validate(); });
    EXPECT_TRUE(poly.isZero());
    EXPECT_EQ(poly.countTerms(), 0);
    EXPECT_EQ(poly.begin(), poly.end());
}

void validate(const L& poly, const L& expect) {
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
void validate(const L& poly, const L& from, TranslateTerm&& translate) {
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

void verifyMult(const L& x, const Integer& y,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_REGINA(y);

    const L zero;

    validate(x * y, coeffs);
    validate((x + zero) * y, coeffs);
    validate(y * x, coeffs);
    validate(y * (x + zero), coeffs);
    {
        L z(x);
        validate(z *= y, coeffs);
    }
}

template <regina::CppInteger U>
void verifyMult(const L& x, U y,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_INTEGER(y);

    const L zero;

    validate(x * y, coeffs);
    validate((x + zero) * y, coeffs);
    validate(y * x, coeffs);
    validate(y * (x + zero), coeffs);
    {
        L z(x);
        validate(z *= y, coeffs);
    }

    verifyMult(x, Integer(y), coeffs);
}

void verifyDiv(const L& x, const Integer& y,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_REGINA(y);

    const L zero;

    validate(x / y, coeffs);
    validate((x + zero) / y, coeffs);
    {
        L z(x);
        validate(z /= y, coeffs);
    }
}

void verifyMult(const L& x, const L& y,
        std::initializer_list<std::tuple<long, long, Integer>> coeffs) {
    SCOPED_TRACE_REGINA(x);
    SCOPED_TRACE_REGINA(y);

    const L zero;

    validate(x * y, coeffs);
    validate((x + zero) * y, coeffs);
    validate(x * (y + zero), coeffs);
    validate((x + zero) * (y + zero), coeffs);

    validate(y * x, coeffs);
    validate((y + zero) * x, coeffs);
    validate(y * (x + zero), coeffs);
    validate((y + zero) * (x + zero), coeffs);

    {
        L z(x);
        validate(z *= y, coeffs);
    }
    {
        L z(x);
        validate(z *= (y + zero), coeffs);
    }
    {
        L z(y);
        validate(z *= x, coeffs);
    }
    {
        L z(y);
        validate(z *= (x + zero), coeffs);
    }
}

TEST_F(Laurent2Test, arithmetic) {
    verifyMult(zero, 0, {});
    verifyMult(zero, 1, {});
    verifyMult(zero, 2, {});
    verifyMult(a, 0, {});
    verifyMult(a, 1, { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} });
    verifyMult(a, -1, { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });
    verifyMult(a, 2, { {0, 0, 2}, {1, 2, -4}, {-2, -1, 6} });

    verifyDiv(zero, 1, {});
    verifyDiv(zero, 2, {});
    verifyDiv(a, 1, { {0, 0, 1}, {1, 2, -2}, {-2, -1, 3} });
    verifyDiv(a, -1, { {0, 0, -1}, {1, 2, 2}, {-2, -1, -3} });
    verifyDiv(b, 2, { {1, 0, 1}, {1, 2, -2}, {0, -1, -1} });

    verifyMult(zero, zero, {});
    verifyMult(zero, a, {});
    verifyMult({ {1, 0, 1}, {0, -1, 1} },
               { {1, 0, 1}, {0, -1, -1} },
               { {2, 0, 1}, {0, -2, -1} });
    verifyMult(a, xy, { {1, 1, 1}, {2, 3, -2}, {-1, 0, 3} });

    {
        L x { {0, -1, 1}, {0, 1, 1} };
        validate(x * x, { {0, -2, 1}, {0, 0, 2}, {0, 2, 1} });
        validate(x *= x, { {0, -2, 1}, {0, 0, 2}, {0, 2, 1} });
    }
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
