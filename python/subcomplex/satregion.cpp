
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "manifold/sfs.h"
#include "subcomplex/satregion-impl.h"
#include <iostream>
#include "../helpers.h"

using regina::SatBlock;
using regina::SatBlockSpec;
using regina::SatRegion;

namespace {
}

void addSatRegion(pybind11::module_& m) {
    auto s = pybind11::class_<SatBlockSpec>(m, "SatBlockSpec")
        .def("block", &SatBlockSpec::block,
            pybind11::return_value_policy::reference_internal)
        .def("refVert", &SatBlockSpec::refVert)
        .def("refHoriz", &SatBlockSpec::refHoriz)
    ;
    regina::python::add_eq_operators(s);

    auto r = pybind11::class_<SatRegion>(m, "SatRegion")
        .def(pybind11::init<const SatRegion&>())
        .def("swap", &SatRegion::swap)
        .def("countBlocks", &SatRegion::countBlocks)
        .def("numberOfBlocks", &SatRegion::countBlocks) // deprecated
        .def("block", &SatRegion::block,
            pybind11::return_value_policy::reference_internal)
        .def("blockIndex", &SatRegion::blockIndex)
        .def("countBoundaryAnnuli", &SatRegion::countBoundaryAnnuli)
        .def("numberOfBoundaryAnnuli", // deprecated
            &SatRegion::countBoundaryAnnuli)
        .def("boundaryAnnulus", &SatRegion::boundaryAnnulus,
            pybind11::return_value_policy::reference_internal)
        .def("createSFS", &SatRegion::createSFS)
        .def("blockAbbrs", &SatRegion::blockAbbrs,
            pybind11::arg("tex") = false)
        .def_static("find", [](regina::Triangulation<3>& tri, bool complete,
                const std::function<bool(std::unique_ptr<SatRegion>)>& action) {
            // We need to strip out any reference to the TetList argument.
            return SatRegion::find(tri, complete,
                    [&](std::unique_ptr<SatRegion> r, SatBlock::TetList&) {
                return action(std::move(r));
            });
        })
        .def_static("beginsRegion", [](const regina::SatAnnulus& a) {
            SatBlock::TetList avoidTets;
            return SatRegion::beginsRegion(a, avoidTets);
        })
    ;
    regina::python::add_output(r, regina::python::PYTHON_REPR_NONE);
    regina::python::add_eq_operators(r);

    m.def("swap", (void(*)(SatRegion&, SatRegion&))(regina::swap));
}

