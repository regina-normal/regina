
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
#include "../pybind11/stl.h"
#include "triangulation/dim3.h"
#include "triangulation/dim2.h" // for build()
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/generic/boundarycomponent.h"
#include "../docstrings/triangulation/detail/boundarycomponent.h"

using regina::BoundaryComponent;

void addBoundaryComponent3(pybind11::module_& m) {
    // We use the global scope here because all of BoundaryComponent's members
    // are inherited, and so BoundaryComponent's own docstring namespace
    // does not exist.
    RDOC_SCOPE_BEGIN_MAIN
    RDOC_SCOPE_BASE(detail::BoundaryComponentBase)

    auto c = pybind11::class_<BoundaryComponent<3>>(m, "BoundaryComponent3",
            rdoc::BoundaryComponent)
        .def("index", &BoundaryComponent<3>::index, rbase::index)
        .def("size", &BoundaryComponent<3>::size, rbase::size)
        .def("countRidges", &BoundaryComponent<3>::countRidges,
            rbase::countRidges)
        .def("countFaces",
            &regina::python::countFaces<BoundaryComponent<3>, 3, 2>,
            pybind11::arg("subdim"), rbase::countFaces)
        .def("countTriangles", &BoundaryComponent<3>::countTriangles,
            rbase::countTriangles)
        .def("countEdges", &BoundaryComponent<3>::countEdges, rbase::countEdges)
        .def("countVertices", &BoundaryComponent<3>::countVertices,
            rbase::countVertices)
        .def("facets", &BoundaryComponent<3>::facets, rbase::facets)
        .def("faces", regina::python::faces<BoundaryComponent<3>, 3>,
            pybind11::arg("subdim"), rbase::faces)
        .def("triangles", &BoundaryComponent<3>::triangles, rbase::triangles)
        .def("edges", &BoundaryComponent<3>::edges, rbase::edges)
        .def("vertices", &BoundaryComponent<3>::vertices, rbase::vertices)
        .def("facet", &BoundaryComponent<3>::facet,
            pybind11::return_value_policy::reference, rbase::facet)
        .def("face", &regina::python::face<BoundaryComponent<3>, 3, size_t>,
            pybind11::arg("subdim"), pybind11::arg("index"), rbase::face)
        .def("triangle", &BoundaryComponent<3>::triangle,
            pybind11::return_value_policy::reference, rbase::triangle)
        .def("edge", &BoundaryComponent<3>::edge,
            pybind11::return_value_policy::reference, rbase::edge)
        .def("vertex", &BoundaryComponent<3>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("component", &BoundaryComponent<3>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("triangulation", &BoundaryComponent<3>::triangulation,
            rbase::triangulation)
        .def("build", [](const BoundaryComponent<3>& b) {
            // Return a clone of the resulting triangulation.
            // This is because Python cannot enforce the constness of
            // the reference that would normally be returned.
            return new regina::Triangulation<2>(b.build());
        }, rbase::build)
        .def("eulerChar", &BoundaryComponent<3>::eulerChar, rbase::eulerChar)
        .def("isReal", &BoundaryComponent<3>::isReal, rbase::isReal)
        .def("isIdeal", &BoundaryComponent<3>::isIdeal, rbase::isIdeal)
        .def("isInvalidVertex", &BoundaryComponent<3>::isInvalidVertex,
            rbase::isInvalidVertex)
        .def("isOrientable", &BoundaryComponent<3>::isOrientable,
            rbase::isOrientable)
        .def_readonly_static("dimension", &BoundaryComponent<3>::dimension)
        .def_readonly_static("allFaces", &BoundaryComponent<3>::allFaces)
        .def_readonly_static("allowVertex", &BoundaryComponent<3>::allowVertex)
        .def_readonly_static("canBuild", &BoundaryComponent<3>::canBuild)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addListView<
        decltype(std::declval<BoundaryComponent<3>>().vertices())>(m);
    regina::python::addListView<
        decltype(std::declval<BoundaryComponent<3>>().edges())>(m);
    regina::python::addListView<
        decltype(std::declval<BoundaryComponent<3>>().triangles())>(m);
}

