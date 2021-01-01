
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
#include "../pybind11/operators.h"
#include "manifold/sfs.h"
#include "subcomplex/satblock.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::SatAnnulus;
using regina::SatBlock;

void addSatBlock(pybind11::module_& m) {
    auto c = pybind11::class_<SatBlock>(m, "SatBlock")
        .def("clone", &SatBlock::clone)
        .def("nAnnuli", &SatBlock::nAnnuli)
        .def("annulus", &SatBlock::annulus,
            pybind11::return_value_policy::reference_internal)
        .def("twistedBoundary", &SatBlock::twistedBoundary)
        .def("hasAdjacentBlock", &SatBlock::hasAdjacentBlock)
        .def("adjacentBlock", &SatBlock::adjacentBlock,
            pybind11::return_value_policy::reference)
        .def("adjacentAnnulus", &SatBlock::adjacentAnnulus)
        .def("adjacentReflected", &SatBlock::adjacentReflected)
        .def("adjacentBackwards", &SatBlock::adjacentBackwards)
        .def("setAdjacent", &SatBlock::setAdjacent)
        .def("adjustSFS", &SatBlock::adjustSFS)
        .def("transform", &SatBlock::transform)
        .def("nextBoundaryAnnulus", [](SatBlock& b, unsigned a, bool fromPrev) {
            SatBlock* nextBlock;
            unsigned nextAnnulus;
            bool refVert, refHoriz;

            b.nextBoundaryAnnulus(a, nextBlock, nextAnnulus, refVert, refHoriz,
                fromPrev);
            return pybind11::make_tuple(nextBlock, nextAnnulus, refVert,
                refHoriz);
        }, pybind11::return_value_policy::reference)
        .def("abbr", &SatBlock::abbr,
            pybind11::arg("tex") = false)
        .def("writeAbbr", [](const SatBlock& b, bool tex) {
            b.writeAbbr(std::cout, tex);
        }, pybind11::arg("tex") = false)
        // We cannot bind the < operator in the normal way:
        // see https://github.com/pybind/pybind11/issues/1487 for details.
        .def("__lt__", [](const SatBlock& lhs, const SatBlock& rhs) {
            return lhs < rhs;
        })
        .def_static("isBlock", [](const SatAnnulus& a) {
            SatBlock::TetList avoidTets;
            return SatBlock::isBlock(a, avoidTets);
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("NSatBlock") = m.attr("SatBlock");
}

