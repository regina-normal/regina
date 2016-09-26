
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

/*! \file surfaces/normalcoords.h
 *  \brief Defines constants for normal surface coordinate systems.
 */

#ifndef __NORMALCOORDS_H
#ifndef __DOXYGEN
#define __NORMALCOORDS_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * Represents different coordinate systems that can
 * be used for enumerating and displaying normal surfaces.
 *
 * IDs 0-9999 are reserved for future use by Regina.  If you are extending
 * Regina to include your own coordinate system, you should choose
 * an ID >= 10000.
 */
enum NormalCoords {
    /**
     * Represents standard triangle-quadrilateral coordinates for
     * normal surfaces.
     */
    NS_STANDARD = 0,
    /**
     * Represents quadrilateral coordinates for normal surfaces.
     * For details, see "Normal surface Q-theory", Jeffrey L. Tollefson,
     * Pacific J. Math. 183 (1998), no. 2, 359--374.
     */
    NS_QUAD = 1,
    /**
     * Indicates that a list of almost normal surfaces was created
     * using Regina 4.5.1 or earlier, where surfaces with more than
     * one octagon of the same type were stripped out of the final
     * solution set.  As of Regina 4.6 such surfaces are now
     * included in the solution set, since we need them if we
     * wish to enumerate \e all almost normal surfaces (not just
     * the \e vertex almost normal surfaces).
     *
     * This coordinate system is only used with legacy data files; new vectors
     * and lists cannot be created in this coordinate system.  The underlying
     * coordinates are identical to those of NS_AN_STANDARD.
     */
    NS_AN_LEGACY = 100,
    /**
     * Represents quadrilateral-octagon coordinates for octagonal
     * almost normal surfaces.  For details, see
     * "Quadrilateral-octagon coordinates for almost normal surfaces",
     * Benjamin A. Burton, Experiment. Math. 19 (2010), 285-315.
     */
    NS_AN_QUAD_OCT = 101,
    /**
     * Represents standard triangle-quadrilateral-octagon coordinates
     * for octagonal almost normal surfaces.
     */
    NS_AN_STANDARD = 102,
    /**
     * Represents edge weight coordinates for normal surfaces.
     * This coordinate system is for display only; surface
     * vectors and lists cannot be created in this coordinate system.
     */
    NS_EDGE_WEIGHT = 200,
    /**
     * Represents triangle arc coordinates for normal surfaces.
     * This coordinate system is for display only; surface
     * vectors and lists cannot be created in this coordinate system.
     */
    NS_TRIANGLE_ARCS = 201,
    /**
     * Represents standard triangle-quadrilateral coordinates for
     * transversely oriented normal surfaces.
     */
    NS_ORIENTED = 300,
    /**
     * Represents quadrilateral coordinates for transversely oriented 
     * normal surfaces.
     */
    NS_ORIENTED_QUAD = 301,
    /**
     * Represents angle structure coordinates.
     *
     * This coordinate system is \e not for use with normal surfaces:
     * it cannot be used either to display them or enumerate them.
     * Instead it is for use with angle structures on triangulations.
     * Because the combinatorics and linear algebra of angle strutures
     * are tightly related to those of normal surfaces, we include
     * NS_ANGLE here so that angle structure routines can make use of
     * some of Regina's existing normal surface machinery.
     *
     * For a triangulation with \a n tetrahedra, this system has
     * 3<i>n</i>+1 coordinates.  The first 3<i>n</i> are analogous
     * to quadrilateral coordinates (specifically, for each
     * quadrilateral type \a Q, the corresponding angle structure coordinate
     * represents the pair of angles in the same tetrahedron that \a Q does
     * not meet).  The final coordinate is a scaling coordinate, used to
     * projectivise the angle structure polytope so that it becomes a
     * polyhedral cone that is invariant under (positive) scaling.
     * If the final scaling coordinate is \a s, then a rational value of \a x
     * in any other coordinate position should be interpreted as the angle
     * <i>x</i>.&pi;/<i>s</i>.
     *
     * \pre This coordinate system must not be used with any of Regina's
     * routines unless they explicitly declare that NS_ANGLE is allowed.
     */
    NS_ANGLE = 400
};

/*@}*/

} // namespace regina

#endif

