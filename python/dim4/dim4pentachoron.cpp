
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include "dim4/dim4component.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4face.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4vertex.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4Pentachoron;

void addDim4Pentachoron() {
    class_<Dim4Pentachoron, bases<regina::ShareableObject>,
            std::auto_ptr<Dim4Pentachoron>, boost::noncopyable>(
            "Dim4Pentachoron", init<>())
        .def(init<const std::string&>())
        .def("getDescription", &Dim4Pentachoron::getDescription,
            return_value_policy<return_by_value>())
        .def("setDescription", &Dim4Pentachoron::setDescription)
        .def("adjacentPentachoron", &Dim4Pentachoron::adjacentPentachoron,
            return_value_policy<reference_existing_object>())
        .def("adjacentGluing", &Dim4Pentachoron::adjacentGluing)
        .def("adjacentFacet", &Dim4Pentachoron::adjacentFacet)
        .def("hasBoundary", &Dim4Pentachoron::hasBoundary)
        .def("joinTo", &Dim4Pentachoron::joinTo)
        .def("unjoin", &Dim4Pentachoron::unjoin,
            return_value_policy<reference_existing_object>())
        .def("isolate", &Dim4Pentachoron::isolate)
        .def("getComponent", &Dim4Pentachoron::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4Pentachoron::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4Pentachoron::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getFace", &Dim4Pentachoron::getFace,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &Dim4Pentachoron::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getEdgeMapping", &Dim4Pentachoron::getEdgeMapping)
        .def("getFaceMapping", &Dim4Pentachoron::getFaceMapping)
        .def("getTetrahedronMapping", &Dim4Pentachoron::getTetrahedronMapping)
        .def("orientation", &Dim4Pentachoron::orientation)
    ;
}

