
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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
#include "link/examplelink.h"
#include "link/link.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::ExampleLink;
using regina::Link;

void addExampleLink() {
    class_<ExampleLink>("ExampleLink", no_init)
        .def("unknot", &ExampleLink::unknot,
            return_value_policy<to_held_type<> >())
        .def("gordian", &ExampleLink::gordian,
            return_value_policy<to_held_type<> >())
        .def("trefoilLeft", &ExampleLink::trefoilLeft,
            return_value_policy<to_held_type<> >())
        .def("trefoilRight", &ExampleLink::trefoilRight,
            return_value_policy<to_held_type<> >())
        .def("trefoil", &ExampleLink::trefoil,
            return_value_policy<to_held_type<> >())
        .def("figureEight", &ExampleLink::figureEight,
            return_value_policy<to_held_type<> >())
        .def("hopf", &ExampleLink::hopf,
            return_value_policy<to_held_type<> >())
        .def("whitehead", &ExampleLink::whitehead,
            return_value_policy<to_held_type<> >())
        .def("borromean", &ExampleLink::borromean,
            return_value_policy<to_held_type<> >())
        .def("monster", &ExampleLink::monster,
            return_value_policy<to_held_type<> >())
        .def("conway", &ExampleLink::conway,
            return_value_policy<to_held_type<> >())
        .def("kinoshitaTerasaka", &ExampleLink::kinoshitaTerasaka,
            return_value_policy<to_held_type<> >())
        /*
        .def("torus", &ExampleLink::torus,
            return_value_policy<to_held_type<> >())
        */
        .def("gst", &ExampleLink::gst,
            return_value_policy<to_held_type<> >())
        .def(regina::python::no_eq_operators())
        .staticmethod("unknot")
        .staticmethod("gordian")
        .staticmethod("trefoilLeft")
        .staticmethod("trefoilRight")
        .staticmethod("trefoil")
        .staticmethod("figureEight")
        .staticmethod("hopf")
        .staticmethod("whitehead")
        .staticmethod("borromean")
        .staticmethod("monster")
        .staticmethod("conway")
        .staticmethod("kinoshitaTerasaka")
        // .staticmethod("torus")
        .staticmethod("gst")
    ;
}

