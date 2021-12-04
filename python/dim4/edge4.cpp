
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
#include "triangulation/dim4.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using regina::Edge;
using regina::EdgeEmbedding;
using regina::Face;
using regina::FaceEmbedding;

namespace {
    regina::python::GlobalArray2D<int> Edge4_edgeNumber(
        Edge<4>::edgeNumber, 5);
    regina::python::GlobalArray2D<int> Edge4_edgeVertex(
        Edge<4>::edgeVertex, 10);
}

void addEdge4(pybind11::module_& m) {
    auto e = pybind11::class_<FaceEmbedding<4, 1>>(m, "FaceEmbedding4_1")
        .def(pybind11::init<regina::Pentachoron<4>*, regina::Perm<5>>())
        .def(pybind11::init<const EdgeEmbedding<4>&>())
        .def("simplex", &EdgeEmbedding<4>::simplex,
            pybind11::return_value_policy::reference)
        .def("pentachoron", &EdgeEmbedding<4>::pentachoron,
            pybind11::return_value_policy::reference)
        .def("face", &EdgeEmbedding<4>::face)
        .def("edge", &EdgeEmbedding<4>::edge)
        .def("vertices", &EdgeEmbedding<4>::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<Face<4, 1>>(m, "Face4_1")
        .def("index", &Edge<4>::index)
        .def("embedding", &Edge<4>::embedding)
        .def("embeddings", &Edge<4>::embeddings)
        .def("front", &Edge<4>::front)
        .def("back", &Edge<4>::back)
        .def("triangulation", &Edge<4>::triangulation)
        .def("component", &Edge<4>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Edge<4>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Edge<4>, 1, int>)
        .def("vertex", &Edge<4>::vertex,
            pybind11::return_value_policy::reference)
        .def("faceMapping", &regina::python::faceMapping<Edge<4>, 1, 5>)
        .def("vertexMapping", &Edge<4>::vertexMapping)
        .def("degree", &Edge<4>::degree)
        .def("isBoundary", &Edge<4>::isBoundary)
        .def("isLinkOrientable", &Edge<4>::isLinkOrientable)
        .def("isValid", &Edge<4>::isValid)
        .def("hasBadIdentification", &Edge<4>::hasBadIdentification)
        .def("hasBadLink", &Edge<4>::hasBadLink)
        .def("buildLink", [](const Edge<4>& e) {
            // Return a clone of the resulting triangulation.
            // This is because Python cannot enforce the constness of
            // the reference that would normally be returned.
            return new regina::Triangulation<2>(e.buildLink());
        })
        .def("buildLinkInclusion", &Edge<4>::buildLinkInclusion)
        .def_static("ordering", &Edge<4>::ordering)
        .def_static("faceNumber", &Edge<4>::faceNumber)
        .def_static("containsVertex", &Edge<4>::containsVertex)
        .def_readonly_static("edgeNumber", &Edge4_edgeNumber)
        .def_readonly_static("edgeVertex", &Edge4_edgeVertex)
        .def_readonly_static("nFaces", &Edge<4>::nFaces)
        .def_readonly_static("lexNumbering", &Edge<4>::lexNumbering)
        .def_readonly_static("oppositeDim", &Edge<4>::oppositeDim)
        .def_readonly_static("dimension", &Edge<4>::dimension)
        .def_readonly_static("subdimension", &Edge<4>::subdimension)
    ;
    regina::python::add_output(c, regina::python::PYTHON_REPR_NONE);
    regina::python::add_eq_operators(c);

    regina::python::addListView<
        decltype(std::declval<Edge<4>>().embeddings())>(m);

    m.attr("EdgeEmbedding4") = m.attr("FaceEmbedding4_1");
    m.attr("Edge4") = m.attr("Face4_1");
}

