
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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/dim3/component3.h"
#include "../docstrings/triangulation/detail/component.h"

using regina::Component;

void addComponent3(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(Component)
    RDOC_SCOPE_BASE(detail::ComponentBase)

    auto c = pybind11::class_<Component<3>>(m, "Component3", rdoc_scope)
        .def("index", &Component<3>::index, rbase::index)
        .def("size", &Component<3>::size, rbase::size)
        .def("countTetrahedra", &Component<3>::countTetrahedra,
            rbase::countTetrahedra)
        .def("countFaces", &regina::python::countFaces<Component<3>, 3, 3>,
            pybind11::arg("subdim"), rdoc::countFaces)
        .def("countTriangles", &Component<3>::countTriangles,
            rbase::countTriangles)
        .def("countEdges", &Component<3>::countEdges, rbase::countEdges)
        .def("countVertices", &Component<3>::countVertices,
            rbase::countVertices)
        .def("countBoundaryComponents", &Component<3>::countBoundaryComponents,
            rbase::countBoundaryComponents)
        .def("simplices", &Component<3>::simplices, rbase::simplices)
        .def("tetrahedra", &Component<3>::tetrahedra, rbase::tetrahedra)
        .def("simplex", &Component<3>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("tetrahedron", &Component<3>::tetrahedron,
            pybind11::return_value_policy::reference, rbase::tetrahedron)
        .def("faces", &regina::python::faces<Component<3>, 3>,
            pybind11::arg("subdim"), rdoc::faces)
        .def("vertices", &Component<3>::vertices, rbase::vertices)
        .def("edges", &Component<3>::edges, rbase::edges)
        .def("triangles", &Component<3>::triangles, rbase::triangles)
        .def("boundaryComponents", &Component<3>::boundaryComponents,
            rbase::boundaryComponents)
        .def("face", &regina::python::face<Component<3>, 3, size_t>,
            pybind11::arg("subdim"), pybind11::arg("index"), rdoc::face)
        .def("triangle", &Component<3>::triangle,
            pybind11::return_value_policy::reference, rbase::triangle)
        .def("edge", &Component<3>::edge,
            pybind11::return_value_policy::reference, rbase::edge)
        .def("vertex", &Component<3>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("boundaryComponent", &Component<3>::boundaryComponent,
            pybind11::return_value_policy::reference, rbase::boundaryComponent)
        .def("isIdeal", &Component<3>::isIdeal, rdoc::isIdeal)
        .def("isValid", &Component<3>::isValid, rbase::isValid)
        .def("isOrientable", &Component<3>::isOrientable, rbase::isOrientable)
        .def("isClosed", &Component<3>::isClosed, rdoc::isClosed)
        .def("hasBoundaryFacets", &Component<3>::hasBoundaryFacets,
            rbase::hasBoundaryFacets)
        .def("hasBoundaryTriangles", &Component<3>::hasBoundaryTriangles,
            rdoc::hasBoundaryTriangles)
        .def("countBoundaryFacets", &Component<3>::countBoundaryFacets,
            rbase::countBoundaryFacets)
        .def("countBoundaryTriangles", &Component<3>::countBoundaryTriangles,
            rdoc::countBoundaryTriangles)
        .def_readonly_static("dimension", &Component<3>::dimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    // No need for lower-dimensional faces here, since these reuse the same
    // ListView classes as Triangulation2.
    regina::python::addListView<
        decltype(std::declval<Component<3>>().tetrahedra())>(internal,
        "Component3_simplices");
    regina::python::addListView<
        decltype(std::declval<Component<3>>().boundaryComponents())>(internal,
        "Component3_boundaryComponents");
}

