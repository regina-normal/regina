
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

using pybind11::overload_cast;
using regina::Script;

void addScript(pybind11::module_& m) {
    auto c = pybind11::class_<Script, regina::Packet, std::shared_ptr<Script>>(
            m, "Script")
        .def(pybind11::init<>())
        .def(pybind11::init<const Script&>())
        .def("swap", &Script::swap)
        .def("text", &Script::text)
        .def("setText", &Script::setText)
        // We define Packet::append() again, since otherwise this is
        // hidden by the binding for Script::append().
        .def("append", &regina::Packet::append)
        .def("append", &Script::append)
        .def("countVariables", &Script::countVariables)
        .def("variableName", &Script::variableName)
        .def("variableValue", overload_cast<size_t>(
            &Script::variableValue, pybind11::const_))
        .def("variableValue", overload_cast<const std::string&>(
            &Script::variableValue, pybind11::const_))
        .def("variableIndex", &Script::variableIndex)
        .def("setVariableName", &Script::setVariableName)
        .def("setVariableValue", [](Script& s, size_t index,
                std::shared_ptr<regina::Packet> value) {
            // We need to reimplement this, since Regina's function
            // takes a weak_ptr, not a shared_ptr.
            s.setVariableValue(index, value);
        }, pybind11::arg(), pybind11::arg("value") = nullptr)
        .def("addVariable", [](Script& s, const std::string& name,
                std::shared_ptr<regina::Packet> value) {
            // We need to reimplement this, since Regina's function
            // takes a weak_ptr, not a shared_ptr.
            return s.addVariable(name, value);
        }, pybind11::arg(), pybind11::arg("value") = nullptr)
        .def("addVariableName", [](Script& s, const std::string& name,
                std::shared_ptr<regina::Packet> value) {
            // We need to reimplement this, since Regina's function
            // takes a weak_ptr, not a shared_ptr.
            return s.addVariableName(name, value);
        }, pybind11::arg(), pybind11::arg("value") = nullptr)
        .def("removeVariable",
            overload_cast<size_t>(&Script::removeVariable))
        .def("removeVariable",
            overload_cast<const std::string&>(&Script::removeVariable))
        .def("removeAllVariables", &Script::removeAllVariables)
        .def_readonly_static("typeID", &Script::typeID)
        .def("listenVariables", &Script::listenVariables)
        .def("unlistenVariables", &Script::unlistenVariables)
    ;
    regina::python::add_output(c);
    regina::python::packet_eq_operators(c);

    m.def("swap", (void(*)(Script&, Script&))(regina::swap));
}

