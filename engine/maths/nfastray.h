
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

/*! \file maths/nray.h
 *  \brief Provides a fast but inflexible rational ray class for heavy
 *  computation.
 */

#ifndef __NFASTRAY_H
#ifndef __DOXYGEN
#define __NFASTRAY_H
#endif

#include "maths/nfastvector.h"
#include "maths/nlargeinteger.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * A fast but inflexible class storing a ray rooted at the origin whose
 * coordinates are rational.
 *
 * This class is similar to NRay but is better suited to heavy
 * computation.  Like its base class NFastVector, it has a streamlined
 * implementation with no virtual methods, but it cannot talk easily to
 * any vector class other than itself.  For a slower but more flexible
 * ray class, see the NRay hierarchy instead.
 *
 * As with the NRay class, the ray described by this class is a
 * half-line beginning at the origin, represented by an integer point that
 * it passes through.  Positive scalar multiples of a ray are considered to
 * represent the same ray.
 *
 * \ifacespython Not present.
 */
class NFastRay : public NFastVector<NLargeInteger> {
    public:
        /**
         * Creates a new ray all of whose coordinates are initialised to zero.
         *
         * @param length the number of elements in the new vector.
         */
        NFastRay(unsigned length);
        /**
         * Creates a new ray that is a clone of the given ray.
         *
         * @param cloneMe the ray to clone.
         */
        NFastRay(const NFastVector<NLargeInteger>& cloneMe);

        /**
         * Scales this vector down by the greatest common divisor of all
         * its elements.  The resulting vector will be the smallest
         * multiple of the original that maintains integral entries, and
         * these entries will have the same signs as the originals.
         *
         * This routine thus reduces a ray to its smallest possible
         * representation.
         *
         * This routine poses no problem for vectors containing infinite
         * elements; such elements are simply ignored and left at
         * infinity.
         */
        void scaleDown();
};

/*@}*/

// Inline functions for NFastRay

inline NFastRay::NFastRay(unsigned length) : NFastVector<NLargeInteger>(
        length) {
    // Don't bother passing zero to the parent constructor, since the
    // default NLargeInteger constructor already sets elements to zero.
}

inline NFastRay::NFastRay(const NFastVector<NLargeInteger>& cloneMe) :
        NFastVector<NLargeInteger>(cloneMe) {
}

} // namespace regina

#endif

