
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
#include "triangulation/nboundarycomponent.h"
#include "triangulation/ncomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nvertex.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using namespace regina::python;
using regina::NEdge;
using regina::NEdgeEmbedding;
using regina::Face;
using regina::FaceEmbedding;
using regina::python::GlobalArray2D;

namespace {
    GlobalArray2D<int> NEdge_edgeNumber(NEdge::edgeNumber, 4);
    GlobalArray2D<int> NEdge_edgeVertex(NEdge::edgeVertex, 6);

    boost::python::list edge_embeddings_list(const NEdge* e) {
        boost::python::list ans;
        for (auto& emb: *e)
            ans.append(emb);
        return ans;
    }
}

void addNEdge() {
    // Classes:
    class_<FaceEmbedding<3, 1>>("FaceEmbedding3_1",
            init<regina::NTetrahedron*, int>())
        .def(init<const NEdgeEmbedding&>())
        .def("simplex", &NEdgeEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &NEdgeEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("tetrahedron", &NEdgeEmbedding::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &NEdgeEmbedding::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("face", &NEdgeEmbedding::face)
        .def("getFace", &NEdgeEmbedding::face)
        .def("edge", &NEdgeEmbedding::edge)
        .def("getEdge", &NEdgeEmbedding::edge)
        .def("vertices", &NEdgeEmbedding::vertices)
        .def("getVertices", &NEdgeEmbedding::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    {
        scope s = class_<Face<3, 1>, std::auto_ptr<Face<3, 1>>,
                boost::noncopyable>("Face3_1", no_init)
            .def("index", &NEdge::index)
            .def("embeddings", edge_embeddings_list)
            .def("getEmbeddings", edge_embeddings_list)
            .def("embedding", &NEdge::embedding,
                return_internal_reference<>())
            .def("getEmbedding", &NEdge::embedding,
                return_internal_reference<>())
            .def("front", &NEdge::front,
                return_internal_reference<>())
            .def("back", &NEdge::back,
                return_internal_reference<>())
            .def("triangulation", &NEdge::triangulation,
                return_value_policy<to_held_type<> >())
            .def("getTriangulation", &NEdge::triangulation,
                return_value_policy<to_held_type<> >())
            .def("component", &NEdge::component,
                return_value_policy<reference_existing_object>())
            .def("getComponent", &NEdge::component,
                return_value_policy<reference_existing_object>())
            .def("boundaryComponent", &NEdge::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("getBoundaryComponent", &NEdge::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("face", &regina::python::face<NEdge, 1, int>)
            .def("vertex", &NEdge::vertex,
                return_value_policy<reference_existing_object>())
            .def("getVertex", &NEdge::vertex,
                return_value_policy<reference_existing_object>())
            .def("faceMapping", &regina::python::faceMapping<NEdge, 1, 4>)
            .def("vertexMapping", &NEdge::vertexMapping)
            .def("getVertexMapping", &NEdge::vertexMapping)
            .def("degree", &NEdge::degree)
            .def("getDegree", &NEdge::degree)
            .def("isBoundary", &NEdge::isBoundary)
            .def("isValid", &NEdge::isValid)
            .def("hasBadIdentification", &NEdge::hasBadIdentification)
            .def("hasBadLink", &NEdge::hasBadLink)
            .def("isLinkOrientable", &NEdge::isLinkOrientable)
            .def("ordering", &NEdge::ordering)
            .def("faceNumber", &NEdge::faceNumber)
            .def("containsVertex", &NEdge::containsVertex)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
            .staticmethod("ordering")
            .staticmethod("faceNumber")
            .staticmethod("containsVertex")
        ;

        s.attr("edgeNumber") = &NEdge_edgeNumber;
        s.attr("edgeVertex") = &NEdge_edgeVertex;
    }

    scope().attr("NEdgeEmbedding") = scope().attr("FaceEmbedding3_1");
    scope().attr("NEdge") = scope().attr("Face3_1");
}

