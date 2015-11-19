
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
#include "dim2/dim2triangulation.h"
#include "triangulation/nboundarycomponent.h"
#include "triangulation/ncomponent.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include "triangulation/nvertex.h"
#include "../globalarray.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NVertex;
using regina::NVertexEmbedding;

namespace {
    boost::python::list vertex_getEmbeddings_list(const NVertex* v) {
        const std::vector<NVertexEmbedding>& embs = v->getEmbeddings();
        std::vector<NVertexEmbedding>::const_iterator it;

        boost::python::list ans;
        for (it = embs.begin(); it != embs.end(); it++)
            ans.append(*it);
        return ans;
    }

    boost::python::tuple vertex_buildLinkDetail_bool(const NVertex* v,
            bool labels = true) {
        regina::NIsomorphism* iso;
        regina::Dim2Triangulation* link = v->buildLinkDetail(labels, &iso);
        return make_tuple(
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Dim2Triangulation*>::type()(link))),
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::NIsomorphism*>::type()(iso))));
    }

    boost::python::tuple vertex_buildLinkDetail_void(const NVertex* v) {
        return vertex_buildLinkDetail_bool(v);
    }
}

void addNVertex() {
    class_<NVertexEmbedding>("NVertexEmbedding",
            init<regina::NTetrahedron*, int>())
        .def(init<const NVertexEmbedding&>())
        .def("getTetrahedron", &NVertexEmbedding::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NVertexEmbedding::getVertex)
        .def("getVertices", &NVertexEmbedding::getVertices)
        .def(regina::python::add_eq_operators())
    ;

    scope s = class_<NVertex, std::auto_ptr<NVertex>, boost::noncopyable>
            ("NVertex", no_init)
        .def("index", &NVertex::index)
        .def("getEmbeddings", vertex_getEmbeddings_list)
        .def("getNumberOfEmbeddings", &NVertex::getNumberOfEmbeddings)
        .def("getEmbedding", &NVertex::getEmbedding,
            return_internal_reference<>())
        .def("getTriangulation", &NVertex::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &NVertex::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &NVertex::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getDegree", &NVertex::getDegree)
        .def("getLink", &NVertex::getLink)
        .def("buildLink", &NVertex::buildLink,
            return_value_policy<reference_existing_object>())
        .def("buildLinkDetail", vertex_buildLinkDetail_void)
        .def("buildLinkDetail", vertex_buildLinkDetail_bool)
        .def("isLinkClosed", &NVertex::isLinkClosed)
        .def("isIdeal", &NVertex::isIdeal)
        .def("isBoundary", &NVertex::isBoundary)
        .def("isStandard", &NVertex::isStandard)
        .def("isLinkOrientable", &NVertex::isLinkOrientable)
        .def("getLinkEulerChar", &NVertex::getLinkEulerChar)
        .def("getLinkEulerCharacteristic",
            &NVertex::getLinkEulerCharacteristic)
        .def("str", &NVertex::str)
        .def("toString", &NVertex::toString)
        .def("detail", &NVertex::detail)
        .def("toStringLong", &NVertex::toStringLong)
        .def("__str__", &NVertex::str)
        .def(regina::python::add_eq_operators())
    ;

    enum_<regina::NVertex::LinkType>("LinkType")
        .value("SPHERE", regina::NVertex::SPHERE)
        .value("DISC", regina::NVertex::DISC)
        .value("TORUS", regina::NVertex::TORUS)
        .value("KLEIN_BOTTLE", regina::NVertex::KLEIN_BOTTLE)
        .value("NON_STANDARD_CUSP", regina::NVertex::NON_STANDARD_CUSP)
        .value("NON_STANDARD_BDRY", regina::NVertex::NON_STANDARD_BDRY)
        ;

    s.attr("SPHERE") = NVertex::SPHERE;
    s.attr("DISC") = NVertex::DISC;
    s.attr("TORUS") = NVertex::TORUS;
    s.attr("KLEIN_BOTTLE") = NVertex::KLEIN_BOTTLE;
    s.attr("NON_STANDARD_CUSP") = NVertex::NON_STANDARD_CUSP;
    s.attr("NON_STANDARD_BDRY") = NVertex::NON_STANDARD_BDRY;
}

