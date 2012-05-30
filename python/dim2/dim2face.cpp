
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
#include "dim2/dim2component.h"
#include "dim2/dim2edge.h"
#include "dim2/dim2face.h"
#include "dim2/dim2triangulation.h"
#include "dim2/dim2vertex.h"

using namespace boost::python;
using regina::Dim2Face;

void addDim2Face() {
    class_<Dim2Face, bases<regina::ShareableObject>,
            std::auto_ptr<Dim2Face>, boost::noncopyable>(
            "Dim2Face", no_init)
        .def("getDescription", &Dim2Face::getDescription,
            return_value_policy<return_by_value>())
        .def("setDescription", &Dim2Face::setDescription)
        .def("adjacentFace", &Dim2Face::adjacentFace,
            return_value_policy<reference_existing_object>())
        .def("adjacentGluing", &Dim2Face::adjacentGluing)
        .def("adjacentEdge", &Dim2Face::adjacentEdge)
        .def("hasBoundary", &Dim2Face::hasBoundary)
        .def("joinTo", &Dim2Face::joinTo)
        .def("unjoin", &Dim2Face::unjoin,
            return_value_policy<reference_existing_object>())
        .def("isolate", &Dim2Face::isolate)
        .def("getTriangulation", &Dim2Face::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &Dim2Face::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim2Face::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim2Face::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getVertexMapping", &Dim2Face::getVertexMapping)
        .def("getEdgeMapping", &Dim2Face::getEdgeMapping)
        .def("orientation", &Dim2Face::orientation)
    ;
}

