
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
#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using pybind11::overload_cast;
using regina::Triangle;

void addTriangle2(pybind11::module_& m) {
    auto c = pybind11::class_<regina::Simplex<2>>(m, "Simplex2")
        .def("description", &Triangle<2>::description)
        .def("setDescription", &Triangle<2>::setDescription)
        .def("index", &Triangle<2>::index)
        .def("adjacentTriangle", &Triangle<2>::adjacentTriangle,
            pybind11::return_value_policy::reference)
        .def("adjacentSimplex", &Triangle<2>::adjacentSimplex,
            pybind11::return_value_policy::reference)
        .def("adjacentGluing", &Triangle<2>::adjacentGluing)
        .def("adjacentEdge", &Triangle<2>::adjacentEdge)
        .def("adjacentFacet", &Triangle<2>::adjacentFacet)
        .def("hasBoundary", &Triangle<2>::hasBoundary)
        .def("join", &Triangle<2>::join)
        .def("unjoin", &Triangle<2>::unjoin,
            pybind11::return_value_policy::reference)
        .def("isolate", &Triangle<2>::isolate)
        .def("triangulation", &Triangle<2>::triangulation)
        .def("component", &Triangle<2>::component,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Triangle<2>, 2, int,
            pybind11::return_value_policy::reference>)
        .def("vertex", &Triangle<2>::vertex,
            pybind11::return_value_policy::reference)
        .def("edge",
            overload_cast<int>(&Triangle<2>::edge, pybind11::const_),
            pybind11::return_value_policy::reference)
        .def("edge",
            overload_cast<int, int>(&Triangle<2>::edge, pybind11::const_),
            pybind11::return_value_policy::reference)
        .def("faceMapping", &regina::python::faceMapping<Triangle<2>, 2>)
        .def("vertexMapping", &Triangle<2>::vertexMapping)
        .def("edgeMapping", &Triangle<2>::edgeMapping)
        .def("orientation", &Triangle<2>::orientation)
        .def("facetInMaximalForest", &Triangle<2>::facetInMaximalForest)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("Dim2Triangle") = m.attr("Simplex2");
    m.attr("Triangle2") = m.attr("Simplex2");
    m.attr("Face2_2") = m.attr("Simplex2");
}

