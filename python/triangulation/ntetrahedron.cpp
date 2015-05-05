
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
#include "triangulation/ncomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangle.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nvertex.h"
#include "../globalarray.h"

using namespace boost::python;
using regina::NTetrahedron;

void addNTetrahedron() {
    class_<regina::Simplex<3>, std::auto_ptr<regina::Simplex<3>>,
            boost::noncopyable>("Simplex3", no_init)
        .def("getDescription", &NTetrahedron::getDescription,
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
        .def("joinTo", &NTetrahedron::joinTo)
        .def("join", &NTetrahedron::join)
        .def("unjoin", &NTetrahedron::unjoin,
            return_value_policy<reference_existing_object>())
        .def("isolate", &NTetrahedron::isolate)
        .def("getTriangulation", &NTetrahedron::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &NTetrahedron::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NTetrahedron::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &NTetrahedron::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &NTetrahedron::getTriangle,
            return_value_policy<reference_existing_object>())
        .def("getVertexMapping", &NTetrahedron::getVertexMapping)
        .def("getEdgeMapping", &NTetrahedron::getEdgeMapping)
        .def("getTriangleMapping", &NTetrahedron::getTriangleMapping)
        .def("orientation", &NTetrahedron::orientation)
        .def("str", &NTetrahedron::str)
        .def("toString", &NTetrahedron::toString)
        .def("detail", &NTetrahedron::detail)
        .def("toStringLong", &NTetrahedron::toStringLong)
        .def("__str__", &NTetrahedron::str)
    ;

    scope().attr("NTetrahedron") = scope().attr("Simplex3");
}

