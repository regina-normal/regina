
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include "triangulation/dim4.h"
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/simplex.h"

using namespace pybind11::literals;

using pybind11::overload_cast;
using regina::Pentachoron;

void addPentachoron4(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Simplex)

    auto c = pybind11::class_<regina::Simplex<4>>(m, "Simplex4", rdoc::__class)
        .def("description", &Pentachoron<4>::description, rdoc::description)
        .def("setDescription", &Pentachoron<4>::setDescription,
            rdoc::setDescription)
        .def("index", &Pentachoron<4>::index, rdoc::index)
        .def("adjacentPentachoron", &Pentachoron<4>::adjacentPentachoron,
            pybind11::return_value_policy::reference, rdoc::adjacentPentachoron)
        .def("adjacentSimplex", &Pentachoron<4>::adjacentSimplex,
            pybind11::return_value_policy::reference, rdoc::adjacentSimplex)
        .def("adjacentGluing", &Pentachoron<4>::adjacentGluing,
            rdoc::adjacentGluing)
        .def("adjacentFacet", &Pentachoron<4>::adjacentFacet,
            rdoc::adjacentFacet)
        .def("hasBoundary", &Pentachoron<4>::hasBoundary, rdoc::hasBoundary)
        .def("join", &Pentachoron<4>::join, rdoc::join)
        .def("unjoin", &Pentachoron<4>::unjoin,
            pybind11::return_value_policy::reference, rdoc::unjoin)
        .def("isolate", &Pentachoron<4>::isolate, rdoc::isolate)
        .def("lock", &Pentachoron<4>::lock, rdoc::lock)
        .def("lockFacet", &Pentachoron<4>::lockFacet, rdoc::lockFacet)
        .def("unlock", &Pentachoron<4>::unlock, rdoc::unlock)
        .def("unlockFacet", &Pentachoron<4>::unlockFacet, rdoc::unlockFacet)
        .def("unlockAll", &Pentachoron<4>::unlockAll, rdoc::unlockAll)
        .def("isLocked", &Pentachoron<4>::isLocked, rdoc::isLocked)
        .def("isFacetLocked", &Pentachoron<4>::isFacetLocked,
            rdoc::isFacetLocked)
        .def("lockMask", &Pentachoron<4>::lockMask, rdoc::lockMask)
        .def("triangulation", &Pentachoron<4>::triangulation,
            rdoc::triangulation)
        .def("component", &Pentachoron<4>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("face", &regina::python::face<Pentachoron<4>>,
            "subdim"_a, "face"_a, rdoc::face)
        .def("vertex", &Pentachoron<4>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("edge",
            overload_cast<int>(&Pentachoron<4>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("edge",
            overload_cast<int, int>(&Pentachoron<4>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rdoc::edge_2)
        .def("triangle", &Pentachoron<4>::triangle,
            pybind11::return_value_policy::reference, rdoc::triangle)
        .def("tetrahedron", &Pentachoron<4>::tetrahedron,
            pybind11::return_value_policy::reference, rdoc::tetrahedron)
        .def("faceMapping", &regina::python::faceMapping<Pentachoron<4>>,
            "subdim"_a, "face"_a, rdoc::faceMapping)
        .def("vertexMapping", &Pentachoron<4>::vertexMapping,
            rdoc::vertexMapping)
        .def("edgeMapping", &Pentachoron<4>::edgeMapping, rdoc::edgeMapping)
        .def("triangleMapping", &Pentachoron<4>::triangleMapping,
            rdoc::triangleMapping)
        .def("tetrahedronMapping", &Pentachoron<4>::tetrahedronMapping,
            rdoc::tetrahedronMapping)
        .def("orientation", &Pentachoron<4>::orientation, rdoc::orientation)
        .def("facetInMaximalForest", &Pentachoron<4>::facetInMaximalForest,
            rdoc::facetInMaximalForest)
        .def_readonly_static("dimension", &Pentachoron<4>::dimension)
        .def_readonly_static("subdimension", &Pentachoron<4>::subdimension)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    m.attr("Pentachoron4") = m.attr("Simplex4");
    m.attr("Face4_4") = m.attr("Simplex4");
}

