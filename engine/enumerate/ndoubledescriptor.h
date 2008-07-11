
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
#include <iterator>
#include <list>

namespace regina {

class NCompConstraintSet;
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
         * The given cone is represented by a list of its extremal rays and a
         * list of hyperplanes that determine its faces.  Specifically the list
         * of face hyperplanes must be a set of hyperplanes passing through the
         * origin for which the actual faces of the cone are determined by
         * intersecting this set of hyperplanes with some subspace of the entire
         * vector space.  Note that this list of hyperplanes might well be the
         * faces themselves.
         *
         * The new linear subspace to intersect is represented by a matrix in
         * which each row represents a hyperplane through
         * the origin; the subspace is the intersection of all these
         * hyperplanes.  Each hyperplane is represented by the vector of a ray
         * perpendicular to it.
         *
         * The resulting list of extremal rays is guaranteed not to contain any
         * duplicates or redundancies.  They are guaranteed to be of the same
         * subclass of NRay as the initial extremal rays.
         *
         * Parameter \a constraints may contain a set of compatibility
         * constraints representing necessary and sufficient conditions
         * for a convex combination of two valid rays to be also valid,
         * where the definition of \e valid depends upon the specific
         * application at hand.  If a set of constraints is passed, only
         * valid extremal rays (as defined by these constraints) will
         * be found.  In this case the given cone may be a union of many
         * smaller cones, since validity need not be preserved under addition.
         * These smaller cones may intersect, and an extremal ray may belong to
         * more than one such cone.  In such cases, the ray should \b not be
         * duplicated.
         *
         * A numeric progress watcher may be passed for progress reporting.
         * If so, this routine will poll for cancellation requests accordingly.
         *
         * \pre The cone described by <tt>[oldRaysFirst, oldRaysLast)</tt> and
         * <tt>[facesFirst, facesLast)</tt> is convex and
         * satisfies the structural requirements given above.
         * \pre The list <tt>[oldRaysFirst, oldRaysLast)</tt> of extremal rays
         * does not contain any duplicates or redundancies.
         * \pre If \a constraints is non-zero, then the given list
         * <tt>[oldRaysFirst, oldRaysLast)</tt> must contain only valid rays,
         * where validity is defined by the given set of compatibility
         * constraints.
         *
         * \todo \opt Intersect the hyperplanes in a good order.
         *
         * @param results the output iterator to which the resulting extremal
         * rays will be written; this must accept objects of type <tt>NRay*</tt>
         * (or alternatively pointers to the same subclass of NRay as is
         * used for the initial list of rays).
         * @param oldRaysFirst the beginning of the list
         * <tt>[oldRaysFirst, oldRaysLast)</tt> of extremal rays
         * defining the cone to intersect with the given subspace; this must
         * be a forward iterator over objects of type <tt>const NRay*</tt>
         * (or some subclass thereof).
         * @param oldRaysLast the end of the list
         * <tt>[oldRaysFirst, oldRaysLast)</tt> of extremal rays
         * defining the cone to intersect with the given subspace; this must
         * be a forward iterator over objects of type <tt>const NRay*</tt>
         * (or some subclass thereof).
         * @param facesFirst the beginning of the list <tt>[facesFirst,
         * facesLast)</tt> of hyperplanes that determine the faces of the given
         * cone, as described above; each hyperplane is represented by the
         * vector of a ray perpendicular
         * to it.  Note that this list is allowed to contain duplicates or
         * redundancies.  This must be a forward iterator over objects of type
         * <tt>NVector\<NLargeInteger\>*</tt>.
         * @param facesLast the end of the list <tt>[facesFirst, facesLast)</tt>
         * of hyperplanes that determine the faces of the given cone.
         * This must be a forward iterator over objects of type
         * <tt>NVector\<NLargeInteger\>*</tt>.
         * @param subspace a matrix whose rows are hyperplanes whose
         * intersection defines the subspace to intersect with the given cone.
         * @param constraints a set of compatibility constraints that define
         * validity if we are only to find "valid" extremal rays, or 0 if
         * no additional constraints should be imposed.
         * @param progress a numeric progress watcher through which progress
         * will be reported, or 0 if no progress reporting is required.  If
         * a progress watcher is passed, its expected total will be
         * increased immediately by some number of steps and the completed
         * total will be increased gradually by this same number.  Note that
         * NProgress::setFinished() will \e not be called, since
         * whoever called this routine may need to do further processing.
         */
        template <class OutputIterator, class RayIterator, class FaceIterator>
        static void enumerateVertices(OutputIterator results,
            RayIterator oldRaysFirst, RayIterator oldRaysLast,
            FaceIterator facesFirst, FaceIterator facesLast,
            const NMatrixInt& subspace, const NCompConstraintSet* constraints,
            NProgressNumber* progress = 0);

