
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include "packet/ntext.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NText;

namespace {
    void (NText::*setText_string)(const std::string&) = &NText::setText;
    void (NText::*setText_chars)(const char*) = &NText::setText;
}

void addNText() {
    scope s = class_<NText, bases<regina::NPacket>,
            std::auto_ptr<NText>, boost::noncopyable>("NText", init<>())
        .def(init<const std::string&>())
        .def(init<const char*>())
        .def("getText", &NText::getText,
            return_value_policy<return_by_value>())
        .def("setText", setText_string)
        .def("setText", setText_chars)
    ;

    s.attr("packetType") = NText::packetType;

    implicitly_convertible<std::auto_ptr<NText>,
        std::auto_ptr<regina::NPacket> >();
}

