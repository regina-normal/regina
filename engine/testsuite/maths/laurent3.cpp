
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

using L = Laurent<Integer, 3>;

// An integer that cannot fit into 128 bits.
// We split the digits into chunks to not break syntax highlighting.
#define BIG "5421309874" "5789403215" "6654013103" "5798756432" "1035741817"

class Laurent3Test : public LaurentTest {
    protected:
        const L zero {};
        const L one {{{0,0,0},1}};
        const L two {{{0,0,0},2}};
        const L minusOne {{{0,0,0},-1}};
        const L x {{{1,0,0},1}};
        const L y {{{0,1,0},1}};
        const L z {{{0,0,1},1}};
        const L x2 {{{2,0,0},1}};
        const L y2 {{{0,2,0},1}};
        const L z2 {{{0,0,2},1}};
        const L xInv {{{-1,0,0},1}};
        const L yInv {{{0,-1,0},1}};
        const L zInv {{{0,0,-1},1}};
        const L xyz {{{1,1,1},1}};
        const L xy2z3 {{{1,2,3},1}};
        const L xyzInvX {{{-1,1,1},1}};
        const L xyzInvY {{{1,-1,1},1}};
        const L xyzInvZ {{{1,1,-1},1}};
        const L xTerm {{{2,0,0},3}};
        const L yTerm {{{0,-2,0},-3}};
        const L zTerm {{{0,0,2},-3}};
        const L xyzTerm {{{-2,3,4},-1}};
        const L a {{{3,-1,0},-5}, {{3,2,-2},3}, {{3,2,0},2}, {{3,2,1},-1},
            {{5,-1,0},7}, {{5,2,0},-3}, {{5,3,0},2}};
        #if 0
        const L a {{{0,0},1}, {{1,2},-2}, {{-2,-1},3}};
        const L b {{{1,0},2}, {{1,2},-4}, {{0,-1},-2}};
        const L c {{{1,0},-2}, {{1,1},5}, {{0,1},7}};
        const L d {{{1,1},-1}, {{1,2},1}, {{0,-1},-2}, {{1,0},-2}};
        const L e {{{-1,0},-5}, {{2,-2},3}, {{2,0},2}, {{2,1},-1}};
        const L f {{{0,0},1}, {{0,1},2}, {{0,2},3}, {{-1,0},4}, {{-2,0},5}};
        const L big {{{1,2},bigInt}, {{1,-2},-bigInt}, {{1,1},-1}};
        #endif

        // Some polynomials with more space allocated than they need
        // (with padding occurring along both coordinate axes):
        const L paddedZero = padded(zero);
        const L paddedConst = padded(two);
        const L paddedPower = padded(xy2z3);
        const L paddedPoly = padded(a);

        // Some polynomials whose exponent sequences span a cube in the
        // 3-D space of all exponent sequences:
        const L corners3 {corners1, Laurent<Integer, 2>(corners1, corners1)};
        const L lattice3 {lattice1, Laurent<Integer, 2>(lattice1, lattice1)};

        const std::array<std::reference_wrapper<const L>, 29> cases {
            std::cref(zero), std::cref(one), std::cref(two),
            std::cref(minusOne),
            std::cref(x), std::cref(y), std::cref(z),
            std::cref(x2), std::cref(y2), std::cref(z2),
            std::cref(xInv), std::cref(yInv), std::cref(zInv),
            std::cref(xyz), std::cref(xy2z3),
            std::cref(xyzInvX), std::cref(xyzInvY), std::cref(xyzInvZ),
            std::cref(xTerm), std::cref(yTerm), std::cref(zTerm),
            std::cref(xyzTerm),
            std::cref(a),
            #if 0
            std::cref(a), std::cref(b), std::cref(c), std::cref(d),
            std::cref(e), std::cref(f), std::cref(big),
            #endif
            std::cref(paddedZero), std::cref(paddedConst),
            std::cref(paddedPower), std::cref(paddedPoly),
            std::cref(corners3), std::cref(lattice3)
        };

