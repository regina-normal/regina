
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
#include "triangulation/dim2.h"
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../helpers/tableview.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/alias/face.h"
#include "../docstrings/triangulation/alias/facenumber.h"
#include "../docstrings/triangulation/dim4/edge4.h"
#include "../docstrings/triangulation/detail/face.h"
#include "../docstrings/triangulation/detail/facenumbering.h"
#include "../docstrings/triangulation/generic/faceembedding.h"

using regina::Edge;
using regina::EdgeEmbedding;
using regina::Face;
using regina::FaceEmbedding;
using regina::python::wrapTableView;

void addEdge4(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)
    RDOC_SCOPE_BASE_3(detail::FaceEmbeddingBase, alias::FaceNumber,
        alias::SimplexVoid)

    auto e = pybind11::class_<FaceEmbedding<4, 1>>(m, "FaceEmbedding4_1",
            rdoc_scope)
        .def(pybind11::init<regina::Pentachoron<4>*, regina::Perm<5>>(),
            rdoc::__init)
        .def(pybind11::init<const EdgeEmbedding<4>&>(), rdoc::__copy)
        .def("simplex", &EdgeEmbedding<4>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("pentachoron", &EdgeEmbedding<4>::pentachoron,
            pybind11::return_value_policy::reference, rbase3::pentachoron)
        .def("face", &EdgeEmbedding<4>::face, rbase::face)
        .def("edge", &EdgeEmbedding<4>::edge, rbase2::edge)
        .def("vertices", &EdgeEmbedding<4>::vertices, rbase::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e, rbase::__eq);

    RDOC_SCOPE_SWITCH(Face)
    RDOC_SCOPE_BASE_2(detail::FaceBase, detail::FaceNumberingAPI)

    auto c = pybind11::class_<Face<4, 1>>(m, "Face4_1", rdoc_scope)
        .def("index", &Edge<4>::index, rbase::index)
        .def("embedding", &Edge<4>::embedding, rbase::embedding)
        .def("embeddings", &Edge<4>::embeddings, rbase::embeddings)
        .def("__iter__", [](const Edge<4>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rbase::__iter__)
        .def("front", &Edge<4>::front, rbase::front)
        .def("back", &Edge<4>::back, rbase::back)
        .def("triangulation", &Edge<4>::triangulation, rbase::triangulation)
        .def("component", &Edge<4>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("boundaryComponent", &Edge<4>::boundaryComponent,
            pybind11::return_value_policy::reference, rbase::boundaryComponent)
        .def("face", &regina::python::face<Edge<4>, 1, int>,
            pybind11::arg("lowerdim"), pybind11::arg("face"),
            rbase::face)
        .def("vertex", &Edge<4>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("faceMapping", &regina::python::faceMapping<Edge<4>, 1, 5>,
            pybind11::arg("lowerdim"), pybind11::arg("face"),
            rbase::faceMapping)
        .def("vertexMapping", &Edge<4>::vertexMapping, rbase::vertexMapping)
        .def("degree", &Edge<4>::degree, rbase::degree)
        .def("isBoundary", &Edge<4>::isBoundary, rbase::isBoundary)
        .def("isLinkOrientable", &Edge<4>::isLinkOrientable,
            rbase::isLinkOrientable)
        .def("isValid", &Edge<4>::isValid, rbase::isValid)
        .def("hasBadIdentification", &Edge<4>::hasBadIdentification,
            rbase::hasBadIdentification)
        .def("hasBadLink", &Edge<4>::hasBadLink, rbase::hasBadLink)
        .def("buildLink", [](const Edge<4>& e) {
            // Return a clone of the resulting triangulation.
            // This is because Python cannot enforce the constness of
            // the reference that would normally be returned.
            return new regina::Triangulation<2>(e.buildLink());
        }, rdoc::buildLink)
        .def("buildLinkInclusion", &Edge<4>::buildLinkInclusion,
            rdoc::buildLinkInclusion)
        .def("linkingSurface", &Edge<4>::linkingSurface, rdoc::linkingSurface)
        .def_static("ordering", &Edge<4>::ordering)
        .def_static("faceNumber", &Edge<4>::faceNumber)
        .def_static("containsVertex", &Edge<4>::containsVertex)
        .def_readonly_static("nFaces", &Edge<4>::nFaces)
        .def_readonly_static("lexNumbering", &Edge<4>::lexNumbering)
        .def_readonly_static("oppositeDim", &Edge<4>::oppositeDim)
        .def_readonly_static("dimension", &Edge<4>::dimension)
        .def_readonly_static("subdimension", &Edge<4>::subdimension)
    ;

    c.attr("edgeNumber") = wrapTableView(m, Edge<4>::edgeNumber);
    c.attr("edgeVertex") = wrapTableView(m, Edge<4>::edgeVertex);

    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addListView<
        decltype(std::declval<Edge<4>>().embeddings())>(m);

    m.attr("EdgeEmbedding4") = m.attr("FaceEmbedding4_1");
    m.attr("Edge4") = m.attr("Face4_1");
}

