
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
        const L zero {};
        const L one {{{0,0},1}};
        const L two {{{0,0},2}};
        const L minusOne {{{0,0},-1}};
        const L x2 {{{2,0},1}};
        const L xy {{{1,1},1}};
        const L xy2 {{{1,2},1}};
        const L xTerm {{{2,0},3}};
        const L yTerm {{{0,-2},-3}};
        const L a {{{0,0},1}, {{1,2},-2}, {{-2,-1},3}};
        const L b {{{1,0},2}, {{1,2},-4}, {{0,-1},-2}};
        const L c {{{1,0},-2}, {{1,1},5}, {{0,1},7}};
        const L d {{{1,1},-1}, {{1,2},1}, {{0,-1},-2}, {{1,0},-2}};
        const L e {{{-1,0},-5}, {{2,-2},3}, {{2,0},2}, {{2,1},-1}};
        const L f {{{0,0},1}, {{0,1},2}, {{0,2},3}, {{-1,0},4}, {{-2,0},5}};
        const L big {{{1,2},bigInt}, {{1,-2},-bigInt}, {{1,1},-1}};

        // Some polynomials with more space allocated than they need
        // (with padding occurring along both coordinate axes):
        const L paddedZero = padded(zero);
        const L paddedConst = padded(two);
        const L paddedPower = padded(xy2);
        const L paddedPoly = padded(a);

        const std::array<std::reference_wrapper<const L>, 20> cases {
            std::cref(zero), std::cref(one), std::cref(two),
            std::cref(minusOne),
            std::cref(x2), std::cref(xy), std::cref(xy2),
            std::cref(xTerm), std::cref(yTerm),
            std::cref(a), std::cref(b), std::cref(c), std::cref(d),
            std::cref(e), std::cref(f), std::cref(big),
            std::cref(paddedZero), std::cref(paddedConst),
            std::cref(paddedPower), std::cref(paddedPoly)
        };

        // Some single-variable polynomials to use with routines such as
        // embed() and diagonalise().
        const std::array<const regina::Laurent<Integer>, 3> sub {
            Laurent<Integer>(),
            Laurent<Integer>(-5),
            Laurent<Integer>(-2, {3,0,2,-1})
        };

        // Note which of our cases represent the same mathematical polynomial.
        enum class Batch { Unique, Zero, Const, Power, Poly };
        Batch batch(const L& poly) {
            if (std::addressof(poly) == std::addressof(zero) ||
                    std::addressof(poly) == std::addressof(paddedZero))
                return Batch::Zero;
            else if (std::addressof(poly) == std::addressof(two) ||
                    std::addressof(poly) == std::addressof(paddedConst))
                return Batch::Const;
            else if (std::addressof(poly) == std::addressof(xy2) ||
                    std::addressof(poly) == std::addressof(paddedPower))
                return Batch::Power;
            else if (std::addressof(poly) == std::addressof(a) ||
                    std::addressof(poly) == std::addressof(paddedPoly))
                return Batch::Poly;
            else
                return Batch::Unique;
        }
};

TEST_F(Laurent2Test, construct) {
    // Start by verifying our hand-coded test cases.
    // This also effectively tests isZero() via the calls to validate() and
    // validateZero().
    validateZero(zero);
    validate(one, {{{0,0},1}});
    validate(two, {{{0,0},2}});
    validate(minusOne, {{{0,0},-1}});
    validate(x2, {{{2,0},1}});
    validate(xy, {{{1,1},1}});
    validate(xy2, {{{1,2},1}});
    validate(xTerm, {{{2,0},3}});
    validate(yTerm, {{{0,-2},-3}});
    validate(a, {{{-2,-1},3}, {{0,0},1}, {{1,2},-2}});
    validate(b, {{{0,-1},-2}, {{1,0},2}, {{1,2},-4}});
    validate(c, {{{0,1},7}, {{1,0},-2}, {{1,1},5}});
    validate(d, {{{0,-1},-2}, {{1,0},-2}, {{1,1},-1}, {{1,2},1}});
    validate(e, {{{-1,0},-5}, {{2,-2},3}, {{2,0},2}, {{2,1},-1}});
    validate(f, {{{-2,0},5}, {{-1,0},4}, {{0,0},1}, {{0,1},2}, {{0,2},3}});
    validate(big, {{{1,-2},-bigInt}, {{1,1},-1}, {{1,2},bigInt}});
    validateZero(paddedZero);
    validate(paddedConst, {{{0,0},2}});
    validate(paddedPower, {{{1,2},1}});
    validate(paddedPoly, {{{-2,-1},3}, {{0,0},1}, {{1,2},-2}});

    // On to our generic tests:
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyConstruct(c);
    }

    verifyConstructBasic<2>();

    // Test the product constructor (which is specific to multivariate Laurent
    // polynomials).
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
    // This test is specific to multivariate Laurent polynomials.
    validateZero(L::embed<0>(sub[0]));
    validateZero(L::embed<1>(sub[0]));
    validate(L::embed<0>(sub[1]), {{{0,0},-5}});
    validate(L::embed<1>(sub[1]), {{{0,0},-5}});
    validate(L::embed<0>(sub[2]), {{{-2,0},3}, {{0,0},2}, {{1,0},-1}});
    validate(L::embed<1>(sub[2]), {{{0,-2},3}, {{0,0},2}, {{0,1},-1}});
}

