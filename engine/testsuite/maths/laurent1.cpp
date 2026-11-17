
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
#include "maths/polynomial.h"
#include "utilities/stringutils.h"
#include <array>
#include <ranges>

#include "maths/laurenttest.h"
#include "utilities/tightencodingtest.h"

using regina::CoefficientDomain;
using regina::Integer;
using regina::Laurent;
using regina::polynomialProduct;

using L = Laurent<Integer>;

class Laurent1Test : public LaurentTest {
    protected:
        // The coefficient sequence for the constant polynomial bigInt.
        const std::array<regina::Integer, 1> bigCoeff { bigInt };

        const L zero {};
        const L zero2 { 0, {} };
        const L zero3 { 2, {} };
        const L one { 0, { 1 } };
        const L two { 0, { 2 } };
        const L minusOne { 0, { -1 } };
        const L x2 { 2, { 1 } };
        const L x5Inv { -5, { 1 } };
        const L a { -1, { 1, -1, 1 } };
        const L b { 0, { 1, -1, 1 } };
        const L c { 1, { 1, -1, 1 } };
        const L d { -2, { -1, 1, -1, 1 } };
        const L e { 4, { 2, 4, -2, 2 } };
        const L f { 3, { -1, 0, 0, 0, 1 } };
        const L g { 20, { 2, -3 } };

        // Several ranges each of which overlap at a single coefficient:
        const L low { -7, { 1, -2, 3, -4 } };
        const L lowish { -4, { -1, 2, -3, 4 } };
        const L mid { -1, { 1, -2, 3 } };
        const L highish { 1, { 1, -2, -3, 4 } };
        const L high { 4, { -1, 2, 3, -4 } };

        // The same polynomials as before, but this time forcing large integer
        // arithmetic:
        const L bigLow { -7, { bigInt, bigInt * -2, bigInt * 3, bigInt * -4 } };
        const L bigLowish { -4,
            { -bigInt, bigInt * 2, bigInt * -3, bigInt * 4 } };
        const L bigMid { -1, { bigInt, bigInt * -2, bigInt * 3 } };
        const L bigHighish { 1,
            { bigInt, bigInt * -2, bigInt * -3, bigInt * 4 } };
        const L bigHigh { 4, { -bigInt, bigInt * 2, bigInt * 3, bigInt * -4 } };

        // Some longer polynomials:
        const L long1 { 2, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };
        const L long2 { 3, { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 } };

        // Some polynomials with more space allocated than they need:
        const L paddedZero = g - g;
        const L paddedConst = (two + d) - d;
        const L paddedPower = (x2 + highish) - highish;
        const L paddedPoly = ((c + low + high) - low) - high;

        const std::array<std::reference_wrapper<const L>, 31> cases {
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

        // Note which of our cases represent the same mathematical polynomial.
        enum class Batch { Unique, Zero, Two, X2, Short };
        Batch batch(const L& poly) {
            if (std::addressof(poly) == std::addressof(zero) ||
                    std::addressof(poly) == std::addressof(zero2) ||
                    std::addressof(poly) == std::addressof(zero3) ||
                    std::addressof(poly) == std::addressof(paddedZero))
                return Batch::Zero;
            else if (std::addressof(poly) == std::addressof(two) ||
                    std::addressof(poly) == std::addressof(paddedConst))
                return Batch::Two;
            else if (std::addressof(poly) == std::addressof(x2) ||
                    std::addressof(poly) == std::addressof(paddedPower))
                return Batch::X2;
            else if (std::addressof(poly) == std::addressof(c) ||
                    std::addressof(poly) == std::addressof(paddedPoly))
                return Batch::Short;
            else
                return Batch::Unique;
        }
};

TEST_F(Laurent1Test, construct) {
    // Start by verifying our hand-coded test cases.
    // This also effectively tests isZero() via the calls to validate() and
    // validateZero().
    validateZero(zero);
    validateZero(zero2);
    validateZero(zero3);
    validate(one, 0, { 1 });
    validate(two, 0, { 2 });
    validate(minusOne, 0, { -1 });
    validate(x2, 2, { 1 });
    validate(x5Inv, -5, { 1 });
    validate(a, -1, { 1, -1, 1 });
    validate(b, 0, { 1, -1, 1 });
    validate(c, 1, { 1, -1, 1 });
    validate(d, -2, { -1, 1, -1, 1 });
    validate(e, 4, { 2, 4, -2, 2 });
    validate(f, 3, { -1, 0, 0, 0, 1 });
    validate(g, 20, { 2, -3 });
    validate(low, -7, { 1, -2, 3, -4 });
    validate(lowish, -4, { -1, 2, -3, 4 });
    validate(mid, -1, { 1, -2, 3 });
    validate(highish, 1, { 1, -2, -3, 4 });
    validate(high, 4, { -1, 2, 3, -4 });
    validate(bigLow, -7, { bigInt, bigInt * -2, bigInt * 3, bigInt * -4 });
    validate(bigLowish, -4,
            { -bigInt, bigInt * 2, bigInt * -3, bigInt * 4 });
    validate(bigMid, -1, { bigInt, bigInt * -2, bigInt * 3 });
    validate(bigHighish, 1,
            { bigInt, bigInt * -2, bigInt * -3, bigInt * 4 });
    validate(bigHigh, 4, { -bigInt, bigInt * 2, bigInt * 3, bigInt * -4 });
    validate(long1, 2, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });
    validate(long2, 3, { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 });
    validateZero(paddedZero);
    validate(paddedConst, 0, {2});
    validate(paddedPower, 2, {1});
    validate(paddedPoly, 1, { 1, -1, 1 });

