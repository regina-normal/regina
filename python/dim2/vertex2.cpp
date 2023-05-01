
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
#include "../docstrings/triangulation/alias/face.h"
#include "../docstrings/triangulation/alias/facenumber.h"
#include "../docstrings/triangulation/generic/face.h"
#include "../docstrings/triangulation/generic/faceembedding.h"
#include "../docstrings/triangulation/detail/face.h"
#include "../docstrings/triangulation/detail/facenumbering.h"

using regina::Vertex;
using regina::VertexEmbedding;
using regina::Face;
using regina::FaceEmbedding;

void addVertex2(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)
    RDOC_SCOPE_BASE_3(detail::FaceEmbeddingBase, alias::FaceNumber,
        alias::SimplexVoid)

    auto e = pybind11::class_<FaceEmbedding<2, 0>>(m, "FaceEmbedding2_0",
            rdoc_scope)
        .def(pybind11::init<regina::Triangle<2>*, regina::Perm<3>>(),
            rdoc::__init)
        .def(pybind11::init<const VertexEmbedding<2>&>(), rdoc::__copy)
        .def("simplex", &VertexEmbedding<2>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("triangle", &VertexEmbedding<2>::triangle,
            pybind11::return_value_policy::reference, rbase3::triangle)
        .def("face", &VertexEmbedding<2>::face, rbase::face)
        .def("vertex", &VertexEmbedding<2>::vertex, rbase2::vertex)
        .def("vertices", &VertexEmbedding<2>::vertices, rbase::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e, rbase::__eq, rbase::__ne);

    // We use the global scope here because all of Face's members are
    // inherited, and so Face's own docstring namespace does not exist.
    RDOC_SCOPE_SWITCH_MAIN
    RDOC_SCOPE_BASE_2(detail::FaceBase, detail::FaceNumberingAPI)

    auto c = pybind11::class_<Face<2, 0>>(m, "Face2_0", rdoc::Face)
        .def("index", &Vertex<2>::index, rbase::index)
        .def("isValid", &Vertex<2>::isValid, rbase::isValid)
        .def("hasBadIdentification", &Vertex<2>::hasBadIdentification,
            rbase::hasBadIdentification)
        .def("hasBadLink", &Vertex<2>::hasBadLink, rbase::hasBadLink)
        .def("isLinkOrientable", &Vertex<2>::isLinkOrientable,
            rbase::isLinkOrientable)
        .def("embedding", &Vertex<2>::embedding, rbase::embedding)
        .def("embeddings", &Vertex<2>::embeddings, rbase::embeddings)
        .def("__iter__", [](const Vertex<2>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rbase::__iter__)
        .def("front", &Vertex<2>::front, rbase::front)
        .def("back", &Vertex<2>::back, rbase::back)
        .def("triangulation", &Vertex<2>::triangulation, rbase::triangulation)
        .def("component", &Vertex<2>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("boundaryComponent", &Vertex<2>::boundaryComponent,
            pybind11::return_value_policy::reference, rbase::boundaryComponent)
        .def("degree", &Vertex<2>::degree, rbase::degree)
        .def("isBoundary", &Vertex<2>::isBoundary, rbase::isBoundary)
        .def_static("ordering", &Vertex<2>::ordering)
        .def_static("faceNumber", &Vertex<2>::faceNumber)
        .def_static("containsVertex", &Vertex<2>::containsVertex)
        .def_readonly_static("nFaces", &Vertex<2>::nFaces)
        .def_readonly_static("lexNumbering", &Vertex<2>::lexNumbering)
        .def_readonly_static("oppositeDim", &Vertex<2>::oppositeDim)
        .def_readonly_static("dimension", &Vertex<2>::dimension)
        .def_readonly_static("subdimension", &Vertex<2>::subdimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addListView<
        decltype(std::declval<Vertex<2>>().embeddings())>(m);

    m.attr("VertexEmbedding2") = m.attr("FaceEmbedding2_0");
    m.attr("Vertex2") = m.attr("Face2_0");
}

