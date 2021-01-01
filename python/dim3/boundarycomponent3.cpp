
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
#include "triangulation/dim3.h"
#include "triangulation/dim2.h" // for build()
#include "../helpers.h"
#include "../generic/facehelper.h"

using regina::BoundaryComponent;

void addBoundaryComponent3(pybind11::module_& m) {
    auto c = pybind11::class_<BoundaryComponent<3>>(m, "BoundaryComponent3")
        .def("index", &BoundaryComponent<3>::index)
        .def("size", &BoundaryComponent<3>::size)
        .def("countFaces", &regina::python::countFaces<BoundaryComponent<3>, 3>)
        .def("countTriangles", &BoundaryComponent<3>::countTriangles)
        .def("countEdges", &BoundaryComponent<3>::countEdges)
        .def("countVertices", &BoundaryComponent<3>::countVertices)
        .def("facets", &BoundaryComponent<3>::facets,
            pybind11::return_value_policy::reference)
        .def("faces", regina::python::faces<BoundaryComponent<3>, 3,
            pybind11::return_value_policy::reference>)
        .def("triangles", &BoundaryComponent<3>::triangles,
            pybind11::return_value_policy::reference)
        .def("edges", &BoundaryComponent<3>::edges,
            pybind11::return_value_policy::reference)
        .def("vertices", &BoundaryComponent<3>::vertices,
            pybind11::return_value_policy::reference)
        .def("facet", &BoundaryComponent<3>::facet,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<BoundaryComponent<3>, 3, size_t,
            pybind11::return_value_policy::reference>)
        .def("triangle", &BoundaryComponent<3>::triangle,
            pybind11::return_value_policy::reference)
        .def("edge", &BoundaryComponent<3>::edge,
            pybind11::return_value_policy::reference)
        .def("vertex", &BoundaryComponent<3>::vertex,
            pybind11::return_value_policy::reference)
        .def("component", &BoundaryComponent<3>::component,
            pybind11::return_value_policy::reference)
        .def("triangulation", &BoundaryComponent<3>::triangulation)
        .def("build", [](const BoundaryComponent<3>* b) {
            // Return a clone of the resulting triangulation.  This is because
            // triangulations have a custom holder type, and so pybind11 ignores
            // any attempt to pass return_value_policy::reference_internal.
            return new regina::Triangulation<2>(*(b->build()));
        })
        .def("eulerChar", &BoundaryComponent<3>::eulerChar)
        .def("isReal", &BoundaryComponent<3>::isReal)
        .def("isIdeal", &BoundaryComponent<3>::isIdeal)
        .def("isInvalidVertex", &BoundaryComponent<3>::isInvalidVertex)
        .def("isOrientable", &BoundaryComponent<3>::isOrientable)
        // We cannot take the addresses of the following header-only properties,
        // so we define getter functions instead.
        .def_property_readonly_static("allFaces", [](pybind11::object) {
            return BoundaryComponent<3>::allFaces;
        })
        .def_property_readonly_static("allowVertex", [](pybind11::object) {
            return BoundaryComponent<3>::allowVertex;
        })
        .def_property_readonly_static("canBuild", [](pybind11::object) {
            return BoundaryComponent<3>::canBuild;
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("NBoundaryComponent") = m.attr("BoundaryComponent3");
}

