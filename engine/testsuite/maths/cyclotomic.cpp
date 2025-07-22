
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "maths/cyclotomic.h"
#include "utilities/stringutils.h"

#include "testhelper.h"

using regina::Cyclotomic;
using regina::Rational;

class CyclotomicTest : public testing::Test {
    protected:
        // We do all our tests in the field of order 5.
        static constexpr size_t field = 5;

        // All polynomials are reduced mod x^4 + x^3 + x^2 + x + 1.
        // In particular, x^5 == 1.
        Cyclotomic zero { 5 };
        Cyclotomic one { 5, 1 };
        Cyclotomic two { 5, 2 };
        Cyclotomic x1 { 5, { 0, 1 } };
        Cyclotomic x2 { 5, { 0, 0, 1 } };
        Cyclotomic x3 { 5, { 0, 0, 0, 1 } };
        Cyclotomic xPlus1 { 5, { 1, 1 } };
        Cyclotomic xMinus1 { 5, { -1, 1 } };

        static void verifyEqual(const Cyclotomic& result,
                std::initializer_list<Rational> coeffs) {
            SCOPED_TRACE_REGINA(result);

            Cyclotomic expect(field, coeffs);
            SCOPED_TRACE_REGINA(expect);

            EXPECT_EQ(result, expect);
            EXPECT_FALSE(result != expect);
            EXPECT_EQ(result.str(), expect.str());
        }

        void verifyPlus(const Cyclotomic& a, const Cyclotomic& b,
                std::initializer_list<Rational> coeffs) {
            SCOPED_TRACE_REGINA(a);
            SCOPED_TRACE_REGINA(b);

            verifyEqual(a + b, coeffs);
            verifyEqual((a + zero) + b, coeffs);
            verifyEqual(a + (b + zero), coeffs);
            verifyEqual((a + zero) + (b + zero), coeffs);

            verifyEqual(b + a, coeffs);
            verifyEqual((b + zero) + a, coeffs);
            verifyEqual(b + (a + zero), coeffs);
            verifyEqual((b + zero) + (a + zero), coeffs);

            {
                Cyclotomic x(a);
                verifyEqual(x += b, coeffs);
            }
            {
                Cyclotomic x(a);
                verifyEqual(x += (b + zero), coeffs);
            }
            {
                Cyclotomic x(b);
                verifyEqual(x += a, coeffs);
            }
            {
                Cyclotomic x(b);
                verifyEqual(x += (a + zero), coeffs);
            }
        }

        void verifyMinus(const Cyclotomic& a, const Cyclotomic& b,
                std::initializer_list<Rational> coeffs) {
            SCOPED_TRACE_REGINA(a);
            SCOPED_TRACE_REGINA(b);

            verifyEqual(a - b, coeffs);
            verifyEqual((a + zero) - b, coeffs);
            verifyEqual(a - (b + zero), coeffs);
            verifyEqual((a + zero) - (b + zero), coeffs);

            {
                Cyclotomic x(a);
                verifyEqual(x -= b, coeffs);
            }
            {
                Cyclotomic x(a);
                verifyEqual(x -= (b + zero), coeffs);
            }

            verifyPlus(a, -b, coeffs);
            {
                Cyclotomic x(b);
                x.negate();
                verifyPlus(a, x, coeffs);
            }
        }

        void verifyMult(const Cyclotomic& a, const Rational& b,
                std::initializer_list<Rational> coeffs) {
            SCOPED_TRACE_REGINA(a);
            SCOPED_TRACE_REGINA(b);

            verifyEqual(a * b, coeffs);
            verifyEqual((a + zero) * b, coeffs);
            verifyEqual(b * a, coeffs);
            verifyEqual(b * (a + zero), coeffs);
            {
                Cyclotomic x(a);
                verifyEqual(x *= b, coeffs);
            }
        }

        void verifyDiv(const Cyclotomic& a, const Rational& b,
                std::initializer_list<Rational> coeffs) {
            SCOPED_TRACE_REGINA(a);
            SCOPED_TRACE_REGINA(b);

            verifyEqual(a / b, coeffs);
            verifyEqual((a + zero) / b, coeffs);
            {
                Cyclotomic x(a);
                verifyEqual(x /= b, coeffs);
            }
            verifyMult(a, b.inverse(), coeffs);
            {
                Rational x(b);
                x.invert();
                verifyMult(a, x, coeffs);
            }
        }

