
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

/*! \file enumerate/hilbertcd.h
 *  \brief Provides a modified Contejean-Devie algorithm for Hilbert basis
 * enumeration.
 */

#ifndef __HILBERTCD_H
#ifndef __DOXYGEN
#define __HILBERTCD_H
#endif

#include "regina-core.h"
#include "maths/ray.h"
#include <iterator>
#include <list>
#include <vector>

namespace regina {

class EnumConstraints;
class Ray;

/**
 * \weakgroup enumerate
 * @{
 */

/**
 * Implements a modified Contejean-Devie algorithm for enumerating Hilbert
 * bases.  This is based on the stack-based algorithm described in
 * "An efficient incremental algorithm for solving systems of linear
 * Diophantine equations", Inform. and Comput. 113 (1994), 143-172,
 * and has been modified to allow for additional constraints (such as
 * the quadrilateral constraints from normal surface theory).
 *
 * All routines of interest within this class are static; no object of
 * this class should ever be created.
 *
 * \warning For normal surface theory, the Contejean-Devie algorithm is
 * extremely slow, even when modified to incorporate admissibility
 * constraints.  Consider using the much faster HilbertPrimal or
 * HilbertDual instead.
 *
 * \ifacespython Not present.
 */
class HilbertCD {
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
         * The non-negative orthant is an <i>n</i>-dimensional cone with
         * its vertex at the origin.  The extremal rays of this cone are
         * the \a n non-negative coordinate axes.  This cone also has \a n
         * facets, where the <i>i</i>th facet is the non-negative
         * orthant of the plane perpendicular to the <i>i</i>th coordinate
         * axis.
         *
         * This routine takes a linear subspace, defined by the
         * intersection of a set of hyperplanes through the origin (this
         * subspace is described as a matrix, with each row giving the
         * equation for one hyperplane).
         *
         * The purpose of this routine is to compute the Hilbert basis of
         * the set of all integer points in the intersection of the
         * original cone with this linear subspace.  The resulting list
         * of basis vectors will contain no duplicates or redundancies.
         *
         * The parameter \a constraints may contain a set of validity
         * constraints, in which case this routine will only return \e valid
         * basis elements.  Each validity constraint is of the form "at
         * most one of these coordinates may be non-zero"; see the
         * EnumConstraints class for details.  These contraints have the
         * important property that, although validity is not preserved under
         * addition, \e invalidity is.
         *
         * \pre The template argument RayClass is derived from Ray (or
         * may possibly be Ray itself).
         *
         * \warning For normal surface theory, the Contejean-Devie algorithm is
         * extremely slow, even when modified to incorporate admissibility
         * constraints.  Consider using the much faster HilbertPrimal or
         * HilbertDual instead.
         *
         * @param results the output iterator to which the resulting basis
         * elements will be written; this must accept objects of type
         * <tt>RayClass*</tt>.
         * @param subspace a matrix defining the linear subspace to intersect
         * with the given cone.  Each row of this matrix is the equation
         * for one of the hyperplanes whose intersection forms this linear
         * subspace.  The number of columns in this matrix must be the
         * dimension of the overall space in which we are working.
         * @param constraints a set of validity constraints as described
         * above, or 0 if no additional constraints should be imposed.
         */
        template <class RayClass, class OutputIterator>
        static void enumerateHilbertBasis(OutputIterator results,
            const MatrixInt& subspace, const EnumConstraints* constraints);

    private:
        /**
         * A helper class for Hilbert basis enumeration, describing a
         * single candidate basis vector.
         *
         * The coordinates of the vector are inherited through the
         * superclass Ray.
         *
         * The \a BitmaskType template argument is used to store one bit
         * per coordinate, which is \c false if the coordinate is zero
         * or \c true if the coordinate is non-zero.
         *
         * \pre The template argument \a BitmaskType is one of Regina's
         * bitmask types, such as Bitmask, Bitmask1 or Bitmask2.
         */
        template <class BitmaskType>
        struct VecSpec : public Ray {
            BitmaskType mask_;
                /**< A bitmask indicating which coordinates are zero
                     (\c false) and which are non-zero (\c true). */

            /**
             * Creates the zero vector.
             *
             * @param dim the total dimension of the space (and
             * therefore the toatl length of this vector).
             */
            inline VecSpec(size_t dim);
        };
        /**
         * Identical to the public routine enumerateHilbertBasis(),
         * except that there is an extra template parameter \a BitmaskType.
         * This describes what type should be used for bitmasks that
         * assign flags to individual coordinate positions.
         *
         * All arguments to this function are identical to those for the
         * public routine enumerateHilbertBasis().
         *
         * \pre The bitmask type is one of Regina's bitmask types, such
         * as Bitmask, Bitmask1 or Bitmask2.
         * \pre The type \a BitmaskType can handle at least \a n bits,
         * where \a n is the dimension of the Euclidean space (i.e., the
         * number of columns in \a subspace).
         */
        template <class RayClass, class BitmaskType, class OutputIterator>
        static void enumerateUsingBitmask(OutputIterator results,
            const MatrixInt& subspace, const EnumConstraints* constraints);

        /**
         * Private constructor to ensure that objects of this class are
         * never created.
         */
        HilbertCD();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NHilbertCD has now been renamed to HilbertCD.
 */
REGINA_DEPRECATED typedef HilbertCD NHilbertCD;

/*@}*/

// Inline functions for HilbertCD

inline HilbertCD::HilbertCD() {
}

// Inline functions for HilbertCD::VecSpec

template <class BitmaskType>
inline HilbertCD::VecSpec<BitmaskType>::VecSpec(size_t dim) :
        Ray(dim), mask_(dim) {
    // All vector elements are initialised to zero thanks to the
    // LargeInteger default constructor.
}

} // namespace regina

// Template definitions

#include "enumerate/hilbertcd-impl.h"

#endif

