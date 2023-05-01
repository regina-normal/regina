
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
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/alias/face.h"
#include "../docstrings/triangulation/alias/facenumber.h"
#include "../docstrings/triangulation/dim4/tetrahedron4.h"
#include "../docstrings/triangulation/detail/face.h"
#include "../docstrings/triangulation/detail/facenumbering.h"
#include "../docstrings/triangulation/generic/faceembedding.h"

using regina::Tetrahedron;
using regina::TetrahedronEmbedding;
using regina::Face;
using regina::FaceEmbedding;

void addTetrahedron4(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)
    RDOC_SCOPE_BASE_3(detail::FaceEmbeddingBase, alias::FaceNumber,
        alias::SimplexVoid)

    auto e = pybind11::class_<FaceEmbedding<4, 3>>(m, "FaceEmbedding4_3",
            rdoc_scope)
        .def(pybind11::init<regina::Pentachoron<4>*, regina::Perm<5>>(),
            rdoc::__init)
        .def(pybind11::init<const TetrahedronEmbedding<4>&>(), rdoc::__copy)
        .def("simplex", &TetrahedronEmbedding<4>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("pentachoron", &TetrahedronEmbedding<4>::pentachoron,
            pybind11::return_value_policy::reference, rbase3::pentachoron)
        .def("face", &TetrahedronEmbedding<4>::face, rbase::face)
        .def("tetrahedron", &TetrahedronEmbedding<4>::tetrahedron,
            rbase2::tetrahedron)
        .def("vertices", &TetrahedronEmbedding<4>::vertices, rbase::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e, rbase::__eq, rbase::__ne);

    RDOC_SCOPE_SWITCH(Face)
    RDOC_SCOPE_BASE_2(detail::FaceBase, detail::FaceNumberingAPI)

    auto c = pybind11::class_<Face<4, 3>>(m, "Face4_3", rdoc_scope)
        .def("index", &Tetrahedron<4>::index, rbase::index)
        .def("degree", &Tetrahedron<4>::degree, rbase::degree)
        .def("embedding", &Tetrahedron<4>::embedding, rbase::embedding)
        .def("embeddings", &Tetrahedron<4>::embeddings, rbase::embeddings)
        .def("__iter__", [](const Tetrahedron<4>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rbase::__iter__)
        .def("front", &Tetrahedron<4>::front, rbase::front)
        .def("back", &Tetrahedron<4>::back, rbase::back)
        .def("triangulation", &Tetrahedron<4>::triangulation,
            rbase::triangulation)
        .def("component", &Tetrahedron<4>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("boundaryComponent", &Tetrahedron<4>::boundaryComponent,
            pybind11::return_value_policy::reference, rbase::boundaryComponent)
        .def("face", &regina::python::face<Tetrahedron<4>, 3, int>,
            pybind11::arg("lowerdim"), pybind11::arg("face"),
            rbase::face)
        .def("vertex", &Tetrahedron<4>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("edge", &Tetrahedron<4>::edge,
            pybind11::return_value_policy::reference, rbase::edge)
        .def("triangle", &Tetrahedron<4>::triangle,
            pybind11::return_value_policy::reference, rbase::triangle)
        .def("faceMapping", &regina::python::faceMapping<Tetrahedron<4>, 3, 5>,
            pybind11::arg("lowerdim"), pybind11::arg("face"),
            rbase::faceMapping)
        .def("vertexMapping", &Tetrahedron<4>::vertexMapping,
            rbase::vertexMapping)
        .def("edgeMapping", &Tetrahedron<4>::edgeMapping, rbase::edgeMapping)
        .def("triangleMapping", &Tetrahedron<4>::triangleMapping,
            rbase::triangleMapping)
        .def("lock", &Tetrahedron<4>::lock, rbase::lock)
        .def("unlock", &Tetrahedron<4>::unlock, rbase::unlock)
        .def("isLocked", &Tetrahedron<4>::isLocked, rbase::isLocked)
        .def("isValid", &Tetrahedron<4>::isValid, rbase::isValid)
        .def("hasBadIdentification", &Tetrahedron<4>::hasBadIdentification,
            rbase::hasBadIdentification)
        .def("hasBadLink", &Tetrahedron<4>::hasBadLink, rbase::hasBadLink)
        .def("isLinkOrientable", &Tetrahedron<4>::isLinkOrientable,
            rbase::isLinkOrientable)
        .def("isBoundary", &Tetrahedron<4>::isBoundary, rbase::isBoundary)
        .def("inMaximalForest", &Tetrahedron<4>::inMaximalForest,
            rbase::inMaximalForest)
        .def("linkingSurface", &Tetrahedron<4>::linkingSurface,
            rdoc::linkingSurface)
        .def_static("ordering", &Tetrahedron<4>::ordering)
        .def_static("faceNumber", &Tetrahedron<4>::faceNumber)
        .def_static("containsVertex", &Tetrahedron<4>::containsVertex)
        .def_readonly_static("nFaces", &Tetrahedron<4>::nFaces)
        .def_readonly_static("lexNumbering", &Tetrahedron<4>::lexNumbering)
        .def_readonly_static("oppositeDim", &Tetrahedron<4>::oppositeDim)
        .def_readonly_static("dimension", &Tetrahedron<4>::dimension)
        .def_readonly_static("subdimension", &Tetrahedron<4>::subdimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addListView<
        decltype(std::declval<Tetrahedron<4>>().embeddings())>(m);

    m.attr("TetrahedronEmbedding4") = m.attr("FaceEmbedding4_3");
    m.attr("Tetrahedron4") = m.attr("Face4_3");
}

