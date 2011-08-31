
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
 *  \brief Provides a fast class for rational rays rooted at the origin.
 */

#ifndef __NRAY_H
#ifndef __DOXYGEN
#define __NRAY_H
#endif

#include "regina-core.h"
#include "maths/nlargeinteger.h"
#include "maths/nvector.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * A fast class for storing a ray rooted at the origin whose
 * coordinates are rational.  Such a ray is a half-line beginning at
 * the origin and is represented by an integer point that it passes through.
 * Positive scalar multiples of a ray are considered to represent the same ray.
 *
 * This class is intended for serious computation, and as a result it
 * has a streamlined implementation with no virtual methods.  It can be
 * subclassed, but since there are no virtual methods, type information
 * must generally be known at compile time.  Nevertheless, in many respects,
 * different subclasses of NRay can happily interact with one another.
 *
 * \warning As of Regina 4.90, this class merges the old functionality
 * of NFastRay and NRay from Regina 4.6.  Since functions are no longer
 * virtual, the old clone() method and intersect() function are gone
 * completely.  Instead you can just use the copy constructor and standard
 * linear operators respectively.
 *
 * \ifacespython Not present.
 */
class REGINA_API NRay : public NVector<NLargeInteger> {
    public:
        /**
         * Creates a new ray all of whose coordinates are initialised to zero.
         *
         * @param length the number of elements in the new vector.
         */
        NRay(unsigned length);
        /**
         * Creates a new ray that is a clone of the given ray.
         *
         * @param cloneMe the ray to clone.
         */
        NRay(const NVector<NLargeInteger>& cloneMe);

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

        inline void negate();
};

/*@}*/

// Inline functions for NRay

inline NRay::NRay(unsigned length) : NVector<NLargeInteger>(length) {
    // Don't bother passing zero to the parent constructor, since the
    // default NLargeInteger constructor already sets elements to zero.
}

inline NRay::NRay(const NVector<NLargeInteger>& cloneMe) :
        NVector<NLargeInteger>(cloneMe) {
}

inline void NRay::negate() {
    // Slightly more efficient than the default implementation.
    for (NLargeInteger* e = elements; e < end; ++e)
        e->negate();
}

} // namespace regina

#endif

