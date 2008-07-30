
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#ifndef __NUMBERTHEORY_H
#ifndef __DOXYGEN
#define __NUMBERTHEORY_H
#endif

/*! \file numbertheory.h
 *  \brief Provides miscellaneous number theory routines.
 */

#include <list>
#include "utilities/nmpi.h"

namespace regina {

/**
 * \addtogroup maths Mathematical Support
 * Underlying mathematical gruntwork.
 * @{
 */

/**
 * Reduces \a k modulo \a modBase to give the smallest possible
 * absolute value.  For instance, <tt>reducedMod(4,10) = 4</tt> but
 * <tt>reducedMod(6,10) = -4</tt>.  In the case of a tie, the positive
 * solution is taken.
 *
 * \pre \a modBase is strictly positive.
 *
 * \testfull
 *
 * @param k the number to reduce modulo \a modBase.
 * @param modBase the modular base in which to work.
 */
long reducedMod(long k, long modBase);

/**
 * Calculates the greatest common divisor of two signed integers.
 * Although the arguments may be negative, the result is guaranteed to
 * be non-negative.  This routine is not recursive.
 *
 * \testfull
 *
 * @param a one of the two integers to work with.
 * @param b the other integer with which to work.
 * @return the greatest common divisor of \a a and \a b.
 */
long gcd(long a, long b);

/**
 * Calculates the greatest common divisor of two given integers and finds the
 * smallest coefficients with which these integers combine to give their
 * gcd.
 * This routine is not recursive.
 *
 * Note that the given integers need not be non-negative.  However, the
 * gcd returned is guaranteed to be non-negative.
 *
 * If \a d is the gcd of \a a and \a b, the values placed in \a u and \a
 * v will be those for which <tt>u*a + v*b = d</tt>,
 * <tt>-abs(a)/d < v*sign(b) <= 0</tt> and
 * <tt>1 <= u*sign(a) <= abs(b)/d</tt>.
 *
 * In the special case where one of the given integers is zero, the
 * corresponding coefficient will also be zero and the other coefficient
 * will be 1 or -1 so that <tt>u*a + v*b = d</tt> still holds.  If both
 * given integers are zero, both of the coefficients will be set to zero.
 *
 * \testfull
 *
 * @param a one of the integers to work with.
 * @param b the other integer with which to work.
 * @param u a variable into which the final coefficient of \a a will be
 * placed.
 * @param v a variable into which the final coefficient of \a b will be
 * placed.
 * @return the greatest common divisor of \a a and \a b.
 */
long gcdWithCoeffs(long a, long b, long& u, long& v);

/**
 * Calculates the lowest common multiple of two signed integers.
 * Although the arguments may be negative, the result is guaranteed to
 * be non-negative.
 *
 * If either of the arguments is zero, the return value will also be zero.
 *
 * Regarding possible overflow:  This routine does not create any temporary
 * integers that are larger than the final LCM.
 *
 * @param a one of the two integers to work with.
 * @param b the other integer with which to work.
 * @return the lowest common multiple of \a a and \a b.
 */
long lcm(long a, long b);

/**
 * Calculates the multiplicative inverse of one integer modulo another.
 * The inverse returned will be between 0 and <i>n</i>-1 inclusive.
 *
 * \pre \a n and \a k are both strictly positive;
 * \pre \a n and \a k have no common factors.
 *
 * \testfull
 *
 * @param n the modular base in which to work.
 * @param k the number whose multiplicative inverse should be found.
 * @return the inverse \a v for which <tt>k * v == 1 (mod n)</tt>.
 */
unsigned long modularInverse(unsigned long n, unsigned long k);

/**
 * Calculates the prime factorisation of the given integer.
 * All the prime factors will be inserted into the given list.
 * The algorithm used is <b>very neanderthal</b> and should only be used with
 * reasonably sized integers.  Don't use it to do RSA!
 *
 * If a prime factor is repeated, it will be inserted multiple times into
 * the list.  The primes in the list are not guaranteed to appear in any
 * specific order, nor are multiple occurrences of the same prime
 * guaranteed to appear together.
 *
 * Note that once finished the list will contain the prime factors as well
 * as whatever happened to be in the list before this function was
 * called.
 *
 * \pre The given integer is at least 1.
 *
 * \deprecated This routine is old and slow; please consider using the
 * much faster routines from the NPrimes class instead.
 *
 * \ifacespython Argument \a factors is not present; instead this
 * routine returns a python list containing the prime factors.
 *
 * @param n the integer to factorise.
 * @param factors the list into which prime factors will be inserted.
 */
void factorise(unsigned long n, std::list<unsigned long>& factors);

/**
 * Determines all primes up to and including the given upper bound.
 * All the primes found will be inserted into the given list in
 * increasing order.
 *
 * The algorithm currently used is <b>fairly neanderthal</b>.
 *
 * \pre The given list is empty.
 *
 * \deprecated This routine is old and slow; please consider using the
 * much faster routines from the NPrimes class instead.
 *
 * \ifacespython Argument \a primes is not present; instead this routine
 * returns a python list containing the primes up to and including \a roof.
 *
 * @param roof the upper bound up to which primes will be found.
 * @param primes the list into which the primes will be inserted.
 */
void primesUpTo(const NLargeInteger& roof, std::list<NLargeInteger>& primes);

/*@}*/

} // namespace regina

#endif

