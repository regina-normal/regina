
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
#include "../helpers.h"
#include "../generic/facehelper.h"

using regina::Component;

void addComponent3(pybind11::module_& m) {
    auto c = pybind11::class_<Component<3>>(m, "Component3")
        .def("index", &Component<3>::index)
        .def("size", &Component<3>::size)
        .def("countTetrahedra", &Component<3>::countTetrahedra)
        .def("countFaces", &regina::python::countFaces<Component<3>, 3, 3>)
        .def("countTriangles", &Component<3>::countTriangles)
        .def("countEdges", &Component<3>::countEdges)
        .def("countVertices", &Component<3>::countVertices)
        .def("countBoundaryComponents", &Component<3>::countBoundaryComponents)
        .def("simplices", &Component<3>::simplices)
        .def("tetrahedra", &Component<3>::tetrahedra)
        .def("simplex", &Component<3>::simplex,
            pybind11::return_value_policy::reference)
        .def("tetrahedron", &Component<3>::tetrahedron,
            pybind11::return_value_policy::reference)
        .def("faces", &regina::python::faces<Component<3>, 3>)
        .def("vertices", &Component<3>::vertices)
        .def("edges", &Component<3>::edges)
        .def("triangles", &Component<3>::triangles)
        .def("boundaryComponents", &Component<3>::boundaryComponents)
        .def("face", &regina::python::face<Component<3>, 3, size_t>)
        .def("triangle", &Component<3>::triangle,
            pybind11::return_value_policy::reference)
        .def("edge", &Component<3>::edge,
            pybind11::return_value_policy::reference)
        .def("vertex", &Component<3>::vertex,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Component<3>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("isIdeal", &Component<3>::isIdeal)
        .def("isValid", &Component<3>::isValid)
        .def("isOrientable", &Component<3>::isOrientable)
        .def("isClosed", &Component<3>::isClosed)
        .def("hasBoundaryFacets", &Component<3>::hasBoundaryFacets)
        .def("hasBoundaryTriangles", &Component<3>::hasBoundaryTriangles)
        .def("countBoundaryFacets", &Component<3>::countBoundaryFacets)
        .def("countBoundaryTriangles", &Component<3>::countBoundaryTriangles)
        .def_readonly_static("dimension", &Component<3>::dimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    // No need for lower-dimensional faces here, since these reuse the same
    // ListView classes as Triangulation2.
    regina::python::addListView<
        decltype(std::declval<Component<3>>().tetrahedra())>(m);
    regina::python::addListView<
        decltype(std::declval<Component<3>>().boundaryComponents())>(m);
}

