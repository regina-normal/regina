
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "../helpers/tableview.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/alias/face.h"
#include "../docstrings/triangulation/alias/facenumber.h"
#include "../docstrings/triangulation/dim4/triangle4.h"
#include "../docstrings/triangulation/detail/face.h"
#include "../docstrings/triangulation/detail/facenumbering.h"
#include "../docstrings/triangulation/generic/faceembedding.h"

using regina::Triangle;
using regina::TriangleEmbedding;
using regina::Face;
using regina::FaceEmbedding;
using regina::python::wrapTableView;

void addTriangle4(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)
    RDOC_SCOPE_BASE_3(detail::FaceEmbeddingBase, alias::FaceNumber,
        alias::SimplexVoid)

    auto e = pybind11::class_<FaceEmbedding<4, 2>>(m, "FaceEmbedding4_2",
            rdoc_scope)
        .def(pybind11::init<regina::Pentachoron<4>*, regina::Perm<5>>(),
            rdoc::__init)
        .def(pybind11::init<const TriangleEmbedding<4>&>(), rdoc::__copy)
        .def("simplex", &TriangleEmbedding<4>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("pentachoron", &TriangleEmbedding<4>::pentachoron,
            pybind11::return_value_policy::reference, rbase3::pentachoron)
        .def("face", &TriangleEmbedding<4>::face, rbase::face)
        .def("triangle", &TriangleEmbedding<4>::triangle, rbase2::triangle)
        .def("vertices", &TriangleEmbedding<4>::vertices, rbase::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e, rbase::__eq, rbase::__ne);

    RDOC_SCOPE_SWITCH(Face)
    RDOC_SCOPE_BASE_2(detail::FaceBase, detail::FaceNumberingAPI)

    auto c = pybind11::class_<Face<4, 2>>(m, "Face4_2")
        .def("index", &Triangle<4>::index)
        .def("embedding", &Triangle<4>::embedding)
        .def("embeddings", &Triangle<4>::embeddings)
        .def("__iter__", [](const Triangle<4>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>()) // iterator keeps Face alive
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
        .def("linkingSurface", &Triangle<4>::linkingSurface)
        .def_static("ordering", &Triangle<4>::ordering)
        .def_static("faceNumber", &Triangle<4>::faceNumber)
        .def_static("containsVertex", &Triangle<4>::containsVertex)
        .def_readonly_static("nFaces", &Triangle<4>::nFaces)
        .def_readonly_static("lexNumbering", &Triangle<4>::lexNumbering)
        .def_readonly_static("oppositeDim", &Triangle<4>::oppositeDim)
        .def_readonly_static("dimension", &Triangle<4>::dimension)
        .def_readonly_static("subdimension", &Triangle<4>::subdimension)
    ;

    c.attr("triangleNumber") = wrapTableView(m, Triangle<4>::triangleNumber);
    c.attr("triangleVertex") = wrapTableView(m, Triangle<4>::triangleVertex);

    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addListView<
        decltype(std::declval<Triangle<4>>().embeddings())>(m);

    m.attr("TriangleEmbedding4") = m.attr("FaceEmbedding4_2");
    m.attr("Triangle4") = m.attr("Face4_2");
}

