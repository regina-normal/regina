
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
#include "hypersurface/hypercoords.h"
#include "../helpers.h"

using regina::HyperCoords;
using regina::HyperEncoding;
using regina::HyperInfo;

void addHyperCoords(pybind11::module_& m) {
    pybind11::enum_<regina::HyperCoords>(m, "HyperCoords")
        .value("HS_STANDARD", regina::HS_STANDARD)
        .value("HS_PRISM", regina::HS_PRISM)
        .value("HS_EDGE_WEIGHT", regina::HS_EDGE_WEIGHT)
        .export_values()
        ;

    auto e = pybind11::class_<HyperEncoding>(m, "HyperEncoding")
        .def(pybind11::init<HyperCoords>())
        .def(pybind11::init<const HyperEncoding&>())
        .def("valid", &HyperEncoding::valid)
        .def("block", &HyperEncoding::block)
        .def("storesTetrahedra", &HyperEncoding::storesTetrahedra)
        .def("couldBeVertexLink", &HyperEncoding::couldBeVertexLink)
        .def("couldBeNonCompact", &HyperEncoding::couldBeNonCompact)
        .def("withTetrahedra", &HyperEncoding::withTetrahedra)
        .def("intValue", &HyperEncoding::intValue)
        .def_static("fromIntValue", &HyperEncoding::fromIntValue)
        .def(pybind11::self + pybind11::self)
        ;
    regina::python::add_eq_operators(e);

    auto i = pybind11::class_<HyperInfo>(m, "HyperInfo")
        .def_static("name", &HyperInfo::name)
        ;
    regina::python::no_eq_operators(i);
}

