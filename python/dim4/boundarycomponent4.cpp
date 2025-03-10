
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "triangulation/dim4.h"
#include "triangulation/dim3.h" // for build()
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/generic/boundarycomponent.h"
#include "../docstrings/triangulation/detail/boundarycomponent.h"

using regina::BoundaryComponent;

void addBoundaryComponent4(pybind11::module_& m) {
    // We use the global scope here because all of BoundaryComponent's members
    // are inherited, and so BoundaryComponent's own docstring namespace
    // does not exist.
    RDOC_SCOPE_BEGIN_MAIN
    RDOC_SCOPE_BASE(detail::BoundaryComponentBase)

    auto c = pybind11::class_<BoundaryComponent<4>>(m, "BoundaryComponent4",
            rdoc::BoundaryComponent)
        .def("index", &BoundaryComponent<4>::index, rbase::index)
        .def("size", &BoundaryComponent<4>::size, rbase::size)
        .def("countRidges", &BoundaryComponent<4>::countRidges,
            rbase::countRidges)
        .def("countFaces",
            &regina::python::countFaces<BoundaryComponent<4>, 4, 3>,
            pybind11::arg("subdim"), rbase::countFaces)
        .def("countTetrahedra",
            &BoundaryComponent<4>::countTetrahedra, rbase::countTetrahedra)
        .def("countTriangles", &BoundaryComponent<4>::countTriangles,
            rbase::countTriangles)
        .def("countEdges", &BoundaryComponent<4>::countEdges, rbase::countEdges)
        .def("countVertices", &BoundaryComponent<4>::countVertices,
            rbase::countVertices)
        .def("facets", &BoundaryComponent<4>::facets, rbase::facets)
        .def("faces", regina::python::faces<BoundaryComponent<4>, 4>,
            pybind11::arg("subdim"), rbase::faces)
        .def("tetrahedra", &BoundaryComponent<4>::tetrahedra, rbase::tetrahedra)
        .def("triangles", &BoundaryComponent<4>::triangles, rbase::triangles)
        .def("edges", &BoundaryComponent<4>::edges, rbase::edges)
        .def("vertices", &BoundaryComponent<4>::vertices, rbase::vertices)
        .def("facet", &BoundaryComponent<4>::facet,
            pybind11::return_value_policy::reference, rbase::facet)
        .def("face", &regina::python::face<BoundaryComponent<4>, 4, size_t>,
            pybind11::arg("subdim"), pybind11::arg("index"), rbase::face)
        .def("tetrahedron", &BoundaryComponent<4>::tetrahedron,
            pybind11::return_value_policy::reference, rbase::tetrahedron)
        .def("triangle", &BoundaryComponent<4>::triangle,
            pybind11::return_value_policy::reference, rbase::triangle)
        .def("edge", &BoundaryComponent<4>::edge,
            pybind11::return_value_policy::reference, rbase::edge)
        .def("vertex", &BoundaryComponent<4>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("component", &BoundaryComponent<4>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("triangulation", &BoundaryComponent<4>::triangulation,
            rbase::triangulation)
        .def("build", [](const BoundaryComponent<4>& b) {
            // Return a clone of the resulting triangulation.
            // This is because Python cannot enforce the constness of
            // the reference that would normally be returned.
            return new regina::Triangulation<3>(b.build());
        }, rbase::build)
        .def("eulerChar", &BoundaryComponent<4>::eulerChar, rbase::eulerChar)
        .def("isReal", &BoundaryComponent<4>::isReal, rbase::isReal)
        .def("isIdeal", &BoundaryComponent<4>::isIdeal, rbase::isIdeal)
        .def("isInvalidVertex", &BoundaryComponent<4>::isInvalidVertex,
            rbase::isInvalidVertex)
        .def("isOrientable", &BoundaryComponent<4>::isOrientable,
            rbase::isOrientable)
        .def_readonly_static("dimension", &BoundaryComponent<4>::dimension)
        .def_readonly_static("allFaces", &BoundaryComponent<4>::allFaces)
        .def_readonly_static("allowVertex", &BoundaryComponent<4>::allowVertex)
        .def_readonly_static("canBuild", &BoundaryComponent<4>::canBuild)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addListView<
        decltype(std::declval<BoundaryComponent<4>>().vertices())>(m);
    regina::python::addListView<
        decltype(std::declval<BoundaryComponent<4>>().edges())>(m);
    regina::python::addListView<
        decltype(std::declval<BoundaryComponent<4>>().triangles())>(m);
    regina::python::addListView<
        decltype(std::declval<BoundaryComponent<4>>().tetrahedra())>(m);
}

