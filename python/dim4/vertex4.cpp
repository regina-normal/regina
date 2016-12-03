
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
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::Vertex;
using regina::VertexEmbedding;
using regina::Face;
using regina::FaceEmbedding;

namespace {
    boost::python::list Vertex4_embeddings_list(const Vertex<4>* v) {
        boost::python::list ans;
        for (auto& emb : *v)
            ans.append(emb);
        return ans;
    }

    regina::Triangulation<3>* vertex_buildLink(const Vertex<4>* v) {
        return new regina::Triangulation<3>(*(v->buildLink()));
    }

    boost::python::tuple vertex_buildLinkDetail_bool(const Vertex<4>* v,
            bool labels = true) {
        regina::Isomorphism<4>* iso;
        regina::Triangulation<3>* link = new regina::Triangulation<3>(
            *(v->buildLinkDetail(labels, &iso)));
        return boost::python::make_tuple(
            boost::python::object(boost::python::handle<>(
                regina::python::to_held_type<>::
                apply<regina::Triangulation<3>*>::type()(link))),
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Isomorphism<4>*>::type()(iso))));
    }

    boost::python::tuple vertex_buildLinkDetail_void(const Vertex<4>* v) {
        return vertex_buildLinkDetail_bool(v);
    }
}

void addVertex4() {
    class_<FaceEmbedding<4, 0>>("FaceEmbedding4_0",
            init<regina::Pentachoron<4>*, int>())
        .def(init<const VertexEmbedding<4>&>())
        .def("simplex", &VertexEmbedding<4>::simplex,
            return_value_policy<reference_existing_object>())
        .def("pentachoron", &VertexEmbedding<4>::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("face", &VertexEmbedding<4>::face)
        .def("vertex", &VertexEmbedding<4>::vertex)
        .def("vertices", &VertexEmbedding<4>::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<Face<4, 0>, std::auto_ptr<Face<4, 0>>, boost::noncopyable>
            ("Face4_0", no_init)
        .def("index", &Vertex<4>::index)
        .def("embeddings", Vertex4_embeddings_list)
        .def("embedding", &Vertex<4>::embedding,
            return_internal_reference<>())
        .def("front", &Vertex<4>::front,
            return_internal_reference<>())
        .def("back", &Vertex<4>::back,
            return_internal_reference<>())
        .def("triangulation", &Vertex<4>::triangulation,
            return_value_policy<to_held_type<>>())
        .def("component", &Vertex<4>::component,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Vertex<4>::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("degree", &Vertex<4>::degree)
        .def("buildLink", &vertex_buildLink,
            return_value_policy<to_held_type<>>())
        .def("buildLinkDetail", vertex_buildLinkDetail_void)
        .def("buildLinkDetail", vertex_buildLinkDetail_bool)
        .def("isLinkOrientable", &Vertex<4>::isLinkOrientable)
        .def("isValid", &Vertex<4>::isValid)
        .def("hasBadIdentification", &Vertex<4>::hasBadIdentification)
        .def("hasBadLink", &Vertex<4>::hasBadLink)
        .def("isIdeal", &Vertex<4>::isIdeal)
        .def("isBoundary", &Vertex<4>::isBoundary)
        .def("ordering", &Vertex<4>::ordering)
        .def("faceNumber", &Vertex<4>::faceNumber)
        .def("containsVertex", &Vertex<4>::containsVertex)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("ordering")
        .staticmethod("faceNumber")
        .staticmethod("containsVertex")
    ;

    scope().attr("Dim4VertexEmbedding") = scope().attr("FaceEmbedding4_0");
    scope().attr("VertexEmbedding4") = scope().attr("FaceEmbedding4_0");
    scope().attr("Dim4Vertex") = scope().attr("Face4_0");
    scope().attr("Vertex4") = scope().attr("Face4_0");
}

