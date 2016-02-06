
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

#include "angle/nanglestructurelist.h"
#include "progress/nprogresstracker.h"
#include "triangulation/ntriangulation.h"
#include "../safeheldtype.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::NAngleStructureList;

namespace {
    // Write manual overload wrappers since this is a static member function.
    NAngleStructureList* enumerate_1(regina::NTriangulation* owner) {
        return NAngleStructureList::enumerate(owner);
    }
    NAngleStructureList* enumerate_2(regina::NTriangulation* owner,
            bool tautOnly) {
        return NAngleStructureList::enumerate(owner, tautOnly);
    }
    NAngleStructureList* enumerate_3(regina::NTriangulation* owner,
            bool tautOnly, regina::NProgressTracker* tracker) {
        return NAngleStructureList::enumerate(owner, tautOnly, tracker);
    }
}

void addNAngleStructureList() {
    def("makeAngleEquations", regina::makeAngleEquations,
        return_value_policy<manage_new_object>());

    scope s = class_<NAngleStructureList, bases<regina::NPacket>,
            SafeHeldType<NAngleStructureList>, boost::noncopyable>
            ("NAngleStructureList", no_init)
        .def("triangulation", &NAngleStructureList::triangulation,
            return_value_policy<to_held_type<> >())
        .def("getTriangulation", &NAngleStructureList::triangulation,
            return_value_policy<to_held_type<> >())
        .def("isTautOnly", &NAngleStructureList::isTautOnly)
        .def("size", &NAngleStructureList::size)
        .def("getNumberOfStructures", &NAngleStructureList::size)
        .def("structure", &NAngleStructureList::structure,
            return_internal_reference<>())
        .def("getStructure", &NAngleStructureList::structure,
            return_internal_reference<>())
        .def("spansStrict", &NAngleStructureList::spansStrict)
        .def("spansTaut", &NAngleStructureList::spansTaut)
        .def("allowsStrict", &NAngleStructureList::spansStrict)
        .def("allowsTaut", &NAngleStructureList::spansTaut)
        .def("enumerate", enumerate_1,
            return_value_policy<to_held_type<> >())
        .def("enumerate", enumerate_2,
            return_value_policy<to_held_type<> >())
        .def("enumerate", enumerate_3,
            return_value_policy<to_held_type<> >())
        .def("enumerateTautDD", &NAngleStructureList::enumerateTautDD,
            return_value_policy<to_held_type<> >())
        .staticmethod("enumerate")
        .staticmethod("enumerateTautDD")
    ;

    s.attr("typeID") = regina::PACKET_ANGLESTRUCTURELIST;
    s.attr("packetType") = regina::PACKET_ANGLESTRUCTURELIST;

    implicitly_convertible<SafeHeldType<NAngleStructureList>,
        SafeHeldType<regina::NPacket> >();
}

