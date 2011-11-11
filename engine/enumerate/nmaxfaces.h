
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

/*! \file enumerate/nmaxfaces.h
 *  \brief Provides an algorithm for enumerating maximal faces of a
 *  polyhedral cone that satisfy a set of admissibility constraints.
 */

#ifndef __NMAXFACES_H
#ifndef __DOXYGEN
#define __NMAXFACES_H
#endif

#include <vector>

namespace regina {

class NEnumConstraintList;

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
class NMaxAdmissible {
    public:
        /**
         * Enumerates all maximal admissible faces of the given polyhedral cone.
         * The cone must be the intersection of the non-negative orthant in some
         * Euclidean space R^n with a linear subspace.
         *
         * Admissibility is defined by the given set of constraints.  Each
         * constraint requires that at most one of a given set of coordinates
         * can be non-zero; see the NEnumConstraintList class for details.
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
         * using the routine NDoubleDescription::enumerateExtremalRays().
         *
         * The return value is the set of all maximal admissible faces, stored
         * in a newly allocated vector.  Each face \a F is described by a
         * bitmask.  Specifically: if we are working in R^n, then each face is
         * described by a bitmask \a b of length n, where <tt>b[i]</tt>
         * is \c false if every point \a x in \a F has <tt>x[i]=0</tt>,
         * and <tt>b[i]</tt> is \c true if every point \a x in the relative
         * interior of \a F has <tt>x[i] &gt; 0</tt>.
         *
         * \pre TODO: Bitmask type is valid, and is large enough.
         *
         * TODO: Parameters and return value.
         */
        template <class RayIterator, class BitmaskType>
        static std::vector<BitmaskType>* enumerate(
                RayIterator beginExtremalRays, RayIterator endExtremalRays,
                const NEnumConstraintList* constraints);
};

} // namespace regina

// Template definitions

#include "enumerate/nmaxfaces.tcc"

#endif

