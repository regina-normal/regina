
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
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using namespace regina::python;
using regina::Pentachoron;

void addPentachoron4() {
    class_<regina::Simplex<4>, std::auto_ptr<regina::Simplex<4>>,
            boost::noncopyable>("Simplex4", no_init)
        .def("description", &Pentachoron<4>::description,
            return_value_policy<return_by_value>())
        .def("setDescription", &Pentachoron<4>::setDescription)
        .def("index", &Pentachoron<4>::index)
        .def("adjacentPentachoron", &Pentachoron<4>::adjacentPentachoron,
            return_value_policy<reference_existing_object>())
        .def("adjacentSimplex", &Pentachoron<4>::adjacentSimplex,
            return_value_policy<reference_existing_object>())
        .def("adjacentGluing", &Pentachoron<4>::adjacentGluing)
        .def("adjacentFacet", &Pentachoron<4>::adjacentFacet)
        .def("hasBoundary", &Pentachoron<4>::hasBoundary)
        .def("join", &Pentachoron<4>::join)
        .def("unjoin", &Pentachoron<4>::unjoin,
            return_value_policy<reference_existing_object>())
        .def("isolate", &Pentachoron<4>::isolate)
        .def("triangulation", &Pentachoron<4>::triangulation,
            return_value_policy<to_held_type<>>())
        .def("component", &Pentachoron<4>::component,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<Pentachoron<4>, 4, int>)
        .def("vertex", &Pentachoron<4>::vertex,
            return_value_policy<reference_existing_object>())
        .def("edge", &Pentachoron<4>::edge,
            return_value_policy<reference_existing_object>())
        .def("triangle", &Pentachoron<4>::triangle,
            return_value_policy<reference_existing_object>())
        .def("tetrahedron", &Pentachoron<4>::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("faceMapping", &regina::python::faceMapping<Pentachoron<4>, 4>)
        .def("vertexMapping", &Pentachoron<4>::vertexMapping)
        .def("edgeMapping", &Pentachoron<4>::edgeMapping)
        .def("triangleMapping", &Pentachoron<4>::triangleMapping)
        .def("tetrahedronMapping", &Pentachoron<4>::tetrahedronMapping)
        .def("orientation", &Pentachoron<4>::orientation)
        .def("facetInMaximalForest", &Pentachoron<4>::facetInMaximalForest)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("Dim4Pentachoron") = scope().attr("Simplex4");
    scope().attr("Pentachoron4") = scope().attr("Simplex4");
    scope().attr("Face4_4") = scope().attr("Simplex4");
}

