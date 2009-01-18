
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
#include "dim4/dim4edge.h"
#include "dim4/dim4face.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4vertex.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4Component;

void addDim4Component() {
    class_<Dim4Component, bases<regina::ShareableObject>,
            std::auto_ptr<Dim4Component>, boost::noncopyable>
            ("Dim4Component", no_init)
        .def("getNumberOfPentachora", &Dim4Component::getNumberOfPentachora)
        .def("getNumberOfTetrahedra", &Dim4Component::getNumberOfTetrahedra)
        .def("getNumberOfFaces", &Dim4Component::getNumberOfFaces)
        .def("getNumberOfEdges", &Dim4Component::getNumberOfEdges)
        .def("getNumberOfVertices", &Dim4Component::getNumberOfVertices)
        .def("getNumberOfBoundaryComponents",
            &Dim4Component::getNumberOfBoundaryComponents)
        .def("getPentachoron", &Dim4Component::getPentachoron,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &Dim4Component::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getFace", &Dim4Component::getFace,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4Component::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4Component::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Component::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isIdeal", &Dim4Component::isIdeal)
        .def("isOrientable", &Dim4Component::isOrientable)
        .def("isClosed", &Dim4Component::isClosed)
    ;
}

