
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
#include "triangulation/dim3.h"
#include "triangulation/dim2.h" // for build()
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/boundarycomponent.h"

using namespace pybind11::literals;

using regina::BoundaryComponent;

void addBoundaryComponent3(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(BoundaryComponent)

    auto c = pybind11::class_<BoundaryComponent<3>>(m, "BoundaryComponent3",
            rdoc::BoundaryComponent::__class)
        .def("index", &BoundaryComponent<3>::index, rdoc::index)
        .def("size", &BoundaryComponent<3>::size, rdoc::size)
        .def("countRidges", &BoundaryComponent<3>::countRidges,
            rdoc::countRidges)
        .def("countFaces",
            (regina::python::countFacesFunc<BoundaryComponent<3>>)(
                &BoundaryComponent<3>::countFaces),
            rdoc::countFaces)
        .def("countTriangles", &BoundaryComponent<3>::countTriangles,
            rdoc::countTriangles)
        .def("countEdges", &BoundaryComponent<3>::countEdges, rdoc::countEdges)
        .def("countVertices", &BoundaryComponent<3>::countVertices,
            rdoc::countVertices)
        .def("facets", &BoundaryComponent<3>::facets, rdoc::facets)
        .def("faces", (regina::python::facesFunc<BoundaryComponent<3>>)(
                &BoundaryComponent<3>::faces),
            "subdim"_a, rdoc::faces)
        .def("triangles", &BoundaryComponent<3>::triangles, rdoc::triangles)
        .def("edges", &BoundaryComponent<3>::edges, rdoc::edges)
        .def("vertices", &BoundaryComponent<3>::vertices, rdoc::vertices)
        .def("facet", &BoundaryComponent<3>::facet,
            pybind11::return_value_policy::reference, rdoc::facet)
        .def("face", (regina::python::faceFunc<BoundaryComponent<3>>)(
                &BoundaryComponent<3>::face),
            pybind11::return_value_policy::reference,
            "subdim"_a, "index"_a, rdoc::face)
        .def("triangle", &BoundaryComponent<3>::triangle,
            pybind11::return_value_policy::reference, rdoc::triangle)
        .def("edge", &BoundaryComponent<3>::edge,
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("vertex", &BoundaryComponent<3>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("component", &BoundaryComponent<3>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("triangulation", &BoundaryComponent<3>::triangulation,
            rdoc::triangulation)
        .def("build", [](const BoundaryComponent<3>& b) {
            // Return a clone of the resulting triangulation.
            // This is because Python cannot enforce the constness of
            // the reference that would normally be returned.
            return new regina::Triangulation<2>(b.build());
        }, rdoc::build)
        .def("eulerChar", &BoundaryComponent<3>::eulerChar, rdoc::eulerChar)
        .def("isReal", &BoundaryComponent<3>::isReal, rdoc::isReal)
        .def("isIdeal", &BoundaryComponent<3>::isIdeal, rdoc::isIdeal)
        .def("isInvalidVertex", &BoundaryComponent<3>::isInvalidVertex,
            rdoc::isInvalidVertex)
        .def("isOrientable", &BoundaryComponent<3>::isOrientable,
            rdoc::isOrientable)
        .def_readonly_static("dimension", &BoundaryComponent<3>::dimension)
        .def_readonly_static("allFaces", &BoundaryComponent<3>::allFaces)
        .def_readonly_static("allowVertex", &BoundaryComponent<3>::allowVertex)
        .def_readonly_static("canBuild", &BoundaryComponent<3>::canBuild)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addStdView<
        decltype(std::declval<BoundaryComponent<3>>().vertices())>(internal,
        "BoundaryComponent3_vertices");
    regina::python::addStdView<
        decltype(std::declval<BoundaryComponent<3>>().edges())>(internal,
        "BoundaryComponent3_edges");
    regina::python::addStdView<
        decltype(std::declval<BoundaryComponent<3>>().triangles())>(internal,
        "BoundaryComponent3_triangles");
}