    // On to our generic tests:
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyConstruct(c);
    }

    verifyConstructBasic<1>();
}

TEST_F(Laurent1Test, assign) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyAssign(c, d);
        }
        verifyAssign(c);
    }
}

TEST_F(Laurent1Test, padding) {
    verifyPadding(paddedZero, zero);
    verifyPadding(paddedConst, two);
    verifyPadding(paddedPower, x2);
    verifyPadding(paddedPoly, c);

    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyPadding(padded(c), c);
    }
}

TEST_F(Laurent1Test, reserveRange) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyReserveRange(c);
    }
}

TEST_F(Laurent1Test, init) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyInit(c, -2);
    }
}

TEST_F(Laurent1Test, moveThenAssign) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            for (const L& e : cases) {
                SCOPED_TRACE_REGINA(e);
                verifyMoveThenAssign(c, d, std::addressof(d));
            }
            verifyMoveThenAssign(c, d, nullptr);
            verifyMoveThenAssign(c, std::addressof(d));
        }
        verifyMoveThenAssign(c, nullptr);
    }
}

TEST_F(Laurent1Test, set) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifySet(c);
    }
    verifySetFromZero<1>();
}

TEST_F(Laurent1Test, iteration) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyIteration(c);
    }
}

TEST_F(Laurent1Test, comparison) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        const auto cBatch = batch(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            const auto dBatch = batch(d);
            for (const L& e : cases) {
                SCOPED_TRACE_REGINA(e);
                verifyComparison(c, d, e);
            }
            verifyComparison(c, d,
                std::addressof(c) == std::addressof(d) ||
                (cBatch == dBatch && cBatch != Batch::Unique));
        }
        verifyComparison(c);

        // Comparisons against integers that appear in some of our tests:
        verifyComparison(c, 0, cBatch == Batch::Zero);
        verifyComparison(c, 2, cBatch == Batch::Two);
        verifyComparison(c, 1, std::addressof(c) == std::addressof(one));
        verifyComparison(c, -1, std::addressof(c) == std::addressof(minusOne));

        // Comparisons against integers that appear in none of our tests:
        verifyComparison(c, 3, false);
        verifyComparison(c, bigInt, false);
    }

    verifyComparisonBasic<1>();
}

TEST_F(Laurent1Test, shift) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyShift(c);
    }
}

