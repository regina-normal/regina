
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
#include "dim4/dim4edge.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4triangle.h"
#include "dim4/dim4triangulation.h"
#include "dim4/dim4vertex.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using namespace regina::python;
using regina::Dim4Triangle;
using regina::Dim4TriangleEmbedding;
using regina::Face;
using regina::FaceEmbedding;
using regina::python::GlobalArray;
using regina::python::GlobalArray2D;
using regina::python::GlobalArray3D;

namespace {
    GlobalArray3D<int> Dim4Triangle_triangleNumber(
        Dim4Triangle::triangleNumber, 5);
    GlobalArray2D<int> Dim4Triangle_triangleVertex(
        Dim4Triangle::triangleVertex, 10);

    boost::python::list Dim4Triangle_embeddings_list(const Dim4Triangle* t) {
        boost::python::list ans;
        for (auto& emb : *t)
            ans.append(emb);
        return ans;
    }
}

void addDim4Triangle() {
    class_<FaceEmbedding<4, 2>>("FaceEmbedding4_2",
            init<regina::Dim4Pentachoron*, int>())
        .def(init<const Dim4TriangleEmbedding&>())
        .def("simplex", &Dim4TriangleEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &Dim4TriangleEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("pentachoron", &Dim4TriangleEmbedding::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("getPentachoron", &Dim4TriangleEmbedding::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("face", &Dim4TriangleEmbedding::face)
        .def("getFace", &Dim4TriangleEmbedding::face)
        .def("triangle", &Dim4TriangleEmbedding::triangle)
        .def("getTriangle", &Dim4TriangleEmbedding::triangle)
        .def("vertices", &Dim4TriangleEmbedding::vertices)
        .def("getVertices", &Dim4TriangleEmbedding::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    {
        scope s = class_<Face<4, 2>, std::auto_ptr<Face<4, 2>>,
                boost::noncopyable>("Face4_2", no_init)
            .def("index", &Dim4Triangle::index)
            .def("embeddings", Dim4Triangle_embeddings_list)
            .def("getEmbeddings", Dim4Triangle_embeddings_list)
            .def("embedding", &Dim4Triangle::embedding,
                return_internal_reference<>())
            .def("getEmbedding", &Dim4Triangle::embedding,
                return_internal_reference<>())
            .def("front", &Dim4Triangle::front,
                return_internal_reference<>())
            .def("back", &Dim4Triangle::back,
                return_internal_reference<>())
            .def("triangulation", &Dim4Triangle::triangulation,
                return_value_policy<to_held_type<>>())
            .def("getTriangulation", &Dim4Triangle::triangulation,
                return_value_policy<to_held_type<>>())
            .def("component", &Dim4Triangle::component,
                return_value_policy<reference_existing_object>())
            .def("getComponent", &Dim4Triangle::component,
                return_value_policy<reference_existing_object>())
            .def("boundaryComponent", &Dim4Triangle::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("getBoundaryComponent", &Dim4Triangle::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("face", &regina::python::face<Dim4Triangle, 2, int>)
            .def("vertex", &Dim4Triangle::vertex,
                return_value_policy<reference_existing_object>())
            .def("getVertex", &Dim4Triangle::vertex,
                return_value_policy<reference_existing_object>())
            .def("edge", &Dim4Triangle::edge,
                return_value_policy<reference_existing_object>())
            .def("getEdge", &Dim4Triangle::edge,
                return_value_policy<reference_existing_object>())
            .def("faceMapping",
                &regina::python::faceMapping<Dim4Triangle, 2, 5>)
            .def("vertexMapping", &Dim4Triangle::vertexMapping)
            .def("getVertexMapping", &Dim4Triangle::vertexMapping)
            .def("edgeMapping", &Dim4Triangle::edgeMapping)
            .def("getEdgeMapping", &Dim4Triangle::edgeMapping)
            .def("degree", &Dim4Triangle::degree)
            .def("getDegree", &Dim4Triangle::degree)
            .def("isBoundary", &Dim4Triangle::isBoundary)
            .def("isLinkOrientable", &Dim4Triangle::isLinkOrientable)
            .def("isValid", &Dim4Triangle::isValid)
            .def("hasBadIdentification", &Dim4Triangle::hasBadIdentification)
            .def("hasBadLink", &Dim4Triangle::hasBadLink)
            .def("ordering", &Dim4Triangle::ordering)
            .def("faceNumber", &Dim4Triangle::faceNumber)
            .def("containsVertex", &Dim4Triangle::containsVertex)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
            .staticmethod("ordering")
            .staticmethod("faceNumber")
            .staticmethod("containsVertex")
        ;

        s.attr("triangleNumber") = &Dim4Triangle_triangleNumber;
        s.attr("triangleVertex") = &Dim4Triangle_triangleVertex;
    }

    scope().attr("Dim4TriangleEmbedding") = scope().attr("FaceEmbedding4_2");
    scope().attr("Dim4Triangle") = scope().attr("Face4_2");
}

