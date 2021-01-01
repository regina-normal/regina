
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
#include "packet/pdf.h"
#include "utilities/safeptr.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::PDF;

void addPDF(pybind11::module_& m) {
    pybind11::class_<PDF, regina::Packet, regina::SafePtr<PDF>>(m, "PDF")
        .def(pybind11::init<>())
        .def(pybind11::init<const char*>())
        .def("isNull", &PDF::isNull)
        .def("size", &PDF::size)
        .def("reset", overload_cast<>(&PDF::reset))
        .def("savePDF", &PDF::savePDF)
        .def_property_readonly_static("typeID", [](pybind11::object) {
            // We cannot take the address of typeID, so use a getter function.
            return PDF::typeID;
        })
    ;

    m.attr("NPDF") = m.attr("PDF");
}

