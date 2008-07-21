
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file ndoubledescriptor.h
 *  \brief Provides a modified double descriptor method for polytope
 *  vertex enumeration.
 */

#ifndef __NDOUBLEDESCRIPTOR_H
#ifndef __DOXYGEN
#define __NDOUBLEDESCRIPTOR_H
#endif

#include "maths/nfastvector.h"
#include "maths/nray.h"
#include "maths/nvectormatrix.h"
#include <iterator>
#include <vector>

namespace regina {

class NEnumConstraintList;
class NMatrixInt;
class NProgressNumber;

/**
 * \addtogroup enumerate Vertex Enumeration
 * Polytope vertex enumeration algorithms.
 * @{
 */

/**
 * Implements a modified double descriptor method for polytope vertex
 * enumeration.
 *
 * All routines of interest within this class are static; no object of
 * this class should ever be created.
 *
 * \ifacespython Not present.
 */
class NDoubleDescriptor {
    public:
        /**
         * Determines the extremal rays of the intersection of the given cone
         * with the given linear subspace.  The resulting rays will be newly
         * allocated and written to the given output iterator.  Their
         * deallocation is the responsibility of whoever called this routine.
         *
         * The given cone must be of full dimension and must have its vertex
         * at the origin.  The cone is represented by:
         *
         * - a list of its extremal rays (all described as vectors
         *   through the origin);
         *
         * - a list of the facets of this cone (all described as hyperplanes
         *   through the origin, where the cone sits on the non-negative
         *   side).
         *
         * The extremal rays must indeed be extremal (i.e., one cannot
         * be a convex combination of the others).  The facet list must
         * not contain any redundant hyperplanes (i.e., the given hyperplanes
         * must be in one-to-one correspondence with the actual facets of the
         * cone).
         *
         * This routine also takes a linear subspace, defined by the
         * intersection of a set of hyperplanes through the origin (this
         * subspace is described as a matrix, with each row giving the
         * equation for one hyperplane).
         *
         * The purpose of this routine is to compute the extremal rays of
         * the new cone formed by intersecting the original cone with this
         * linear subspace.  The resulting list of extremal rays will
         * contain no duplicates or redundancies, and they are guaranteed to
         * be of the same subclass of NRay as the initial extremal rays.
         *
         * Parameter \a constraints may contain a set of validity constraints,
         * in which case this routine will only return \e valid extremal
         * rays.  Each validity constraint is of the form "an extremal ray
         * may only lie outside at most one of these original facets"; see the
         * NEnumConstraintList class for details.  These contraints have the
         * important property that, although validity is not preserved
         * under convex combination, \e invalidity is.
         *
         * A numeric progress watcher may be passed for progress reporting.
         * If so, this routine will poll for cancellation requests accordingly.
         *
         * \pre The cone described by <tt>[oldRaysFirst, oldRaysLast)</tt> and
         * <tt>[facetsFirst, facetsLast)</tt> is convex and
         * satisfies the structural requirements given above.  In
         * particular, it is full dimensional, and neither the ray list nor
         * the facet list contains duplicates or redundancies.
         * \pre If \a constraints is non-zero, then the given list
         * <tt>[oldRaysFirst, oldRaysLast)</tt> must contain only valid rays,
         * where validity is defined by the given set of validity constraints.
         *
         * @param results the output iterator to which the resulting extremal
         * rays will be written; this must accept objects of type <tt>NRay*</tt>
         * (or alternatively pointers to the same subclass of NRay as is
         * used for the initial list of rays).
         * @param oldRaysFirst the beginning of the list
         * <tt>[oldRaysFirst, oldRaysLast)</tt> of extremal rays
         * defining the original cone; this must be a forward iterator over
         * objects of type <tt>const NRay*</tt> (or some subclass thereof).
         * @param oldRaysLast the end of the list of extremal rays
         * defining the original cone.
         * @param facetsFirst the beginning of the list <tt>[facetsFirst,
         * facetsLast)</tt> of facets of the original cone; this must be a
         * forward iterator over objects of type
         * <tt>NVector\<NLargeInteger\>*</tt>.
         * @param facetsLast the end of the list of facets of the original cone.
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
         */
        template <class OutputIterator, class RayIterator, class FacetIterator>
        static void enumerateExtremalRays(OutputIterator results,
            RayIterator oldRaysFirst, RayIterator oldRaysLast,
            FacetIterator facetsFirst, FacetIterator facetsLast,
            const NMatrixInt& subspace,
            const NEnumConstraintList* constraints,
            NProgressNumber* progress = 0);

