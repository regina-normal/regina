
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

/*! \file approx.h
 *  \brief Provides facilities for working around rounding errors when
 *  dealing with real numbers.
 */

#ifndef __APPROX_H
#ifndef __DOXYGEN
#define __APPROX_H
#endif

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * A very small positive real designed to accommodate for rounding error.
 * Any two numbers within \a epsilon of each other are considered to be
 * equal by the generic zero-testing and sign-testing routines defined in
 * this file (isZero(), isPositive(), isNonNegative() and so on).
 */
extern const double epsilon;

/**
 * Determines whether the given real number is zero.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is approximately zero.
 */
template <class R>
inline bool isZero(R x) {
    return (x > -epsilon && x < epsilon);
}

/**
 * Determines whether the given real number is non-zero.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is approximately non-zero.
 */
template <class R>
inline bool isNonZero(R x) {
    return (x < -epsilon || x > epsilon);
}

/**
 * Determines whether the given real number is strictly positive.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is strictly positive.
 */
template <class R>
inline bool isPositive(R x) {
    return (x > epsilon);
}

/**
 * Determines whether the given real number is strictly negative.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is strictly negative.
 */
template <class R>
inline bool isNegative(R x) {
    return (x < -epsilon);
}

/**
 * Determines whether the given real number is non-negative.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is non-negative.
 */
template <class R>
inline bool isNonNegative(R x) {
    return (x > -epsilon);
}

/**
 * Determines whether the given real number is non-positive.
 * Any number within \a regina::epsilon of zero is considered to be zero.
 *
 * \pre R must be of a floating point real type.
 *
 * \ifacespython Not present.
 *
 * @param x the number to examine.
 * @return \c true if and only if the given number is non-positive.
 */
template <class R>
inline bool isNonPositive(R x) {
    return (x < epsilon);
}

/*@}*/

} // namespace regina

#endif

