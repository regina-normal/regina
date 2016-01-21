
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
#include "dim4/dim4edge.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4triangle.h"
#include "dim4/dim4vertex.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using regina::Dim4BoundaryComponent;

void addDim4BoundaryComponent() {
    class_<Dim4BoundaryComponent, std::auto_ptr<Dim4BoundaryComponent>,
            boost::noncopyable>("Dim4BoundaryComponent", no_init)
        .def("index", &Dim4BoundaryComponent::index)
        .def("countFaces",
            &regina::python::countFaces<Dim4BoundaryComponent, 4>)
        .def("getNumberOfFaces",
            &regina::python::countFaces<Dim4BoundaryComponent, 4>)
        .def("countTetrahedra",
            &Dim4BoundaryComponent::countTetrahedra)
        .def("getNumberOfTetrahedra",
            &Dim4BoundaryComponent::getNumberOfTetrahedra)
        .def("countTriangles", &Dim4BoundaryComponent::countTriangles)
        .def("getNumberOfTriangles",
            &Dim4BoundaryComponent::getNumberOfTriangles)
        .def("countEdges", &Dim4BoundaryComponent::countEdges)
        .def("getNumberOfEdges", &Dim4BoundaryComponent::getNumberOfEdges)
        .def("countVertices", &Dim4BoundaryComponent::countVertices)
        .def("getNumberOfVertices", &Dim4BoundaryComponent::getNumberOfVertices)
        .def("face", &regina::python::face<Dim4BoundaryComponent, 4, size_t>)
        .def("getFace", &regina::python::face<Dim4BoundaryComponent, 4, size_t>)
        .def("tetrahedron", &Dim4BoundaryComponent::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &Dim4BoundaryComponent::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("triangle", &Dim4BoundaryComponent::triangle,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &Dim4BoundaryComponent::getTriangle,
            return_value_policy<reference_existing_object>())
        .def("edge", &Dim4BoundaryComponent::edge,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4BoundaryComponent::getEdge,
            return_value_policy<reference_existing_object>())
        .def("vertex", &Dim4BoundaryComponent::vertex,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4BoundaryComponent::getVertex,
            return_value_policy<reference_existing_object>())
        .def("component", &Dim4BoundaryComponent::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &Dim4BoundaryComponent::getComponent,
            return_value_policy<reference_existing_object>())
        .def("isIdeal", &Dim4BoundaryComponent::isIdeal)
        .def("isInvalidVertex", &Dim4BoundaryComponent::isInvalidVertex)
        .def("str", &Dim4BoundaryComponent::str)
        .def("toString", &Dim4BoundaryComponent::toString)
        .def("detail", &Dim4BoundaryComponent::detail)
        .def("toStringLong", &Dim4BoundaryComponent::toStringLong)
        .def("__str__", &Dim4BoundaryComponent::str)
        .def(regina::python::add_eq_operators())
    ;
}

