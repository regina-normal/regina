
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
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Vertex;
using regina::VertexEmbedding;
using regina::Face;
using regina::FaceEmbedding;

namespace {
    boost::python::list Vertex2_embeddings_list(const Vertex<2>* v) {
        boost::python::list ans;
        for (auto& emb: *v)
            ans.append(emb);
        return ans;
    }
}

void addVertex2() {
    class_<FaceEmbedding<2, 0>>("FaceEmbedding2_0",
            init<regina::Triangle<2>*, int>())
        .def(init<const VertexEmbedding<2>&>())
        .def("simplex", &VertexEmbedding<2>::simplex,
            return_value_policy<reference_existing_object>())
        .def("triangle", &VertexEmbedding<2>::triangle,
            return_value_policy<reference_existing_object>())
        .def("face", &VertexEmbedding<2>::face)
        .def("vertex", &VertexEmbedding<2>::vertex)
        .def("vertices", &VertexEmbedding<2>::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<Face<2, 0>, std::auto_ptr<Face<2, 0>>, boost::noncopyable>
            ("Face2_0", no_init)
        .def("index", &Vertex<2>::index)
        .def("isValid", &Vertex<2>::isValid)
        .def("isLinkOrientable", &Vertex<2>::isLinkOrientable)
        .def("embeddings", Vertex2_embeddings_list)
        .def("embedding", &Vertex<2>::embedding,
            return_internal_reference<>())
        .def("front", &Vertex<2>::front,
            return_internal_reference<>())
        .def("back", &Vertex<2>::back,
            return_internal_reference<>())
        .def("triangulation", &Vertex<2>::triangulation,
            return_value_policy<to_held_type<> >())
        .def("component", &Vertex<2>::component,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Vertex<2>::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("degree", &Vertex<2>::degree)
        .def("isBoundary", &Vertex<2>::isBoundary)
        .def("ordering", &Vertex<2>::ordering)
        .def("faceNumber", &Vertex<2>::faceNumber)
        .def("containsVertex", &Vertex<2>::containsVertex)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("ordering")
        .staticmethod("faceNumber")
        .staticmethod("containsVertex")
    ;

    scope().attr("Dim2VertexEmbedding") = scope().attr("FaceEmbedding2_0");
    scope().attr("VertexEmbedding2") = scope().attr("FaceEmbedding2_0");
    scope().attr("Dim2Vertex") = scope().attr("Face2_0");
    scope().attr("Vertex2") = scope().attr("Face2_0");
}

