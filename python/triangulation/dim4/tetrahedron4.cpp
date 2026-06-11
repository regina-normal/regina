
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include "triangulation/dim4.h"
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/facenumbering.h"
#include "../../docstrings/triangulation/face.h"

using namespace pybind11::literals;

using regina::Tetrahedron;
using regina::TetrahedronEmbedding;
using regina::Face;
using regina::FaceEmbedding;

void addTetrahedron4(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)

    auto e = pybind11::class_<FaceEmbedding<4, 3>>(m, "FaceEmbedding4_3",
            rdoc::__class)
        .def(pybind11::init<regina::Pentachoron<4>*, regina::Perm<5>>(),
            rdoc::__init)
        .def(pybind11::init<const TetrahedronEmbedding<4>&>(), rdoc::__copy)
        .def("simplex", &TetrahedronEmbedding<4>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex)
        // So: clang can resolve pentachoron() but gcc cannot.
        // We could fix this with a static_cast, but we will just bind to
        // simplex() instead (which gcc _can_ resolve).
        .def("pentachoron", &TetrahedronEmbedding<4>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex_dim4)
        .def("face", &TetrahedronEmbedding<4>::face, rdoc::face)
        // Likewise: clang can resolve tetrahedron() but gcc cannot.
        // Bind to face() instead.
        .def("tetrahedron", &TetrahedronEmbedding<4>::face,
            rdoc::tetrahedron)
        .def("vertices", &TetrahedronEmbedding<4>::vertices, rdoc::vertices)
    ;
    regina::python::add_output_rich(e);
    regina::python::add_eq_operators(e, rdoc::__eq);

    RDOC_SCOPE_SWITCH(Face)
    RDOC_SCOPE_BASE(FaceNumbering)

    auto c = pybind11::class_<Face<4, 3>>(m, "Face4_3", rdoc::Face::__class)
        .def("index", &Tetrahedron<4>::index, rdoc::index)
        .def("degree", &Tetrahedron<4>::degree, rdoc::degree)
        .def("embedding", &Tetrahedron<4>::embedding, rdoc::embedding)
        .def("embeddings", &Tetrahedron<4>::embeddings, rdoc::embeddings)
        .def("__iter__", [](const Tetrahedron<4>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rdoc::__iter__)
        .def("front", &Tetrahedron<4>::front, rdoc::front)
        .def("back", &Tetrahedron<4>::back, rdoc::back)
        .def("triangulation", &Tetrahedron<4>::triangulation,
            rdoc::triangulation)
        .def("component", &Tetrahedron<4>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("boundaryComponent", &Tetrahedron<4>::boundaryComponent,
            pybind11::return_value_policy::reference, rdoc::boundaryComponent)
        .def("face", &regina::python::face<Tetrahedron<4>>,
            "lowerdim"_a, "face"_a, rdoc::face)
        .def("vertex", &Tetrahedron<4>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("edge", &Tetrahedron<4>::edge,
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("triangle", &Tetrahedron<4>::triangle,
            pybind11::return_value_policy::reference, rdoc::triangle)
        .def("faceMapping", &regina::python::faceMapping<Tetrahedron<4>>,
            "lowerdim"_a, "face"_a, rdoc::faceMapping)
        .def("vertexMapping", &Tetrahedron<4>::vertexMapping,
            rdoc::vertexMapping)
        .def("edgeMapping", &Tetrahedron<4>::edgeMapping, rdoc::edgeMapping)
        .def("triangleMapping", &Tetrahedron<4>::triangleMapping,
            rdoc::triangleMapping)
        .def("join", &Tetrahedron<4>::join, rdoc::join)
        .def("lock", &Tetrahedron<4>::lock, rdoc::lock)
        .def("unlock", &Tetrahedron<4>::unlock, rdoc::unlock)
        .def("isLocked", &Tetrahedron<4>::isLocked, rdoc::isLocked)
        .def("isValid", &Tetrahedron<4>::isValid, rdoc::isValid)
        .def("hasBadIdentification", &Tetrahedron<4>::hasBadIdentification,
            rdoc::hasBadIdentification)
        .def("hasBadLink", &Tetrahedron<4>::hasBadLink, rdoc::hasBadLink)
        .def("isLinkOrientable", &Tetrahedron<4>::isLinkOrientable,
            rdoc::isLinkOrientable)
        .def("isBoundary", &Tetrahedron<4>::isBoundary, rdoc::isBoundary)
        .def("isInternal", &Tetrahedron<4>::isInternal, rdoc::isInternal)
        .def("inMaximalForest", &Tetrahedron<4>::inMaximalForest,
            rdoc::inMaximalForest)
        .def("linkingSurface",
            static_cast<regina::python::generalLinkingSurface<4, 3>>(
                &Tetrahedron<4>::linkingSurface),
            rdoc::linkingSurface)
        .def_static("ordering", &Tetrahedron<4>::ordering, rbase::ordering)
        .def_static("faceNumber",
            pybind11::overload_cast<regina::Perm<5>>(
                &Tetrahedron<4>::faceNumber),
            rbase::faceNumber)
        .def_static("containsVertex", &Tetrahedron<4>::containsVertex,
            rbase::containsVertex)
        .def_readonly_static("nFaces", &Tetrahedron<4>::nFaces)
        .def_readonly_static("lexNumbering", &Tetrahedron<4>::lexNumbering)
        .def_readonly_static("oppositeDim", &Tetrahedron<4>::oppositeDim)
        .def_readonly_static("dimension", &Tetrahedron<4>::dimension)
        .def_readonly_static("subdimension", &Tetrahedron<4>::subdimension)
        .def_readonly_static("hasNumberingTables",
            &Tetrahedron<4>::hasNumberingTables)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addStdView<
        decltype(std::declval<Tetrahedron<4>>().embeddings())>(internal,
        "Face4_3_embeddings");

    m.attr("TetrahedronEmbedding4") = m.attr("FaceEmbedding4_3");
    m.attr("Tetrahedron4") = m.attr("Face4_3");
}