TEST_F(Laurent2Test, diagonalise) {
    // This test is specific to multivariate Laurent polynomials.
    validateZero(L::diagonalise(sub[0]));
    validate(L::diagonalise(sub[1]), {{{0,0},-5}});
    validate(L::diagonalise(sub[2]), {{{-2,-2},3}, {{0,0},2}, {{1,1},-1}});
}

TEST_F(Laurent2Test, assign) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyAssign(c, d);
        }
        verifyAssign(c);
    }
}

TEST_F(Laurent2Test, padding) {
    verifyPadding(paddedZero, zero);
    verifyPadding(paddedConst, two);
    verifyPadding(paddedPower, xy2);
    verifyPadding(paddedPoly, a);

    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyPadding(padded(c), c);
    }
}

TEST_F(Laurent2Test, reserveRange) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyReserveRange(c);
    }
}

TEST_F(Laurent2Test, init) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyInit(c, -2, 3);
    }
}

TEST_F(Laurent2Test, moveThenAssign) {
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

TEST_F(Laurent2Test, set) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifySet(c);
    }
}

TEST_F(Laurent2Test, iteration) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyIteration(c);
    }
}

TEST_F(Laurent2Test, comparison) {
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
        verifyComparison(c, 2, cBatch == Batch::Const);
        verifyComparison(c, 1, std::addressof(c) == std::addressof(one));
        verifyComparison(c, -1, std::addressof(c) == std::addressof(minusOne));

        // Comparisons against integers that appear in none of our tests:
        verifyComparison(c, 3, false);
        verifyComparison(c, bigInt, false);
    }
    verifyComparisonBasic<2>();
}

TEST_F(Laurent2Test, shift) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyShift(c);
    }
}

TEST_F(Laurent2Test, negate) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyNegate(c);
    }
}

TEST_F(Laurent2Test, invert) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        L invX(c), invY(c);
        invX.invert<0>();
        invY.invert<1>();

        if (c.isZero()) {
            validateZero(invX);
            validateZero(invY);
        } else {
            validate(invX);
            validate(invY);
            EXPECT_FALSE(invX.isZero());
            EXPECT_FALSE(invY.isZero());

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

            // Invert all remaining variables for each polynomial.
            invX.invert<1>();
            invY.invert<0>();

            validate(invX);
            validate(invY);
            EXPECT_EQ(invX, invY);
            {
                // Verify the terms of just one of these (we use invX here).
                std::vector<Term> expect;
                for (auto it = c.first(); it; ++it)
                    expect.emplace_back(-it.exp<0>(), -it.exp<1>(), *it);
                std::reverse(expect.begin(), expect.end());

                std::vector<Term> found;
                for (auto it = invX.first(); it; ++it)
                    found.emplace_back(it.exp<0>(), it.exp<1>(), *it);

                EXPECT_EQ(found, expect);
            }
        }
    }
}

TEST_F(Laurent2Test, transform) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyTransform(c);
    }
}

TEST_F(Laurent2Test, scalarMultiplyDivide) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyScalarMultiplyDivide(c);
    }
}

TEST_F(Laurent2Test, addSubtract) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyAddSubtract(c, d);
        }
        verifyAddSubtract(c);
    }
}

TEST_F(Laurent2Test, multiply) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyMultiply(c, d);
        }
        verifyMultiply(c);
    }
}

TEST_F(Laurent2Test, scalarAddProduct) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyScalarAddProduct(c, d);
        }
        verifyScalarAddProduct(c);
    }
}

TEST_F(Laurent2Test, polynomialAddProduct) {
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

TEST_F(Laurent2Test, ringConstants) {
    verifyRingConstants<L>();
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
            verifySwap(c, d);
        }
    }
}

