
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

#include "triangulation/dim2.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Edge;
using regina::EdgeEmbedding;
using regina::Face;
using regina::FaceEmbedding;
using regina::python::GlobalArray;

namespace {
    boost::python::list Edge2_embeddings_list(const Edge<2>* e) {
        boost::python::list ans;
        for (auto& emb: *e)
            ans.append(emb);
        return ans;
    }
}

void addEdge2() {
    class_<FaceEmbedding<2, 1>>("FaceEmbedding2_1",
            init<regina::Triangle<2>*, int>())
        .def(init<const EdgeEmbedding<2>&>())
        .def("simplex", &EdgeEmbedding<2>::simplex,
            return_value_policy<reference_existing_object>())
        .def("triangle", &EdgeEmbedding<2>::triangle,
            return_value_policy<reference_existing_object>())
        .def("face", &EdgeEmbedding<2>::face)
        .def("edge", &EdgeEmbedding<2>::edge)
        .def("vertices", &EdgeEmbedding<2>::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<Face<2, 1>, std::auto_ptr<Face<2, 1>>, boost::noncopyable>
            ("Face2_1", no_init)
        .def("index", &Edge<2>::index)
        .def("isValid", &Edge<2>::isValid)
        .def("isLinkOrientable", &Edge<2>::isLinkOrientable)
        .def("embeddings", Edge2_embeddings_list)
        .def("degree", &Edge<2>::degree)
        .def("embedding", &Edge<2>::embedding,
            return_internal_reference<>())
        .def("front", &Edge<2>::front,
            return_internal_reference<>())
        .def("back", &Edge<2>::back,
            return_internal_reference<>())
        .def("triangulation", &Edge<2>::triangulation,
            return_value_policy<to_held_type<> >())
        .def("component", &Edge<2>::component,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Edge<2>::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<Edge<2>, 1, int>)
        .def("vertex", &Edge<2>::vertex,
            return_value_policy<reference_existing_object>())
        .def("faceMapping", &regina::python::faceMapping<Edge<2>, 1, 3>)
        .def("vertexMapping", &Edge<2>::vertexMapping)
        .def("isBoundary", &Edge<2>::isBoundary)
        .def("inMaximalForest", &Edge<2>::inMaximalForest)
        .def("ordering", &Edge<2>::ordering)
        .def("faceNumber", &Edge<2>::faceNumber)
        .def("containsVertex", &Edge<2>::containsVertex)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("ordering")
        .staticmethod("faceNumber")
        .staticmethod("containsVertex")
    ;

    scope().attr("Dim2EdgeEmbedding") = scope().attr("FaceEmbedding2_1");
    scope().attr("EdgeEmbedding2") = scope().attr("FaceEmbedding2_1");
    scope().attr("Dim2Edge") = scope().attr("Face2_1");
    scope().attr("Edge2") = scope().attr("Face2_1");
}

