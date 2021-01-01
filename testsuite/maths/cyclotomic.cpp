
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/cyclotomic.h"
#include "utilities/stringutils.h"
#include "testsuite/utilities/testutilities.h"

using regina::Cyclotomic;
using regina::Rational;

class CyclotomicTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(CyclotomicTest);

    CPPUNIT_TEST(arithmetic);

    CPPUNIT_TEST_SUITE_END();

    private:
        // We do all our tests in the field of order 5.
        // All polynomials are reduced mod x^4 + x^3 + x^2 + x + 1.
        // In particular, x^5 == 1.
        Cyclotomic zero { 5 };
        Cyclotomic one { 5, 1 };
        Cyclotomic two { 5, 2 };
        Cyclotomic x { 5, { 0, 1 } };
        Cyclotomic x2 { 5, { 0, 0, 1 } };
        Cyclotomic x3 { 5, { 0, 0, 0, 1 } };
        Cyclotomic xPlus1 { 5, { 1, 1 } };
        Cyclotomic xMinus1 { 5, { -1, 1 } };

    public:
        void setUp() {
        }

        void tearDown() {
        }

        template <typename T>
        std::string str(T x) {
            std::ostringstream ans;
            ans << x;
            return ans.str();
        }

        void verifyEqual(const Cyclotomic& result, size_t field,
                std::initializer_list<Rational> coeffs) {
            Cyclotomic expect(field, coeffs);
            if (result != expect) {
                std::ostringstream msg;
                msg << "Cyclotomic mismatch: "
                    << result << " != " << expect;
                CPPUNIT_FAIL(msg.str());
            }
            if (! (result == expect)) {
                std::ostringstream msg;
                msg << "Cyclotomic mismatch: ! "
                    << result << " == " << expect;
                CPPUNIT_FAIL(msg.str());
            }
            if (str(result) != str(expect)) {
                std::ostringstream msg;
                msg << "Cyclotomic string mismatch: "
                    << result << " != " << expect;
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyPlus(const Cyclotomic& a, const Cyclotomic& b,
                size_t field, std::initializer_list<Rational> coeffs) {
            verifyEqual(a + b, field, coeffs);
            verifyEqual((a + zero) + b, field, coeffs);
            verifyEqual(a + (b + zero), field, coeffs);
            verifyEqual((a + zero) + (b + zero), field, coeffs);

            verifyEqual(b + a, field, coeffs);
            verifyEqual((b + zero) + a, field, coeffs);
            verifyEqual(b + (a + zero), field, coeffs);
            verifyEqual((b + zero) + (a + zero), field, coeffs);

            {
                Cyclotomic x(a);
                verifyEqual(x += b, field, coeffs);
            }
            {
                Cyclotomic x(a);
                verifyEqual(x += (b + zero), field, coeffs);
            }
            {
                Cyclotomic x(b);
                verifyEqual(x += a, field, coeffs);
            }
            {
                Cyclotomic x(b);
                verifyEqual(x += (a + zero), field, coeffs);
            }
        }

        void verifyMinus(const Cyclotomic& a, const Cyclotomic& b,
                size_t field, std::initializer_list<Rational> coeffs) {
            verifyEqual(a - b, field, coeffs);
            verifyEqual((a + zero) - b, field, coeffs);
            verifyEqual(a - (b + zero), field, coeffs);
            verifyEqual((a + zero) - (b + zero), field, coeffs);

            {
                Cyclotomic x(a);
                verifyEqual(x -= b, field, coeffs);
            }
            {
                Cyclotomic x(a);
                verifyEqual(x -= (b + zero), field, coeffs);
            }

            verifyPlus(a, -b, field, coeffs);
            {
                Cyclotomic x(b);
                x.negate();
                verifyPlus(a, x, field, coeffs);
            }
        }

        void verifyMult(const Cyclotomic& a, const Rational& b,
                size_t field, std::initializer_list<Rational> coeffs) {
            verifyEqual(a * b, field, coeffs);
            verifyEqual((a + zero) * b, field, coeffs);
            verifyEqual(b * a, field, coeffs);
            verifyEqual(b * (a + zero), field, coeffs);
            {
                Cyclotomic x(a);
                verifyEqual(x *= b, field, coeffs);
            }
        }

        void verifyDiv(const Cyclotomic& a, const Rational& b,
                size_t field, std::initializer_list<Rational> coeffs) {
            verifyEqual(a / b, field, coeffs);
            verifyEqual((a + zero) / b, field, coeffs);
            {
                Cyclotomic x(a);
                verifyEqual(x /= b, field, coeffs);
            }
            verifyMult(a, b.inverse(), field, coeffs);
            {
                Rational x(b);
                x.invert();
                verifyMult(a, x, field, coeffs);
            }
        }

        void verifyMult(const Cyclotomic& a, const Cyclotomic& b,
                size_t field, std::initializer_list<Rational> coeffs) {
            verifyEqual(a * b, field, coeffs);
            verifyEqual((a + zero) * b, field, coeffs);
            verifyEqual(a * (b + zero), field, coeffs);
            verifyEqual((a + zero) * (b + zero), field, coeffs);

            verifyEqual(b * a, field, coeffs);
            verifyEqual((b + zero) * a, field, coeffs);
            verifyEqual(b * (a + zero), field, coeffs);
            verifyEqual((b + zero) * (a + zero), field, coeffs);

            {
                Cyclotomic x(a);
                verifyEqual(x *= b, field, coeffs);
            }
            {
                Cyclotomic x(a);
                verifyEqual(x *= (b + zero), field, coeffs);
            }
            {
                Cyclotomic x(b);
                verifyEqual(x *= a, field, coeffs);
            }
            {
                Cyclotomic x(b);
                verifyEqual(x *= (a + zero), field, coeffs);
            }
        }

        void verifyDiv(const Cyclotomic& a, const Cyclotomic& b,
                size_t field, std::initializer_list<Rational> coeffs) {
            verifyEqual(a / b, field, coeffs);
            verifyEqual((a + zero) / b, field, coeffs);
            verifyEqual(a / (b + zero), field, coeffs);
            verifyEqual((a + zero) / (b + zero), field, coeffs);

            {
                Cyclotomic x(a);
                verifyEqual(x /= b, field, coeffs);
            }
            {
                Cyclotomic x(a);
                verifyEqual(x /= (b + zero), field, coeffs);
            }

            verifyMult(a, b.inverse(), field, coeffs);
            {
                Cyclotomic x(b);
                x.invert();
                verifyMult(a, x, field, coeffs);
            }
        }

        void arithmetic() {
            verifyEqual(zero, 5, {});

            verifyEqual(-one, 5, { -1 });
            verifyEqual(-x, 5, { 0, -1 });
            verifyEqual(-xMinus1, 5, { 1, -1 });

            verifyPlus(zero, zero, 5, {});
            verifyPlus(xPlus1, zero, 5, { 1, 1 });
            verifyPlus(xPlus1, xMinus1, 5, { 0, 2 });

            verifyMinus(zero, zero, 5, {});
            verifyMinus(xPlus1, zero, 5, { 1, 1 });
            verifyMinus(zero, xPlus1, 5, { -1, -1 });
            verifyMinus(xPlus1, xMinus1, 5, { 2 });

            verifyMult(zero, 0, 5, {});
            verifyMult(zero, 1, 5, {});
            verifyMult(zero, 2, 5, {});
            verifyMult(xMinus1, 0, 5, {});
            verifyMult(xMinus1, 1, 5, { -1, 1 });
            verifyMult(xMinus1, -1, 5, { 1, -1 });
            verifyMult(xMinus1, 2, 5, { -2, 2 });
            verifyMult(xMinus1, Rational(1, 2), 5,
                { -Rational(1, 2), Rational(1, 2) });

            verifyDiv(zero, 1, 5, {});
            verifyDiv(zero, 2, 5, {});
            verifyDiv(xMinus1, 1, 5, { -1, 1 });
            verifyDiv(xMinus1, -1, 5, { 1, -1 });
            verifyDiv(xMinus1, Rational(1, 2), 5, { -2, 2 });
            verifyDiv(xMinus1, 2, 5,
                { -Rational(1, 2), Rational(1, 2) });

            verifyMult(zero, zero, 5, {});
            verifyMult(zero, xPlus1, 5, {});
            verifyMult(xPlus1, xMinus1, 5, { -1, 0, 1});
            verifyMult(x2, x3, 5, { 1 });
            verifyMult(x3, x3, 5, { 0, 1 });
            verifyMult(xPlus1, x3 + x2, 5, { -1, -1, 0, 1 });

            verifyDiv(zero, xPlus1, 5, {});
            verifyDiv(zero, xPlus1, 5, {});
            verifyDiv(xMinus1, one, 5, { -1, 1 });
            verifyDiv(xMinus1, -one, 5, { 1, -1 });
            verifyDiv(xMinus1, two, 5, { -Rational(1, 2), Rational(1, 2) });
            verifyDiv(xMinus1, xMinus1, 5, { 1 });
            verifyDiv(two, x2, 5, { 0, 0, 0, 2 });
            verifyDiv(one, x3, 5, { 0, 0, 1 });
            verifyDiv(x3, x2, 5, { 0, 1 });
            verifyDiv(x2, x3, 5, { -1, -1, -1, -1 });

            {
                Cyclotomic x(xMinus1);
                verifyEqual(x + x, 5, {-2, 2});
                verifyEqual(x += x, 5, {-2, 2});
            }
            {
                Cyclotomic x(xMinus1);
                verifyEqual(x - x, 5, {});
                verifyEqual(x -= x, 5, {});
            }
            {
                Cyclotomic x(xMinus1);
                verifyEqual(x * x, 5, {1, -2, 1});
                verifyEqual(x *= x, 5, {1, -2, 1});
            }
            {
                Cyclotomic x(5, {1, 0, 1});
                verifyEqual(x * x, 5, {0, -1, 1, -1});
                verifyEqual(x *= x, 5, {0, -1, 1, -1});
            }
            {
                Cyclotomic x(xMinus1);
                verifyEqual(x / x, 5, {1});
                verifyEqual(x /= x, 5, {1});
            }
        }
};

void addCyclotomic(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(CyclotomicTest::suite());
}

