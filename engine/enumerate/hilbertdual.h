
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

/*! \file enumerate/hilbertdual.h
 *  \brief Provides a modified dual algorithm for Hilbert basis enumeration.
 */

#ifndef __HILBERTDUAL_H
#ifndef __DOXYGEN
#define __HILBERTDUAL_H
#endif

#include "regina-core.h"
#include "maths/matrix.h"
#include "maths/ray.h"
#include <iterator>
#include <list>
#include <vector>

#ifndef __DOXYGEN
    // Optimisations:

    /**
     * Bruns and Ichim, J. Algebra 324 (2010), 1098-1113, remark 16(d).
     * This doesn't seem to help for fundamental normal surfaces (and in
     * fact seems to slow things down a small amount).
     */
    // #define __REGINA_HILBERT_DUAL_OPT_BI16D

    /**
     * When generating new vectors, only reduce against older vectors
     * that lie in the corresponding strict half-space, and do not perform
     * the additional (and unnecessary) reduction against older vectors that
     * lie directly on the hyperplane.
     * In practice this does speed things up, but only a little.
     */
    #define __REGINA_HILBERT_DUAL_OPT_NEWGEN_STRICT_ONLY

    /**
     * When reducing a potential basis, "darwinistically" reorder it so
     * that successful reducers are near the front.  See Bruns and Ichim,
     * J. Algebra 324 (2010), 1098-1113, remark 6(a).
     * In practice, for fundamental normal surfaces this does not seem
     * to help (and in fact it slows things down a little).
     */
    // #define __REGINA_HILBERT_DUAL_OPT_DARWIN
#endif

namespace regina {

class EnumConstraints;
class ProgressTracker;

/**
 * \weakgroup enumerate
 * @{
 */

/**
 * Implements a modified dual algorithm for enumerating Hilbert bases.
 * This is based on the dual algorithm as described in
 * "Normaliz: Algorithms for affine monoids and rational cones",
 * Winfried Bruns and Bogdan Ichim, J. Algebra 324 (2010), 1098-1113,
 * and has been modified to allow for additional constraints (such as
 * the quadrilateral constraints from normal surface theory).
 *
 * All routines of interest within this class are static; no object of
 * this class should ever be created.
 *
 * \ifacespython Not present.
 */
class HilbertDual {
    public:
        /**
         * Determines the Hilbert basis that generates all integer
         * points in the intersection of the <i>n</i>-dimensional
         * non-negative orthant with the given linear subspace.
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
         * Parameter \a constraints may contain a set of validity constraints,
         * in which case this routine will only return \e valid basis elements.
         * Each validity constraint is of the form "a basis element may only
         * lie outside at most one of these facets of the original
         * cone"; see the EnumConstraints class for details.  These
         * contraints have the important property that, although validity is
         * not preserved under addition, \e invalidity is.
         *
         * An optional progress tracker may be passed.  If so, this routine
         * will update the percentage progress and poll for cancellation
         * requests.  It will be assumed that an appropriate stage has already
         * been declared via ProgressTracker::newStage() before this routine
         * is called, and that ProgressTracker::setFinished() will be
         * called after this routine returns.
         *
         * \pre The template argument RayClass is derived from Ray (or
         * may possibly be Ray itself).
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
         * @param tracker a progress tracker through which progress
         * will be reported, or 0 if no progress reporting is required.
         * @param initialRows specifies how many initial rows of \a subspace
         * are to be processed in the precise order in which they appear.
         * The remaining rows will be sorted using the PosOrder class
         * before they are processed.
         */
        template <class RayClass, class OutputIterator>
        static void enumerateHilbertBasis(OutputIterator results,
            const MatrixInt& subspace, const EnumConstraints* constraints,
            ProgressTracker* tracker = 0, unsigned initialRows = 0);

    private:
        /**
         * A helper class for Hilbert basis enumeration, describing a
         * single vector (which is typically a basis element in some
         * partial solution space).
         *
         * The coordinates of the vector are inherited through the
         * superclass Ray.
         *
         * In addition, this class stores a data member \a nextHyp_,
         * which gives fast access to the dot product of this vector
         * with the hyperplane currently being processed.
         *
         * The \a BitmaskType template argument is used to store one bit
         * per coordinate, which is \c false if the coordinate is zero
         * or \c true if the coordinate is non-zero.
         *
         * \pre The template argument \a BitmaskType is one of Regina's
         * bitmask types, such as Bitmask, Bitmask1 or Bitmask2.
         */
        template <class BitmaskType>
        class VecSpec : private Ray {
            private:
                LargeInteger nextHyp_;
                    /**< The dot product of this vector with the
                         hyperplane currently being processed. */
                BitmaskType mask_;
                    /**< A bitmask indicating which coordinates are zero
                         (\c false) and which are non-zero (\c true). */
#ifdef __REGINA_HILBERT_DUAL_OPT_BI16D
                LargeInteger srcNextHyp_;
                    /**< Stores information from the summands used to
                         create this vector.  See srcNextHyp() for details. */
#endif

