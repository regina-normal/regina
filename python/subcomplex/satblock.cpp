
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
#include "../pybind11/operators.h"
#include "../pybind11/stl.h"
#include "manifold/sfs.h"
#include "subcomplex/satblock.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/satblock.h"

using regina::SatBlock;
using regina::SatBlockModel;

void addSatBlock(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SatBlock)

    auto c = pybind11::class_<SatBlock>(m, "SatBlock", rdoc_scope)
        .def("countAnnuli", &SatBlock::countAnnuli, rdoc::countAnnuli)
        .def("annulus", &SatBlock::annulus,
            pybind11::return_value_policy::reference_internal, rdoc::annulus)
        .def("twistedBoundary", &SatBlock::twistedBoundary,
            rdoc::twistedBoundary)
        .def("hasAdjacentBlock", &SatBlock::hasAdjacentBlock,
            rdoc::hasAdjacentBlock)
        .def("adjacentBlock", &SatBlock::adjacentBlock,
            pybind11::return_value_policy::reference, rdoc::adjacentBlock)
        .def("adjacentAnnulus", &SatBlock::adjacentAnnulus,
            rdoc::adjacentAnnulus)
        .def("adjacentReflected", &SatBlock::adjacentReflected,
            rdoc::adjacentReflected)
        .def("adjacentBackwards", &SatBlock::adjacentBackwards,
            rdoc::adjacentBackwards)
        .def("adjustSFS", &SatBlock::adjustSFS, rdoc::adjustSFS)
        .def("nextBoundaryAnnulus", &SatBlock::nextBoundaryAnnulus,
            pybind11::return_value_policy::reference, rdoc::nextBoundaryAnnulus)
        .def("abbr", &SatBlock::abbr,
            pybind11::arg("tex") = false, rdoc::abbr)
        // We cannot bind the < operator in the normal way:
        // see https://github.com/pybind/pybind11/issues/1487 for details.
        .def("__lt__", [](const SatBlock& lhs, const SatBlock& rhs) {
            return lhs < rhs;
        }, rdoc::__lt)
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_SWITCH(SatBlockModel)

    auto d = pybind11::class_<SatBlockModel>(m, "SatBlockModel", rdoc_scope)
        .def(pybind11::init<const SatBlockModel&>(), rdoc::__copy)
        .def("swap", &SatBlockModel::swap, rdoc::swap)
        .def("triangulation", &SatBlockModel::triangulation,
            rdoc::triangulation)
        .def("block", &SatBlockModel::block, rdoc::block)
    ;
    regina::python::add_output(d);
    regina::python::add_eq_operators(d, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<SatBlockModel>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

