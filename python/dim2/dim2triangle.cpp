
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

#include "dim2/dim2component.h"
#include "dim2/dim2edge.h"
#include "dim2/dim2triangle.h"
#include "dim2/dim2triangulation.h"
#include "dim2/dim2vertex.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Dim2Triangle;

void addDim2Triangle() {
    class_<regina::Simplex<2>, std::auto_ptr<regina::Simplex<2>>,
            boost::noncopyable>("Simplex2", no_init)
        .def("description", &Dim2Triangle::description,
            return_value_policy<return_by_value>())
        .def("getDescription", &Dim2Triangle::description,
            return_value_policy<return_by_value>())
        .def("setDescription", &Dim2Triangle::setDescription)
        .def("index", &Dim2Triangle::index)
        .def("adjacentTriangle", &Dim2Triangle::adjacentTriangle,
            return_value_policy<reference_existing_object>())
        .def("adjacentSimplex", &Dim2Triangle::adjacentSimplex,
            return_value_policy<reference_existing_object>())
        .def("adjacentGluing", &Dim2Triangle::adjacentGluing)
        .def("adjacentEdge", &Dim2Triangle::adjacentEdge)
        .def("adjacentFacet", &Dim2Triangle::adjacentFacet)
        .def("hasBoundary", &Dim2Triangle::hasBoundary)
        .def("joinTo", &Dim2Triangle::join)
        .def("join", &Dim2Triangle::join)
        .def("unjoin", &Dim2Triangle::unjoin,
            return_value_policy<reference_existing_object>())
        .def("isolate", &Dim2Triangle::isolate)
        .def("triangulation", &Dim2Triangle::triangulation,
            return_value_policy<to_held_type<> >())
        .def("getTriangulation", &Dim2Triangle::triangulation,
            return_value_policy<to_held_type<> >())
        .def("component", &Dim2Triangle::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &Dim2Triangle::component,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<Dim2Triangle, 2, int>)
        .def("vertex", &Dim2Triangle::vertex,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim2Triangle::vertex,
            return_value_policy<reference_existing_object>())
        .def("edge", &Dim2Triangle::edge,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim2Triangle::edge,
            return_value_policy<reference_existing_object>())
        .def("faceMapping", &regina::python::faceMapping<Dim2Triangle, 2>)
        .def("vertexMapping", &Dim2Triangle::vertexMapping)
        .def("getVertexMapping", &Dim2Triangle::vertexMapping)
        .def("edgeMapping", &Dim2Triangle::edgeMapping)
        .def("getEdgeMapping", &Dim2Triangle::edgeMapping)
        .def("orientation", &Dim2Triangle::orientation)
        .def("facetInMaximalForest", &Dim2Triangle::facetInMaximalForest)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("Dim2Triangle") = scope().attr("Simplex2");
}

