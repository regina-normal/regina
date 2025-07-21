
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

/*! \file enumerate/hilbertprimal.h
 *  \brief Provides a modified primal algorithm for Hilbert basis enumeration.
 */

#ifndef __REGINA_HILBERTPRIMAL_H
#ifndef __DOXYGEN
#define __REGINA_HILBERTPRIMAL_H
#endif

#include "regina-core.h"
#include "maths/matrix.h"
#include <gmpxx.h>
#include <iterator>
#include <vector>

namespace regina {

class ProgressTracker;
class ValidityConstraints;

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
 * \ingroup enumerate
 */
class HilbertPrimal {
    public:
        /**
         * Determines the Hilbert basis that generates all integer
         * points in the intersection of the <i>n</i>-dimensional
         * non-negative orthant with some linear subspace.
         * The resulting basis elements will be of the class \a RayClass,
         * and will be passed into the given action function one at a time.
         *
         * The intersection of the non-negative orthant with this linear
         * subspace is a pointed polyhedral cone with apex at the origin,
         * and this routine requires the extremal rays of this cone to
         * be provided as input.  The extremal rays can be computed
         * using DoubleDescription::enumerate() in general cases,
         * though sometimes (such as with normal surface theory) more
         * efficient methods are available.
         *
         * This routine computes the Hilbert basis of all integer points in
         * this cone.  The resulting list of basis vectors will contain
         * no duplicates or redundancies.
         *
         * The parameter \a constraints may contain a set of validity
         * constraints, in which case this routine will only return _valid_
         * basis elements.  Each validity constraint is of the form "at
         * most one of these coordinates may be non-zero"; see the
         * ValidityConstraints class for details.  These contraints have the
         * important property that, although validity is not preserved under
         * addition, _invalidity_ is.
         *
         * An optional progress tracker may be passed.  If so, this routine
         * will update the percentage progress and poll for cancellation
         * requests.  It will be assumed that an appropriate stage has already
         * been declared via ProgressTracker::newStage() before this routine
         * is called, and that ProgressTracker::setFinished() will be
         * called after this routine returns.
         *
         * For each of the resulting basis elements, this routine will call
         * \a action (which must be a function or some other callable object).
         * This action should return \c void, and must take exactly one
         * argument, which will be the basis element stored using \a RayClass.
         * The argument will be passed as an rvalue; a typical \a action
         * would take it as an rvalue reference (RayClass&&) and move its
         * contents into some other more permanent storage.
         *
         * \pre If \a constraints is passed, then the given list of
         * extremal rays contains _only_ those extremal rays that satisfy
         * all of the given constraints.
         * \pre The template argument RayClass is derived from (or equal to)
         * Vector<T>, where \a T is one of Regina's arbitrary-precision
         * integer classes (Integer or LargeInteger).
         * \pre The template argument RayIterator is a forward iterator type,
         * and when dereferenced has the interface of a constant vector of
         * Regina's arbitrary-precision integers (i.e., the elements of the
         * vector must be of type Integer or LargeInteger).  For example, it
         * might dereference to a (const Vector<Integer>&), or a
         * (const std::vector<LargeInteger>&).
         * The vector interface required is minimal (just a bracket operator
         * and a size() function), and in particular both std::vector and
         * regina::Vector will suffice for this.
         *
         * \warning If a progress tracker is passed, be aware that the
         * present implementation updates percentage progress very infrequently,
         * and may take a very long time to honour cancellation requests.
         *
         * \exception UnsolvedCase Normaliz was unable to compute the
         * Hilbert basis for one or more maximal admissible faces.
         *
         * \python There are two versions of this function available in Python:
         * - `enumerate(action, rays, constraints, tracker)`, which
         *   mirrors the C++ function; here \a action may be a pure Python
         *   function.
         * - `enumerate(rays, constraints, tracker)`, where the \a action
         *   argument is missing, and instead the function returns a
         *   Python list containing all Hilbert basis elements.
         * \par
         * In both versions, the extremal rays must be passed as either:
         * - a Python list of VectorInt objects, which means the output type
         *   \a RayClass will be VectorInt; or
         * - a NormalSurfaces or NormalHypersurfaces object, which meams the
         *   output type \a RayClass will be VectorLarge, and the input list
         *   of extremal rays will be the range from `rays.beginVectors()` to
         *   `rays.endVectors()`.
         * \par
         * The global interpreter lock will be released while this
         * function runs, so you can use it with Python-based multithreading.
         *
         * \param action a function (or other callable object) that will be
         * called for each basis element.  This function must take a single
         * argument, which will be passed as an rvalue of type RayClass.
         * \param raysBegin an iterator pointing to the beginning of the
         * list of extremal rays.
         * \param raysEnd an iterator pointing past the end of the
         * list of extremal rays.
         * \param constraints a set of validity constraints as described above,
         * or ValidityConstraints::none if none should be imposed.
         * \param tracker a progress tracker through which progress
         * will be reported, or \c null if no progress reporting is required.
         */
        template <class RayClass, class RayIterator, typename Action>
        static void enumerate(Action&& action,
            const RayIterator& raysBegin, const RayIterator& raysEnd,
            const ValidityConstraints& constraints,
            ProgressTracker* tracker = nullptr);

