
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
#include "triangulation/nboundarycomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/ntriangle.h"
#include "triangulation/nvertex.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using regina::NBoundaryComponent;

void addNBoundaryComponent() {
    class_<NBoundaryComponent, std::auto_ptr<NBoundaryComponent>,
            boost::noncopyable> ("NBoundaryComponent", no_init)
        .def("index", &NBoundaryComponent::index)
        .def("countFaces", &regina::python::countFaces<NBoundaryComponent, 3>)
        .def("countTriangles", &NBoundaryComponent::countTriangles)
        .def("getNumberOfTriangles", &NBoundaryComponent::countTriangles)
        .def("countEdges", &NBoundaryComponent::countEdges)
        .def("getNumberOfEdges", &NBoundaryComponent::countEdges)
        .def("countVertices", &NBoundaryComponent::countVertices)
        .def("getNumberOfVertices", &NBoundaryComponent::countVertices)
        .def("face", &regina::python::face<NBoundaryComponent, 3, size_t>)
        .def("triangle", &NBoundaryComponent::triangle,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &NBoundaryComponent::triangle,
            return_value_policy<reference_existing_object>())
        .def("edge", &NBoundaryComponent::edge,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &NBoundaryComponent::edge,
            return_value_policy<reference_existing_object>())
        .def("vertex", &NBoundaryComponent::vertex,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NBoundaryComponent::vertex,
            return_value_policy<reference_existing_object>())
        .def("component", &NBoundaryComponent::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &NBoundaryComponent::component,
            return_value_policy<reference_existing_object>())
        .def("eulerChar", &NBoundaryComponent::eulerChar)
        .def("getEulerChar", &NBoundaryComponent::eulerChar)
        .def("getEulerCharacteristic", &NBoundaryComponent::eulerChar)
        .def("isIdeal", &NBoundaryComponent::isIdeal)
        .def("isOrientable", &NBoundaryComponent::isOrientable)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}

