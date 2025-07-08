
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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/generic/boundarycomponent.h"
#include "../docstrings/triangulation/detail/boundarycomponent.h"

using regina::BoundaryComponent;

void addBoundaryComponent2(pybind11::module_& m, pybind11::module_& internal) {
    // We use the global scope here because all of BoundaryComponent's members
    // are inherited, and so BoundaryComponent's own docstring namespace
    // does not exist.
    RDOC_SCOPE_BEGIN_MAIN
    RDOC_SCOPE_BASE(detail::BoundaryComponentBase)

    auto c = pybind11::class_<BoundaryComponent<2>>(m, "BoundaryComponent2",
            rdoc::BoundaryComponent)
        .def("index", &BoundaryComponent<2>::index, rbase::index)
        .def("size", &BoundaryComponent<2>::size, rbase::size)
        .def("countRidges", &BoundaryComponent<2>::countRidges,
            rbase::countRidges)
        .def("countFaces",
            &regina::python::countFaces<BoundaryComponent<2>, 2, 1>,
            pybind11::arg("subdim"), rbase::countFaces)
        .def("countEdges", &BoundaryComponent<2>::countEdges, rbase::countEdges)
        .def("countVertices", &BoundaryComponent<2>::countVertices,
            rbase::countVertices)
        .def("facets", &BoundaryComponent<2>::facets, rbase::facets)
        .def("faces", &regina::python::faces<BoundaryComponent<2>, 2>,
            pybind11::arg("subdim"), rbase::faces)
        .def("edges", &BoundaryComponent<2>::edges, rbase::edges)
        .def("vertices", &BoundaryComponent<2>::vertices, rbase::vertices)
        .def("facet", &BoundaryComponent<2>::facet,
            pybind11::return_value_policy::reference, rbase::facet)
        .def("face", &regina::python::face<BoundaryComponent<2>, 2, size_t>,
            pybind11::arg("subdim"), pybind11::arg("index"), rbase::face)
        .def("edge", &BoundaryComponent<2>::edge,
            pybind11::return_value_policy::reference, rbase::edge)
        .def("vertex", &BoundaryComponent<2>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("component", &BoundaryComponent<2>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("triangulation", &BoundaryComponent<2>::triangulation,
            rbase::triangulation)
        .def("eulerChar", &BoundaryComponent<2>::eulerChar, rbase::eulerChar)
        .def("isReal", &BoundaryComponent<2>::isReal, rbase::isReal)
        .def("isIdeal", &BoundaryComponent<2>::isIdeal, rbase::isIdeal)
        .def("isInvalidVertex", &BoundaryComponent<2>::isInvalidVertex,
            rbase::isInvalidVertex)
        .def("isOrientable", &BoundaryComponent<2>::isOrientable,
            rbase::isOrientable)
        .def_readonly_static("dimension", &BoundaryComponent<2>::dimension)
        .def_readonly_static("allFaces", &BoundaryComponent<2>::allFaces)
        .def_readonly_static("allowVertex", &BoundaryComponent<2>::allowVertex)
        .def_readonly_static("canBuild", &BoundaryComponent<2>::canBuild)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addListView<
        decltype(std::declval<BoundaryComponent<2>>().vertices())>(internal,
        "BoundaryComponent2_vertices");
    regina::python::addListView<
        decltype(std::declval<BoundaryComponent<2>>().edges())>(internal,
        "BoundaryComponent2_edges");
}

