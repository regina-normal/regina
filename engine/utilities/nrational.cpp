
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

namespace regina {

const NRational NRational::zero;
const NRational NRational::one(1);
const NRational NRational::infinity(1, 0);
const NRational NRational::undefined(0, 0);

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

double NRational::getDoubleApprox(bool &inrange) const
 {// first we should decide if the NRational has the right order
  // of magnitude.  a long double stretches from 3.4E-308 to
  // 3.4E+308
  static NLargeInteger pb("100000000000000000000000000000");
  static NRational bigone(NRational(pb*pb*pb*pb*pb*pb*pb*pb*pb*pb));
  static NRational smallone(bigone.inverse() );
  static NLargeInteger nten("10");
  static double dten(10.0);
  static double iten(0.1);

  double magt=1.0; // order of magnitude device through alg...
  double retval=0.0;
  bool negative=false;

  NLargeInteger q, tN,tD,tR;

  if ( (this->abs() > bigone) || (this->abs() < smallone) ) 
        { inrange=false; retval=0.0; }
  if (*this == zero) { inrange=true; retval=0.0; }
  else
   { // we'll do the first 15 sig digits? something like that.
    inrange=true;
    tN = this->getNumerator();
    tD = this->getDenominator();
    // the strategy should be to multiply or divide by 10 until we
    // get a number between 0 and 1, then expand using the euclidean
    // algorithm.
    if (tN < NLargeInteger::zero) { negative=true; tN.negate(); }
    // now tN and tD are positive.
    // we should proceed to scale tN until tN/tD is between 0 and 1, all
    // the while building the appropriate double.
    // so first we scale down, then scale up...
    while (tN >= tD*nten) { magt *= dten; tD *= nten; }
    // now we have tN/tD < 10.
    while (tN < tD) { magt *= iten; tN *= nten; }
    // now we have 1 <= tN/tD < 10. and magt is the order of magnitude
    // of *this.
    for (int i=0; i<15; i++)
        {
        q = tN.euclideanAlg(tD,tR);
        // q must be one of 1,2,...,9.
        retval += ( double(q.longValue()) )*magt;
        tN = tR * nten;
        magt*=iten;        
        }
     if (negative) retval *= (-1.0);
   }
 return retval;
 }


} // namespace regina

