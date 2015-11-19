
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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
#include "triangulation/nboundarycomponent.h"
#include "triangulation/ncomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangle.h"
#include "triangulation/nvertex.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NComponent;

void addNComponent() {
    class_<NComponent, std::auto_ptr<NComponent>, boost::noncopyable>
            ("NComponent", no_init)
        .def("index", &NComponent::index)
        .def("getNumberOfTetrahedra", &NComponent::getNumberOfTetrahedra)
        .def("getNumberOfSimplices", &NComponent::getNumberOfSimplices)
        .def("getNumberOfFaces", &NComponent::getNumberOfFaces)
        .def("getNumberOfTriangles", &NComponent::getNumberOfTriangles)
        .def("getNumberOfEdges", &NComponent::getNumberOfEdges)
        .def("getNumberOfVertices", &NComponent::getNumberOfVertices)
        .def("getNumberOfBoundaryComponents",
            &NComponent::getNumberOfBoundaryComponents)
        .def("getTetrahedron", &NComponent::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &NComponent::getSimplex,
            return_value_policy<reference_existing_object>())
        .def("getFace", &NComponent::getFace,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &NComponent::getTriangle,
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
        .def("getNumberOfBoundaryTriangles",
            &NComponent::getNumberOfBoundaryTriangles)
        .def("str", &NComponent::str)
        .def("toString", &NComponent::toString)
        .def("detail", &NComponent::detail)
        .def("toStringLong", &NComponent::toStringLong)
        .def("__str__", &NComponent::str)
        .def(regina::python::add_eq_operators())
    ;
}

