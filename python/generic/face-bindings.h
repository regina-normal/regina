
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

using regina::Face;
using regina::FaceEmbedding;

namespace {
    template <int dim, int subdim>
    struct embedding_aliases {
        template <typename Class>
        static void add(Class& c) {
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 0> {
        template <typename Class>
        static void add(Class& c) {
            c.def("vertex", &FaceEmbedding<dim, 0>::vertex);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 1> {
        template <typename Class>
        static void add(Class& c) {
            c.def("edge", &FaceEmbedding<dim, 1>::edge);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 2> {
        template <typename Class>
        static void add(Class& c) {
            c.def("triangle", &FaceEmbedding<dim, 2>::triangle);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 3> {
        template <typename Class>
        static void add(Class& c) {
            c.def("tetrahedron", &FaceEmbedding<dim, 3>::tetrahedron);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 4> {
        template <typename Class>
        static void add(Class& c) {
            c.def("pentachoron", &FaceEmbedding<dim, 4>::pentachoron);
        }
    };

    template <int dim, int subdim, int codim>
    struct face_validity_types {
        template <typename Class>
        static void add(Class& c) {
            // Only standard dimensions offer hasBadLink().
            c.def("hasBadIdentification",
                &Face<dim, subdim>::hasBadIdentification);
        }
    };

    template <int dim, int subdim>
    struct face_validity_types<dim, subdim, 1> {
        template <typename Class>
        static void add(Class&) {
        }
    };

    template <int dim, int subdim, int maxlower>
    struct subface_aliases {
        template <typename Class>
        static void add(Class& c) {
            subface_aliases<dim, subdim, maxlower - 1>::add(c);
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, -1> {
        template <typename Class>
        static void add(Class& c) {
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, 0> {
        template <typename Class>
        static void add(Class& c) {
            c.def("vertex", &Face<dim, subdim>::vertex,
                pybind11::return_value_policy::reference);
            c.def("vertexMapping", &Face<dim, subdim>::vertexMapping);
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, 1> {
        template <typename Class>
        static void add(Class& c) {
            c.def("edge", &Face<dim, subdim>::edge,
                pybind11::return_value_policy::reference);
            c.def("edgeMapping", &Face<dim, subdim>::edgeMapping);
            subface_aliases<dim, subdim, 0>::add(c);
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, 2> {
        template <typename Class>
        static void add(Class& c) {
            c.def("triangle", &Face<dim, subdim>::triangle,
                pybind11::return_value_policy::reference);
            c.def("triangleMapping", &Face<dim, subdim>::triangleMapping);
            subface_aliases<dim, subdim, 1>::add(c);
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, 3> {
        template <typename Class>
        static void add(Class& c) {
            c.def("tetrahedron", &Face<dim, subdim>::tetrahedron,
                pybind11::return_value_policy::reference);
            c.def("tetrahedronMapping", &Face<dim, subdim>::tetrahedronMapping);
            subface_aliases<dim, subdim, 2>::add(c);
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, 4> {
        template <typename Class>
        static void add(Class& c) {
            c.def("pentachoron", &Face<dim, subdim>::pentachoron,
                pybind11::return_value_policy::reference);
            c.def("pentachoronMapping", &Face<dim, subdim>::pentachoronMapping);
            subface_aliases<dim, subdim, 3>::add(c);
        }
    };

    template <int dim, int subdim, int codim>
    struct face_in_maximal_forest {
        template <typename Class>
        static void add(Class&) {
        }
    };

    template <int dim, int subdim>
    struct face_in_maximal_forest<dim, subdim, 1> {
        template <typename Class>
        static void add(Class& c) {
            c.def("inMaximalForest", &Face<dim, subdim>::inMaximalForest);
        }
    };
}

template <int dim, int subdim>
void addFace(pybind11::module_& m, const char* name, const char* embName) {
    auto e = pybind11::class_<FaceEmbedding<dim, subdim>>(m, embName)
        .def(pybind11::init<regina::Simplex<dim>*, int>())
        .def(pybind11::init<const FaceEmbedding<dim, subdim>&>())
        .def("simplex", &FaceEmbedding<dim, subdim>::simplex,
            pybind11::return_value_policy::reference)
        .def("face", &FaceEmbedding<dim, subdim>::face)
        .def("vertices", &FaceEmbedding<dim, subdim>::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e);
    embedding_aliases<dim, subdim>::add(e);

    auto c = pybind11::class_<regina::Face<dim, subdim>>(m, name)
        .def("isValid", &Face<dim, subdim>::isValid)
        .def("isLinkOrientable", &Face<dim, subdim>::isLinkOrientable)
        .def("degree", &Face<dim, subdim>::degree)
        .def("embeddings", [](const Face<dim, subdim>& f) {
            pybind11::list ans;
            for (const auto& emb : f)
                ans.append(emb);
            return ans;
        })
        .def("embedding", &Face<dim, subdim>::embedding,
            pybind11::return_value_policy::reference_internal)
        .def("front", &Face<dim, subdim>::front,
            pybind11::return_value_policy::reference_internal)
        .def("back", &Face<dim, subdim>::back,
            pybind11::return_value_policy::reference_internal)
        .def("index", &Face<dim, subdim>::index)
        .def("triangulation", &Face<dim, subdim>::triangulation)
        .def("component", &Face<dim, subdim>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Face<dim, subdim>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("isBoundary", &Face<dim, subdim>::isBoundary)
        .def("face", &regina::python::face<Face<dim, subdim>, subdim, int,
            pybind11::return_value_policy::reference>)
        .def("faceMapping",
            &regina::python::faceMapping<Face<dim, subdim>, subdim, dim + 1>)
        .def_static("ordering", &Face<dim, subdim>::ordering)
        .def_static("faceNumber", &Face<dim, subdim>::faceNumber)
        .def_static("containsVertex", &Face<dim, subdim>::containsVertex)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);
    face_validity_types<dim, subdim, dim - subdim>::add(c);
    face_in_maximal_forest<dim, subdim, dim - subdim>::add(c);
    subface_aliases<dim, subdim, subdim - 1>::add(c);
}

