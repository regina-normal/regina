
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#ifndef __REGINA_NORMALCOORDS_H
#ifndef __DOXYGEN
#define __REGINA_NORMALCOORDS_H
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
     * transversely oriented normal surfaces.
     */
    NS_ORIENTED = 300,
    /**
     * Represents quadrilateral coordinates for transversely oriented 
     * normal surfaces.
     */
    NS_ORIENTED_QUAD = 301,
};

/*@}*/

} // namespace regina

#endif

