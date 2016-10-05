
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

/*! \file maths/approx.h
 *  \brief Deprecated header for managing floating-point roundoff errors.
 */

#ifndef __APPROX_H
#ifndef __DOXYGEN
#define __APPROX_H
#endif

#warning The header approx.h and all of its functions are now deprecated.

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Deprecated constant for managing floating-point roundoff errors.
 *
 * A very small positive real designed to accommodate for rounding error.
 * Any two numbers within \a epsilon of each other are considered to be
 * equal by the generic zero-testing and sign-testing routines defined in
 * this file (isZero(), isPositive(), isNonNegative() and so on).
 *
 * \deprecated This method of using a hard-coded \a epsilon is very blunt.
 * Currently Regina does not provide alternative methods for managing
 * round-off error, though since Regina's computations are exact, this should
 * not be necessary.
 *
 * \ifacespython Not present.
 */
REGINA_DEPRECATED REGINA_API extern const double epsilon;

/**
 * Deprecated routine for managing floating-point roundoff errors.
 *
 * Determines whether the given real number is zero.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \deprecated This method of using a hard-coded \a epsilon is very blunt.
 * Currently Regina does not provide alternative methods for managing
 * round-off error, though since Regina's computations are exact, this should
 * not be necessary.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is approximately zero.
 */
template <class R>
inline REGINA_DEPRECATED bool isZero(R x) {
    return (x > -epsilon && x < epsilon);
}

/**
 * Deprecated routine for managing floating-point roundoff errors.
 *
 * Determines whether the given real number is non-zero.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \deprecated This method of using a hard-coded \a epsilon is very blunt.
 * Currently Regina does not provide alternative methods for managing
 * round-off error, though since Regina's computations are exact, this should
 * not be necessary.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is approximately non-zero.
 */
template <class R>
inline REGINA_DEPRECATED bool isNonZero(R x) {
    return (x < -epsilon || x > epsilon);
}

/**
 * Deprecated routine for managing floating-point roundoff errors.
 *
 * Determines whether the given real number is strictly positive.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \deprecated This method of using a hard-coded \a epsilon is very blunt.
 * Currently Regina does not provide alternative methods for managing
 * round-off error, though since Regina's computations are exact, this should
 * not be necessary.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is strictly positive.
 */
template <class R>
inline REGINA_DEPRECATED bool isPositive(R x) {
    return (x > epsilon);
}

/**
 * Deprecated routine for managing floating-point roundoff errors.
 *
 * Determines whether the given real number is strictly negative.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \deprecated This method of using a hard-coded \a epsilon is very blunt.
 * Currently Regina does not provide alternative methods for managing
 * round-off error, though since Regina's computations are exact, this should
 * not be necessary.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is strictly negative.
 */
template <class R>
inline REGINA_DEPRECATED bool isNegative(R x) {
    return (x < -epsilon);
}

/**
 * Deprecated routine for managing floating-point roundoff errors.
 *
 * Determines whether the given real number is non-negative.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \deprecated This method of using a hard-coded \a epsilon is very blunt.
 * Currently Regina does not provide alternative methods for managing
 * round-off error, though since Regina's computations are exact, this should
 * not be necessary.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is non-negative.
 */
template <class R>
inline REGINA_DEPRECATED bool isNonNegative(R x) {
    return (x > -epsilon);
}

/**
 * Deprecated routine for managing floating-point roundoff errors.
 *
 * Determines whether the given real number is non-positive.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \deprecated This method of using a hard-coded \a epsilon is very blunt.
 * Currently Regina does not provide alternative methods for managing
 * round-off error, though since Regina's computations are exact, this should
 * not be necessary.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is non-positive.
 */
template <class R>
inline REGINA_DEPRECATED bool isNonPositive(R x) {
    return (x < epsilon);
}

/*@}*/

} // namespace regina

#endif

