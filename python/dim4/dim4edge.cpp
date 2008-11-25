
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "dim4/dim4boundarycomponent.h"
#include "dim4/dim4component.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4vertex.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4Edge;
using regina::Dim4EdgeEmbedding;
using regina::python::GlobalArray;
using regina::python::GlobalArray2D;

namespace {
    GlobalArray2D<int> Dim4Edge_edgeNumber(Dim4Edge::edgeNumber, 5);
    GlobalArray2D<int> Dim4Edge_edgeVertex(Dim4Edge::edgeVertex, 10);
    GlobalArray<regina::NPerm5> Dim4Edge_ordering(Dim4Edge::ordering, 10);

    boost::python::list Dim4Edge_getEmbeddings_list(const Dim4Edge* e) {
        const std::vector<Dim4EdgeEmbedding>& embs = e->getEmbeddings();
        std::vector<Dim4EdgeEmbedding>::const_iterator it;

        boost::python::list ans;
        for (it = embs.begin(); it != embs.end(); ++it)
            ans.append(*it);
        return ans;
    }
}

void addDim4Edge() {
    class_<Dim4EdgeEmbedding>("Dim4EdgeEmbedding",
            init<regina::Dim4Pentachoron*, int>())
        .def(init<const Dim4EdgeEmbedding&>())
        .def("getPentachoron", &Dim4EdgeEmbedding::getPentachoron,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4EdgeEmbedding::getEdge)
        .def("getVertices", &Dim4EdgeEmbedding::getVertices)
    ;

    scope s = class_<Dim4Edge, bases<regina::ShareableObject>,
            std::auto_ptr<Dim4Edge>, boost::noncopyable>("Dim4Edge", no_init)
        .def("getEmbeddings", Dim4Edge_getEmbeddings_list)
        .def("getNumberOfEmbeddings", &Dim4Edge::getNumberOfEmbeddings)
        .def("getEmbedding", &Dim4Edge::getEmbedding,
            return_internal_reference<>())
        .def("getComponent", &Dim4Edge::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Edge::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4Edge::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getDegree", &Dim4Edge::getDegree)
        .def("isBoundary", &Dim4Edge::isBoundary)
        .def("isValid", &Dim4Edge::isValid)
        .def("hasBadIdentification", &Dim4Edge::hasBadIdentification)
        .def("hasBadLink", &Dim4Edge::hasBadLink)
    ;

    s.attr("edgeNumber") = &Dim4Edge_edgeNumber;
    s.attr("edgeVertex") = &Dim4Edge_edgeVertex;
    s.attr("ordering") = &Dim4Edge_ordering;
}

