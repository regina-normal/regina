
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

#define WRAP_PACKET_TYPE(val) .value(#val, regina::val, rdoc::val)

void addPacketType(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(PacketType)

    pybind11::enum_<regina::PacketType>(m, "PacketType", rdoc_scope)
        WRAP_PACKET_TYPE(PACKET_NONE)
        WRAP_PACKET_TYPE(PACKET_CONTAINER)
        WRAP_PACKET_TYPE(PACKET_TEXT)
        WRAP_PACKET_TYPE(PACKET_SCRIPT)
        WRAP_PACKET_TYPE(PACKET_ATTACHMENT)
        WRAP_PACKET_TYPE(PACKET_ANGLESTRUCTURES)
        WRAP_PACKET_TYPE(PACKET_NORMALSURFACES)
        WRAP_PACKET_TYPE(PACKET_SURFACEFILTER)
        WRAP_PACKET_TYPE(PACKET_NORMALHYPERSURFACES)
        WRAP_PACKET_TYPE(PACKET_SNAPPEATRIANGULATION)
        WRAP_PACKET_TYPE(PACKET_LINK)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION2)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION3)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION4)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION5)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION6)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION7)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION8)
#ifdef REGINA_HIGHDIM
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION9)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION10)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION11)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION12)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION13)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION14)
        WRAP_PACKET_TYPE(PACKET_TRIANGULATION15)
#endif
        .export_values();

    RDOC_SCOPE_SWITCH(PacketInfo)

    auto i = pybind11::class_<regina::PacketInfo>(m, "PacketInfo", rdoc_scope)
        .def_static("name", &regina::PacketInfo::name, rdoc::name)
        ;
    regina::python::no_eq_static(i);

    RDOC_SCOPE_END
}

