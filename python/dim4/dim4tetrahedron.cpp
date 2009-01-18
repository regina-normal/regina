
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

#include "dim4/dim4boundarycomponent.h"
#include "dim4/dim4component.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4face.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4vertex.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4Tetrahedron;
using regina::Dim4TetrahedronEmbedding;
using regina::python::GlobalArray;

namespace {
    GlobalArray<regina::NPerm5> Dim4Tetrahedron_ordering(
        Dim4Tetrahedron::ordering, 5);
}

void addDim4Tetrahedron() {
    class_<Dim4TetrahedronEmbedding, boost::noncopyable>
            ("Dim4TetrahedronEmbedding", init<regina::Dim4Pentachoron*, int>())
        .def(init<const Dim4TetrahedronEmbedding&>())
        .def("getPentachoron", &Dim4TetrahedronEmbedding::getPentachoron,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &Dim4TetrahedronEmbedding::getTetrahedron)
        .def("getVertices", &Dim4TetrahedronEmbedding::getVertices)
    ;

    scope s = class_<Dim4Tetrahedron, bases<regina::ShareableObject>,
            std::auto_ptr<Dim4Tetrahedron>, boost::noncopyable>
            ("Dim4Tetrahedron", no_init)
        .def("getNumberOfEmbeddings", &Dim4Tetrahedron::getNumberOfEmbeddings)
        .def("getEmbedding", &Dim4Tetrahedron::getEmbedding,
            return_internal_reference<>())
        .def("getComponent", &Dim4Tetrahedron::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Tetrahedron::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4Tetrahedron::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4Tetrahedron::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getFace", &Dim4Tetrahedron::getFace,
            return_value_policy<reference_existing_object>())
        .def("getEdgeMapping", &Dim4Tetrahedron::getEdgeMapping)
        .def("getFaceMapping", &Dim4Tetrahedron::getFaceMapping)
        .def("isBoundary", &Dim4Tetrahedron::isBoundary)
    ;

    s.attr("ordering") = &Dim4Tetrahedron_ordering;
}

