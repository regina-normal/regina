
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

/*! \file packet/packettype.h
 *  \brief Defines constants for the various packet types known to Regina.
 */

#ifndef __PACKETTYPE_H
#ifndef __DOXYGEN
#define __PACKETTYPE_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * \weakgroup packet
 * @{
 */

/**
 * Represents the different types of packet that are available in Regina.
 *
 * IDs 0-9999 are reserved for future use by Regina.  If you are extending
 * Regina to include your own packet type, you should choose an ID >= 10000.
 */
enum PacketType {
    /**
     * Represents a container packet, of class Container.
     */
    PACKET_CONTAINER = 1,
    /**
     * Represents a text packet, of class Text.
     */
    PACKET_TEXT = 2,
    /**
     * Deprecated alias for PACKET_TRIANGULATION3.
     *
     * \deprecated This constant has been renamed to PACKET_TRIANGULATION3.
     */
    REGINA_DEPRECATED_ENUM PACKET_TRIANGULATION = 3,
    /**
     * Represents a normal surface list, of class NormalSurfaces.
     */
    PACKET_NORMALSURFACES = 6,
    /**
     * Deprecated alias for PACKET_NORMALSURFACES.
     *
     * \deprecated This constant has been renamed to PACKET_NORMALSURFACES.
     */
    REGINA_DEPRECATED_ENUM PACKET_NORMALSURFACELIST = 6,
    /**
     * Represents a script packet, of class Script.
     */
    PACKET_SCRIPT = 7,
    /**
     * Represents a normal surface filter, of class SurfaceFilter or
     * one of its descendant classes.
     */
    PACKET_SURFACEFILTER = 8,
    /**
     * Represents an angle structure list, of class AngleStructures.
     */
    PACKET_ANGLESTRUCTURES = 9,
    /**
     * Deprecated alias for PACKET_ANGLESTRUCTURES.
     *
     * \deprecated This constant has been renamed to PACKET_ANGLESTRUCTURES.
     */
    REGINA_DEPRECATED_ENUM PACKET_ANGLESTRUCTURELIST = 9,
    /**
     * Represents a PDF document, of class PDF.
     */
    PACKET_PDF = 10,
    /**
     * Deprecated alias for PACKET_TRIANGULATION4.
     *
     * \deprecated This constant has been renamed to PACKET_TRIANGULATION4.
     */
    REGINA_DEPRECATED_ENUM PACKET_DIM4TRIANGULATION = 11,
    /**
     * Represents a normal hypersurface list, of class NormalHypersurfaces.
     */
    PACKET_NORMALHYPERSURFACES = 13,
    /**
     * Deprecated alias for PACKET_NORMALHYPERSURFACES.
     *
     * \deprecated This constant has been renamed to PACKET_NORMALHYPERSURFACES.
     */
    REGINA_DEPRECATED_ENUM PACKET_NORMALHYPERSURFACELIST = 13,
    /**
     * Deprecated alias for PACKET_TRIANGULATION2.
     *
     * \deprecated This constant has been renamed to PACKET_TRIANGULATION2.
     */
    REGINA_DEPRECATED_ENUM PACKET_DIM2TRIANGULATION = 15,
    /**
     * Represents a triangulation in the embedded SnapPea kernel, of
     * class SnapPeaTriangulation.
     */
    PACKET_SNAPPEATRIANGULATION = 16,
    /**
     * Represents a 2-dimensional triangulation, of class Triangulation<2>.
     */
    PACKET_TRIANGULATION2 = 15,
    /**
     * Represents a 3-dimensional triangulation, of class Triangulation<3>.
     */
    PACKET_TRIANGULATION3 = 3,
    /**
     * Represents a 4-dimensional triangulation, of class Triangulation<4>.
     */
    PACKET_TRIANGULATION4 = 11,
    /**
     * Represents a 5-dimensional triangulation, of class Triangulation<5>.
     */
    PACKET_TRIANGULATION5 = 105,
    /**
     * Represents a 6-dimensional triangulation, of class Triangulation<6>.
     */
    PACKET_TRIANGULATION6 = 106,
    /**
     * Represents a 7-dimensional triangulation, of class Triangulation<7>.
     */
    PACKET_TRIANGULATION7 = 107,
    /**
     * Represents a 8-dimensional triangulation, of class Triangulation<8>.
     */
    PACKET_TRIANGULATION8 = 108,
    /**
     * Represents a 9-dimensional triangulation, of class Triangulation<9>.
     */
    PACKET_TRIANGULATION9 = 109,
    /**
     * Represents a 10-dimensional triangulation, of class Triangulation<10>.
     */
    PACKET_TRIANGULATION10 = 110,
    /**
     * Represents a 11-dimensional triangulation, of class Triangulation<11>.
     */
    PACKET_TRIANGULATION11 = 111,
    /**
     * Represents a 12-dimensional triangulation, of class Triangulation<12>.
     */
    PACKET_TRIANGULATION12 = 112,
    /**
     * Represents a 13-dimensional triangulation, of class Triangulation<13>.
     */
    PACKET_TRIANGULATION13 = 113,
    /**
     * Represents a 14-dimensional triangulation, of class Triangulation<14>.
     */
    PACKET_TRIANGULATION14 = 114,
    /**
     * Represents a 15-dimensional triangulation, of class Triangulation<15>.
     */
    PACKET_TRIANGULATION15 = 115
};

/*@}*/

} // namespace regina

#endif

