
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

#include "packet/nscript.h"
#include "../safeheldtype.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::NScript;

namespace {
    regina::NPacket* (NScript::*variableValue_long)(size_t) const =
        &NScript::variableValue;
    regina::NPacket* (NScript::*variableValue_string)(const std::string&)
        const = &NScript::variableValue;

    void (NScript::*removeVariable_long)(size_t) = &NScript::removeVariable;
    void (NScript::*removeVariable_string)(const std::string&) =
        &NScript::removeVariable;
}

void addNScript() {
    scope s = class_<NScript, bases<regina::NPacket>,
            SafeHeldType<NScript>, boost::noncopyable>("NScript", init<>())
        .def("text", &NScript::text,
            return_value_policy<return_by_value>())
        .def("getText", &NScript::getText,
            return_value_policy<return_by_value>())
        .def("setText", &NScript::setText)
        .def("append", &NScript::append)
        .def("countVariables", &NScript::countVariables)
        .def("getNumberOfVariables", &NScript::getNumberOfVariables)
        .def("variableName", &NScript::variableName,
            return_value_policy<return_by_value>())
        .def("getVariableName", &NScript::getVariableName,
            return_value_policy<return_by_value>())
        .def("variableValue", variableValue_long,
            return_value_policy<to_held_type<>>())
        .def("getVariableValue", variableValue_long,
            return_value_policy<to_held_type<>>())
        .def("variableValue", variableValue_string,
            return_value_policy<to_held_type<>>())
        .def("getVariableValue", variableValue_string,
            return_value_policy<to_held_type<>>())
        .def("variableIndex", &NScript::variableIndex)
        .def("getVariableIndex", &NScript::getVariableIndex)
        .def("setVariableName", &NScript::setVariableName)
        .def("setVariableValue", &NScript::setVariableValue)
        .def("addVariable", &NScript::addVariable)
        .def("removeVariable", removeVariable_long)
        .def("removeVariable", removeVariable_string)
        .def("removeAllVariables", &NScript::removeAllVariables)
    ;

    s.attr("typeID") = regina::PACKET_SCRIPT;
    s.attr("packetType") = regina::PACKET_SCRIPT;

    implicitly_convertible<SafeHeldType<NScript>,
        SafeHeldType<regina::NPacket> >();
}

