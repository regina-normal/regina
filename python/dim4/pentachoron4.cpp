
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
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/dim4/pentachoron4.h"
#include "../docstrings/triangulation/detail/simplex.h"

using pybind11::overload_cast;
using regina::Pentachoron;

void addPentachoron4(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Face)
    RDOC_SCOPE_BASE(detail::SimplexBase)

    auto c = pybind11::class_<regina::Simplex<4>>(m, "Simplex4", rdoc_scope)
        .def("description", &Pentachoron<4>::description, rbase::description)
        .def("setDescription", &Pentachoron<4>::setDescription,
            rbase::setDescription)
        .def("index", &Pentachoron<4>::index, rbase::index)
        .def("adjacentPentachoron", &Pentachoron<4>::adjacentPentachoron,
            pybind11::return_value_policy::reference, rdoc::adjacentPentachoron)
        .def("adjacentSimplex", &Pentachoron<4>::adjacentSimplex,
            pybind11::return_value_policy::reference, rbase::adjacentSimplex)
        .def("adjacentGluing", &Pentachoron<4>::adjacentGluing,
            rbase::adjacentGluing)
        .def("adjacentFacet", &Pentachoron<4>::adjacentFacet,
            rbase::adjacentFacet)
        .def("hasBoundary", &Pentachoron<4>::hasBoundary, rbase::hasBoundary)
        .def("join", &Pentachoron<4>::join, rbase::join)
        .def("unjoin", &Pentachoron<4>::unjoin,
            pybind11::return_value_policy::reference, rbase::unjoin)
        .def("isolate", &Pentachoron<4>::isolate, rbase::isolate)
        .def("triangulation", &Pentachoron<4>::triangulation,
            rbase::triangulation)
        .def("component", &Pentachoron<4>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("face", &regina::python::face<Pentachoron<4>, 4, int>,
            pybind11::arg("subdim"), pybind11::arg("face"), rbase::face)
        .def("vertex", &Pentachoron<4>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("edge",
            overload_cast<int>(&Pentachoron<4>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rbase::edge)
        .def("edge",
            overload_cast<int, int>(&Pentachoron<4>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rbase::edge_2)
        .def("triangle", &Pentachoron<4>::triangle,
            pybind11::return_value_policy::reference, rbase::triangle)
        .def("tetrahedron", &Pentachoron<4>::tetrahedron,
            pybind11::return_value_policy::reference, rbase::tetrahedron)
        .def("faceMapping", &regina::python::faceMapping<Pentachoron<4>, 4>,
            pybind11::arg("subdim"), pybind11::arg("face"), rbase::faceMapping)
        .def("vertexMapping", &Pentachoron<4>::vertexMapping,
            rbase::vertexMapping)
        .def("edgeMapping", &Pentachoron<4>::edgeMapping, rbase::edgeMapping)
        .def("triangleMapping", &Pentachoron<4>::triangleMapping,
            rbase::triangleMapping)
        .def("tetrahedronMapping", &Pentachoron<4>::tetrahedronMapping,
            rbase::tetrahedronMapping)
        .def("orientation", &Pentachoron<4>::orientation, rbase::orientation)
        .def("facetInMaximalForest", &Pentachoron<4>::facetInMaximalForest,
            rbase::facetInMaximalForest)
        .def_readonly_static("dimension", &Pentachoron<4>::dimension)
        .def_readonly_static("subdimension", &Pentachoron<4>::subdimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    m.attr("Pentachoron4") = m.attr("Simplex4");
    m.attr("Face4_4") = m.attr("Simplex4");
}

