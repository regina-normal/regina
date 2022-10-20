
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#ifndef __REGINA_BINOM_H
#ifndef __DOXYGEN
#define __REGINA_BINOM_H
#endif

/*! \file maths/binom.h
 *  \brief Provides small binomial coefficients.
 */

#include "regina-core.h"

namespace regina {

/**
 * Returns the binomial coefficient \a n choose \a k in constant time
 * for small arguments (\a n &le; 16).
 *
 * This routine is very fast, since it uses a constant-time lookup.
 * The trade-off is that it can only be used for \a n &le; 16.
 *
 * \note The constraint \a n &le; 16 is large enough for working with
 * triangulations in Regina, since Regina restricts its triangulations to
 * dimension &le; 15 (where each simplex has 16 vertices).
 *
 * \param n the parameter \a n in (\a n choose \a k); this must be
 * between 0 and 16 inclusive.
 * \param k the parameter \a k in (\a n choose \a k); this must be
 * between 0 and \a n inclusive.
 * @return the binomial coefficient \a n choose \a k.
 *
 * \ingroup maths
 */
constexpr int binomSmall(int n, int k);

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
 * \param n the parameter \a n in (\a n choose \a k); this must be
 * between 0 and 29 inclusive.
 * \param k the parameter \a k in (\a n choose \a k); this must be
 * between 0 and \a n inclusive.
 * @return the binomial coefficient \a n choose \a k.
 *
 * \ingroup maths
 */
constexpr long binomMedium(int n, int k);

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
 *
 * \ingroup maths
 */
inline constexpr int binomSmall_[17][17] = {
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,4,6,4,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,5,10,10,5,1,0,0,0,0,0,0,0,0,0,0,0},
    {1,6,15,20,15,6,1,0,0,0,0,0,0,0,0,0,0},
    {1,7,21,35,35,21,7,1,0,0,0,0,0,0,0,0,0},
    {1,8,28,56,70,56,28,8,1,0,0,0,0,0,0,0,0},
    {1,9,36,84,126,126,84,36,9,1,0,0,0,0,0,0,0},
    {1,10,45,120,210,252,210,120,45,10,1,0,0,0,0,0,0},
    {1,11,55,165,330,462,462,330,165,55,11,1,0,0,0,0,0},
    {1,12,66,220,495,792,924,792,495,220,66,12,1,0,0,0,0},
    {1,13,78,286,715,1287,1716,1716,1287,715,286,78,13,1,0,0,0},
    {1,14,91,364,1001,2002,3003,3432,3003,2002,1001,364,91,14,1,0,0},
    {1,15,105,455,1365,3003,5005,6435,6435,5005,3003,1365,455,105,15,1,0},
    {1,16,120,560,1820,4368,8008,11440,12870,11440,8008,4368,1820,560,120,16,1}
};

} // namespace detail

// Inline functions

inline constexpr int binomSmall(int n, int k) {
    return detail::binomSmall_[n][k];
}

inline constexpr long binomMedium(int n, int k) {
    if (n <= 16)
        return detail::binomSmall_[n][k];

    if (k + k > n)
        k = n - k;

    long ans = 1;
    for (int i = 1; i <= k; ++i) {
        ans *= (n + 1 - i);
        ans /= i;
    }
    return ans;
}

} // namespace regina

#endif

