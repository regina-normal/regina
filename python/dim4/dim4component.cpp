
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
#include "dim4/dim4boundarycomponent.h"
#include "dim4/dim4component.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4triangle.h"
#include "dim4/dim4vertex.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using regina::Component;
using regina::Dim4Component;

namespace {
    boost::python::list getSimplices_list(Dim4Component& t) {
        boost::python::list ans;
        for (std::vector<regina::Simplex<4>*>::const_iterator it =
                t.simplices().begin(); it != t.simplices().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }
}

void addDim4Component() {
    class_<Component<4>, std::auto_ptr<Component<4>>, boost::noncopyable>
            ("Component4", no_init)
        .def("index", &Dim4Component::index)
        .def("size", &Dim4Component::size)
        .def("countPentachora", &Dim4Component::countPentachora)
        .def("getNumberOfPentachora", &Dim4Component::countPentachora)
        .def("getNumberOfSimplices", &Dim4Component::size)
        .def("countFaces", &regina::python::countFaces<Dim4Component, 4>)
        .def("getNumberOfFaces", &regina::python::countFaces<Dim4Component, 4>)
        .def("countTetrahedra", &Dim4Component::countTetrahedra)
        .def("getNumberOfTetrahedra", &Dim4Component::countTetrahedra)
        .def("countTriangles", &Dim4Component::countTriangles)
        .def("getNumberOfTriangles", &Dim4Component::countTriangles)
        .def("countEdges", &Dim4Component::countEdges)
        .def("getNumberOfEdges", &Dim4Component::countEdges)
        .def("countVertices", &Dim4Component::countVertices)
        .def("getNumberOfVertices", &Dim4Component::countVertices)
        .def("countBoundaryComponents", &Dim4Component::countBoundaryComponents)
        .def("getNumberOfBoundaryComponents",
            &Dim4Component::countBoundaryComponents)
        .def("simplices", getSimplices_list)
        .def("getSimplices", getSimplices_list)
        .def("pentachora", getSimplices_list)
        .def("getPentachora", getSimplices_list)
        .def("faces", &regina::python::faces<Dim4Component, 4>)
        .def("vertices", regina::python::faces_list<Dim4Component, 4, 0>)
        .def("getVertices", regina::python::faces_list<Dim4Component, 4, 0>)
        .def("edges", regina::python::faces_list<Dim4Component, 4, 1>)
        .def("getEdges", regina::python::faces_list<Dim4Component, 4, 1>)
        .def("triangles", regina::python::faces_list<Dim4Component, 4, 2>)
        .def("getTriangles", regina::python::faces_list<Dim4Component, 4, 2>)
        .def("tetrahedra", regina::python::faces_list<Dim4Component, 4, 3>)
        .def("getTetrahedra", regina::python::faces_list<Dim4Component, 4, 3>)
        .def("simplex", &Dim4Component::simplex,
            return_value_policy<reference_existing_object>())
        .def("pentachoron", &Dim4Component::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("getPentachoron", &Dim4Component::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &Dim4Component::simplex,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<Dim4Component, 4, size_t>)
        .def("tetrahedron", &Dim4Component::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &Dim4Component::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("triangle", &Dim4Component::triangle,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &Dim4Component::triangle,
            return_value_policy<reference_existing_object>())
        .def("edge", &Dim4Component::edge,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4Component::edge,
            return_value_policy<reference_existing_object>())
        .def("vertex", &Dim4Component::vertex,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4Component::vertex,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Dim4Component::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Component::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isIdeal", &Dim4Component::isIdeal)
        .def("isValid", &Dim4Component::isValid)
        .def("isOrientable", &Dim4Component::isOrientable)
        .def("isClosed", &Dim4Component::isClosed)
        .def("hasBoundaryFacets", &Dim4Component::hasBoundaryFacets)
        .def("hasBoundaryTetrahedra", &Dim4Component::hasBoundaryTetrahedra)
        .def("countBoundaryFacets", &Dim4Component::countBoundaryFacets)
        .def("getNumberOfBoundaryFacets", &Dim4Component::countBoundaryFacets)
        .def("countBoundaryTetrahedra", &Dim4Component::countBoundaryTetrahedra)
        .def("getNumberOfBoundaryTetrahedra",
            &Dim4Component::countBoundaryTetrahedra)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("Dim4Component") = scope().attr("Component4");
}

