
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
#include "snappea/examplesnappea.h"
#include "snappea/snappeatriangulation.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::ExampleSnapPea;

void addExampleSnapPea() {
    class_<ExampleSnapPea>("ExampleSnapPea",
            no_init)
        .def("figureEight", &ExampleSnapPea::figureEight,
            return_value_policy<to_held_type<>>())
        .def("trefoil", &ExampleSnapPea::trefoil,
            return_value_policy<to_held_type<>>())
        .def("whiteheadLink", &ExampleSnapPea::whiteheadLink,
            return_value_policy<to_held_type<>>())
        .def("gieseking", &ExampleSnapPea::gieseking,
            return_value_policy<to_held_type<>>())
        .def("x101", &ExampleSnapPea::x101,
            return_value_policy<to_held_type<>>())
        .def(regina::python::no_eq_operators())
        .staticmethod("figureEight")
        .staticmethod("trefoil")
        .staticmethod("whiteheadLink")
        .staticmethod("gieseking")
        .staticmethod("x101")
    ;

    scope().attr("NExampleSnapPeaTriangulation") =
        scope().attr("ExampleSnapPea");
}

