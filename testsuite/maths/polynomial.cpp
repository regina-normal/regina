
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
#include "maths/integer.h"
#include "maths/polynomial.h"
#include "maths/rational.h"
#include "utilities/stringutils.h"
#include "testsuite/utilities/testutilities.h"

using regina::Integer;
using regina::Polynomial;
using regina::Rational;

class PolynomialTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(PolynomialTest);

    CPPUNIT_TEST(set);
    CPPUNIT_TEST(arithmetic);

    CPPUNIT_TEST_SUITE_END();

    private:
        Polynomial<Integer> zero = {};
        Polynomial<Integer> one = { 1 };
        Polynomial<Integer> two = { 2 };
        Polynomial<Integer> a = { 1, -1, 1 };
        Polynomial<Integer> b = { 0, 1, -1, 1 };
        Polynomial<Integer> c = { 0, 0, 1, -1, 1 };
        Polynomial<Integer> d = { 1, -1, 1, -1, 1 };
        Polynomial<Integer> e = { 0, 2, 4, -2, 2 };

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

        template <typename T>
        void verifyEqual(const Polynomial<T>& result,
                std::initializer_list<T> coeffs) {
            Polynomial<T> expect(coeffs);
            if (result != expect) {
                std::ostringstream msg;
                msg << "Polynomial mismatch: "
                    << result << " != " << expect;
                CPPUNIT_FAIL(msg.str());
            }
            if (! (result == expect)) {
                std::ostringstream msg;
                msg << "Polynomial mismatch: ! "
                    << result << " == " << expect;
                CPPUNIT_FAIL(msg.str());
            }
            if (str(result) != str(expect)) {
                std::ostringstream msg;
                msg << "Polynomial string mismatch: "
                    << result << " != " << expect;
                CPPUNIT_FAIL(msg.str());
            }
            if (coeffs.size() == 0) {
                if (! result.isZero()) {
                    std::ostringstream msg;
                    msg << "Polynomial is non-zero: " << result;
                    CPPUNIT_FAIL(msg.str());
                }
                if (result.degree() != 0) {
                    std::ostringstream msg;
                    msg << "Zero polynomial has non-zero degree: " << result;
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (result.isZero()) {
                    std::ostringstream msg;
                    msg << "Polynomial is zero: " << result;
                    CPPUNIT_FAIL(msg.str());
                }
                if (result.degree() + 1 != coeffs.size()) {
                    std::ostringstream msg;
                    msg << "Polynomial has wrong degree: "
                        << result << " degree not "
                        << (coeffs.size() - 1);
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void set() {
            Polynomial<Integer> a = { 1, 2, 1 };

            verifyEqual<Integer>(a, {1, 2, 1});
            a.set(1, 3);
            verifyEqual<Integer>(a, {1, 3, 1});
            a.set(2, 0);
            verifyEqual<Integer>(a, {1, 3});
            a.set(1, 0);
            verifyEqual<Integer>(a, {1});
            a.set(1, 0);
            verifyEqual<Integer>(a, {1});
            a.set(0, 0);
            verifyEqual<Integer>(a, {});
            a.set(0, 0);
            verifyEqual<Integer>(a, {});
            a.set(2, 1);
            verifyEqual<Integer>(a, {0, 0, 1});
        }

        template <typename T>
        void verifyPlus(const Polynomial<T>& a, const Polynomial<T>& b,
                std::initializer_list<T> coeffs) {
            verifyEqual(a + b, coeffs);
            verifyEqual((a + zero) + b, coeffs);
            verifyEqual(a + (b + zero), coeffs);
            verifyEqual((a + zero) + (b + zero), coeffs);

            verifyEqual(b + a, coeffs);
            verifyEqual((b + zero) + a, coeffs);
            verifyEqual(b + (a + zero), coeffs);
            verifyEqual((b + zero) + (a + zero), coeffs);

            {
                Polynomial<T> x(a);
                verifyEqual(x += b, coeffs);
            }
            {
                Polynomial<T> x(a);
                verifyEqual(x += (b + zero), coeffs);
            }
            {
                Polynomial<T> x(b);
                verifyEqual(x += a, coeffs);
            }
            {
                Polynomial<T> x(b);
                verifyEqual(x += (a + zero), coeffs);
            }
        }

        template <typename T>
        void verifyMinus(const Polynomial<T>& a, const Polynomial<T>& b,
                std::initializer_list<T> coeffs) {
            verifyEqual(a - b, coeffs);
            verifyEqual((a + zero) - b, coeffs);
            verifyEqual(a - (b + zero), coeffs);
            verifyEqual((a + zero) - (b + zero), coeffs);

            {
                Polynomial<T> x(a);
                verifyEqual(x -= b, coeffs);
            }
            {
                Polynomial<T> x(a);
                verifyEqual(x -= (b + zero), coeffs);
            }

            verifyPlus(a, -b, coeffs);
            {
                Polynomial<T> x(b);
                x.negate();
                verifyPlus(a, x, coeffs);
            }
        }

        template <typename T>
        void verifyMult(const Polynomial<T>& a, const T& b,
                std::initializer_list<T> coeffs) {
            verifyEqual(a * b, coeffs);
            verifyEqual((a + zero) * b, coeffs);
            verifyEqual(b * a, coeffs);
            verifyEqual(b * (a + zero), coeffs);
            {
                Polynomial<T> x(a);
                verifyEqual(x *= b, coeffs);
            }
        }

        template <typename T>
        void verifyDiv(const Polynomial<T>& a, const T& b,
                std::initializer_list<T> coeffs) {
            verifyEqual(a / b, coeffs);
            verifyEqual((a + zero) / b, coeffs);
            {
                Polynomial<T> x(a);
                verifyEqual(x /= b, coeffs);
            }
        }

        template <typename T>
        void verifyMult(const Polynomial<T>& a, const Polynomial<T>& b,
                std::initializer_list<T> coeffs) {
            verifyEqual(a * b, coeffs);
            verifyEqual((a + zero) * b, coeffs);
            verifyEqual(a * (b + zero), coeffs);
            verifyEqual((a + zero) * (b + zero), coeffs);

            verifyEqual(b * a, coeffs);
            verifyEqual((b + zero) * a, coeffs);
            verifyEqual(b * (a + zero), coeffs);
            verifyEqual((b + zero) * (a + zero), coeffs);

            {
                Polynomial<T> x(a);
                verifyEqual(x *= b, coeffs);
            }
            {
                Polynomial<T> x(a);
                verifyEqual(x *= (b + zero), coeffs);
            }
            {
                Polynomial<T> x(b);
                verifyEqual(x *= a, coeffs);
            }
            {
                Polynomial<T> x(b);
                verifyEqual(x *= (a + zero), coeffs);
            }
        }

        template <typename T>
        void verifyDiv(const Polynomial<T>& a, const Polynomial<T>& b,
                std::initializer_list<T> coeffs) {
            verifyEqual(a / b, coeffs);
            verifyEqual((a + zero) / b, coeffs);
            verifyEqual(a / (b + zero), coeffs);
            verifyEqual((a + zero) / (b + zero), coeffs);

            {
                Polynomial<T> x(a);
                verifyEqual(x /= b, coeffs);
            }
            {
                Polynomial<T> x(a);
                verifyEqual(x /= (b + zero), coeffs);
            }
        }

        void arithmetic() {
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
};

void addPolynomial(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(PolynomialTest::suite());
}

