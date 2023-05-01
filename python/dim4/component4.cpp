
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

#include "../pybind11/pybind11.h"
#include "../pybind11/stl.h"
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/dim4/component4.h"
#include "../docstrings/triangulation/detail/component.h"

using regina::Component;

void addComponent4(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Component)
    RDOC_SCOPE_BASE(detail::ComponentBase)

    auto c = pybind11::class_<Component<4>>(m, "Component4", rdoc_scope)
        .def("index", &Component<4>::index, rbase::index)
        .def("size", &Component<4>::size, rbase::size)
        .def("countPentachora", &Component<4>::countPentachora,
            rbase::countPentachora)
        .def("countFaces", &regina::python::countFaces<Component<4>, 4, 4>,
            pybind11::arg("subdim"), rdoc::countFaces)
        .def("countTetrahedra", &Component<4>::countTetrahedra,
            rbase::countTetrahedra)
        .def("countTriangles", &Component<4>::countTriangles,
            rbase::countTriangles)
        .def("countEdges", &Component<4>::countEdges, rbase::countEdges)
        .def("countVertices", &Component<4>::countVertices,
            rbase::countVertices)
        .def("countBoundaryComponents", &Component<4>::countBoundaryComponents,
            rbase::countBoundaryComponents)
        .def("simplices", &Component<4>::simplices, rbase::simplices)
        .def("pentachora", &Component<4>::pentachora, rbase::pentachora)
        .def("faces", &regina::python::faces<Component<4>, 4>,
            pybind11::arg("subdim"), rdoc::faces)
        .def("vertices", &Component<4>::vertices, rbase::vertices)
        .def("edges", &Component<4>::edges, rbase::edges)
        .def("triangles", &Component<4>::triangles, rbase::triangles)
        .def("tetrahedra", &Component<4>::tetrahedra, rbase::tetrahedra)
        .def("boundaryComponents", &Component<4>::boundaryComponents,
            rbase::boundaryComponents)
        .def("simplex", &Component<4>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("pentachoron", &Component<4>::pentachoron,
            pybind11::return_value_policy::reference, rbase::pentachoron)
        .def("face", &regina::python::face<Component<4>, 4, size_t>,
            pybind11::arg("subdim"), pybind11::arg("index"), rdoc::face)
        .def("tetrahedron", &Component<4>::tetrahedron,
            pybind11::return_value_policy::reference, rbase::tetrahedron)
        .def("triangle", &Component<4>::triangle,
            pybind11::return_value_policy::reference, rbase::triangle)
        .def("edge", &Component<4>::edge,
            pybind11::return_value_policy::reference, rbase::edge)
        .def("vertex", &Component<4>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("boundaryComponent", &Component<4>::boundaryComponent,
            pybind11::return_value_policy::reference, rbase::boundaryComponent)
        .def("isIdeal", &Component<4>::isIdeal, rdoc::isIdeal)
        .def("isValid", &Component<4>::isValid, rbase::isValid)
        .def("isOrientable", &Component<4>::isOrientable, rbase::isOrientable)
        .def("isClosed", &Component<4>::isClosed, rdoc::isClosed)
        .def("hasBoundaryFacets", &Component<4>::hasBoundaryFacets,
            rbase::hasBoundaryFacets)
        .def("hasBoundaryTetrahedra", &Component<4>::hasBoundaryTetrahedra,
            rdoc::hasBoundaryTetrahedra)
        .def("countBoundaryFacets", &Component<4>::countBoundaryFacets,
            rbase::countBoundaryFacets)
        .def("countBoundaryTetrahedra", &Component<4>::countBoundaryTetrahedra,
            rdoc::countBoundaryTetrahedra)
        .def_readonly_static("dimension", &Component<4>::dimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    // No need for lower-dimensional faces here, since these reuse the same
    // ListView classes as Triangulation2.
    regina::python::addListView<
        decltype(std::declval<Component<4>>().pentachora())>(m);
    regina::python::addListView<
        decltype(std::declval<Component<4>>().boundaryComponents())>(m);
}

