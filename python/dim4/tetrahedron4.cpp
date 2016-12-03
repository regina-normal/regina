
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
using regina::Tetrahedron;
using regina::TetrahedronEmbedding;
using regina::Face;
using regina::FaceEmbedding;
using regina::python::GlobalArray;

namespace {
    boost::python::list Tetrahedron4_embeddings_list(
            const Tetrahedron<4>* t) {
        boost::python::list ans;
        for (auto& emb : *t)
            ans.append(emb);
        return ans;
    }
}

void addTetrahedron4() {
    class_<FaceEmbedding<4, 3>>("FaceEmbedding4_3",
            init<regina::Pentachoron<4>*, int>())
        .def(init<const TetrahedronEmbedding<4>&>())
        .def("simplex", &TetrahedronEmbedding<4>::simplex,
            return_value_policy<reference_existing_object>())
        .def("pentachoron", &TetrahedronEmbedding<4>::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("face", &TetrahedronEmbedding<4>::face)
        .def("tetrahedron", &TetrahedronEmbedding<4>::tetrahedron)
        .def("vertices", &TetrahedronEmbedding<4>::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<Face<4, 3>, std::auto_ptr<Face<4, 3>>,
            boost::noncopyable>("Face4_3", no_init)
        .def("index", &Tetrahedron<4>::index)
        .def("degree", &Tetrahedron<4>::degree)
        .def("embeddings", Tetrahedron4_embeddings_list)
        .def("embedding", &Tetrahedron<4>::embedding,
            return_internal_reference<>())
        .def("front", &Tetrahedron<4>::front,
            return_internal_reference<>())
        .def("back", &Tetrahedron<4>::back,
            return_internal_reference<>())
        .def("triangulation", &Tetrahedron<4>::triangulation,
            return_value_policy<to_held_type<>>())
        .def("component", &Tetrahedron<4>::component,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Tetrahedron<4>::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<Tetrahedron<4>, 3, int>)
        .def("vertex", &Tetrahedron<4>::vertex,
            return_value_policy<reference_existing_object>())
        .def("edge", &Tetrahedron<4>::edge,
            return_value_policy<reference_existing_object>())
        .def("triangle", &Tetrahedron<4>::triangle,
            return_value_policy<reference_existing_object>())
        .def("faceMapping", &regina::python::faceMapping<Tetrahedron<4>, 3, 5>)
        .def("vertexMapping", &Tetrahedron<4>::vertexMapping)
        .def("edgeMapping", &Tetrahedron<4>::edgeMapping)
        .def("triangleMapping", &Tetrahedron<4>::triangleMapping)
        .def("isValid", &Tetrahedron<4>::isValid)
        .def("isLinkOrientable", &Tetrahedron<4>::isLinkOrientable)
        .def("isBoundary", &Tetrahedron<4>::isBoundary)
        .def("inMaximalForest", &Tetrahedron<4>::inMaximalForest)
        .def("ordering", &Tetrahedron<4>::ordering)
        .def("faceNumber", &Tetrahedron<4>::faceNumber)
        .def("containsVertex", &Tetrahedron<4>::containsVertex)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("ordering")
        .staticmethod("faceNumber")
        .staticmethod("containsVertex")
    ;

    scope().attr("Dim4TetrahedronEmbedding") = scope().attr("FaceEmbedding4_3");
    scope().attr("TetrahedronEmbedding4") = scope().attr("FaceEmbedding4_3");
    scope().attr("Dim4Tetrahedron") = scope().attr("Face4_3");
    scope().attr("Tetrahedron4") = scope().attr("Face4_3");
}

