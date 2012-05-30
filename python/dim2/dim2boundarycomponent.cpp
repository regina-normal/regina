
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
#include "dim2/dim2edge.h"
#include "dim2/dim2vertex.h"

using namespace boost::python;
using regina::Dim2BoundaryComponent;

void addDim2BoundaryComponent() {
    class_<Dim2BoundaryComponent, bases<regina::ShareableObject>,
            std::auto_ptr<Dim2BoundaryComponent>, boost::noncopyable>
            ("Dim2BoundaryComponent", no_init)
        .def("getNumberOfEdges", &Dim2BoundaryComponent::getNumberOfEdges)
        .def("getNumberOfVertices", &Dim2BoundaryComponent::getNumberOfVertices)
        .def("getEdge", &Dim2BoundaryComponent::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim2BoundaryComponent::getVertex,
            return_value_policy<reference_existing_object>())
    ;
}

