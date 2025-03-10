
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "maths/integer.h"
#include "maths/polynomial.h"
#include "maths/rational.h"
#include "utilities/stringutils.h"

#include "testhelper.h"

using regina::Integer;
using regina::Polynomial;
using regina::Rational;

class PolynomialTest : public testing::Test {
    protected:
        Polynomial<Integer> zero {};
        Polynomial<Integer> one { 1 };
        Polynomial<Integer> two { 2 };
        Polynomial<Integer> a { 1, -1, 1 };
        Polynomial<Integer> b { 0, 1, -1, 1 };
        Polynomial<Integer> c { 0, 0, 1, -1, 1 };
        Polynomial<Integer> d { 1, -1, 1, -1, 1 };
        Polynomial<Integer> e { 0, 2, 4, -2, 2 };

        template <typename T>
        static void verifyEqual(const Polynomial<T>& result,
                const Polynomial<T>& expect) {
            SCOPED_TRACE_REGINA(result);
            SCOPED_TRACE_REGINA(expect);

            EXPECT_EQ(result, expect);
            EXPECT_FALSE(result != expect);
            EXPECT_EQ(result.str(), expect.str());
            EXPECT_EQ(result.isZero(), expect.isZero());
            EXPECT_EQ(result.degree(), expect.degree());
        }

        template <typename T>
        static void verifyEqual(const Polynomial<T>& result,
                std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(result);

            Polynomial<T> expect(coeffs);
            SCOPED_TRACE_REGINA(expect);

            verifyEqual(result, expect);
            if (coeffs.size() == 0) {
                EXPECT_TRUE(result.isZero());
                EXPECT_EQ(result.degree(), 0);
            } else {
                EXPECT_FALSE(result.isZero());
                EXPECT_EQ(result.degree() + 1, coeffs.size());
            }
        }

        template <typename T>
        void verifyPlus(const Polynomial<T>& x, const Polynomial<T>& y,
                std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x + y, coeffs);
            verifyEqual((x + zero) + y, coeffs);
            verifyEqual(x + (y + zero), coeffs);
            verifyEqual((x + zero) + (y + zero), coeffs);

            verifyEqual(y + x, coeffs);
            verifyEqual((y + zero) + x, coeffs);
            verifyEqual(y + (x + zero), coeffs);
            verifyEqual((y + zero) + (x + zero), coeffs);

