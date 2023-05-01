
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

#include "../pybind11/pybind11.h"
#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/alias/face.h"
#include "../docstrings/triangulation/alias/facenumber.h"
#include "../docstrings/triangulation/generic/face.h"
#include "../docstrings/triangulation/generic/faceembedding.h"
#include "../docstrings/triangulation/detail/face.h"
#include "../docstrings/triangulation/detail/facenumbering.h"

using regina::Edge;
using regina::EdgeEmbedding;
using regina::Face;
using regina::FaceEmbedding;

void addEdge2(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)
    RDOC_SCOPE_BASE_3(detail::FaceEmbeddingBase, alias::FaceNumber,
        alias::SimplexVoid)

    auto e = pybind11::class_<FaceEmbedding<2, 1>>(m, "FaceEmbedding2_1",
            rdoc_scope)
        .def(pybind11::init<regina::Triangle<2>*, regina::Perm<3>>(),
            rdoc::__init)
        .def(pybind11::init<const EdgeEmbedding<2>&>(), rdoc::__copy)
        .def("simplex", &EdgeEmbedding<2>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("triangle", &EdgeEmbedding<2>::triangle,
            pybind11::return_value_policy::reference, rbase3::triangle)
        .def("face", &EdgeEmbedding<2>::face, rbase::face)
        .def("edge", &EdgeEmbedding<2>::edge, rbase2::edge)
        .def("vertices", &EdgeEmbedding<2>::vertices, rbase::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e, rbase::__eq, rbase::__ne);

    // We use the global scope here because all of Face's members are
    // inherited, and so Face's own docstring namespace does not exist.
    RDOC_SCOPE_SWITCH_MAIN
    RDOC_SCOPE_BASE_2(detail::FaceBase, detail::FaceNumberingAPI)

    auto c = pybind11::class_<Face<2, 1>>(m, "Face2_1", rdoc::Face)
        .def("index", &Edge<2>::index, rbase::index)
        .def("isValid", &Edge<2>::isValid, rbase::isValid)
        .def("hasBadIdentification", &Edge<2>::hasBadIdentification,
            rbase::hasBadIdentification)
        .def("hasBadLink", &Edge<2>::hasBadLink, rbase::hasBadLink)
        .def("isLinkOrientable", &Edge<2>::isLinkOrientable,
            rbase::isLinkOrientable)
        .def("degree", &Edge<2>::degree, rbase::degree)
        .def("embedding", &Edge<2>::embedding, rbase::embedding)
        .def("embeddings", &Edge<2>::embeddings, rbase::embeddings)
        .def("__iter__", [](const Edge<2>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rbase::__iter__)
        .def("front", &Edge<2>::front, rbase::front)
        .def("back", &Edge<2>::back, rbase::back)
        .def("triangulation", &Edge<2>::triangulation, rbase::triangulation)
        .def("component", &Edge<2>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("boundaryComponent", &Edge<2>::boundaryComponent,
            pybind11::return_value_policy::reference, rbase::boundaryComponent)
        .def("face", &regina::python::face<Edge<2>, 1, int>,
            pybind11::arg("lowerdim"), pybind11::arg("face"),
            rbase::face)
        .def("vertex", &Edge<2>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("faceMapping", &regina::python::faceMapping<Edge<2>, 1, 3>,
            pybind11::arg("lowerdim"), pybind11::arg("face"),
            rbase::faceMapping)
        .def("vertexMapping", &Edge<2>::vertexMapping, rbase::vertexMapping)
        .def("lock", &Edge<2>::lock, rbase::lock)
        .def("unlock", &Edge<2>::unlock, rbase::unlock)
        .def("isLocked", &Edge<2>::isLocked, rbase::isLocked)
        .def("isBoundary", &Edge<2>::isBoundary, rbase::isBoundary)
        .def("inMaximalForest", &Edge<2>::inMaximalForest,
            rbase::inMaximalForest)
        .def_static("ordering", &Edge<2>::ordering)
        .def_static("faceNumber", &Edge<2>::faceNumber)
        .def_static("containsVertex", &Edge<2>::containsVertex)
        .def_readonly_static("nFaces", &Edge<2>::nFaces)
        .def_readonly_static("lexNumbering", &Edge<2>::lexNumbering)
        .def_readonly_static("oppositeDim", &Edge<2>::oppositeDim)
        .def_readonly_static("dimension", &Edge<2>::dimension)
        .def_readonly_static("subdimension", &Edge<2>::subdimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addListView<
        decltype(std::declval<Edge<2>>().embeddings())>(m);

    m.attr("EdgeEmbedding2") = m.attr("FaceEmbedding2_1");
    m.attr("Edge2") = m.attr("Face2_1");
}

