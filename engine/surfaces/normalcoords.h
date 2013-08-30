
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

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
 * Represents different flavours of coordinate system that can
 * be used for enumerating and displaying normal surfaces.
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
     * This flavour is only used with legacy data files; new vectors
     * and lists of this flavour cannot be created.  The underlying
     * coordinate system is identical to NS_AN_STANDARD.
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
     * This flavour is for display only; surface
     * vectors and lists of this flavour cannot be created.
     */
    NS_EDGE_WEIGHT = 200,
    /**
     * Represents face arc coordinates for normal surfaces.
     * This flavour is for display only; surface
     * vectors and lists of this flavour cannot be created.
     */
    NS_FACE_ARCS = 201,
    /**
     * Represents standard triangle-quadrilateral coordinates for
     * transversely oriented normal surfaces.
     */
    NS_ORIENTED = 300,
    /**
     * Represents quadrilateral coordinates for transversely oriented 
     * normal surfaces.
     */
    NS_ORIENTED_QUAD = 301
};

/*@}*/

} // namespace regina

#endif

