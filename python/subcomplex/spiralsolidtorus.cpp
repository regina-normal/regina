
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
#include "subcomplex/spiralsolidtorus.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::SpiralSolidTorus;

void addSpiralSolidTorus(pybind11::module_& m) {
    pybind11::class_<SpiralSolidTorus, regina::StandardTriangulation>
            (m, "SpiralSolidTorus")
        .def("clone", &SpiralSolidTorus::clone)
        .def("size", &SpiralSolidTorus::size)
        .def("tetrahedron", &SpiralSolidTorus::tetrahedron,
            pybind11::return_value_policy::reference)
        .def("vertexRoles", &SpiralSolidTorus::vertexRoles)
        .def("reverse", &SpiralSolidTorus::reverse)
        .def("cycle", &SpiralSolidTorus::cycle)
        .def("makeCanonical", &SpiralSolidTorus::makeCanonical)
        .def("isCanonical", &SpiralSolidTorus::isCanonical)
        .def_static("formsSpiralSolidTorus",
            &SpiralSolidTorus::formsSpiralSolidTorus)
    ;

    m.attr("NSpiralSolidTorus") = m.attr("SpiralSolidTorus");
}

