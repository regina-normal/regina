
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

using regina::BoundaryComponent;

void addBoundaryComponent2(pybind11::module_& m) {
    auto c = pybind11::class_<BoundaryComponent<2>>(m, "BoundaryComponent2")
        .def("index", &BoundaryComponent<2>::index)
        .def("size", &BoundaryComponent<2>::size)
        .def("countFaces",
            &regina::python::countFaces<BoundaryComponent<2>, 2>)
        .def("countEdges", &BoundaryComponent<2>::countEdges)
        .def("countVertices", &BoundaryComponent<2>::countVertices)
        .def("facets", &BoundaryComponent<2>::facets,
            pybind11::return_value_policy::reference)
        .def("faces", &regina::python::faces<BoundaryComponent<2>, 2,
            pybind11::return_value_policy::reference>)
        .def("edges", &BoundaryComponent<2>::edges,
            pybind11::return_value_policy::reference)
        .def("vertices", &BoundaryComponent<2>::vertices,
            pybind11::return_value_policy::reference)
        .def("facet", &BoundaryComponent<2>::facet,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<BoundaryComponent<2>, 2, size_t,
            pybind11::return_value_policy::reference>)
        .def("edge", &BoundaryComponent<2>::edge,
            pybind11::return_value_policy::reference)
        .def("vertex", &BoundaryComponent<2>::vertex,
            pybind11::return_value_policy::reference)
        .def("component", &BoundaryComponent<2>::component,
            pybind11::return_value_policy::reference)
        .def("triangulation", &BoundaryComponent<2>::triangulation)
        .def("isOrientable", &BoundaryComponent<2>::isOrientable)
        // We cannot take the addresses of the following header-only properties,
        // so we define getter functions instead.
        .def_property_readonly_static("allFaces", [](pybind11::object) {
            return BoundaryComponent<2>::allFaces;
        })
        .def_property_readonly_static("allowVertex", [](pybind11::object) {
            return BoundaryComponent<2>::allowVertex;
        })
        .def_property_readonly_static("canBuild", [](pybind11::object) {
            return BoundaryComponent<2>::canBuild;
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("Dim2BoundaryComponent") = m.attr("BoundaryComponent2");
}

