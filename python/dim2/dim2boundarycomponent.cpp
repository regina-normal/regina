
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "dim2/dim2boundarycomponent.h"
#include "dim2/dim2edge.h"
#include "dim2/dim2vertex.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using regina::Dim2BoundaryComponent;

void addDim2BoundaryComponent() {
    class_<Dim2BoundaryComponent, std::auto_ptr<Dim2BoundaryComponent>,
            boost::noncopyable> ("Dim2BoundaryComponent", no_init)
        .def("index", &Dim2BoundaryComponent::index)
        .def("countFaces",
            &regina::python::countFaces<Dim2BoundaryComponent, 2>)
        .def("countEdges", &Dim2BoundaryComponent::countEdges)
        .def("getNumberOfEdges", &Dim2BoundaryComponent::countEdges)
        .def("countVertices", &Dim2BoundaryComponent::countVertices)
        .def("getNumberOfVertices", &Dim2BoundaryComponent::countVertices)
        .def("face", &regina::python::face<Dim2BoundaryComponent, 2, size_t>)
        .def("edge", &Dim2BoundaryComponent::edge,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim2BoundaryComponent::edge,
            return_value_policy<reference_existing_object>())
        .def("vertex", &Dim2BoundaryComponent::vertex,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim2BoundaryComponent::vertex,
            return_value_policy<reference_existing_object>())
        .def("component", &Dim2BoundaryComponent::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &Dim2BoundaryComponent::component,
            return_value_policy<reference_existing_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}

