
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ndoublelist.h"
    #include "nnormalsurface.h"
#else
    #include "engine/utilities/ndoublelist.h"
    #include "engine/surfaces/nnormalsurface.h"
#endif

class NMatrixInt;

/**
 * Refers to a normal surface as a ray in the boundary of a cone.
 * This is just to aid documentation(!) - the types NNormalSurfaceVector
 * and NConeRay are identical.
 *
 * The cone is assumed to have its apex at the origin.  A ray is
 * represented by the coordinates of the first point it passes through
 * whose coordinates are all integers; it is assumed that all the faces
 * that define the cone are rational.
 *
 * \ifaces Not present.
 */
typedef NNormalSurfaceVector NConeRay;

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
 * allocated and placed in \a results.  Their deallocation is the
 * responsibility of whoever called this routine.
 *
 * The given cone is represented by a list of its extremal rays.
 * Conditions upon its structure are identical to those for
 * intersectCone(const NDoubleList<NConeRay*>&, const NMatrixInt&, bool).
 *
 * The hyperplane must pass through the origin, and is represented by
 * the vector of a ray perpendicular to it.
 *
 * The resulting extremal rays are guaranteed not to contain any
 * duplicates or redundancies.
 *
 * If \a embeddedOnly is set to \c true, only extremal rays representing
 * embedded normal surfaces will be found.
 *
 * \pre The cone described by \a oldRays satisfies the
 * structural requirements given above.
 * \pre The list \a oldRays of extremal rays does not
 * contain any duplicates or redundancies.
 * \pre The list \a results is empty.
 * \pre If \a embeddedOnly is passed as \c true, then
 * the given list \a oldRays must contain only rays representing
 * embedded normal surfaces.
 *
 * \ifaces Not present.
 *
 * @param results the list into which the resulting extremal rays will
 * be placed.
 * @param oldRays the extremal rays defining the cone to intersect with
 * the given hyperplane.
 * @param hyperplane the hyperplane to intersect with the given cone.
 * @param embeddedOnly \c true if we are to only find extremal rays that
 * represent embedded normal surfaces.
 */
void intersectCone(NDoubleList<NConeRay*>& results,
    const NDoubleList<NConeRay*>& oldRays,
    const NConeRay& hyperplane,
    bool embeddedOnly);

/**
 * Determines the extremal rays of the intersection of the given cone
 * with the given linear subspace.  The resulting rays will be newly
 * allocated and placed in a newly allocated list which will be
 * returned.  The deallocation of both the rays and the list is the
 * responsibility of whoever called this routine.
 *
 * The given cone is represented by a list of its extremal rays.
 * It should be a cone that is obtained by intersecting the non-negative
 * cone in the corresponding coordinate space
 * with some previous linear subspace.
 *
 * The new linear subspace to intersect is represented by a matrix in
 * which each row represents a hyperplane through
 * the origin; the subspace is the intersection of all these
 * hyperplanes.  Each hyperplane is represented by the vector of a ray
 * perpendicular to it.
 *
 * The resulting list of extremal rays is guaranteed not to contain any
 * duplicates or redundancies.
 *
 * If \a embeddedOnly is set to \c true, only extremal rays representing
 * embedded normal surfaces will be found.  In this case, the given cone
 * may be a union of many cones, each obtained by adding extra linear
 * equations that force certain quadrilaterals and octahedra to appear
 * zero times.
 * These smaller cones may intersect, and an extremal ray may belong to
 * more than one such cone.  In such cases, the ray should \b not be
 * duplicated.
 * The function NNormalSurfaceVector::isCompatibleWith()
 * will be used to determine if two rays belong to a common smaller
 * cone.
 *
 * The algorithm used is a modified double descriptor method.
 *
 * \pre The cone described by \a oldRays satisfies the
 * structural requirements given above.
 * \pre The list \a oldRays of extremal rays does not
 * contain any duplicates or redundancies.
 * \pre If \a embeddedOnly is passed as \c true, then
 * the given list \a oldRays must contain only rays representing
 * embedded normal surfaces.
 *
 * \todo \opt Intersect the hyperplanes in a good order.
 *
 * \ifaces Not present.
 *
 * @param oldRays the extremal rays defining the cone to intersect with
 * the given subspace.
 * @param subspace a matrix whose rows are hyperplanes whose intersection
 * defines the subspace to intersect with the given cone.
 * @param embeddedOnly \c true if we are to only find extremal rays that
 * represent embedded normal surfaces.
 * @return a newly allocated list of newly allocated rays representing
 * the extremal rays of the intersection of the given cone and the given
 * subspace.
 */
NDoubleList<NConeRay*>* intersectCone(
    const NDoubleList<NConeRay*>& oldRays,
    const NMatrixInt& subspace,
    bool embeddedOnly);

#endif

