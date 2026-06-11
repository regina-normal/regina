
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
#include "../../docstrings/triangulation/component.h"

using namespace pybind11::literals;

using regina::Component;

void addComponent2(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(Component)

    auto c = pybind11::class_<Component<2>>(m, "Component2", rdoc::__class)
        .def("index", &Component<2>::index, rdoc::index)
        .def("triangulation", &Component<2>::triangulation,
            rdoc::triangulation)
        .def("size", &Component<2>::size, rdoc::size)
        .def("countTriangles", &Component<2>::countTriangles,
            rdoc::countTriangles)
        .def("countFaces", (regina::python::countFacesFunc<Component<2>>)(
            &Component<2>::countFaces), rdoc::countFaces)
        .def("countEdges", &Component<2>::countEdges, rdoc::countEdges)
        .def("countVertices", &Component<2>::countVertices,
            rdoc::countVertices)
        .def("countFacets", &Component<2>::countFacets, rdoc::countFacets)
        .def("countBoundaryComponents", &Component<2>::countBoundaryComponents,
            rdoc::countBoundaryComponents)
        .def("simplices", &Component<2>::simplices, rdoc::simplices)
        .def("triangles", &Component<2>::triangles, rdoc::triangles)
        .def("hasLocks", &Component<2>::hasLocks, rdoc::hasLocks)
        .def("faces",
            (regina::python::facesFunc<Component<2>>)(&Component<2>::faces),
            "subdim"_a, rdoc::faces)
        .def("vertices", &Component<2>::vertices, rdoc::vertices)
        .def("edges", &Component<2>::edges, rdoc::edges)
        .def("boundaryComponents", &Component<2>::boundaryComponents,
            rdoc::boundaryComponents)
        .def("triangle", &Component<2>::triangle,
            pybind11::return_value_policy::reference, rdoc::triangle)
        .def("simplex", &Component<2>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex)
        .def("face",
            (regina::python::faceFunc<Component<2>>)(&Component<2>::face),
            pybind11::return_value_policy::reference,
            "subdim"_a, "index"_a, rdoc::face)
        .def("edge", &Component<2>::edge,
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("vertex", &Component<2>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("boundaryComponent", &Component<2>::boundaryComponent,
            pybind11::return_value_policy::reference, rdoc::boundaryComponent)
        .def("isValid", &Component<2>::isValid, rdoc::isValid)
        .def("isOrientable", &Component<2>::isOrientable, rdoc::isOrientable)
        .def("isClosed", &Component<2>::isClosed, rdoc::isClosed)
        .def("hasBoundaryFacets", &Component<2>::hasBoundaryFacets,
            rdoc::hasBoundaryFacets)
        .def("hasBoundaryEdges", &Component<2>::hasBoundaryEdges,
            rdoc::hasBoundaryEdges)
        .def("countBoundaryFacets", &Component<2>::countBoundaryFacets,
            rdoc::countBoundaryFacets)
        .def("countBoundaryEdges", &Component<2>::countBoundaryEdges,
            rdoc::countBoundaryEdges)
        .def_readonly_static("dimension", &Component<2>::dimension)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    // No need for lower-dimensional faces here, since these reuse the same
    // view classes as Triangulation2.
    regina::python::addStdView<
        decltype(std::declval<Component<2>>().triangles())>(internal,
        "Component2_simplices");
    regina::python::addStdView<
        decltype(std::declval<Component<2>>().boundaryComponents())>(internal,
        "Component2_boundaryComponents");
}

