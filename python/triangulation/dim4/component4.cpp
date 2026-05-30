
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
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/dim4/component4.h"
#include "../../docstrings/triangulation/component.h"

using namespace pybind11::literals;

using regina::Component;

void addComponent4(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(Component4)

    auto c = pybind11::class_<Component<4>>(m, "Component4", rdoc::__class)
        .def("index", &Component<4>::index, rdoc::index)
        .def("triangulation", &Component<4>::triangulation,
            rdoc::triangulation)
        .def("size", &Component<4>::size, rdoc::size)
        .def("countPentachora", &Component<4>::countPentachora,
            rdoc::countPentachora)
        .def("countFaces", (regina::python::countFacesFunc<Component<4>>)(
            &Component<4>::countFaces), rdoc::countFaces)
        .def("countTetrahedra", &Component<4>::countTetrahedra,
            rdoc::countTetrahedra)
        .def("countTriangles", &Component<4>::countTriangles,
            rdoc::countTriangles)
        .def("countEdges", &Component<4>::countEdges, rdoc::countEdges)
        .def("countVertices", &Component<4>::countVertices,
            rdoc::countVertices)
        .def("countFacets", &Component<4>::countFacets, rdoc::countFacets)
        .def("countBoundaryComponents", &Component<4>::countBoundaryComponents,
            rdoc::countBoundaryComponents)
        .def("simplices", &Component<4>::simplices, rdoc::simplices)
        .def("pentachora", &Component<4>::pentachora, rdoc::pentachora)
        .def("hasLocks", &Component<4>::hasLocks, rdoc::hasLocks)
        .def("faces",
            (regina::python::facesFunc<Component<4>>)(&Component<4>::faces),
            "subdim"_a, rdoc::faces)
        .def("vertices", &Component<4>::vertices, rdoc::vertices)
        .def("edges", &Component<4>::edges, rdoc::edges)
        .def("triangles", &Component<4>::triangles, rdoc::triangles)
        .def("tetrahedra", &Component<4>::tetrahedra, rdoc::tetrahedra)
        .def("boundaryComponents", &Component<4>::boundaryComponents,
            rdoc::boundaryComponents)
        .def("simplex", &Component<4>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex)
        .def("pentachoron", &Component<4>::pentachoron,
            pybind11::return_value_policy::reference, rdoc::pentachoron)
        .def("face",
            (regina::python::faceFunc<Component<4>>)(&Component<4>::face),
            pybind11::return_value_policy::reference,
            "subdim"_a, "index"_a, rdoc::face)
        .def("tetrahedron", &Component<4>::tetrahedron,
            pybind11::return_value_policy::reference, rdoc::tetrahedron)
        .def("triangle", &Component<4>::triangle,
            pybind11::return_value_policy::reference, rdoc::triangle)
        .def("edge", &Component<4>::edge,
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("vertex", &Component<4>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("boundaryComponent", &Component<4>::boundaryComponent,
            pybind11::return_value_policy::reference, rdoc::boundaryComponent)
        .def("isIdeal", &Component<4>::isIdeal, rdoc::isIdeal)
        .def("isValid", &Component<4>::isValid, rdoc::isValid)
        .def("isOrientable", &Component<4>::isOrientable, rdoc::isOrientable)
        .def("isClosed", &Component<4>::isClosed, rdoc::isClosed)
        .def("hasBoundaryFacets", &Component<4>::hasBoundaryFacets,
            rdoc::hasBoundaryFacets)
        .def("hasBoundaryTetrahedra", &Component<4>::hasBoundaryTetrahedra,
            rdoc::hasBoundaryTetrahedra)
        .def("countBoundaryFacets", &Component<4>::countBoundaryFacets,
            rdoc::countBoundaryFacets)
        .def("countBoundaryTetrahedra", &Component<4>::countBoundaryTetrahedra,
            rdoc::countBoundaryTetrahedra)
        .def_readonly_static("dimension", &Component<4>::dimension)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    // No need for lower-dimensional faces here, since these reuse the same
    // view classes as Triangulation2.
    regina::python::addStdView<
        decltype(std::declval<Component<4>>().pentachora())>(internal,
        "Component4_simplices");
    regina::python::addStdView<
        decltype(std::declval<Component<4>>().boundaryComponents())>(internal,
        "Component4_boundaryComponents");
}

