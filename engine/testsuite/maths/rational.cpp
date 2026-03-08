
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

#include "maths/rational.h"

#include "testhelper.h"

using regina::Rational;

// Most of these tests are relatively simple so far.

static void verifyInfinite(const Rational& r) {
    SCOPED_TRACE_REGINA(r);

    EXPECT_EQ(r.numerator(), 1);
    EXPECT_EQ(r.denominator(), 0);
    EXPECT_EQ(r.str(), "Inf");

    // Test "typical" assignment.
    Rational alt = Rational(3, 5);
    EXPECT_EQ(alt.numerator(), 3);
    EXPECT_EQ(alt.denominator(), 5);
    EXPECT_EQ(alt.str(), "3/5");

    alt = r;
    EXPECT_EQ(alt.numerator(), 1);
    EXPECT_EQ(alt.denominator(), 0);
    EXPECT_EQ(alt.str(), "Inf");

    // Test self-assignment.
    alt = alt;
    EXPECT_EQ(alt.numerator(), 1);
    EXPECT_EQ(alt.denominator(), 0);
    EXPECT_EQ(alt.str(), "Inf");
}

TEST(RationalTest, infinity) {
    verifyInfinite({ 1, 0 });
    verifyInfinite({ -1, 0 });
    verifyInfinite({ 3, 0 });
    verifyInfinite({ -2, 0 });
    verifyInfinite({ LONG_MAX, 0 });
    verifyInfinite({ LONG_MIN, 0 });
    verifyInfinite({ regina::Integer(LONG_MAX) + 1, regina::Integer::zero });
    verifyInfinite({ regina::LargeInteger(LONG_MAX) + 1,
        regina::LargeInteger::zero });
    verifyInfinite({ regina::Integer(LONG_MIN) - 1, regina::Integer::zero });
    verifyInfinite({ regina::LargeInteger(LONG_MIN) - 1,
        regina::LargeInteger::zero });
    verifyInfinite({ regina::LargeInteger::infinity });
}

static void verifyUndefined(const Rational& r) {
    SCOPED_TRACE_REGINA(r);

    EXPECT_EQ(r.numerator(), 0);
    EXPECT_EQ(r.denominator(), 0);
    EXPECT_EQ(r.str(), "Undef");

    // Test "typical" assignment.
    Rational alt = Rational(3, 5);
    EXPECT_EQ(alt.numerator(), 3);
    EXPECT_EQ(alt.denominator(), 5);
    EXPECT_EQ(alt.str(), "3/5");

    alt = r;
    EXPECT_EQ(alt.numerator(), 0);
    EXPECT_EQ(alt.denominator(), 0);
    EXPECT_EQ(alt.str(), "Undef");

    // Test self-assignment.
    alt = alt;
    EXPECT_EQ(alt.numerator(), 0);
    EXPECT_EQ(alt.denominator(), 0);
    EXPECT_EQ(alt.str(), "Undef");
}

TEST(RationalTest, undefined) {
    verifyUndefined({ 0, 0 });
    verifyUndefined({ regina::Integer::zero, regina::Integer::zero });
    verifyUndefined({ regina::LargeInteger::zero, regina::LargeInteger::zero });
}

template <typename T>
static void verifyInteger(T&& val) {
    Rational r(val);
    SCOPED_TRACE_REGINA(r);

    std::string valStr;
    if constexpr (regina::ReginaInteger<T>)
        valStr = val.str();
    else
        valStr = std::to_string(val);

    EXPECT_EQ(r.numerator(), val);
    EXPECT_EQ(r.denominator(), 1);
    EXPECT_EQ(r.str(), valStr);

    // Test "typical" assignment.
    r = Rational(3, 5);
    EXPECT_EQ(r.numerator(), 3);
    EXPECT_EQ(r.denominator(), 5);
    EXPECT_EQ(r.str(), "3/5");

    r = val;
    EXPECT_EQ(r.numerator(), val);
    EXPECT_EQ(r.denominator(), 1);
    EXPECT_EQ(r.str(), valStr);

    // Test self-assignment.
    r = r;
    EXPECT_EQ(r.numerator(), val);
    EXPECT_EQ(r.denominator(), 1);
    EXPECT_EQ(r.str(), valStr);

    // Test inversion.
    r.invert();
    if (val >= 0) {
        EXPECT_EQ(r.numerator(), 1);
        EXPECT_EQ(r.denominator(), val);
    } else {
        EXPECT_EQ(r.numerator(), -1);
        // Cast to Integer because native C++ integer types cannot always
        // be negated (e.g., -LONG_MIN will overflow).
        EXPECT_EQ(r.denominator(), -regina::Integer(val));
    }
}

TEST(RationalTest, integer) {
    verifyInteger(-1);
    verifyInteger(0);
    verifyInteger(1);
    verifyInteger(LONG_MAX);
    verifyInteger(LONG_MIN);
    verifyInteger(regina::Integer(LONG_MAX) + 1);
    verifyInteger(regina::LargeInteger(LONG_MAX) + 1);
    verifyInteger(regina::Integer(LONG_MIN) - 1);
    verifyInteger(regina::LargeInteger(LONG_MIN) - 1);

    static constexpr const char* HUGE_NEGATIVE =
        "-12364981726394781629378461923786491874569283746672";
    static constexpr const char* HUGE_POSITIVE = HUGE_NEGATIVE + 1; // skip '-'

    verifyInteger(regina::Integer(HUGE_POSITIVE));
    verifyInteger(regina::LargeInteger(HUGE_POSITIVE));
    verifyInteger(regina::Integer(HUGE_NEGATIVE));
    verifyInteger(regina::LargeInteger(HUGE_NEGATIVE));
}

TEST(RationalTest, doubleApprox) {
    EXPECT_THROW({ Rational::infinity.doubleApprox(); }, regina::UnsolvedCase);
    EXPECT_THROW({ Rational::undefined.doubleApprox(); }, regina::UnsolvedCase);

    EXPECT_DOUBLE_EQ(Rational::zero.doubleApprox(), 0.0);
    EXPECT_NEAR(Rational(5, 3).doubleApprox(), 1.666, 0.001);
    EXPECT_NEAR(Rational(-5, 3).doubleApprox(), -1.667, 0.001);

    // Construct something out of int's usual range but well within double's.
    // Here we aim for around 2^70, or about 7^25.
    regina::Integer in(7);
    in.raiseToPower(25);
    regina::Integer three(3);
    EXPECT_NEAR(Rational(in, three).doubleApprox(), 4.470e+20, 0.001e+20);
    EXPECT_NEAR(Rational(-in, three).doubleApprox(), -4.470e+20, 0.001e+20);

    // Construct something well out of double's usual range.
    // Here we aim for around 2^10000, or about 13^2702.
    regina::Integer out(13);
    out.raiseToPower(2702);
    regina::Integer two(2);
    EXPECT_THROW({ Rational(out, two).doubleApprox(); }, regina::UnsolvedCase);
    EXPECT_THROW({ Rational(-out, two).doubleApprox(); }, regina::UnsolvedCase);

    // Check precision bounds close to zero also.
    EXPECT_NEAR(Rational(three, in).doubleApprox(), 2.2370e-21, 0.0001e-21);
    EXPECT_NEAR(Rational(-three, in).doubleApprox(), -2.2370e-21, 0.0001e-21);
    EXPECT_THROW({ Rational(two, out).doubleApprox(); }, regina::UnsolvedCase);
    EXPECT_THROW({ Rational(-two, out).doubleApprox(); }, regina::UnsolvedCase);
}
