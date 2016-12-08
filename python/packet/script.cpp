
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

#include "packet/script.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Script;

namespace {
    regina::Packet* (Script::*variableValue_long)(size_t) const =
        &Script::variableValue;
    regina::Packet* (Script::*variableValue_string)(const std::string&)
        const = &Script::variableValue;

    void (Script::*removeVariable_long)(size_t) = &Script::removeVariable;
    void (Script::*removeVariable_string)(const std::string&) =
        &Script::removeVariable;
}

void addScript() {
    class_<Script, bases<regina::Packet>,
            SafeHeldType<Script>, boost::noncopyable>("Script", init<>())
        .def("text", &Script::text,
            return_value_policy<return_by_value>())
        .def("setText", &Script::setText)
        .def("append", &Script::append)
        .def("countVariables", &Script::countVariables)
        .def("variableName", &Script::variableName,
            return_value_policy<return_by_value>())
        .def("variableValue", variableValue_long,
            return_value_policy<to_held_type<>>())
        .def("variableValue", variableValue_string,
            return_value_policy<to_held_type<>>())
        .def("variableIndex", &Script::variableIndex)
        .def("setVariableName", &Script::setVariableName)
        .def("setVariableValue", &Script::setVariableValue)
        .def("addVariable", &Script::addVariable)
        .def("addVariableName", &Script::addVariableName,
            return_value_policy<return_by_value>())
        .def("removeVariable", removeVariable_long)
        .def("removeVariable", removeVariable_string)
        .def("removeAllVariables", &Script::removeAllVariables)
        .attr("typeID") = regina::PACKET_SCRIPT
    ;

    implicitly_convertible<SafeHeldType<Script>,
        SafeHeldType<regina::Packet> >();

    FIX_REGINA_BOOST_CONVERTERS(Script);

    scope().attr("NScript") = scope().attr("Script");
}

