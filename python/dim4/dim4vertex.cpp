
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
#include "dim4/dim4boundarycomponent.h"
#include "dim4/dim4component.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4triangulation.h"
#include "dim4/dim4vertex.h"
#include "triangulation/ntriangulation.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::Dim4Vertex;
using regina::Dim4VertexEmbedding;
using regina::Face;
using regina::FaceEmbedding;

namespace {
    boost::python::list Dim4Vertex_embeddings_list(const Dim4Vertex* v) {
        boost::python::list ans;
        for (auto& emb : *v)
            ans.append(emb);
        return ans;
    }

    regina::NTriangulation* vertex_buildLink(const Dim4Vertex* v) {
        return new regina::NTriangulation(*(v->buildLink()));
    }

    boost::python::tuple vertex_buildLinkDetail_bool(const Dim4Vertex* v,
            bool labels = true) {
        regina::Dim4Isomorphism* iso;
        regina::NTriangulation* link = new regina::NTriangulation(
            *(v->buildLinkDetail(labels, &iso)));
        return boost::python::make_tuple(
            boost::python::object(boost::python::handle<>(
                regina::python::to_held_type<>::
                apply<regina::NTriangulation*>::type()(link))),
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Dim4Isomorphism*>::type()(iso))));
    }

    boost::python::tuple vertex_buildLinkDetail_void(const Dim4Vertex* v) {
        return vertex_buildLinkDetail_bool(v);
    }
}

void addDim4Vertex() {
    class_<FaceEmbedding<4, 0>>("FaceEmbedding4_0",
            init<regina::Dim4Pentachoron*, int>())
        .def(init<const Dim4VertexEmbedding&>())
        .def("simplex", &Dim4VertexEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &Dim4VertexEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("pentachoron", &Dim4VertexEmbedding::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("getPentachoron", &Dim4VertexEmbedding::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("face", &Dim4VertexEmbedding::face)
        .def("getFace", &Dim4VertexEmbedding::face)
        .def("vertex", &Dim4VertexEmbedding::vertex)
        .def("getVertex", &Dim4VertexEmbedding::vertex)
        .def("vertices", &Dim4VertexEmbedding::vertices)
        .def("getVertices", &Dim4VertexEmbedding::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<Face<4, 0>, std::auto_ptr<Face<4, 0>>, boost::noncopyable>
            ("Face4_0", no_init)
        .def("index", &Dim4Vertex::index)
        .def("embeddings", Dim4Vertex_embeddings_list)
        .def("getEmbeddings", Dim4Vertex_embeddings_list)
        .def("embedding", &Dim4Vertex::embedding,
            return_internal_reference<>())
        .def("getEmbedding", &Dim4Vertex::embedding,
            return_internal_reference<>())
        .def("front", &Dim4Vertex::front,
            return_internal_reference<>())
        .def("back", &Dim4Vertex::back,
            return_internal_reference<>())
        .def("triangulation", &Dim4Vertex::triangulation,
            return_value_policy<to_held_type<>>())
        .def("getTriangulation", &Dim4Vertex::triangulation,
            return_value_policy<to_held_type<>>())
        .def("component", &Dim4Vertex::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &Dim4Vertex::component,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Dim4Vertex::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Vertex::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("degree", &Dim4Vertex::degree)
        .def("getDegree", &Dim4Vertex::degree)
        .def("buildLink", &vertex_buildLink,
            return_value_policy<to_held_type<>>())
        .def("buildLinkDetail", vertex_buildLinkDetail_void)
        .def("buildLinkDetail", vertex_buildLinkDetail_bool)
        .def("isLinkOrientable", &Dim4Vertex::isLinkOrientable)
        .def("isValid", &Dim4Vertex::isValid)
        .def("hasBadIdentification", &Dim4Vertex::hasBadIdentification)
        .def("hasBadLink", &Dim4Vertex::hasBadLink)
        .def("isIdeal", &Dim4Vertex::isIdeal)
        .def("isBoundary", &Dim4Vertex::isBoundary)
        .def("ordering", &Dim4Vertex::ordering)
        .def("faceNumber", &Dim4Vertex::faceNumber)
        .def("containsVertex", &Dim4Vertex::containsVertex)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("ordering")
        .staticmethod("faceNumber")
        .staticmethod("containsVertex")
    ;

    scope().attr("Dim4VertexEmbedding") = scope().attr("FaceEmbedding4_0");
    scope().attr("Dim4Vertex") = scope().attr("Face4_0");
}

