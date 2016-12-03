
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

#include "angle/anglestructures.h"
#include "progress/progresstracker.h"
#include "triangulation/dim3.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::AngleStructures;

namespace {
    // Write manual overload wrappers since this is a static member function.
    AngleStructures* enumerate_1(regina::Triangulation<3>* owner) {
        return AngleStructures::enumerate(owner);
    }
    AngleStructures* enumerate_2(regina::Triangulation<3>* owner,
            bool tautOnly) {
        return AngleStructures::enumerate(owner, tautOnly);
    }
    AngleStructures* enumerate_3(regina::Triangulation<3>* owner,
            bool tautOnly, regina::ProgressTracker* tracker) {
        return AngleStructures::enumerate(owner, tautOnly, tracker);
    }
}

void addAngleStructures() {
    def("makeAngleEquations", regina::makeAngleEquations,
        return_value_policy<manage_new_object>());

    class_<AngleStructures, bases<regina::Packet>,
            SafeHeldType<AngleStructures>, boost::noncopyable>
            ("AngleStructures", no_init)
        .def("triangulation", &AngleStructures::triangulation,
            return_value_policy<to_held_type<> >())
        .def("isTautOnly", &AngleStructures::isTautOnly)
        .def("size", &AngleStructures::size)
        .def("structure", &AngleStructures::structure,
            return_internal_reference<>())
        .def("spansStrict", &AngleStructures::spansStrict)
        .def("spansTaut", &AngleStructures::spansTaut)
        .def("enumerate", enumerate_1,
            return_value_policy<to_held_type<> >())
        .def("enumerate", enumerate_2,
            return_value_policy<to_held_type<> >())
        .def("enumerate", enumerate_3,
            return_value_policy<to_held_type<> >())
        .def("enumerateTautDD", &AngleStructures::enumerateTautDD,
            return_value_policy<to_held_type<> >())
        .staticmethod("enumerate")
        .staticmethod("enumerateTautDD")
        .attr("typeID") = regina::PACKET_ANGLESTRUCTURES
    ;

    implicitly_convertible<SafeHeldType<AngleStructures>,
        SafeHeldType<regina::Packet> >();

    FIX_REGINA_BOOST_CONVERTERS(AngleStructures);

    scope().attr("NAngleStructureList") = scope().attr("AngleStructures");
}