            {
                Polynomial<T> z(x);
                verifyEqual(z += y, coeffs);
            }
            {
                Polynomial<T> z(x);
                verifyEqual(z += (y + zero), coeffs);
            }
            {
                Polynomial<T> z(y);
                verifyEqual(z += x, coeffs);
            }
            {
                Polynomial<T> z(y);
                verifyEqual(z += (x + zero), coeffs);
            }
        }

        template <typename T>
        void verifyMinus(const Polynomial<T>& x, const Polynomial<T>& y,
                std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x - y, coeffs);
            verifyEqual((x + zero) - y, coeffs);
            verifyEqual(x - (y + zero), coeffs);
            verifyEqual((x + zero) - (y + zero), coeffs);

            {
                Polynomial<T> z(x);
                verifyEqual(z -= y, coeffs);
            }
            {
                Polynomial<T> z(x);
                verifyEqual(z -= (y + zero), coeffs);
            }

            verifyPlus(x, -y, coeffs);
            {
                Polynomial<T> z(y);
                z.negate();
                verifyPlus(x, z, coeffs);
            }
        }

        template <typename T>
        void verifyMult(const Polynomial<T>& x, const T& y,
                std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x * y, coeffs);
            verifyEqual((x + zero) * y, coeffs);
            verifyEqual(y * x, coeffs);
            verifyEqual(y * (x + zero), coeffs);
            {
                Polynomial<T> z(x);
                verifyEqual(z *= y, coeffs);
            }
        }

        template <typename T>
        void verifyDiv(const Polynomial<T>& x, const T& y,
                std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x / y, coeffs);
            verifyEqual((x + zero) / y, coeffs);
            {
                Polynomial<T> z(x);
                verifyEqual(z /= y, coeffs);
            }
        }

        template <typename T>
        void verifyMult(const Polynomial<T>& x, const Polynomial<T>& y,
                std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x * y, coeffs);
            verifyEqual((x + zero) * y, coeffs);
            verifyEqual(x * (y + zero), coeffs);
            verifyEqual((x + zero) * (y + zero), coeffs);

            verifyEqual(y * x, coeffs);
            verifyEqual((y + zero) * x, coeffs);
            verifyEqual(y * (x + zero), coeffs);
            verifyEqual((y + zero) * (x + zero), coeffs);

            {
                Polynomial<T> z(x);
                verifyEqual(z *= y, coeffs);
            }
            {
                Polynomial<T> z(x);
                verifyEqual(z *= (y + zero), coeffs);
            }
            {
                Polynomial<T> z(y);
                verifyEqual(z *= x, coeffs);
            }
            {
                Polynomial<T> z(y);
                verifyEqual(z *= (x + zero), coeffs);
            }
        }

        template <typename T>
        void verifyDiv(const Polynomial<T>& x, const Polynomial<T>& y,
                std::initializer_list<T> coeffs) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(y);

            verifyEqual(x / y, coeffs);
            verifyEqual((x + zero) / y, coeffs);
            verifyEqual(x / (y + zero), coeffs);
            verifyEqual((x + zero) / (y + zero), coeffs);

            {
                Polynomial<T> z(x);
                verifyEqual(z /= y, coeffs);
            }
            {
                Polynomial<T> z(x);
                verifyEqual(z /= (y + zero), coeffs);
            }
        }

        static void verifyDivisionAlg(const Polynomial<Rational>& x,
                const Polynomial<Rational>& divisor) {
            SCOPED_TRACE_REGINA(x);
            SCOPED_TRACE_REGINA(divisor);

            auto [q, r] = x.divisionAlg(divisor);

            /* deprecated verison of divisionAlg():
            {
                Polynomial<Rational> q2, r2;
                x.divisionAlg(divisor, q2, r2);

                verifyEqual(q, q2);
                verifyEqual(r, r2);
            }
            */

            verifyEqual(x, q * divisor + r);
            if (! r.isZero())
                EXPECT_LT(r.degree(), divisor.degree());
}

};

TEST_F(PolynomialTest, set) {
    Polynomial<Integer> x { 1, 2, 1 };

    verifyEqual<Integer>(x, {1, 2, 1});
    x.set(1, 3);
    verifyEqual<Integer>(x, {1, 3, 1});
    x.set(2, 0);
    verifyEqual<Integer>(x, {1, 3});
    x.set(1, 0);
    verifyEqual<Integer>(x, {1});
    x.set(1, 0);
    verifyEqual<Integer>(x, {1});
    x.set(0, 0);
    verifyEqual<Integer>(x, {});
    x.set(0, 0);
    verifyEqual<Integer>(x, {});
    x.set(2, 1);
    verifyEqual<Integer>(x, {0, 0, 1});
}

