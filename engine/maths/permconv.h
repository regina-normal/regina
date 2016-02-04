
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file maths/permconv.h
 *  \brief Conversions between various permutation classes.
 */

#ifndef __PERMCONV_H
#ifndef __DOXYGEN
#define __PERMCONV_H
#endif

#include "regina-core.h"
#include "maths/nperm3.h"
#include "maths/nperm4.h"
#include "maths/nperm5.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Deprecated routine that converts a 4-element permutation to a
 * 5-element permutation.
 *
 * The resulting 5-element permutation will map 4 to 4, and will map
 * 0, 1, 2 and 3 to their respective images under \a p.
 *
 * \deprecated This routine will be removed from some future release of
 * Regina.  You should instead call NPerm5::extend(\a p).
 *
 * @param p the given 4-element permutation.
 * @return the permutation \a p expressed as a permutation of five
 * elements, not four.
 */
REGINA_DEPRECATED REGINA_API NPerm5 perm4to5(const NPerm4& p);

/**
 * Deprecated routine that expresses the given 5-element permutation as a
 * 4-element permutation.
 *
 * The resulting 4-element permutation will map 0, 1, 2 and 3 to their
 * respective images under \a p.  It is assumed that the image of 4 is 4
 * under \a p; otherwise this conversion cannot be performed.
 *
 * \pre The given permutation maps 4 to 4.
 *
 * \deprecated This routine will be removed from some future release of
 * Regina.  You should instead call NPerm4::contract(\a p).
 *
 * @param p the given 5-element permutation.
 * @return the permutation \a p expressed as a permutation of four
 * elements, not five.
 */
REGINA_DEPRECATED REGINA_API NPerm4 perm5to4(const NPerm5& p);

/**
 * Deprecated routine that converts a 3-element permutation to a
 * 4-element permutation.
 *
 * The resulting 4-element permutation will map 3 to 3, and will map
 * 0, 1 and 2 to their respective images under \a p.
 *
 * \deprecated This routine will be removed from some future release of
 * Regina.  You should instead call NPerm4::extend(\a p).
 *
 * @param p the given 3-element permutation.
 * @return the permutation \a p expressed as a permutation of four
 * elements, not three.
 */
REGINA_DEPRECATED REGINA_API NPerm4 perm3to4(const NPerm3& p);

/**
 * Deprecated routine that expresses the given 4-element permutation as a
 * 3-element permutation.
 *
 * The resulting 3-element permutation will map 0, 1 and 2 to their
 * respective images under \a p.  It is assumed that the image of 3 is 3
 * under \a p; otherwise this conversion cannot be performed.
 *
 * \pre The given permutation maps 3 to 3.
 *
 * \deprecated This routine will be removed from some future release of
 * Regina.  You should instead call NPerm3::contract(\a p).
 *
 * @param p the given 4-element permutation.
 * @return the permutation \a p expressed as a permutation of three
 * elements, not four.
 */
REGINA_DEPRECATED REGINA_API NPerm3 perm4to3(const NPerm4& p);

/**
 * Deprecated routine that converts a 3-element permutation to a
 * 5-element permutation.
 *
 * The resulting 5-element permutation will map 3 to 3 and 4 to 4, and will
 * map 0, 1 and 2 to their respective images under \a p.
 *
 * \deprecated This routine will be removed from some future release of
 * Regina.  You should instead call NPerm5::extend(\a p).
 *
 * @param p the given 3-element permutation.
 * @return the permutation \a p expressed as a permutation of five
 * elements, not three.
 */
REGINA_DEPRECATED REGINA_API NPerm5 perm3to5(const NPerm3& p);

/**
 * Deprecated routine that expresses the given 5-element permutation as a
 * 3-element permutation.
 *
 * The resulting 3-element permutation will map 0, 1 and 2 to their
 * respective images under \a p.  It is assumed that \p maps 3,4 to 3,4
 * (but it may do so in either order); otherwise this conversion cannot
 * be performed.
 *
 * \pre The given permutation maps 3,4 to 3,4 in some order.
 *
 * \deprecated This routine will be removed from some future release of
 * Regina.  You should instead call NPerm3::contract(\a p).
 *
 * @param p the given 5-element permutation.
 * @return the permutation \a p expressed as a permutation of three
 * elements, not five.
 */
REGINA_DEPRECATED REGINA_API NPerm3 perm5to3(const NPerm5& p);

/*@}*/

// Inline conversion functions

inline NPerm5 perm4to5(const NPerm4& p) {
    return NPerm5::extend(p);
}

inline NPerm4 perm5to4(const NPerm5& p) {
    return NPerm4::contract(p);
}

inline NPerm4 perm3to4(const NPerm3& p) {
    return NPerm4::extend(p);
}

inline NPerm3 perm4to3(const NPerm4& p) {
    return NPerm3::contract(p);
}

inline NPerm5 perm3to5(const NPerm3& p) {
    return NPerm5::extend(p);
}

inline NPerm3 perm5to3(const NPerm5& p) {
    return NPerm3::contract(p);
}

} // namespace regina

#endif

