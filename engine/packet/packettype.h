
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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
     * Represents a container packet, of class NContainer.
     */
    PACKET_CONTAINER = 1,
    /**
     * Represents a text packet, of class NText.
     */
    PACKET_TEXT = 2,
    /**
     * Represents a 3-manifold triangulation, of class NTriangulation.
     */
    PACKET_TRIANGULATION = 3,
    /**
     * Represents a normal surface list, of class NNormalSurfaceList.
     */
    PACKET_NORMALSURFACELIST = 6,
    /**
     * Represents a script packet, of class NScript.
     */
    PACKET_SCRIPT = 7,
    /**
     * Represents a normal surface filter, of class NSurfaceFilter or
     * one of its descendant classes.
     */
    PACKET_SURFACEFILTER = 8,
    /**
     * Represents an angle structure list, of class NAngleStructureList.
     */
    PACKET_ANGLESTRUCTURELIST = 9,
    /**
     * Represents a PDF document, of class NPDF.
     */
    PACKET_PDF = 10,
    /**
     * Represents a 4-manifold triangulation, of class Dim4Triangulation.
     */
    PACKET_DIM4TRIANGULATION = 11,
    /**
     * Represents a normal hypersurface list, of class NNormalHypersurfaceList.
     */
    PACKET_NORMALHYPERSURFACELIST = 13,
    /**
     * Represents a 2-manifold triangulation, of class Dim2Triangulation.
     */
    PACKET_DIM2TRIANGULATION = 15,
    /**
     * Represents a triangulation in the embedded SnapPea kernel, of
     * class NSnapPeaTriangulation.
     */
    PACKET_SNAPPEATRIANGULATION = 16
};

/*@}*/

} // namespace regina

#endif

