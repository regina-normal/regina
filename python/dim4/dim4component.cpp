
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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
#include "dim4/dim4boundarycomponent.h"
#include "dim4/dim4component.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4triangle.h"
#include "dim4/dim4vertex.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Dim4Component;

void addDim4Component() {
    class_<Dim4Component, std::auto_ptr<Dim4Component>, boost::noncopyable>
            ("Dim4Component", no_init)
        .def("index", &Dim4Component::index)
        .def("getNumberOfPentachora", &Dim4Component::getNumberOfPentachora)
        .def("getNumberOfSimplices", &Dim4Component::getNumberOfSimplices)
        .def("getNumberOfTetrahedra", &Dim4Component::getNumberOfTetrahedra)
        .def("getNumberOfTriangles", &Dim4Component::getNumberOfTriangles)
        .def("getNumberOfEdges", &Dim4Component::getNumberOfEdges)
        .def("getNumberOfVertices", &Dim4Component::getNumberOfVertices)
        .def("getNumberOfBoundaryComponents",
            &Dim4Component::getNumberOfBoundaryComponents)
        .def("getPentachoron", &Dim4Component::getPentachoron,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &Dim4Component::getSimplex,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &Dim4Component::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &Dim4Component::getTriangle,
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
        .def("getNumberOfBoundaryTetrahedra",
            &Dim4Component::getNumberOfBoundaryTetrahedra)
        .def("str", &Dim4Component::str)
        .def("toString", &Dim4Component::toString)
        .def("detail", &Dim4Component::detail)
        .def("toStringLong", &Dim4Component::toStringLong)
        .def("__str__", &Dim4Component::str)
        .def(regina::python::add_eq_operators())
    ;
}

