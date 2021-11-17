
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
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"

using regina::Vertex;
using regina::VertexEmbedding;
using regina::Face;
using regina::FaceEmbedding;

void addVertex4(pybind11::module_& m) {
    auto e = pybind11::class_<FaceEmbedding<4, 0>>(m, "FaceEmbedding4_0")
        .def(pybind11::init<regina::Pentachoron<4>*, regina::Perm<5>>())
        .def(pybind11::init<const VertexEmbedding<4>&>())
        .def("simplex", &VertexEmbedding<4>::simplex,
            pybind11::return_value_policy::reference)
        .def("pentachoron", &VertexEmbedding<4>::pentachoron,
            pybind11::return_value_policy::reference)
        .def("face", &VertexEmbedding<4>::face)
        .def("vertex", &VertexEmbedding<4>::vertex)
        .def("vertices", &VertexEmbedding<4>::vertices)
    ;
    regina::python::add_output(e, true /* __repr__ */);
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<Face<4, 0>>(m, "Face4_0")
        .def("index", &Vertex<4>::index)
        // Make all embeddings functions return by value in Python, since
        // embeddings are lightweight and we wish to enforce constness.
        .def("embeddings", [](const Vertex<4>& v) {
            pybind11::list ans;
            for (const auto& emb : v)
                ans.append(emb);
            return ans;
        })
        .def("embedding", &Vertex<4>::embedding)
        .def("front", &Vertex<4>::front)
        .def("back", &Vertex<4>::back)
        .def("triangulation", &Vertex<4>::triangulation)
        .def("component", &Vertex<4>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Vertex<4>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("degree", &Vertex<4>::degree)
        .def("buildLink", [](const Vertex<4>& v) {
            // Return a clone of the resulting triangulation.
            // This is because Python cannot enforce the constness of
            // the reference that would normally be returned.
            return new regina::Triangulation<3>(v.buildLink());
        })
        .def("buildLinkInclusion", &Vertex<4>::buildLinkInclusion)
        .def("isLinkOrientable", &Vertex<4>::isLinkOrientable)
        .def("isValid", &Vertex<4>::isValid)
        .def("hasBadIdentification", &Vertex<4>::hasBadIdentification)
        .def("hasBadLink", &Vertex<4>::hasBadLink)
        .def("isIdeal", &Vertex<4>::isIdeal)
        .def("isBoundary", &Vertex<4>::isBoundary)
        .def_static("ordering", &Vertex<4>::ordering)
        .def_static("faceNumber", &Vertex<4>::faceNumber)
        .def_static("containsVertex", &Vertex<4>::containsVertex)
        .def_readonly_static("nFaces", &Vertex<4>::nFaces)
        .def_readonly_static("lexNumbering", &Vertex<4>::lexNumbering)
        .def_readonly_static("oppositeDim", &Vertex<4>::oppositeDim)
        .def_readonly_static("dimension", &Vertex<4>::dimension)
        .def_readonly_static("subdimension", &Vertex<4>::subdimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("VertexEmbedding4") = m.attr("FaceEmbedding4_0");
    m.attr("Vertex4") = m.attr("Face4_0");
}

