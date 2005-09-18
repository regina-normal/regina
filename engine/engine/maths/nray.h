
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file nray.h
 *  \brief Deals with rational rays rooted at the origin.
 */

#ifndef __NRAY_H
#ifndef __DOXYGEN
#define __NRAY_H
#endif

#include "utilities/nmpi.h"
#include "maths/nvectordense.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a ray rooted at the origin whose coordinates are rational.
 * Such a ray is a half-line beginning at the origin and is
 * represented by a point it passes through whose coordinates are
 * all integers.
 *
 * Note that scalar multiples of a ray in fact represent the same ray.
 * 
 * \warning Subclasses of NRay \b must override clone() to return a
 * ray of the correct subclass!  Otherwise the vectors returned by
 * vertex enumeration routines might be NRay objects instead of objects
 * of the appropriate derived class.
 *
 * \ifacespython Not present.
 */
class NRay : public NVectorDense<NLargeInteger> {
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

        virtual NVector<NLargeInteger>* clone() const;

        virtual void negate();

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

/**
 * Returns a newly allocated ray representing the intersection
 * of the hyperplane joining two given rays with the given additional
 * hyperplane.  The resulting ray will be in its smallest integral form.
 *
 * The given additional hyperplane must pass through the origin, and is
 * represented by a vector perpendicular to it.
 *
 * If the arguments \a pos and \a neg are on the positive and negative
 * sides of the hyperplane respectively (where positive and
 * negative sides are determined by the sign of the dot product of a
 * ray vector with the hyperplane representation vector), the resulting
 * ray is guaranteed to be a positive multiple of a convex combination of
 * the two original rays.
 *
 * The resulting ray is guaranteed to be of the same subclass of
 * NRay as argument \a neg.
 *
 * \pre The two given rays lie on opposite sides of the given additional
 * hyperplane; neither actually lies within the given additional hyperplane.
 *
 * \ifacespython Not present.
 *
 * @param pos one of the two given rays.
 * @param neg the other of the two given rays.
 * @param hyperplane a perpendicular to the given additional hyperplane.
 * @return a newly allocated ray representing the intersection of
 * \a hyperplane with the hyperplane joining \a a and \a b.
 */
NRay* intersect(const NRay& pos, const NRay& neg,
    const NVector<NLargeInteger>& hyperplane);

/*@}*/

// Inline functions for NRay

inline NRay::NRay(unsigned length) : NVectorDense<NLargeInteger>(length, zero) {
}

inline NRay::NRay(const NVector<NLargeInteger>& cloneMe) :
        NVectorDense<NLargeInteger>(cloneMe) {
}

inline NVector<NLargeInteger>* NRay::clone() const {
    return new NRay(*this);
}

inline void NRay::negate() {
    // Slightly more efficient than the default implementation.
    for (unsigned i=0; i<vectorSize; i++)
        elements[i].negate();
}

} // namespace regina

#endif

