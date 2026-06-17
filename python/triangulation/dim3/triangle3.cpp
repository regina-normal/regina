
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
#include <pybind11/stl.h>
#include "triangulation/dim3.h"
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/facenumbering.h"
#include "../../docstrings/triangulation/face.h"

using namespace pybind11::literals;

using regina::Face;
using regina::FaceEmbedding;
using regina::Triangle;
using regina::TriangleEmbedding;
using regina::TriangleType;

void addTriangle3(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)

    auto e = pybind11::class_<FaceEmbedding<3, 2>>(m, "FaceEmbedding3_2",
            rdoc::__class)
        .def(pybind11::init<regina::Tetrahedron<3>*, regina::Perm<4>>(),
            rdoc::__init)
        .def(pybind11::init<const TriangleEmbedding<3>&>(), rdoc::__copy)
        .def("simplex", &TriangleEmbedding<3>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex)
        // So: clang can resolve tetrahedron() but gcc cannot.
        // We could fix this with a static_cast, but we will just bind to
        // simplex() instead (which gcc _can_ resolve).
        .def("tetrahedron", &TriangleEmbedding<3>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex_dim3)
        .def("face", &TriangleEmbedding<3>::face, rdoc::face)
        // Likewise: clang can resolve triangle() but gcc cannot.
        // Bind to face() instead.
        .def("triangle", &TriangleEmbedding<3>::face, rdoc::triangle)
        .def("vertices", &TriangleEmbedding<3>::vertices, rdoc::vertices)
    ;
    regina::python::add_output_rich(e);
    regina::python::add_eq_operators(e, rdoc::__eq);

    RDOC_SCOPE_SWITCH(Face)
    RDOC_SCOPE_BASE(FaceNumbering)

    auto c = pybind11::class_<Face<3, 2>>(m, "Face3_2", rdoc::__class)
        .def("index", &Triangle<3>::index, rdoc::index)
        .def("embedding", &Triangle<3>::embedding, rdoc::embedding)
        .def("embeddings", &Triangle<3>::embeddings, rdoc::embeddings)
        .def("__iter__", [](const Triangle<3>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rdoc::__iter__)
        .def("front", &Triangle<3>::front, rdoc::front)
        .def("back", &Triangle<3>::back, rdoc::back)
        .def("isBoundary", &Triangle<3>::isBoundary, rdoc::isBoundary)
        .def("isInternal", &Triangle<3>::isInternal, rdoc::isInternal)
        .def("inMaximalForest", &Triangle<3>::inMaximalForest,
            rdoc::inMaximalForest)
        .def("linkingSurface",
            static_cast<regina::python::generalLinkingSurface<3, 2>>(
                &Triangle<3>::linkingSurface),
            rdoc::linkingSurface)
        .def("isValid", &Triangle<3>::isValid, rdoc::isValid)
        .def("hasBadIdentification", &Triangle<3>::hasBadIdentification,
            rdoc::hasBadIdentification)
        .def("hasBadLink", &Triangle<3>::hasBadLink, rdoc::hasBadLink)
        .def("isLinkOrientable", &Triangle<3>::isLinkOrientable,
            rdoc::isLinkOrientable)
        .def("degree", &Triangle<3>::degree, rdoc::degree)
        .def("triangulation", &Triangle<3>::triangulation, rdoc::triangulation)
        .def("component", &Triangle<3>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("boundaryComponent", &Triangle<3>::boundaryComponent,
            pybind11::return_value_policy::reference, rdoc::boundaryComponent)
        .def("face", &regina::python::face<Triangle<3>>,
            "lowerdim"_a, "face"_a, rdoc::face)
        .def("vertex", &Triangle<3>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("edge", &Triangle<3>::edge,
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("faceMapping", &regina::python::faceMapping<Triangle<3>>,
            "lowerdim"_a, "face"_a, rdoc::faceMapping)
        .def("vertexMapping", &Triangle<3>::vertexMapping, rdoc::vertexMapping)
        .def("edgeMapping", &Triangle<3>::edgeMapping, rdoc::edgeMapping)
        .def("join", &Triangle<3>::join, rdoc::join)
        .def("triangleType", &Triangle<3>::triangleType, rdoc::triangleType)
        .def("triangleSubtype", &Triangle<3>::triangleSubtype,
            rdoc::triangleSubtype)
        .def("formsMobiusBand", &Triangle<3>::formsMobiusBand,
            rdoc::formsMobiusBand)
        .def("formsCone", &Triangle<3>::formsCone, rdoc::formsCone)
        .def("lock", &Triangle<3>::lock, rdoc::lock)
        .def("unlock", &Triangle<3>::unlock, rdoc::unlock)
        .def("isLocked", &Triangle<3>::isLocked, rdoc::isLocked)
        .def_static("ordering", &Triangle<3>::ordering, rbase::ordering)
        .def_static("faceNumber",
            pybind11::overload_cast<regina::Perm<4>>(&Triangle<3>::faceNumber),
            rbase::faceNumber)
        .def_static("containsVertex", &Triangle<3>::containsVertex,
            rbase::containsVertex)
        .def_readonly_static("nFaces", &Triangle<3>::nFaces)
        .def_readonly_static("lexNumbering", &Triangle<3>::lexNumbering)
        .def_readonly_static("oppositeDim", &Triangle<3>::oppositeDim)
        .def_readonly_static("dimension", &Triangle<3>::dimension)
        .def_readonly_static("subdimension", &Triangle<3>::subdimension)
        .def_readonly_static("hasNumberingTables",
            &Triangle<3>::hasNumberingTables)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    regina::python::addStdView<
        decltype(std::declval<Triangle<3>>().embeddings())>(internal,
        "Face3_2_embeddings");

    RDOC_SCOPE_END

    m.attr("TriangleEmbedding3") = m.attr("FaceEmbedding3_2");
    m.attr("Triangle3") = m.attr("Face3_2");
}

