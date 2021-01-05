
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
#include "maths/laurent.h"
#include "utilities/stringutils.h"
#include "testsuite/utilities/testutilities.h"

using regina::Integer;
using regina::Laurent;

class LaurentTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LaurentTest);

    CPPUNIT_TEST(set);
    CPPUNIT_TEST(arithmetic);

    CPPUNIT_TEST_SUITE_END();

    private:
        Laurent<Integer> zero = {};
        Laurent<Integer> zero2 = { 0, {} };
        Laurent<Integer> zero3 = { 2, {} };
        Laurent<Integer> one = { 0, { 1 } };
        Laurent<Integer> two = { 0, { 2 } };
        Laurent<Integer> x2 = { 2, { 1 } };
        Laurent<Integer> a = { -1, { 1, -1, 1 } };
        Laurent<Integer> b = { 0, { 1, -1, 1 } };
        Laurent<Integer> c = { 1, { 1, -1, 1 } };
        Laurent<Integer> d = { -2, { -1, 1, -1, 1 } };
        Laurent<Integer> e = { 4, { 2, 4, -2, 2 } };

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
        void verifyEqual(const Laurent<T>& result,
                long minExp, std::initializer_list<T> coeffs) {
            Laurent<T> expect(minExp, coeffs);
            if (result != expect) {
                std::ostringstream msg;
                msg << "Laurent polynomial mismatch: "
                    << result << " != " << expect;
                CPPUNIT_FAIL(msg.str());
            }
            if (! (result == expect)) {
                std::ostringstream msg;
                msg << "Laurent polynomial mismatch: ! "
                    << result << " == " << expect;
                CPPUNIT_FAIL(msg.str());
            }
            if (str(result) != str(expect)) {
                std::ostringstream msg;
                msg << "Laurent polynomial string mismatch: "
                    << result << " != " << expect;
                CPPUNIT_FAIL(msg.str());
            }
            if (coeffs.size() == 0) {
                if (! result.isZero()) {
                    std::ostringstream msg;
                    msg << "Laurent polynomial is non-zero: " << result;
                    CPPUNIT_FAIL(msg.str());
                }
                if (result.minExp() != 0 || result.maxExp() != 0) {
                    std::ostringstream msg;
                    msg << "Zero Laurent polynomial has non-zero "
                        "min/max exponents: " << result;
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (result.isZero()) {
                    std::ostringstream msg;
                    msg << "Laurent polynomial is zero: " << result;
                    CPPUNIT_FAIL(msg.str());
                }
                if (result.minExp() != minExp) {
                    std::ostringstream msg;
                    msg << "Laurent polynomial has wrong minimum exponent: "
                        << result << " not " << minExp;
                    CPPUNIT_FAIL(msg.str());
                }
                if (result.maxExp() != minExp + coeffs.size() - 1) {
                    std::ostringstream msg;
                    msg << "Laurent polynomial has wrong maximum exponent: "
                        << result << " not " << (minExp + coeffs.size() - 1);
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void set() {
            Laurent<Integer> a = { -1, { 1, 2, 1 } };

            verifyEqual<Integer>(a, -1, {1, 2, 1});
            a.set(0, 3);
            verifyEqual<Integer>(a, -1, {1, 3, 1});
            a.set(1, 0);
            verifyEqual<Integer>(a, -1, {1, 3});
            a.set(0, 0);
            verifyEqual<Integer>(a, -1, {1});
            a.set(1, 0);
            verifyEqual<Integer>(a, -1, {1});
            a.set(-1, 0);
            verifyEqual<Integer>(a, 0, {});
            a.set(-1, 0);
            verifyEqual<Integer>(a, 0, {});
            a.set(2, 1);
            verifyEqual<Integer>(a, 2, {1});

            Laurent<Integer> b = { -1, { 1, 2, 1 } };
            b.set(-1, 0);
            verifyEqual<Integer>(b, 0, {2, 1});
            b.set(0, 0);
            verifyEqual<Integer>(b, 1, {1});
            b.set(-2, 3);
            verifyEqual<Integer>(b, -2, {3, 0, 0, 1});
            b.set(-2, 0);
            verifyEqual<Integer>(b, 1, {1});
            b.set(1, 0);
            verifyEqual<Integer>(b, 0, {});
        }

        template <typename T>
        void verifyPlus(const Laurent<T>& a, const Laurent<T>& b,
                long minExp, std::initializer_list<T> coeffs) {
            verifyEqual(a + b, minExp, coeffs);
            verifyEqual((a + zero) + b, minExp, coeffs);
            verifyEqual(a + (b + zero), minExp, coeffs);
            verifyEqual((a + zero) + (b + zero), minExp, coeffs);

            verifyEqual(b + a, minExp, coeffs);
            verifyEqual((b + zero) + a, minExp, coeffs);
            verifyEqual(b + (a + zero), minExp, coeffs);
            verifyEqual((b + zero) + (a + zero), minExp, coeffs);

            {
                Laurent<T> x(a);
                verifyEqual(x += b, minExp, coeffs);
            }
            {
                Laurent<T> x(a);
                verifyEqual(x += (b + zero), minExp, coeffs);
            }
            {
                Laurent<T> x(b);
                verifyEqual(x += a, minExp, coeffs);
            }
            {
                Laurent<T> x(b);
                verifyEqual(x += (a + zero), minExp, coeffs);
            }
        }

        template <typename T>
        void verifyMinus(const Laurent<T>& a, const Laurent<T>& b,
                long minExp, std::initializer_list<T> coeffs) {
            verifyEqual(a - b, minExp, coeffs);
            verifyEqual((a + zero) - b, minExp, coeffs);
            verifyEqual(a - (b + zero), minExp, coeffs);
            verifyEqual((a + zero) - (b + zero), minExp, coeffs);

            {
                Laurent<T> x(a);
                verifyEqual(x -= b, minExp, coeffs);
            }
            {
                Laurent<T> x(a);
                verifyEqual(x -= (b + zero), minExp, coeffs);
            }

            verifyPlus(a, -b, minExp, coeffs);
            {
                Laurent<T> x(b);
                x.negate();
                verifyPlus(a, x, minExp, coeffs);
            }
        }

        template <typename T>
        void verifyMult(const Laurent<T>& a, const T& b,
                long minExp, std::initializer_list<T> coeffs) {
            verifyEqual(a * b, minExp, coeffs);
            verifyEqual((a + zero) * b, minExp, coeffs);
            verifyEqual(b * a, minExp, coeffs);
            verifyEqual(b * (a + zero), minExp, coeffs);
            {
                Laurent<T> x(a);
                verifyEqual(x *= b, minExp, coeffs);
            }
        }

        template <typename T>
        void verifyDiv(const Laurent<T>& a, const T& b,
                long minExp, std::initializer_list<T> coeffs) {
            verifyEqual(a / b, minExp, coeffs);
            verifyEqual((a + zero) / b, minExp, coeffs);
            {
                Laurent<T> x(a);
                verifyEqual(x /= b, minExp, coeffs);
            }
        }

        template <typename T>
        void verifyMult(const Laurent<T>& a, const Laurent<T>& b,
                long minExp, std::initializer_list<T> coeffs) {
            verifyEqual(a * b, minExp, coeffs);
            verifyEqual((a + zero) * b, minExp, coeffs);
            verifyEqual(a * (b + zero), minExp, coeffs);
            verifyEqual((a + zero) * (b + zero), minExp, coeffs);

            verifyEqual(b * a, minExp, coeffs);
            verifyEqual((b + zero) * a, minExp, coeffs);
            verifyEqual(b * (a + zero), minExp, coeffs);
            verifyEqual((b + zero) * (a + zero), minExp, coeffs);

            {
                Laurent<T> x(a);
                verifyEqual(x *= b, minExp, coeffs);
            }
            {
                Laurent<T> x(a);
                verifyEqual(x *= (b + zero), minExp, coeffs);
            }
            {
                Laurent<T> x(b);
                verifyEqual(x *= a, minExp, coeffs);
            }
            {
                Laurent<T> x(b);
                verifyEqual(x *= (a + zero), minExp, coeffs);
            }
        }

        void arithmetic() {
            verifyEqual<Integer>(zero, 0, {});
            verifyEqual<Integer>(zero2, 0, {});
            verifyEqual<Integer>(zero3, 0, {});

            verifyEqual<Integer>(-zero, 0, {});
            verifyEqual<Integer>(-a, -1, {-1, 1, -1});
            verifyEqual<Integer>(-c, 1, {-1, 1, -1});

            verifyPlus<Integer>(zero, zero, 0, {});
            verifyPlus<Integer>(one, two, 0, {3});
            verifyPlus<Integer>(a, zero, -1, {1, -1, 1});
            verifyPlus<Integer>(a, one, -1, {1, 0, 1});
            verifyPlus<Integer>(b, one, 0, {2, -1, 1});
            verifyPlus<Integer>(b, x2, 0, {1, -1, 2});
            verifyPlus<Integer>(a, b, -1, {1, 0, 0, 1});
            verifyPlus<Integer>(a, a + a, -1, {3, -3, 3});
            verifyPlus<Integer>(a, e, -1, {1, -1, 1, 0, 0, 2, 4, -2, 2});
            verifyPlus<Integer>(b, d, -2, {-1, 1, 0, 0, 1});
            verifyPlus<Integer>(c, d, -2, {-1, 1, -1, 2, -1, 1});

            verifyMinus<Integer>(zero, zero, 0, {});
            verifyMinus<Integer>(one, two, 0, {-1});
            verifyMinus<Integer>(a, zero, -1, {1, -1, 1});
            verifyMinus<Integer>(zero, a, -1, {-1, 1, -1});
            verifyMinus<Integer>(b, one, 1, {-1, 1});
            verifyMinus<Integer>(b, x2, 0, {1, -1});
            verifyMinus<Integer>(a, b, -1, {1, -2, 2, -1});
            verifyMinus<Integer>(b, a, -1, {-1, 2, -2, 1});
            verifyMinus<Integer>(a, a + a, -1, {-1, 1, -1});
            verifyMinus<Integer>(a, e, -1, {1, -1, 1, 0, 0, -2, -4, 2, -2});
            verifyMinus<Integer>(e, a, -1, {-1, 1, -1, 0, 0, 2, 4, -2, 2});
            verifyMinus<Integer>(b, d, -2, {1, -1, 2, -2, 1});
            verifyMinus<Integer>(d, b, -2, {-1, 1, -2, 2, -1});
            verifyMinus<Integer>(a, a, 0, {});

            verifyMult<Integer>(zero, 0, 0, {});
            verifyMult<Integer>(zero, 1, 0, {});
            verifyMult<Integer>(zero, 2, 0, {});
            verifyMult<Integer>(a, 0, 0, {});
            verifyMult<Integer>(a, 1, -1, {1, -1, 1});
            verifyMult<Integer>(a, -1, -1, {-1, 1, -1});
            verifyMult<Integer>(a, 2, -1, {2, -2, 2});

            verifyDiv<Integer>(zero, 1, 0, {});
            verifyDiv<Integer>(zero, 2, 0, {});
            verifyDiv<Integer>(a, 1, -1, {1, -1, 1});
            verifyDiv<Integer>(a, -1, -1, {-1, 1, -1});
            verifyDiv<Integer>(e, 2, 4, {1, 2, -1, 1});

            verifyMult<Integer>(zero, zero, 0, {});
            verifyMult<Integer>(zero, a, 0, {});
            verifyMult<Integer>(a, b, -1, {1, -2, 3, -2, 1});

            {
                Laurent<Integer> x(a);
                verifyEqual<Integer>(x + x, -1, {2, -2, 2});
                verifyEqual<Integer>(x += x, -1, {2, -2, 2});
            }
            {
                Laurent<Integer> x(a);
                verifyEqual<Integer>(x - x, 0, {});
                verifyEqual<Integer>(x -= x, 0, {});
            }
            {
                Laurent<Integer> x(-1, {1, 0, 1});
                verifyEqual<Integer>(x * x, -2, {1, 0, 2, 0, 1});
                verifyEqual<Integer>(x *= x, -2, {1, 0, 2, 0, 1});
            }
        }
};

void addLaurent(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(LaurentTest::suite());
}