    private:
        /**
         * A helper class for vertex enumeration, describing both a ray
         * (a vertex of the solution space) and the set of faces that
         * this ray belongs to.
         *
         * The \a BitmaskType template argument describes how the set of
         * faces will be stored.  Specifically, the set of faces is
         * stored as bitmask, with one bit per face; each bit is set if
         * and only if this ray belongs to the corresponding face.
         *
         * Since this class is used heavily, faster bitmask types such
         * as NBitmask1 and NBitmask2 are preferred; however, if the
         * number of faces is too large then the slower general-use NBitmask
         * class will need to be used instead.
         *
         * \pre The template argument \a BitmaskType is one of Regina's
         * bitmask types, such as NBitmask, NBitmask1 or NBitmask2.
         */
        template <class BitmaskType>
        class RaySpec : public NFastVector<NLargeInteger> {
            private:
                BitmaskType faces;
                    /**< A bitmask listing which faces this ray belongs to. */

            public:
                /**
                 * Creates a new ray specification.  The bitmask listing
                 * which faces the ray belongs to will be computed
                 * automatically for the given range of faces.
                 *
                 * \pre The given range of faces is not empty, but is
                 * small enough that \a BitmaskType can hold the
                 * corresponding number of bits.
                 *
                 * @param ray the new ray, in pure vector form.
                 * @param facesFirst the beginning of the range of
                 * faces, as passed to the main routine
                 * NDoubleDescriptor::enumerateVertices().
                 * @param facesLast the end of the range of
                 * faces, as passed to the main routine
                 * NDoubleDescriptor::enumerateVertices().
                 */
                template <class FaceIterator>
                inline RaySpec(const NRay& ray,
                    FaceIterator facesFirst, FaceIterator facesLast);

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
                 * Determines whether this ray belongs to all of the
                 * faces that are common to both given rays.
                 *
                 * For this routine to return \c true, every face that
                 * contains both \a x and \a y must contain this ray also.
                 *
                 * @param x the first of the given rays.
                 * @param y the second of the given rays.
                 * @return \c true if and only if this ray belongs to all
                 * of the faces that \e both \a x and \a y belong to.
                 */
                inline bool onAllCommonFaces(
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
         * Private constructor to ensure that objects of this class are
         * never created.
         */
        NDoubleDescriptor();

        /**
         * Identical to the public routine enumerateVertices(), except
         * that there is an extra template parameter \a BitmaskType.
         * This specifies what type should be used for the bitmask
         * describing which faces a ray belongs to.
         *
         * All arguments to this function are identical to those for the
         * public routine enumerateVertices().
         *
         * \pre The bitmask type is one of Regina's bitmask types, such
         * as NBitmask, NBitmask1 or NBitmask2.
         * \pre The type \a BitmaskType can handle at least \a f bits,
         * where \a f is the number of faces in the given range.
         * \pre The given range of faces is not empty.
         */
        template <class BitmaskType,
                  class OutputIterator, class RayIterator, class FaceIterator>
        static void enumerateUsingBitmask(OutputIterator results,
            RayIterator oldRaysFirst, RayIterator oldRaysLast,
            FaceIterator facesFirst, FaceIterator facesLast,
            const NMatrixInt& subspace, const NCompConstraintSet* constraints,
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
         * \pre The input list \a src owns its elements, and the output
         * list \a dest is empty.
         * \post The input list \a src will be empty, and the output
         * list \a dest will own all of the elements that it contains.
         *
         * @param src contains the vertices of the current solution space
         * before this routine is called.
         * @param dest contains the vertices of the new solution space
         * after this routine returns.
         * @param hyperplane the hyperplane to intersect, represented by
         * a vector perpendicular to it.
         * @param constraints the set of compatibility constraints, as
         * passed to the main routine enumerateVertices().  This may be 0
         * if no additional constraints should be imposed.
         */
        template <class BitmaskType>
        static void intersectHyperplane(
            std::list<RaySpec<BitmaskType>*>& src,
            std::list<RaySpec<BitmaskType>*>& dest,
            const NVector<NLargeInteger>& hyperplane,
            const NCompConstraintSet* constraints);
};

/*@}*/

// Inline functions for NDoubleDescriptor::RayDesc

template <class BitmaskType>
template <class FaceIterator>
inline NDoubleDescriptor::RaySpec<BitmaskType>::RaySpec(
        const NRay& ray, FaceIterator facesFirst, FaceIterator facesLast) :
        NFastVector<NLargeInteger>(ray),
        faces(std::distance(facesFirst, facesLast)) {
    FaceIterator it;
    unsigned i = 0;
    for (it = facesFirst; it != facesLast; ++i, ++it)
        faces.set(i, (**it) * ray == 0);
}

template <class BitmaskType>
inline bool NDoubleDescriptor::RaySpec<BitmaskType>::onAllCommonFaces(
        const RaySpec<BitmaskType>& x, const RaySpec<BitmaskType>& y) const {
    return faces.containsIntn(x.faces, y.faces);
}

template <class BitmaskType>
inline NLargeInteger NDoubleDescriptor::RaySpec<BitmaskType>::operator * (
        const NVector<NLargeInteger>& v) const {
    NLargeInteger ans(zero);
    const NLargeInteger* e = elements;
    for (unsigned i = 0; e < end; ++i, ++e)
        ans += v[i] * (*e);
    return ans;
}

// Inline functions for NDoubleDescriptor

inline NDoubleDescriptor::NDoubleDescriptor() {
}

} // namespace regina

// Template definitions

#include "enumerate/ndoubledescriptor.tcc"

#endif

