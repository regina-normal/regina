
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
        .def(pybind11::init<regina::Tetrahedron<3>*, regina::Perm<4>>())
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
        .def("embedding", &Edge<3>::embedding)
        .def("embeddings", &Edge<3>::embeddings)
        .def("front", &Edge<3>::front)
        .def("back", &Edge<3>::back)
        .def("triangulation", &Edge<3>::triangulation)
        .def("component", &Edge<3>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Edge<3>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Edge<3>, 1, int>)
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
        .def("linkingSurface", &Edge<3>::linkingSurface)
        .def_static("ordering", &Edge<3>::ordering)
        .def_static("faceNumber", &Edge<3>::faceNumber)
        .def_static("containsVertex", &Edge<3>::containsVertex)
        .def_readonly_static("edgeNumber", &Edge3_edgeNumber)
        .def_readonly_static("edgeVertex", &Edge3_edgeVertex)
        .def_readonly_static("nFaces", &Edge<3>::nFaces)
        .def_readonly_static("lexNumbering", &Edge<3>::lexNumbering)
        .def_readonly_static("oppositeDim", &Edge<3>::oppositeDim)
        .def_readonly_static("dimension", &Edge<3>::dimension)
        .def_readonly_static("subdimension", &Edge<3>::subdimension)
        // Since the Triangulation<3> maximal forest routines return
        // sets of edges, we need edges to be hashable.
        .def("__hash__", [](const Edge<3>& e) {
            // Edges are equal in python if they reference the same C++
            // objects.  Therefore we can just use the C++ pointer as a hash.
            //
            // So: we cannot cast to long, since this is too small on
            // 64-bit Windows.  The *correct* type is uintptr_t, but the
            // C++ standard says this is optional.  Does any compiler *not*
            // support uintptr_t?  If anyone gets a compile error from this,
            // please do let Ben know (and in the meantime if you are on a
            // 64-bit machine then you should be able to change it back to a
            // long for your own quick fix.)
            //
            // A further note: even though long is too small on 64-bit Windows,
            // this is harmless - since we are only using this as a hash, so
            // it is okay if the integer is truncated.  Perhaps then the
            // real standards-compliant solution is to just cast to a long
            // regardless of platform.
            return uintptr_t(std::addressof(e));
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    regina::python::addListView<
        decltype(std::declval<Edge<3>>().embeddings())>(m);

    m.attr("EdgeEmbedding3") = m.attr("FaceEmbedding3_1");
    m.attr("Edge3") = m.attr("Face3_1");
}

