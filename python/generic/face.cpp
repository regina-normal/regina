
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include <boost/python.hpp>
#include "generic/component.h"
#include "generic/face.h"
#include "generic/simplex.h"
#include "generic/triangulation.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using regina::Face;
using regina::FaceEmbedding;

namespace {
    template <int dim, int subdim>
    boost::python::list Face_getEmbeddings_list(const Face<dim, subdim>* f) {
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
            c.def("getVertex", &FaceEmbedding<dim, 0>::getVertex);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 1> :
            boost::python::def_visitor<embedding_aliases<dim, 1>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("edge", &FaceEmbedding<dim, 1>::edge);
            c.def("getEdge", &FaceEmbedding<dim, 1>::getEdge);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 2> :
            boost::python::def_visitor<embedding_aliases<dim, 2>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("triangle", &FaceEmbedding<dim, 2>::triangle);
            c.def("getTriangle", &FaceEmbedding<dim, 2>::getTriangle);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 3> :
            boost::python::def_visitor<embedding_aliases<dim, 3>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("tetrahedron", &FaceEmbedding<dim, 3>::tetrahedron);
            c.def("getTetrahedron", &FaceEmbedding<dim, 3>::getTetrahedron);
        }
    };

    template <int dim>
    struct embedding_aliases<dim, 4> :
            boost::python::def_visitor<embedding_aliases<dim, 4>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("pentachoron", &FaceEmbedding<dim, 4>::pentachoron);
            c.def("getPentachoron", &FaceEmbedding<dim, 4>::getPentachoron);
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
            c.def("getVertex", &Face<dim, subdim>::getVertex,
                return_value_policy<reference_existing_object>());
            c.def("vertexMapping", &Face<dim, subdim>::vertexMapping);
            c.def("getVertexMapping", &Face<dim, subdim>::getVertexMapping);
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
            c.def("getEdge", &Face<dim, subdim>::getEdge,
                return_value_policy<reference_existing_object>());
            c.def("edgeMapping", &Face<dim, subdim>::edgeMapping);
            c.def("getEdgeMapping", &Face<dim, subdim>::getEdgeMapping);
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
            c.def("getTriangle", &Face<dim, subdim>::getTriangle,
                return_value_policy<reference_existing_object>());
            c.def("triangleMapping", &Face<dim, subdim>::triangleMapping);
            c.def("getTriangleMapping", &Face<dim, subdim>::getTriangleMapping);
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
            c.def("getTetrahedron", &Face<dim, subdim>::getTetrahedron,
                return_value_policy<reference_existing_object>());
            c.def("tetrahedronMapping", &Face<dim, subdim>::tetrahedronMapping);
            c.def("getTetrahedronMapping",
                &Face<dim, subdim>::getTetrahedronMapping);
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
            c.def("getPentachoron", &Face<dim, subdim>::getPentachoron,
                return_value_policy<reference_existing_object>());
            c.def("pentachoronMapping", &Face<dim, subdim>::pentachoronMapping);
            c.def("getPentachoronMapping",
                &Face<dim, subdim>::getPentachoronMapping);
            c.def(subface_aliases<dim, subdim, 3>());
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
        .def("getSimplex", &FaceEmbedding<dim, subdim>::getSimplex,
            return_value_policy<reference_existing_object>())
        .def("face", &FaceEmbedding<dim, subdim>::face)
        .def("getFace", &FaceEmbedding<dim, subdim>::getFace)
        .def("vertices", &FaceEmbedding<dim, subdim>::vertices)
        .def("getVertices", &FaceEmbedding<dim, subdim>::getVertices)
        .def("str", &FaceEmbedding<dim, subdim>::str)
        .def("toString", &FaceEmbedding<dim, subdim>::toString)
        .def("detail", &FaceEmbedding<dim, subdim>::detail)
        .def("toStringLong", &FaceEmbedding<dim, subdim>::toStringLong)
        .def("__str__", &FaceEmbedding<dim, subdim>::str)
        .def(embedding_aliases<dim, subdim>())
        .def(regina::python::add_eq_operators())
    ;

    class_<regina::Face<dim, subdim>, std::auto_ptr<regina::Face<dim, subdim>>,
            boost::noncopyable>(name, no_init)
        .def("isValid", &Face<dim, subdim>::isValid)
        .def(face_validity_types<dim, subdim, dim - subdim>())
        .def("isLinkOrientable", &Face<dim, subdim>::isLinkOrientable)
        .def("degree", &Face<dim, subdim>::degree)
        .def("getDegree", &Face<dim, subdim>::getDegree)
        .def("embeddings", Face_getEmbeddings_list<dim, subdim>)
        .def("getEmbeddings", Face_getEmbeddings_list<dim, subdim>)
        .def("embedding", &Face<dim, subdim>::embedding,
            return_internal_reference<>())
        .def("getEmbedding", &Face<dim, subdim>::getEmbedding,
            return_internal_reference<>())
        .def("front", &Face<dim, subdim>::front,
            return_internal_reference<>())
        .def("back", &Face<dim, subdim>::back,
            return_internal_reference<>())
        .def("index", &Face<dim, subdim>::index)
        .def("triangulation", &Face<dim, subdim>::triangulation,
            return_value_policy<reference_existing_object>())
        .def("getTriangulation", &Face<dim, subdim>::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("component", &Face<dim, subdim>::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &Face<dim, subdim>::getComponent,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<Face<dim, subdim>, subdim, int>)
        .def("getFace", &regina::python::face<Face<dim, subdim>, subdim, int>)
        .def("faceMapping",
            &regina::python::faceMapping<Face<dim, subdim>, subdim, dim + 1>)
        .def("getFaceMapping",
            &regina::python::faceMapping<Face<dim, subdim>, subdim, dim + 1>)
        .def(subface_aliases<dim, subdim, subdim - 1>())
        .def("str", &Face<dim, subdim>::str)
        .def("toString", &Face<dim, subdim>::toString)
        .def("detail", &Face<dim, subdim>::detail)
        .def("toStringLong", &Face<dim, subdim>::toStringLong)
        .def("__str__", &Face<dim, subdim>::str)
        .def("ordering", &Face<dim, subdim>::ordering)
        .def("faceNumber", &Face<dim, subdim>::faceNumber)
        .def("containsVertex", &Face<dim, subdim>::containsVertex)
        .def(regina::python::add_eq_operators())
        .staticmethod("ordering")
        .staticmethod("faceNumber")
        .staticmethod("containsVertex")
    ;
}

