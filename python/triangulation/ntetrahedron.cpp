
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
#include "triangulation/ncomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangle.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nvertex.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using namespace regina::python;
using regina::NTetrahedron;

void addNTetrahedron() {
    class_<regina::Simplex<3>, std::auto_ptr<regina::Simplex<3>>,
            boost::noncopyable>("Simplex3", no_init)
        .def("description", &NTetrahedron::description,
            return_value_policy<return_by_value>())
        .def("getDescription", &NTetrahedron::description,
            return_value_policy<return_by_value>())
        .def("setDescription", &NTetrahedron::setDescription)
        .def("index", &NTetrahedron::index)
        .def("adjacentTetrahedron", &NTetrahedron::adjacentTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("adjacentSimplex", &NTetrahedron::adjacentSimplex,
            return_value_policy<reference_existing_object>())
        .def("adjacentGluing", &NTetrahedron::adjacentGluing)
        .def("adjacentFace", &NTetrahedron::adjacentFace)
        .def("adjacentFacet", &NTetrahedron::adjacentFacet)
        .def("hasBoundary", &NTetrahedron::hasBoundary)
        .def("joinTo", &NTetrahedron::join)
        .def("join", &NTetrahedron::join)
        .def("unjoin", &NTetrahedron::unjoin,
            return_value_policy<reference_existing_object>())
        .def("isolate", &NTetrahedron::isolate)
        .def("triangulation", &NTetrahedron::triangulation,
            return_value_policy<to_held_type<> >())
        .def("getTriangulation", &NTetrahedron::triangulation,
            return_value_policy<to_held_type<> >())
        .def("component", &NTetrahedron::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &NTetrahedron::component,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<NTetrahedron, 3, int>)
        .def("vertex", &NTetrahedron::vertex,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NTetrahedron::vertex,
            return_value_policy<reference_existing_object>())
        .def("edge", &NTetrahedron::edge,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &NTetrahedron::edge,
            return_value_policy<reference_existing_object>())
        .def("triangle", &NTetrahedron::triangle,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &NTetrahedron::triangle,
            return_value_policy<reference_existing_object>())
        .def("faceMapping", &regina::python::faceMapping<NTetrahedron, 3>)
        .def("vertexMapping", &NTetrahedron::vertexMapping)
        .def("getVertexMapping", &NTetrahedron::vertexMapping)
        .def("edgeMapping", &NTetrahedron::edgeMapping)
        .def("getEdgeMapping", &NTetrahedron::edgeMapping)
        .def("triangleMapping", &NTetrahedron::triangleMapping)
        .def("getTriangleMapping", &NTetrahedron::triangleMapping)
        .def("orientation", &NTetrahedron::orientation)
        .def("facetInMaximalForest", &NTetrahedron::facetInMaximalForest)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NTetrahedron") = scope().attr("Simplex3");
}