        // Some single-variable polynomials and two-variable polynomials to use
        // with routines such as embed() and diagonalise() and the product
        // constructor.
        const std::array<const Laurent<Integer>, 3> sub1 {
            Laurent<Integer>{},
            Laurent<Integer>{-5},
            Laurent<Integer>{-2, {3,0,2,-1}}
        };
        const std::array<const Laurent<Integer, 2>, 3> sub2 {
            Laurent<Integer, 2>{},
            Laurent<Integer, 2>{6},
            Laurent<Integer, 2>{{{0,0},1}, {{1,2},-2}, {{-2,-1},3}}
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
            else if (std::addressof(poly) == std::addressof(xy2z3) ||
                    std::addressof(poly) == std::addressof(paddedPower))
                return Batch::Power;
            else if (std::addressof(poly) == std::addressof(a) ||
                    std::addressof(poly) == std::addressof(paddedPoly))
                return Batch::Poly;
            else
                return Batch::Unique;
        }
};

TEST_F(Laurent3Test, construct) {
    // Start by verifying our hand-coded test cases.
    // This also effectively tests isZero() via the calls to validate() and
    // validateZero().
    #if 0
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
    #endif

    // For the product-generated cases, just check that they have the expected
    // number of non-zero terms.
    {
        regina::LaurentCoefficient<Integer, 3> end;
        validate(corners3);
        EXPECT_EQ(std::distance(corners3.first(), end), 8);
        validate(lattice3);
        EXPECT_EQ(std::distance(lattice3.first(), end), 64);
    }

    // On to our generic tests:
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyConstruct(c);
    }

    verifyConstructBasic<3>();

    // Test the product constructor (which is specific to multivariate Laurent
    // polynomials).
    for (const auto& s : sub2) {
        validateZero(L(sub1[0], s));
        validateZero(L(s, sub1[0]));
    }
    for (const auto& s : sub1) {
        validateZero(L(sub2[0], s));
        validateZero(L(s, sub2[0]));
    }

    validate(L(sub1[1], sub2[1]), {{{0,0,0},-30}});
    validate(L(sub2[1], sub1[1]), {{{0,0,0},-30}});
    validate(L(sub1[1], sub2[2]),
        {{{0,-2,-1},-15}, {{0,0,0},-5}, {{0,1,2},10}});
    validate(L(sub2[2], sub1[1]),
        {{{-2,-1,0},-15}, {{0,0,0},-5}, {{1,2,0},10}});
    validate(L(sub1[2], sub2[1]),
        {{{-2,0,0},18}, {{0,0,0},12}, {{1,0,0},-6}});
    validate(L(sub2[1], sub1[2]),
        {{{0,0,-2},18}, {{0,0,0},12}, {{0,0,1},-6}});
    validate(L(sub1[2], sub2[2]), {
        {{-2,-2,-1},9}, {{-2,0,0},3}, {{-2,1,2},-6},
        {{0,-2,-1},6}, {{0,0,0},2}, {{0,1,2},-4},
        {{1,-2,-1},-3}, {{1,0,0},-1}, {{1,1,2},2}
    });
    validate(L(sub2[2], sub1[2]), {
        {{-2,-1,-2},9}, {{-2,-1,0},6}, {{-2,-1,1},-3},
        {{0,0,-2},3}, {{0,0,0},2}, {{0,0,1},-1},
        {{1,2,-2},-6}, {{1,2,0},-4}, {{1,2,1},2}
    });
}

TEST_F(Laurent3Test, embed) {
    // This test is specific to multivariate Laurent polynomials.
    validateZero(L::embed<0>(sub1[0]));
    validateZero(L::embed<1>(sub1[0]));
    validateZero(L::embed<2>(sub1[0]));
    validate(L::embed<0>(sub1[1]), {{{0,0,0},-5}});
    validate(L::embed<1>(sub1[1]), {{{0,0,0},-5}});
    validate(L::embed<2>(sub1[1]), {{{0,0,0},-5}});
    validate(L::embed<0>(sub1[2]), {{{-2,0,0},3}, {{0,0,0},2}, {{1,0,0},-1}});
    validate(L::embed<1>(sub1[2]), {{{0,-2,0},3}, {{0,0,0},2}, {{0,1,0},-1}});
    validate(L::embed<2>(sub1[2]), {{{0,0,-2},3}, {{0,0,0},2}, {{0,0,1},-1}});
}

