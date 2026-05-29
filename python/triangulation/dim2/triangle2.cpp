
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
#include "triangulation/dim2.h"
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/simplex.h"

using namespace pybind11::literals;

using pybind11::overload_cast;
using regina::Triangle;

void addTriangle2(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Simplex)

    auto c = pybind11::class_<regina::Simplex<2>>(m, "Simplex2", rdoc::__class)
        .def("description", &Triangle<2>::description, rdoc::description)
        .def("setDescription", &Triangle<2>::setDescription,
            rdoc::setDescription)
        .def("index", &Triangle<2>::index, rdoc::index)
        .def("adjacentTriangle", &Triangle<2>::adjacentTriangle,
            pybind11::return_value_policy::reference, rdoc::adjacentTriangle)
        .def("adjacentSimplex", &Triangle<2>::adjacentSimplex,
            pybind11::return_value_policy::reference, rdoc::adjacentSimplex)
        .def("adjacentGluing", &Triangle<2>::adjacentGluing,
            rdoc::adjacentGluing)
        .def("adjacentEdge", &Triangle<2>::adjacentEdge, rdoc::adjacentEdge)
        .def("adjacentFacet", &Triangle<2>::adjacentFacet, rdoc::adjacentFacet)
        .def("hasBoundary", &Triangle<2>::hasBoundary, rdoc::hasBoundary)
        .def("join", &Triangle<2>::join, rdoc::join)
        .def("unjoin", &Triangle<2>::unjoin,
            pybind11::return_value_policy::reference, rdoc::unjoin)
        .def("isolate", &Triangle<2>::isolate, rdoc::isolate)
        .def("lock", &Triangle<2>::lock, rdoc::lock)
        .def("lockFacet", &Triangle<2>::lockFacet, rdoc::lockFacet)
        .def("unlock", &Triangle<2>::unlock, rdoc::unlock)
        .def("unlockFacet", &Triangle<2>::unlockFacet, rdoc::unlockFacet)
        .def("unlockAll", &Triangle<2>::unlockAll, rdoc::unlockAll)
        .def("isLocked", &Triangle<2>::isLocked, rdoc::isLocked)
        .def("isFacetLocked", &Triangle<2>::isFacetLocked, rdoc::isFacetLocked)
        .def("lockMask", &Triangle<2>::lockMask, rdoc::lockMask)
        .def("triangulation", &Triangle<2>::triangulation, rdoc::triangulation)
        .def("component", &Triangle<2>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("face", &regina::python::face<2, 2>,
            "subdim"_a, "face"_a, rdoc::face)
        .def("vertex", &Triangle<2>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("edge",
            overload_cast<int>(&Triangle<2>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("edge",
            overload_cast<int, int>(&Triangle<2>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rdoc::edge_2)
        .def("faceMapping", &regina::python::faceMapping<2, 2>,
            "subdim"_a, "face"_a, rdoc::faceMapping)
        .def("vertexMapping", &Triangle<2>::vertexMapping, rdoc::vertexMapping)
        .def("edgeMapping", &Triangle<2>::edgeMapping, rdoc::edgeMapping)
        .def("orientation", &Triangle<2>::orientation, rdoc::orientation)
        .def("facetInMaximalForest", &Triangle<2>::facetInMaximalForest,
            rdoc::facetInMaximalForest)
        .def_readonly_static("dimension", &Triangle<2>::dimension)
        .def_readonly_static("subdimension", &Triangle<2>::subdimension)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    m.attr("Triangle2") = m.attr("Simplex2");
    m.attr("Face2_2") = m.attr("Simplex2");
}

