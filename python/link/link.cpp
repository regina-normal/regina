
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

#include "link/link.h"
#include "../helpers.h"
#include "../safeheldtype.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Crossing;
using regina::CrossingStrand;
using regina::Link;

namespace {
    regina::Crossing* (Link::*crossing_nonconst)(size_t) = &Link::crossing;
    Link* (*fromJenkins_str)(const std::string&) = &Link::fromJenkins;

    void strand_inc_operator(CrossingStrand& s) {
       ++s;
    }

    void strand_dec_operator(CrossingStrand& s) {
       --s;
    }
}

void addLink() {
    class_<CrossingStrand>("CrossingStrand", init<>())
        .def(init<Crossing*, int>())
        .def(init<const CrossingStrand&>())
        .def("crossing", &CrossingStrand::crossing,
            return_value_policy<reference_existing_object>())
        .def("strand", &CrossingStrand::strand)
        .def("inc", strand_inc_operator)
        .def("dec", strand_dec_operator)
        // TODO: bool()
        .def("next", &CrossingStrand::next)
        .def("prev", &CrossingStrand::prev)
        .def(regina::python::add_eq_operators())
    ;

    class_<Crossing, std::auto_ptr<Crossing>, boost::noncopyable>
            ("Crossing", no_init)
        .def("index", &Crossing::index)
        .def("sign", &Crossing::sign)
        .def("next", &Crossing::next)
        .def("prev", &Crossing::prev)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope s = class_<Link, bases<regina::NPacket>, SafeHeldType<Link>,
            boost::noncopyable>("Link", init<>())
        .def(init<const Link&>())
        .def("size", &Link::size)
        .def("countComponents", &Link::countComponents)
        .def("crossing", crossing_nonconst,
            return_internal_reference<>())
        .def("fromJenkins", fromJenkins_str,
            return_value_policy<to_held_type<>>())
        .def("reflect", &Link::reflect)
        .def("rotate", &Link::rotate)
        .staticmethod("fromJenkins")
    ;

    s.attr("typeID") = regina::PACKET_LINK;
    s.attr("packetType") = regina::PACKET_LINK;

    implicitly_convertible<SafeHeldType<Link>, SafeHeldType<regina::NPacket>>();
}
