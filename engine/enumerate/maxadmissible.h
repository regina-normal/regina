
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file enumerate/maxadmissible.h
 *  \brief Provides an algorithm for enumerating maximal faces of a
 *  polyhedral cone that satisfy a set of admissibility constraints.
 */

#ifndef __REGINA_MAXADMISSIBLE_H
#ifndef __DOXYGEN
#define __REGINA_MAXADMISSIBLE_H
#endif

#include "regina-core.h"
#include "concepts/core.h"
#include "utilities/bitmask.h"
#include <vector>

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class ValidityConstraints;

/**
 * Used to enumerate all maximal admissible faces of a polyhedral cone
 * under a given set of admissibility constraints.  See the routine
 * enumerate() for details.
 *
 * All routines of interest within this class are static; no object of
 * this class should ever be created.
 *
 * \ingroup enumerate
 */
class MaxAdmissible {
    public:
        /**
         * Enumerates all maximal admissible faces of the given polyhedral cone.
         * The cone must be the intersection of the non-negative orthant in some
         * Euclidean space `R^n` with a linear subspace.
         *
         * Admissibility is defined by the given set of constraints.  Each
         * constraint requires that at most one of a given set of coordinates
         * can be non-zero; see the ValidityConstraints class for details.
         * In particular, the quadrilateral constraints from normal surface
         * theory are of this type.
         *
         * It is simple to show that the admissible region of the cone is a
         * union of faces, which we call _admissible faces_.  Those admissible
         * faces that do not appear as a strict subface of some other
         * admissible face are called _maximal admissible faces_.
         * The admissible region can therefore be expressed as the union of
         * all maximal admissible faces.
         *
         * The input for this routine is the list of all admissible extremal rays
         * of the cone.  These should be computed beforehand; for instance,
         * using the routine DoubleDescription::enumerate().
         *
         * The return value is a vector containing all maximal admissible
         * faces.  Each face \a F is described by a bitmask.  Specifically:
         * if we are working in `R^n`, then each face is described by a bitmask
         * \a b of length \a n, where `b[i]` is `false` if every point
         * \a x in \a F has `x[i]=0`, and `b[i]` is `true` if
         * every point \a x in the relative interior of \a F has `x[i] > 0`.
         *
         * \tparam BitmaskType the type to use for the bitmasks that are
         * returned as output.  This type must be able to hold \a n bits,
         * where \a n is the dimension of the underlying space (i.e., the size
         * of each extreme ray given as input).  This is always true of
         * `regina::Bitmask`, but you must pay attention to this requirement
         * when using one of the fast but size-limited types
         * `regina::Bitmask1` or `regina::Bitmask2`.
         * \tparam RayIterator the iterator type used to supply the list of
         * admissible extremal rays given as input.  Typically these extremal
         * rays would be integer vectors.
         *
         * \python The extremal rays should be passed as a Python list
         * of VectorInt objects, not a pair of iterators.  The bitmasks
         * that are returned will be of type regina::Bitmask.
         *
         * \param beginExtremalRays an iterator that begins the set of
         * admissible extremal rays, as described above.
         * \param endExtremalRays an iterator that is past-the-end of the set
         * of admissible extremal rays.
         * \param constraints a set of validity constraints as described above.
         * This may be ValidityConstraints::none to indicate no constraints
         * (in which case there will be just one maximal admissible face).
         * \return a vector containing one bitmask representing each
         * maximal admissible face, as described above.
         */
        template <ReginaBitmask BitmaskType, std::input_iterator RayIterator>
        requires
            IndexedContainer<std::iter_value_t<RayIterator>> &&
            requires(RayIterator it, size_t index) {
                { (*it)[index] == 0 } -> std::convertible_to<bool>;
            }
        static std::vector<BitmaskType> enumerate(
                RayIterator beginExtremalRays, RayIterator endExtremalRays,
                const ValidityConstraints& constraints);

        // Mark this class as non-constructible.
        MaxAdmissible() = delete;
};

} // namespace regina

// Template definitions

#include "enumerate/maxadmissible-impl.h"

#endif

