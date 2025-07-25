
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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
#include "utilities/exception.h"
#include <cfloat>
#include <sstream>

namespace regina {

const Rational Rational::zero;
const Rational Rational::one(1);
const Rational Rational::infinity(1, 0);
const Rational Rational::undefined(0, 0);

// These two constants are initialised to their intended values in
// initDoubleBounds().
const Rational Rational::maxDouble(0, 0);
const Rational Rational::minDouble(0, 0);

Rational::Rational(long num, unsigned long den) {
    mpq_init(data);
    if (den == 0) {
        if (num == 0)
            flavour = Flavour::Undefined;
        else
            flavour = Flavour::Infinity;
    } else {
        flavour = Flavour::Normal;
        mpq_set_si(data, num, den);
    }
}

Integer Rational::numerator() const {
    if (flavour == Flavour::Infinity)
        return 1;
    else if (flavour == Flavour::Undefined)
        return 0;

    Integer ans;
    ans.setRaw(mpq_numref(data));
    return ans;
}

Integer Rational::denominator() const {
    if (flavour != Flavour::Normal)
        return 0;

    Integer ans;
    ans.setRaw(mpq_denref(data));
    return ans;
}

Rational Rational::operator *(const Rational& r) const {
    if (flavour == Flavour::Undefined || r.flavour == Flavour::Undefined)
        return undefined;
    if (flavour == Flavour::Infinity) {
        if (r == zero)
            return undefined;
        return infinity;
    }
    if (r.flavour == Flavour::Infinity) {
        if (*this == zero)
            return undefined;
        return infinity;
    }
    Rational ans;
    mpq_mul(ans.data, data, r.data);
    return ans;
}

Rational Rational::operator /(const Rational& r) const {
    if (flavour == Flavour::Undefined || r.flavour == Flavour::Undefined)
        return undefined;
    if (flavour == Flavour::Infinity) {
        if (r.flavour == Flavour::Infinity)
            return undefined;
        return infinity;
    }
    if (r.flavour == Flavour::Infinity)
        return zero;
    if (r == zero) {
        if (*this == zero)
            return undefined;
        return infinity;
    }
    Rational ans;
    mpq_div(ans.data, data, r.data);
    return ans;
}

Rational Rational::operator +(const Rational& r) const {
    if (flavour == Flavour::Undefined || r.flavour == Flavour::Undefined)
        return undefined;
    if (flavour == Flavour::Infinity || r.flavour == Flavour::Infinity)
        return infinity;
    Rational ans;
    mpq_add(ans.data, data, r.data);
    return ans;
}

Rational Rational::operator -(const Rational& r) const {
    if (flavour == Flavour::Undefined || r.flavour == Flavour::Undefined)
        return undefined;
    if (flavour == Flavour::Infinity || r.flavour == Flavour::Infinity)
        return infinity;
    Rational ans;
    mpq_sub(ans.data, data, r.data);
    return ans;
}

Rational Rational::operator - () const {
    if (flavour != Flavour::Normal)
        return *this;
    Rational ans;
    mpq_neg(ans.data, data);
    return ans;
}

Rational Rational::inverse() const {
    if (flavour == Flavour::Undefined)
        return undefined;
    if (flavour == Flavour::Infinity)
        return zero;
    if (*this == zero)
        return infinity;
    Rational ans;
    mpq_inv(ans.data, data);
    return ans;
}

Rational Rational::abs() const {
    if (flavour != Flavour::Normal || mpq_cmp(data, zero.data) >= 0)
        return *this;
    Rational ans;
    mpq_neg(ans.data, data);
    return ans;
}

Rational& Rational::operator += (const Rational& other) {
    if (flavour == Flavour::Undefined || other.flavour == Flavour::Undefined)
        flavour = Flavour::Undefined;
    else if (flavour == Flavour::Infinity || other.flavour == Flavour::Infinity)
        flavour = Flavour::Infinity;
    else
        mpq_add(data, data, other.data);
    return *this;
}

Rational& Rational::operator -= (const Rational& other) {
    if (flavour == Flavour::Undefined || other.flavour == Flavour::Undefined)
        flavour = Flavour::Undefined;
    else if (flavour == Flavour::Infinity || other.flavour == Flavour::Infinity)
        flavour = Flavour::Infinity;
    else
        mpq_sub(data, data, other.data);
    return *this;
}

Rational& Rational::operator *= (const Rational& other) {
    if (flavour == Flavour::Undefined || other.flavour == Flavour::Undefined)
        flavour = Flavour::Undefined;
    else if (flavour == Flavour::Infinity) {
        if (other == zero)
            flavour = Flavour::Undefined;
        else
            flavour = Flavour::Infinity;
    } else if (other.flavour == Flavour::Infinity) {
        if (*this == zero)
            flavour = Flavour::Undefined;
        else
            flavour = Flavour::Infinity;
    } else
        mpq_mul(data, data, other.data);
    return *this;
}

Rational& Rational::operator /= (const Rational& other) {
    if (flavour == Flavour::Undefined || other.flavour == Flavour::Undefined)
        flavour = Flavour::Undefined;
    else if (flavour == Flavour::Infinity) {
        if (other.flavour == Flavour::Infinity)
            flavour = Flavour::Undefined;
        else
            flavour = Flavour::Infinity;
    } else if (other.flavour == Flavour::Infinity)
        mpq_set(data, zero.data);
    else if (other == zero) {
        if (*this == zero)
            flavour = Flavour::Undefined;
        else
            flavour = Flavour::Infinity;
    } else
        mpq_div(data, data, other.data);
    return *this;
}

void Rational::invert() {
    if (flavour == Flavour::Undefined)
        return;
    else if (flavour == Flavour::Infinity) {
        flavour = Flavour::Normal;
        mpq_set(data, zero.data);
    } else if (*this == zero) {
        flavour = Flavour::Infinity;
    } else
        mpq_inv(data, data);
}

bool Rational::operator == (const Rational& compare) const {
    if (flavour != compare.flavour)
        return false;
    if (flavour != Flavour::Normal)
        return true;
    return mpq_equal(data, compare.data);
}

std::strong_ordering Rational::operator <=> (const Rational& compare) const {
    if (flavour == Flavour::Infinity)
        return (compare.flavour == Flavour::Infinity ?
            std::strong_ordering::equal : std::strong_ordering::greater);
    if (compare.flavour == Flavour::Infinity)
        return std::strong_ordering::less;

    if (flavour == Flavour::Undefined)
        return (compare.flavour == Flavour::Undefined ?
            std::strong_ordering::equal : std::strong_ordering::less);
    if (compare.flavour == Flavour::Undefined)
        return std::strong_ordering::greater;

    return mpq_cmp(data, compare.data) <=> 0;
}

std::ostream& operator << (std::ostream& out, const Rational& rat) {
    if (rat.flavour == Rational::Flavour::Infinity)
        out << "Inf";
    else if (rat.flavour == Rational::Flavour::Undefined)
        out << "Undef";
    else if (rat.denominator() == 1)
        out << rat.numerator();
    else
        out << rat.numerator() << '/' << rat.denominator();
    return out;
}

std::string Rational::str() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::string Rational::tex() const {
    std::ostringstream out;
    writeTeX(out);
    return out.str();
}

std::ostream& Rational::writeTeX(std::ostream &out) const {
    if (flavour == Rational::Flavour::Infinity)
        out << "\\infty";
    else if (flavour == Rational::Flavour::Undefined)
        out << "0/0";
    else if (denominator() == 1)
        out << numerator();
    else
        out << "\\frac{" << numerator() << "}{" << denominator() << "}";
    return out;
}

double Rational::doubleApprox() const {
    // Initialise maxDouble and minDouble if this has not already been done.
    // Do this even if the current doubleApprox() call is trivial, since we
    // promise this initialisation on the very first call to doubleApprox().
    if (maxDouble.flavour == Flavour::Undefined)
        initDoubleBounds();

    // Trivial cases.
    if (flavour == Flavour::Infinity || flavour == Flavour::Undefined)
        throw UnsolvedCase("Rational is infinite or undefined");

    // Treat zero separately so that "abs < minDouble" is meaningful later on.
    if (*this == zero)
        return 0.0;

    // In bounds or out of bounds?
    Rational magnitude = this->abs();
    if (magnitude < minDouble || magnitude > maxDouble)
        throw UnsolvedCase("Rational is out of range for double");

    // The rational is in range.  Use GMP's native conversion routines,
    // since GMP knows best.
    return mpq_get_d(data);
}

void Rational::initDoubleBounds() {
    // The largest and smallest possible (positive) doubles should be:
    //     FLT_RADIX ^ DBL_MAX_EXP (minus a small amount)
    //     FLT_RADIX ^ (DBL_MIN_EXP - 1)
    //
    // However, I have also seen the following crop up in some places:
    //     FLT_RADIX ^ (DBL_MAX_EXP + 1) (minus a small amount)
    //     FLT_RADIX ^ DBL_MIN_EXP
    //
    // Best to be conservative here and choose the weaker in each case:
    //     FLT_RADIX ^ DBL_MAX_EXP (minus a small amount)
    //     FLT_RADIX ^ DBL_MIN_EXP
    //
    // In fact, we'll be even more conservative and divide by an extra
    // factor of FLT_RADIX to account for "minus a small amount".

    Integer maxNum = FLT_RADIX;
    maxNum.raiseToPower(DBL_MAX_EXP - 1);

    Integer minNum = FLT_RADIX;
    minNum.raiseToPower(- DBL_MIN_EXP);

    // Cast away constness so we can actually change these variables.
    const_cast<Rational&>(maxDouble) = Rational(maxNum, Integer(1));
    const_cast<Rational&>(minDouble) = Rational(Integer(1), minNum);
}

} // namespace regina