TEST_F(Laurent3Test, diagonalise) {
    // This test is specific to multivariate Laurent polynomials.
    validateZero(L::diagonalise(sub1[0]));
    validate(L::diagonalise(sub1[1]), {{{0,0,0},-5}});
    validate(L::diagonalise(sub1[2]),
        {{{-2,-2,-2},3}, {{0,0,0},2}, {{1,1,1},-1}});
}

TEST_F(Laurent3Test, assign) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyAssign(c, d);
        }
        verifyAssign(c);
    }
}

TEST_F(Laurent3Test, padding) {
    verifyPadding(paddedZero, zero);
    verifyPadding(paddedConst, two);
    verifyPadding(paddedPower, xy2z3);
    verifyPadding(paddedPoly, a);

    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyPadding(padded(c), c);
    }
}

TEST_F(Laurent3Test, reserveRange) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyReserveRange(c);
    }
}

TEST_F(Laurent3Test, init) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyInit(c, -2, 3, 4);
    }
}

TEST_F(Laurent3Test, moveThenAssign) {
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

TEST_F(Laurent3Test, set) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifySet(c);
    }
    verifySetFromZero<3>();
}

TEST_F(Laurent3Test, iteration) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyIteration(c);
    }
}

TEST_F(Laurent3Test, comparison) {
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
    verifyComparisonBasic<3>();
}

TEST_F(Laurent3Test, shift) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyShift(c);
    }
}

TEST_F(Laurent3Test, negate) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyNegate(c);
    }
}

TEST_F(Laurent3Test, invert) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);

        L invX(c), invY(c), invZ(c);
        invX.invert<0>();
        invY.invert<1>();
        invZ.invert<2>();

        if (c.isZero()) {
            validateZero(invX);
            validateZero(invY);
            validateZero(invZ);
        } else {
            validate(invX);
            validate(invY);
            validate(invZ);
            EXPECT_FALSE(invX.isZero());
            EXPECT_FALSE(invY.isZero());
            EXPECT_FALSE(invZ.isZero());

            using Term = std::tuple<long, long, long, Integer>;
            {
                std::vector<Term> expect;
                for (auto it = c.first(); it; ++it)
                    expect.emplace_back(-it.exp<0>(), it.exp<1>(), it.exp<2>(),
                        *it);
                std::sort(expect.begin(), expect.end());

                std::vector<Term> found;
                for (auto it = invX.first(); it; ++it)
                    found.emplace_back(it.exp<0>(), it.exp<1>(), it.exp<2>(),
                        *it);

                EXPECT_EQ(found, expect);
            }
            {
                std::vector<Term> expect;
                for (auto it = c.first(); it; ++it)
                    expect.emplace_back(it.exp<0>(), -it.exp<1>(), it.exp<2>(),
                        *it);
                std::sort(expect.begin(), expect.end());

                std::vector<Term> found;
                for (auto it = invY.first(); it; ++it)
                    found.emplace_back(it.exp<0>(), it.exp<1>(), it.exp<2>(),
                        *it);

                EXPECT_EQ(found, expect);
            }
            {
                std::vector<Term> expect;
                for (auto it = c.first(); it; ++it)
                    expect.emplace_back(it.exp<0>(), it.exp<1>(), -it.exp<2>(),
                        *it);
                std::sort(expect.begin(), expect.end());

                std::vector<Term> found;
                for (auto it = invZ.first(); it; ++it)
                    found.emplace_back(it.exp<0>(), it.exp<1>(), it.exp<2>(),
                        *it);

                EXPECT_EQ(found, expect);
            }

            // Invert all remaining variables for each polynomial.
            invX.invert<1>();
            invX.invert<2>();
            invY.invert<0>();
            invY.invert<2>();
            invZ.invert<0>();
            invZ.invert<1>();

            validate(invX);
            validate(invY);
            validate(invZ);
            EXPECT_EQ(invX, invY);
            EXPECT_EQ(invX, invZ);
            EXPECT_EQ(invY, invZ);
            {
                // Verify the terms of just one of these (we use invX here).
                std::vector<Term> expect;
                for (auto it = c.first(); it; ++it)
                    expect.emplace_back(-it.exp<0>(), -it.exp<1>(),
                        -it.exp<2>(), *it);
                std::reverse(expect.begin(), expect.end());

                std::vector<Term> found;
                for (auto it = invX.first(); it; ++it)
                    found.emplace_back(it.exp<0>(), it.exp<1>(), it.exp<2>(),
                        *it);

                EXPECT_EQ(found, expect);
            }
        }
    }
}

