
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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
#include "angle/nanglestructure.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::NAngleStructure;
using regina::NTriangulation;

void addNAngleStructure() {
    class_<NAngleStructure, std::auto_ptr<NAngleStructure>, boost::noncopyable>
            ("NAngleStructure", no_init)
        .def("clone", &NAngleStructure::clone,
            return_value_policy<manage_new_object>())
        .def("angle", &NAngleStructure::angle)
        .def("getAngle", &NAngleStructure::angle)
        .def("triangulation", &NAngleStructure::triangulation,
            return_value_policy<to_held_type<> >())
        .def("getTriangulation", &NAngleStructure::triangulation,
            return_value_policy<to_held_type<> >())
        .def("isStrict", &NAngleStructure::isStrict)
        .def("isTaut", &NAngleStructure::isTaut)
        .def("isVeering", &NAngleStructure::isVeering)
        .def("str", &NAngleStructure::str)
        .def("toString", &NAngleStructure::str)
        .def("detail", &NAngleStructure::detail)
        .def("toStringLong", &NAngleStructure::detail)
        .def("__str__", &NAngleStructure::str)
        .def(regina::python::add_eq_operators())
    ;
}