    private:
        /**
         * A helper class for vertex enumeration, describing both a ray
         * (a vertex of the solution space) and the set of original facets
         * that this ray belongs to.
         *
         * The \a BitmaskType template argument describes how the set of
         * original facets will be stored.  Specifically, the set of facets is
         * stored as a bitmask, with one bit per facet; each bit is set if
         * and only if this ray belongs to the corresponding original facet.
         *
         * Since this class is used heavily, faster bitmask types such
         * as NBitmask1 and NBitmask2 are preferred; however, if the
         * number of facets is too large then the slower general-use NBitmask
         * class will need to be used instead.
         *
         * \pre The template argument \a BitmaskType is one of Regina's
         * bitmask types, such as NBitmask, NBitmask1 or NBitmask2.
         */
        template <class BitmaskType>
        class RaySpec : public NFastVector<NLargeInteger> {
            private:
                BitmaskType facets_;
                    /**< A bitmask listing which original facets this ray
                         belongs to. */

            public:
                /**
                 * Creates a new ray specification.  The bitmask listing
                 * which facets the ray belongs to will be computed
                 * automatically for the given range of facets.
                 *
                 * \pre The given range of facets is not empty, but is
                 * small enough that \a BitmaskType can hold the
                 * corresponding number of bits.
                 *
                 * @param ray the new ray, in pure vector form.
                 * @param facetsFirst the beginning of the range of
                 * original facets, as passed to the main routine
                 * NDoubleDescriptor::enumerateExtremalRays().
                 * @param facetsLast the end of the range of original facets.
                 */
                template <class FacetIterator>
                inline RaySpec(const NRay& ray,
                    FacetIterator facetsFirst, FacetIterator facetsLast);

                /**
                 * Creates a new ray, describing where the plane between
                 * two given rays meets the given hyperplane.
                 *
                 * \pre The two given rays lie on opposite sides of the
                 * given hyperplane, and neither ray actually lies \e in
                 * the given hyperplane.
                 *
                 * @param first the first of the given rays.
                 * @param second the second of the given rays.
                 * @param hyperplane the hyperplane that runs between
                 * the two given rays.
                 */
                RaySpec(const RaySpec& first, const RaySpec& second,
                    const NVector<NLargeInteger>& hyperplane);

                /**
                 * Returns a raw bitmask listing which original facets this
                 * ray belongs to.  The individual bits correspond to the
                 * facets passed to the RaySpec constructor; bits are set
                 * to \c true if and only if this ray belongs to the
                 * corresponding facet.
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
                 * Returns the scalar (dot) product of this ray with the given
                 * vector.
                 *
                 * \pre The vector defining this ray and the given vector
                 * both have the same length.
                 *
                 * @param v the vector whose scalar product we are
                 * taking with this ray.
                 * @return the corresponding scalar product.
                 */
                inline NLargeInteger operator * (
                    const NVector<NLargeInteger>& v) const;

                /**
                 * Scale this ray down as far as possible, so that the
                 * coordinates remain integers but are as small in
                 * magnitude as possible.
                 *
                 * The scaling factor is guaranteed to be positive (i.e., the
                 * underlying vector will not accidentally negate itself).
                 */
                void scaleDown();
        };

