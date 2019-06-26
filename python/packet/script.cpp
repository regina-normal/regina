
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

#include "../pybind11/pybind11.h"
#include "packet/script.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace regina::python;
using regina::Script;

void addScript(pybind11::module& m) {
    pybind11::class_<Script, regina::Packet, SafeHeldType<Script>>(m, "Script")
        .def(pybind11::init<>())
        .def("text", &Script::text)
        .def("setText", &Script::setText)
        .def("append", &Script::append)
        .def("countVariables", &Script::countVariables)
        .def("variableName", &Script::variableName)
        .def("variableValue", (regina::Packet* (Script::*)(size_t) const)
            &Script::variableValue)
        .def("variableValue",
            (regina::Packet* (Script::*)(const std::string&) const)
            &Script::variableValue)
        .def("variableIndex", &Script::variableIndex)
        .def("setVariableName", &Script::setVariableName)
        .def("setVariableValue", &Script::setVariableValue)
        .def("addVariable", &Script::addVariable)
        .def("addVariableName", &Script::addVariableName)
        .def("removeVariable", (void (Script::*)(size_t))
            &Script::removeVariable)
        .def("removeVariable", (void (Script::*)(const std::string&))
            &Script::removeVariable)
        .def("removeAllVariables", &Script::removeAllVariables)
        .def_property_readonly_static("typeID", [](pybind11::object) {
            // We cannot take the address of typeID, so use a getter function.
            return Script::typeID;
        })
    ;

    m.attr("NScript") = m.attr("Script");
}

