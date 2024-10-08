
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

#include <iomanip>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "hypersurface/hypercoords.h"
#include "../helpers.h"
#include "../docstrings/hypersurface/hypercoords.h"

using regina::HyperCoords;
using regina::HyperEncoding;
using regina::HyperInfo;

void addHyperCoords(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(HyperCoords)

    pybind11::enum_<regina::HyperCoords>(m, "HyperCoords", rdoc_scope)
        .value("Standard", HyperCoords::Standard, rdoc::Standard)
        .value("Prism", HyperCoords::Prism, rdoc::Prism)
        .value("Edge", HyperCoords::Edge, rdoc::Edge)
        ;

    m.attr("HS_STANDARD") = HyperCoords::Standard;
    m.attr("HS_PRISM") = HyperCoords::Prism;
    m.attr("HS_EDGE_WEIGHT") = HyperCoords::Edge;

    RDOC_SCOPE_SWITCH(HyperEncoding)

    auto e = pybind11::class_<HyperEncoding>(m, "HyperEncoding", rdoc_scope)
        .def(pybind11::init<HyperCoords>(), rdoc::__init)
        .def(pybind11::init<const HyperEncoding&>(), rdoc::__copy)
        .def("valid", &HyperEncoding::valid, rdoc::valid)
        .def("block", &HyperEncoding::block, rdoc::block)
        .def("storesTetrahedra", &HyperEncoding::storesTetrahedra,
            rdoc::storesTetrahedra)
        .def("couldBeVertexLink", &HyperEncoding::couldBeVertexLink,
            rdoc::couldBeVertexLink)
        .def("couldBeNonCompact", &HyperEncoding::couldBeNonCompact,
            rdoc::couldBeNonCompact)
        .def("withTetrahedra", &HyperEncoding::withTetrahedra,
            rdoc::withTetrahedra)
        .def("intValue", &HyperEncoding::intValue, rdoc::intValue)
        .def_static("fromIntValue", &HyperEncoding::fromIntValue,
            rdoc::fromIntValue)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def_static("empty", &HyperEncoding::empty, rdoc::empty)
        .def("__str__", [](HyperEncoding e) {
            std::ostringstream out;
            out << "0x" << std::hex << std::setw(4) << std::setfill('0')
                << e.intValue();
            return out.str();
        })
        .def("__repr__", [](HyperEncoding e) {
            std::ostringstream out;
            out << "<regina.HyperEncoding: 0x" << std::hex << std::setw(4)
                << std::setfill('0') << e.intValue() << '>';
            return out.str();
        })
        ;
    regina::python::add_eq_operators(e, rdoc::__eq);

    RDOC_SCOPE_SWITCH(HyperInfo)

    auto i = pybind11::class_<HyperInfo>(m, "HyperInfo", rdoc_scope)
        .def_static("name", &HyperInfo::name, rdoc::name)
        ;
    regina::python::no_eq_static(i);

    RDOC_SCOPE_END
}