TEST_F(Laurent1Test, scale) {
    // This test is specific to single-variable Laurent polynomials.
    static constexpr int factors[6] = { 1, -1, 2, -2, 5, -5 };
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        {
            L x(c);
            EXPECT_THROW({ x.scaleUp(0); }, regina::InvalidArgument);
        }
        {
            L x(c);
            EXPECT_THROW({ x.scaleDown(0); }, regina::InvalidArgument);
        }
        for (int factor : factors) {
            SCOPED_TRACE_NUMERIC(factor);
            if (c.isZero()) {
                {
                    L x(c);
                    EXPECT_NO_THROW({ x.scaleUp(factor); });
                    validateZero(x);
                }
                {
                    L x(c);
                    EXPECT_NO_THROW({ x.scaleDown(factor); });
                    validateZero(x);
                }
            } else {
                {
                    L x(c);
                    EXPECT_NO_THROW({ x.scaleUp(factor); });
                    validate(x);
                    EXPECT_FALSE(x.isZero());
                    if (factor >= 0) {
                        EXPECT_EQ(x.minExp(), c.minExp() * factor);
                        EXPECT_EQ(x.maxExp(), c.maxExp() * factor);
                    } else {
                        EXPECT_EQ(x.maxExp(), c.minExp() * factor);
                        EXPECT_EQ(x.minExp(), c.maxExp() * factor);
                    }
                    for (long i = x.minExp(); i <= x.maxExp(); ++i)
                        if (i % factor == 0)
                            EXPECT_EQ(x[i], c[i / factor]);
                        else
                            EXPECT_EQ(x[i], 0);
                }
                bool canScaleDown = true;
                for (long i = c.minExp(); i <= c.maxExp(); ++i)
                    if (c[i] != 0 && i % factor != 0) {
                        canScaleDown = false;
                        break;
                    }
                if (canScaleDown) {
                    L x(c);
                    EXPECT_NO_THROW({ x.scaleDown(factor); });
                    validate(x);
                    EXPECT_FALSE(x.isZero());
                    if (factor >= 0) {
                        EXPECT_EQ(x.minExp() * factor, c.minExp());
                        EXPECT_EQ(x.maxExp() * factor, c.maxExp());
                    } else {
                        EXPECT_EQ(x.maxExp() * factor, c.minExp());
                        EXPECT_EQ(x.minExp() * factor, c.maxExp());
                    }
                    for (long i = x.minExp(); i <= x.maxExp(); ++i)
                        EXPECT_EQ(x[i], c[i * factor]);
                } else {
                    L x(c);
                    EXPECT_THROW({ x.scaleDown(factor); },
                        regina::FailedPrecondition);
                }

                {
                    L x(c);
                    EXPECT_NO_THROW({ x.scaleUp(factor); });
                    EXPECT_NO_THROW({ x.scaleDown(factor); });
                    validate(x, c);
                }
                {
                    L x(c);
                    EXPECT_NO_THROW({ x.scaleUp(1); });
                    validate(x, c);
                }
                {
                    L x(c);
                    EXPECT_NO_THROW({ x.scaleDown(1); });
                    validate(x, c);
                }
                {
                    L x(c), y(c);
                    EXPECT_NO_THROW({ x.scaleUp(-1); });
                    y.invert<0>();
                    validate(x);
                    validate(y);
                    EXPECT_EQ(x, y);
                }
                {
                    L x(c), y(c);
                    EXPECT_NO_THROW({ x.scaleDown(-1); });
                    y.invert<0>();
                    validate(x);
                    validate(y);
                    EXPECT_EQ(x, y);
                }
            }
        }
    }
}

TEST_F(Laurent1Test, negate) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyNegate(c);
    }
}

TEST_F(Laurent1Test, invert) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        L x(c);
        x.invert<0>();

        if (c.isZero()) {
            validateZero(x);
        } else {
            validate(x);
            EXPECT_FALSE(x.isZero());

            EXPECT_EQ(x.maxExp(), -c.minExp());
            EXPECT_EQ(x.minExp(), -c.maxExp());
            long cExp = c.minExp();
            long xExp = -c.minExp();
            while (cExp <= c.maxExp()) {
                EXPECT_EQ(x[xExp], c[cExp]);
                ++cExp;
                --xExp;
            }
        }
    }
}

TEST_F(Laurent1Test, transform) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyTransform(c);
    }
}

TEST_F(Laurent1Test, scalarMultiplyDivide) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyScalarMultiplyDivide(c);
    }
}

TEST_F(Laurent1Test, addSubtract) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyAddSubtract(c, d);
        }
        verifyAddSubtract(c);
    }
}