TEST_F(PolynomialTest, arithmetic) {
    verifyEqual<Integer>(zero, {});

    verifyEqual<Integer>(-zero, {});
    verifyEqual<Integer>(-a, {-1, 1, -1});
    verifyEqual<Integer>(-c, {0, 0, -1, 1, -1});

    verifyPlus<Integer>(zero, zero, {});
    verifyPlus<Integer>(a, zero, {1, -1, 1});
    verifyPlus<Integer>(a, b, {1, 0, 0, 1});
    verifyPlus<Integer>(b, d, {1, 0, 0, 0, 1});
    verifyPlus<Integer>(c, d, {1, -1, 2, -2, 2});

    verifyMinus<Integer>(zero, zero, {});
    verifyMinus<Integer>(a, zero, {1, -1, 1});
    verifyMinus<Integer>(zero, a, {-1, 1, -1});
    verifyMinus<Integer>(a, b, {1, -2, 2, -1});
    verifyMinus<Integer>(b, d, {-1, 2, -2, 2, -1});
    verifyMinus<Integer>(c, d, {-1, 1});
    verifyMinus<Integer>(d, c, {1, -1});
    verifyMinus<Integer>(a, a, {});

    verifyMult<Integer>(zero, 0, {});
    verifyMult<Integer>(zero, 1, {});
    verifyMult<Integer>(zero, 2, {});
    verifyMult<Integer>(a, 0, {});
    verifyMult<Integer>(a, 1, {1, -1, 1});
    verifyMult<Integer>(a, -1, {-1, 1, -1});
    verifyMult<Integer>(a, 2, {2, -2, 2});

    verifyDiv<Integer>(zero, 1, {});
    verifyDiv<Integer>(zero, 2, {});
    verifyDiv<Integer>(a, 1, {1, -1, 1});
    verifyDiv<Integer>(a, -1, {-1, 1, -1});
    verifyDiv<Integer>(e, 2, {0, 1, 2, -1, 1});

    verifyMult<Integer>(zero, zero, {});
    verifyMult<Integer>(zero, a, {});
    verifyMult<Integer>(a, b, {0, 1, -2, 3, -2, 1});

    verifyDiv<Integer>(zero, one, {});
    verifyDiv<Integer>(zero, a, {});
    verifyDiv<Integer>(a, one, {1, -1, 1});
    verifyDiv<Integer>(a, -one, {-1, 1, -1});
    verifyDiv<Integer>(e, two, {0, 1, 2, -1, 1});
    verifyDiv<Integer>(a, a, {1});
    verifyDiv<Integer>(b, a, {0, 1});
    verifyDiv<Integer>(c, a, {0, 0, 1});
    verifyDiv<Integer>(c, b, {0, 1});
    verifyDiv<Integer>(d, a, {0, 0, 1}); // leaves remainder
    verifyDiv<Integer>(e, a, {2, 0, 2}); // leaves remainder
    verifyDiv<Integer>(e * d, d, {0, 2, 4, -2, 2});

    {
        Polynomial<Integer> x(a);
        verifyEqual<Integer>(x + x, {2, -2, 2});
        verifyEqual<Integer>(x += x, {2, -2, 2});
    }
    {
        Polynomial<Integer> x(a);
        verifyEqual<Integer>(x - x, {});
        verifyEqual<Integer>(x -= x, {});
    }
    {
        Polynomial<Integer> x = {1, 0, 1};
        verifyEqual<Integer>(x * x, {1, 0, 2, 0, 1});
        verifyEqual<Integer>(x *= x, {1, 0, 2, 0, 1});
    }
    {
        Polynomial<Integer> x(a);
        verifyEqual<Integer>(x / x, {1});
        verifyEqual<Integer>(x /= x, {1});
    }
}

TEST_F(PolynomialTest, divisionAlg) {
    verifyDivisionAlg({}, {2});
    verifyDivisionAlg({5}, {2});
    verifyDivisionAlg({1, 3}, {2});
    verifyDivisionAlg({-1, 1, -1}, {2});

    verifyDivisionAlg({}, {-3, 2});
    verifyDivisionAlg({5}, {-3, 2});
    verifyDivisionAlg({1, 3}, {-3, 2});
    verifyDivisionAlg({-1, 1, -1}, {-3, 2});

    verifyDivisionAlg({}, {4, 3, 6});
    verifyDivisionAlg({5}, {4, 3, 6});
    verifyDivisionAlg({1, 3}, {4, 3, 6});
    verifyDivisionAlg({-1, 1, -1}, {4, 3, 6});

    verifyDivisionAlg({1, 0, 0, 0, 1}, {0, 1});
    verifyDivisionAlg({0, 0, 0, 0, 1}, {0, 1});

    // Test the case where x and divisor are the same C++ object.
    Polynomial p {1, 3, 2};
    verifyDivisionAlg(p, p);
}
