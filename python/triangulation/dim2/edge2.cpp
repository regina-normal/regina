
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
#include "../../docstrings/triangulation/facenumbering.h"
#include "../../docstrings/triangulation/face.h"

using namespace pybind11::literals;

using regina::Edge;
using regina::EdgeEmbedding;
using regina::Face;
using regina::FaceEmbedding;

void addEdge2(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)

    auto e = pybind11::class_<FaceEmbedding<2, 1>>(m, "FaceEmbedding2_1",
            rdoc::__class)
        .def(pybind11::init<regina::Triangle<2>*, regina::Perm<3>>(),
            rdoc::__init)
        .def(pybind11::init<const EdgeEmbedding<2>&>(), rdoc::__copy)
        .def("simplex", &EdgeEmbedding<2>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex)
        // So: clang can resolve triangle() but gcc cannot.
        // We could fix this with a static_cast, but we will just bind to
        // simplex() instead (which gcc _can_ resolve).
        .def("triangle", &EdgeEmbedding<2>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex_dim2)
        .def("face", &EdgeEmbedding<2>::face, rdoc::face)
        .def("edge", &EdgeEmbedding<2>::edge, rdoc::edge)
        .def("vertices", &EdgeEmbedding<2>::vertices, rdoc::vertices)
    ;
    regina::python::add_output_rich(e);
    regina::python::add_eq_operators(e, rdoc::__eq);

    RDOC_SCOPE_SWITCH(Face)
    RDOC_SCOPE_BASE(FaceNumbering)

    auto c = pybind11::class_<Face<2, 1>>(m, "Face2_1", rdoc::__class)
        .def("index", &Edge<2>::index, rdoc::index)
        .def("isValid", &Edge<2>::isValid, rdoc::isValid)
        .def("hasBadIdentification", &Edge<2>::hasBadIdentification,
            rdoc::hasBadIdentification)
        .def("hasBadLink", &Edge<2>::hasBadLink, rdoc::hasBadLink)
        .def("isLinkOrientable", &Edge<2>::isLinkOrientable,
            rdoc::isLinkOrientable)
        .def("degree", &Edge<2>::degree, rdoc::degree)
        .def("embedding", &Edge<2>::embedding, rdoc::embedding)
        .def("embeddings", &Edge<2>::embeddings, rdoc::embeddings)
        .def("__iter__", [](const Edge<2>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rdoc::__iter__)
        .def("front", &Edge<2>::front, rdoc::front)
        .def("back", &Edge<2>::back, rdoc::back)
        .def("triangulation", &Edge<2>::triangulation, rdoc::triangulation)
        .def("component", &Edge<2>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("boundaryComponent", &Edge<2>::boundaryComponent,
            pybind11::return_value_policy::reference, rdoc::boundaryComponent)
        .def("face", &regina::python::face<2, 1>,
            "lowerdim"_a, "face"_a, rdoc::face)
        .def("vertex", &Edge<2>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("faceMapping", &regina::python::faceMapping<2, 1>,
            "lowerdim"_a, "face"_a, rdoc::faceMapping)
        .def("vertexMapping", &Edge<2>::vertexMapping, rdoc::vertexMapping)
        .def("join", &Edge<2>::join, rdoc::join)
        .def("isLoop", &Edge<2>::isLoop, rdoc::isLoop)
        .def("lock", &Edge<2>::lock, rdoc::lock)
        .def("unlock", &Edge<2>::unlock, rdoc::unlock)
        .def("isLocked", &Edge<2>::isLocked, rdoc::isLocked)
        .def("isBoundary", &Edge<2>::isBoundary, rdoc::isBoundary)
        .def("isInternal", &Edge<2>::isInternal, rdoc::isInternal)
        .def("inMaximalForest", &Edge<2>::inMaximalForest,
            rdoc::inMaximalForest)
        .def_static("ordering", &Edge<2>::ordering, rbase::ordering)
        .def_static("faceNumber",
            pybind11::overload_cast<regina::Perm<3>>(&Edge<2>::faceNumber),
            rbase::faceNumber)
        .def_static("faceNumber",
            pybind11::overload_cast<int, int>(&Edge<2>::faceNumber),
            rbase::faceNumber_2)
        .def_static("containsVertex", &Edge<2>::containsVertex,
            rbase::containsVertex)
        .def_readonly_static("nFaces", &Edge<2>::nFaces)
        .def_readonly_static("lexNumbering", &Edge<2>::lexNumbering)
        .def_readonly_static("oppositeDim", &Edge<2>::oppositeDim)
        .def_readonly_static("dimension", &Edge<2>::dimension)
        .def_readonly_static("subdimension", &Edge<2>::subdimension)
        .def_readonly_static("hasNumberingTables", &Edge<2>::hasNumberingTables)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addStdView<
        decltype(std::declval<Edge<2>>().embeddings())>(internal,
        "Face2_1_embeddings");

    m.attr("EdgeEmbedding2") = m.attr("FaceEmbedding2_1");
    m.attr("Edge2") = m.attr("Face2_1");
}

