
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nconeray.h
 *  \brief Provides routines to enumerate extremal rays of cones of
 *  convex polytopes.
 */

#ifndef __NCONERAY_H
#ifndef __DOXYGEN
#define __NCONERAY_H
#endif

#include "utilities/nmpi.h"
#include "maths/nvectordense.h"

namespace regina {

class NMatrixInt;

/**
 * Represents a ray in the boundary of a cone.
 * The cone is assumed to have its apex at the origin.  A ray is
 * represented by the coordinates of the first point it passes through
 * whose coordinates are all integers; it is assumed that all the faces
 * that define the cone are rational.
 *
 * \warning Subclasses of NConeRay \b must override clone() to return a
 * vector of the correct subclass!  Otherwise the vectors returned by
 * vertex solution routines such as ::intersectCone()
 * will be NConeRay objects instead of objects of the appropriate
 * derived class.
 *
 * \ifaces Not present.
 */
class NConeRay : public NVectorDense<NLargeInteger> {
    public:
        /**
         * Creates a new cone ray all of whose entries are initialised
         * to zero.
         *
         * @param length the number of elements in the new vector.
         */
        NConeRay(unsigned length);
        /**
         * Creates a new cone ray that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NConeRay(const NVector<NLargeInteger>& cloneMe);

        virtual NVector<NLargeInteger>* clone() const;

        /**
         * Determines if this and the given cone ray can be summed to
         * give another "valid" cone ray, where "valid" may be defined
         * differently for different subclasses of NConeRay.
         *
         * The default implementation for this routine always returns
         * \c true.
         *
         * \pre Both this and the given cone ray are valid.
         * \pre Both this and the given cone ray use the same underlying
         * coordinate system.
         *
         * @param other the cone ray whose compatibility with this is
         * being examined.
         * @return \c true if and only if this and the given vector can
         * be summed to give another valid cone ray.
         */
        virtual bool isCompatibleWith(const NConeRay& other) const;

