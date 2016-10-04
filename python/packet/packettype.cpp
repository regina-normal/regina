
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include <boost/python.hpp>
#include "packet/packettype.h"

using namespace boost::python;

void addPacketType() {
    scope global;

    enum_<regina::PacketType>("PacketType")
        .value("PACKET_CONTAINER", regina::PACKET_CONTAINER)
        .value("PACKET_TEXT", regina::PACKET_TEXT)
        .value("PACKET_SCRIPT", regina::PACKET_SCRIPT)
        .value("PACKET_PDF", regina::PACKET_PDF)
        .value("PACKET_TRIANGULATION", regina::PACKET_TRIANGULATION)
        .value("PACKET_ANGLESTRUCTURES", regina::PACKET_ANGLESTRUCTURES)
        .value("PACKET_ANGLESTRUCTURELIST", regina::PACKET_ANGLESTRUCTURELIST)
        .value("PACKET_NORMALSURFACES", regina::PACKET_NORMALSURFACES)
        .value("PACKET_NORMALSURFACELIST", regina::PACKET_NORMALSURFACELIST)
        .value("PACKET_DIM2TRIANGULATION", regina::PACKET_DIM2TRIANGULATION)
        .value("PACKET_SURFACEFILTER", regina::PACKET_SURFACEFILTER)
        .value("PACKET_DIM4TRIANGULATION", regina::PACKET_DIM4TRIANGULATION)
        .value("PACKET_NORMALHYPERSURFACES", regina::PACKET_NORMALHYPERSURFACES)
        .value("PACKET_NORMALHYPERSURFACELIST", regina::PACKET_NORMALHYPERSURFACELIST)
        .value("PACKET_SNAPPEATRIANGULATION", regina::PACKET_SNAPPEATRIANGULATION)
        .value("PACKET_TRIANGULATION2", regina::PACKET_TRIANGULATION2)
        .value("PACKET_TRIANGULATION3", regina::PACKET_TRIANGULATION3)
        .value("PACKET_TRIANGULATION4", regina::PACKET_TRIANGULATION4)
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
        ;

    global.attr("PACKET_CONTAINER") = regina::PACKET_CONTAINER;
    global.attr("PACKET_TEXT") = regina::PACKET_TEXT;
    global.attr("PACKET_SCRIPT") = regina::PACKET_SCRIPT;
    global.attr("PACKET_PDF") = regina::PACKET_PDF;
    global.attr("PACKET_TRIANGULATION") = regina::PACKET_TRIANGULATION;
    global.attr("PACKET_ANGLESTRUCTURES") = regina::PACKET_ANGLESTRUCTURES;
    global.attr("PACKET_ANGLESTRUCTURELIST") = regina::PACKET_ANGLESTRUCTURELIST;
    global.attr("PACKET_NORMALSURFACES") = regina::PACKET_NORMALSURFACES;
    global.attr("PACKET_NORMALSURFACELIST") = regina::PACKET_NORMALSURFACELIST;
    global.attr("PACKET_DIM2TRIANGULATION") = regina::PACKET_DIM2TRIANGULATION;
    global.attr("PACKET_SURFACEFILTER") = regina::PACKET_SURFACEFILTER;
    global.attr("PACKET_DIM4TRIANGULATION") = regina::PACKET_DIM4TRIANGULATION;
    global.attr("PACKET_NORMALHYPERSURFACES") = regina::PACKET_NORMALHYPERSURFACES;
    global.attr("PACKET_NORMALHYPERSURFACELIST") = regina::PACKET_NORMALHYPERSURFACELIST;
    global.attr("PACKET_SNAPPEATRIANGULATION") = regina::PACKET_SNAPPEATRIANGULATION;
    global.attr("PACKET_TRIANGULATION2") = regina::PACKET_TRIANGULATION2;
    global.attr("PACKET_TRIANGULATION3") = regina::PACKET_TRIANGULATION3;
    global.attr("PACKET_TRIANGULATION4") = regina::PACKET_TRIANGULATION4;
    global.attr("PACKET_TRIANGULATION5") = regina::PACKET_TRIANGULATION5;
    global.attr("PACKET_TRIANGULATION6") = regina::PACKET_TRIANGULATION6;
    global.attr("PACKET_TRIANGULATION7") = regina::PACKET_TRIANGULATION7;
    global.attr("PACKET_TRIANGULATION8") = regina::PACKET_TRIANGULATION8;
    global.attr("PACKET_TRIANGULATION9") = regina::PACKET_TRIANGULATION9;
    global.attr("PACKET_TRIANGULATION10") = regina::PACKET_TRIANGULATION10;
    global.attr("PACKET_TRIANGULATION11") = regina::PACKET_TRIANGULATION11;
    global.attr("PACKET_TRIANGULATION12") = regina::PACKET_TRIANGULATION12;
    global.attr("PACKET_TRIANGULATION13") = regina::PACKET_TRIANGULATION13;
    global.attr("PACKET_TRIANGULATION14") = regina::PACKET_TRIANGULATION14;
    global.attr("PACKET_TRIANGULATION15") = regina::PACKET_TRIANGULATION15;
}

