
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

#include <boost/python.hpp>
#include "subcomplex/nplugtrisolidtorus.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NPlugTriSolidTorus;

void addNPlugTriSolidTorus() {
    scope s = class_<NPlugTriSolidTorus, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NPlugTriSolidTorus>, boost::noncopyable>
            ("NPlugTriSolidTorus", no_init)
        .def("clone", &NPlugTriSolidTorus::clone,
            return_value_policy<manage_new_object>())
        .def("core", &NPlugTriSolidTorus::core,
            return_internal_reference<>())
        .def("getCore", &NPlugTriSolidTorus::core,
            return_internal_reference<>())
        .def("chain", &NPlugTriSolidTorus::chain,
            return_internal_reference<>())
        .def("getChain", &NPlugTriSolidTorus::chain,
            return_internal_reference<>())
        .def("chainType", &NPlugTriSolidTorus::chainType)
        .def("getChainType", &NPlugTriSolidTorus::chainType)
        .def("equatorType", &NPlugTriSolidTorus::equatorType)
        .def("getEquatorType", &NPlugTriSolidTorus::equatorType)
        .def("isPlugTriSolidTorus", &NPlugTriSolidTorus::isPlugTriSolidTorus,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
        .staticmethod("isPlugTriSolidTorus")
    ;

    s.attr("CHAIN_NONE") = NPlugTriSolidTorus::CHAIN_NONE;
    s.attr("CHAIN_MAJOR") = NPlugTriSolidTorus::CHAIN_MAJOR;
    s.attr("CHAIN_MINOR") = NPlugTriSolidTorus::CHAIN_MINOR;
    s.attr("EQUATOR_MAJOR") = NPlugTriSolidTorus::EQUATOR_MAJOR;
    s.attr("EQUATOR_MINOR") = NPlugTriSolidTorus::EQUATOR_MINOR;

    implicitly_convertible<std::auto_ptr<NPlugTriSolidTorus>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

