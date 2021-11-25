
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file enumerate/doubledescription.h
 *  \brief Provides a modified double description method for polytope
 *  vertex enumeration.
 */

#ifndef __REGINA_DOUBLEDESCRIPTION_H
#ifndef __DOXYGEN
#define __REGINA_DOUBLEDESCRIPTION_H
#endif

#include "regina-core.h"
#include "enumerate/ordering.h"
#include "maths/matrix.h"
#include "maths/vector.h"
#include <iterator>
#include <vector>

namespace regina {

class ProgressTracker;
class ValidityConstraints;

/**
 * \defgroup enumerate Vertex Enumeration
 * Polytope vertex enumeration algorithms.
 */

/**
 * Implements a modified double description method for polytope vertex
 * enumeration.  For details of the underlying algorithm, see
 * "Optimizing the double description method for normal surface enumeration",
 * Benjamin A. Burton, Math. Comp. 79 (2010), 453-484.
 *
 * All routines of interest within this class are static; no object of
 * this class should ever be created.
 *
 * \ingroup enumerate
 */
class DoubleDescription {
    public:
        /**
         * Determines the extremal rays of the intersection of the
         * <i>n</i>-dimensional non-negative orthant with the given linear
         * subspace.  The resulting rays will be of the class \a RayClass,
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
         * The purpose of this routine is to compute the extremal rays of
         * the new cone formed by intersecting the original cone with this
         * linear subspace.  The resulting list of extremal rays will
         * contain no duplicates or redundancies.
         *
         * Parameter \a constraints may contain a set of validity constraints,
         * in which case this routine will only return \e valid extremal
         * rays.  Each validity constraint is of the form "an extremal ray
         * may only lie outside at most one of these facets of the original
         * cone"; see the ValidityConstraints class for details.  These
         * contraints have the important property that, although validity is
         * not preserved under convex combination, \e invalidity is.
         *
         * An optional progress tracker may be passed.  If so, this routine
         * will update the percentage progress and poll for cancellation
         * requests.  It will be assumed that an appropriate stage has already
         * been declared via ProgressTracker::newStage() before this routine
         * is called, and that ProgressTracker::setFinished() will be
         * called after this routine returns.
         *
         * For each of the resulting extremal rays, this routine will call
         * \a action (which must be a function or some other callable object).
         * This action should return \c void, and must take exactly one
         * argument, which will be the extremal ray stored using \a RayClass.
         * The argument will be passed as an rvalue; a typical \a action
         * would take it as an rvalue reference (RayClass&&) and move its
         * contents into some other more permanent storage.
         *
         * \pre The template argument RayClass is derived from (or equal to)
         * Vector<T>, where \a T is one of Regina's arbitrary-precision
         * integer classes (Integer or LargeInteger).
         *
         * \ifacespython There are two versions of this function available
         * in Python.  The first version is the same as the C++ function;
         * here you must pass \a action, which may be a pure Python function.
         * The second form does not have an \a action argument; instead you
         * call <tt>enumerate(subspace, constraints, tracker, initialRows)</tt>,
         * and it returns a Python list containing all extremal rays.
         * In both versions, the argument \a RayClass is fixed as VectorInt.
         *
         * @param action a function (or other callable object) that will be
         * called for each extremal ray.  This function must take a single
         * argument, which will be passed as an rvalue of type RayClass.
         * @param subspace a matrix defining the linear subspace to intersect
         * with the given cone.  Each row of this matrix is the equation
         * for one of the hyperplanes whose intersection forms this linear
         * subspace.  The number of columns in this matrix must be the
         * dimension of the overall space in which we are working.
         * @param constraints a set of validity constraints as described above,
         * or ValidityConstraints::none if none should be imposed.
         * @param tracker a progress tracker through which progress
         * will be reported, or \c null if no progress reporting is required.
         * @param initialRows specifies how many initial rows of \a subspace
         * are to be processed in the precise order in which they appear.
         * The remaining rows will be sorted using the PosOrder class
         * before they are processed.
         */
        template <class RayClass, typename Action>
        static void enumerate(Action&& action,
            const MatrixInt& subspace, const ValidityConstraints& constraints,
            ProgressTracker* tracker = nullptr, unsigned long initialRows = 0);

        // Mark this class as non-constructible.
        DoubleDescription() = delete;

