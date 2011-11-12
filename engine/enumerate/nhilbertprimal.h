
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

/*! \file enumerate/nhilbertprimal.h
 *  \brief Provides a modified primal algorithm for Hilbert basis enumeration.
 */

#ifndef __NHILBERTPRIMAL_H
#ifndef __DOXYGEN
#define __NHILBERTPRIMAL_H
#endif

#include "regina-core.h"
#include "maths/nmatrixint.h"
#include <iterator>
#include <list>
#include <vector>

namespace regina {

class NEnumConstraintList;
class NMatrixInt;
class NProgressMessage;
class NRay;

/**
 * \weakgroup enumerate
 * @{
 */

/**
 * Implements a modified primal algorithm for enumerating Hilbert bases.
 * This incorporates the primal algorithm described in
 * "Normaliz: Algorithms for affine monoids and rational cones",
 * Winfried Bruns and Bogdan Ichim, J. Algebra 324 (2010), 1098-1113,
 * and has been modified to allow for additional constraints (such as
 * the quadrilateral constraints from normal surface theory).
 *
 * To summarise: the algorithm first enumerates extremal rays of the rational
 * cone, and then decomposes the admissible region of the cone (where the
 * extra constraints are satisfied) into maximal admissible faces.
 * It calls Normaliz directly to enumerate the Hilbert basis for
 * each maximal admissible faces, and finally combines these into a basis
 * for the entire space.
 *
 * All routines of interest within this class are static; no object of
 * this class should ever be created.
 *
 * \ifacespython Not present.
 */
class NHilbertPrimal {
    public:
        /**
         * Determines the Hilbert basis that generates all integer
         * points in the intersection of the <i>n</i>-dimensional
         * non-negative orthant with some linear subspace.
         * The resulting basis elements will be of the class \a RayClass,
         * will be newly allocated, and will be written to the given output
         * iterator.  Their deallocation is the responsibility of whoever
         * called this routine.
         *
         * The intersection of the non-negative orthant with this linear
         * subspace is a pointed polyhedral cone with apex at the origin,
         * and this routine requires the extremal rays of this cone to
         * be provided as input.  The extremal rays can be computed
         * using NDoubleDescription::enumerate() in general cases,
         * though sometimes (such as with normal surface theory) more
         * efficient methods are available.
         *
         * This routine computes the Hilbert basis of all integer points in
         * this cone.  The resulting list of basis vectors will contain
         * no duplicates or redundancies.
         *
         * The parameter \a constraints may contain a set of validity
         * constraints, in which case this routine will only return \e valid
         * basis elements.  Each validity constraint is of the form "at
         * most one of these coordinates may be non-zero"; see the
         * NEnumConstraintList class for details.  These contraints have the
         * important property that, although validity is not preserved under
         * addition, \e invalidity is.
         *
         * A text-based progress watcher may be passed for progress reporting.
         * If so, this routine will poll for cancellation requests accordingly.
         *
         * \pre If \a constraints is passed, then the given list of
         * extremal rays contains \e only those extremal rays that satisfy
         * all of the given constraints.
         * \pre The template argument RayClass is derived from NRay (or
         * may possibly be NRay itself).
         * \pre The template argument RayIterator is a forward iterator type,
         * and when dereferenced can be cast to (const NRay*).
         *
         * @param results the output iterator to which the resulting basis
         * elements will be written; this must accept objects of type
         * <tt>RayClass*</tt>.
         * @param raysBegin an iterator pointing to the beginning of the
         * list of extremal rays.
         * @param raysEnd an iterator pointing past the end of the
         * list of extremal rays.
         * @param constraints a set of validity constraints as described
         * above, or 0 if no additional constraints should be imposed.
         * @param progress a text-based progress watcher through which progress
         * will be reported, or 0 if no progress reporting is required.
         * Note that NProgress::setFinished() will \e not be called, since
         * whoever called this routine may need to do further processing.
         */
        template <class RayClass, class RayIterator, class OutputIterator>
        static void enumerateHilbertBasis(OutputIterator results,
            const RayIterator& raysBegin, const RayIterator& raysEnd,
            const NEnumConstraintList* constraints,
            NProgressMessage* progress = 0);

    private:
        /**
         * Identical to the public routine enumerateHilbertBasis(),
         * except that there is an extra template parameter \a BitmaskType.
         * This describes what type should be used for bitmasks that
         * represent maximal admissible faces.
         *
         * All arguments to this function are identical to those for the
         * public routine enumerateHilbertBasis().
         *
         * \pre The bitmask type is one of Regina's bitmask types, such
         * as NBitmask, NBitmask1 or NBitmask2.
         * \pre The type \a BitmaskType can handle at least \a n bits,
         * where \a n is the dimension of the Euclidean space (i.e., the
         * number of columns in \a subspace).
         */
        template <class RayClass, class BitmaskType,
            class RayIterator, class OutputIterator>
        static void enumerateUsingBitmask(OutputIterator results,
            const RayIterator& raysBegin, const RayIterator& raysEnd,
            const NEnumConstraintList* constraints, NProgressMessage* progress);

        /**
         * Determines whether the given ray lies in the face specified
         * by the given bitmask.
         *
         * Faces are described using bitmasks in the same manner as described
         * by NMaxAdmissible::enumerate(), where \c true represents a
         * coordinate that is non-zero in the relative interior of the
         * face, and \c false represents a coordinate that is always
         * zero throughout the face.
         *
         * @param ray the ray to test, given as a vector of integers.
         * @param face the face to test, given as a bitmask.
         * @return \c true if the given ray lies within the given face,
         * or \c false otherwise.
         */
        template <class BitmaskType>
        static bool inFace(const NRay& ray, const BitmaskType& face);

        /**
         * Private constructor to ensure that objects of this class are
         * never created.
         */
        NHilbertPrimal();
};

/*@}*/

// Inline functions for NHilbertPrimal

inline NHilbertPrimal::NHilbertPrimal() {
}

} // namespace regina

// Template definitions

#include "enumerate/nhilbertprimal.tcc"

#endif

