
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

#include "pythonoutputstream.h"
#include "../pybind11/pybind11.h"
#include "../helpers/gil.h"

namespace regina::python {

void PythonOutputStream::write(const std::string& data) {
    buffer.append(data);

    auto pos = buffer.find('\n');
    if (pos < buffer.length()) {
        if (PyGILState_Check()) {
            GILScopedRelease release;
            do {
                processOutput(buffer.substr(0, pos + 1));
                buffer.erase(0, pos + 1);
                pos = buffer.find('\n');
            } while (pos < buffer.length());
        } else {
            do {
                processOutput(buffer.substr(0, pos + 1));
                buffer.erase(0, pos + 1);
                pos = buffer.find('\n');
            } while (pos < buffer.length());
        }
    }
}

void PythonOutputStream::flush() {
    if (! buffer.empty()) {
        if (PyGILState_Check()) {
            GILScopedRelease release;
            processOutput(buffer);
        } else {
            processOutput(buffer);
        }
        buffer.clear();
    }
}

void PythonOutputStream::addBindings() {
    pybind11::class_<PythonOutputStream>(pybind11::handle(),
            "PythonOutputStream")
        .def("write", &PythonOutputStream::write)
        .def("flush", &PythonOutputStream::flush);
}

void PythonOutputStream::install(const char* streamName) {
    // Create a python wrapper for this custom stream.
    pybind11::object stream = pybind11::cast(this,
        pybind11::return_value_policy::reference);
    if (! stream.ptr())
        throw std::runtime_error(
            std::string("Could not create pybind11 wrapper for sys.")
                + streamName);

    // Redirect the system output stream to our custom stream wrapper.
    if (PySys_SetObject(const_cast<char*>(streamName), stream.ptr())) {
        PyErr_Print();
        PyErr_Clear();
        throw std::runtime_error(
            std::string("Failed to set sys.") + streamName);
    }
}

} // namespace regina::python
