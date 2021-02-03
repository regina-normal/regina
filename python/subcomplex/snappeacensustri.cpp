
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
#include "subcomplex/snappeacensustri.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::SnapPeaCensusTri;

void addSnapPeaCensusTri(pybind11::module_& m) {
    auto c = pybind11::class_<SnapPeaCensusTri, regina::StandardTriangulation>
            (m, "SnapPeaCensusTri")
        .def("clone", &SnapPeaCensusTri::clone)
        .def("section", &SnapPeaCensusTri::section)
        .def("index", &SnapPeaCensusTri::index)
        .def_static("isSmallSnapPeaCensusTri",
            &SnapPeaCensusTri::isSmallSnapPeaCensusTri)
        // On some systems we cannot take addresses of the following
        // inline class constants (e.g., this fails with gcc10 on windows).
        // We therefore define getter functions instead.
        .def_property_readonly_static("SEC_5", [](pybind11::object) {
            return SnapPeaCensusTri::SEC_5;
        })
        .def_property_readonly_static("SEC_6_OR", [](pybind11::object) {
            return SnapPeaCensusTri::SEC_6_OR;
        })
        .def_property_readonly_static("SEC_6_NOR", [](pybind11::object) {
            return SnapPeaCensusTri::SEC_6_NOR;
        })
        .def_property_readonly_static("SEC_7_OR", [](pybind11::object) {
            return SnapPeaCensusTri::SEC_7_OR;
        })
        .def_property_readonly_static("SEC_7_NOR", [](pybind11::object) {
            return SnapPeaCensusTri::SEC_7_NOR;
        })
    ;
    // The SnapPeaCensusTri subclass defines its own equality tests, so
    // we should override the compare-by-pointer test that we inherit from
    // StandardTriangulation.
    regina::python::add_eq_operators(c);
}

