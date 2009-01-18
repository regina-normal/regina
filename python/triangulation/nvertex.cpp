
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

#include "triangulation/nboundarycomponent.h"
#include "triangulation/ncomponent.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/nvertex.h"
#include "../globalarray.h"
#include <boost/python.hpp>

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
}

void addNVertex() {
    class_<NVertexEmbedding>("NVertexEmbedding",
            init<regina::NTetrahedron*, int>())
        .def(init<const NVertexEmbedding&>())
        .def("getTetrahedron", &NVertexEmbedding::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NVertexEmbedding::getVertex)
    ;

    scope s = class_<NVertex, bases<regina::ShareableObject>,
            std::auto_ptr<NVertex>, boost::noncopyable>("NVertex", no_init)
        .def("getEmbeddings", vertex_getEmbeddings_list)
        .def("getNumberOfEmbeddings", &NVertex::getNumberOfEmbeddings)
        .def("getEmbedding", &NVertex::getEmbedding,
            return_internal_reference<>())
        .def("getComponent", &NVertex::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &NVertex::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getDegree", &NVertex::getDegree)
        .def("getLink", &NVertex::getLink)
        .def("isLinkClosed", &NVertex::isLinkClosed)
        .def("isIdeal", &NVertex::isIdeal)
        .def("isBoundary", &NVertex::isBoundary)
        .def("isStandard", &NVertex::isStandard)
        .def("isLinkOrientable", &NVertex::isLinkOrientable)
        .def("getLinkEulerCharacteristic",
            &NVertex::getLinkEulerCharacteristic)
    ;

    s.attr("SPHERE") = NVertex::SPHERE;
    s.attr("DISC") = NVertex::DISC;
    s.attr("TORUS") = NVertex::TORUS;
    s.attr("KLEIN_BOTTLE") = NVertex::KLEIN_BOTTLE;
    s.attr("NON_STANDARD_CUSP") = NVertex::NON_STANDARD_CUSP;
    s.attr("NON_STANDARD_BDRY") = NVertex::NON_STANDARD_BDRY;
}