            public:
                /**
                 * Creates the zero vector.
                 *
                 * @param dim the total dimension of the space (and
                 * therefore the toatl length of this vector).
                 */
                inline VecSpec(size_t dim);

                /**
                 * Creates the given unit vector.
                 *
                 * The \a nextHyp_ data member will be left uninitialised.
                 *
                 * @param pos indicates which coordinate is set to one
                 * in this unit vector.
                 * @param dim the total dimension of the space (and
                 * therefore the total length of this vector).
                 */
                inline VecSpec(size_t pos, size_t dim);

                /**
                 * Creates a clone of the given vector.
                 *
                 * @param other the vector to clone.
                 */
                inline VecSpec(const VecSpec& other);

                /**
                 * Updates the \a nextHyp_ member to reflect the dot
                 * product with the given hyperplane.
                 *
                 * This routine also sets the member \a srcNextHyp_ to zero.
                 *
                 * @param subspace the matrix containing the full set of
                 * hyperplanes.
                 * @param row the row of the given matrix that stores
                 * the specific hyperplane in which we are interested.
                 */
                inline void initNextHyp(const MatrixInt& subspace,
                    unsigned row);

                /**
                 * Sets this to the sum of the two given vectors.
                 *
                 * \pre <tt>pos.nextHyp() &gt; 0</tt>, and
                 * <tt>neg.nextHyp() &lt; 0</tt>.
                 *
                 * @param pos the first vector to add, which must lie on
                 * the strictly positive side of the current hyperplane.
                 * @param neg the second vector to add, which must lie
                 * on the strictly negative side of the current hyperplane.
                 */
                inline void formSum(const VecSpec& pos, const VecSpec& neg);

                /**
                 * Returns the dot product of this vector with the
                 * hyperplane currently being processed.
                 */
                inline const LargeInteger& nextHyp() const;

                /**
                 * Returns the bitmask describing which coordinate are
                 * non-zero.
                 */
                inline const BitmaskType& mask() const;

                /**
                 * Returns the sign of the dot product of this vector
                 * with the hyperplane currently being processed.  This
                 * is simply the sign of the data member \a nextHyp_.
                 *
                 * @return 1, 0 or -1 according to the sign of \a nextHyp_.
                 */
                inline int sign() const;

#ifdef __REGINA_HILBERT_DUAL_OPT_BI16D
                /**
                 * Returns information from the summands used to
                 * create this vector.
                 *
                 * Specifically: Suppose this vector was created using
                 * formSum().  If nextHyp() &ge; 0, then this routine returns
                 * <tt>pos.nextHyp()</tt> where \a pos was the
                 * positive summand passed to formSum().
                 * If nextHyp() &lt; 0, then this routine returns
                 * <tt>neg.nextHyp()</tt> where \a neg was the
                 * negative summand passed to formSum().
                 *
                 * If this vector was not created using formSum(),
                 * or if initNextHyp() has since been called, then this
                 * routine returns zero.
                 *
                 * @return the summand information as described above.
                 */
                inline const LargeInteger& srcNextHyp() const;
#endif

                /**
                 * Determines if this and the given vector are identical.
                 *
                 * @param other the vector to compare with this.
                 * @return \c true if this vector is identical to the
                 * given vector, or \c false if not.
                 */
                inline bool operator == (const VecSpec& other) const;

                /**
                 * Determines if every element of this vector is less
                 * than or equal to every element of the given vector.
                 *
                 * @param other the vector to compare with this.
                 * @return \c true if every element of this vector is
                 * less than or equal to every element of \a other, or
                 * \c false otherwise.
                 */
                inline bool operator <= (const VecSpec& other) const;

                using Ray::operator [];
        };

