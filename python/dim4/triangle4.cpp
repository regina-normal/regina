
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
#include "triangulation/dim4.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using regina::Triangle;
using regina::TriangleEmbedding;
using regina::Face;
using regina::FaceEmbedding;

namespace {
    regina::python::GlobalArray3D<int> Triangle4_triangleNumber(
        Triangle<4>::triangleNumber, 5);
    regina::python::GlobalArray2D<int> Triangle4_triangleVertex(
        Triangle<4>::triangleVertex, 10);
}

void addTriangle4(pybind11::module_& m) {
    auto e = pybind11::class_<FaceEmbedding<4, 2>>(m, "FaceEmbedding4_2")
        .def(pybind11::init<regina::Pentachoron<4>*, regina::Perm<5>>())
        .def(pybind11::init<const TriangleEmbedding<4>&>())
        .def("simplex", &TriangleEmbedding<4>::simplex,
            pybind11::return_value_policy::reference)
        .def("pentachoron", &TriangleEmbedding<4>::pentachoron,
            pybind11::return_value_policy::reference)
        .def("face", &TriangleEmbedding<4>::face)
        .def("triangle", &TriangleEmbedding<4>::triangle)
        .def("vertices", &TriangleEmbedding<4>::vertices)
    ;
    regina::python::add_output(e, true /* __repr__ */);
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<Face<4, 2>>(m, "Face4_2")
        .def("index", &Triangle<4>::index)
        // Make all embeddings functions return by value in Python, since
        // embeddings are lightweight and we wish to enforce constness.
        .def("embeddings", [](const Triangle<4>& t) {
            pybind11::list ans;
            for (const auto& emb : t)
                ans.append(emb);
            return ans;
        })
        .def("embedding", &Triangle<4>::embedding)
        .def("front", &Triangle<4>::front)
        .def("back", &Triangle<4>::back)
        .def("triangulation", &Triangle<4>::triangulation)
        .def("component", &Triangle<4>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Triangle<4>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Triangle<4>, 2, int>)
        .def("vertex", &Triangle<4>::vertex,
            pybind11::return_value_policy::reference)
        .def("edge", &Triangle<4>::edge,
            pybind11::return_value_policy::reference)
        .def("faceMapping", &regina::python::faceMapping<Triangle<4>, 2, 5>)
        .def("vertexMapping", &Triangle<4>::vertexMapping)
        .def("edgeMapping", &Triangle<4>::edgeMapping)
        .def("degree", &Triangle<4>::degree)
        .def("isBoundary", &Triangle<4>::isBoundary)
        .def("isLinkOrientable", &Triangle<4>::isLinkOrientable)
        .def("isValid", &Triangle<4>::isValid)
        .def("hasBadIdentification", &Triangle<4>::hasBadIdentification)
        .def("hasBadLink", &Triangle<4>::hasBadLink)
        .def_static("ordering", &Triangle<4>::ordering)
        .def_static("faceNumber", &Triangle<4>::faceNumber)
        .def_static("containsVertex", &Triangle<4>::containsVertex)
        .def_readonly_static("triangleNumber", &Triangle4_triangleNumber)
        .def_readonly_static("triangleVertex", &Triangle4_triangleVertex)
        .def_readonly_static("nFaces", &Triangle<4>::nFaces)
        .def_readonly_static("lexNumbering", &Triangle<4>::lexNumbering)
        .def_readonly_static("oppositeDim", &Triangle<4>::oppositeDim)
        .def_readonly_static("dimension", &Triangle<4>::dimension)
        .def_readonly_static("subdimension", &Triangle<4>::subdimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("TriangleEmbedding4") = m.attr("FaceEmbedding4_2");
    m.attr("Triangle4") = m.attr("Face4_2");
}

