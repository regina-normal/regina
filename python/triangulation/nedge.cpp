
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include "triangulation/nboundarycomponent.h"
#include "triangulation/ncomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/nvertex.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NEdge;
using regina::NEdgeEmbedding;
using regina::python::GlobalArray;
using regina::python::GlobalArray2D;

namespace {
    GlobalArray2D<int> edgeNumber_arr(regina::edgeNumber, 4);
    GlobalArray<int> edgeStart_arr(regina::edgeStart, 6);
    GlobalArray<int> edgeEnd_arr(regina::edgeEnd, 6);

    boost::python::list edge_getEmbeddings_list(const NEdge* e) {
        const std::deque<NEdgeEmbedding>& embs = e->getEmbeddings();
        std::deque<NEdgeEmbedding>::const_iterator it;

        boost::python::list ans;
        for (it = embs.begin(); it != embs.end(); it++)
            ans.append(*it);
        return ans;
    }
}

void addNEdge() {
    class_<NEdgeEmbedding>("NEdgeEmbedding",
            init<regina::NTetrahedron*, int>())
        .def(init<const NEdgeEmbedding&>())
        .def("getTetrahedron", &NEdgeEmbedding::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &NEdgeEmbedding::getEdge)
        .def("getVertices", &NEdgeEmbedding::getVertices)
    ;

    class_<NEdge, bases<regina::ShareableObject>,
            std::auto_ptr<NEdge>, boost::noncopyable>("NEdge", no_init)
        .def("getEmbeddings", edge_getEmbeddings_list)
        .def("getNumberOfEmbeddings", &NEdge::getNumberOfEmbeddings)
        .def("getEmbedding", &NEdge::getEmbedding,
            return_internal_reference<>())
        .def("getComponent", &NEdge::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &NEdge::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NEdge::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getDegree", &NEdge::getDegree)
        .def("isBoundary", &NEdge::isBoundary)
        .def("isValid", &NEdge::isValid)
    ;

    // Global arrays:
    scope().attr("edgeNumber") = &edgeNumber_arr;
    scope().attr("edgeStart") = &edgeStart_arr;
    scope().attr("edgeEnd") = &edgeEnd_arr;
}

