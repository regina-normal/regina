
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
#include "../pybind11/stl.h"
#include "subcomplex/snappeacensustri.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/snappeacensustri.h"

using regina::SnapPeaCensusTri;

void addSnapPeaCensusTri(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SnapPeaCensusTri)

    auto c = pybind11::class_<SnapPeaCensusTri, regina::StandardTriangulation>
            (m, "SnapPeaCensusTri", rdoc_scope)
        .def(pybind11::init<const SnapPeaCensusTri&>(), rdoc::__copy)
        .def("swap", &SnapPeaCensusTri::swap, rdoc::swap)
        .def("section", &SnapPeaCensusTri::section, rdoc::section)
        .def("index", &SnapPeaCensusTri::index, rdoc::index)
        .def_static("recognise", &SnapPeaCensusTri::recognise, rdoc::recognise)
        .def_readonly_static("SEC_5", &SnapPeaCensusTri::SEC_5)
        .def_readonly_static("SEC_6_OR", &SnapPeaCensusTri::SEC_6_OR)
        .def_readonly_static("SEC_6_NOR", &SnapPeaCensusTri::SEC_6_NOR)
        .def_readonly_static("SEC_7_OR", &SnapPeaCensusTri::SEC_7_OR)
        .def_readonly_static("SEC_7_NOR", &SnapPeaCensusTri::SEC_7_NOR)
    ;
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_output(c);

    regina::python::add_global_swap<SnapPeaCensusTri>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

