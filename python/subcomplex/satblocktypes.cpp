
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include "subcomplex/layeredsolidtorus.h"
#include "subcomplex/satblocktypes.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/satblocktypes.h"

using regina::SatBlock;
using regina::SatCube;
using regina::SatLayering;
using regina::SatLST;
using regina::SatMobius;
using regina::SatReflectorStrip;
using regina::SatTriPrism;

void addSatBlockTypes(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SatMobius)

    auto mob = pybind11::class_<SatMobius, regina::SatBlock>(m, "SatMobius",
            rdoc::__class)
        .def("position", &SatMobius::position, rdoc::position)
    ;
    regina::python::add_output_rich(mob);

    RDOC_SCOPE_SWITCH(SatLST)

    auto lst = pybind11::class_<SatLST, regina::SatBlock>(m, "SatLST",
            rdoc::__class)
        .def("lst", &SatLST::lst,
            pybind11::return_value_policy::reference_internal, rdoc::lst)
        .def("roles", &SatLST::roles, rdoc::roles)
    ;
    regina::python::add_output_rich(lst);

    RDOC_SCOPE_SWITCH(SatTriPrism)

    auto tri = pybind11::class_<SatTriPrism, regina::SatBlock>(m, "SatTriPrism",
            rdoc::__class)
        .def("isMajor", &SatTriPrism::isMajor, rdoc::isMajor)
        .def_static("model", &SatTriPrism::model, rdoc::model)
    ;
    regina::python::add_output_rich(tri);

    RDOC_SCOPE_SWITCH(SatCube)

    auto cube = pybind11::class_<SatCube, regina::SatBlock>(m, "SatCube",
            rdoc::__class)
        .def_static("model", &SatCube::model, rdoc::model)
    ;
    regina::python::add_output_rich(cube);

    RDOC_SCOPE_SWITCH(SatReflectorStrip)

    auto ref = pybind11::class_<SatReflectorStrip, regina::SatBlock>(
            m, "SatReflectorStrip", rdoc::__class)
        .def_static("model", &SatReflectorStrip::model, rdoc::model)
    ;
    regina::python::add_output_rich(ref);

    RDOC_SCOPE_SWITCH(SatLayering)

    auto l = pybind11::class_<SatLayering, regina::SatBlock>(m, "SatLayering",
            rdoc::__class)
        .def("overHorizontal", &SatLayering::overHorizontal,
            rdoc::overHorizontal)
    ;
    regina::python::add_output_rich(l);

    RDOC_SCOPE_END
}