void addFace() {
    addFace<5, 0>("Face5_0", "FaceEmbedding5_0");
    addFace<5, 1>("Face5_1", "FaceEmbedding5_1");
    addFace<5, 2>("Face5_2", "FaceEmbedding5_2");
    addFace<5, 3>("Face5_3", "FaceEmbedding5_3");
    addFace<5, 4>("Face5_4", "FaceEmbedding5_4");

    addFace<6, 0>("Face6_0", "FaceEmbedding6_0");
    addFace<6, 1>("Face6_1", "FaceEmbedding6_1");
    addFace<6, 2>("Face6_2", "FaceEmbedding6_2");
    addFace<6, 3>("Face6_3", "FaceEmbedding6_3");
    addFace<6, 4>("Face6_4", "FaceEmbedding6_4");
    addFace<6, 5>("Face6_5", "FaceEmbedding6_5");

    addFace<7, 0>("Face7_0", "FaceEmbedding7_0");
    addFace<7, 1>("Face7_1", "FaceEmbedding7_1");
    addFace<7, 2>("Face7_2", "FaceEmbedding7_2");
    addFace<7, 3>("Face7_3", "FaceEmbedding7_3");
    addFace<7, 4>("Face7_4", "FaceEmbedding7_4");
    addFace<7, 5>("Face7_5", "FaceEmbedding7_5");
    addFace<7, 6>("Face7_6", "FaceEmbedding7_6");

    addFace<8, 0>("Face8_0", "FaceEmbedding8_0");
    addFace<8, 1>("Face8_1", "FaceEmbedding8_1");
    addFace<8, 2>("Face8_2", "FaceEmbedding8_2");
    addFace<8, 3>("Face8_3", "FaceEmbedding8_3");
    addFace<8, 4>("Face8_4", "FaceEmbedding8_4");
    addFace<8, 5>("Face8_5", "FaceEmbedding8_5");
    addFace<8, 6>("Face8_6", "FaceEmbedding8_6");
    addFace<8, 7>("Face8_7", "FaceEmbedding8_7");

    addFace<9, 0>("Face9_0", "FaceEmbedding9_0");
    addFace<9, 1>("Face9_1", "FaceEmbedding9_1");
    addFace<9, 2>("Face9_2", "FaceEmbedding9_2");
    addFace<9, 3>("Face9_3", "FaceEmbedding9_3");
    addFace<9, 4>("Face9_4", "FaceEmbedding9_4");
    addFace<9, 5>("Face9_5", "FaceEmbedding9_5");
    addFace<9, 6>("Face9_6", "FaceEmbedding9_6");
    addFace<9, 7>("Face9_7", "FaceEmbedding9_7");
    addFace<9, 8>("Face9_8", "FaceEmbedding9_8");

    addFace<10, 0>("Face10_0", "FaceEmbedding10_0");
    addFace<10, 1>("Face10_1", "FaceEmbedding10_1");
    addFace<10, 2>("Face10_2", "FaceEmbedding10_2");
    addFace<10, 3>("Face10_3", "FaceEmbedding10_3");
    addFace<10, 4>("Face10_4", "FaceEmbedding10_4");
    addFace<10, 5>("Face10_5", "FaceEmbedding10_5");
    addFace<10, 6>("Face10_6", "FaceEmbedding10_6");
    addFace<10, 7>("Face10_7", "FaceEmbedding10_7");
    addFace<10, 8>("Face10_8", "FaceEmbedding10_8");
    addFace<10, 9>("Face10_9", "FaceEmbedding10_9");

    addFace<11, 0>("Face11_0", "FaceEmbedding11_0");
    addFace<11, 1>("Face11_1", "FaceEmbedding11_1");
    addFace<11, 2>("Face11_2", "FaceEmbedding11_2");
    addFace<11, 3>("Face11_3", "FaceEmbedding11_3");
    addFace<11, 4>("Face11_4", "FaceEmbedding11_4");
    addFace<11, 5>("Face11_5", "FaceEmbedding11_5");
    addFace<11, 6>("Face11_6", "FaceEmbedding11_6");
    addFace<11, 7>("Face11_7", "FaceEmbedding11_7");
    addFace<11, 8>("Face11_8", "FaceEmbedding11_8");
    addFace<11, 9>("Face11_9", "FaceEmbedding11_9");
    addFace<11, 10>("Face11_10", "FaceEmbedding11_10");

    addFace<12, 0>("Face12_0", "FaceEmbedding12_0");
    addFace<12, 1>("Face12_1", "FaceEmbedding12_1");
    addFace<12, 2>("Face12_2", "FaceEmbedding12_2");
    addFace<12, 3>("Face12_3", "FaceEmbedding12_3");
    addFace<12, 4>("Face12_4", "FaceEmbedding12_4");
    addFace<12, 5>("Face12_5", "FaceEmbedding12_5");
    addFace<12, 6>("Face12_6", "FaceEmbedding12_6");
    addFace<12, 7>("Face12_7", "FaceEmbedding12_7");
    addFace<12, 8>("Face12_8", "FaceEmbedding12_8");
    addFace<12, 9>("Face12_9", "FaceEmbedding12_9");
    addFace<12, 10>("Face12_10", "FaceEmbedding12_10");
    addFace<12, 11>("Face12_11", "FaceEmbedding12_11");

    addFace<13, 0>("Face13_0", "FaceEmbedding13_0");
    addFace<13, 1>("Face13_1", "FaceEmbedding13_1");
    addFace<13, 2>("Face13_2", "FaceEmbedding13_2");
    addFace<13, 3>("Face13_3", "FaceEmbedding13_3");
    addFace<13, 4>("Face13_4", "FaceEmbedding13_4");
    addFace<13, 5>("Face13_5", "FaceEmbedding13_5");
    addFace<13, 6>("Face13_6", "FaceEmbedding13_6");
    addFace<13, 7>("Face13_7", "FaceEmbedding13_7");
    addFace<13, 8>("Face13_8", "FaceEmbedding13_8");
    addFace<13, 9>("Face13_9", "FaceEmbedding13_9");
    addFace<13, 10>("Face13_10", "FaceEmbedding13_10");
    addFace<13, 11>("Face13_11", "FaceEmbedding13_11");
    addFace<13, 12>("Face13_12", "FaceEmbedding13_12");

    addFace<14, 0>("Face14_0", "FaceEmbedding14_0");
    addFace<14, 1>("Face14_1", "FaceEmbedding14_1");
    addFace<14, 2>("Face14_2", "FaceEmbedding14_2");
    addFace<14, 3>("Face14_3", "FaceEmbedding14_3");
    addFace<14, 4>("Face14_4", "FaceEmbedding14_4");
    addFace<14, 5>("Face14_5", "FaceEmbedding14_5");
    addFace<14, 6>("Face14_6", "FaceEmbedding14_6");
    addFace<14, 7>("Face14_7", "FaceEmbedding14_7");
    addFace<14, 8>("Face14_8", "FaceEmbedding14_8");
    addFace<14, 9>("Face14_9", "FaceEmbedding14_9");
    addFace<14, 10>("Face14_10", "FaceEmbedding14_10");
    addFace<14, 11>("Face14_11", "FaceEmbedding14_11");
    addFace<14, 12>("Face14_12", "FaceEmbedding14_12");
    addFace<14, 13>("Face14_13", "FaceEmbedding14_13");

    addFace<15, 0>("Face15_0", "FaceEmbedding15_0");
    addFace<15, 1>("Face15_1", "FaceEmbedding15_1");
    addFace<15, 2>("Face15_2", "FaceEmbedding15_2");
    addFace<15, 3>("Face15_3", "FaceEmbedding15_3");
    addFace<15, 4>("Face15_4", "FaceEmbedding15_4");
    addFace<15, 5>("Face15_5", "FaceEmbedding15_5");
    addFace<15, 6>("Face15_6", "FaceEmbedding15_6");
    addFace<15, 7>("Face15_7", "FaceEmbedding15_7");
    addFace<15, 8>("Face15_8", "FaceEmbedding15_8");
    addFace<15, 9>("Face15_9", "FaceEmbedding15_9");
    addFace<15, 10>("Face15_10", "FaceEmbedding15_10");
    addFace<15, 11>("Face15_11", "FaceEmbedding15_11");
    addFace<15, 12>("Face15_12", "FaceEmbedding15_12");
    addFace<15, 13>("Face15_13", "FaceEmbedding15_13");
    addFace<15, 14>("Face15_14", "FaceEmbedding15_14");
}