TEST_F(Laurent3Test, transform) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyTransform(c);
    }
}

TEST_F(Laurent3Test, scalarMultiplyDivide) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        verifyScalarMultiplyDivide(c);
    }
}

TEST_F(Laurent3Test, addSubtract) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyAddSubtract(c, d);
        }
        verifyAddSubtract(c);
    }
}

TEST_F(Laurent3Test, multiply) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyMultiply(c, d);
        }
        verifyMultiply(c);
    }
}

TEST_F(Laurent3Test, scalarAddProduct) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifyScalarAddProduct(c, d);
        }
        verifyScalarAddProduct(c);
    }
}

TEST_F(Laurent3Test, polynomialAddProduct) {
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

TEST_F(Laurent3Test, ringConstants) {
    verifyRingConstants<L>();
}

#if 0
TEST_F(Laurent3Test, str) {
    verifyStr(zero, "0");
    verifyStr(one, "1");
    verifyStr(two, "2");
    verifyStr(minusOne, "-1");
    verifyStr(x, "x");
    verifyStr(y, "y");
    verifyStr(x2, "x^2");
    verifyStr(y2, "y^2");
    verifyStr(xInv, "x^-1");
    verifyStr(yInv, "y^-1");
    verifyStr(xy, "x y");
    verifyStr(xy2, "x y^2");
    verifyStr(xyInvX, "x^-1 y");
    verifyStr(xyInvY, "x y^-1");
    verifyStr(xTerm, "3 x^2");
    verifyStr(yTerm, "-3 y^-2");
    verifyStr(xyTerm, "-x^-2 y^3");
    verifyStr(a, "-2 x y^2 + 1 + 3 x^-2 y^-1");
    verifyStr(b, "-4 x y^2 + 2 x - 2 y^-1");
    verifyStr(c, "5 x y - 2 x + 7 y");
    verifyStr(d, "x y^2 - x y - 2 x - 2 y^-1");
    verifyStr(e, "-x^2 y + 2 x^2 + 3 x^2 y^-2 - 5 x^-1");
    verifyStr(f, "3 y^2 + 2 y + 1 + 4 x^-1 + 5 x^-2");
    verifyStr(big, BIG " x y^2 - x y - " BIG " x y^-2");
    verifyStr(paddedZero, "0");
    verifyStr(paddedConst, "2");
    verifyStr(paddedPower, "x y^2");
    verifyStr(paddedPoly, "-2 x y^2 + 1 + 3 x^-2 y^-1");

    // Test some more monomials and single-term polynomials:
    verifyStr(L::monomial(2, -2), "x^2 y^-2");
    verifyStr(L::monomial(-2, 2), "x^-2 y^2");
    verifyStr(xyTerm * -5, "5 x^-2 y^3");
    verifyStr(xyTerm * 6, "-6 x^-2 y^3");
    // Test some more multi-term polynomials:
    verifyStr(L{{{0,0},3}, {{2,1},3}, {{2,-1},-2}, {{1,-1},5}},
        "3 x^2 y - 2 x^2 y^-1 + 5 x y^-1 + 3");
    verifyStr(L{{{0,0},3}, {{-2,1},3}, {{-2,-1},-2}, {{-1,-1},5}},
        "3 + 5 x^-1 y^-1 + 3 x^-2 y - 2 x^-2 y^-1");
    verifyStr(L{{{0,0},-3}, {{-2,1},3}, {{-2,-1},-2}, {{-1,-1},5}},
        "-3 + 5 x^-1 y^-1 + 3 x^-2 y - 2 x^-2 y^-1");
}

TEST_F(Laurent3Test, utf8) {
    // Here we directly use unicode superscripts and minus signs in the C++
    // source code.
    verifyUtf8(zero, u8"0");
    verifyUtf8(one, u8"1");
    verifyUtf8(two, u8"2");
    verifyUtf8(minusOne, u8"−1");
    verifyUtf8(x, u8"x");
    verifyUtf8(y, u8"y");
    verifyUtf8(x2, u8"x²");
    verifyUtf8(y2, u8"y²");
    verifyUtf8(xInv, u8"x⁻¹");
    verifyUtf8(yInv, u8"y⁻¹");
    verifyUtf8(xy, u8"x y");
    verifyUtf8(xy2, u8"x y²");
    verifyUtf8(xyInvX, u8"x⁻¹ y");
    verifyUtf8(xyInvY, u8"x y⁻¹");
    verifyUtf8(xTerm, u8"3 x²");
    verifyUtf8(yTerm, u8"−3 y⁻²");
    verifyUtf8(xyTerm, u8"−x⁻² y³");
    verifyUtf8(a, u8"−2 x y² + 1 + 3 x⁻² y⁻¹");
    verifyUtf8(b, u8"−4 x y² + 2 x − 2 y⁻¹");
    verifyUtf8(c, u8"5 x y − 2 x + 7 y");
    verifyUtf8(d, u8"x y² − x y − 2 x − 2 y⁻¹");
    verifyUtf8(e, u8"−x² y + 2 x² + 3 x² y⁻² − 5 x⁻¹");
    verifyUtf8(f, u8"3 y² + 2 y + 1 + 4 x⁻¹ + 5 x⁻²");
    verifyUtf8(big, u8"" BIG " x y² − x y − " BIG " x y⁻²");
    verifyUtf8(paddedZero, u8"0");
    verifyUtf8(paddedConst, u8"2");
    verifyUtf8(paddedPower, u8"x y²");
    verifyUtf8(paddedPoly, u8"−2 x y² + 1 + 3 x⁻² y⁻¹");

    // Test some more monomials and single-term polynomials:
    verifyUtf8(L::monomial(2, -2), u8"x² y⁻²");
    verifyUtf8(L::monomial(-2, 2), u8"x⁻² y²");
    verifyUtf8(xyTerm * -5, u8"5 x⁻² y³");
    verifyUtf8(xyTerm * 6, u8"−6 x⁻² y³");
    // Test some more multi-term polynomials:
    verifyUtf8(L{{{0,0},3}, {{2,1},3}, {{2,-1},-2}, {{1,-1},5}},
        u8"3 x² y − 2 x² y⁻¹ + 5 x y⁻¹ + 3");
    verifyUtf8(L{{{0,0},3}, {{-2,1},3}, {{-2,-1},-2}, {{-1,-1},5}},
        u8"3 + 5 x⁻¹ y⁻¹ + 3 x⁻² y − 2 x⁻² y⁻¹");
    verifyUtf8(L{{{0,0},-3}, {{-2,1},3}, {{-2,-1},-2}, {{-1,-1},5}},
        u8"−3 + 5 x⁻¹ y⁻¹ + 3 x⁻² y − 2 x⁻² y⁻¹");
}
#endif

TEST_F(Laurent3Test, tightEncoding) {
    for (const L& c : cases) {
        TightEncodingTest<L>::verifyTightEncoding(c);
    }

    // Test a handful of cases manually, to ensure the format does not change
    // between releases.  (The following tight encodings were all computed
    // using Regina 8.0, which was the first to support 3-variable Laurent
    // polynomials.)
    EXPECT_EQ(L().tightEncoding(), "M");
    {
        L p{{{2,0,0},3}};
        EXPECT_EQ(p.tightEncoding(), "POMMM");
    }
    {
        L p{{{-1,2,3},5}};
        EXPECT_EQ(p.tightEncoding(), "RLOPM");
    }
    {
        L p{{{0,0,0},1}, {{1,2,3},-2}, {{-3,-2,-1},4}};
        EXPECT_EQ(p.tightEncoding(), "QJKLNMMMKNOPM");
    }
    {
        L p{{{1,1,2},-1}, {{1,3,-4},1}, {{0,-1,2},-2}, {{1,-1,-4},-2}};
        EXPECT_EQ(p.tightEncoding(), "KMLOKNLILNNONNPIM");
    }
}

TEST_F(Laurent3Test, swap) {
    for (const L& c : cases) {
        SCOPED_TRACE_REGINA(c);
        for (const L& d : cases) {
            SCOPED_TRACE_REGINA(d);
            verifySwap(c, d);
        }
    }
}

