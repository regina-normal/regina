
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
#include "subcomplex/npillowtwosphere.h"
#include "triangulation/ntriangle.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NPillowTwoSphere;

void addNPillowTwoSphere() {
    class_<NPillowTwoSphere, std::auto_ptr<NPillowTwoSphere>,
            boost::noncopyable>
            ("NPillowTwoSphere", no_init)
        .def("clone", &NPillowTwoSphere::clone,
            return_value_policy<manage_new_object>())
        .def("getFace", &NPillowTwoSphere::getFace,
            return_value_policy<reference_existing_object>())
        .def("getTriangle", &NPillowTwoSphere::getTriangle,
            return_value_policy<reference_existing_object>())
        .def("getFaceMapping", &NPillowTwoSphere::getFaceMapping)
        .def("getTriangleMapping", &NPillowTwoSphere::getTriangleMapping)
        .def("formsPillowTwoSphere", &NPillowTwoSphere::formsPillowTwoSphere,
            return_value_policy<manage_new_object>())
        .def("str", &NPillowTwoSphere::str)
        .def("toString", &NPillowTwoSphere::toString)
        .def("detail", &NPillowTwoSphere::detail)
        .def("toStringLong", &NPillowTwoSphere::toStringLong)
        .def("__str__", &NPillowTwoSphere::str)
        .def(regina::python::add_eq_operators())
        .staticmethod("formsPillowTwoSphere")
    ;
}

