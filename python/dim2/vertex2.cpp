
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
#include "triangulation/dim2.h"
#include "../helpers.h"

using regina::Vertex;
using regina::VertexEmbedding;
using regina::Face;
using regina::FaceEmbedding;

void addVertex2(pybind11::module_& m) {
    auto e = pybind11::class_<FaceEmbedding<2, 0>>(m, "FaceEmbedding2_0")
        .def(pybind11::init<regina::Triangle<2>*, regina::Perm<3>>())
        .def(pybind11::init<const VertexEmbedding<2>&>())
        .def("simplex", &VertexEmbedding<2>::simplex,
            pybind11::return_value_policy::reference)
        .def("triangle", &VertexEmbedding<2>::triangle,
            pybind11::return_value_policy::reference)
        .def("face", &VertexEmbedding<2>::face)
        .def("vertex", &VertexEmbedding<2>::vertex)
        .def("vertices", &VertexEmbedding<2>::vertices)
    ;
    regina::python::add_output(e, true /* __repr__ */);
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<Face<2, 0>>(m, "Face2_0")
        .def("index", &Vertex<2>::index)
        .def("isValid", &Vertex<2>::isValid)
        .def("hasBadIdentification", &Vertex<2>::hasBadIdentification)
        .def("hasBadLink", &Vertex<2>::hasBadLink)
        .def("isLinkOrientable", &Vertex<2>::isLinkOrientable)
        // Make all embeddings functions return by value in Python, since
        // embeddings are lightweight and we wish to enforce constness.
        .def("embeddings", [](const Vertex<2>& v) {
            pybind11::list ans;
            for (const auto& emb : v)
                ans.append(emb);
            return ans;
        })
        .def("embedding", &Vertex<2>::embedding)
        .def("front", &Vertex<2>::front)
        .def("back", &Vertex<2>::back)
        .def("triangulation", &Vertex<2>::triangulation)
        .def("component", &Vertex<2>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Vertex<2>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("degree", &Vertex<2>::degree)
        .def("isBoundary", &Vertex<2>::isBoundary)
        .def_static("ordering", &Vertex<2>::ordering)
        .def_static("faceNumber", &Vertex<2>::faceNumber)
        .def_static("containsVertex", &Vertex<2>::containsVertex)
        // On some systems we cannot take addresses of the following
        // inline class constants (e.g., this fails with gcc10 on windows).
        // We therefore define getter functions instead.
        .def_property_readonly_static("nFaces", [](pybind11::object) {
            return Vertex<2>::nFaces;
        })
        .def_property_readonly_static("lexNumbering", [](pybind11::object) {
            return Vertex<2>::lexNumbering;
        })
        .def_property_readonly_static("oppositeDim", [](pybind11::object) {
            return Vertex<2>::oppositeDim;
        })
        .def_property_readonly_static("dimension", [](pybind11::object) {
            return Vertex<2>::dimension;
        })
        .def_property_readonly_static("subdimension", [](pybind11::object) {
            return Vertex<2>::subdimension;
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("VertexEmbedding2") = m.attr("FaceEmbedding2_0");
    m.attr("Vertex2") = m.attr("Face2_0");
}

