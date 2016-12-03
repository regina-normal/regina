
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

/*! \file enumerate/maxadmissible.h
 *  \brief Provides an algorithm for enumerating maximal faces of a
 *  polyhedral cone that satisfy a set of admissibility constraints.
 */

#ifndef __MAXADMISSIBLE_H
#ifndef __DOXYGEN
#define __MAXADMISSIBLE_H
#endif

#include "regina-core.h"
#include "utilities/bitmask.h"
#include <vector>

namespace regina {

class EnumConstraints;

/**
 * \weakgroup enumerate
 * @{
 */

/**
 * Used to enumerate all maximal admissible faces of a polyhedral cone
 * under a given set of admissibility constraints.  See the routine
 * enumerate() for details.
 *
 * All routines of interest within this class are static; no object of
 * this class should ever be created.
 *
 * \ifacespython Not present.
 */
class MaxAdmissible {
    public:
        /**
         * Enumerates all maximal admissible faces of the given polyhedral cone.
         * The cone must be the intersection of the non-negative orthant in some
         * Euclidean space R^n with a linear subspace.
         *
         * Admissibility is defined by the given set of constraints.  Each
         * constraint requires that at most one of a given set of coordinates
         * can be non-zero; see the EnumConstraints class for details.
         * In particular, the quadrilateral constraints from normal surface
         * theory are of this type.
         *
         * It is simple to show that the admissible region of the cone is a
         * union of faces, which we call "admissible faces".  Those admissible
         * faces that do not appear as a strict subface of some other
         * admissible face are called "maximal admissible faces".
         * The admissible region can therefore be expressed as the union of
         * all maximal admissible faces.
         *
         * The input for this routine is the set of all admissible extremal rays
         * of the cone.  These should be computed beforehand; for instance,
         * using the routine DoubleDescription::enumerateExtremalRays().
         *
         * The return value is the set of all maximal admissible faces, stored
         * in a newly allocated vector.  Each face \a F is described by a
         * bitmask.  Specifically: if we are working in R^n, then each face is
         * described by a bitmask \a b of length n, where <tt>b[i]</tt>
         * is \c false if every point \a x in \a F has <tt>x[i]=0</tt>,
         * and <tt>b[i]</tt> is \c true if every point \a x in the relative
         * interior of \a F has <tt>x[i] &gt; 0</tt>.
         *
         * \pre The template argument RayIterator should be an iterator
         * type that, when dereferenced, can be cast to (const Ray&).
         *
         * \pre The template argument BitmaskType is one of the bitmask
         * types Bitmask, Bitmask1 or Bitmask2.
         *
         * \pre Bitmasks of type BitmaskType can hold \a n bits, where
         * \a n is the dimension of the underlying space (i.e., the size
         * of the input vectors described by \a beginExtremalRays and
         * \a endExtremalRays).  This is always true of Bitmask, but
         * you must be careful when using one of the fast but size-limited
         * types Bitmask1 or Bitmask2.
         *
         * @param beginExtremalRays an iterator that begins the set of
         * admissible extremal rays, as described above.  Typically this would
         * be rays.begin() if \a rays is a standard container type.
         * @param endExtremalRays an iterator that is past-the-end of the set
         * of admissible extremal rays.  Typically this would be rays.end()
         * if \a rays is a standard container type.
         * @param constraints a set of validity constraints as described
         * above.  This may be 0 to indicate no constraints (in which
         * case there will be just one maximal admissible face).
         * @return a newly allocated list containing one bitmask
         * representing each maximal admissible face, as described above.
         */
        template <class BitmaskType, class RayIterator>
        static std::vector<BitmaskType>* enumerate(
                RayIterator beginExtremalRays, RayIterator endExtremalRays,
                const EnumConstraints* constraints);
};

/*@}*/

} // namespace regina

// Template definitions

#include "enumerate/maxadmissible-impl.h"

#endif

