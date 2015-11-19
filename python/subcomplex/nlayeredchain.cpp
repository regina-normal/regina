
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
#include "subcomplex/nlayeredchain.h"
#include "triangulation/ntetrahedron.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NLayeredChain;

void addNLayeredChain() {
    class_<NLayeredChain, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NLayeredChain>, boost::noncopyable>
            ("NLayeredChain", init<regina::NTetrahedron*, regina::NPerm4>())
        .def(init<const NLayeredChain&>())
        .def("getBottom", &NLayeredChain::getBottom,
            return_value_policy<reference_existing_object>())
        .def("getTop", &NLayeredChain::getTop,
            return_value_policy<reference_existing_object>())
        .def("getIndex", &NLayeredChain::getIndex)
        .def("getBottomVertexRoles", &NLayeredChain::getBottomVertexRoles)
        .def("getTopVertexRoles", &NLayeredChain::getTopVertexRoles)
        .def("extendAbove", &NLayeredChain::extendAbove)
        .def("extendBelow", &NLayeredChain::extendBelow)
        .def("extendMaximal", &NLayeredChain::extendMaximal)
        .def("reverse", &NLayeredChain::reverse)
        .def("invert", &NLayeredChain::invert)
        .def(regina::python::add_eq_operators())
    ;

    implicitly_convertible<std::auto_ptr<NLayeredChain>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

