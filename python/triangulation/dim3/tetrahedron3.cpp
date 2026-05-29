
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
#include "triangulation/dim3.h"
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/simplex.h"

using namespace pybind11::literals;

using pybind11::overload_cast;
using regina::Tetrahedron;

void addTetrahedron3(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Simplex)

    auto c = pybind11::class_<regina::Simplex<3>>(m, "Simplex3", rdoc::__class)
        .def("description", &Tetrahedron<3>::description, rdoc::description)
        .def("setDescription", &Tetrahedron<3>::setDescription,
            rdoc::setDescription)
        .def("index", &Tetrahedron<3>::index, rdoc::index)
        .def("adjacentTetrahedron", &Tetrahedron<3>::adjacentTetrahedron,
            pybind11::return_value_policy::reference, rdoc::adjacentTetrahedron)
        .def("adjacentSimplex", &Tetrahedron<3>::adjacentSimplex,
            pybind11::return_value_policy::reference, rdoc::adjacentSimplex)
        .def("adjacentGluing", &Tetrahedron<3>::adjacentGluing,
            rdoc::adjacentGluing)
        .def("adjacentFace", &Tetrahedron<3>::adjacentFace, rdoc::adjacentFace)
        .def("adjacentFacet", &Tetrahedron<3>::adjacentFacet,
            rdoc::adjacentFacet)
        .def("hasBoundary", &Tetrahedron<3>::hasBoundary, rdoc::hasBoundary)
        .def("join", &Tetrahedron<3>::join, rdoc::join)
        .def("unjoin", &Tetrahedron<3>::unjoin,
            pybind11::return_value_policy::reference, rdoc::unjoin)
        .def("isolate", &Tetrahedron<3>::isolate, rdoc::isolate)
        .def("lock", &Tetrahedron<3>::lock, rdoc::lock)
        .def("lockFacet", &Tetrahedron<3>::lockFacet, rdoc::lockFacet)
        .def("unlock", &Tetrahedron<3>::unlock, rdoc::unlock)
        .def("unlockFacet", &Tetrahedron<3>::unlockFacet, rdoc::unlockFacet)
        .def("unlockAll", &Tetrahedron<3>::unlockAll, rdoc::unlockAll)
        .def("isLocked", &Tetrahedron<3>::isLocked, rdoc::isLocked)
        .def("isFacetLocked", &Tetrahedron<3>::isFacetLocked,
            rdoc::isFacetLocked)
        .def("lockMask", &Tetrahedron<3>::lockMask, rdoc::lockMask)
        .def("triangulation", &Tetrahedron<3>::triangulation,
            rdoc::triangulation)
        .def("component", &Tetrahedron<3>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("face", &regina::python::face<3, 3>,
            "subdim"_a, "face"_a, rdoc::face)
        .def("vertex", &Tetrahedron<3>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("edge",
            overload_cast<int>(&Tetrahedron<3>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("edge",
            overload_cast<int, int>(&Tetrahedron<3>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rdoc::edge_2)
        .def("triangle", &Tetrahedron<3>::triangle,
            pybind11::return_value_policy::reference, rdoc::triangle)
        .def("faceMapping", &regina::python::faceMapping<3, 3>,
            "subdim"_a, "face"_a, rdoc::faceMapping)
        .def("vertexMapping", &Tetrahedron<3>::vertexMapping,
            rdoc::vertexMapping)
        .def("edgeMapping", &Tetrahedron<3>::edgeMapping, rdoc::edgeMapping)
        .def("triangleMapping", &Tetrahedron<3>::triangleMapping,
            rdoc::triangleMapping)
        .def("orientation", &Tetrahedron<3>::orientation, rdoc::orientation)
        .def("facetInMaximalForest", &Tetrahedron<3>::facetInMaximalForest,
            rdoc::facetInMaximalForest)
        .def_readonly_static("dimension", &Tetrahedron<3>::dimension)
        .def_readonly_static("subdimension", &Tetrahedron<3>::subdimension)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    m.attr("Tetrahedron3") = m.attr("Simplex3");
    m.attr("Face3_3") = m.attr("Simplex3");
}

