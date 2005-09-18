
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include "angle/nanglestructure.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NAngleStructure;

void addNAngleStructure() {
    class_<NAngleStructure, bases<regina::ShareableObject>,
            std::auto_ptr<NAngleStructure>, boost::noncopyable>
            ("NAngleStructure", no_init)
        .def("clone", &NAngleStructure::clone,
            return_value_policy<manage_new_object>())
        .def("getAngle", &NAngleStructure::getAngle)
        .def("getTriangulation", &NAngleStructure::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("isStrict", &NAngleStructure::isStrict)
        .def("isTaut", &NAngleStructure::isTaut)
    ;
}

