
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
#include "triangulation/triangulation.h"
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/facenumbering.h"
#include "../../docstrings/triangulation/face.h"

using namespace pybind11::literals;

using regina::Face;
using regina::FaceEmbedding;

template <int dim, int subdim>
requires (regina::supportedDim(dim) && subdim >= 0 && subdim < dim)
void addFace(pybind11::module_& m, pybind11::module_& internal,
        const char* name, const char* embName) {
    RDOC_SCOPE_BEGIN(FaceEmbedding)

    auto e = pybind11::class_<FaceEmbedding<dim, subdim>>(m, embName,
            rdoc::__class)
        .def(pybind11::init<regina::Simplex<dim>*, regina::Perm<dim + 1>>(),
            rdoc::__init)
        .def(pybind11::init<const FaceEmbedding<dim, subdim>&>(), rdoc::__copy)
        .def("simplex", &FaceEmbedding<dim, subdim>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex)
        .def("face", &FaceEmbedding<dim, subdim>::face, rdoc::face)
        .def("vertices", &FaceEmbedding<dim, subdim>::vertices, rdoc::vertices)
    ;
    // For the dimension-specific aliases below, clang can resolve the
    // overloads but gcc cannot.  We could fix this with a static_cast, but
    // we will just bind to face() instead (which gcc _can_ resolve).
    if constexpr (subdim == 0)
        e.def("vertex", &FaceEmbedding<dim, subdim>::face, rdoc::vertex);
    else if constexpr (subdim == 1)
        e.def("edge", &FaceEmbedding<dim, subdim>::face, rdoc::edge);
    else if constexpr (subdim == 2)
        e.def("triangle", &FaceEmbedding<dim, subdim>::face,
            rdoc::triangle);
    else if constexpr (subdim == 3)
        e.def("tetrahedron", &FaceEmbedding<dim, subdim>::face,
            rdoc::tetrahedron);
    else if constexpr (subdim == 4)
        e.def("pentachoron", &FaceEmbedding<dim, subdim>::face,
            rdoc::pentachoron);
    regina::python::add_output_rich(e);
    regina::python::add_eq_operators(e, rdoc::__eq);

    RDOC_SCOPE_SWITCH(Face)
    RDOC_SCOPE_BASE(FaceNumbering)

    auto c = pybind11::class_<regina::Face<dim, subdim>>(m, name,
            rdoc::Face::__class)
        .def("isValid", &Face<dim, subdim>::isValid, rdoc::isValid)
        // Only standard dimensions offer hasBadLink().
        .def("hasBadIdentification", &Face<dim, subdim>::hasBadIdentification,
            rdoc::hasBadIdentification)
        .def("isLinkOrientable", &Face<dim, subdim>::isLinkOrientable,
            rdoc::isLinkOrientable)
        .def("degree", &Face<dim, subdim>::degree, rdoc::degree)
        .def("embedding", &Face<dim, subdim>::embedding, rdoc::embedding)
        .def("embeddings", &Face<dim, subdim>::embeddings, rdoc::embeddings)
        .def("__iter__", [](const Face<dim, subdim>& f) {
            // By default, make_iterator uses reference_internal.
            return pybind11::make_iterator<pybind11::return_value_policy::copy>(
                f.begin(), f.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Face alive
            rdoc::__iter__)
        .def("front", &Face<dim, subdim>::front, rdoc::front)
        .def("back", &Face<dim, subdim>::back, rdoc::back)
        .def("index", &Face<dim, subdim>::index, rdoc::index)
        .def("triangulation", &Face<dim, subdim>::triangulation,
            rdoc::triangulation)
        .def("component", &Face<dim, subdim>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("boundaryComponent", &Face<dim, subdim>::boundaryComponent,
            pybind11::return_value_policy::reference, rdoc::boundaryComponent)
        .def("isBoundary", &Face<dim, subdim>::isBoundary, rdoc::isBoundary)
        .def_static("ordering", &Face<dim, subdim>::ordering, rbase::ordering)
        .def_static("faceNumber",
            pybind11::overload_cast<regina::Perm<dim+1>>(
                &Face<dim, subdim>::faceNumber),
            rbase::faceNumber)
        .def_static("containsVertex", &Face<dim, subdim>::containsVertex,
            rbase::containsVertex)
        .def_readonly_static("nFaces", &Face<dim, subdim>::nFaces)
        .def_readonly_static("lexNumbering", &Face<dim, subdim>::lexNumbering)
        .def_readonly_static("oppositeDim", &Face<dim, subdim>::oppositeDim)
        .def_readonly_static("dimension", &Face<dim, subdim>::dimension)
        .def_readonly_static("subdimension", &Face<dim, subdim>::subdimension)
        .def_readonly_static("hasNumberingTables",
            &Face<dim, subdim>::hasNumberingTables)
    ;
    if constexpr (subdim < dim - 1) {
        c.def("isLinkClosed", &Face<dim, subdim>::isLinkClosed,
            rdoc::isLinkClosed);
    }
    if constexpr (subdim == 1) {
        c.def_static("faceNumber",
            pybind11::overload_cast<int, int>(&Face<dim, subdim>::faceNumber),
            rbase::faceNumber_2);
    }
    if constexpr (subdim > 0) {
        c.def("face", &regina::python::face<Face<dim, subdim>>,
            "lowerdim"_a, "face"_a, rdoc::face);
        c.def("faceMapping",
            &regina::python::faceMapping<Face<dim, subdim>>,
            "lowerdim"_a, "face"_a, rdoc::faceMapping);
    }
    if constexpr (subdim > 4) {
        c.def("pentachoron", &Face<dim, subdim>::pentachoron,
            pybind11::return_value_policy::reference, rdoc::pentachoron);
        c.def("pentachoronMapping", &Face<dim, subdim>::pentachoronMapping,
            rdoc::pentachoronMapping);
    }
    if constexpr (subdim > 3) {
        c.def("tetrahedron", &Face<dim, subdim>::tetrahedron,
            pybind11::return_value_policy::reference, rdoc::tetrahedron);
        c.def("tetrahedronMapping", &Face<dim, subdim>::tetrahedronMapping,
            rdoc::tetrahedronMapping);
    }
    if constexpr (subdim > 2) {
        c.def("triangle", &Face<dim, subdim>::triangle,
            pybind11::return_value_policy::reference, rdoc::triangle);
        c.def("triangleMapping", &Face<dim, subdim>::triangleMapping,
            rdoc::triangleMapping);
    }
    if constexpr (subdim > 1) {
        c.def("edge", &Face<dim, subdim>::edge,
            pybind11::return_value_policy::reference, rdoc::edge);
        c.def("edgeMapping", &Face<dim, subdim>::edgeMapping,
            rdoc::edgeMapping);
    }
    if constexpr (subdim > 0) {
        c.def("vertex", &Face<dim, subdim>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex);
        c.def("vertexMapping", &Face<dim, subdim>::vertexMapping,
            rdoc::vertexMapping);
    }
    if constexpr (subdim == 1) {
        c.def("isLoop", &Face<dim, subdim>::isLoop, rdoc::isLoop);
    }
    if constexpr (subdim == 2) {
        c.def("triangleType", &Face<dim, subdim>::triangleType,
            rdoc::triangleType);
        c.def("triangleSubtype", &Face<dim, subdim>::triangleSubtype,
            rdoc::triangleSubtype);
        c.def("formsMobiusBand", &Face<dim, subdim>::formsMobiusBand,
            rdoc::formsMobiusBand);
        c.def("formsCone", &Face<dim, subdim>::formsCone, rdoc::formsCone);
    }
    if constexpr (dim - subdim == 1) {
        c.def("join", &Face<dim, subdim>::join, rdoc::join);
        c.def("lock", &Face<dim, subdim>::lock, rdoc::lock);
        c.def("unlock", &Face<dim, subdim>::unlock, rdoc::unlock);
        c.def("isLocked", &Face<dim, subdim>::isLocked, rdoc::isLocked);
        c.def("inMaximalForest", &Face<dim, subdim>::inMaximalForest,
            rdoc::inMaximalForest);
    }
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addStdView<
        decltype(std::declval<Face<dim, subdim>>().embeddings())>(internal,
        (std::string(name) + "_embeddings").c_str());
    // The name we give to the next view class is not in the typical form
    // Triangulation<dim>_faces<subdim>; however, this is an internal class,
    // and the name we do use is easy to build from what we already know.
    regina::python::addStdView<
        decltype(regina::Triangulation<dim>().template faces<subdim>())>(
        internal, (std::string(name) + "_faces").c_str());
}