TEST_F(Laurent1Test, multiply) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyMultiply(c, d);
        }
        verifyMultiply(c);
    }
}

TEST_F(Laurent1Test, scalarAddProduct) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyScalarAddProduct(c, d);
        }
        verifyScalarAddProduct(c);
    }
}

TEST_F(Laurent1Test, polynomialAddProduct) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            for (const L& e : cases) {
                SCOPED_TRACE_REGINA(e);
                verifyPolynomialAddProduct(c, d, e);
            }
            verifyPolynomialAddProduct(c, d);
        }
        verifyPolynomialAddProduct(c);
    }
}

TEST_F(Laurent1Test, polynomial) {
    // This test is specific to single-variable Laurent polynomials.
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        auto p = c.polynomial();

        if (c.isZero()) {
            EXPECT_TRUE(p.first.isZero());
            EXPECT_EQ(p.second, 0);
        } else {
            ASSERT_FALSE(p.first.isZero());
            ASSERT_EQ(p.first.degree(), c.maxExp() - c.minExp());

            EXPECT_NE(p.first[0], 0);
            EXPECT_EQ(p.second, c.minExp());

            size_t i = 0;
            long j = c.minExp();
            while (j <= c.maxExp()) {
                EXPECT_EQ(p.first[i++], c[j++]);
            }
        }
    }
}

TEST_F(Laurent1Test, ringConstants) {
    verifyRingConstants<L>();
}

TEST_F(Laurent1Test, str) {
    EXPECT_EQ(zero.str(), "0");
    EXPECT_EQ(zero2.str(), "0");
    EXPECT_EQ(zero3.str(), "0");
    EXPECT_EQ(one.str(), "1");
    EXPECT_EQ(two.str(), "2");
    EXPECT_EQ(minusOne.str(), "-1");
    EXPECT_EQ(x2.str(), "x^2");
    EXPECT_EQ(x5Inv.str(), "x^-5");
    EXPECT_EQ(a.str(), "x - 1 + x^-1");
    EXPECT_EQ(b.str(), "x^2 - x + 1");
    EXPECT_EQ(c.str(), "x^3 - x^2 + x");
    EXPECT_EQ(d.str(), "x - 1 + x^-1 - x^-2");
    EXPECT_EQ(e.str(), "2 x^7 - 2 x^6 + 4 x^5 + 2 x^4");
    EXPECT_EQ(f.str(), "x^7 - x^3");
    EXPECT_EQ(g.str(), "-3 x^21 + 2 x^20");

    EXPECT_EQ(low.str(), "-4 x^-4 + 3 x^-5 - 2 x^-6 + x^-7");
    EXPECT_EQ(lowish.str(), "4 x^-1 - 3 x^-2 + 2 x^-3 - x^-4");
    EXPECT_EQ(mid.str(), "3 x - 2 + x^-1");
    EXPECT_EQ(highish.str(), "4 x^4 - 3 x^3 - 2 x^2 + x");
    EXPECT_EQ(high.str(), "-4 x^7 + 3 x^6 + 2 x^5 - x^4");

    EXPECT_EQ(bigLow.str(),
        "-" BIG4 " x^-4 + " BIG3 " x^-5 - " BIG2 " x^-6 + " BIG " x^-7");
    EXPECT_EQ(bigLowish.str(),
        BIG4 " x^-1 - " BIG3 " x^-2 + " BIG2 " x^-3 - " BIG " x^-4");
    EXPECT_EQ(bigMid.str(),
        BIG3 " x - " BIG2 " + " BIG " x^-1");
    EXPECT_EQ(bigHighish.str(),
        BIG4 " x^4 - " BIG3 " x^3 - " BIG2 " x^2 + " BIG " x");
    EXPECT_EQ(bigHigh.str(),
        "-" BIG4 " x^7 + " BIG3 " x^6 + " BIG2 " x^5 - " BIG " x^4");

    EXPECT_EQ(long1.str(), "x^14 + x^13 + x^12 + x^11 + x^10 + x^9 + x^8 + "
        "x^7 + x^6 + x^5 + x^4 + x^3 + x^2");
    EXPECT_EQ(long2.str(), "x^13 - x^3");

    EXPECT_EQ(paddedZero.str(), "0");
    EXPECT_EQ(paddedConst.str(), "2");
    EXPECT_EQ(paddedPower.str(), "x^2");
    EXPECT_EQ(paddedPoly.str(), "x^3 - x^2 + x");
}

