
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "manifold/sfs.h"
#include "subcomplex/satregion-impl.h"
#include <iostream>
#include "../helpers.h"
#include "../docstrings/subcomplex/satregion.h"

using regina::SatBlock;
using regina::SatBlockSpec;
using regina::SatRegion;

namespace {
}

void addSatRegion(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SatBlockSpec)

    auto s = pybind11::class_<SatBlockSpec>(m, "SatBlockSpec", rdoc_scope)
        .def("swap", &SatBlockSpec::swap, rdoc::swap)
        .def("block", &SatBlockSpec::block,
            pybind11::return_value_policy::reference_internal, rdoc::block)
        .def("refVert", &SatBlockSpec::refVert, rdoc::refVert)
        .def("refHoriz", &SatBlockSpec::refHoriz, rdoc::refHoriz)
    ;
    regina::python::add_output(s);
    regina::python::add_eq_operators(s, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<SatBlockSpec>(m, rdoc::global_swap);

    RDOC_SCOPE_SWITCH(SatRegion)

    auto r = pybind11::class_<SatRegion>(m, "SatRegion", rdoc_scope)
        .def(pybind11::init<const SatRegion&>(), rdoc::__copy)
        .def("swap", &SatRegion::swap, rdoc::swap)
        .def("countBlocks", &SatRegion::countBlocks, rdoc::countBlocks)
        .def("block", &SatRegion::block,
            pybind11::return_value_policy::reference_internal, rdoc::block)
        .def("blockIndex", &SatRegion::blockIndex, rdoc::blockIndex)
        .def("countBoundaryAnnuli", &SatRegion::countBoundaryAnnuli,
            rdoc::countBoundaryAnnuli)
        .def("boundaryAnnulus", &SatRegion::boundaryAnnulus,
            pybind11::return_value_policy::reference_internal,
            rdoc::boundaryAnnulus)
        .def("createSFS", &SatRegion::createSFS, rdoc::createSFS)
        .def("blockAbbrs", &SatRegion::blockAbbrs,
            pybind11::arg("tex") = false, rdoc::blockAbbrs)
        .def_static("find", [](regina::Triangulation<3>& tri, bool complete,
                const std::function<bool(std::unique_ptr<SatRegion>)>& action) {
            // We need to strip out any reference to the TetList argument.
            return SatRegion::find(tri, complete,
                    [&](std::unique_ptr<SatRegion> r, SatBlock::TetList&) {
                return action(std::move(r));
            });
        }, pybind11::arg("tri"),
            pybind11::arg("mustBeComplete"),
            pybind11::arg("action"),
            rdoc::find)
        .def_static("beginsRegion", [](const regina::SatAnnulus& a) {
            SatBlock::TetList avoidTets;
            return SatRegion::beginsRegion(a, avoidTets);
        }, rdoc::beginsRegion)
    ;
    regina::python::add_output(r);
    regina::python::add_eq_operators(r, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<SatRegion>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

