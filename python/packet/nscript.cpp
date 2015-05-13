
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
#include "packet/nscript.h"
#include "../utilities/equality.h"

using namespace boost::python;
using regina::NScript;

namespace {
    regina::NPacket* (NScript::*getVariableValue_long)(unsigned long)
        const = &NScript::getVariableValue;
    regina::NPacket* (NScript::*getVariableValue_string)(const std::string&)
        const = &NScript::getVariableValue;

    void (NScript::*removeVariable_long)(unsigned long) =
        &NScript::removeVariable;
    void (NScript::*removeVariable_string)(const std::string&) =
        &NScript::removeVariable;
}

void addNScript() {
    scope s = class_<NScript, bases<regina::NPacket>,
            std::auto_ptr<NScript>, boost::noncopyable>("NScript", init<>())
        .def("getText", &NScript::getText,
            return_value_policy<return_by_value>())
        .def("setText", &NScript::setText)
        .def("append", &NScript::append)
        .def("getNumberOfVariables", &NScript::getNumberOfVariables)
        .def("getVariableName", &NScript::getVariableName,
            return_value_policy<return_by_value>())
        .def("getVariableValue", getVariableValue_long,
            return_value_policy<reference_existing_object>())
        .def("getVariableValue", getVariableValue_string,
            return_value_policy<reference_existing_object>())
        .def("getVariableIndex", &NScript::getVariableIndex)
        .def("setVariableName", &NScript::setVariableName)
        .def("setVariableValue", &NScript::setVariableValue)
        .def("addVariable", &NScript::addVariable)
        .def("removeVariable", removeVariable_long)
        .def("removeVariable", removeVariable_string)
        .def("removeAllVariables", &NScript::removeAllVariables)
        EQUAL_BY_PTR(NScript)
    ;

    s.attr("packetType") = regina::PacketType(NScript::packetType);

    implicitly_convertible<std::auto_ptr<NScript>,
        std::auto_ptr<regina::NPacket> >();
}

