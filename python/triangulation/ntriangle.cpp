
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

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

using namespace boost::python;
using regina::NTriangle;
using regina::NTriangleEmbedding;
using regina::python::GlobalArray;

namespace {
    boost::python::list NTriangle_getEmbeddings_list(const NTriangle* t) {
        boost::python::list ans;
        for (auto& emb: *t)
            ans.append(emb);
        return ans;
    }
}

void addNTriangle() {
    class_<NTriangleEmbedding, boost::noncopyable>("NTriangleEmbedding",
            init<regina::NTetrahedron*, int>())
        .def(init<const NTriangleEmbedding&>())
        .def("simplex", &NTriangleEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &NTriangleEmbedding::getSimplex,
            return_value_policy<reference_existing_object>())
        .def("tetrahedron", &NTriangleEmbedding::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &NTriangleEmbedding::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("face", &NTriangleEmbedding::face)
        .def("getFace", &NTriangleEmbedding::getFace)
        .def("triangle", &NTriangleEmbedding::triangle)
        .def("getTriangle", &NTriangleEmbedding::getTriangle)
        .def("vertices", &NTriangleEmbedding::vertices)
        .def("getVertices", &NTriangleEmbedding::getVertices)
        .def("str", &NTriangleEmbedding::str)
        .def("toString", &NTriangleEmbedding::toString)
        .def("detail", &NTriangleEmbedding::detail)
        .def("toStringLong", &NTriangleEmbedding::toStringLong)
        .def("__str__", &NTriangleEmbedding::str)
        .def(regina::python::add_eq_operators())
    ;

    {
        scope s = class_<NTriangle, std::auto_ptr<NTriangle>,
                boost::noncopyable>("NTriangle", no_init)
            .def("index", &NTriangle::index)
            .def("embeddings", NTriangle_getEmbeddings_list)
            .def("getEmbeddings", NTriangle_getEmbeddings_list)
            .def("embedding", &NTriangle::embedding,
                return_internal_reference<>())
            .def("getEmbedding", &NTriangle::getEmbedding,
                return_internal_reference<>())
            .def("isBoundary", &NTriangle::isBoundary)
            .def("inMaximalForest", &NTriangle::inMaximalForest)
            .def("getType", &NTriangle::getType)
            .def("getSubtype", &NTriangle::getSubtype)
            .def("isMobiusBand", &NTriangle::isMobiusBand)
            .def("isCone", &NTriangle::isCone)
            .def("isValid", &NTriangle::isValid)
            .def("isLinkOrientable", &NTriangle::isLinkOrientable)
            .def("degree", &NTriangle::degree)
            .def("getDegree", &NTriangle::getDegree)
            .def("front", &NTriangle::front,
                return_internal_reference<>())
            .def("back", &NTriangle::back,
                return_internal_reference<>())
            .def("triangulation", &NTriangle::triangulation,
                return_value_policy<reference_existing_object>())
            .def("getTriangulation", &NTriangle::getTriangulation,
                return_value_policy<reference_existing_object>())
            .def("component", &NTriangle::component,
                return_value_policy<reference_existing_object>())
            .def("getComponent", &NTriangle::getComponent,
                return_value_policy<reference_existing_object>())
            .def("getBoundaryComponent", &NTriangle::getBoundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("vertex", &NTriangle::vertex,
                return_value_policy<reference_existing_object>())
            .def("getVertex", &NTriangle::getVertex,
                return_value_policy<reference_existing_object>())
            .def("edge", &NTriangle::edge,
                return_value_policy<reference_existing_object>())
            .def("getEdge", &NTriangle::getEdge,
                return_value_policy<reference_existing_object>())
            .def("vertexMapping", &NTriangle::vertexMapping)
            .def("getVertexMapping", &NTriangle::getVertexMapping)
            .def("edgeMapping", &NTriangle::edgeMapping)
            .def("getEdgeMapping", &NTriangle::getEdgeMapping)
            .def("str", &NTriangle::str)
            .def("toString", &NTriangle::toString)
            .def("detail", &NTriangle::detail)
            .def("toStringLong", &NTriangle::toStringLong)
            .def("__str__", &NTriangle::str)
            .def("ordering", &NTriangle::ordering)
            .def("faceNumber", &NTriangle::faceNumber)
            .def("containsVertex", &NTriangle::containsVertex)
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

    // Support for deprecated typedefs:
    scope().attr("NFace") = scope().attr("NTriangle");
    scope().attr("NFaceEmbedding") = scope().attr("NTriangleEmbedding");
}