        /**
         * A comparison object that helps sort hyperplanes into a good
         * order before running the double descriptor algorithm.
         * A hyperplane is described by a row of the \a subspace matrix
         * passed to NDoubleDescriptor::enumerateExtremalRays().
         *
         * The ordering also involves the range of original facets passed
         * to NDoubleDescriptor::enumerateExtremalRays(), and is defined
         * as follows.  For each hyperplane, we create a sequence
         * (h_1, ..., h_f), where h_i is 0 if the hyperplane and the
         * ith facet are perpendicular, or 1 if not.  We then simply
         * compare these sequences lexicographically.
         */
        template <typename FacetIterator>
        class LexComp {
            private:
                const NMatrixInt& matrix_;
                    /**< The \a subspace matrix passed to
                         NDoubleDescriptor::enumerateExtremalRays(). */
                FacetIterator facetsFirst_;
                    /**< The beginning of the range of original facets passed
                         to NDoubleDescriptor::enumerateExtremalRays(). */
                FacetIterator facetsLast_;
                    /**< The end of the range of original facets passed to
                         NDoubleDescriptor::enumerateExtremalRays(). */

            public:
                /**
                 * Creates a new helper object for comparing hyperplanes.
                 *
                 * @param matrix the \a subspace matrix as passed to
                 * NDoubleDescriptor::enumerateExtremalRays().
                 * @param facetsFirst the beginning of the range of original
                 * facets passed to NDoubleDescriptor::enumerateExtremalRays().
                 * @param facetsLast the end of the range of original facets
                 * passed to NDoubleDescriptor::enumerateExtremalRays().
                 */
                inline LexComp(const NMatrixInt& matrix,
                        FacetIterator facetsFirst, FacetIterator facetsLast);

                /**
                 * Determines whether the hyperplane described by
                 * row \a i of the matrix is lexicographically smaller
                 * than the hyperplane described by row \a j.  See the
                 * LexComp class notes for details on what
                 * "lexicographically smaller" means here.
                 *
                 * @param i the first matrix row index; this must be between
                 * 0 and matrix.rows()-1 inclusive, where \a matrix is
                 * the matrix passed to the class constructor.
                 * @param j the second matrix row index; this must also be
                 * between 0 and matrix.rows()-1 inclusive.
                 * @return \c true if and only if the hyperplane
                 * described by row \a i is lexicographically smaller
                 * than the hyperplane described by row \a j.
                 */
                inline bool operator () (int i, int j) const;
        };

        /**
         * Private constructor to ensure that objects of this class are
         * never created.
         */
        NDoubleDescriptor();

        /**
         * Identical to the public routine enumerateExtremalRays(), except
         * that there is an extra template parameter \a BitmaskType.
         * This specifies what type should be used for the bitmask
         * describing which original facets a ray belongs to.
         *
         * All arguments to this function are identical to those for the
         * public routine enumerateExtremalRays().
         *
         * \pre The bitmask type is one of Regina's bitmask types, such
         * as NBitmask, NBitmask1 or NBitmask2.
         * \pre The type \a BitmaskType can handle at least \a f bits,
         * where \a f is the number of original facets in the given range.
         * \pre The given range of facets is not empty.
         */
        template <class BitmaskType,
                  class OutputIterator, class RayIterator, class FacetIterator>
        static void enumerateUsingBitmask(OutputIterator results,
            RayIterator oldRaysFirst, RayIterator oldRaysLast,
            FacetIterator facetsFirst, FacetIterator facetsLast,
            const NMatrixInt& subspace,
            const NEnumConstraintList* constraints,
            NProgressNumber* progress);

