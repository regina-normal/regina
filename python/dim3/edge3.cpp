
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
#include "../globalarray.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using regina::Edge;
using regina::EdgeEmbedding;
using regina::Face;
using regina::FaceEmbedding;

namespace {
    regina::python::GlobalArray2D<int> Edge3_edgeNumber(Edge<3>::edgeNumber, 4);
    regina::python::GlobalArray2D<int> Edge3_edgeVertex(Edge<3>::edgeVertex, 6);
}

void addEdge3(pybind11::module_& m) {
    auto e = pybind11::class_<FaceEmbedding<3, 1>>(m, "FaceEmbedding3_1")
        .def(pybind11::init<regina::Tetrahedron<3>*, int>())
        .def(pybind11::init<const EdgeEmbedding<3>&>())
        .def("simplex", &EdgeEmbedding<3>::simplex,
            pybind11::return_value_policy::reference)
        .def("tetrahedron", &EdgeEmbedding<3>::tetrahedron,
            pybind11::return_value_policy::reference)
        .def("face", &EdgeEmbedding<3>::face)
        .def("edge", &EdgeEmbedding<3>::edge)
        .def("vertices", &EdgeEmbedding<3>::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<Face<3, 1>>(m, "Face3_1")
        .def("index", &Edge<3>::index)
        .def("embeddings", [](const Edge<3>& e) {
            pybind11::list ans;
            for (const auto& emb : e)
                ans.append(emb);
            return ans;
        })
        .def("embedding", &Edge<3>::embedding,
            pybind11::return_value_policy::reference_internal)
        .def("front", &Edge<3>::front,
            pybind11::return_value_policy::reference_internal)
        .def("back", &Edge<3>::back,
            pybind11::return_value_policy::reference_internal)
        .def("triangulation", &Edge<3>::triangulation)
        .def("component", &Edge<3>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Edge<3>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Edge<3>, 1, int,
            pybind11::return_value_policy::reference>)
        .def("vertex", &Edge<3>::vertex,
            pybind11::return_value_policy::reference)
        .def("faceMapping", &regina::python::faceMapping<Edge<3>, 1, 4>)
        .def("vertexMapping", &Edge<3>::vertexMapping)
        .def("degree", &Edge<3>::degree)
        .def("isBoundary", &Edge<3>::isBoundary)
        .def("isValid", &Edge<3>::isValid)
        .def("hasBadIdentification", &Edge<3>::hasBadIdentification)
        .def("hasBadLink", &Edge<3>::hasBadLink)
        .def("isLinkOrientable", &Edge<3>::isLinkOrientable)
        .def_static("ordering", &Edge<3>::ordering)
        .def_static("faceNumber", &Edge<3>::faceNumber)
        .def_static("containsVertex", &Edge<3>::containsVertex)
        .def_readonly_static("edgeNumber", &Edge3_edgeNumber)
        .def_readonly_static("edgeVertex", &Edge3_edgeVertex)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("NEdgeEmbedding") = m.attr("FaceEmbedding3_1");
    m.attr("EdgeEmbedding3") = m.attr("FaceEmbedding3_1");
    m.attr("NEdge") = m.attr("Face3_1");
    m.attr("Edge3") = m.attr("Face3_1");
}

