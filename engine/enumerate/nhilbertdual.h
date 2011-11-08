
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

/*! \file enumerate/nhilbertdual.h
 *  \brief Provides a modified dual algorithm for Hilbert basis enumeration.
 */

#ifndef __NHILBERTDUAL_H
#ifndef __DOXYGEN
#define __NHILBERTDUAL_H
#endif

#include "maths/nmatrixint.h"
#include "maths/nray.h"
#include <iterator>
#include <list>
#include <vector>

namespace regina {

class NEnumConstraintList;
class NMatrixInt;
class NProgressNumber;

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
class NHilbertDual {
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
         * cone"; see the NEnumConstraintList class for details.  These
         * contraints have the important property that, although validity is
         * not preserved under addition, \e invalidity is.
         *
         * A numeric progress watcher may be passed for progress reporting.
         * If so, this routine will poll for cancellation requests accordingly.
         *
         * \pre The template argument RayClass is derived from NRay (or
         * may possibly be NRay itself).
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
         * @param progress a numeric progress watcher through which progress
         * will be reported, or 0 if no progress reporting is required.  If
         * a progress watcher is passed, its expected total will be
         * increased immediately by some number of steps and the completed
         * total will be increased gradually by this same number.  Note that
         * NProgress::setFinished() will \e not be called, since
         * whoever called this routine may need to do further processing.
         * @param initialRows specifies how many initial rows of \a subspace
         * are to be processed in the precise order in which they appear.
         * The remaining rows will be sorted using the NPosOrder class
         * before they are processed.
         */
        template <class RayClass, class OutputIterator>
        static void enumerateHilbertBasis(OutputIterator results,
            const NMatrixInt& subspace, const NEnumConstraintList* constraints,
            NProgressNumber* progress = 0, unsigned initialRows = 0);

    private:
        /**
         * A helper class for Hilbert basis enumeration, describing a
         * single vector (which is typically a basis element in some
         * partial solution space).
         *
         * The coordinates of the vector are inherited through the
         * superclass NRay.
         *
         * In addition, this class stores a data member \a nextHyp_,
         * which gives fast access to the dot product of this vector
         * with the hyperplane currently being processed.
         */
        class VecSpec : private NRay {
            private:
                NLargeInteger nextHyp_;
                    /**< The dot product of this vector with the
                         hyperplane currently being processed. */

            public:
                /**
                 * Creates the zero vector.
                 *
                 * @param dim the total dimension of the space (and
                 * therefore the toatl length of this vector).
                 */
                inline VecSpec(unsigned dim);

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
                inline VecSpec(unsigned pos, unsigned dim);

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
                 * @param subspace the matrix containing the full set of
                 * hyperplanes.
                 * @param row the row of the given matrix that stores
                 * the specific hyperplane in which we are interested.
                 */
                inline void initNextHyp(const NMatrixInt& subspace,
                    unsigned row);

                /**
                 * Sets this to the sum of the two given vectors.
                 *
                 * @param x the first vector to add.
                 * @param y the second vector to add.
                 */
                inline void formSum(const VecSpec& x, const VecSpec& y);

                /**
                 * Returns the dot product of this vector with the
                 * hyperplane currently being processed.
                 */
                inline const NLargeInteger& nextHyp() const;

                /**
                 * Returns the sign of the dot product of this vector
                 * with the hyperplane currently being processed.  This
                 * is simply the sign of the data member \a nextHyp_.
                 *
                 * @return 1, 0 or -1 according to the sign of \a nextHyp_.
                 */
                inline int sign() const;

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

                using NRay::operator [];
        };

        /**
         * A linked list of VecSpec items, with fast insertion and deletion.
         */
        typedef std::list<VecSpec*> VecSpecList;

        /**
         * A vector of VecSpec items, with fast random access and
         * better caching.
         */
        typedef std::vector<VecSpec*> VecSpecVector;

        /**
         * Private constructor to ensure that objects of this class are
         * never created.
         */
        NHilbertDual();

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
         * @param ignore a single candidate basis vector from \a against
         * that should be ignored (for instance, to stop us from
         * reducing \a vec against itself), or against.end() if every
         * candidate basis vector in \a against should be considered.
         */
        static bool reduces(const VecSpec& vec, const VecSpecList& against,
            int listSign, VecSpecList::const_iterator ignore);

        /**
         * Removes all vectors from \reduce that can be reduced against
         * any of the candidate basis vectors in \a against.  For
         * details of what reduction means, see reduces() above.
         *
         * This routine will work even if \a reduce and \a against are
         * the same list.
         *
         * @param reduce the list of vectors to test for reducibility.
         * @param against the list of candidate basis vectors to reduce against.
         * @param listSign an integer indicating which sign of the
         * current hyperplane we are working on.
         */
        static void reduceBasis(VecSpecList& reduce, VecSpecList& against,
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
         * @param list contains the original Hilbert basis on entry to
         * this function, and will contain the updated Hilbert basis upon
         * returning.
         * @param subspace a matrix of hyperplanes.
         * @param row indicates which row of \a subspace contains the
         * hyperplane that we will intersect with the cone defined by
         * the old Hilbert basis.
         * @param constraints a set of validity constraints to impose
         * as described in enumerateHilbertBasis(), or 0 if no additional
         * constraints should be imposed.
         */
        static void intersectHyperplane(VecSpecVector& list,
            const NMatrixInt& subspace, unsigned row,
            const NEnumConstraintList* constraints);
};

/*@}*/

// Inline functions for NHilbertDual

inline NHilbertDual::NHilbertDual() {
}

// Inline functions for NHilbertDual::VecSpec

inline NHilbertDual::VecSpec::VecSpec(unsigned dim) : NRay(dim) {
    // All vector elements and nextHyp_ are initialised to zero
    // thanks to the NLargeInteger default constructor.
}

inline NHilbertDual::VecSpec::VecSpec(unsigned pos, unsigned dim) : NRay(dim) {
    // All coordinates are initialised to zero by default thanks to
    // the NLargeInteger constructor.
    setElement(pos, NLargeInteger::one);
}

inline NHilbertDual::VecSpec::VecSpec(const NHilbertDual::VecSpec& other) :
        NRay(other), nextHyp_(other.nextHyp_) {
}

inline void NHilbertDual::VecSpec::initNextHyp(const NMatrixInt& subspace,
        unsigned row) {
    nextHyp_ = NLargeInteger::zero;

    NLargeInteger tmp;
    for (int i = 0; i < subspace.columns(); ++i)
        if (subspace.entry(row, i) != 0 && (*this)[i] != 0) {
            tmp = subspace.entry(row, i);
            tmp *= (*this)[i];
            nextHyp_ += tmp;
        }
}

inline void NHilbertDual::VecSpec::formSum(
        const NHilbertDual::VecSpec& x, const NHilbertDual::VecSpec& y) {
    (*this) = x; // The default assignment operator.

    (*this) += y;
    nextHyp_ += y.nextHyp_;
}

inline const NLargeInteger& NHilbertDual::VecSpec::nextHyp() const {
    return nextHyp_;
}

inline int NHilbertDual::VecSpec::sign() const {
    return (nextHyp_ == 0 ? 0 : nextHyp_ > 0 ? 1 : -1);
}

inline bool NHilbertDual::VecSpec::operator <= (
        const NHilbertDual::VecSpec& other) const {
    for (unsigned i = 0; i < size(); ++i)
        if ((*this)[i] > other[i])
            return false;
    return true;
}

} // namespace regina

// Template definitions

#include "enumerate/nhilbertdual.tcc"

#endif