        /**
         * A part of the full double descriptor algorithm that
         * intersects the current solution set with a new hyperplane.
         *
         * The input list \a src must contain the vertices of the
         * current solution space.  This routine intersects the current
         * solution space with the given hyperplane, and places the
         * vertices of the new solution space in the output list \a dest.
         *
         * The set of validity constraints must be passed here as a
         * C-style array of bitmasks.  Each bitmask is a bitmask of facets,
         * as seen in the RaySpec inner class.  Each constraint is of the
         * form "a point cannot live outside more than one of these facets";
         * the bits for these facets must be set to 1 in the corresponding
         * bitmask, and all other bits must be set to 0.
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
         * @param hyperplane the equation for the new hyperplane to intersect.
         * @param prevHyperplanes the number of hyperplanes that have
         * already been intersected with the original cone to form the
         * current solution set.  This does not include the hyperplane
         * currently under consideration.
         * @param constraintsBegin the beginning of the C-style array of
         * validity constraints.  This should be 0 if no additional
         * constraints are to be imposed.
         * @param constraintsEnd a pointer just past the end of the
         * C-style array of validity constraints.  This should be 0
         * if no additional constraints are to be imposed.
         */
        template <class BitmaskType>
        static void intersectHyperplane(
            std::vector<RaySpec<BitmaskType>*>& src,
            std::vector<RaySpec<BitmaskType>*>& dest,
            const NVector<NLargeInteger>& hyperplane,
            unsigned prevHyperplanes,
            const BitmaskType* constraintsBegin,
            const BitmaskType* constraintsEnd);
};

/*@}*/

// Inline functions for NDoubleDescriptor::RayDesc

template <class BitmaskType>
template <class FacetIterator>
inline NDoubleDescriptor::RaySpec<BitmaskType>::RaySpec(
        const NRay& ray, FacetIterator facetsFirst, FacetIterator facetsLast) :
        NFastVector<NLargeInteger>(ray),
        facets_(std::distance(facetsFirst, facetsLast)) {
    FacetIterator it = facetsFirst;
    for (unsigned i = 0; it != facetsLast; ++i)
        facets_.set(i, (**it++) * ray == 0);
}

template <class BitmaskType>
inline const BitmaskType& NDoubleDescriptor::RaySpec<BitmaskType>::facets()
        const {
    return facets_;
}

template <class BitmaskType>
inline bool NDoubleDescriptor::RaySpec<BitmaskType>::onAllCommonFacets(
        const RaySpec<BitmaskType>& x, const RaySpec<BitmaskType>& y) const {
    return facets_.containsIntn(x.facets_, y.facets_);
}

template <class BitmaskType>
inline NLargeInteger NDoubleDescriptor::RaySpec<BitmaskType>::operator * (
        const NVector<NLargeInteger>& v) const {
    NLargeInteger ans(zero);
    const NLargeInteger* e = elements;
    for (unsigned i = 0; e < end; ++i)
        ans += v[i] * (*e++);
    return ans;
}

// Inline functions for NDoubleDescriptor::LexComp

template <typename FacetIterator>
inline NDoubleDescriptor::LexComp<FacetIterator>::LexComp(
        const NMatrixInt& matrix,
        FacetIterator facetsFirst, FacetIterator facetsLast) :
        matrix_(matrix), facetsFirst_(facetsFirst), facetsLast_(facetsLast) {
}

template <typename FacetIterator>
inline bool NDoubleDescriptor::LexComp<FacetIterator>::operator () (
        int i, int j) const {
    NLargeInteger iProd, jProd;
    for (FacetIterator it = facetsFirst_; it != facetsLast_; ++it) {
        iProd = (**it) * NVectorMatrixRow<NLargeInteger>(matrix_, i);
        jProd = (**it) * NVectorMatrixRow<NLargeInteger>(matrix_, j);
        if (iProd == 0 && jProd != 0)
            return true;
        if (iProd != 0 && jProd == 0)
            return false;
    }
    return false;
}

// Inline functions for NDoubleDescriptor

inline NDoubleDescriptor::NDoubleDescriptor() {
}

} // namespace regina

// Template definitions

#include "enumerate/ndoubledescriptor.tcc"

#endif

