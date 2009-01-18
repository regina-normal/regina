
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

#include "manifold/nsfs.h"
#include "subcomplex/naugtrisolidtorus.h"
#include "triangulation/ncomponent.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NAugTriSolidTorus;

void addNAugTriSolidTorus() {
    scope s = class_<NAugTriSolidTorus, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NAugTriSolidTorus>, boost::noncopyable>
            ("NAugTriSolidTorus", no_init)
        .def("clone", &NAugTriSolidTorus::clone,
            return_value_policy<manage_new_object>())
        .def("getCore", &NAugTriSolidTorus::getCore,
            return_internal_reference<>())
        .def("getAugTorus", &NAugTriSolidTorus::getAugTorus,
            return_internal_reference<>())
        .def("getEdgeGroupRoles", &NAugTriSolidTorus::getEdgeGroupRoles)
        .def("getChainLength", &NAugTriSolidTorus::getChainLength)
        .def("getChainType", &NAugTriSolidTorus::getChainType)
        .def("getTorusAnnulus", &NAugTriSolidTorus::getTorusAnnulus)
        .def("hasLayeredChain", &NAugTriSolidTorus::hasLayeredChain)
        .def("isAugTriSolidTorus", &NAugTriSolidTorus::isAugTriSolidTorus,
            return_value_policy<manage_new_object>())
        .staticmethod("isAugTriSolidTorus")
    ;

    s.attr("CHAIN_NONE") = NAugTriSolidTorus::CHAIN_NONE;
    s.attr("CHAIN_MAJOR") = NAugTriSolidTorus::CHAIN_MAJOR;
    s.attr("CHAIN_AXIS") = NAugTriSolidTorus::CHAIN_AXIS;

    implicitly_convertible<std::auto_ptr<NAugTriSolidTorus>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

