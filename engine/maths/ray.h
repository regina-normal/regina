
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

/*! \file maths/ray.h
 *  \brief Provides a fast class for rational rays rooted at the origin.
 */

#ifndef __RAY_H
#ifndef __DOXYGEN
#define __RAY_H
#endif

#include "regina-core.h"
#include "maths/integer.h"
#include "maths/vector.h"

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
 * different subclasses of Ray can happily interact with one another.
 *
 * \warning As of Regina 4.90, this class merges the old functionality
 * of NFastRay and NRay from Regina 4.6.  Since functions are no longer
 * virtual, the old clone() method and intersect() function are gone
 * completely.  Instead you can just use the copy constructor and standard
 * linear operators respectively.
 *
 * \ifacespython Not present.
 */
class REGINA_API Ray : public Vector<LargeInteger> {
    public:
        /**
         * Creates a new ray all of whose coordinates are initialised to zero.
         *
         * @param length the number of elements in the new vector.
         */
        Ray(size_t length);
        /**
         * Creates a new ray that is a clone of the given ray.
         *
         * @param cloneMe the ray to clone.
         */
        Ray(const Vector<LargeInteger>& cloneMe);

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

        /**
         * Negates every element of this vector.
         *
         * This is an optimised implementation that overrides
         * Vector<LargeInteger>::negate().
         */
        inline void negate();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NRay has now been renamed to Ray.
 */
REGINA_DEPRECATED typedef Ray NRay;

/*@}*/

// Inline functions for Ray

inline Ray::Ray(size_t length) : Vector<LargeInteger>(length) {
    // Don't bother passing zero to the parent constructor, since the
    // default LargeInteger constructor already sets elements to zero.
}

inline Ray::Ray(const Vector<LargeInteger>& cloneMe) :
        Vector<LargeInteger>(cloneMe) {
}

inline void Ray::negate() {
    // Slightly more efficient than the default implementation.
    for (LargeInteger* e = elements; e < end; ++e)
        e->negate();
}

} // namespace regina

#endif

