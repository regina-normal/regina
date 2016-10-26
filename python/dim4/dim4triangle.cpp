
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
#include "triangulation/dim4.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using namespace regina::python;
using regina::Triangle;
using regina::TriangleEmbedding;
using regina::Face;
using regina::FaceEmbedding;
using regina::python::GlobalArray;
using regina::python::GlobalArray2D;
using regina::python::GlobalArray3D;

namespace {
    GlobalArray3D<int> Triangle4_triangleNumber(
        Triangle<4>::triangleNumber, 5);
    GlobalArray2D<int> Triangle4_triangleVertex(
        Triangle<4>::triangleVertex, 10);

    boost::python::list Triangle4_embeddings_list(const Triangle<4>* t) {
        boost::python::list ans;
        for (auto& emb : *t)
            ans.append(emb);
        return ans;
    }
}

void addTriangle4() {
    class_<FaceEmbedding<4, 2>>("FaceEmbedding4_2",
            init<regina::Pentachoron<4>*, int>())
        .def(init<const TriangleEmbedding<4>&>())
        .def("simplex", &TriangleEmbedding<4>::simplex,
            return_value_policy<reference_existing_object>())
        .def("pentachoron", &TriangleEmbedding<4>::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("face", &TriangleEmbedding<4>::face)
        .def("triangle", &TriangleEmbedding<4>::triangle)
        .def("vertices", &TriangleEmbedding<4>::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    {
        scope s = class_<Face<4, 2>, std::auto_ptr<Face<4, 2>>,
                boost::noncopyable>("Face4_2", no_init)
            .def("index", &Triangle<4>::index)
            .def("embeddings", Triangle4_embeddings_list)
            .def("embedding", &Triangle<4>::embedding,
                return_internal_reference<>())
            .def("front", &Triangle<4>::front,
                return_internal_reference<>())
            .def("back", &Triangle<4>::back,
                return_internal_reference<>())
            .def("triangulation", &Triangle<4>::triangulation,
                return_value_policy<to_held_type<>>())
            .def("component", &Triangle<4>::component,
                return_value_policy<reference_existing_object>())
            .def("boundaryComponent", &Triangle<4>::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("face", &regina::python::face<Triangle<4>, 2, int>)
            .def("vertex", &Triangle<4>::vertex,
                return_value_policy<reference_existing_object>())
            .def("edge", &Triangle<4>::edge,
                return_value_policy<reference_existing_object>())
            .def("faceMapping",
                &regina::python::faceMapping<Triangle<4>, 2, 5>)
            .def("vertexMapping", &Triangle<4>::vertexMapping)
            .def("edgeMapping", &Triangle<4>::edgeMapping)
            .def("degree", &Triangle<4>::degree)
            .def("isBoundary", &Triangle<4>::isBoundary)
            .def("isLinkOrientable", &Triangle<4>::isLinkOrientable)
            .def("isValid", &Triangle<4>::isValid)
            .def("hasBadIdentification", &Triangle<4>::hasBadIdentification)
            .def("hasBadLink", &Triangle<4>::hasBadLink)
            .def("ordering", &Triangle<4>::ordering)
            .def("faceNumber", &Triangle<4>::faceNumber)
            .def("containsVertex", &Triangle<4>::containsVertex)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
            .staticmethod("ordering")
            .staticmethod("faceNumber")
            .staticmethod("containsVertex")
        ;

        s.attr("triangleNumber") = &Triangle4_triangleNumber;
        s.attr("triangleVertex") = &Triangle4_triangleVertex;
    }

    scope().attr("Dim4TriangleEmbedding") = scope().attr("FaceEmbedding4_2");
    scope().attr("TriangleEmbedding4") = scope().attr("FaceEmbedding4_2");
    scope().attr("Dim4Triangle") = scope().attr("Face4_2");
    scope().attr("Triangle4") = scope().attr("Face4_2");
}

