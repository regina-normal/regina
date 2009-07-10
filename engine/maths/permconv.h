
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file maths/permconv.h
 *  \brief Conversions between various permutation classes.
 */

#ifndef __PERMCONV_H
#ifndef __DOXYGEN
#define __PERMCONV_H
#endif

#include "maths/nperm3.h"
#include "maths/nperm4.h"
#include "maths/nperm5.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Converts the given 4-element permutation to a 5-element permutation.
 * The resulting 5-element permutation will map 4 to 4, and will map
 * 0, 1, 2 and 3 to their respective images under \a p.
 *
 * @param p the given 4-element permutation.
 * @return the permutation \a p expressed as a permutation of five
 * elements, not four.
 */
NPerm5 perm4to5(const NPerm4& p);

/**
 * Expresses the given 5-element permutation as a 4-element permutation.
 * The resulting 4-element permutation will map 0, 1, 2 and 3 to their
 * respective images under \a p.  It is assumed that the image of 4 is 4
 * under \a p; otherwise this conversion cannot be performed.
 *
 * \pre The given permutation maps 4 to 4.
 *
 * @param p the given 5-element permutation.
 * @return the permutation \a p expressed as a permutation of four
 * elements, not five.
 */
NPerm4 perm5to4(const NPerm5& p);

/*@}*/

// Inline conversion functions

inline NPerm5 perm4to5(const NPerm4& p) {
    // Cast the NPerm4 code (char) to an NPerm5 code (unsigned).
    unsigned code = p.getPermCode();
    return NPerm5::fromPermCode(
        (code & 0x0003) |
        ((code & 0x000c) << 1) |
        ((code & 0x0030) << 2) |
        ((code & 0x00c0) << 3) |
        (4 << 12));
}

inline NPerm4 perm5to4(const NPerm5& p) {
    unsigned code = p.getPermCode();
    return NPerm4::fromPermCode(static_cast<unsigned char>(
        (code & 0x03) |
        ((code >> 1) & 0x0c) |
        ((code >> 2) & 0x30) |
        ((code >> 3) & 0xc0)));
}

} // namespace regina

#endif

