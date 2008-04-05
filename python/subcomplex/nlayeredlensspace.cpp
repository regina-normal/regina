
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "subcomplex/nlayeredlensspace.h"
#include "triangulation/ncomponent.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NLayeredLensSpace;

void addNLayeredLensSpace() {
    class_<NLayeredLensSpace, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NLayeredLensSpace>, boost::noncopyable>
            ("NLayeredLensSpace", no_init)
        .def("clone", &NLayeredLensSpace::clone,
            return_value_policy<manage_new_object>())
        .def("getP", &NLayeredLensSpace::getP)
        .def("getQ", &NLayeredLensSpace::getQ)
        .def("getTorus", &NLayeredLensSpace::getTorus,
            return_internal_reference<>())
        .def("getMobiusBoundaryGroup",
            &NLayeredLensSpace::getMobiusBoundaryGroup)
        .def("isSnapped", &NLayeredLensSpace::isSnapped)
        .def("isTwisted", &NLayeredLensSpace::isTwisted)
        .def("isLayeredLensSpace", &NLayeredLensSpace::isLayeredLensSpace,
            return_value_policy<manage_new_object>())
        .staticmethod("isLayeredLensSpace")
    ;

    implicitly_convertible<std::auto_ptr<NLayeredLensSpace>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

