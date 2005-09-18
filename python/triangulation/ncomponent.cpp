
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
#include "triangulation/nface.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/nvertex.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NComponent;

void addNComponent() {
    class_<NComponent, bases<regina::ShareableObject>,
            std::auto_ptr<NComponent>, boost::noncopyable>
            ("NComponent", no_init)
        .def("getNumberOfTetrahedra", &NComponent::getNumberOfTetrahedra)
        .def("getNumberOfFaces", &NComponent::getNumberOfFaces)
        .def("getNumberOfEdges", &NComponent::getNumberOfEdges)
        .def("getNumberOfVertices", &NComponent::getNumberOfVertices)
        .def("getNumberOfBoundaryComponents",
            &NComponent::getNumberOfBoundaryComponents)
        .def("getTetrahedron", &NComponent::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getFace", &NComponent::getFace,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &NComponent::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NComponent::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &NComponent::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isIdeal", &NComponent::isIdeal)
        .def("isOrientable", &NComponent::isOrientable)
        .def("isClosed", &NComponent::isClosed)
    ;
}

