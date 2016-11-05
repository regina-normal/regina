
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "triangulation/generic.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using namespace regina::python;
using regina::Face;
using regina::FaceEmbedding;

namespace {
    template <int dim, int subdim>
    boost::python::list Face_embeddings_list(const Face<dim, subdim>* f) {
        boost::python::list ans;
        for (auto& emb: *f)
            ans.append(emb);
        return ans;
    }

    template <int dim, int subdim>
    struct embedding_aliases :
            boost::python::def_visitor<embedding_aliases<dim, subdim>> {
        template <typename Class>
        void visit(Class& c) const {
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 0> :
            boost::python::def_visitor<embedding_aliases<dim, 0>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("vertex", &FaceEmbedding<dim, 0>::vertex);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 1> :
            boost::python::def_visitor<embedding_aliases<dim, 1>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("edge", &FaceEmbedding<dim, 1>::edge);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 2> :
            boost::python::def_visitor<embedding_aliases<dim, 2>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("triangle", &FaceEmbedding<dim, 2>::triangle);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 3> :
            boost::python::def_visitor<embedding_aliases<dim, 3>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("tetrahedron", &FaceEmbedding<dim, 3>::tetrahedron);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 4> :
            boost::python::def_visitor<embedding_aliases<dim, 4>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("pentachoron", &FaceEmbedding<dim, 4>::pentachoron);
        }
    };

    template <int dim, int subdim, int codim>
    struct face_validity_types : boost::python::def_visitor<
            face_validity_types<dim, subdim, codim>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            // Only standard dimensions offer hasBadLink().
            c.def("hasBadIdentification",
                &Face<dim, subdim>::hasBadIdentification);
        }
    };

    template <int dim, int subdim>
    struct face_validity_types<dim, subdim, 1> :
            boost::python::def_visitor<face_validity_types<dim, subdim, 1>> {
        template <typename Class>
        void visit(Class&) const {
        }
    };

    template <int dim, int subdim, int maxlower>
    struct subface_aliases :
            boost::python::def_visitor<subface_aliases<dim, subdim, maxlower>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def(subface_aliases<dim, subdim, maxlower - 1>());
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, -1> :
            boost::python::def_visitor<subface_aliases<dim, subdim, -1>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, 0> :
            boost::python::def_visitor<subface_aliases<dim, subdim, 0>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("vertex", &Face<dim, subdim>::vertex,
                return_value_policy<reference_existing_object>());
            c.def("vertexMapping", &Face<dim, subdim>::vertexMapping);
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, 1> :
            boost::python::def_visitor<subface_aliases<dim, subdim, 1>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("edge", &Face<dim, subdim>::edge,
                return_value_policy<reference_existing_object>());
            c.def("edgeMapping", &Face<dim, subdim>::edgeMapping);
            c.def(subface_aliases<dim, subdim, 0>());
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, 2> :
            boost::python::def_visitor<subface_aliases<dim, subdim, 2>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("triangle", &Face<dim, subdim>::triangle,
                return_value_policy<reference_existing_object>());
            c.def("triangleMapping", &Face<dim, subdim>::triangleMapping);
            c.def(subface_aliases<dim, subdim, 1>());
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, 3> :
            boost::python::def_visitor<subface_aliases<dim, subdim, 3>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("tetrahedron", &Face<dim, subdim>::tetrahedron,
                return_value_policy<reference_existing_object>());
            c.def("tetrahedronMapping", &Face<dim, subdim>::tetrahedronMapping);
            c.def(subface_aliases<dim, subdim, 2>());
        }
    };

    template <int dim, int subdim>
    struct subface_aliases<dim, subdim, 4> :
            boost::python::def_visitor<subface_aliases<dim, subdim, 4>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("pentachoron", &Face<dim, subdim>::pentachoron,
                return_value_policy<reference_existing_object>());
            c.def("pentachoronMapping", &Face<dim, subdim>::pentachoronMapping);
            c.def(subface_aliases<dim, subdim, 3>());
        }
    };

    template <int dim, int subdim, int codim>
    struct face_in_maximal_forest : boost::python::def_visitor<
            face_in_maximal_forest<dim, subdim, codim>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class&) const {
        }
    };

    template <int dim, int subdim>
    struct face_in_maximal_forest<dim, subdim, 1> :
            boost::python::def_visitor<face_in_maximal_forest<dim, subdim, 1>> {
        template <typename Class>
        void visit(Class& c) const {
            c.def("inMaximalForest", &Face<dim, subdim>::inMaximalForest);
        }
    };
}

template <int dim, int subdim>
void addFace(const char* name, const char* embName) {
    class_<FaceEmbedding<dim, subdim>>(embName,
            init<regina::Simplex<dim>*, int>())
        .def(init<const FaceEmbedding<dim, subdim>&>())
        .def("simplex", &FaceEmbedding<dim, subdim>::simplex,
            return_value_policy<reference_existing_object>())
        .def("face", &FaceEmbedding<dim, subdim>::face)
        .def("vertices", &FaceEmbedding<dim, subdim>::vertices)
        .def(embedding_aliases<dim, subdim>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<regina::Face<dim, subdim>, std::auto_ptr<regina::Face<dim, subdim>>,
            boost::noncopyable>(name, no_init)
        .def("isValid", &Face<dim, subdim>::isValid)
        .def(face_validity_types<dim, subdim, dim - subdim>())
        .def("isLinkOrientable", &Face<dim, subdim>::isLinkOrientable)
        .def("degree", &Face<dim, subdim>::degree)
        .def("embeddings", Face_embeddings_list<dim, subdim>)
        .def("embedding", &Face<dim, subdim>::embedding,
            return_internal_reference<>())
        .def("front", &Face<dim, subdim>::front,
            return_internal_reference<>())
        .def("back", &Face<dim, subdim>::back,
            return_internal_reference<>())
        .def(face_in_maximal_forest<dim, subdim, dim - subdim>())
        .def("index", &Face<dim, subdim>::index)
        .def("triangulation", &Face<dim, subdim>::triangulation,
            return_value_policy<regina::python::to_held_type<>>())
        .def("component", &Face<dim, subdim>::component,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Face<dim, subdim>::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isBoundary", &Face<dim, subdim>::isBoundary)
        .def("face", &regina::python::face<Face<dim, subdim>, subdim, int>)
        .def("faceMapping",
            &regina::python::faceMapping<Face<dim, subdim>, subdim, dim + 1>)
        .def(subface_aliases<dim, subdim, subdim - 1>())
        .def("ordering", &Face<dim, subdim>::ordering)
        .def("faceNumber", &Face<dim, subdim>::faceNumber)
        .def("containsVertex", &Face<dim, subdim>::containsVertex)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("ordering")
        .staticmethod("faceNumber")
        .staticmethod("containsVertex")
    ;
}

