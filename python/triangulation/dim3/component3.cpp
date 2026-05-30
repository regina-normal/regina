
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
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/dim3/component3.h"
#include "../../docstrings/triangulation/detail/component.h"

using namespace pybind11::literals;

using regina::Component;

void addComponent3(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(Component3)

    auto c = pybind11::class_<Component<3>>(m, "Component3", rdoc::__class)
        .def("index", &Component<3>::index, rdoc::index)
        .def("triangulation", &Component<3>::triangulation,
            rdoc::triangulation)
        .def("size", &Component<3>::size, rdoc::size)
        .def("countTetrahedra", &Component<3>::countTetrahedra,
            rdoc::countTetrahedra)
        .def("countFaces", (regina::python::countFacesFunc<Component<3>>)(
            &Component<3>::countFaces), rdoc::countFaces)
        .def("countTriangles", &Component<3>::countTriangles,
            rdoc::countTriangles)
        .def("countEdges", &Component<3>::countEdges, rdoc::countEdges)
        .def("countVertices", &Component<3>::countVertices,
            rdoc::countVertices)
        .def("countFacets", &Component<3>::countFacets, rdoc::countFacets)
        .def("countBoundaryComponents", &Component<3>::countBoundaryComponents,
            rdoc::countBoundaryComponents)
        .def("simplices", &Component<3>::simplices, rdoc::simplices)
        .def("tetrahedra", &Component<3>::tetrahedra, rdoc::tetrahedra)
        .def("simplex", &Component<3>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex)
        .def("tetrahedron", &Component<3>::tetrahedron,
            pybind11::return_value_policy::reference, rdoc::tetrahedron)
        .def("hasLocks", &Component<3>::hasLocks, rdoc::hasLocks)
        .def("faces",
            (regina::python::facesFunc<Component<3>>)(&Component<3>::faces),
            "subdim"_a, rdoc::faces)
        .def("vertices", &Component<3>::vertices, rdoc::vertices)
        .def("edges", &Component<3>::edges, rdoc::edges)
        .def("triangles", &Component<3>::triangles, rdoc::triangles)
        .def("boundaryComponents", &Component<3>::boundaryComponents,
            rdoc::boundaryComponents)
        .def("face",
            (regina::python::faceFunc<Component<3>>)(&Component<3>::face),
            pybind11::return_value_policy::reference,
            "subdim"_a, "index"_a, rdoc::face)
        .def("triangle", &Component<3>::triangle,
            pybind11::return_value_policy::reference, rdoc::triangle)
        .def("edge", &Component<3>::edge,
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("vertex", &Component<3>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("boundaryComponent", &Component<3>::boundaryComponent,
            pybind11::return_value_policy::reference, rdoc::boundaryComponent)
        .def("isIdeal", &Component<3>::isIdeal, rdoc::isIdeal)
        .def("isValid", &Component<3>::isValid, rdoc::isValid)
        .def("isOrientable", &Component<3>::isOrientable, rdoc::isOrientable)
        .def("isClosed", &Component<3>::isClosed, rdoc::isClosed)
        .def("hasBoundaryFacets", &Component<3>::hasBoundaryFacets,
            rdoc::hasBoundaryFacets)
        .def("hasBoundaryTriangles", &Component<3>::hasBoundaryTriangles,
            rdoc::hasBoundaryTriangles)
        .def("countBoundaryFacets", &Component<3>::countBoundaryFacets,
            rdoc::countBoundaryFacets)
        .def("countBoundaryTriangles", &Component<3>::countBoundaryTriangles,
            rdoc::countBoundaryTriangles)
        .def_readonly_static("dimension", &Component<3>::dimension)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    // No need for lower-dimensional faces here, since these reuse the same
    // view classes as Triangulation2.
    regina::python::addStdView<
        decltype(std::declval<Component<3>>().tetrahedra())>(internal,
        "Component3_simplices");
    regina::python::addStdView<
        decltype(std::declval<Component<3>>().boundaryComponents())>(internal,
        "Component3_boundaryComponents");
}

