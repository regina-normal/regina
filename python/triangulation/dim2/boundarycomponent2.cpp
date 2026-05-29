
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
#include <pybind11/stl.h>
#include "triangulation/dim2.h"
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/boundarycomponent.h"

using namespace pybind11::literals;

using regina::BoundaryComponent;

void addBoundaryComponent2(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(BoundaryComponent)

    auto c = pybind11::class_<BoundaryComponent<2>>(m, "BoundaryComponent2",
            rdoc::BoundaryComponent::__class)
        .def("index", &BoundaryComponent<2>::index, rdoc::index)
        .def("size", &BoundaryComponent<2>::size, rdoc::size)
        .def("countRidges", &BoundaryComponent<2>::countRidges,
            rdoc::countRidges)
        .def("countFaces",
            (regina::python::countFacesFunc<BoundaryComponent<2>>)(
                &BoundaryComponent<2>::countFaces),
            rdoc::countFaces)
        .def("countEdges", &BoundaryComponent<2>::countEdges, rdoc::countEdges)
        .def("countVertices", &BoundaryComponent<2>::countVertices,
            rdoc::countVertices)
        .def("facets", &BoundaryComponent<2>::facets, rdoc::facets)
        .def("faces", (regina::python::facesFunc<BoundaryComponent<2>>)(
                &BoundaryComponent<2>::faces),
            "subdim"_a, rdoc::faces)
        .def("edges", &BoundaryComponent<2>::edges, rdoc::edges)
        .def("vertices", &BoundaryComponent<2>::vertices, rdoc::vertices)
        .def("facet", &BoundaryComponent<2>::facet,
            pybind11::return_value_policy::reference, rdoc::facet)
        .def("face", (regina::python::faceFunc<BoundaryComponent<2>>)(
                &BoundaryComponent<2>::face),
            pybind11::return_value_policy::reference,
            "subdim"_a, "index"_a, rdoc::face)
        .def("edge", &BoundaryComponent<2>::edge,
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("vertex", &BoundaryComponent<2>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("component", &BoundaryComponent<2>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("triangulation", &BoundaryComponent<2>::triangulation,
            rdoc::triangulation)
        .def("eulerChar", &BoundaryComponent<2>::eulerChar, rdoc::eulerChar)
        .def("isReal", &BoundaryComponent<2>::isReal, rdoc::isReal)
        .def("isIdeal", &BoundaryComponent<2>::isIdeal, rdoc::isIdeal)
        .def("isInvalidVertex", &BoundaryComponent<2>::isInvalidVertex,
            rdoc::isInvalidVertex)
        .def("isOrientable", &BoundaryComponent<2>::isOrientable,
            rdoc::isOrientable)
        .def_readonly_static("dimension", &BoundaryComponent<2>::dimension)
        .def_readonly_static("allFaces", &BoundaryComponent<2>::allFaces)
        .def_readonly_static("allowVertex", &BoundaryComponent<2>::allowVertex)
        .def_readonly_static("canBuild", &BoundaryComponent<2>::canBuild)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addStdView<
        decltype(std::declval<BoundaryComponent<2>>().vertices())>(internal,
        "BoundaryComponent2_vertices");
    regina::python::addStdView<
        decltype(std::declval<BoundaryComponent<2>>().edges())>(internal,
        "BoundaryComponent2_edges");
}