        // Mark this class as non-constructible.
        HilbertPrimal() = delete;

    private:
        /**
         * Identical to the public routine enumerate(),
         * except that there is an extra template parameter \a BitmaskType.
         * This describes what type should be used for bitmasks that
         * represent maximal admissible faces.
         *
         * All arguments to this function are identical to those for the
         * public routine enumerate().
         *
         * \pre The bitmask type is one of Regina's bitmask types, such
         * as Bitmask, Bitmask1 or Bitmask2.
         * \pre The type \a BitmaskType can handle at least \a n bits,
         * where \a n is the dimension of the Euclidean space (i.e., the
         * number of columns in \a subspace).
         *
         * \exception UnsolvedCase Normaliz was unable to compute the
         * Hilbert basis for one or more maximal admissible faces.
         */
        template <class RayClass, class BitmaskType,
            class RayIterator, typename Action>
        static void enumerateUsingBitmask(Action&& action,
            const RayIterator& raysBegin, const RayIterator& raysEnd,
            const ValidityConstraints& constraints, ProgressTracker* tracker);

        /**
         * Uses Normaliz to enumerate the Hilbert basis on a single
         * maximal admissible face.
         *
         * The purpose of this routine is simply to quarantine the code that
         * relies on Normaliz data structures and functions.  This is
         * because Regina does not ship the Normaliz headers for end users;
         * instead it treats Normaliz as internal to the calculation engine.
         *
         * This function is non-templated, and so can be compiled directly into
         * Regina's engine (unlike most of the other functions in this class,
         * which are templated and therefore implemented in the headers).
         *
         * \exception UnsolvedCase Normaliz was unable to compute the
         * requested Hilbert basis.
         *
         * \param input the extreme rays of a single maximal admissible face,
         * presented in a form that Normaliz can understand (i.e., as vectors
         * of GMP integers).
         * \return the Hilbert basis for the given maximal admissible face.
         */
        static std::vector<std::vector<mpz_class>> normaliz(
            const std::vector<std::vector<mpz_class>>& input);

        /**
         * Determines whether the given ray lies in the face specified
         * by the given bitmask.
         *
         * Faces are described using bitmasks in the same manner as described
         * by MaxAdmissible::enumerate(), where \c true represents a
         * coordinate that is non-zero in the relative interior of the
         * face, and \c false represents a coordinate that is always
         * zero throughout the face.
         *
         * \param ray the ray to test, given as a vector of integers.
         * \param face the face to test, given as a bitmask.
         * \return \c true if the given ray lies within the given face,
         * or \c false otherwise.
         */
        template <class VectorClass, class BitmaskType>
        static bool inFace(const VectorClass& ray, const BitmaskType& face);
};

} // namespace regina

// Template definitions

#include "enumerate/hilbertprimal-impl.h"

#endif

