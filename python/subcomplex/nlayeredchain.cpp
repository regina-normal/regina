
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "subcomplex/nlayeredchain.h"
#include "triangulation/ntetrahedron.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NLayeredChain;

void addNLayeredChain() {
    class_<NLayeredChain, bases<regina::ShareableObject>,
            std::auto_ptr<NLayeredChain>, boost::noncopyable>
            ("NLayeredChain", init<regina::NTetrahedron*, regina::NPerm>())
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
    ;
}

