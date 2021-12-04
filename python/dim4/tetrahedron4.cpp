
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
#include "../helpers.h"
#include "../generic/facehelper.h"

using regina::Tetrahedron;
using regina::TetrahedronEmbedding;
using regina::Face;
using regina::FaceEmbedding;

void addTetrahedron4(pybind11::module_& m) {
    auto e = pybind11::class_<FaceEmbedding<4, 3>>(m, "FaceEmbedding4_3")
        .def(pybind11::init<regina::Pentachoron<4>*, regina::Perm<5>>())
        .def(pybind11::init<const TetrahedronEmbedding<4>&>())
        .def("simplex", &TetrahedronEmbedding<4>::simplex,
            pybind11::return_value_policy::reference)
        .def("pentachoron", &TetrahedronEmbedding<4>::pentachoron,
            pybind11::return_value_policy::reference)
        .def("face", &TetrahedronEmbedding<4>::face)
        .def("tetrahedron", &TetrahedronEmbedding<4>::tetrahedron)
        .def("vertices", &TetrahedronEmbedding<4>::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<Face<4, 3>>(m, "Face4_3")
        .def("index", &Tetrahedron<4>::index)
        .def("degree", &Tetrahedron<4>::degree)
        .def("embedding", &Tetrahedron<4>::embedding)
        .def("embeddings", &Tetrahedron<4>::embeddings)
        .def("front", &Tetrahedron<4>::front)
        .def("back", &Tetrahedron<4>::back)
        .def("triangulation", &Tetrahedron<4>::triangulation)
        .def("component", &Tetrahedron<4>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Tetrahedron<4>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Tetrahedron<4>, 3, int>)
        .def("vertex", &Tetrahedron<4>::vertex,
            pybind11::return_value_policy::reference)
        .def("edge", &Tetrahedron<4>::edge,
            pybind11::return_value_policy::reference)
        .def("triangle", &Tetrahedron<4>::triangle,
            pybind11::return_value_policy::reference)
        .def("faceMapping", &regina::python::faceMapping<Tetrahedron<4>, 3, 5>)
        .def("vertexMapping", &Tetrahedron<4>::vertexMapping)
        .def("edgeMapping", &Tetrahedron<4>::edgeMapping)
        .def("triangleMapping", &Tetrahedron<4>::triangleMapping)
        .def("isValid", &Tetrahedron<4>::isValid)
        .def("hasBadIdentification", &Tetrahedron<4>::hasBadIdentification)
        .def("hasBadLink", &Tetrahedron<4>::hasBadLink)
        .def("isLinkOrientable", &Tetrahedron<4>::isLinkOrientable)
        .def("isBoundary", &Tetrahedron<4>::isBoundary)
        .def("inMaximalForest", &Tetrahedron<4>::inMaximalForest)
        .def_static("ordering", &Tetrahedron<4>::ordering)
        .def_static("faceNumber", &Tetrahedron<4>::faceNumber)
        .def_static("containsVertex", &Tetrahedron<4>::containsVertex)
        .def_readonly_static("nFaces", &Tetrahedron<4>::nFaces)
        .def_readonly_static("lexNumbering", &Tetrahedron<4>::lexNumbering)
        .def_readonly_static("oppositeDim", &Tetrahedron<4>::oppositeDim)
        .def_readonly_static("dimension", &Tetrahedron<4>::dimension)
        .def_readonly_static("subdimension", &Tetrahedron<4>::subdimension)
    ;
    regina::python::add_output(c, regina::python::PYTHON_REPR_NONE);
    regina::python::add_eq_operators(c);

    regina::python::addListView<
        decltype(std::declval<Tetrahedron<4>>().embeddings())>(m);

    m.attr("TetrahedronEmbedding4") = m.attr("FaceEmbedding4_3");
    m.attr("Tetrahedron4") = m.attr("Face4_3");
}

