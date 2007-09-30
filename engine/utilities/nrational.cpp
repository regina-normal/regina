
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include "utilities/nrational.h"
#include <cfloat>

namespace regina {

const NRational NRational::zero;
const NRational NRational::one(1);
const NRational NRational::infinity(1, 0);
const NRational NRational::undefined(0, 0);

// These two constants are initialised to their intended values in
// initDoubleBounds().
const NRational NRational::maxDouble(0, 0);
const NRational NRational::minDouble(0, 0);

NRational::NRational(const NLargeInteger& newNum,
        const NLargeInteger& newDen) {
    mpq_init(data);
    if (newDen == 0) {
        if (newNum == 0)
            flavour = f_undefined;
        else
            flavour = f_infinity;
    } else {
        flavour = f_normal;
        mpz_set(mpq_numref(data), newNum.data);
        mpz_set(mpq_denref(data), newDen.data);
    }
}

NRational::NRational(long newNum, unsigned long newDen) {
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

NLargeInteger NRational::getNumerator() const {
    if (flavour == f_infinity)
        return NLargeInteger::one;
    else if (flavour == f_undefined)
        return NLargeInteger::zero;

    NLargeInteger ans;
    mpz_set(ans.data, mpq_numref(data));
    return ans;
}

NLargeInteger NRational::getDenominator() const {
    if (flavour != f_normal)
        return NLargeInteger::zero;

    NLargeInteger ans;
    mpz_set(ans.data, mpq_denref(data));
    return ans;
}

NRational NRational::operator *(const NRational& r) const {
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
    NRational ans;
    mpq_mul(ans.data, data, r.data);
    return ans;
}

NRational NRational::operator /(const NRational& r) const {
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
    NRational ans;
    mpq_div(ans.data, data, r.data);
    return ans;
}

NRational NRational::operator +(const NRational& r) const {
    if (flavour == f_undefined || r.flavour == f_undefined)
        return undefined;
    if (flavour == f_infinity || r.flavour == f_infinity)
        return infinity;
    NRational ans;
    mpq_add(ans.data, data, r.data);
    return ans;
}

NRational NRational::operator -(const NRational& r) const {
    if (flavour == f_undefined || r.flavour == f_undefined)
        return undefined;
    if (flavour == f_infinity || r.flavour == f_infinity)
        return infinity;
    NRational ans;
    mpq_sub(ans.data, data, r.data);
    return ans;
}

NRational NRational::operator - () const {
    if (flavour != f_normal)
        return *this;
    NRational ans;
    mpq_neg(ans.data, data);
    return ans;
}

NRational NRational::inverse() const {
    if (flavour == f_undefined)
        return undefined;
    if (flavour == f_infinity)
        return zero;
    if (*this == zero)
        return infinity;
    NRational ans;
    mpq_inv(ans.data, data);
    return ans;
}

NRational NRational::abs() const {
    if (flavour != f_normal || mpq_cmp(data, zero.data) >= 0)
        return *this;
    NRational ans;
    mpq_neg(ans.data, data);
    return ans;
}

NRational& NRational::operator += (const NRational& other) {
    if (flavour == f_undefined || other.flavour == f_undefined)
        flavour = f_undefined;
    else if (flavour == f_infinity || other.flavour == f_infinity)
        flavour = f_infinity;
    else
        mpq_add(data, data, other.data);
    return *this;
}

NRational& NRational::operator -= (const NRational& other) {
    if (flavour == f_undefined || other.flavour == f_undefined)
        flavour = f_undefined;
    else if (flavour == f_infinity || other.flavour == f_infinity)
        flavour = f_infinity;
    else
        mpq_sub(data, data, other.data);
    return *this;
}

NRational& NRational::operator *= (const NRational& other) {
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

NRational& NRational::operator /= (const NRational& other) {
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

void NRational::invert() {
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

bool NRational::operator == (const NRational& compare) const {
    if (flavour != compare.flavour)
        return false;
    if (flavour != f_normal)
        return true;
    return mpq_equal(data, compare.data);
}

bool NRational::operator < (const NRational& compare) const {
    if (flavour == f_infinity || compare.flavour == f_undefined)
        return false;
    if (flavour == f_undefined || compare.flavour == f_infinity)
        return (compare.flavour != flavour);
    return (mpq_cmp(data, compare.data) < 0);
}

bool NRational::operator > (const NRational& compare) const {
    if (flavour == f_undefined || compare.flavour == f_infinity)
        return false;
    if (flavour == f_infinity || compare.flavour == f_undefined)
        return (compare.flavour != flavour);
    return (mpq_cmp(data, compare.data) > 0);
}

std::ostream& operator << (std::ostream& out, const NRational& rat) {
    if (rat.flavour == NRational::f_infinity)
        out << "Inf";
    else if (rat.flavour == NRational::f_undefined)
        out << "Undef";
    else if (rat.getDenominator() == 1)
        out << rat.getNumerator();
    else
        out << rat.getNumerator() << '/' << rat.getDenominator();
    return out;
}

double NRational::doubleApprox(bool* inRange) const {
    // Initialise maxDouble and minDouble if this has not already been done.
    // Do this even if the current doubleApprox() call is trivial, since we
    // promise this initialisation on the very first call to doubleApprox().
    if (maxDouble.flavour == f_undefined)
        initDoubleBounds();

    // Trivial cases.
    if (flavour == NRational::f_infinity || flavour == NRational::f_undefined) {
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
    NRational magnitude = this->abs();
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

void NRational::initDoubleBounds() {
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

    NLargeInteger maxNum = FLT_RADIX;
    maxNum.raiseToPower(DBL_MAX_EXP - 1);

    NLargeInteger minNum = FLT_RADIX;
    minNum.raiseToPower(- DBL_MIN_EXP);

    // Cast away constness so we can actually change these variables.
    const_cast<NRational&>(maxDouble) = NRational(maxNum, 1);
    const_cast<NRational&>(minDouble) = NRational(1, minNum);
}

} // namespace regina

