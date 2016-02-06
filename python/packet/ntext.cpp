
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

#include "packet/ntext.h"
#include "../safeheldtype.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::NText;

namespace {
    void (NText::*setText_string)(const std::string&) = &NText::setText;
    void (NText::*setText_chars)(const char*) = &NText::setText;
}

void addNText() {
    scope s = class_<NText, bases<regina::NPacket>,
            SafeHeldType<NText>, boost::noncopyable>("NText", init<>())
        .def(init<const std::string&>())
        .def(init<const char*>())
        .def("text", &NText::text,
            return_value_policy<return_by_value>())
        .def("getText", &NText::text,
            return_value_policy<return_by_value>())
        .def("setText", setText_string)
        .def("setText", setText_chars)
    ;

    s.attr("typeID") = regina::PACKET_TEXT;
    s.attr("packetType") = regina::PACKET_TEXT;

    implicitly_convertible<SafeHeldType<NText>,
        SafeHeldType<regina::NPacket> >();
}

