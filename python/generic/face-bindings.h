
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
#include "triangulation/generic.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/alias/facenumber.h"
#include "../docstrings/triangulation/generic/face.h"
#include "../docstrings/triangulation/generic/faceembedding.h"
#include "../docstrings/triangulation/detail/face.h"
#include "../docstrings/triangulation/detail/facenumbering.h"

using regina::Face;
using regina::FaceEmbedding;

template <int dim, int subdim>
void addFace(pybind11::module_& m, const char* name, const char* embName) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)
    RDOC_SCOPE_BASE_2(detail::FaceEmbeddingBase, alias::FaceNumber)

    auto e = pybind11::class_<FaceEmbedding<dim, subdim>>(m, embName,
            rdoc_scope)
        .def(pybind11::init<regina::Simplex<dim>*, regina::Perm<dim + 1>>(),
            rdoc::__init)
        .def(pybind11::init<const FaceEmbedding<dim, subdim>&>(), rdoc::__copy)
        .def("simplex", &FaceEmbedding<dim, subdim>::simplex,
            pybind11::return_value_policy::reference, rbase::simplex)
        .def("face", &FaceEmbedding<dim, subdim>::face, rbase::face)
        .def("vertices", &FaceEmbedding<dim, subdim>::vertices, rbase::vertices)
    ;
    if constexpr (subdim == 0)
        e.def("vertex", &FaceEmbedding<dim, subdim>::vertex, rbase2::vertex);
    else if constexpr (subdim == 1)
        e.def("edge", &FaceEmbedding<dim, subdim>::edge, rbase2::edge);
    else if constexpr (subdim == 2)
        e.def("triangle", &FaceEmbedding<dim, subdim>::triangle,
            rbase2::triangle);
    else if constexpr (subdim == 3)
        e.def("tetrahedron", &FaceEmbedding<dim, subdim>::tetrahedron,
            rbase2::tetrahedron);
    else if constexpr (subdim == 4)
        e.def("pentachoron", &FaceEmbedding<dim, subdim>::pentachoron,
            rbase2::pentachoron);
    regina::python::add_output(e);
    regina::python::add_eq_operators(e, rbase::__eq, rbase::__ne);

    // We use the global scope here because all of Face's members are
    // inherited, and so Face's own docstring namespace does not exist.
    RDOC_SCOPE_SWITCH_MAIN
    RDOC_SCOPE_BASE_2(detail::FaceBase, detail::FaceNumberingAPI)

    auto c = pybind11::class_<regina::Face<dim, subdim>>(m, name, rdoc::Face)
        .def("isValid", &Face<dim, subdim>::isValid, rbase::isValid)
        // Only standard dimensions offer hasBadLink().
        .def("hasBadIdentification", &Face<dim, subdim>::hasBadIdentification,
            rbase::hasBadIdentification)
        .def("isLinkOrientable", &Face<dim, subdim>::isLinkOrientable,
            rbase::isLinkOrientable)
        .def("degree", &Face<dim, subdim>::degree, rbase::degree)
        .def("embedding", &Face<dim, subdim>::embedding, rbase::embedding)
        .def("embeddings", &Face<dim, subdim>::embeddings, rbase::embeddings)
        .def("__iter__", [](const Face<dim, subdim>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rbase::__iter__)
        .def("front", &Face<dim, subdim>::front, rbase::front)
        .def("back", &Face<dim, subdim>::back, rbase::back)
        .def("index", &Face<dim, subdim>::index, rbase::index)
        .def("triangulation", &Face<dim, subdim>::triangulation,
            rbase::triangulation)
        .def("component", &Face<dim, subdim>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("boundaryComponent", &Face<dim, subdim>::boundaryComponent,
            pybind11::return_value_policy::reference, rbase::boundaryComponent)
        .def("isBoundary", &Face<dim, subdim>::isBoundary, rbase::isBoundary)
        .def_static("ordering", &Face<dim, subdim>::ordering, rbase2::ordering)
        .def_static("faceNumber", &Face<dim, subdim>::faceNumber,
            rbase2::faceNumber)
        .def_static("containsVertex", &Face<dim, subdim>::containsVertex,
            rbase2::containsVertex)
        .def_readonly_static("nFaces", &Face<dim, subdim>::nFaces)
        .def_readonly_static("lexNumbering", &Face<dim, subdim>::lexNumbering)
        .def_readonly_static("oppositeDim", &Face<dim, subdim>::oppositeDim)
        .def_readonly_static("dimension", &Face<dim, subdim>::dimension)
        .def_readonly_static("subdimension", &Face<dim, subdim>::subdimension)
    ;
    if constexpr (subdim > 0) {
        c.def("face", &regina::python::face<Face<dim, subdim>, subdim, int>,
            pybind11::arg("lowerdim"), pybind11::arg("face"),
            rbase::face);
        c.def("faceMapping",
            &regina::python::faceMapping<Face<dim, subdim>, subdim, dim + 1>,
            pybind11::arg("lowerdim"), pybind11::arg("face"),
            rbase::faceMapping);
    }
    if constexpr (subdim > 4) {
        c.def("pentachoron", &Face<dim, subdim>::pentachoron,
            pybind11::return_value_policy::reference, rbase::pentachoron);
        c.def("pentachoronMapping", &Face<dim, subdim>::pentachoronMapping,
            rbase::pentachoronMapping);
    }
    if constexpr (subdim > 3) {
        c.def("tetrahedron", &Face<dim, subdim>::tetrahedron,
            pybind11::return_value_policy::reference, rbase::tetrahedron);
        c.def("tetrahedronMapping", &Face<dim, subdim>::tetrahedronMapping,
            rbase::tetrahedronMapping);
    }
    if constexpr (subdim > 2) {
        c.def("triangle", &Face<dim, subdim>::triangle,
            pybind11::return_value_policy::reference, rbase::triangle);
        c.def("triangleMapping", &Face<dim, subdim>::triangleMapping,
            rbase::triangleMapping);
    }
    if constexpr (subdim > 1) {
        c.def("edge", &Face<dim, subdim>::edge,
            pybind11::return_value_policy::reference, rbase::edge);
        c.def("edgeMapping", &Face<dim, subdim>::edgeMapping,
            rbase::edgeMapping);
    }
    if constexpr (subdim > 0) {
        c.def("vertex", &Face<dim, subdim>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex);
        c.def("vertexMapping", &Face<dim, subdim>::vertexMapping,
            rbase::vertexMapping);
    }
    if constexpr (dim - subdim == 1)
        c.def("inMaximalForest", &Face<dim, subdim>::inMaximalForest,
            rbase::inMaximalForest);
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addListView<
        decltype(std::declval<Face<dim, subdim>>().embeddings())>(m);
    regina::python::addListView<
        decltype(regina::Triangulation<dim>().template faces<subdim>())>(m);
}

