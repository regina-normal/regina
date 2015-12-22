
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
#include "dim2/dim2triangulation.h"
#include "dim4/dim4boundarycomponent.h"
#include "dim4/dim4component.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4triangulation.h"
#include "dim4/dim4vertex.h"
#include "../globalarray.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Dim4Edge;
using regina::Dim4EdgeEmbedding;
using regina::python::GlobalArray;
using regina::python::GlobalArray2D;

namespace {
    GlobalArray2D<int> Dim4Edge_edgeNumber(Dim4Edge::edgeNumber, 5);
    GlobalArray2D<int> Dim4Edge_edgeVertex(Dim4Edge::edgeVertex, 10);

    boost::python::list Dim4Edge_getEmbeddings_list(const Dim4Edge* e) {
        boost::python::list ans;
        for (auto& emb : *e)
            ans.append(emb);
        return ans;
    }

    boost::python::tuple edge_buildLinkDetail_bool(const Dim4Edge* e,
            bool labels = true) {
        regina::Dim4Isomorphism* iso;
        regina::Dim2Triangulation* link = e->buildLinkDetail(labels, &iso);
        return make_tuple(
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Dim2Triangulation*>::type()(link))),
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Dim4Isomorphism*>::type()(iso))));
    }

    boost::python::tuple edge_buildLinkDetail_void(const Dim4Edge* e) {
        return edge_buildLinkDetail_bool(e);
    }
}

void addDim4Edge() {
    class_<Dim4EdgeEmbedding>("Dim4EdgeEmbedding",
            init<regina::Dim4Pentachoron*, int>())
        .def(init<const Dim4EdgeEmbedding&>())
        .def("simplex", &Dim4EdgeEmbedding::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &Dim4EdgeEmbedding::getSimplex,
            return_value_policy<reference_existing_object>())
        .def("pentachoron", &Dim4EdgeEmbedding::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("getPentachoron", &Dim4EdgeEmbedding::getPentachoron,
            return_value_policy<reference_existing_object>())
        .def("face", &Dim4EdgeEmbedding::face)
        .def("getFace", &Dim4EdgeEmbedding::getFace)
        .def("edge", &Dim4EdgeEmbedding::edge)
        .def("getEdge", &Dim4EdgeEmbedding::getEdge)
        .def("vertices", &Dim4EdgeEmbedding::vertices)
        .def("getVertices", &Dim4EdgeEmbedding::getVertices)
        .def("str", &Dim4EdgeEmbedding::str)
        .def("toString", &Dim4EdgeEmbedding::toString)
        .def("detail", &Dim4EdgeEmbedding::detail)
        .def("toStringLong", &Dim4EdgeEmbedding::toStringLong)
        .def("__str__", &Dim4EdgeEmbedding::str)
        .def(regina::python::add_eq_operators())
    ;

    scope s = class_<Dim4Edge, std::auto_ptr<Dim4Edge>,
            boost::noncopyable>("Dim4Edge", no_init)
        .def("index", &Dim4Edge::index)
        .def("embeddings", Dim4Edge_getEmbeddings_list)
        .def("getEmbeddings", Dim4Edge_getEmbeddings_list)
        .def("embedding", &Dim4Edge::embedding,
            return_internal_reference<>())
        .def("getEmbedding", &Dim4Edge::getEmbedding,
            return_internal_reference<>())
        .def("front", &Dim4Edge::front,
            return_internal_reference<>())
        .def("back", &Dim4Edge::back,
            return_internal_reference<>())
        .def("triangulation", &Dim4Edge::triangulation,
            return_value_policy<reference_existing_object>())
        .def("getTriangulation", &Dim4Edge::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("component", &Dim4Edge::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &Dim4Edge::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Edge::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4Edge::getVertex,
            return_value_policy<reference_existing_object>())
        .def("degree", &Dim4Edge::degree)
        .def("getDegree", &Dim4Edge::getDegree)
        .def("isBoundary", &Dim4Edge::isBoundary)
        .def("isLinkOrientable", &Dim4Edge::isLinkOrientable)
        .def("isValid", &Dim4Edge::isValid)
        .def("hasBadIdentification", &Dim4Edge::hasBadIdentification)
        .def("hasBadLink", &Dim4Edge::hasBadLink)
        .def("buildLink", &Dim4Edge::buildLink,
            return_value_policy<reference_existing_object>())
        .def("buildLinkDetail", edge_buildLinkDetail_void)
        .def("buildLinkDetail", edge_buildLinkDetail_bool)
        .def("str", &Dim4Edge::str)
        .def("toString", &Dim4Edge::toString)
        .def("detail", &Dim4Edge::detail)
        .def("toStringLong", &Dim4Edge::toStringLong)
        .def("__str__", &Dim4Edge::str)
        .def("ordering", &Dim4Edge::ordering)
        .def("faceNumber", &Dim4Edge::faceNumber)
        .def("containsVertex", &Dim4Edge::containsVertex)
        .def(regina::python::add_eq_operators())
        .staticmethod("ordering")
        .staticmethod("faceNumber")
        .staticmethod("containsVertex")
    ;

    s.attr("edgeNumber") = &Dim4Edge_edgeNumber;
    s.attr("edgeVertex") = &Dim4Edge_edgeVertex;
}