        /**
         * Identical to the public routine enumerateHilbertBasis(),
         * except that there is an extra template parameter \a BitmaskType.
         * This describes what type should be used for bitmasks that
         * represent zero/non-zero coordinates in a vector.
         *
         * All argument are identical to those for the public routine
         * enumerateHilbertBasis().
         *
         * \pre The bitmask type is one of Regina's bitmask types, such
         * as Bitmask, Bitmask1 or Bitmask2.
         * \pre The type \a BitmaskType can handle at least \a n bits,
         * where \a n is the number of coordinates in the underlying vectors.
         */
        template <class RayClass, class BitmaskType, class OutputIterator>
        static void enumerateUsingBitmask(OutputIterator results,
            const MatrixInt& subspace, const EnumConstraints* constraints,
            ProgressTracker* tracker, unsigned initialRows);

        /**
         * Private constructor to ensure that objects of this class are
         * never created.
         */
        HilbertDual();

        /**
         * Test whether the vector \a vec can be reduced using
         * any of the candidate basis vectors in \a against.
         *
         * We say that \a vec reduces against a candidate basis vector
         * \a b if and only if:
         *
         * - the vector <tt>vec-b</tt> is non-negative;
         * - if \a listSign is 0, then <tt>vec-b</tt> lies on the
         *   hyperplane currently under investigation;
         * - if \a listSign is positive, then <tt>vec-b</tt> lies either
         *   on or to the positive side of the hyperplane under investigation;
         * - if \a listSign is negative, then <tt>vec-b</tt> lies either
         *   on or to the negative side of the hyperplane under investigation.
         *
         * This routine uses VecSpec::nextHyp() to determine the
         * relationships between vectors and the current hyperplane.
         *
         * @param vec the vector to test for reducibility.
         * @param against the list of candidate basis vectors to reduce
         * \a vec against.
         * @param listSign an integer indicating which sign of the
         * current hyperplane we are working on.
         * @return \c true if the given vector can be reduced, or \c false 
         * otherwise.
         */
        template <class BitmaskType>
        static bool reduces(const VecSpec<BitmaskType>& vec,
            const std::list<VecSpec<BitmaskType>*>& against,
            int listSign);

        /**
         * Removes all vectors from \reduce that can be reduced against
         * any of the candidate basis vectors in \a against.
         * This routine might also reorder the list \a against in the
         * hope of detecting future reductions more quickly.
         * For details of what reduction means, see reduces() above.
         *
         * This routine will work even if \a reduce and \a against are
         * the same list.
         *
         * @param reduce the list of vectors to test for reducibility.
         * @param against the list of candidate basis vectors to reduce against.
         * @param listSign an integer indicating which sign of the
         * current hyperplane we are working on.
         */
        template <class BitmaskType>
        static void reduceBasis(std::list<VecSpec<BitmaskType>*>& reduce,
            std::list<VecSpec<BitmaskType>*>& against,
            int listSign);

