
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "dim2/dim2boundarycomponent.h"
#include "dim2/dim2component.h"
#include "dim2/dim2triangle.h"
#include "dim2/dim2vertex.h"
#include "../globalarray.h"

using namespace boost::python;
using regina::Dim2Vertex;
using regina::Dim2VertexEmbedding;

namespace {
    boost::python::list Dim2Vertex_getEmbeddings_list(const Dim2Vertex* v) {
        const std::deque<Dim2VertexEmbedding>& embs = v->getEmbeddings();
        std::deque<Dim2VertexEmbedding>::const_iterator it;

        boost::python::list ans;
        for (it = embs.begin(); it != embs.end(); it++)
            ans.append(*it);
        return ans;
    }
}

void addDim2Vertex() {
    class_<Dim2VertexEmbedding>("Dim2VertexEmbedding",
            init<regina::Dim2Triangle*, int>())
        .def(init<const Dim2VertexEmbedding&>())
        .def("getTriangle", &Dim2VertexEmbedding::getTriangle,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim2VertexEmbedding::getVertex)
        .def("getVertices", &Dim2VertexEmbedding::getVertices)
    ;

    scope s = class_<Dim2Vertex, bases<regina::ShareableObject>,
            std::auto_ptr<Dim2Vertex>, boost::noncopyable>
            ("Dim2Vertex", no_init)
        .def("getEmbeddings", Dim2Vertex_getEmbeddings_list)
        .def("getNumberOfEmbeddings", &Dim2Vertex::getNumberOfEmbeddings)
        .def("getEmbedding", &Dim2Vertex::getEmbedding,
            return_internal_reference<>())
        .def("getComponent", &Dim2Vertex::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim2Vertex::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getDegree", &Dim2Vertex::getDegree)
        .def("isBoundary", &Dim2Vertex::isBoundary)
    ;
}

