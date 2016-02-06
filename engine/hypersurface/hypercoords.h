
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

/*! \file hypersurface/hypercoords.h
 *  \brief Defines constants for normal hypersurface coordinate systems.
 */

#ifndef __HYPERCOORDS_H
#ifndef __DOXYGEN
#define __HYPERCOORDS_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup hypersurface
 * @{
 */

/**
 * Represents different coordinate systems that can
 * be used for enumerating and displaying normal hypersurface
 * within 4-manifold triangulations.
 *
 * IDs 0-9999 are reserved for future use by Regina.  If you are extending
 * Regina to include your own coordinate system, you should choose
 * an ID >= 10000.
 */
enum HyperCoords {
    /**
     * Represents standard tetrahedron-prism coordinates for
     * normal hypersurfaces.
     */
    HS_STANDARD = 0,

    /**
     * Represents prism coordinates for normal hypersurfaces.
     *
     * For now, this coordinate system is for representation only;
     * hypersurface vectors and lists cannot be created in this
     * coordinate system.  This will likely change in a future release
     * of Regina, when prism coordinates will be supported for creation
     * as well as viewing.
     */
    HS_PRISM = 1,

    /**
     * Represents edge weight coordinates for normal hypersurfaces.
     * This coordinate system is for representation only; hypersurface
     * vectors and lists cannot be created in this coordinate system.
     */
    HS_EDGE_WEIGHT = 200
};

/*@}*/

} // namespace regina

#endif

