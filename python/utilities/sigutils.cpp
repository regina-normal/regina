
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "../pybind11/stl.h"
#include "utilities/sigutils.h"
#include "../helpers.h"

using regina::Base64SigEncoding;

void addSigUtils(pybind11::module_& m) {
    auto c = pybind11::class_<Base64SigEncoding>(m, "Base64SigEncoding")
        .def_static("decodeSingle", &Base64SigEncoding::decodeSingle)
        .def_static("encodeSingle", &Base64SigEncoding::encodeSingle)
        .def_static("isValid", &Base64SigEncoding::isValid)
        .def_static("encodeInt", &Base64SigEncoding::encodeInt<long>)
        .def_static("decodeInt", &Base64SigEncoding::decodeInt<long>)
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 0>&>(
            &Base64SigEncoding::encodeTrits<0>))
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 1>&>(
            &Base64SigEncoding::encodeTrits<1>))
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 2>&>(
            &Base64SigEncoding::encodeTrits<2>))
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 3>&>(
            &Base64SigEncoding::encodeTrits<3>))
        // overload_cast cannot handle template vs non-template overloads.
        .def_static("decodeTrits", (std::array<uint8_t, 3> (*)(char))(
            &Base64SigEncoding::decodeTrits))
    ;
    regina::python::no_eq_operators(c);
}

