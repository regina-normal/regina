
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#ifndef __BINOM_H
#ifndef __DOXYGEN
#define __BINOM_H
#endif

/*! \file maths/binom.h
 *  \brief Provides small binomial coefficients.
 */

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Returns the binomial coefficient \a n choose \a k in constant time
 * for small arguments (\a n &le; 16).
 *
 * This routine is very fast, since it uses a constant-time lookup.
 * The trade-off is that it can only be used for \a n &le; 16.
 *
 * If you need a compile-time constant, you should use the constant
 * FaceNumbering<n-1, k-1>::nFaces instead.  This function is provided for
 * situations where \a n and/or \a k are not known until runtime.
 *
 * \note The constraint \a n &le; 16 is large enough for working with
 * triangulations in Regina, since Regina restricts its triangulations to
 * dimension &le; 15 (where each simplex has 16 vertices).
 *
 * @param n the parameter \a n in (\a n choose \a k); this must be
 * between 0 and 16 inclusive.
 * @param k the parameter \a k in (\a n choose \a k); this must be
 * between 0 and \a n inclusive.
 * @return the binomial coefficient \a n choose \a k.
 */
int binomSmall(int n, int k);

/**
 * Returns the binomial coefficient \a n choose \a k in linear time
 * for medium-sized arguments (\a n &le; 29).
 *
 * This routine computes the binomial coefficient using the standard
 * formula.  It works entirely with native long integers; the constraint
 * \a n &le; 29 is designed to avoid overflow (since all intermediate
 * results are guaranteed to stay below 2^31).
 *
 * If \a n &le; 16 then this routine will use the same constant-time
 * lookup as binomSmall() (i.e., there is no penalty for calling this
 * routine with very small arguments).
 *
 * @param n the parameter \a n in (\a n choose \a k); this must be
 * between 0 and 29 inclusive.
 * @param k the parameter \a k in (\a n choose \a k); this must be
 * between 0 and \a n inclusive.
 * @return the binomial coefficient \a n choose \a k.
 */
REGINA_API long binomMedium(int n, int k);

namespace detail {

/**
 * A lookup table that stores (\a n choose \a k) for all \a n &le; \a 16.
 *
 * For all values 0 &le; \a k &le; \a n &le; 16, the value
 * \a binomSmall_[\a n][\a k] is the binomial coefficient (\a n choose \a k).
 *
 * This array is used in the implementation of the function binomSmall().
 * End users should call binomSmall() instead of referring to this array
 * directly.
 */
REGINA_API extern const int* const binomSmall_[17];

/*@}*/

} // namespace regina::detail

// Inline functions

inline int binomSmall(int n, int k) {
    return detail::binomSmall_[n][k];
}

} // namespace regina

#endif

