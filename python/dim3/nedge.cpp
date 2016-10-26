
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
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using namespace regina::python;
using regina::Edge;
using regina::EdgeEmbedding;
using regina::Face;
using regina::FaceEmbedding;
using regina::python::GlobalArray2D;

namespace {
    GlobalArray2D<int> Edge3_edgeNumber(Edge<3>::edgeNumber, 4);
    GlobalArray2D<int> Edge3_edgeVertex(Edge<3>::edgeVertex, 6);

    boost::python::list edge_embeddings_list(const Edge<3>* e) {
        boost::python::list ans;
        for (auto& emb: *e)
            ans.append(emb);
        return ans;
    }
}

void addEdge3() {
    // Classes:
    class_<FaceEmbedding<3, 1>>("FaceEmbedding3_1",
            init<regina::Tetrahedron<3>*, int>())
        .def(init<const EdgeEmbedding<3>&>())
        .def("simplex", &EdgeEmbedding<3>::simplex,
            return_value_policy<reference_existing_object>())
        .def("tetrahedron", &EdgeEmbedding<3>::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("face", &EdgeEmbedding<3>::face)
        .def("edge", &EdgeEmbedding<3>::edge)
        .def("vertices", &EdgeEmbedding<3>::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    {
        scope s = class_<Face<3, 1>, std::auto_ptr<Face<3, 1>>,
                boost::noncopyable>("Face3_1", no_init)
            .def("index", &Edge<3>::index)
            .def("embeddings", edge_embeddings_list)
            .def("embedding", &Edge<3>::embedding,
                return_internal_reference<>())
            .def("front", &Edge<3>::front,
                return_internal_reference<>())
            .def("back", &Edge<3>::back,
                return_internal_reference<>())
            .def("triangulation", &Edge<3>::triangulation,
                return_value_policy<to_held_type<> >())
            .def("component", &Edge<3>::component,
                return_value_policy<reference_existing_object>())
            .def("boundaryComponent", &Edge<3>::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("face", &regina::python::face<Edge<3>, 1, int>)
            .def("vertex", &Edge<3>::vertex,
                return_value_policy<reference_existing_object>())
            .def("faceMapping", &regina::python::faceMapping<Edge<3>, 1, 4>)
            .def("vertexMapping", &Edge<3>::vertexMapping)
            .def("degree", &Edge<3>::degree)
            .def("isBoundary", &Edge<3>::isBoundary)
            .def("isValid", &Edge<3>::isValid)
            .def("hasBadIdentification", &Edge<3>::hasBadIdentification)
            .def("hasBadLink", &Edge<3>::hasBadLink)
            .def("isLinkOrientable", &Edge<3>::isLinkOrientable)
            .def("ordering", &Edge<3>::ordering)
            .def("faceNumber", &Edge<3>::faceNumber)
            .def("containsVertex", &Edge<3>::containsVertex)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
            .staticmethod("ordering")
            .staticmethod("faceNumber")
            .staticmethod("containsVertex")
        ;

        s.attr("edgeNumber") = &Edge3_edgeNumber;
        s.attr("edgeVertex") = &Edge3_edgeVertex;
    }

    scope().attr("NEdgeEmbedding") = scope().attr("FaceEmbedding3_1");
    scope().attr("EdgeEmbedding3") = scope().attr("FaceEmbedding3_1");
    scope().attr("NEdge") = scope().attr("Face3_1");
    scope().attr("Edge3") = scope().attr("Face3_1");
}

