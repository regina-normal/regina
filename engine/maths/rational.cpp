
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "maths/rational.h"
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

Rational::Rational(long newNum, unsigned long newDen) {
    mpq_init(data);
    if (newDen == 0) {
        if (newNum == 0)
            flavour = f_undefined;
        else
            flavour = f_infinity;
    } else {
        flavour = f_normal;
        mpq_set_si(data, newNum, newDen);
    }
}

Integer Rational::numerator() const {
    if (flavour == f_infinity)
        return Integer::one;
    else if (flavour == f_undefined)
        return Integer::zero;

    Integer ans;
    ans.setRaw(mpq_numref(data));
    return ans;
}

Integer Rational::denominator() const {
    if (flavour != f_normal)
        return Integer::zero;

    Integer ans;
    ans.setRaw(mpq_denref(data));
    return ans;
}

Rational Rational::operator *(const Rational& r) const {
    if (flavour == f_undefined || r.flavour == f_undefined)
        return undefined;
    if (flavour == f_infinity) {
        if (r == zero)
            return undefined;
        return infinity;
    }
    if (r.flavour == f_infinity) {
        if (*this == zero)
            return undefined;
        return infinity;
    }
    Rational ans;
    mpq_mul(ans.data, data, r.data);
    return ans;
}

Rational Rational::operator /(const Rational& r) const {
    if (flavour == f_undefined || r.flavour == f_undefined)
        return undefined;
    if (flavour == f_infinity) {
        if (r.flavour == f_infinity)
            return undefined;
        return infinity;
    }
    if (r.flavour == f_infinity)
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
    if (flavour == f_undefined || r.flavour == f_undefined)
        return undefined;
    if (flavour == f_infinity || r.flavour == f_infinity)
        return infinity;
    Rational ans;
    mpq_add(ans.data, data, r.data);
    return ans;
}

Rational Rational::operator -(const Rational& r) const {
    if (flavour == f_undefined || r.flavour == f_undefined)
        return undefined;
    if (flavour == f_infinity || r.flavour == f_infinity)
        return infinity;
    Rational ans;
    mpq_sub(ans.data, data, r.data);
    return ans;
}

Rational Rational::operator - () const {
    if (flavour != f_normal)
        return *this;
    Rational ans;
    mpq_neg(ans.data, data);
    return ans;
}

Rational Rational::inverse() const {
    if (flavour == f_undefined)
        return undefined;
    if (flavour == f_infinity)
        return zero;
    if (*this == zero)
        return infinity;
    Rational ans;
    mpq_inv(ans.data, data);
    return ans;
}

Rational Rational::abs() const {
    if (flavour != f_normal || mpq_cmp(data, zero.data) >= 0)
        return *this;
    Rational ans;
    mpq_neg(ans.data, data);
    return ans;
}

Rational& Rational::operator += (const Rational& other) {
    if (flavour == f_undefined || other.flavour == f_undefined)
        flavour = f_undefined;
    else if (flavour == f_infinity || other.flavour == f_infinity)
        flavour = f_infinity;
    else
        mpq_add(data, data, other.data);
    return *this;
}

Rational& Rational::operator -= (const Rational& other) {
    if (flavour == f_undefined || other.flavour == f_undefined)
        flavour = f_undefined;
    else if (flavour == f_infinity || other.flavour == f_infinity)
        flavour = f_infinity;
    else
        mpq_sub(data, data, other.data);
    return *this;
}

Rational& Rational::operator *= (const Rational& other) {
    if (flavour == f_undefined || other.flavour == f_undefined)
        flavour = f_undefined;
    else if (flavour == f_infinity) {
        if (other == zero)
            flavour = f_undefined;
        else
            flavour = f_infinity;
    } else if (other.flavour == f_infinity) {
        if (*this == zero)
            flavour = f_undefined;
        else
            flavour = f_infinity;
    } else
        mpq_mul(data, data, other.data);
    return *this;
}

Rational& Rational::operator /= (const Rational& other) {
    if (flavour == f_undefined || other.flavour == f_undefined)
        flavour = f_undefined;
    else if (flavour == f_infinity) {
        if (other.flavour == f_infinity)
            flavour = f_undefined;
        else
            flavour = f_infinity;
    } else if (other.flavour == f_infinity)
        mpq_set(data, zero.data);
    else if (other == zero) {
        if (*this == zero)
            flavour = f_undefined;
        else
            flavour = f_infinity;
    } else
        mpq_div(data, data, other.data);
    return *this;
}

void Rational::invert() {
    if (flavour == f_undefined)
        return;
    else if (flavour == f_infinity) {
        flavour = f_normal;
        mpq_set(data, zero.data);
    } else if (*this == zero) {
        flavour = f_infinity;
    } else
        mpq_inv(data, data);
}

bool Rational::operator == (const Rational& compare) const {
    if (flavour != compare.flavour)
        return false;
    if (flavour != f_normal)
        return true;
    return mpq_equal(data, compare.data);
}

bool Rational::operator < (const Rational& compare) const {
    if (flavour == f_infinity || compare.flavour == f_undefined)
        return false;
    if (flavour == f_undefined || compare.flavour == f_infinity)
        return (compare.flavour != flavour);
    return (mpq_cmp(data, compare.data) < 0);
}

bool Rational::operator > (const Rational& compare) const {
    if (flavour == f_undefined || compare.flavour == f_infinity)
        return false;
    if (flavour == f_infinity || compare.flavour == f_undefined)
        return (compare.flavour != flavour);
    return (mpq_cmp(data, compare.data) > 0);
}

std::ostream& operator << (std::ostream& out, const Rational& rat) {
    if (rat.flavour == Rational::f_infinity)
        out << "Inf";
    else if (rat.flavour == Rational::f_undefined)
        out << "Undef";
    else if (rat.denominator() == 1)
        out << rat.numerator();
    else
        out << rat.numerator() << '/' << rat.denominator();
    return out;
}

std::string Rational::TeX() const {
    std::ostringstream out;
    writeTeX(out);
    return out.str();
}

std::ostream& Rational::writeTeX(std::ostream &out) const {
    if (flavour == Rational::f_infinity)
        out << "\\infty";
    else if (flavour == Rational::f_undefined)
        out << "0/0";
    else if (denominator() == 1)
        out << numerator();
    else
        out << "\\frac{" << numerator() << "}{" << denominator() << "}";
    return out;
}

double Rational::doubleApprox(bool* inRange) const {
    // Initialise maxDouble and minDouble if this has not already been done.
    // Do this even if the current doubleApprox() call is trivial, since we
    // promise this initialisation on the very first call to doubleApprox().
    if (maxDouble.flavour == f_undefined)
        initDoubleBounds();

    // Trivial cases.
    if (flavour == Rational::f_infinity || flavour == Rational::f_undefined) {
        if (inRange)
            *inRange = false;
        return 0.0;
    }

    // Treat zero separately so that "abs < minDouble" is meaningful later on.
    if (*this == zero) {
        if (inRange)
            *inRange = true;
        return 0.0;
    }

    // In bounds or out of bounds?
    Rational magnitude = this->abs();
    if (magnitude < minDouble || magnitude > maxDouble) {
        if (inRange)
            *inRange = false;
        return 0.0;
    }

    // The rational is in range.  Use GMP's native conversion routines,
    // since GMP knows best.
    if (inRange)
        *inRange = true;

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

