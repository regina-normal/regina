
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
#include "triangulation/ntriangle.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nvertex.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using namespace regina::python;
using regina::Face;
using regina::FaceEmbedding;
using regina::NTriangle;
using regina::NTriangleEmbedding;
using regina::python::GlobalArray;

namespace {
    boost::python::list NTriangle_embeddings_list(const NTriangle* t) {
        boost::python::list ans;
        for (auto& emb: *t)
            ans.append(emb);
        return ans;
    }
}

void addNTriangle() {
    class_<FaceEmbedding<3, 2>>("FaceEmbedding3_2",
            init<regina::NTetrahedron*, int>())
        .def(init<const NTriangleEmbedding&>())
        .def("simplex", &NTriangleEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &NTriangleEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("tetrahedron", &NTriangleEmbedding::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &NTriangleEmbedding::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("face", &NTriangleEmbedding::face)
        .def("getFace", &NTriangleEmbedding::face)
        .def("triangle", &NTriangleEmbedding::triangle)
        .def("getTriangle", &NTriangleEmbedding::triangle)
        .def("vertices", &NTriangleEmbedding::vertices)
        .def("getVertices", &NTriangleEmbedding::vertices)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    {
        scope s = class_<Face<3, 2>, std::auto_ptr<Face<3, 2>>,
                boost::noncopyable>("Face3_2", no_init)
            .def("index", &NTriangle::index)
            .def("embeddings", NTriangle_embeddings_list)
            .def("getEmbeddings", NTriangle_embeddings_list)
            .def("embedding", &NTriangle::embedding,
                return_internal_reference<>())
            .def("getEmbedding", &NTriangle::embedding,
                return_internal_reference<>())
            .def("isBoundary", &NTriangle::isBoundary)
            .def("inMaximalForest", &NTriangle::inMaximalForest)
            .def("type", &NTriangle::type)
            .def("getType", &NTriangle::type)
            .def("subtype", &NTriangle::subtype)
            .def("getSubtype", &NTriangle::subtype)
            .def("isMobiusBand", &NTriangle::isMobiusBand)
            .def("isCone", &NTriangle::isCone)
            .def("isValid", &NTriangle::isValid)
            .def("isLinkOrientable", &NTriangle::isLinkOrientable)
            .def("degree", &NTriangle::degree)
            .def("getDegree", &NTriangle::degree)
            .def("front", &NTriangle::front,
                return_internal_reference<>())
            .def("back", &NTriangle::back,
                return_internal_reference<>())
            .def("triangulation", &NTriangle::triangulation,
                return_value_policy<to_held_type<> >())
            .def("getTriangulation", &NTriangle::triangulation,
                return_value_policy<to_held_type<> >())
            .def("component", &NTriangle::component,
                return_value_policy<reference_existing_object>())
            .def("getComponent", &NTriangle::component,
                return_value_policy<reference_existing_object>())
            .def("boundaryComponent", &NTriangle::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("getBoundaryComponent", &NTriangle::boundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("face", &regina::python::face<NTriangle, 2, int>)
            .def("vertex", &NTriangle::vertex,
                return_value_policy<reference_existing_object>())
            .def("getVertex", &NTriangle::vertex,
                return_value_policy<reference_existing_object>())
            .def("edge", &NTriangle::edge,
                return_value_policy<reference_existing_object>())
            .def("getEdge", &NTriangle::edge,
                return_value_policy<reference_existing_object>())
            .def("faceMapping", &regina::python::faceMapping<NTriangle, 2, 4>)
            .def("vertexMapping", &NTriangle::vertexMapping)
            .def("getVertexMapping", &NTriangle::vertexMapping)
            .def("edgeMapping", &NTriangle::edgeMapping)
            .def("getEdgeMapping", &NTriangle::edgeMapping)
            .def("ordering", &NTriangle::ordering)
            .def("faceNumber", &NTriangle::faceNumber)
            .def("containsVertex", &NTriangle::containsVertex)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
            .staticmethod("ordering")
            .staticmethod("faceNumber")
            .staticmethod("containsVertex")
        ;

        enum_<regina::NTriangle::Type>("Type")
            .value("UNKNOWN_TYPE", regina::NTriangle::UNKNOWN_TYPE)
            .value("TRIANGLE", regina::NTriangle::TRIANGLE)
            .value("SCARF", regina::NTriangle::SCARF)
            .value("PARACHUTE", regina::NTriangle::PARACHUTE)
            .value("CONE", regina::NTriangle::CONE)
            .value("MOBIUS", regina::NTriangle::MOBIUS)
            .value("HORN", regina::NTriangle::HORN)
            .value("DUNCEHAT", regina::NTriangle::DUNCEHAT)
            .value("L31", regina::NTriangle::L31)
            ;

        s.attr("TRIANGLE") = NTriangle::TRIANGLE;
        s.attr("SCARF") = NTriangle::SCARF;
        s.attr("PARACHUTE") = NTriangle::PARACHUTE;
        s.attr("CONE") = NTriangle::CONE;
        s.attr("MOBIUS") = NTriangle::MOBIUS;
        s.attr("HORN") = NTriangle::HORN;
        s.attr("DUNCEHAT") = NTriangle::DUNCEHAT;
        s.attr("L31") = NTriangle::L31;
    }

    scope().attr("NTriangleEmbedding") = scope().attr("FaceEmbedding3_2");
    scope().attr("NTriangle") = scope().attr("Face3_2");

    // Support for deprecated typedefs:
    scope().attr("NFace") = scope().attr("NTriangle");
    scope().attr("NFaceEmbedding") = scope().attr("NTriangleEmbedding");
}