TEST_F(Laurent1Test, utf8) {
    // Here we directly use unicode superscripts and minus signs in the C++
    // source code.
    verifyUtf8(zero, u8"0");
    verifyUtf8(zero2, u8"0");
    verifyUtf8(zero3, u8"0");
    verifyUtf8(one, u8"1");
    verifyUtf8(two, u8"2");
    verifyUtf8(minusOne, u8"−1");
    verifyUtf8(x2, u8"x²");
    verifyUtf8(x5Inv, u8"x⁻⁵");
    verifyUtf8(a, u8"x − 1 + x⁻¹");
    verifyUtf8(b, u8"x² − x + 1");
    verifyUtf8(c, u8"x³ − x² + x");
    verifyUtf8(d, u8"x − 1 + x⁻¹ − x⁻²");
    verifyUtf8(e, u8"2 x⁷ − 2 x⁶ + 4 x⁵ + 2 x⁴");
    verifyUtf8(f, u8"x⁷ − x³");
    verifyUtf8(g, u8"−3 x²¹ + 2 x²⁰");

    verifyUtf8(low, u8"−4 x⁻⁴ + 3 x⁻⁵ − 2 x⁻⁶ + x⁻⁷");
    verifyUtf8(lowish, u8"4 x⁻¹ − 3 x⁻² + 2 x⁻³ − x⁻⁴");
    verifyUtf8(mid, u8"3 x − 2 + x⁻¹");
    verifyUtf8(highish, u8"4 x⁴ − 3 x³ − 2 x² + x");
    verifyUtf8(high, u8"−4 x⁷ + 3 x⁶ + 2 x⁵ − x⁴");

    verifyUtf8(bigLow,
        u8"−" BIG4 " x⁻⁴ + " BIG3 " x⁻⁵ − " BIG2 " x⁻⁶ + " BIG " x⁻⁷");
    verifyUtf8(bigLowish,
        u8"" BIG4 " x⁻¹ − " BIG3 " x⁻² + " BIG2 " x⁻³ − " BIG " x⁻⁴");
    verifyUtf8(bigMid,
        u8"" BIG3 " x − " BIG2 " + " BIG " x⁻¹");
    verifyUtf8(bigHighish,
        u8"" BIG4 " x⁴ − " BIG3 " x³ − " BIG2 " x² + " BIG " x");
    verifyUtf8(bigHigh,
        u8"−" BIG4 " x⁷ + " BIG3 " x⁶ + " BIG2 " x⁵ − " BIG " x⁴");

    verifyUtf8(long1, u8"x¹⁴ + x¹³ + x¹² + x¹¹ + x¹⁰ + x⁹ + x⁸ + "
        "x⁷ + x⁶ + x⁵ + x⁴ + x³ + x²");
    verifyUtf8(long2, u8"x¹³ − x³");

    verifyUtf8(paddedZero, u8"0");
    verifyUtf8(paddedConst, u8"2");
    verifyUtf8(paddedPower, u8"x²");
    verifyUtf8(paddedPoly, u8"x³ − x² + x");
}

TEST_F(Laurent1Test, tightEncoding) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        TightEncodingTest<L>::verifyTightEncoding(c);
    }

    // Test a handful of cases manually, to ensure the format does not change
    // between releases.  (The following tight encodings were all computed
    // using Regina 7.4.1.)
    EXPECT_EQ(zero.tightEncoding(), "M");
    EXPECT_EQ(x2.tightEncoding(), "NOM");
    EXPECT_EQ(x5Inv.tightEncoding(), "NHM");
    EXPECT_EQ(f.tightEncoding(), "LPNTM");
    EXPECT_EQ(low.tightEncoding(), "NFKGPHIIM");
    EXPECT_EQ(long2.tightEncoding(), "LPNZM");
}

TEST_F(Laurent1Test, swap) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifySwap(c, d);
        }
    }
}

