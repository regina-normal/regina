
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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
#include "triangulation/nedge.h"
#include "triangulation/nface.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/nvertex.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NFace;
using regina::NFaceEmbedding;

void addNFace() {
    class_<NFaceEmbedding, boost::noncopyable>("NFaceEmbedding",
            init<regina::NTetrahedron*, int>())
        .def(init<const NFaceEmbedding&>())
        .def("getTetrahedron", &NFaceEmbedding::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getFace", &NFaceEmbedding::getFace)
        .def("getVertices", &NFaceEmbedding::getVertices)
    ;

    scope s = class_<NFace, bases<regina::ShareableObject>,
            std::auto_ptr<NFace>, boost::noncopyable>("NFace", no_init)
        .def("isBoundary", &NFace::isBoundary)
        .def("getType", &NFace::getType)
        .def("getSubtype", &NFace::getSubtype)
        .def("isMobiusBand", &NFace::isMobiusBand)
        .def("isCone", &NFace::isCone)
        .def("getNumberOfEmbeddings", &NFace::getNumberOfEmbeddings)
        .def("getEmbedding", &NFace::getEmbedding,
            return_internal_reference<>())
        .def("getComponent", &NFace::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &NFace::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &NFace::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &NFace::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getEdgeMapping", &NFace::getEdgeMapping)
    ;

    s.attr("TRIANGLE") = NFace::TRIANGLE;
    s.attr("SCARF") = NFace::SCARF;
    s.attr("PARACHUTE") = NFace::PARACHUTE;
    s.attr("CONE") = NFace::CONE;
    s.attr("MOBIUS") = NFace::MOBIUS;
    s.attr("HORN") = NFace::HORN;
    s.attr("DUNCEHAT") = NFace::DUNCEHAT;
    s.attr("L31") = NFace::L31;
}

