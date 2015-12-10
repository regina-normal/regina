
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
#include "triangulation/ntriangulation.h"
#include "triangulation/nvertex.h"
#include "../globalarray.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NEdge;
using regina::NEdgeEmbedding;
using regina::python::GlobalArray;
using regina::python::GlobalArray2D;

namespace {
    GlobalArray2D<int> edgeNumber_arr(regina::edgeNumber, 4);
    GlobalArray<int> edgeStart_arr(regina::edgeStart, 6);
    GlobalArray<int> edgeEnd_arr(regina::edgeEnd, 6);

    GlobalArray2D<int> NEdge_edgeNumber(NEdge::edgeNumber, 4);
    GlobalArray2D<int> NEdge_edgeVertex(NEdge::edgeVertex, 6);
    GlobalArray<regina::NPerm4> NEdge_ordering(NEdge::ordering, 6);

    boost::python::list edge_getEmbeddings_list(const NEdge* e) {
        boost::python::list ans;
        for (auto& emb: *e)
            ans.append(emb);
        return ans;
    }
}

void addNEdge() {
    // Global arrays:
    scope().attr("edgeNumber") = &edgeNumber_arr;
    scope().attr("edgeStart") = &edgeStart_arr;
    scope().attr("edgeEnd") = &edgeEnd_arr;

    // Classes:
    class_<NEdgeEmbedding>("NEdgeEmbedding",
            init<regina::NTetrahedron*, int>())
        .def(init<const NEdgeEmbedding&>())
        .def("simplex", &NEdgeEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &NEdgeEmbedding::getSimplex,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &NEdgeEmbedding::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("face", &NEdgeEmbedding::face)
        .def("getFace", &NEdgeEmbedding::getFace)
        .def("getEdge", &NEdgeEmbedding::getEdge)
        .def("vertices", &NEdgeEmbedding::vertices)
        .def("getVertices", &NEdgeEmbedding::getVertices)
        .def("str", &NEdgeEmbedding::str)
        .def("toString", &NEdgeEmbedding::toString)
        .def("detail", &NEdgeEmbedding::detail)
        .def("toStringLong", &NEdgeEmbedding::toStringLong)
        .def("__str__", &NEdgeEmbedding::str)
        .def(regina::python::add_eq_operators())
    ;

    scope s = class_<NEdge, std::auto_ptr<NEdge>, boost::noncopyable>
            ("NEdge", no_init)
        .def("index", &NEdge::index)
        .def("getEmbeddings", edge_getEmbeddings_list)
        .def("getEmbedding", &NEdge::getEmbedding,
            return_internal_reference<>())
        .def("front", &NEdge::front,
            return_internal_reference<>())
        .def("back", &NEdge::back,
            return_internal_reference<>())
        .def("triangulation", &NEdge::triangulation,
            return_value_policy<reference_existing_object>())
        .def("getTriangulation", &NEdge::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("component", &NEdge::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &NEdge::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &NEdge::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NEdge::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getDegree", &NEdge::getDegree)
        .def("isBoundary", &NEdge::isBoundary)
        .def("isValid", &NEdge::isValid)
        .def("str", &NEdge::str)
        .def("toString", &NEdge::toString)
        .def("detail", &NEdge::detail)
        .def("toStringLong", &NEdge::toStringLong)
        .def("__str__", &NEdge::str)
        .def(regina::python::add_eq_operators())
    ;

    s.attr("edgeNumber") = &NEdge_edgeNumber;
    s.attr("edgeVertex") = &NEdge_edgeVertex;
    s.attr("ordering") = &NEdge_ordering;
}

