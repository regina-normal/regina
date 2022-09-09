
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

using regina::SatBlock;
using regina::SatCube;
using regina::SatLayering;
using regina::SatLST;
using regina::SatMobius;
using regina::SatReflectorStrip;
using regina::SatTriPrism;

void addSatBlockTypes(pybind11::module_& m) {
    auto mob = pybind11::class_<SatMobius, regina::SatBlock>(m, "SatMobius")
        .def("position", &SatMobius::position)
    ;
    regina::python::add_output(mob);

    auto lst = pybind11::class_<SatLST, regina::SatBlock>(m, "SatLST")
        .def("lst", &SatLST::lst,
            pybind11::return_value_policy::reference_internal)
        .def("roles", &SatLST::roles)
    ;
    regina::python::add_output(lst);

    auto tri = pybind11::class_<SatTriPrism, regina::SatBlock>(m, "SatTriPrism")
        .def("isMajor", &SatTriPrism::isMajor)
        .def_static("model", &SatTriPrism::model)
    ;
    regina::python::add_output(tri);

    auto cube = pybind11::class_<SatCube, regina::SatBlock>(m, "SatCube")
        .def_static("model", &SatCube::model)
    ;
    regina::python::add_output(cube);

    auto ref = pybind11::class_<SatReflectorStrip, regina::SatBlock>(
            m, "SatReflectorStrip")
        .def_static("model", &SatReflectorStrip::model)
    ;
    regina::python::add_output(ref);

    auto l = pybind11::class_<SatLayering, regina::SatBlock>(m, "SatLayering")
        .def("overHorizontal", &SatLayering::overHorizontal)
    ;
    regina::python::add_output(l);
}

