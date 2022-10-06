
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
#include "maths/integer.h"
#include "utilities/tightencoding.h"

using pybind11::overload_cast;

void addTightEncoding(pybind11::module_& m) {
    // We cannot use overload_cast here because there is a templated
    // global tightEncoding() function.
    m.def("tightEncoding", static_cast<std::string(&)(long)>(
        regina::tightEncoding));
    m.def("tightEncoding", static_cast<std::string(&)(long long)>(
        regina::tightEncoding));
    m.def("tightEncoding", static_cast<std::string(&)(bool)>(
        regina::tightEncoding));
    m.def("tightDecoding", [](const std::string& enc) {
        // Try a native integer conversion first.
        try {
            return pybind11::cast(regina::tightDecoding<long>(enc));
        } catch (const regina::InvalidArgument&) {
            // It could have been out of range.  Try arbitrary precision
            // integers before aborting.
            regina::Integer ans = regina::tightDecoding<regina::Integer>(enc);

            // At this point we have a valid solution, so we should be
            // able to convert to a Python long via strings without trouble.
            return pybind11::reinterpret_steal<pybind11::object>(
                PyLong_FromString(ans.stringValue().c_str(), nullptr, 10));
        }
    });
}

