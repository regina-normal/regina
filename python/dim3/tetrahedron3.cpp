
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
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using pybind11::overload_cast;
using regina::Tetrahedron;

void addTetrahedron3(pybind11::module_& m) {
    auto c = pybind11::class_<regina::Simplex<3>>(m, "Simplex3")
        .def("description", &Tetrahedron<3>::description)
        .def("setDescription", &Tetrahedron<3>::setDescription)
        .def("index", &Tetrahedron<3>::index)
        .def("adjacentTetrahedron", &Tetrahedron<3>::adjacentTetrahedron,
            pybind11::return_value_policy::reference)
        .def("adjacentSimplex", &Tetrahedron<3>::adjacentSimplex,
            pybind11::return_value_policy::reference)
        .def("adjacentGluing", &Tetrahedron<3>::adjacentGluing)
        .def("adjacentFace", &Tetrahedron<3>::adjacentFace)
        .def("adjacentFacet", &Tetrahedron<3>::adjacentFacet)
        .def("hasBoundary", &Tetrahedron<3>::hasBoundary)
        .def("join", &Tetrahedron<3>::join)
        .def("unjoin", &Tetrahedron<3>::unjoin,
            pybind11::return_value_policy::reference)
        .def("isolate", &Tetrahedron<3>::isolate)
        .def("triangulation", &Tetrahedron<3>::triangulation)
        .def("component", &Tetrahedron<3>::component,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Tetrahedron<3>, 3, int>)
        .def("vertex", &Tetrahedron<3>::vertex,
            pybind11::return_value_policy::reference)
        .def("edge",
            overload_cast<int>(&Tetrahedron<3>::edge, pybind11::const_),
            pybind11::return_value_policy::reference)
        .def("edge",
            overload_cast<int, int>(&Tetrahedron<3>::edge, pybind11::const_),
            pybind11::return_value_policy::reference)
        .def("triangle", &Tetrahedron<3>::triangle,
            pybind11::return_value_policy::reference)
        .def("faceMapping", &regina::python::faceMapping<Tetrahedron<3>, 3>)
        .def("vertexMapping", &Tetrahedron<3>::vertexMapping)
        .def("edgeMapping", &Tetrahedron<3>::edgeMapping)
        .def("triangleMapping", &Tetrahedron<3>::triangleMapping)
        .def("orientation", &Tetrahedron<3>::orientation)
        .def("facetInMaximalForest", &Tetrahedron<3>::facetInMaximalForest)
        .def_readonly_static("dimension", &Tetrahedron<3>::dimension)
        .def_readonly_static("subdimension", &Tetrahedron<3>::subdimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("Tetrahedron3") = m.attr("Simplex3");
    m.attr("Face3_3") = m.attr("Simplex3");
}

