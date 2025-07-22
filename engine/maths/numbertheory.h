
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

#ifndef __REGINA_NUMBERTHEORY_H
#ifndef __DOXYGEN
#define __REGINA_NUMBERTHEORY_H
#endif

/*! \file maths/numbertheory.h
 *  \brief Provides miscellaneous number theory routines.
 */

#include <list>
#include <tuple>
#include "regina-core.h"
#include "maths/integer.h"

namespace regina {

/**
 * \defgroup maths Mathematical Support
 * Underlying mathematical gruntwork.
 */

/**
 * Reduces \a k modulo \a modBase to give the smallest possible
 * absolute value.  For instance, `reducedMod(4,10) = 4` but
 * `reducedMod(6,10) = -4`.  In the case of a tie, the positive
 * solution is taken.
 *
 * \pre \a modBase is strictly positive.
 *
 * \exception InvalidArgument The argument \a modBase is zero or negative.
 *
 * \param k the number to reduce modulo \a modBase.
 * \param modBase the modular base in which to work.
 *
 * \ingroup maths
 */
long reducedMod(long k, long modBase);

/**
 * Deprecated routine that calculates the greatest common divisor of
 * two signed integers.  This routine is not recursive.
 *
 * Although the arguments may be negative, the result is guaranteed to
 * be non-negative.  As a special case, gcd(0,0) is considered to be zero.
 *
 * \deprecated Simply use std::gcd(), which was introduced with C++17.
 *
 * \param a one of the two integers to work with.
 * \param b the other integer with which to work.
 * \return the greatest common divisor of \a a and \a b.
 *
 * \ingroup maths
 */
[[deprecated]] long gcd(long a, long b);

/**
 * Calculates the greatest common divisor of two given integers and finds the
 * smallest coefficients with which these integers combine to give their
 * gcd.  This routine is not recursive.
 *
 * Note that the given integers need not be non-negative.  However, the
 * gcd returned is guaranteed to be non-negative.
 * As a special case, gcd(0,0) is considered to be zero.
 *
 * If \a d is the gcd of \a a and \a b, then this routine returns the
 * tuple (\a d, \a u, \a v), where \a u and \a v are coefficients for which:
 *
 * - `u⋅a + v⋅b = d`;
 * - `-|a|/d < v⋅sign(b) ≤ 0 < u⋅sign(a) ≤ |b|/d`.
 *
 * In the special case where one of the given integers is zero, the
 * corresponding coefficient will also be zero and the other coefficient
 * will be 1 or -1 so that `u⋅a + v⋅b = d` still holds.  If both
 * given integers are zero, both of the coefficients will be set to zero.
 *
 * \warning This routine might give incorrect answers if a or b is precisely
 * LONG_MIN, since this value cannot be correctly negated as a \c long.
 *
 * \param a the first integer to compute the gcd of.
 * \param b the second integer to compute the gcd of.
 * \return a tuple containing: the greatest common divisor of \a a and \a b;
 * the final coefficient of \a a; and the final coefficient of \a b.
 *
 * \ingroup maths
 */
std::tuple<long, long, long> gcdWithCoeffs(long a, long b);

/**
 * Deprecated routine that calculates the lowest common multiple of
 * two signed integers.  Although the arguments may be negative, the result
 * is guaranteed to be non-negative.
 *
 * If either of the arguments is zero, the return value will also be zero.
 *
 * Regarding possible overflow:  This routine does not create any temporary
 * integers that are larger in magnitude than the final LCM.
 *
 * \deprecated Simply use std::lcm(), which was introduced with C++17.
 *
 * \param a one of the two integers to work with.
 * \param b the other integer with which to work.
 * \return the lowest common multiple of \a a and \a b.
 *
 * \ingroup maths
 */
[[deprecated]] long lcm(long a, long b);

/**
 * Calculates the multiplicative inverse of one integer modulo another.
 * Specifically, this computes the inverse of \a k modulo \a n, and
 * returns a result between 0 and `n - 1` inclusive.
 *
 * Note that `n == 1` _is_ allowed, and will return 0 for any \a k.
 *
 * \pre \a n is strictly positive;
 * \pre \a n and \a k are coprime.
 *
 * \exception InvalidArgument Either \a n is zero or negative, or the given
 * arguments are not coprime.
 *
 * \param n the modular base in which to work.
 * \param k the number whose multiplicative inverse should be found.
 * \return the inverse \a v for which `k * v == 1 (mod n)`.
 *
 * \ingroup maths
 */
long modularInverse(long n, long k);

} // namespace regina

#endif

