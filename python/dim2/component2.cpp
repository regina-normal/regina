
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
#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/dim2/component2.h"
#include "../docstrings/triangulation/detail/component.h"

using regina::Component;

void addComponent2(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(Component)
    RDOC_SCOPE_BASE(detail::ComponentBase)

    auto c = pybind11::class_<Component<2>>(m, "Component2", rdoc_scope)
        .def("index", &Component<2>::index, rbase::index)
        .def("size", &Component<2>::size, rbase::size)
        .def("countTriangles", &Component<2>::countTriangles,
            rbase::countTriangles)
        .def("countFaces", &regina::python::countFaces<Component<2>, 2, 2>,
            pybind11::arg("subdim"), rdoc::countFaces)
        .def("countEdges", &Component<2>::countEdges, rbase::countEdges)
        .def("countVertices", &Component<2>::countVertices,
            rbase::countVertices)
        .def("countBoundaryComponents", &Component<2>::countBoundaryComponents,
            rbase::countBoundaryComponents)
        .def("simplices", &Component<2>::simplices, rbase::simplices)
        .def("triangles", &Component<2>::triangles, rbase::triangles)
        .def("faces", &regina::python::faces<Component<2>, 2>,
            pybind11::arg("subdim"), rdoc::faces)
        .def("vertices", &Component<2>::vertices, rbase::vertices)
        .def("edges", &Component<2>::edges, rbase::edges)
        .def("boundaryComponents", &Component<2>::boundaryComponents,
            rbase::boundaryComponents)
        .def("triangle", &Component<2>::triangle,
            pybind11::return_value_policy::reference, rbase::triangle)
        .def("simplex", &Component<2>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("face", &regina::python::face<Component<2>, 2, size_t>,
            pybind11::arg("subdim"), pybind11::arg("index"), rdoc::face)
        .def("edge", &Component<2>::edge,
            pybind11::return_value_policy::reference, rbase::edge)
        .def("vertex", &Component<2>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("boundaryComponent", &Component<2>::boundaryComponent,
            pybind11::return_value_policy::reference, rbase::boundaryComponent)
        .def("isValid", &Component<2>::isValid, rbase::isValid)
        .def("isOrientable", &Component<2>::isOrientable, rbase::isOrientable)
        .def("isClosed", &Component<2>::isClosed, rdoc::isClosed)
        .def("hasBoundaryFacets", &Component<2>::hasBoundaryFacets,
            rbase::hasBoundaryFacets)
        .def("hasBoundaryEdges", &Component<2>::hasBoundaryEdges,
            rdoc::hasBoundaryEdges)
        .def("countBoundaryFacets", &Component<2>::countBoundaryFacets,
            rbase::countBoundaryFacets)
        .def("countBoundaryEdges", &Component<2>::countBoundaryEdges,
            rdoc::countBoundaryEdges)
        .def_readonly_static("dimension", &Component<2>::dimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    // No need for lower-dimensional faces here, since these reuse the same
    // ListView classes as Triangulation2.
    regina::python::addListView<
        decltype(std::declval<Component<2>>().triangles())>(internal,
        "Component2_simplices");
    regina::python::addListView<
        decltype(std::declval<Component<2>>().boundaryComponents())>(internal,
        "Component2_boundaryComponents");
}

