
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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
#include "dim4/dim4boundarycomponent.h"
#include "dim4/dim4component.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4tetrahedron.h"
#include "dim4/dim4triangle.h"
#include "dim4/dim4triangulation.h"
#include "dim4/dim4vertex.h"
#include "../globalarray.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Dim4Tetrahedron;
using regina::Dim4TetrahedronEmbedding;
using regina::python::GlobalArray;

namespace {
    boost::python::list Dim4Tetrahedron_getEmbeddings_list(
            const Dim4Tetrahedron* t) {
        boost::python::list ans;
        for (auto& emb : *t)
            ans.append(emb);
        return ans;
    }
}

void addDim4Tetrahedron() {
    class_<Dim4TetrahedronEmbedding, boost::noncopyable>
            ("Dim4TetrahedronEmbedding", init<regina::Dim4Pentachoron*, int>())
        .def(init<const Dim4TetrahedronEmbedding&>())
        .def("simplex", &Dim4TetrahedronEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &Dim4TetrahedronEmbedding::getSimplex,
            return_value_policy<reference_existing_object>())
        .def("pentachoron", &Dim4TetrahedronEmbedding::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("getPentachoron", &Dim4TetrahedronEmbedding::getPentachoron,
            return_value_policy<reference_existing_object>())
        .def("face", &Dim4TetrahedronEmbedding::face)
        .def("getFace", &Dim4TetrahedronEmbedding::getFace)
        .def("tetrahedron", &Dim4TetrahedronEmbedding::tetrahedron)
        .def("getTetrahedron", &Dim4TetrahedronEmbedding::getTetrahedron)
        .def("vertices", &Dim4TetrahedronEmbedding::vertices)
        .def("getVertices", &Dim4TetrahedronEmbedding::getVertices)
        .def("str", &Dim4TetrahedronEmbedding::str)
        .def("toString", &Dim4TetrahedronEmbedding::toString)
        .def("detail", &Dim4TetrahedronEmbedding::detail)
        .def("toStringLong", &Dim4TetrahedronEmbedding::toStringLong)
        .def("__str__", &Dim4TetrahedronEmbedding::str)
        .def(regina::python::add_eq_operators())
    ;

    class_<Dim4Tetrahedron, std::auto_ptr<Dim4Tetrahedron>,
            boost::noncopyable>("Dim4Tetrahedron", no_init)
        .def("index", &Dim4Tetrahedron::index)
        .def("degree", &Dim4Tetrahedron::degree)
        .def("getDegree", &Dim4Tetrahedron::getDegree)
        .def("embeddings", Dim4Tetrahedron_getEmbeddings_list)
        .def("getEmbeddings", Dim4Tetrahedron_getEmbeddings_list)
        .def("embedding", &Dim4Tetrahedron::embedding,
            return_internal_reference<>())
        .def("getEmbedding", &Dim4Tetrahedron::getEmbedding,
            return_internal_reference<>())
        .def("front", &Dim4Tetrahedron::front,
            return_internal_reference<>())
        .def("back", &Dim4Tetrahedron::back,
            return_internal_reference<>())
        .def("triangulation", &Dim4Tetrahedron::triangulation,
            return_value_policy<reference_existing_object>())
        .def("getTriangulation", &Dim4Tetrahedron::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("component", &Dim4Tetrahedron::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &Dim4Tetrahedron::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Tetrahedron::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4Tetrahedron::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4Tetrahedron::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &Dim4Tetrahedron::getTriangle,
            return_value_policy<reference_existing_object>())
        .def("getEdgeMapping", &Dim4Tetrahedron::getEdgeMapping)
        .def("getTriangleMapping", &Dim4Tetrahedron::getTriangleMapping)
        .def("isValid", &Dim4Tetrahedron::isValid)
        .def("isLinkOrientable", &Dim4Tetrahedron::isLinkOrientable)
        .def("isBoundary", &Dim4Tetrahedron::isBoundary)
        .def("inMaximalForest", &Dim4Tetrahedron::inMaximalForest)
        .def("str", &Dim4Tetrahedron::str)
        .def("toString", &Dim4Tetrahedron::toString)
        .def("detail", &Dim4Tetrahedron::detail)
        .def("toStringLong", &Dim4Tetrahedron::toStringLong)
        .def("__str__", &Dim4Tetrahedron::str)
        .def("ordering", &Dim4Tetrahedron::ordering)
        .def("faceNumber", &Dim4Tetrahedron::faceNumber)
        .def("containsVertex", &Dim4Tetrahedron::containsVertex)
        .def(regina::python::add_eq_operators())
        .staticmethod("ordering")
        .staticmethod("faceNumber")
        .staticmethod("containsVertex")
    ;
}

