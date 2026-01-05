
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "regina-config.h" // for REGINA_PYBIND11_VERSION
#include "pybind11/pybind11.h"
#if REGINA_PYBIND11_VERSION == 3
#include <pybind11/native_enum.h>
#endif
#include <pybind11/stl.h>
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/triangulation/alias/face.h"
#include "../docstrings/triangulation/alias/facenumber.h"
#include "../docstrings/triangulation/dim3/vertex3.h"
#include "../docstrings/triangulation/detail/face.h"
#include "../docstrings/triangulation/detail/facenumbering.h"
#include "../docstrings/triangulation/generic/faceembedding.h"

using regina::Face;
using regina::FaceEmbedding;
using regina::Vertex;
using regina::VertexEmbedding;

void addVertex3(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)
    RDOC_SCOPE_BASE_3(detail::FaceEmbeddingBase, alias::FaceNumber,
        alias::SimplexVoid)

    auto e = pybind11::class_<FaceEmbedding<3, 0>>(m, "FaceEmbedding3_0",
            rdoc_scope)
        .def(pybind11::init<regina::Tetrahedron<3>*, regina::Perm<4>>(),
            rdoc::__init)
        .def(pybind11::init<const VertexEmbedding<3>&>(), rdoc::__copy)
        .def("simplex", &VertexEmbedding<3>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("tetrahedron", &VertexEmbedding<3>::tetrahedron,
            pybind11::return_value_policy::reference, rbase3::tetrahedron)
        .def("face", &VertexEmbedding<3>::face, rbase::face)
        .def("vertex", &VertexEmbedding<3>::vertex, rbase2::vertex)
        .def("vertices", &VertexEmbedding<3>::vertices, rbase::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e, rbase::__eq);

    RDOC_SCOPE_SWITCH(Face)
    RDOC_SCOPE_BASE_2(detail::FaceBase, detail::FaceNumberingAPI)

    auto c = pybind11::class_<Face<3, 0>>(m, "Face3_0", rdoc_scope)
        .def("index", &Vertex<3>::index, rbase::index)
        .def("embedding", &Vertex<3>::embedding, rbase::embedding)
        .def("embeddings", &Vertex<3>::embeddings, rbase::embeddings)
        .def("__iter__", [](const Vertex<3>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rbase::__iter__)
        .def("front", &Vertex<3>::front, rbase::front)
        .def("back", &Vertex<3>::back, rbase::back)
        .def("triangulation", &Vertex<3>::triangulation, rbase::triangulation)
        .def("component", &Vertex<3>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("boundaryComponent", &Vertex<3>::boundaryComponent,
            pybind11::return_value_policy::reference, rbase::boundaryComponent)
        .def("degree", &Vertex<3>::degree, rbase::degree)
        .def("linkType", &Vertex<3>::linkType, rdoc::linkType)
        .def("link", [](const Vertex<3>&) -> Vertex<3>::Link {
            // Removed completely in v7.0.  C++ does not get a deprecated
            // alias.  Keep this here for Python users for a little while so
            // that people can see what needs to be done.  Probably we can
            // remove this placeholder routine in Regina 8.0.
            throw std::runtime_error(
                "Vertex3::link() has been renamed to Vertex3::linkType().  "
                "You should change your code now, because the name link() "
                "will be used for a different function in the future.");
        }, R"doc(Old routine that was renamed in Regina 7.0.

Please update your code to use Vertex3.linkType() instead, which does
exactly what Vertex3.link() used to do in Regina 6.0.1 and earlier.
The name link() has _not_ been kept as an alias because it is now
being reserved for a different purpose in a future release.)doc")
        .def("buildLink", [](const Vertex<3>& v) {
            // Return a clone of the resulting triangulation.
            // This is because Python cannot enforce the constness of
            // the reference that would normally be returned.
            return new regina::Triangulation<2>(v.buildLink());
        }, rdoc::buildLink)
        .def("buildLinkInclusion", &Vertex<3>::buildLinkInclusion,
            rdoc::buildLinkInclusion)
        .def("isLinkClosed", &Vertex<3>::isLinkClosed, rdoc::isLinkClosed)
        .def("isIdeal", &Vertex<3>::isIdeal, rdoc::isIdeal)
        .def("isBoundary", &Vertex<3>::isBoundary, rbase::isBoundary)
        .def("isStandard", &Vertex<3>::isStandard, rdoc::isStandard)
        .def("isValid", &Vertex<3>::isValid, rbase::isValid)
        .def("hasBadIdentification", &Vertex<3>::hasBadIdentification,
            rbase::hasBadIdentification)
        .def("hasBadLink", &Vertex<3>::hasBadLink, rbase::hasBadLink)
        .def("isLinkOrientable", &Vertex<3>::isLinkOrientable,
            rbase::isLinkOrientable)
        .def("linkEulerChar", &Vertex<3>::linkEulerChar, rdoc::linkEulerChar)
        .def("linkingSurface", &Vertex<3>::linkingSurface, rdoc::linkingSurface)
        .def_static("ordering", &Vertex<3>::ordering)
        .def_static("faceNumber", &Vertex<3>::faceNumber)
        .def_static("containsVertex", &Vertex<3>::containsVertex)
        .def_readonly_static("nFaces", &Vertex<3>::nFaces)
        .def_readonly_static("lexNumbering", &Vertex<3>::lexNumbering)
        .def_readonly_static("oppositeDim", &Vertex<3>::oppositeDim)
        .def_readonly_static("dimension", &Vertex<3>::dimension)
        .def_readonly_static("subdimension", &Vertex<3>::subdimension)
        .def_readonly_static("hasNumberingTables",
            &Vertex<3>::hasNumberingTables)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    regina::python::addListView<
        decltype(std::declval<Vertex<3>>().embeddings())>(internal,
        "Face3_0_embeddings");

    RDOC_SCOPE_INNER_BEGIN(Link)

#if REGINA_PYBIND11_VERSION == 3
    pybind11::native_enum<regina::Vertex<3>::Link>(c, "Link", "enum.Enum",
            rdoc_inner_scope)
#elif REGINA_PYBIND11_VERSION == 2
    pybind11::enum_<regina::Vertex<3>::Link>(c, "Link", rdoc_inner_scope)
#else
    #error "Unsupported pybind11 version"
#endif
        .value("Sphere", regina::Vertex<3>::Link::Sphere, rdoc_inner::Sphere)
        .value("Disc", regina::Vertex<3>::Link::Disc, rdoc_inner::Disc)
        .value("Torus", regina::Vertex<3>::Link::Torus, rdoc_inner::Torus)
        .value("KleinBottle", regina::Vertex<3>::Link::KleinBottle,
            rdoc_inner::KleinBottle)
        .value("NonStandardCusp", regina::Vertex<3>::Link::NonStandardCusp,
            rdoc_inner::NonStandardCusp)
        .value("Invalid", regina::Vertex<3>::Link::Invalid,
            rdoc_inner::Invalid)
#if REGINA_PYBIND11_VERSION == 3
        .finalize()
#endif
        ;

    // Deprecated type alias and constants:
    c.attr("LinkType") = c.attr("Link");
    c.attr("SPHERE") = regina::Vertex<3>::Link::Sphere;
    c.attr("DISC") = regina::Vertex<3>::Link::Disc;
    c.attr("TORUS") = regina::Vertex<3>::Link::Torus;
    c.attr("KLEIN_BOTTLE") = regina::Vertex<3>::Link::KleinBottle;
    c.attr("NON_STANDARD_CUSP") = regina::Vertex<3>::Link::NonStandardCusp;
    c.attr("INVALID") = regina::Vertex<3>::Link::Invalid;

    RDOC_SCOPE_INNER_END
    RDOC_SCOPE_END

    m.attr("VertexEmbedding3") = m.attr("FaceEmbedding3_0");
    m.attr("Vertex3") = m.attr("Face3_0");
}

