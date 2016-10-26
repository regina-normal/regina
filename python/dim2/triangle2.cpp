
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

#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Triangle;

void addTriangle2() {
    class_<regina::Simplex<2>, std::auto_ptr<regina::Simplex<2>>,
            boost::noncopyable>("Simplex2", no_init)
        .def("description", &Triangle<2>::description,
            return_value_policy<return_by_value>())
        .def("setDescription", &Triangle<2>::setDescription)
        .def("index", &Triangle<2>::index)
        .def("adjacentTriangle", &Triangle<2>::adjacentTriangle,
            return_value_policy<reference_existing_object>())
        .def("adjacentSimplex", &Triangle<2>::adjacentSimplex,
            return_value_policy<reference_existing_object>())
        .def("adjacentGluing", &Triangle<2>::adjacentGluing)
        .def("adjacentEdge", &Triangle<2>::adjacentEdge)
        .def("adjacentFacet", &Triangle<2>::adjacentFacet)
        .def("hasBoundary", &Triangle<2>::hasBoundary)
        .def("join", &Triangle<2>::join)
        .def("unjoin", &Triangle<2>::unjoin,
            return_value_policy<reference_existing_object>())
        .def("isolate", &Triangle<2>::isolate)
        .def("triangulation", &Triangle<2>::triangulation,
            return_value_policy<to_held_type<> >())
        .def("component", &Triangle<2>::component,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<Triangle<2>, 2, int>)
        .def("vertex", &Triangle<2>::vertex,
            return_value_policy<reference_existing_object>())
        .def("edge", &Triangle<2>::edge,
            return_value_policy<reference_existing_object>())
        .def("faceMapping", &regina::python::faceMapping<Triangle<2>, 2>)
        .def("vertexMapping", &Triangle<2>::vertexMapping)
        .def("edgeMapping", &Triangle<2>::edgeMapping)
        .def("orientation", &Triangle<2>::orientation)
        .def("facetInMaximalForest", &Triangle<2>::facetInMaximalForest)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("Dim2Triangle") = scope().attr("Simplex2");
    scope().attr("Triangle2") = scope().attr("Simplex2");
    scope().attr("Face2_2") = scope().attr("Simplex2");
}

