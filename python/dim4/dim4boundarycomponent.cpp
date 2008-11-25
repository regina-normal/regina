
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
#include "dim4/dim4edge.h"
#include "dim4/dim4face.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4vertex.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4BoundaryComponent;

void addDim4BoundaryComponent() {
    class_<Dim4BoundaryComponent, bases<regina::ShareableObject>,
            std::auto_ptr<Dim4BoundaryComponent>, boost::noncopyable>
            ("Dim4BoundaryComponent", no_init)
        .def("getNumberOfTetrahedra",
            &Dim4BoundaryComponent::getNumberOfTetrahedra)
        .def("getNumberOfFaces", &Dim4BoundaryComponent::getNumberOfFaces)
        .def("getNumberOfEdges", &Dim4BoundaryComponent::getNumberOfEdges)
        .def("getNumberOfVertices", &Dim4BoundaryComponent::getNumberOfVertices)
        .def("getTetrahedron", &Dim4BoundaryComponent::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getFace", &Dim4BoundaryComponent::getFace,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4BoundaryComponent::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4BoundaryComponent::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getTriangulation", &Dim4BoundaryComponent::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("isIdeal", &Dim4BoundaryComponent::isIdeal)
        .def("isInvalidVertex", &Dim4BoundaryComponent::isInvalidVertex)
    ;
}

