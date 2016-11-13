
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
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::Face;
using regina::FaceEmbedding;
using regina::Vertex;
using regina::VertexEmbedding;

namespace {
    boost::python::list vertex_embeddings_list(const Vertex<3>* v) {
        boost::python::list ans;
        for (auto& emb: *v)
            ans.append(emb);
        return ans;
    }

    regina::Triangulation<2>* vertex_buildLink(const Vertex<3>* v) {
        return new regina::Triangulation<2>(*(v->buildLink()));
    }

    boost::python::tuple vertex_buildLinkDetail_bool(const Vertex<3>* v,
            bool labels = true) {
        regina::Isomorphism<3>* iso;
        regina::Triangulation<2>* link = new regina::Triangulation<2>(
            *(v->buildLinkDetail(labels, &iso)));
        return boost::python::make_tuple(
            boost::python::object(boost::python::handle<>(
                regina::python::to_held_type<>::
                apply<regina::Triangulation<2>*>::type()(link))),
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Isomorphism<3>*>::type()(iso))));
    }

    boost::python::tuple vertex_buildLinkDetail_void(const Vertex<3>* v) {
        return vertex_buildLinkDetail_bool(v);
    }
}

void addVertex3() {
    class_<FaceEmbedding<3, 0>>("FaceEmbedding3_0",
            init<regina::Tetrahedron<3>*, int>())
        .def(init<const VertexEmbedding<3>&>())
        .def("simplex", &VertexEmbedding<3>::simplex,
            return_value_policy<reference_existing_object>())
        .def("tetrahedron", &VertexEmbedding<3>::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("face", &VertexEmbedding<3>::face)
        .def("vertex", &VertexEmbedding<3>::vertex)
        .def("vertices", &VertexEmbedding<3>::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    {
        scope s = class_<Face<3, 0>, std::auto_ptr<Face<3, 0>>,
                boost::noncopyable>("Face3_0", no_init)
            .def("index", &Vertex<3>::index)
            .def("embeddings", vertex_embeddings_list)
            .def("embedding", &Vertex<3>::embedding,
                return_internal_reference<>())
            .def("front", &Vertex<3>::front,
                return_internal_reference<>())
            .def("back", &Vertex<3>::back,
                return_internal_reference<>())
            .def("triangulation", &Vertex<3>::triangulation,
                return_value_policy<to_held_type<> >())
            .def("component", &Vertex<3>::component,
                return_value_policy<reference_existing_object>())
            .def("boundaryComponent", &Vertex<3>::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("degree", &Vertex<3>::degree)
            .def("link", &Vertex<3>::link)
            .def("buildLink", &vertex_buildLink,
                return_value_policy<to_held_type<>>())
            .def("buildLinkDetail", vertex_buildLinkDetail_void)
            .def("buildLinkDetail", vertex_buildLinkDetail_bool)
            .def("isLinkClosed", &Vertex<3>::isLinkClosed)
            .def("isIdeal", &Vertex<3>::isIdeal)
            .def("isBoundary", &Vertex<3>::isBoundary)
            .def("isStandard", &Vertex<3>::isStandard)
            .def("isValid", &Vertex<3>::isValid)
            .def("hasBadIdentification", &Vertex<3>::hasBadIdentification)
            .def("hasBadLink", &Vertex<3>::hasBadLink)
            .def("isLinkOrientable", &Vertex<3>::isLinkOrientable)
            .def("linkEulerChar", &Vertex<3>::linkEulerChar)
            .def("ordering", &Vertex<3>::ordering)
            .def("faceNumber", &Vertex<3>::faceNumber)
            .def("containsVertex", &Vertex<3>::containsVertex)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
            .staticmethod("ordering")
            .staticmethod("faceNumber")
            .staticmethod("containsVertex")
        ;

        enum_<regina::Vertex<3>::LinkType>("LinkType")
            .value("SPHERE", regina::Vertex<3>::SPHERE)
            .value("DISC", regina::Vertex<3>::DISC)
            .value("TORUS", regina::Vertex<3>::TORUS)
            .value("KLEIN_BOTTLE", regina::Vertex<3>::KLEIN_BOTTLE)
            .value("NON_STANDARD_CUSP", regina::Vertex<3>::NON_STANDARD_CUSP)
            .value("INVALID", regina::Vertex<3>::INVALID)
            ;

        s.attr("SPHERE") = Vertex<3>::SPHERE;
        s.attr("DISC") = Vertex<3>::DISC;
        s.attr("TORUS") = Vertex<3>::TORUS;
        s.attr("KLEIN_BOTTLE") = Vertex<3>::KLEIN_BOTTLE;
        s.attr("NON_STANDARD_CUSP") = Vertex<3>::NON_STANDARD_CUSP;
        s.attr("INVALID") = Vertex<3>::INVALID;
    }

    scope().attr("NVertexEmbedding") = scope().attr("FaceEmbedding3_0");
    scope().attr("VertexEmbedding3") = scope().attr("FaceEmbedding3_0");
    scope().attr("NVertex") = scope().attr("Face3_0");
    scope().attr("Vertex3") = scope().attr("Face3_0");
}

