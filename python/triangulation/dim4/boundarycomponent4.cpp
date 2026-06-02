
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
#include "triangulation/dim4.h"
#include "triangulation/dim3.h" // for build()
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/boundarycomponent.h"

using namespace pybind11::literals;

using regina::BoundaryComponent;

void addBoundaryComponent4(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(BoundaryComponent)

    auto c = pybind11::class_<BoundaryComponent<4>>(m, "BoundaryComponent4",
            rdoc::BoundaryComponent::__class)
        .def("index", &BoundaryComponent<4>::index, rdoc::index)
        .def("size", &BoundaryComponent<4>::size, rdoc::size)
        .def("countRidges", &BoundaryComponent<4>::countRidges,
            rdoc::countRidges)
        .def("countFaces",
            (regina::python::countFacesFunc<BoundaryComponent<4>>)(
                &BoundaryComponent<4>::countFaces),
            rdoc::countFaces)
        .def("countTetrahedra",
            &BoundaryComponent<4>::countTetrahedra, rdoc::countTetrahedra)
        .def("countTriangles", &BoundaryComponent<4>::countTriangles,
            rdoc::countTriangles)
        .def("countEdges", &BoundaryComponent<4>::countEdges, rdoc::countEdges)
        .def("countVertices", &BoundaryComponent<4>::countVertices,
            rdoc::countVertices)
        .def("facets", &BoundaryComponent<4>::facets, rdoc::facets)
        .def("faces", (regina::python::facesFunc<BoundaryComponent<4>>)(
                &BoundaryComponent<4>::faces),
            "subdim"_a, rdoc::faces)
        .def("tetrahedra", &BoundaryComponent<4>::tetrahedra, rdoc::tetrahedra)
        .def("triangles", &BoundaryComponent<4>::triangles, rdoc::triangles)
        .def("edges", &BoundaryComponent<4>::edges, rdoc::edges)
        .def("vertices", &BoundaryComponent<4>::vertices, rdoc::vertices)
        .def("facet", &BoundaryComponent<4>::facet,
            pybind11::return_value_policy::reference, rdoc::facet)
        .def("face", (regina::python::faceFunc<BoundaryComponent<4>>)(
                &BoundaryComponent<4>::face),
            pybind11::return_value_policy::reference,
            "subdim"_a, "index"_a, rdoc::face)
        .def("tetrahedron", &BoundaryComponent<4>::tetrahedron,
            pybind11::return_value_policy::reference, rdoc::tetrahedron)
        .def("triangle", &BoundaryComponent<4>::triangle,
            pybind11::return_value_policy::reference, rdoc::triangle)
        .def("edge", &BoundaryComponent<4>::edge,
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("vertex", &BoundaryComponent<4>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("component", &BoundaryComponent<4>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("triangulation", &BoundaryComponent<4>::triangulation,
            rdoc::triangulation)
        .def("build", [](const BoundaryComponent<4>& b) {
            // Return a clone of the resulting triangulation.
            // This is because Python cannot enforce the constness of
            // the reference that would normally be returned.
            return new regina::Triangulation<3>(b.build());
        }, rdoc::build)
        .def("eulerChar", &BoundaryComponent<4>::eulerChar, rdoc::eulerChar)
        .def("isReal", &BoundaryComponent<4>::isReal, rdoc::isReal)
        .def("isIdeal", &BoundaryComponent<4>::isIdeal, rdoc::isIdeal)
        .def("isInvalidVertex", &BoundaryComponent<4>::isInvalidVertex,
            rdoc::isInvalidVertex)
        .def("isOrientable", &BoundaryComponent<4>::isOrientable,
            rdoc::isOrientable)
        .def_readonly_static("dimension", &BoundaryComponent<4>::dimension)
        .def_readonly_static("allFaces", &BoundaryComponent<4>::allFaces)
        .def_readonly_static("allowVertex", &BoundaryComponent<4>::allowVertex)
        .def_readonly_static("canBuild", &BoundaryComponent<4>::canBuild)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addStdView<
        decltype(std::declval<BoundaryComponent<4>>().vertices())>(internal,
        "BoundaryComponent4_vertices");
    regina::python::addStdView<
        decltype(std::declval<BoundaryComponent<4>>().edges())>(internal,
        "BoundaryComponent4_edges");
    regina::python::addStdView<
        decltype(std::declval<BoundaryComponent<4>>().triangles())>(internal,
        "BoundaryComponent4_triangles");
    regina::python::addStdView<
        decltype(std::declval<BoundaryComponent<4>>().tetrahedra())>(internal,
        "BoundaryComponent4_tetrahedra");
}

