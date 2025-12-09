
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

/*! \file enumerate/hilbertcd.h
 *  \brief Provides a modified Contejean-Devie algorithm for Hilbert basis
 * enumeration.
 */

#ifndef __REGINA_HILBERTCD_H
#ifndef __DOXYGEN
#define __REGINA_HILBERTCD_H
#endif

#include "regina-core.h"
#include "concepts/maths.h"
#include "maths/matrix.h"
#include "maths/vector.h"
#include <iterator>
#include <list>
#include <vector>

namespace regina {

class ValidityConstraints;

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
 * \ingroup enumerate
 */
class HilbertCD {
    public:
        /**
         * Determines the Hilbert basis that generates all integer
         * points in the intersection of the <i>n</i>-dimensional
         * non-negative orthant with some linear subspace.
         * The resulting basis elements will be of the class \a Ray,
         * and will be passed into the given action function one at a time.
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
         * constraints, in which case this routine will only return _valid_
         * basis elements.  Each validity constraint is of the form "at
         * most one of these coordinates may be non-zero"; see the
         * ValidityConstraints class for details.  These contraints have the
         * important property that, although validity is not preserved under
         * addition, _invalidity_ is.
         *
         * For each of the resulting basis elements, this routine will call
         * \a action (which must be a function or some other callable object).
         * This action should return \c void, and must take exactly one
         * argument, which will be the basis element stored using type \a Ray.
         * The argument will be passed as an rvalue; a typical \a action
         * would take it as an rvalue reference (`Ray&&`) and move its
         * contents into some other more permanent storage.
         *
         * \warning For normal surface theory, the Contejean-Devie algorithm is
         * extremely slow, even when modified to incorporate admissibility
         * constraints.  Consider using the much faster HilbertPrimal or
         * HilbertDual instead.
         *
         * \python There are two versions of this function available
         * in Python.  The first version is the same as the C++ function;
         * here you must pass \a action, which may be a pure Python function.
         * The second form does not have an \a action argument; instead you
         * call `enumerate(subspace, constraints)`,
         * and it returns a Python list containing all Hilbert basis elements.
         * In both versions, the template argument \a Ray is fixed as VectorInt.
         *
         * \param action a function (or other callable object) that will be
         * called for each basis element.  This function must take a single
         * argument, which will be passed as an rvalue of type Ray.
         * \param subspace a matrix defining the linear subspace to intersect
         * with the given cone.  Each row of this matrix is the equation
         * for one of the hyperplanes whose intersection forms this linear
         * subspace.  The number of columns in this matrix must be the
         * dimension of the overall space in which we are working.
         * \param constraints a set of validity constraints as described above,
         * or ValidityConstraints::none if none should be imposed.
         */
        template <ArbitraryPrecisionIntegerVector Ray, typename Action>
        static void enumerate(Action&& action,
            const MatrixInt& subspace, const ValidityConstraints& constraints);

        // Mark this class as non-constructible.
        HilbertCD() = delete;

    private:
        /**
         * A helper class for Hilbert basis enumeration, describing a
         * single candidate basis vector.
         *
         * The coordinates of the vector are inherited through the
         * Vector superclass.
         *
         * The \a BitmaskType template argument is used to store one bit
         * per coordinate, which is \c false if the coordinate is zero
         * or \c true if the coordinate is non-zero.
         *
         * \tparam IntegerType the integer type used to store and manipulate
         * vectors; this must be one of Regina's own integer types.
         *
         * \tparam BitmaskType the bitmask type used to indicate zero/non-zero
         * coordinates; this must be one of Regina's own bitmask types, such as
         * Bitmask, Bitmask1 or Bitmask2.
         */
        template <typename IntegerType, typename BitmaskType>
        struct VecSpec : public Vector<IntegerType> {
            BitmaskType mask_;
                /**< A bitmask indicating which coordinates are zero
                     (\c false) and which are non-zero (\c true). */

            /**
             * Creates the zero vector.
             *
             * \param dim the total dimension of the space (and
             * therefore the toatl length of this vector).
             */
            inline VecSpec(size_t dim);

            /**
             * Creates a new clone of the given vector.
             */
            VecSpec(const VecSpec&) = default;

            /**
             * Sets this to be a clone of the given vector.
             */
            VecSpec& operator = (const VecSpec&) = default;
        };
        /**
         * Identical to the public routine enumerate(),
         * except that there is an extra template parameter \a BitmaskType.
         * This describes what type should be used for bitmasks that
         * assign flags to individual coordinate positions.
         *
         * All arguments to this function are identical to those for the
         * public routine enumerate().
         *
         * \pre The bitmask type is one of Regina's bitmask types, such
         * as Bitmask, Bitmask1 or Bitmask2.
         * \pre The type \a BitmaskType can handle at least \a n bits,
         * where \a n is the dimension of the Euclidean space (i.e., the
         * number of columns in \a subspace).
         */
        template <ArbitraryPrecisionIntegerVector Ray, typename BitmaskType,
            typename Action>
        static void enumerateUsingBitmask(Action&& action,
            const MatrixInt& subspace, const ValidityConstraints& constraints);
};

// Inline functions for HilbertCD::VecSpec

template <typename IntegerType, typename BitmaskType>
inline HilbertCD::VecSpec<IntegerType, BitmaskType>::VecSpec(size_t dim) :
        Vector<IntegerType>(dim), mask_(dim) {
    // All vector elements are initialised to zero thanks to the default
    // constructors in Regina's integer classes.
}

} // namespace regina

// Template definitions

#include "enumerate/hilbertcd-impl.h"

#endif

