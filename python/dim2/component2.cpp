
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using regina::Component;

void addComponent2(pybind11::module_& m) {
    auto c = pybind11::class_<Component<2>>(m, "Component2")
        .def("index", &Component<2>::index)
        .def("size", &Component<2>::size)
        .def("countTriangles", &Component<2>::countTriangles)
        .def("countFaces", &regina::python::countFaces<Component<2>, 2>)
        .def("countEdges", &Component<2>::countEdges)
        .def("countVertices", &Component<2>::countVertices)
        .def("countBoundaryComponents", &Component<2>::countBoundaryComponents)
        .def("simplices", &Component<2>::simplices,
            pybind11::return_value_policy::reference)
        .def("triangles", &Component<2>::triangles,
            pybind11::return_value_policy::reference)
        .def("faces", &regina::python::faces<Component<2>, 2,
            pybind11::return_value_policy::reference>)
        .def("vertices", &Component<2>::vertices,
            pybind11::return_value_policy::reference)
        .def("edges", &Component<2>::edges,
            pybind11::return_value_policy::reference)
        .def("boundaryComponents", &Component<2>::boundaryComponents,
            pybind11::return_value_policy::reference)
        .def("triangle", &Component<2>::triangle,
            pybind11::return_value_policy::reference)
        .def("simplex", &Component<2>::simplex,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Component<2>, 2, size_t,
            pybind11::return_value_policy::reference>)
        .def("edge", &Component<2>::edge,
            pybind11::return_value_policy::reference)
        .def("vertex", &Component<2>::vertex,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Component<2>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("isValid", &Component<2>::isValid)
        .def("isOrientable", &Component<2>::isOrientable)
        .def("isClosed", &Component<2>::isClosed)
        .def("hasBoundaryFacets", &Component<2>::hasBoundaryFacets)
        .def("hasBoundaryEdges", &Component<2>::hasBoundaryEdges)
        .def("countBoundaryFacets", &Component<2>::countBoundaryFacets)
        .def("countBoundaryEdges", &Component<2>::countBoundaryEdges)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("Dim2Component") = m.attr("Component2");
}

