
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
#include "../utilities/equality.h"

using namespace boost::python;
using regina::NTriangle;
using regina::NTriangleEmbedding;
using regina::python::GlobalArray;

namespace {
    GlobalArray<regina::NPerm4> NTriangle_ordering(NTriangle::ordering, 4);
}

void addNTriangle() {
    class_<NTriangleEmbedding, boost::noncopyable>("NTriangleEmbedding",
            init<regina::NTetrahedron*, int>())
        .def(init<const NTriangleEmbedding&>())
        .def("getTetrahedron", &NTriangleEmbedding::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getFace", &NTriangleEmbedding::getFace)
        .def("getTriangle", &NTriangleEmbedding::getTriangle)
        .def("getVertices", &NTriangleEmbedding::getVertices)
        .def(self == self)
        .def(self != self)
    ;

    {
        scope s = class_<NTriangle, std::auto_ptr<NTriangle>,
                boost::noncopyable>("NTriangle", no_init)
            .def("index", &NTriangle::index)
            .def("isBoundary", &NTriangle::isBoundary)
            .def("getType", &NTriangle::getType)
            .def("getSubtype", &NTriangle::getSubtype)
            .def("isMobiusBand", &NTriangle::isMobiusBand)
            .def("isCone", &NTriangle::isCone)
            .def("getNumberOfEmbeddings", &NTriangle::getNumberOfEmbeddings)
            .def("getEmbedding", &NTriangle::getEmbedding,
                return_internal_reference<>())
            .def("getTriangulation", &NTriangle::getTriangulation,
                return_value_policy<reference_existing_object>())
            .def("getComponent", &NTriangle::getComponent,
                return_value_policy<reference_existing_object>())
            .def("getBoundaryComponent", &NTriangle::getBoundaryComponent,
                return_value_policy<reference_existing_object>())
            .def("getVertex", &NTriangle::getVertex,
                return_value_policy<reference_existing_object>())
            .def("getEdge", &NTriangle::getEdge,
                return_value_policy<reference_existing_object>())
            .def("getEdgeMapping", &NTriangle::getEdgeMapping)
            .def("str", &NTriangle::str)
            .def("toString", &NTriangle::toString)
            .def("detail", &NTriangle::detail)
            .def("toStringLong", &NTriangle::toStringLong)
            .def("__str__", &NTriangle::str)
            EQUAL_BY_PTR(NTriangle)
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

        s.attr("ordering") = &NTriangle_ordering;
    }

    // Support for deprecated typedefs:
    scope().attr("NFace") = scope().attr("NTriangle");
    scope().attr("NFaceEmbedding") = scope().attr("NTriangleEmbedding");
}

