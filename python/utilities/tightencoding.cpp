
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include "maths/integer.h"
#include "utilities/tightencoding.h"
#include "../helpers/docstrings.h"
#include "../docstrings/utilities/tightencoding.h"

using pybind11::overload_cast;

// Docstrings that are generated once but need to be reused across many
// source files:
namespace regina::python::doc::common {
    const char* TightEncodable_encoding = TightEncodable_::tightEncoding;
    const char* TightEncodable_decoding = TightEncodable_::tightDecoding;
    const char* TightEncodable_hash = TightEncodable_::hash;
}

void addTightEncoding(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    // We cannot use overload_cast here because there is a templated
    // global tightEncoding() function.
    m.def("tightEncoding", static_cast<std::string(&)(long)>(
        regina::tightEncoding), rdoc::tightEncoding);
    m.def("tightEncoding", [](pybind11::int_ val) {
        // Go via regina's Integer class if we are given an integer argument
        // that does not fit into a native C++ long.
        return regina::Integer(pybind11::cast<std::string>(pybind11::str(val))).
            tightEncoding();
    }, rdoc::tightEncoding);
    m.def("tightEncoding", static_cast<std::string(&)(bool)>(
        regina::tightEncoding), rdoc::tightEncoding_2);
    m.def("tightDecoding", [](const std::string& enc) {
        // Try a native integer conversion first.
        try {
            return pybind11::cast(regina::tightDecoding<long>(enc));
        } catch (const regina::InvalidArgument&) {
            // The result could have been outside the range of a native C++
            // long.  Try arbitrary precision integers before aborting.
            regina::Integer ans = regina::tightDecoding<regina::Integer>(enc);

            // At this point we have a valid solution, so we should be
            // able to convert to a Python long via strings without trouble.
            return pybind11::reinterpret_steal<pybind11::object>(
                PyLong_FromString(ans.stringValue().c_str(), nullptr, 10));
        }
    }, rdoc::tightDecoding);

    RDOC_SCOPE_END
}

