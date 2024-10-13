
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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/alias/face.h"
#include "../docstrings/triangulation/alias/facenumber.h"
#include "../docstrings/triangulation/dim3/triangle3.h"
#include "../docstrings/triangulation/detail/face.h"
#include "../docstrings/triangulation/detail/facenumbering.h"
#include "../docstrings/triangulation/generic/faceembedding.h"

using regina::Face;
using regina::FaceEmbedding;
using regina::Triangle;
using regina::TriangleEmbedding;

void addTriangle3(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)
    RDOC_SCOPE_BASE_3(detail::FaceEmbeddingBase, alias::FaceNumber,
        alias::SimplexVoid)

    auto e = pybind11::class_<FaceEmbedding<3, 2>>(m, "FaceEmbedding3_2",
            rdoc_scope)
        .def(pybind11::init<regina::Tetrahedron<3>*, regina::Perm<4>>(),
            rdoc::__init)
        .def(pybind11::init<const TriangleEmbedding<3>&>(), rdoc::__copy)
        .def("simplex", &TriangleEmbedding<3>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("tetrahedron", &TriangleEmbedding<3>::tetrahedron,
            pybind11::return_value_policy::reference, rbase3::tetrahedron)
        .def("face", &TriangleEmbedding<3>::face, rbase::face)
        .def("triangle", &TriangleEmbedding<3>::triangle, rbase2::triangle)
        .def("vertices", &TriangleEmbedding<3>::vertices, rbase::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e, rbase::__eq);

    RDOC_SCOPE_SWITCH(Face)
    RDOC_SCOPE_BASE_2(detail::FaceBase, detail::FaceNumberingAPI)

    auto c = pybind11::class_<Face<3, 2>>(m, "Face3_2", rdoc_scope)
        .def("index", &Triangle<3>::index, rbase::index)
        .def("embedding", &Triangle<3>::embedding, rbase::embedding)
        .def("embeddings", &Triangle<3>::embeddings, rbase::embeddings)
        .def("__iter__", [](const Triangle<3>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rbase::__iter__)
        .def("front", &Triangle<3>::front, rbase::front)
        .def("back", &Triangle<3>::back, rbase::back)
        .def("isBoundary", &Triangle<3>::isBoundary, rbase::isBoundary)
        .def("inMaximalForest", &Triangle<3>::inMaximalForest,
            rbase::inMaximalForest)
        .def("type", &Triangle<3>::type, rdoc::type)
        .def("subtype", &Triangle<3>::subtype, rdoc::subtype)
        .def("isMobiusBand", &Triangle<3>::isMobiusBand, rdoc::isMobiusBand)
        .def("isCone", &Triangle<3>::isCone, rdoc::isCone)
        .def("linkingSurface", &Triangle<3>::linkingSurface,
            rdoc::linkingSurface)
        .def("isValid", &Triangle<3>::isValid, rbase::isValid)
        .def("hasBadIdentification", &Triangle<3>::hasBadIdentification,
            rbase::hasBadIdentification)
        .def("hasBadLink", &Triangle<3>::hasBadLink, rbase::hasBadLink)
        .def("isLinkOrientable", &Triangle<3>::isLinkOrientable,
            rbase::isLinkOrientable)
        .def("degree", &Triangle<3>::degree, rbase::degree)
        .def("triangulation", &Triangle<3>::triangulation, rbase::triangulation)
        .def("component", &Triangle<3>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("boundaryComponent", &Triangle<3>::boundaryComponent,
            pybind11::return_value_policy::reference, rbase::boundaryComponent)
        .def("face", &regina::python::face<Triangle<3>, 2, int>,
            pybind11::arg("lowerdim"), pybind11::arg("face"),
            rbase::face)
        .def("vertex", &Triangle<3>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("edge", &Triangle<3>::edge,
            pybind11::return_value_policy::reference, rbase::edge)
        .def("faceMapping", &regina::python::faceMapping<Triangle<3>, 2, 4>,
            pybind11::arg("lowerdim"), pybind11::arg("face"),
            rbase::faceMapping)
        .def("vertexMapping", &Triangle<3>::vertexMapping, rbase::vertexMapping)
        .def("edgeMapping", &Triangle<3>::edgeMapping, rbase::edgeMapping)
        .def("lock", &Triangle<3>::lock, rbase::lock)
        .def("unlock", &Triangle<3>::unlock, rbase::unlock)
        .def("isLocked", &Triangle<3>::isLocked, rbase::isLocked)
        .def_static("ordering", &Triangle<3>::ordering)
        .def_static("faceNumber", &Triangle<3>::faceNumber)
        .def_static("containsVertex", &Triangle<3>::containsVertex)
        .def_readonly_static("nFaces", &Triangle<3>::nFaces)
        .def_readonly_static("lexNumbering", &Triangle<3>::lexNumbering)
        .def_readonly_static("oppositeDim", &Triangle<3>::oppositeDim)
        .def_readonly_static("dimension", &Triangle<3>::dimension)
        .def_readonly_static("subdimension", &Triangle<3>::subdimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    regina::python::addListView<
        decltype(std::declval<Triangle<3>>().embeddings())>(m);

    RDOC_SCOPE_INNER_BEGIN(Type)

    // Embed this enum in the Triangle3 class.
    pybind11::enum_<Triangle<3>::Type>(c, "Type", rdoc_inner_scope)
        .value("Unknown", Triangle<3>::Type::Unknown, rdoc_inner::Unknown)
        .value("Triangle", Triangle<3>::Type::Triangle, rdoc_inner::Triangle)
        .value("Scarf", Triangle<3>::Type::Scarf, rdoc_inner::Scarf)
        .value("Parachute", Triangle<3>::Type::Parachute, rdoc_inner::Parachute)
        .value("Cone", Triangle<3>::Type::Cone, rdoc_inner::Cone)
        .value("Mobius", Triangle<3>::Type::Mobius, rdoc_inner::Mobius)
        .value("Horn", Triangle<3>::Type::Horn, rdoc_inner::Horn)
        .value("DunceHat", Triangle<3>::Type::DunceHat, rdoc_inner::DunceHat)
        .value("L31", Triangle<3>::Type::L31, rdoc_inner::L31)
        ;

    // Deprecated constants:
    c.attr("UNKNOWN_TYPE") = Triangle<3>::Type::Unknown;
    c.attr("TRIANGLE") = Triangle<3>::Type::Triangle;
    c.attr("SCARF") = Triangle<3>::Type::Scarf;
    c.attr("PARACHUTE") = Triangle<3>::Type::Parachute;
    c.attr("CONE") = Triangle<3>::Type::Cone;
    c.attr("MOBIUS") = Triangle<3>::Type::Mobius;
    c.attr("HORN") = Triangle<3>::Type::Horn;
    c.attr("DUNCEHAT") = Triangle<3>::Type::DunceHat;
    c.attr("L31") = Triangle<3>::Type::L31;

    RDOC_SCOPE_INNER_END
    RDOC_SCOPE_END

    m.attr("TriangleEmbedding3") = m.attr("FaceEmbedding3_2");
    m.attr("Triangle3") = m.attr("Face3_2");
}