    private:
        /**
         * A helper class for vertex enumeration, describing a single ray
         * (typically a vertex in some partial solution space).
         *
         * Although this class represents a ray, it does not actually
         * store the coordinates of the ray.  Instead it stores:
         *
         * - the dot products of this ray with each of the hyperplanes
         *   passed to DoubleDescription::enumerate();
         *
         * - a bitmask indicating which facets of the original cone this
         *   ray belongs to.
         *
         * The dot products are stored as coordinates of the Vector superclass.
         * Dot products are only stored for hyperplanes that have not yet been
         * intersected (thus the vector length becomes smaller as the
         * main algorithm progresses).
         * Dot products are stored in the order in which hyperplanes are
         * to be processed (thus the dot product with the next hyperplane
         * is the first element of this vector, and the dot product with
         * the final hyperplane is the last element).
         *
         * The \a BitmaskType template argument describes how the set of
         * facets will be stored.  Specifically, the set of facets is
         * stored as a bitmask with one bit per facet; each bit is set if
         * and only if this ray belongs to the corresponding original facet.
         *
         * Since this class is used heavily, faster bitmask types such
         * as Bitmask1 and Bitmask2 are preferred; however, if the
         * number of facets is too large then the slower general-use Bitmask
         * class will need to be used instead.
         *
         * \tparam IntegerType the integer type used to store and manipulate
         * rays; this must be one of Regina's own integer types.
         *
         * \tparam BitmaskType the bitmask type used to store a set of facets;
         * this must be one of Regina's own bitmask types, such as
         * Bitmask, Bitmask1 or Bitmask2.
         */
        template <class IntegerType, class BitmaskType>
        class RaySpec : private Vector<IntegerType> {
            private:
                using Vector<IntegerType>::elements;

                BitmaskType facets_;
                    /**< A bitmask listing which original facets this ray
                         belongs to. */

            public:
                /**
                 * Creates a ray specification for the non-negative
                 * portion of the given coordinate axis.
                 *
                 * \pre The dimension of the space in which we are
                 * working is strictly positive, but is small enough that
                 * \a BitmaskType can hold the corresponding number of bits
                 * (one bit per dimension).
                 *
                 * @param axis indicates which coordinate axis to use;
                 * this must be at least zero but strictly less than the
                 * dimension of the entire space.
                 * @param subspace the matrix containing the set of
                 * hyperplanes to intersect with the original cone
                 * (one hyperplane for each row of the matrix).
                 * @param hypOrder the order in which we plan to
                 * intersect the hyperplanes.  The length of this array
                 * must be the number of rows in \a subspace, and the
                 * <i>i</i>th hyperplane to intersect must be described
                 * by row <tt>hypOrder[i]</tt> of \a subspace.
                 */
                inline RaySpec(unsigned long axis, const MatrixInt& subspace,
                    const long* hypOrder);

                /**
                 * Creates a copy of the given ray specification, with the
                 * first dot product removed.  The resulting list of dot
                 * products is thus one element shorter.
                 *
                 * This routine is typically called when we intersect a
                 * hyperplane with a partial solution space and some ray
                 * lies directly in this hyperplane (not on either side).
                 *
                 * @param trunc the ray to copy and truncate.
                 */
                inline RaySpec(const RaySpec& trunc);

                /**
                 * Creates a new ray, describing where the plane between
                 * two given rays meets the next intersecting hyperplane.
                 *
                 * The list of dot products for the new ray will be one
                 * element shorter than the lists for the given rays
                 * (since there will be one less hyperplane remaining to
                 * intersect).
                 *
                 * \pre The two given rays come from the same partial
                 * solution space (i.e., their lists of dot products
                 * are the same length).  Moreover, this partial
                 * solution space still has at least one hyperplane
                 * remaining to intersect (i.e., the lists of dot
                 * products are not empty).
                 * \pre The two given rays lie on opposite sides of the
                 * next hyperplane to intersect, and neither ray actually
                 * lies \e in this next hyperplane.
                 *
                 * @param first the first of the given rays.
                 * @param second the second of the given rays.
                 */
                RaySpec(const RaySpec& first, const RaySpec& second);

                /**
                 * Returns the sign of the dot product of this ray with the
                 * next hyperplane.  This is simply the sign of the first
                 * element in the list of remaining dot products.
                 *
                 * @return 1, 0 or -1 according to the sign of the next
                 * dot product.
                 */
                int sign() const;

                /**
                 * Returns the bitmask listing which facets of the original
                 * cone this ray belongs to.  Each bit is set to \c true
                 * if and only if this ray belongs to the corresponding facet.
                 *
                 * @return a bitmask of facets.
                 */
                inline const BitmaskType& facets() const;

                /**
                 * Determines whether this ray belongs to all of the
                 * facets that are common to both given rays.
                 *
                 * For this routine to return \c true, every facet that
                 * contains both \a x and \a y must contain this ray also.
                 *
                 * @param x the first of the given rays.
                 * @param y the second of the given rays.
                 * @return \c true if and only if this ray belongs to all
                 * of the facets that \e both \a x and \a y belong to.
                 */
                inline bool onAllCommonFacets(
                    const RaySpec& x, const RaySpec& y) const;

                /**
                 * Recovers the coordinates of the actual ray that is
                 * described by this object.  This routine is not fast,
                 * since it needs to solve a system of linear equations.
                 *
                 * \pre This ray is a member of the \e final solution
                 * space.  That is, all hyperplanes have been
                 * intersected with the original cone, and the list of
                 * dot products stored in this object is empty.
                 *
                 * @param dest the ray in which the final coordinates
                 * will be stored; the length of this ray must be the
                 * dimension of the overall space in which we are working.
                 * @param subspace the matrix containing the set of
                 * hyperplanes that were intersected with the original cone
                 * (one hyperplane for each row of the matrix).
                 */
                template <typename RayClass>
                void recover(RayClass& dest, const MatrixInt& subspace) const;
        };