        /**
         * Updates a Hilbert basis by intersecting with a new hyperplane.
         *
         * The input vectors in \a list should contain the Hilbert basis
         * of a cone (defined as the intersection of the non-negative
         * orthant with some linear subspace).  This routine converts
         * \list into the Hilbert basis of this same cone intersected with
         * a new hyperplane.  The new hyperplane is defined by the given
         * row of the \a subspace matrix.
         *
         * This routine can optionally enforce a set of validity
         * constraints (such as the normal surface quadrilateral
         * constraints), as described in enumerateHilbertBasis().
         *
         * The set of validity constraints must be passed here as a
         * C-style array of bitmasks.  Each bitmask contains one bit per
         * coordinate position, as seen in the VecSpec inner class.
         * Each constraint is of the form "at most one of these
         * coordinates can be non-zero"; the bits for these coordinates must
         * be set to 1 in the corresponding bitmask, and all other bits must
         * be set to 0.
         *
         * @param list contains the original Hilbert basis on entry to
         * this function, and will contain the updated Hilbert basis upon
         * returning.
         * @param subspace a matrix of hyperplanes.
         * @param row indicates which row of \a subspace contains the
         * hyperplane that we will intersect with the cone defined by
         * the old Hilbert basis.
         * @param constraintsBegin the beginning of the C-style array of
         * validity constraints.  This should be 0 if no additional
         * constraints are to be imposed.
         * @param constraintsEnd a pointer just past the end of the
         * C-style array of validity constraints.  This should be 0
         * if no additional constraints are to be imposed.
         */
        template <class BitmaskType>
        static void intersectHyperplane(
            std::vector<VecSpec<BitmaskType>*>& list,
            const MatrixInt& subspace, unsigned row,
            const BitmaskType* constraintsBegin,
            const BitmaskType* constraintsEnd);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NHilbertDual has now been renamed to HilbertDual.
 */
REGINA_DEPRECATED typedef HilbertDual NHilbertDual;

/*@}*/

// Inline functions for HilbertDual

inline HilbertDual::HilbertDual() {
}

// Inline functions for HilbertDual::VecSpec

template <class BitmaskType>
inline HilbertDual::VecSpec<BitmaskType>::VecSpec(size_t dim) :
        Ray(dim), mask_(dim) {
    // All vector elements, nextHyp_ and srcNextHyp_ are initialised to
    // zero thanks to the LargeInteger default constructor.
}

template <class BitmaskType>
inline HilbertDual::VecSpec<BitmaskType>::VecSpec(size_t pos, size_t dim) :
        Ray(dim), mask_(dim) {
    // All coordinates are initialised to zero by default thanks to
    // the LargeInteger constructor.
    setElement(pos, LargeInteger::one);
    mask_.set(pos, true);
}

template <class BitmaskType>
inline HilbertDual::VecSpec<BitmaskType>::VecSpec(
        const HilbertDual::VecSpec<BitmaskType>& other) :
        Ray(other),
        nextHyp_(other.nextHyp_),
#ifdef __REGINA_HILBERT_DUAL_OPT_BI16D
        srcNextHyp_(other.srcNextHyp_),
#endif
        mask_(other.mask_) {
}

template <class BitmaskType>
inline void HilbertDual::VecSpec<BitmaskType>::initNextHyp(
        const MatrixInt& subspace, unsigned row) {
    nextHyp_ = LargeInteger::zero;

    LargeInteger tmp;
    for (int i = 0; i < subspace.columns(); ++i)
        if (subspace.entry(row, i) != 0 && (*this)[i] != 0) {
            tmp = subspace.entry(row, i);
            tmp *= (*this)[i];
            nextHyp_ += tmp;
        }

#ifdef __REGINA_HILBERT_DUAL_OPT_BI16D
    srcNextHyp_ = 0;
#endif
}

template <class BitmaskType>
inline void HilbertDual::VecSpec<BitmaskType>::formSum(
        const HilbertDual::VecSpec<BitmaskType>& pos,
        const HilbertDual::VecSpec<BitmaskType>& neg) {
    (*this) = pos; // The default assignment operator.

    (*this) += neg;
    nextHyp_ += neg.nextHyp_;
    mask_ |= neg.mask_;

#ifdef __REGINA_HILBERT_DUAL_OPT_BI16D
    if (nextHyp_ >= 0)
        srcNextHyp_ = pos.nextHyp_;
    else
        srcNextHyp_ = neg.nextHyp_;
#endif
}

template <class BitmaskType>
inline const LargeInteger& HilbertDual::VecSpec<BitmaskType>::nextHyp()
        const {
    return nextHyp_;
}

template <class BitmaskType>
inline const BitmaskType& HilbertDual::VecSpec<BitmaskType>::mask() const {
    return mask_;
}

template <class BitmaskType>
inline int HilbertDual::VecSpec<BitmaskType>::sign() const {
    return (nextHyp_ == 0 ? 0 : nextHyp_ > 0 ? 1 : -1);
}

#ifdef __REGINA_HILBERT_DUAL_OPT_BI16D
template <class BitmaskType>
inline const LargeInteger& HilbertDual::VecSpec<BitmaskType>::srcNextHyp()
        const {
    return srcNextHyp_;
}
#endif

template <class BitmaskType>
inline bool HilbertDual::VecSpec<BitmaskType>::operator == (
        const HilbertDual::VecSpec<BitmaskType>& other) const {
    // Begin with simple tests that give us a fast way of saying no.
    if (! (mask_ == other.mask_))
        return false;
    return (static_cast<const Ray&>(*this) == static_cast<const Ray&>(other));
}

template <class BitmaskType>
inline bool HilbertDual::VecSpec<BitmaskType>::operator <= (
        const HilbertDual::VecSpec<BitmaskType>& other) const {
    // Begin with simple tests that give us a fast way of saying no.
    if (! (mask_ <= other.mask_))
        return false;
    for (unsigned i = 0; i < size(); ++i)
        if ((*this)[i] > other[i])
            return false;
    return true;
}

} // namespace regina

// Template definitions

#include "enumerate/hilbertdual-impl.h"

#endif

