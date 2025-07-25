
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
#include "utilities/base64.h"
#include "../helpers/docstrings.h"
#include "../docstrings/utilities/base64.h"

void addBase64(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    m.def("base64Length", regina::base64Length, rdoc::base64Length);
    m.def("isBase64", regina::isBase64, rdoc::isBase64);
    m.attr("base64Table") = regina::base64Table;
    m.attr("base64Spare") = regina::base64Spare;

    m.def("base64Encode", [](pybind11::bytes b) {
        char* in = nullptr;
        Py_ssize_t inlen = 0;

        // The macro PYBIND11_BYTES_AS_STRING_AND_SIZE calls either
        // PyBytes_AsStringAndSize or PyString_AsStringAndSize,
        // according to whether are using Python 3 or 2 respectively.

        if (PYBIND11_BYTES_AS_STRING_AND_SIZE(b.ptr(), &in, &inlen)) {
            // pybind11_fail throws a std::runtime_error.
            // We use this for consistency with other parts of pybind11
            // that work directly with the Python C API.
            pybind11::pybind11_fail(
                "Unable to extract Python bytes contents in base64Encode()");
        }

        size_t outlen = regina::base64Length(inlen);
        char* out = new char[outlen + 1];
        regina::base64Encode(in, inlen, out, outlen);

        pybind11::str ans(out, outlen);
        delete[] out;
        return ans;
    }, pybind11::arg("input_bytes"), rdoc::base64Encode_2);

    m.def("base64Decode", [](const std::string& s) -> pybind11::object {
        char* out;
        size_t outlen;

        if (regina::base64Decode(s.c_str(), s.size(), &out, &outlen)) {
            pybind11::bytes ans(out, outlen);
            delete[] out;
            return std::move(ans);
        } else {
            return pybind11::none();
        }
    }, pybind11::arg("input_string"), rdoc::base64Decode_2);

    RDOC_SCOPE_END
}

