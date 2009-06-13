
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4vertex.h"
#include "triangulation/ntriangulation.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4Vertex;
using regina::Dim4VertexEmbedding;

namespace {
    boost::python::list Dim4Vertex_getEmbeddings_list(const Dim4Vertex* v) {
        const std::vector<Dim4VertexEmbedding>& embs = v->getEmbeddings();
        std::vector<Dim4VertexEmbedding>::const_iterator it;

        boost::python::list ans;
        for (it = embs.begin(); it != embs.end(); it++)
            ans.append(*it);
        return ans;
    }
}

void addDim4Vertex() {
    class_<Dim4VertexEmbedding>("Dim4VertexEmbedding",
            init<regina::Dim4Pentachoron*, int>())
        .def(init<const Dim4VertexEmbedding&>())
        .def("getPentachoron", &Dim4VertexEmbedding::getPentachoron,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4VertexEmbedding::getVertex)
        .def("getVertices", &Dim4VertexEmbedding::getVertices)
    ;

    scope s = class_<Dim4Vertex, bases<regina::ShareableObject>,
            std::auto_ptr<Dim4Vertex>, boost::noncopyable>
            ("Dim4Vertex", no_init)
        .def("getEmbeddings", Dim4Vertex_getEmbeddings_list)
        .def("getNumberOfEmbeddings", &Dim4Vertex::getNumberOfEmbeddings)
        .def("getEmbedding", &Dim4Vertex::getEmbedding,
            return_internal_reference<>())
        .def("getComponent", &Dim4Vertex::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Vertex::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getDegree", &Dim4Vertex::getDegree)
        .def("getLink", &Dim4Vertex::getLink,
            return_value_policy<reference_existing_object>())
        .def("isValid", &Dim4Vertex::isValid)
        .def("isIdeal", &Dim4Vertex::isIdeal)
        .def("isBoundary", &Dim4Vertex::isBoundary)
    ;
}

