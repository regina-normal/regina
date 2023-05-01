
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
            rdoc_scope)
        .def("position", &SatMobius::position, rdoc::position)
    ;
    regina::python::add_output(mob);

    RDOC_SCOPE_SWITCH(SatLST)

    auto lst = pybind11::class_<SatLST, regina::SatBlock>(m, "SatLST",
            rdoc_scope)
        .def("lst", &SatLST::lst,
            pybind11::return_value_policy::reference_internal, rdoc::lst)
        .def("roles", &SatLST::roles, rdoc::roles)
    ;
    regina::python::add_output(lst);

    RDOC_SCOPE_SWITCH(SatTriPrism)

    auto tri = pybind11::class_<SatTriPrism, regina::SatBlock>(m, "SatTriPrism",
            rdoc_scope)
        .def("isMajor", &SatTriPrism::isMajor, rdoc::isMajor)
        .def_static("model", &SatTriPrism::model, rdoc::model)
    ;
    regina::python::add_output(tri);

    RDOC_SCOPE_SWITCH(SatCube)

    auto cube = pybind11::class_<SatCube, regina::SatBlock>(m, "SatCube",
            rdoc_scope)
        .def_static("model", &SatCube::model, rdoc::model)
    ;
    regina::python::add_output(cube);

    RDOC_SCOPE_SWITCH(SatReflectorStrip)

    auto ref = pybind11::class_<SatReflectorStrip, regina::SatBlock>(
            m, "SatReflectorStrip", rdoc_scope)
        .def_static("model", &SatReflectorStrip::model, rdoc::model)
    ;
    regina::python::add_output(ref);

    RDOC_SCOPE_SWITCH(SatLayering)

    auto l = pybind11::class_<SatLayering, regina::SatBlock>(m, "SatLayering",
            rdoc_scope)
        .def("overHorizontal", &SatLayering::overHorizontal,
            rdoc::overHorizontal)
    ;
    regina::python::add_output(l);

    RDOC_SCOPE_END
}

