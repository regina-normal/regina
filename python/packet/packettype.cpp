
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "../pybind11/pybind11.h"
#include "packet/packettype.h"

void addPacketType(pybind11::module_& m) {
    pybind11::enum_<regina::PacketType>(m, "PacketType")
        .value("PACKET_CONTAINER", regina::PACKET_CONTAINER)
        .value("PACKET_TEXT", regina::PACKET_TEXT)
        .value("PACKET_SCRIPT", regina::PACKET_SCRIPT)
        .value("PACKET_PDF", regina::PACKET_PDF)
        .value("PACKET_TRIANGULATION", regina::PACKET_TRIANGULATION3)
        .value("PACKET_ANGLESTRUCTURES", regina::PACKET_ANGLESTRUCTURES)
        .value("PACKET_ANGLESTRUCTURELIST", regina::PACKET_ANGLESTRUCTURES)
        .value("PACKET_NORMALSURFACES", regina::PACKET_NORMALSURFACES)
        .value("PACKET_NORMALSURFACELIST", regina::PACKET_NORMALSURFACES)
        .value("PACKET_DIM2TRIANGULATION", regina::PACKET_TRIANGULATION2)
        .value("PACKET_SURFACEFILTER", regina::PACKET_SURFACEFILTER)
        .value("PACKET_DIM4TRIANGULATION", regina::PACKET_TRIANGULATION4)
        .value("PACKET_NORMALHYPERSURFACES", regina::PACKET_NORMALHYPERSURFACES)
        .value("PACKET_NORMALHYPERSURFACELIST", regina::PACKET_NORMALHYPERSURFACES)
        .value("PACKET_SNAPPEATRIANGULATION", regina::PACKET_SNAPPEATRIANGULATION)
        .value("PACKET_LINK", regina::PACKET_LINK)
        .value("PACKET_TRIANGULATION2", regina::PACKET_TRIANGULATION2)
        .value("PACKET_TRIANGULATION3", regina::PACKET_TRIANGULATION3)
        .value("PACKET_TRIANGULATION4", regina::PACKET_TRIANGULATION4)
#ifndef REGINA_LOWDIMONLY
        .value("PACKET_TRIANGULATION5", regina::PACKET_TRIANGULATION5)
        .value("PACKET_TRIANGULATION6", regina::PACKET_TRIANGULATION6)
        .value("PACKET_TRIANGULATION7", regina::PACKET_TRIANGULATION7)
        .value("PACKET_TRIANGULATION8", regina::PACKET_TRIANGULATION8)
        .value("PACKET_TRIANGULATION9", regina::PACKET_TRIANGULATION9)
        .value("PACKET_TRIANGULATION10", regina::PACKET_TRIANGULATION10)
        .value("PACKET_TRIANGULATION11", regina::PACKET_TRIANGULATION11)
        .value("PACKET_TRIANGULATION12", regina::PACKET_TRIANGULATION12)
        .value("PACKET_TRIANGULATION13", regina::PACKET_TRIANGULATION13)
        .value("PACKET_TRIANGULATION14", regina::PACKET_TRIANGULATION14)
        .value("PACKET_TRIANGULATION15", regina::PACKET_TRIANGULATION15)
#endif
        .export_values();
}

