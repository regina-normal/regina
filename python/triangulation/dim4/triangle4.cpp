
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
#include "../../helpers/tableview.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/facenumbering.h"
#include "../../docstrings/triangulation/face.h"

using namespace pybind11::literals;

using regina::Triangle;
using regina::TriangleEmbedding;
using regina::Face;
using regina::FaceEmbedding;
using regina::python::wrapTableView;

void addTriangle4(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)

    auto e = pybind11::class_<FaceEmbedding<4, 2>>(m, "FaceEmbedding4_2",
            rdoc::__class)
        .def(pybind11::init<regina::Pentachoron<4>*, regina::Perm<5>>(),
            rdoc::__init)
        .def(pybind11::init<const TriangleEmbedding<4>&>(), rdoc::__copy)
        .def("simplex", &TriangleEmbedding<4>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex)
        // So: clang can resolve pentachoron() but gcc cannot.
        // We could fix this with a static_cast, but we will just bind to
        // simplex() instead (which gcc _can_ resolve).
        .def("pentachoron", &TriangleEmbedding<4>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex_dim4)
        .def("face", &TriangleEmbedding<4>::face, rdoc::face)
        // Likewise: clang can resolve triangle() but gcc cannot.
        // Bind to face() instead.
        .def("triangle", &TriangleEmbedding<4>::face, rdoc::triangle)
        .def("vertices", &TriangleEmbedding<4>::vertices, rdoc::vertices)
    ;
    regina::python::add_output_rich(e);
    regina::python::add_eq_operators(e, rdoc::__eq);

    RDOC_SCOPE_SWITCH(Face)
    RDOC_SCOPE_BASE(FaceNumbering)

    auto c = pybind11::class_<Face<4, 2>>(m, "Face4_2", rdoc::Face::__class)
        .def("index", &Triangle<4>::index, rdoc::index)
        .def("embedding", &Triangle<4>::embedding, rdoc::embedding)
        .def("embeddings", &Triangle<4>::embeddings, rdoc::embeddings)
        .def("__iter__", [](const Triangle<4>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rdoc::__iter__)
        .def("front", &Triangle<4>::front, rdoc::front)
        .def("back", &Triangle<4>::back, rdoc::back)
        .def("triangulation", &Triangle<4>::triangulation, rdoc::triangulation)
        .def("component", &Triangle<4>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("boundaryComponent", &Triangle<4>::boundaryComponent,
            pybind11::return_value_policy::reference, rdoc::boundaryComponent)
        .def("face", &regina::python::face<4, 2>,
            "lowerdim"_a, "face"_a, rdoc::face)
        .def("vertex", &Triangle<4>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("edge", &Triangle<4>::edge,
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("faceMapping", &regina::python::faceMapping<4, 2>,
            "lowerdim"_a, "face"_a, rdoc::faceMapping)
        .def("vertexMapping", &Triangle<4>::vertexMapping, rdoc::vertexMapping)
        .def("edgeMapping", &Triangle<4>::edgeMapping, rdoc::edgeMapping)
        .def("triangleType", &Triangle<4>::triangleType, rdoc::triangleType)
        .def("triangleSubtype", &Triangle<4>::triangleSubtype,
            rdoc::triangleSubtype)
        .def("formsMobiusBand", &Triangle<4>::formsMobiusBand,
            rdoc::formsMobiusBand)
        .def("formsCone", &Triangle<4>::formsCone, rdoc::formsCone)
        .def("degree", &Triangle<4>::degree, rdoc::degree)
        .def("isBoundary", &Triangle<4>::isBoundary, rdoc::isBoundary)
        .def("isInternal", &Triangle<4>::isInternal, rdoc::isInternal)
        .def("isLinkOrientable", &Triangle<4>::isLinkOrientable,
            rdoc::isLinkOrientable)
        .def("isValid", &Triangle<4>::isValid, rdoc::isValid)
        .def("hasBadIdentification", &Triangle<4>::hasBadIdentification,
            rdoc::hasBadIdentification)
        .def("hasBadLink", &Triangle<4>::hasBadLink, rdoc::hasBadLink)
        .def("isLinkClosed", &Triangle<4>::isLinkClosed, rdoc::isLinkClosed)
        .def("linkingSurface",
            static_cast<regina::python::generalLinkingSurface<4, 2>>(
                &Triangle<4>::linkingSurface),
            rdoc::linkingSurface)
        .def_static("ordering", &Triangle<4>::ordering, rbase::ordering)
        .def_static("faceNumber",
            pybind11::overload_cast<regina::Perm<5>>(&Triangle<4>::faceNumber),
            rbase::faceNumber)
        .def_static("containsVertex", &Triangle<4>::containsVertex,
            rbase::containsVertex)
        .def_readonly_static("nFaces", &Triangle<4>::nFaces)
        .def_readonly_static("lexNumbering", &Triangle<4>::lexNumbering)
        .def_readonly_static("oppositeDim", &Triangle<4>::oppositeDim)
        .def_readonly_static("dimension", &Triangle<4>::dimension)
        .def_readonly_static("subdimension", &Triangle<4>::subdimension)
        .def_readonly_static("hasNumberingTables",
            &Triangle<4>::hasNumberingTables)
    ;

    c.attr("triangleNumber") = wrapTableView(internal,
        Triangle<4>::triangleNumber);
    c.attr("triangleVertex") = wrapTableView(internal,
        Triangle<4>::triangleVertex);

    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addStdView<
        decltype(std::declval<Triangle<4>>().embeddings())>(internal,
        "Face4_2_embeddings");

    m.attr("TriangleEmbedding4") = m.attr("FaceEmbedding4_2");
    m.attr("Triangle4") = m.attr("Face4_2");
}

