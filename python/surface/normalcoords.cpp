
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
#include "../pybind11/pybind11.h"
#include "../pybind11/operators.h"
#include "surface/normalcoords.h"
#include "../helpers.h"
#include "../docstrings/surface/normalcoords.h"

using regina::NormalCoords;
using regina::NormalEncoding;
using regina::NormalInfo;

void addNormalCoords(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(NormalCoords)

    pybind11::enum_<NormalCoords>(m, "NormalCoords", rdoc_scope)
        .value("Standard", NormalCoords::Standard, rdoc::Standard)
        .value("Quad", NormalCoords::Quad, rdoc::Quad)
        .value("QuadClosed", NormalCoords::QuadClosed, rdoc::QuadClosed)
        .value("AlmostNormal", NormalCoords::AlmostNormal, rdoc::AlmostNormal)
        .value("LegacyAlmostNormal", NormalCoords::LegacyAlmostNormal,
            rdoc::LegacyAlmostNormal)
        .value("QuadOct", NormalCoords::QuadOct, rdoc::QuadOct)
        .value("QuadOctClosed", NormalCoords::QuadOctClosed,
            rdoc::QuadOctClosed)
        .value("Edge", NormalCoords::Edge, rdoc::Edge)
        .value("Arc", NormalCoords::Arc, rdoc::Arc)
        .value("Angle", NormalCoords::Angle, rdoc::Angle)
        ;

    m.attr("NS_STANDARD") = NormalCoords::Standard;
    m.attr("NS_QUAD") = NormalCoords::Quad;
    m.attr("NS_QUAD_CLOSED") = NormalCoords::QuadClosed;
    m.attr("NS_AN_STANDARD") = NormalCoords::AlmostNormal;
    m.attr("NS_AN_LEGACY") = NormalCoords::LegacyAlmostNormal;
    m.attr("NS_AN_QUAD_OCT") = NormalCoords::QuadOct;
    m.attr("NS_AN_QUAD_OCT_CLOSED") = NormalCoords::QuadOctClosed;
    m.attr("NS_EDGE_WEIGHT") = NormalCoords::Edge;
    m.attr("NS_TRIANGLE_ARCS") = NormalCoords::Arc;
    m.attr("NS_ANGLE") = NormalCoords::Angle;

    RDOC_SCOPE_SWITCH(NormalEncoding)

    auto e = pybind11::class_<NormalEncoding>(m, "NormalEncoding", rdoc_scope)
        .def(pybind11::init<NormalCoords>(), rdoc::__init)
        .def(pybind11::init<const NormalEncoding&>(), rdoc::__copy)
        .def("valid", &NormalEncoding::valid, rdoc::valid)
        .def("block", &NormalEncoding::block, rdoc::block)
        .def("storesTriangles", &NormalEncoding::storesTriangles,
            rdoc::storesTriangles)
        .def("storesOctagons", &NormalEncoding::storesOctagons,
            rdoc::storesOctagons)
        .def("storesAngles", &NormalEncoding::storesAngles, rdoc::storesAngles)
        .def("couldBeVertexLink", &NormalEncoding::couldBeVertexLink,
            rdoc::couldBeVertexLink)
        .def("couldBeNonCompact", &NormalEncoding::couldBeNonCompact,
            rdoc::couldBeNonCompact)
        .def("withTriangles", &NormalEncoding::withTriangles,
            rdoc::withTriangles)
        .def("withoutTriangles", &NormalEncoding::withoutTriangles,
            rdoc::withoutTriangles)
        .def("withOctagons", &NormalEncoding::withOctagons, rdoc::withOctagons)
        .def("withoutOctagons", &NormalEncoding::withoutOctagons,
            rdoc::withoutOctagons)
        .def("intValue", &NormalEncoding::intValue, rdoc::intValue)
        .def_static("fromIntValue", &NormalEncoding::fromIntValue,
            rdoc::fromIntValue)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def_static("empty", &NormalEncoding::empty, rdoc::empty)
        .def("__str__", [](NormalEncoding e) {
            std::ostringstream out;
            out << "0x" << std::hex << std::setw(4) << std::setfill('0')
                << e.intValue();
            return out.str();
        })
        .def("__repr__", [](NormalEncoding e) {
            std::ostringstream out;
            out << "<regina.NormalEncoding: 0x" << std::hex << std::setw(4)
                << std::setfill('0') << e.intValue() << '>';
            return out.str();
        })
        ;
    regina::python::add_eq_operators(e, rdoc::__eq);

    RDOC_SCOPE_SWITCH(NormalInfo)

    auto i = pybind11::class_<NormalInfo>(m, "NormalInfo", rdoc_scope)
        .def_static("name", &NormalInfo::name, rdoc::name)
        ;
    regina::python::no_eq_static(i);

    RDOC_SCOPE_END

    pybind11::implicitly_convertible<NormalCoords, NormalEncoding>();
}

