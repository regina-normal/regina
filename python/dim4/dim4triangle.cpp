
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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
#include "dim4/dim4boundarycomponent.h"
#include "dim4/dim4component.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4triangle.h"
#include "dim4/dim4triangulation.h"
#include "dim4/dim4vertex.h"
#include "../globalarray.h"

using namespace boost::python;
using regina::Dim4Triangle;
using regina::Dim4TriangleEmbedding;
using regina::python::GlobalArray;
using regina::python::GlobalArray2D;
using regina::python::GlobalArray3D;

namespace {
    GlobalArray3D<int> Dim4Triangle_triangleNumber(
        Dim4Triangle::triangleNumber, 5);
    GlobalArray2D<int> Dim4Triangle_triangleVertex(
        Dim4Triangle::triangleVertex, 10);
    GlobalArray<regina::NPerm5> Dim4Triangle_ordering(Dim4Triangle::ordering, 10);

    boost::python::list Dim4Triangle_getEmbeddings_list(const Dim4Triangle* f) {
        const std::deque<Dim4TriangleEmbedding>& embs = f->getEmbeddings();
        std::deque<Dim4TriangleEmbedding>::const_iterator it;

        boost::python::list ans;
        for (it = embs.begin(); it != embs.end(); ++it)
            ans.append(*it);
        return ans;
    }
}

void addDim4Triangle() {
    class_<Dim4TriangleEmbedding, boost::noncopyable>("Dim4TriangleEmbedding",
            init<regina::Dim4Pentachoron*, int>())
        .def(init<const Dim4TriangleEmbedding&>())
        .def("getPentachoron", &Dim4TriangleEmbedding::getPentachoron,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &Dim4TriangleEmbedding::getTriangle)
        .def("getVertices", &Dim4TriangleEmbedding::getVertices)
        .def(self == self)
        .def(self != self)
    ;

    scope s = class_<Dim4Triangle, std::auto_ptr<Dim4Triangle>,
            boost::noncopyable>("Dim4Triangle", no_init)
        .def("index", &Dim4Triangle::index)
        .def("getEmbeddings", Dim4Triangle_getEmbeddings_list)
        .def("getNumberOfEmbeddings", &Dim4Triangle::getNumberOfEmbeddings)
        .def("getEmbedding", &Dim4Triangle::getEmbedding,
            return_internal_reference<>())
        .def("getTriangulation", &Dim4Triangle::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &Dim4Triangle::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Triangle::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4Triangle::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4Triangle::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getEdgeMapping", &Dim4Triangle::getEdgeMapping)
        .def("getDegree", &Dim4Triangle::getDegree)
        .def("isBoundary", &Dim4Triangle::isBoundary)
        .def("isValid", &Dim4Triangle::isValid)
        .def("str", &Dim4Triangle::str)
        .def("toString", &Dim4Triangle::toString)
        .def("detail", &Dim4Triangle::detail)
        .def("toStringLong", &Dim4Triangle::toStringLong)
        .def("__str__", &Dim4Triangle::str)
    ;

    s.attr("triangleNumber") = &Dim4Triangle_triangleNumber;
    s.attr("triangleVertex") = &Dim4Triangle_triangleVertex;
    s.attr("ordering") = &Dim4Triangle_ordering;
}

