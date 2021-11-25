
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
#include "../generic/facehelper.h"

using regina::Edge;
using regina::EdgeEmbedding;
using regina::Face;
using regina::FaceEmbedding;

void addEdge2(pybind11::module_& m) {
    auto e = pybind11::class_<FaceEmbedding<2, 1>>(m, "FaceEmbedding2_1")
        .def(pybind11::init<regina::Triangle<2>*, regina::Perm<3>>())
        .def(pybind11::init<const EdgeEmbedding<2>&>())
        .def("simplex", &EdgeEmbedding<2>::simplex,
            pybind11::return_value_policy::reference)
        .def("triangle", &EdgeEmbedding<2>::triangle,
            pybind11::return_value_policy::reference)
        .def("face", &EdgeEmbedding<2>::face)
        .def("edge", &EdgeEmbedding<2>::edge)
        .def("vertices", &EdgeEmbedding<2>::vertices)
    ;
    regina::python::add_output(e, true /* __repr__ */);
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<Face<2, 1>>(m, "Face2_1")
        .def("index", &Edge<2>::index)
        .def("isValid", &Edge<2>::isValid)
        .def("hasBadIdentification", &Edge<2>::hasBadIdentification)
        .def("hasBadLink", &Edge<2>::hasBadLink)
        .def("isLinkOrientable", &Edge<2>::isLinkOrientable)
        .def("degree", &Edge<2>::degree)
        .def("embedding", &Edge<2>::embedding)
        .def("embeddings", &Edge<2>::embeddings)
        .def("front", &Edge<2>::front)
        .def("back", &Edge<2>::back)
        .def("triangulation", &Edge<2>::triangulation)
        .def("component", &Edge<2>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Edge<2>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Edge<2>, 1, int>)
        .def("vertex", &Edge<2>::vertex,
            pybind11::return_value_policy::reference)
        .def("faceMapping", &regina::python::faceMapping<Edge<2>, 1, 3>)
        .def("vertexMapping", &Edge<2>::vertexMapping)
        .def("isBoundary", &Edge<2>::isBoundary)
        .def("inMaximalForest", &Edge<2>::inMaximalForest)
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

    regina::python::addListView<
        decltype(std::declval<Edge<2>>().embeddings())>(m);

    m.attr("EdgeEmbedding2") = m.attr("FaceEmbedding2_1");
    m.attr("Edge2") = m.attr("Face2_1");
}