        void verifyMult(const Cyclotomic& a, const Cyclotomic& b,
                std::initializer_list<Rational> coeffs) {
            SCOPED_TRACE_REGINA(a);
            SCOPED_TRACE_REGINA(b);

            verifyEqual(a * b, coeffs);
            verifyEqual((a + zero) * b, coeffs);
            verifyEqual(a * (b + zero), coeffs);
            verifyEqual((a + zero) * (b + zero), coeffs);

            verifyEqual(b * a, coeffs);
            verifyEqual((b + zero) * a, coeffs);
            verifyEqual(b * (a + zero), coeffs);
            verifyEqual((b + zero) * (a + zero), coeffs);

            {
                Cyclotomic x(a);
                verifyEqual(x *= b, coeffs);
            }
            {
                Cyclotomic x(a);
                verifyEqual(x *= (b + zero), coeffs);
            }
            {
                Cyclotomic x(b);
                verifyEqual(x *= a, coeffs);
            }
            {
                Cyclotomic x(b);
                verifyEqual(x *= (a + zero), coeffs);
            }
        }

        void verifyDiv(const Cyclotomic& a, const Cyclotomic& b,
                std::initializer_list<Rational> coeffs) {
            SCOPED_TRACE_REGINA(a);
            SCOPED_TRACE_REGINA(b);

            verifyEqual(a / b, coeffs);
            verifyEqual((a + zero) / b, coeffs);
            verifyEqual(a / (b + zero), coeffs);
            verifyEqual((a + zero) / (b + zero), coeffs);

            {
                Cyclotomic x(a);
                verifyEqual(x /= b, coeffs);
            }
            {
                Cyclotomic x(a);
                verifyEqual(x /= (b + zero), coeffs);
            }

            verifyMult(a, b.inverse(), coeffs);
            {
                Cyclotomic x(b);
                x.invert();
                verifyMult(a, x, coeffs);
            }
        }
};

TEST_F(CyclotomicTest, arithmetic) {
    verifyEqual(zero, {});

    verifyEqual(-one, { -1 });
    verifyEqual(-x1, { 0, -1 });
    verifyEqual(-xMinus1, { 1, -1 });

    verifyPlus(zero, zero, {});
    verifyPlus(xPlus1, zero, { 1, 1 });
    verifyPlus(xPlus1, xMinus1, { 0, 2 });

    verifyMinus(zero, zero, {});
    verifyMinus(xPlus1, zero, { 1, 1 });
    verifyMinus(zero, xPlus1, { -1, -1 });
    verifyMinus(xPlus1, xMinus1, { 2 });

    verifyMult(zero, 0, {});
    verifyMult(zero, 1, {});
    verifyMult(zero, 2, {});
    verifyMult(xMinus1, 0, {});
    verifyMult(xMinus1, 1, { -1, 1 });
    verifyMult(xMinus1, -1, { 1, -1 });
    verifyMult(xMinus1, 2, { -2, 2 });
    verifyMult(xMinus1, Rational(1, 2), { -Rational(1, 2), Rational(1, 2) });

    verifyDiv(zero, 1, {});
    verifyDiv(zero, 2, {});
    verifyDiv(xMinus1, 1, { -1, 1 });
    verifyDiv(xMinus1, -1, { 1, -1 });
    verifyDiv(xMinus1, Rational(1, 2), { -2, 2 });
    verifyDiv(xMinus1, 2, { -Rational(1, 2), Rational(1, 2) });

    verifyMult(zero, zero, {});
    verifyMult(zero, xPlus1, {});
    verifyMult(xPlus1, xMinus1, { -1, 0, 1});
    verifyMult(x2, x3, { 1 });
    verifyMult(x3, x3, { 0, 1 });
    verifyMult(xPlus1, x3 + x2, { -1, -1, 0, 1 });

    verifyDiv(zero, xPlus1, {});
    verifyDiv(zero, xPlus1, {});
    verifyDiv(xMinus1, one, { -1, 1 });
    verifyDiv(xMinus1, -one, { 1, -1 });
    verifyDiv(xMinus1, two, { -Rational(1, 2), Rational(1, 2) });
    verifyDiv(xMinus1, xMinus1, { 1 });
    verifyDiv(two, x2, { 0, 0, 0, 2 });
    verifyDiv(one, x3, { 0, 0, 1 });
    verifyDiv(x3, x2, { 0, 1 });
    verifyDiv(x2, x3, { -1, -1, -1, -1 });

    {
        Cyclotomic x(xMinus1);
        verifyEqual(x + x, {-2, 2});
        verifyEqual(x += x, {-2, 2});
    }
    {
        Cyclotomic x(xMinus1);
        verifyEqual(x - x, {});
        verifyEqual(x -= x, {});
    }
    {
        Cyclotomic x(xMinus1);
        verifyEqual(x * x, {1, -2, 1});
        verifyEqual(x *= x, {1, -2, 1});
    }
    {
        Cyclotomic x(5, {1, 0, 1});
        verifyEqual(x * x, {0, -1, 1, -1});
        verifyEqual(x *= x, {0, -1, 1, -1});
    }
    {
        Cyclotomic x(xMinus1);
        verifyEqual(x / x, {1});
        verifyEqual(x /= x, {1});
    }
}
