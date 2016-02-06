
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
#include "triangulation/ncomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangle.h"
#include "triangulation/nvertex.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using regina::Component;
using regina::NComponent;

namespace {
    boost::python::list simplices_list(NComponent& t) {
        boost::python::list ans;
        for (std::vector<regina::Simplex<3>*>::const_iterator it =
                t.simplices().begin(); it != t.simplices().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }
}

void addNComponent() {
    class_<Component<3>, std::auto_ptr<Component<3>>, boost::noncopyable>
            ("Component3", no_init)
        .def("index", &NComponent::index)
        .def("size", &NComponent::size)
        .def("countTetrahedra", &NComponent::countTetrahedra)
        .def("getNumberOfTetrahedra", &NComponent::countTetrahedra)
        .def("getNumberOfSimplices", &NComponent::size)
        .def("countFaces", &regina::python::countFaces<NComponent, 3>)
        .def("getNumberOfFaces", &regina::python::countFaces<NComponent, 3>)
        .def("countTriangles", &NComponent::countTriangles)
        .def("getNumberOfTriangles", &NComponent::countTriangles)
        .def("countEdges", &NComponent::countEdges)
        .def("getNumberOfEdges", &NComponent::countEdges)
        .def("countVertices", &NComponent::countVertices)
        .def("getNumberOfVertices", &NComponent::countVertices)
        .def("countBoundaryComponents", &NComponent::countBoundaryComponents)
        .def("getNumberOfBoundaryComponents",
            &NComponent::countBoundaryComponents)
        .def("simplices", simplices_list)
        .def("getSimplices", simplices_list)
        .def("tetrahedra", simplices_list)
        .def("getTetrahedra", simplices_list)
        .def("simplex", &NComponent::simplex,
            return_value_policy<reference_existing_object>())
        .def("tetrahedron", &NComponent::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &NComponent::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &NComponent::simplex,
            return_value_policy<reference_existing_object>())
        .def("faces", &regina::python::faces<NComponent, 3>)
        .def("triangles", regina::python::faces_list<NComponent, 3, 2>)
        .def("getTriangles", regina::python::faces_list<NComponent, 3, 2>)
        .def("edges", regina::python::faces_list<NComponent, 3, 1>)
        .def("getEdges", regina::python::faces_list<NComponent, 3, 1>)
        .def("vertices", regina::python::faces_list<NComponent, 3, 0>)
        .def("getVertices", regina::python::faces_list<NComponent, 3, 0>)
        .def("face", &regina::python::face<NComponent, 3, size_t>)
        .def("triangle", &NComponent::triangle,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &NComponent::triangle,
            return_value_policy<reference_existing_object>())
        .def("edge", &NComponent::edge,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &NComponent::edge,
            return_value_policy<reference_existing_object>())
        .def("vertex", &NComponent::vertex,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NComponent::vertex,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &NComponent::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &NComponent::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isIdeal", &NComponent::isIdeal)
        .def("isValid", &NComponent::isValid)
        .def("isOrientable", &NComponent::isOrientable)
        .def("isClosed", &NComponent::isClosed)
        .def("hasBoundaryFacets", &NComponent::hasBoundaryFacets)
        .def("hasBoundaryTriangles", &NComponent::hasBoundaryTriangles)
        .def("countBoundaryFacets", &NComponent::countBoundaryFacets)
        .def("getNumberOfBoundaryFacets",
            &NComponent::countBoundaryFacets)
        .def("countBoundaryTriangles", &NComponent::countBoundaryTriangles)
        .def("getNumberOfBoundaryTriangles",
            &NComponent::countBoundaryTriangles)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NComponent") = scope().attr("Component3");
}

