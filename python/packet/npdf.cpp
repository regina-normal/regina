
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include "packet/npdf.h"
#include "../safeheldtype.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using regina::python::SafeHeldType;
using regina::NPDF;

namespace {
    void (NPDF::*reset_empty)() = &NPDF::reset;
}

void addNPDF() {
    scope s = class_<NPDF, bases<regina::NPacket>,
            SafeHeldType<NPDF>, boost::noncopyable>("NPDF", init<>())
        .def(init<const char*>())
        .def("isNull", &NPDF::isNull)
        .def("size", &NPDF::size)
        .def("reset", reset_empty)
        .def("savePDF", &NPDF::savePDF)
    ;

    s.attr("typeID") = regina::PACKET_PDF;
    s.attr("packetType") = regina::PACKET_PDF;

    implicitly_convertible<SafeHeldType<NPDF>,
        SafeHeldType<regina::NPacket> >();
}

