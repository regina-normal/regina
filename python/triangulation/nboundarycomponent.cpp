
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "triangulation/nboundarycomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/nface.h"
#include "triangulation/nvertex.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NBoundaryComponent;

void addNBoundaryComponent() {
    class_<NBoundaryComponent, bases<regina::ShareableObject>,
            std::auto_ptr<NBoundaryComponent>, boost::noncopyable>
            ("NBoundaryComponent", no_init)
        .def("getNumberOfFaces", &NBoundaryComponent::getNumberOfFaces)
        .def("getNumberOfEdges", &NBoundaryComponent::getNumberOfEdges)
        .def("getNumberOfVertices", &NBoundaryComponent::getNumberOfVertices)
        .def("getFace", &NBoundaryComponent::getFace,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &NBoundaryComponent::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NBoundaryComponent::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEulerCharacteristic",
            &NBoundaryComponent::getEulerCharacteristic)
        .def("isIdeal", &NBoundaryComponent::isIdeal)
        .def("isOrientable", &NBoundaryComponent::isOrientable)
    ;
}

