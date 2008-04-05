
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

#include "maths/nray.h"

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
         * Private constructor to ensure that objects of this class are
         * never created.
         */
        NDoubleDescriptor();

        /**
         * Implements vertex enumeration for the simplified case in which
         * the given linear subspace is just a single hyperplane.
         *
         * In all other respects this routine behaves identically to
         * the full vertex enumeration routine
         * enumerateVertices(OutputIterator, RayIterator, RayIterator, FaceIterator, FaceIterator, const NMatrixInt&, const NCompConstraintSet*).
         *
         * All parameters not listed are identical to those for the full
         * vertex enumeration routine.
         *
         * @param hyperplane the hyperplane that forms the given linear
         * subspace; this hyperplane is represented by a vertex
         * perpendicular to it.
         */
        template <class OutputIterator, class RayIterator, class FaceIterator>
        static void enumerateVertices(OutputIterator results,
            RayIterator oldRaysFirst, RayIterator oldRaysLast,
            FaceIterator facesFirst, FaceIterator facesLast,
            const NVector<NLargeInteger>& hyperplane,
            const NCompConstraintSet* constraints);
};

/*@}*/

// Inline functions for NDoubleDescriptor

inline NDoubleDescriptor::NDoubleDescriptor() {
}

} // namespace regina

// Template definitions

#include "enumerate/ndoubledescriptor.tcc"

#endif

