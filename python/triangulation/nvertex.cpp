
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
#include "dim2/dim2triangulation.h"
#include "triangulation/nboundarycomponent.h"
#include "triangulation/ncomponent.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nvertex.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::Face;
using regina::FaceEmbedding;
using regina::NVertex;
using regina::NVertexEmbedding;

namespace {
    boost::python::list vertex_embeddings_list(const NVertex* v) {
        boost::python::list ans;
        for (auto& emb: *v)
            ans.append(emb);
        return ans;
    }

    regina::Dim2Triangulation* vertex_buildLink(const NVertex* v) {
        return new regina::Dim2Triangulation(*(v->buildLink()));
    }

    boost::python::tuple vertex_buildLinkDetail_bool(const NVertex* v,
            bool labels = true) {
        regina::NIsomorphism* iso;
        regina::Dim2Triangulation* link = new regina::Dim2Triangulation(
            *(v->buildLinkDetail(labels, &iso)));
        return boost::python::make_tuple(
            boost::python::object(boost::python::handle<>(
                regina::python::to_held_type<>::
                apply<regina::Dim2Triangulation*>::type()(link))),
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::NIsomorphism*>::type()(iso))));
    }

    boost::python::tuple vertex_buildLinkDetail_void(const NVertex* v) {
        return vertex_buildLinkDetail_bool(v);
    }
}

void addNVertex() {
    class_<FaceEmbedding<3, 0>>("FaceEmbedding3_0",
            init<regina::NTetrahedron*, int>())
        .def(init<const NVertexEmbedding&>())
        .def("simplex", &NVertexEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &NVertexEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("tetrahedron", &NVertexEmbedding::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &NVertexEmbedding::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("face", &NVertexEmbedding::face)
        .def("getFace", &NVertexEmbedding::face)
        .def("vertex", &NVertexEmbedding::vertex)
        .def("getVertex", &NVertexEmbedding::vertex)
        .def("vertices", &NVertexEmbedding::vertices)
        .def("getVertices", &NVertexEmbedding::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    {
        scope s = class_<Face<3, 0>, std::auto_ptr<Face<3, 0>>,
                boost::noncopyable>("Face3_0", no_init)
            .def("index", &NVertex::index)
            .def("embeddings", vertex_embeddings_list)
            .def("getEmbeddings", vertex_embeddings_list)
            .def("embedding", &NVertex::embedding,
                return_internal_reference<>())
            .def("getEmbedding", &NVertex::embedding,
                return_internal_reference<>())
            .def("front", &NVertex::front,
                return_internal_reference<>())
            .def("back", &NVertex::back,
                return_internal_reference<>())
            .def("triangulation", &NVertex::triangulation,
                return_value_policy<to_held_type<> >())
            .def("getTriangulation", &NVertex::triangulation,
                return_value_policy<to_held_type<> >())
            .def("component", &NVertex::component,
                return_value_policy<reference_existing_object>())
            .def("getComponent", &NVertex::component,
                return_value_policy<reference_existing_object>())
            .def("boundaryComponent", &NVertex::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("getBoundaryComponent", &NVertex::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("degree", &NVertex::degree)
            .def("getDegree", &NVertex::degree)
            .def("link", &NVertex::link)
            .def("getLink", &NVertex::link)
            .def("buildLink", &vertex_buildLink,
                return_value_policy<to_held_type<>>())
            .def("buildLinkDetail", vertex_buildLinkDetail_void)
            .def("buildLinkDetail", vertex_buildLinkDetail_bool)
            .def("isLinkClosed", &NVertex::isLinkClosed)
            .def("isIdeal", &NVertex::isIdeal)
            .def("isBoundary", &NVertex::isBoundary)
            .def("isStandard", &NVertex::isStandard)
            .def("isValid", &NVertex::isValid)
            .def("hasBadIdentification", &NVertex::hasBadIdentification)
            .def("hasBadLink", &NVertex::hasBadLink)
            .def("isLinkOrientable", &NVertex::isLinkOrientable)
            .def("linkEulerChar", &NVertex::linkEulerChar)
            .def("getLinkEulerChar", &NVertex::linkEulerChar)
            .def("getLinkEulerCharacteristic", &NVertex::linkEulerChar)
            .def("ordering", &NVertex::ordering)
            .def("faceNumber", &NVertex::faceNumber)
            .def("containsVertex", &NVertex::containsVertex)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
            .staticmethod("ordering")
            .staticmethod("faceNumber")
            .staticmethod("containsVertex")
        ;

        enum_<regina::NVertex::LinkType>("LinkType")
            .value("SPHERE", regina::NVertex::SPHERE)
            .value("DISC", regina::NVertex::DISC)
            .value("TORUS", regina::NVertex::TORUS)
            .value("KLEIN_BOTTLE", regina::NVertex::KLEIN_BOTTLE)
            .value("NON_STANDARD_CUSP", regina::NVertex::NON_STANDARD_CUSP)
            .value("NON_STANDARD_BDRY", regina::NVertex::NON_STANDARD_BDRY)
            ;

        s.attr("SPHERE") = NVertex::SPHERE;
        s.attr("DISC") = NVertex::DISC;
        s.attr("TORUS") = NVertex::TORUS;
        s.attr("KLEIN_BOTTLE") = NVertex::KLEIN_BOTTLE;
        s.attr("NON_STANDARD_CUSP") = NVertex::NON_STANDARD_CUSP;
        s.attr("NON_STANDARD_BDRY") = NVertex::NON_STANDARD_BDRY;
    }

    scope().attr("NVertexEmbedding") = scope().attr("FaceEmbedding3_0");
    scope().attr("NVertex") = scope().attr("Face3_0");
}