        /**
         * Scales this vector down by the greatest common divisor of all
         * its elements.  The resulting vector will be the smallest
         * multiple of the original that maintains integral entries, and
         * these entries will have the same signs as the originals.
         *
         * This routine poses no problem for vectors containing infinite
         * elements; such elements are simply ignored and left at
         * infinity.
         */
        void scaleDown();
};

/**
 * Returns a newly allocated cone ray representing the intersection
 * of the given hyperplane with the cone 2-face joining two given
 * adjacent rays.
 * The resulting ray will be in its smallest integral form.
 *
 * The hyperplane must pass through the origin, and is represented by
 * the vector of a ray perpendicular to it.
 *
 * If the arguments \a pos and \a neg are on the positive and negative
 * sides of the hyperplane respectively (where positive and
 * negative sides are determined by the sign of the dot product of a
 * ray vector with the hyperplane representation vector), the resulting
 * ray is guaranteed to be a positive multiple of a convex combination of
 * the two original rays.
 *
 * The resulting ray is guaranteed to be of the same subclass of
 * NConeRay as argument \a neg.
 *
 * \pre The two given cone rays are adjacent, that is
 * there is a cone 2-face to which they both belong.
 * \pre The two given cone rays lie on opposite sides of
 * the hyperplane; neither actually lies within the hyperplane.
 *
 * \ifaces Not present.
 *
 * @param pos one of the two adjacent cone rays.
 * @param neg the other of the two adjacent cone rays.
 * @param hyperplane the hyperplane to intersect with the 2-face.
 * @return a newly allocated ray representing the intersection of
 * \a hyperplane with the cone 2-face joining \a a and \a b.
 */
NConeRay* intersectLine(const NConeRay& pos, const NConeRay& neg,
    const NConeRay& hyperplane);

/**
 * Determines the extremal rays of the intersection of the given cone
 * with the given hyperplane.  The resulting rays will be newly
 * allocated and written to the given output iterator.  Their deallocation is
 * the responsibility of whoever called this routine.
 *
 * The given cone is represented by a list of its extremal rays and a
 * list of hyperplanes that determine its faces.  Specifically the list
 * of face hyperplanes must be a set of hyperplanes passing through the
 * origin for which the actual faces of the cone are determined by
 * intersecting this set of hyperplanes with some subspace of the entire
 * vector space.  Note that this list of hyperplanes might well be the
 * faces themselves.
 *
 * Conditions upon the structure of the cone and the ray and face lists
 * are identical to those for
 * ::intersectCone(OutputIterator, RayIterator, RayIterator, FaceIterator, FaceIterator, const NMatrixInt&, bool).
 *
 * The hyperplane whose intersection we will take with the cone must pass
 * through the origin, and is represented by the vector of a ray
 * perpendicular to it.
 *
 * The resulting extremal rays are guaranteed not to contain any
 * duplicates or redundancies.  They are guaranteed to be of the same
 * subclass of NConeRay as the initial extremal rays.
 *
 * If \a testCompatibility is set to \c true, only "valid" extremal rays
 * as defined by NConeRay::isCompatibleWith() will be found.
 *
 * \pre The cone described by <tt>[oldRaysFirst, oldRaysLast)</tt>
 * and <tt>[facesFirst, facesLast)</tt> is convex and
 * satisfies the structural requirements given above.
 * \pre The list <tt>[oldRaysFirst, oldRaysLast)</tt> of extremal rays
 * does not contain any duplicates or redundancies.
 * \pre If \a testCompatibility is passed as \c true, then the list
 * </tt>[oldRaysFirst, oldRaysLast)</tt> must contain only "valid" rays.
 *
 * \ifaces Not present.
 *
 * @param results the output iterator to which the resulting extremal
 * rays will be written; this must accept objects of type <tt>NConeRay*</tt>
 * (or alternatively pointers to the same subclass of NConeRay as is
 * used for the initial list of rays).
 * @param oldRaysFirst the beginning of the list
 * <tt>[oldRaysFirst, oldRaysLast)</tt> of extremal rays
 * defining the cone to intersect with the given hyperplane; this must
 * be a forward iterator over objects of type <tt>const NConeRay*</tt>
 * (or some subclass thereof).
 * @param oldRaysLast the end of the list
 * <tt>[oldRaysFirst, oldRaysLast)</tt> of extremal rays
 * defining the cone to intersect with the given hyperplane; this must
 * be a forward iterator over objects of type <tt>const NConeRay*</tt>
 * (or some subclass thereof).
 * @param facesFirst the beginning of the list <tt>[facesFirst, facesLast)</tt>
 * of hyperplanes that determine the faces of the given cone, as described
 * above; each hyperplane is represented by the vector of a ray perpendicular
 * to it.  Note that this list is allowed to contain duplicates or
 * redundancies.  This must be a forward iterator over objects of type
 * <tt>NVector\<NLargeInteger\>*</tt>.
 * @param facesLast the end of the list <tt>[facesFirst, facesLast)</tt>
 * of hyperplanes that determine the faces of the given cone.
 * This must be a forward iterator over objects of type
 * <tt>NVector\<NLargeInteger\>*</tt>.
 * @param hyperplane the hyperplane to intersect with the given cone.
 * @param testCompatibility \c true if we are to only find "valid" extremal
 * rays as defined by NConeRay::isCompatibleWith().
 */
template <class OutputIterator, class RayIterator, class FaceIterator>
void intersectCone(OutputIterator results,
    RayIterator oldRaysFirst, RayIterator oldRaysLast,
    FaceIterator facesFirst, FaceIterator facesLast,
    const NConeRay& hyperplane, bool testCompatibility);

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
 * subclass of NConeRay as the initial extremal rays.
 *
 * If \a testCompatibility is set to \c true, only "valid" extremal rays
 * as defined by NConeRay::isCompatibleWith() will be found.
 * In this case the given cone
 * may be a union of many smaller cones, since "validity" need not be
 * preserved under addition.
 * These smaller cones may intersect, and an extremal ray may belong to
 * more than one such cone.  In such cases, the ray should \b not be
 * duplicated.
 *
 * The algorithm used is a modified double descriptor method.
 *
 * \pre The cone described by <tt>[oldRaysFirst, oldRaysLast)</tt> and
 * <tt>[facesFirst, facesLast)</tt> is convex and
 * satisfies the structural requirements given above.
 * \pre The list <tt>[oldRaysFirst, oldRaysLast)</tt> of extremal rays does not
 * contain any duplicates or redundancies.
 * \pre If \a testCompatibility is passed as \c true, then the given list
 * <tt>[oldRaysFirst, oldRaysLast)</tt> must contain only "valid" rays.
 *
 * \todo \opt Intersect the hyperplanes in a good order.
 *
 * \ifaces Not present.
 *
 * @param results the output iterator to which the resulting extremal
 * rays will be written; this must accept objects of type <tt>NConeRay*</tt>
 * (or alternatively pointers to the same subclass of NConeRay as is
 * used for the initial list of rays).
 * @param oldRaysFirst the beginning of the list
 * <tt>[oldRaysFirst, oldRaysLast)</tt> of extremal rays
 * defining the cone to intersect with the given subspace; this must
 * be a forward iterator over objects of type <tt>const NConeRay*</tt>
 * (or some subclass thereof).
 * @param oldRaysLast the end of the list
 * <tt>[oldRaysFirst, oldRaysLast)</tt> of extremal rays
 * defining the cone to intersect with the given subspace; this must
 * be a forward iterator over objects of type <tt>const NConeRay*</tt>
 * (or some subclass thereof).
 * @param facesFirst the beginning of the list <tt>[facesFirst, facesLast)</tt>
 * of hyperplanes that determine the faces of the given cone, as described
 * above; each hyperplane is represented by the vector of a ray perpendicular
 * to it.  Note that this list is allowed to contain duplicates or
 * redundancies.  This must be a forward iterator over objects of type
 * <tt>NVector\<NLargeInteger\>*</tt>.
 * @param facesLast the end of the list <tt>[facesFirst, facesLast)</tt>
 * of hyperplanes that determine the faces of the given cone.
 * This must be a forward iterator over objects of type
 * <tt>NVector\<NLargeInteger\>*</tt>.
 * @param subspace a matrix whose rows are hyperplanes whose intersection
 * defines the subspace to intersect with the given cone.
 * @param testCompatibility \c true if we are to only find "valid" extremal
 * rays as defined by NConeRay::isCompatibleWith().
 */
template <class OutputIterator, class RayIterator, class FaceIterator>
void intersectCone(OutputIterator results,
    RayIterator oldRaysFirst, RayIterator oldRaysLast,
    FaceIterator facesFirst, FaceIterator facesLast,
    const NMatrixInt& subspace, bool testCompatibility);

// Inline functions for NConeRay

inline NConeRay::NConeRay(unsigned length) :
        NVectorDense<NLargeInteger>(length, zero) {
}

inline NConeRay::NConeRay(const NVector<NLargeInteger>& cloneMe) :
        NVectorDense<NLargeInteger>(cloneMe) {
}

inline NVector<NLargeInteger>* NConeRay::clone() const {
    return new NConeRay(*this);
}

inline bool NConeRay::isCompatibleWith(const NConeRay&) const {
    return true;
}

} // namespace regina

// Template definitions

#include "surfaces/nconeray.tcc"

#endif

