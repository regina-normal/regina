
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include "regina-config.h" // for REGINA_HIGHDIM and REGINA_PYBIND11_VERSION
#include <pybind11/pybind11.h>
#if REGINA_PYBIND11_VERSION == 3
#include <pybind11/native_enum.h>
#endif
#include "packet/packettype.h"
#include "../helpers.h"
#include "../docstrings/packet/packettype.h"

using regina::PacketType;

#define WRAP_PACKET_TYPE(val) .value(#val, PacketType::val, rdoc::val)

void addPacketType(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(PacketType)

#if REGINA_PYBIND11_VERSION == 3
    pybind11::native_enum<PacketType>(m, "PacketType", "enum.Enum", rdoc_scope)
#elif REGINA_PYBIND11_VERSION == 2
    pybind11::enum_<PacketType>(m, "PacketType", rdoc_scope)
#else
    #error "Unsupported pybind11 version"
#endif
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
        WRAP_PACKET_TYPE(SpatialLink)
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
#if REGINA_PYBIND11_VERSION == 3
        .finalize()
#endif
        ;

    // Deprecated constants:
    m.attr("PACKET_NONE") = PacketType::None;
    m.attr("PACKET_CONTAINER") = PacketType::Container;
    m.attr("PACKET_TEXT") = PacketType::Text;
    m.attr("PACKET_SCRIPT") = PacketType::Script;
    m.attr("PACKET_ATTACHMENT") = PacketType::Attachment;
    m.attr("PACKET_ANGLESTRUCTURES") = PacketType::AngleStructures;
    m.attr("PACKET_NORMALSURFACES") = PacketType::NormalSurfaces;
    m.attr("PACKET_SURFACEFILTER") = PacketType::SurfaceFilter;
    m.attr("PACKET_NORMALHYPERSURFACES") = PacketType::NormalHypersurfaces;
    m.attr("PACKET_SNAPPEATRIANGULATION") = PacketType::SnapPea;
    m.attr("PACKET_LINK") = PacketType::Link;
    m.attr("PACKET_TRIANGULATION2") = PacketType::Triangulation2;
    m.attr("PACKET_TRIANGULATION3") = PacketType::Triangulation3;
    m.attr("PACKET_TRIANGULATION4") = PacketType::Triangulation4;
    m.attr("PACKET_TRIANGULATION5") = PacketType::Triangulation5;
    m.attr("PACKET_TRIANGULATION6") = PacketType::Triangulation6;
    m.attr("PACKET_TRIANGULATION7") = PacketType::Triangulation7;
    m.attr("PACKET_TRIANGULATION8") = PacketType::Triangulation8;
#ifdef REGINA_HIGHDIM
    m.attr("PACKET_TRIANGULATION9") = PacketType::Triangulation9;
    m.attr("PACKET_TRIANGULATION10") = PacketType::Triangulation10;
    m.attr("PACKET_TRIANGULATION11") = PacketType::Triangulation11;
    m.attr("PACKET_TRIANGULATION12") = PacketType::Triangulation12;
    m.attr("PACKET_TRIANGULATION13") = PacketType::Triangulation13;
    m.attr("PACKET_TRIANGULATION14") = PacketType::Triangulation14;
    m.attr("PACKET_TRIANGULATION15") = PacketType::Triangulation15;
#endif

    RDOC_SCOPE_SWITCH(PacketInfo)

    auto i = pybind11::class_<regina::PacketInfo>(m, "PacketInfo", rdoc_scope)
        .def_static("name", &regina::PacketInfo::name, rdoc::name)
        ;
    regina::python::no_eq_static(i);

    RDOC_SCOPE_END
}

