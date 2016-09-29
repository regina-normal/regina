
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
#include "dim2/dim2component.h"
#include "dim2/dim2edge.h"
#include "dim2/dim2triangle.h"
#include "dim2/dim2vertex.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using regina::Component;
using regina::Dim2Component;

namespace {
    boost::python::list triangles_list(Dim2Component& t) {
        boost::python::list ans;
        for (auto s : t.triangles())
            ans.append(boost::python::ptr(s));
        return ans;
    }
}

void addDim2Component() {
    class_<Component<2>, std::auto_ptr<Component<2>>, boost::noncopyable>
            ("Component2", no_init)
        .def("index", &Dim2Component::index)
        .def("size", &Dim2Component::size)
        .def("countTriangles", &Dim2Component::countTriangles)
        .def("getNumberOfTriangles", &Dim2Component::countTriangles)
        .def("getNumberOfSimplices", &Dim2Component::size)
        .def("countFaces", &regina::python::countFaces<Dim2Component, 2>)
        .def("getNumberOfFaces", &regina::python::countFaces<Dim2Component, 2>)
        .def("countEdges", &Dim2Component::countEdges)
        .def("getNumberOfEdges", &Dim2Component::countEdges)
        .def("countVertices", &Dim2Component::countVertices)
        .def("getNumberOfVertices", &Dim2Component::countVertices)
        .def("countBoundaryComponents", &Dim2Component::countBoundaryComponents)
        .def("getNumberOfBoundaryComponents",
            &Dim2Component::countBoundaryComponents)
        .def("simplices", triangles_list)
        .def("triangles", triangles_list)
        .def("getTriangles", triangles_list)
        .def("faces", &regina::python::faces<Dim2Component, 2>)
        .def("edges", regina::python::faces_list<Dim2Component, 2, 1>)
        .def("getEdges", regina::python::faces_list<Dim2Component, 2, 1>)
        .def("vertices", regina::python::faces_list<Dim2Component, 2, 0>)
        .def("getVertices", regina::python::faces_list<Dim2Component, 2, 0>)
        .def("triangle", &Dim2Component::triangle,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &Dim2Component::triangle,
            return_value_policy<reference_existing_object>())
        .def("simplex", &Dim2Component::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &Dim2Component::simplex,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<Dim2Component, 2, size_t>)
        .def("edge", &Dim2Component::edge,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim2Component::edge,
            return_value_policy<reference_existing_object>())
        .def("vertex", &Dim2Component::vertex,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim2Component::vertex,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Dim2Component::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim2Component::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isValid", &Dim2Component::isValid)
        .def("isOrientable", &Dim2Component::isOrientable)
        .def("isClosed", &Dim2Component::isClosed)
        .def("hasBoundaryFacets", &Dim2Component::hasBoundaryFacets)
        .def("hasBoundaryEdges", &Dim2Component::hasBoundaryEdges)
        .def("countBoundaryFacets", &Dim2Component::countBoundaryFacets)
        .def("getNumberOfBoundaryFacets", &Dim2Component::countBoundaryFacets)
        .def("countBoundaryEdges", &Dim2Component::countBoundaryEdges)
        .def("getNumberOfBoundaryEdges", &Dim2Component::countBoundaryEdges)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("Dim2Component") = scope().attr("Component2");
}

