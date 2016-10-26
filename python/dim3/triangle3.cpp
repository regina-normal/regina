
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
using regina::Face;
using regina::FaceEmbedding;
using regina::Triangle;
using regina::TriangleEmbedding;
using regina::python::GlobalArray;

namespace {
    boost::python::list Triangle3_embeddings_list(const Triangle<3>* t) {
        boost::python::list ans;
        for (auto& emb: *t)
            ans.append(emb);
        return ans;
    }
}

void addTriangle3() {
    class_<FaceEmbedding<3, 2>>("FaceEmbedding3_2",
            init<regina::Tetrahedron<3>*, int>())
        .def(init<const TriangleEmbedding<3>&>())
        .def("simplex", &TriangleEmbedding<3>::simplex,
            return_value_policy<reference_existing_object>())
        .def("tetrahedron", &TriangleEmbedding<3>::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("face", &TriangleEmbedding<3>::face)
        .def("triangle", &TriangleEmbedding<3>::triangle)
        .def("vertices", &TriangleEmbedding<3>::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    {
        scope s = class_<Face<3, 2>, std::auto_ptr<Face<3, 2>>,
                boost::noncopyable>("Face3_2", no_init)
            .def("index", &Triangle<3>::index)
            .def("embeddings", Triangle3_embeddings_list)
            .def("embedding", &Triangle<3>::embedding,
                return_internal_reference<>())
            .def("isBoundary", &Triangle<3>::isBoundary)
            .def("inMaximalForest", &Triangle<3>::inMaximalForest)
            .def("type", &Triangle<3>::type)
            .def("subtype", &Triangle<3>::subtype)
            .def("isMobiusBand", &Triangle<3>::isMobiusBand)
            .def("isCone", &Triangle<3>::isCone)
            .def("isValid", &Triangle<3>::isValid)
            .def("isLinkOrientable", &Triangle<3>::isLinkOrientable)
            .def("degree", &Triangle<3>::degree)
            .def("front", &Triangle<3>::front,
                return_internal_reference<>())
            .def("back", &Triangle<3>::back,
                return_internal_reference<>())
            .def("triangulation", &Triangle<3>::triangulation,
                return_value_policy<to_held_type<> >())
            .def("component", &Triangle<3>::component,
                return_value_policy<reference_existing_object>())
            .def("boundaryComponent", &Triangle<3>::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("face", &regina::python::face<Triangle<3>, 2, int>)
            .def("vertex", &Triangle<3>::vertex,
                return_value_policy<reference_existing_object>())
            .def("edge", &Triangle<3>::edge,
                return_value_policy<reference_existing_object>())
            .def("faceMapping", &regina::python::faceMapping<Triangle<3>, 2, 4>)
            .def("vertexMapping", &Triangle<3>::vertexMapping)
            .def("edgeMapping", &Triangle<3>::edgeMapping)
            .def("ordering", &Triangle<3>::ordering)
            .def("faceNumber", &Triangle<3>::faceNumber)
            .def("containsVertex", &Triangle<3>::containsVertex)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
            .staticmethod("ordering")
            .staticmethod("faceNumber")
            .staticmethod("containsVertex")
        ;

        enum_<regina::Triangle<3>::Type>("Type")
            .value("UNKNOWN_TYPE", regina::Triangle<3>::UNKNOWN_TYPE)
            .value("TRIANGLE", regina::Triangle<3>::TRIANGLE)
            .value("SCARF", regina::Triangle<3>::SCARF)
            .value("PARACHUTE", regina::Triangle<3>::PARACHUTE)
            .value("CONE", regina::Triangle<3>::CONE)
            .value("MOBIUS", regina::Triangle<3>::MOBIUS)
            .value("HORN", regina::Triangle<3>::HORN)
            .value("DUNCEHAT", regina::Triangle<3>::DUNCEHAT)
            .value("L31", regina::Triangle<3>::L31)
            ;

        s.attr("TRIANGLE") = Triangle<3>::TRIANGLE;
        s.attr("SCARF") = Triangle<3>::SCARF;
        s.attr("PARACHUTE") = Triangle<3>::PARACHUTE;
        s.attr("CONE") = Triangle<3>::CONE;
        s.attr("MOBIUS") = Triangle<3>::MOBIUS;
        s.attr("HORN") = Triangle<3>::HORN;
        s.attr("DUNCEHAT") = Triangle<3>::DUNCEHAT;
        s.attr("L31") = Triangle<3>::L31;
    }

    scope().attr("NTriangleEmbedding") = scope().attr("FaceEmbedding3_2");
    scope().attr("TriangleEmbedding3") = scope().attr("FaceEmbedding3_2");
    scope().attr("NTriangle") = scope().attr("Face3_2");
    scope().attr("Triangle3") = scope().attr("Face3_2");
}

