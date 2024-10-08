
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <pybind11/pybind11.h>
#include "packet/script.h"
#include "../helpers.h"
#include "../docstrings/packet/script.h"

using pybind11::overload_cast;
using regina::Script;

void addScript(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Script)

    auto c = pybind11::class_<Script, regina::Packet, std::shared_ptr<Script>>(
            m, "Script", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Script&>(), rdoc::__copy)
        .def("swap", &Script::swap, rdoc::swap)
        .def("text", &Script::text, rdoc::text)
        .def("setText", &Script::setText, rdoc::setText)
        // We define Packet::append() again, since otherwise this is
        // hidden by the binding for Script::append().
        .def("append", &regina::Packet::append,
            regina::python::doc::common::Packet_append)
        .def("append", &Script::append, rdoc::append)
        .def("countVariables", &Script::countVariables, rdoc::countVariables)
        .def("variableName", &Script::variableName, rdoc::variableName)
        .def("variableValue", overload_cast<size_t>(
            &Script::variableValue, pybind11::const_), rdoc::variableValue)
        .def("variableValue", overload_cast<const std::string&>(
            &Script::variableValue, pybind11::const_), rdoc::variableValue_2)
        .def("variableIndex", &Script::variableIndex, rdoc::variableIndex)
        .def("setVariableName", &Script::setVariableName, rdoc::setVariableName)
        .def("setVariableValue", [](Script& s, size_t index,
                std::shared_ptr<regina::Packet> value) {
            // We need to reimplement this, since Regina's function
            // takes a weak_ptr, not a shared_ptr.
            s.setVariableValue(index, value);
        }, pybind11::arg(), pybind11::arg("value") = nullptr,
            rdoc::setVariableValue)
        .def("addVariable", [](Script& s, const std::string& name,
                std::shared_ptr<regina::Packet> value) {
            // We need to reimplement this, since Regina's function
            // takes a weak_ptr, not a shared_ptr.
            return s.addVariable(name, value);
        }, pybind11::arg(), pybind11::arg("value") = nullptr,
            rdoc::addVariable)
        .def("addVariableName", [](Script& s, const std::string& name,
                std::shared_ptr<regina::Packet> value) {
            // We need to reimplement this, since Regina's function
            // takes a weak_ptr, not a shared_ptr.
            return s.addVariableName(name, value);
        }, pybind11::arg(), pybind11::arg("value") = nullptr,
            rdoc::addVariableName)
        .def("removeVariable",
            overload_cast<const std::string&>(&Script::removeVariable),
            rdoc::removeVariable)
        .def("removeVariable",
            overload_cast<size_t>(&Script::removeVariable),
            rdoc::removeVariable_2)
        .def("removeAllVariables", &Script::removeAllVariables,
            rdoc::removeAllVariables)
        .def_readonly_static("typeID", &Script::typeID)
        .def("listenVariables", &Script::listenVariables, rdoc::listenVariables)
        .def("unlistenVariables", &Script::unlistenVariables,
            rdoc::unlistenVariables)
    ;
    regina::python::add_output(c);
    regina::python::packet_eq_operators(c, rdoc::__eq);

    regina::python::add_global_swap<Script>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

