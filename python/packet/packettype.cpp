
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "regina-config.h" // for REGINA_HIGHDIM
#include "packet/packettype.h"
#include "../helpers.h"
#include "../docstrings/packet/packettype.h"

#define WRAP_PACKET_TYPE(val) .value(#val, regina::PacketType::val, rdoc::val)

void addPacketType(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(PacketType)

    pybind11::enum_<regina::PacketType>(m, "PacketType", rdoc_scope)
        WRAP_PACKET_TYPE(None)
        WRAP_PACKET_TYPE(Container)
        WRAP_PACKET_TYPE(Text)
        WRAP_PACKET_TYPE(Script)
        WRAP_PACKET_TYPE(Attachment)
        WRAP_PACKET_TYPE(AngleStructures)
        WRAP_PACKET_TYPE(NormalSurfaces)
        WRAP_PACKET_TYPE(SurfaceFilter)
        WRAP_PACKET_TYPE(NormalHypersurfaces)
        WRAP_PACKET_TYPE(SnapPea)
        WRAP_PACKET_TYPE(Link)
        WRAP_PACKET_TYPE(Triangulation2)
        WRAP_PACKET_TYPE(Triangulation3)
        WRAP_PACKET_TYPE(Triangulation4)
        WRAP_PACKET_TYPE(Triangulation5)
        WRAP_PACKET_TYPE(Triangulation6)
        WRAP_PACKET_TYPE(Triangulation7)
        WRAP_PACKET_TYPE(Triangulation8)
#ifdef REGINA_HIGHDIM
        WRAP_PACKET_TYPE(Triangulation9)
        WRAP_PACKET_TYPE(Triangulation10)
        WRAP_PACKET_TYPE(Triangulation11)
        WRAP_PACKET_TYPE(Triangulation12)
        WRAP_PACKET_TYPE(Triangulation13)
        WRAP_PACKET_TYPE(Triangulation14)
        WRAP_PACKET_TYPE(Triangulation15)
#endif
        ;

    // Deprecated constants:
    m.attr("PACKET_NONE") = regina::PacketType::None;
    m.attr("PACKET_CONTAINER") = regina::PacketType::Container;
    m.attr("PACKET_TEXT") = regina::PacketType::Text;
    m.attr("PACKET_SCRIPT") = regina::PacketType::Script;
    m.attr("PACKET_ATTACHMENT") = regina::PacketType::Attachment;
    m.attr("PACKET_ANGLESTRUCTURES") = regina::PacketType::AngleStructures;
    m.attr("PACKET_NORMALSURFACES") = regina::PacketType::NormalSurfaces;
    m.attr("PACKET_SURFACEFILTER") = regina::PacketType::SurfaceFilter;
    m.attr("PACKET_NORMALHYPERSURFACES") =
        regina::PacketType::NormalHypersurfaces;
    m.attr("PACKET_SNAPPEATRIANGULATION") = regina::PacketType::SnapPea;
    m.attr("PACKET_LINK") = regina::PacketType::Link;
    m.attr("PACKET_TRIANGULATION2") = regina::PacketType::Triangulation2;
    m.attr("PACKET_TRIANGULATION3") = regina::PacketType::Triangulation3;
    m.attr("PACKET_TRIANGULATION4") = regina::PacketType::Triangulation4;
    m.attr("PACKET_TRIANGULATION5") = regina::PacketType::Triangulation5;
    m.attr("PACKET_TRIANGULATION6") = regina::PacketType::Triangulation6;
    m.attr("PACKET_TRIANGULATION7") = regina::PacketType::Triangulation7;
    m.attr("PACKET_TRIANGULATION8") = regina::PacketType::Triangulation8;
#ifdef REGINA_HIGHDIM
    m.attr("PACKET_TRIANGULATION9") = regina::PacketType::Triangulation9;
    m.attr("PACKET_TRIANGULATION10") = regina::PacketType::Triangulation10;
    m.attr("PACKET_TRIANGULATION11") = regina::PacketType::Triangulation11;
    m.attr("PACKET_TRIANGULATION12") = regina::PacketType::Triangulation12;
    m.attr("PACKET_TRIANGULATION13") = regina::PacketType::Triangulation13;
    m.attr("PACKET_TRIANGULATION14") = regina::PacketType::Triangulation14;
    m.attr("PACKET_TRIANGULATION15") = regina::PacketType::Triangulation15;
#endif

    RDOC_SCOPE_SWITCH(PacketInfo)

    auto i = pybind11::class_<regina::PacketInfo>(m, "PacketInfo", rdoc_scope)
        .def_static("name", &regina::PacketInfo::name, rdoc::name)
        ;
    regina::python::no_eq_static(i);

    RDOC_SCOPE_END
}

