
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
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using regina::Component;

void addComponent4(pybind11::module_& m) {
    auto c = pybind11::class_<Component<4>>(m, "Component4")
        .def("index", &Component<4>::index)
        .def("size", &Component<4>::size)
        .def("countPentachora", &Component<4>::countPentachora)
        .def("countFaces", &regina::python::countFaces<Component<4>, 4>)
        .def("countTetrahedra", &Component<4>::countTetrahedra)
        .def("countTriangles", &Component<4>::countTriangles)
        .def("countEdges", &Component<4>::countEdges)
        .def("countVertices", &Component<4>::countVertices)
        .def("countBoundaryComponents", &Component<4>::countBoundaryComponents)
        .def("simplices", &Component<4>::simplices,
            pybind11::return_value_policy::reference)
        .def("pentachora", &Component<4>::pentachora,
            pybind11::return_value_policy::reference)
        .def("faces", &regina::python::faces<Component<4>, 4,
            pybind11::return_value_policy::reference>)
        .def("vertices", &Component<4>::vertices,
            pybind11::return_value_policy::reference)
        .def("edges", &Component<4>::edges,
            pybind11::return_value_policy::reference)
        .def("triangles", &Component<4>::triangles,
            pybind11::return_value_policy::reference)
        .def("tetrahedra", &Component<4>::tetrahedra,
            pybind11::return_value_policy::reference)
        .def("boundaryComponents", &Component<4>::boundaryComponents,
            pybind11::return_value_policy::reference)
        .def("simplex", &Component<4>::simplex,
            pybind11::return_value_policy::reference)
        .def("pentachoron", &Component<4>::pentachoron,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Component<4>, 4, size_t,
            pybind11::return_value_policy::reference>)
        .def("tetrahedron", &Component<4>::tetrahedron,
            pybind11::return_value_policy::reference)
        .def("triangle", &Component<4>::triangle,
            pybind11::return_value_policy::reference)
        .def("edge", &Component<4>::edge,
            pybind11::return_value_policy::reference)
        .def("vertex", &Component<4>::vertex,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Component<4>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("isIdeal", &Component<4>::isIdeal)
        .def("isValid", &Component<4>::isValid)
        .def("isOrientable", &Component<4>::isOrientable)
        .def("isClosed", &Component<4>::isClosed)
        .def("hasBoundaryFacets", &Component<4>::hasBoundaryFacets)
        .def("hasBoundaryTetrahedra", &Component<4>::hasBoundaryTetrahedra)
        .def("countBoundaryFacets", &Component<4>::countBoundaryFacets)
        .def("countBoundaryTetrahedra", &Component<4>::countBoundaryTetrahedra)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("Dim4Component") = m.attr("Component4");
}