        /**
         * Identical to the public routine enumerate(), except
         * that there is an extra template parameter \a BitmaskType.
         * This specifies what type should be used for the bitmask
         * describing which original facets a ray belongs to.
         *
         * All arguments to this function are identical to those for the
         * public routine enumerate().
         *
         * \pre The bitmask type is one of Regina's bitmask types, such
         * as Bitmask, Bitmask1 or Bitmask2.
         * \pre The type \a BitmaskType can handle at least \a f bits,
         * where \a f is the number of original facets in the given range.
         * \pre The given range of facets is not empty.
         */
        template <class RayClass, class BitmaskType, typename Action>
        static void enumerateUsingBitmask(Action&& action,
            const MatrixInt& subspace, const ValidityConstraints& constraints,
            ProgressTracker* tracker, unsigned long initialRows);

        /**
         * A part of the full double description algorithm that
         * intersects the current solution set with a new hyperplane.
         *
         * The input list \a src must contain the vertices of the
         * solution space after the first \a prevHyperplanes hyperplanes
         * have been intersected with the original cone.  This routine
         * intersects this solution space with the next hyperplane, and
         * places the vertices of the new solution space in the output
         * list \a dest.
         *
         * The set of validity constraints must be passed here as a container
         * of bitmasks, as returned by ValidityConstraints::bitmasks().
         * Each bitmask will be treated as a bitmask of facets, as seen in the
         * RaySpec inner class.  Each constraint will be interpreted as
         * "a point cannot live outside more than one of these facets";
         * the bits for these facets must be set to 1 in the corresponding
         * bitmask, and all other bits must be set to 0.
         *
         * It should be noted that the hyperplane itself is not passed
         * to this routine.  This is because all the necessary information
         * (in particular, the dot products with the new hyperplane) is
         * stored with the individual vertices of the current solution space.
         *
         * \pre The input list \a src owns its elements, and the output
         * list \a dest is empty.
         * \post The input list \a src will be empty, and the output
         * list \a dest will own all of the elements that it contains.
         *
         * @param src contains the vertices of the current solution space
         * before this routine is called.
         * @param dest contains the vertices of the new solution space
         * after this routine returns.
         * @param dim the dimension of the entire space in which we are working.
         * @param prevHyperplanes the number of hyperplanes that have
         * already been intersected with the original cone to form the
         * current solution set.  This does not include the hyperplane
         * currently under consideration.
         * @param constraintsMask the list of additional validity constraints
         * to impose.
         * @param tracker an optional progress tracker that will be polled
         * for cancellation (though no incremental progress will be reported
         * within this routine).  This may be null.
         * @return \c true if vertices of the old solution set were found
         * on both sides of the new hyperplane, \c false if one of
         * the closed half-spaces defined by the new hyperplane contained
         * the entire old solution set, or undefined if the operation
         * was cancelled via the progress tracker.
         */
        template <class IntegerType, class BitmaskType>
        static bool intersectHyperplane(
            std::vector<RaySpec<IntegerType, BitmaskType>*>& src,
            std::vector<RaySpec<IntegerType, BitmaskType>*>& dest,
            unsigned long dim, unsigned long prevHyperplanes,
            const std::vector<BitmaskType>& constraintMasks,
            ProgressTracker* tracker);
};

// Inline functions for DoubleDescription::RaySpec

template <class IntegerType, class BitmaskType>
inline DoubleDescription::RaySpec<IntegerType, BitmaskType>::RaySpec(
        const RaySpec<IntegerType, BitmaskType>& trunc) :
        Vector<IntegerType>(trunc.size() - 1), facets_(trunc.facets_) {
    std::copy(trunc.elements + 1, trunc.end, elements);
}

template <class IntegerType, class BitmaskType>
inline int DoubleDescription::RaySpec<IntegerType, BitmaskType>::sign() const {
    if (*elements < 0)
        return -1;
    if (*elements > 0)
        return 1;
    return 0;
}

template <class IntegerType, class BitmaskType>
inline const BitmaskType&
        DoubleDescription::RaySpec<IntegerType, BitmaskType>::facets() const {
    return facets_;
}

template <class IntegerType, class BitmaskType>
inline bool
        DoubleDescription::RaySpec<IntegerType, BitmaskType>::onAllCommonFacets(
        const RaySpec<IntegerType, BitmaskType>& x,
        const RaySpec<IntegerType, BitmaskType>& y) const {
    return facets_.containsIntn(x.facets_, y.facets_);
}

} // namespace regina

// Template definitions

#include "enumerate/doubledescription-impl.h"

#endif

