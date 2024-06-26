
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

#include "../pybind11/pybind11.h"
#include "../pybind11/stl.h"
#include "utilities/sigutils.h"
#include "../helpers.h"
#include "../docstrings/utilities/sigutils.h"

using regina::Base64SigDecoder;
using regina::Base64SigEncoder;
using regina::Base64SigEncoding;

void addSigUtils(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Base64SigEncoding)

    auto c = pybind11::class_<Base64SigEncoding>(m, "Base64SigEncoding",
            rdoc_scope)
        .def_static("decodeSingle", &Base64SigEncoding::decodeSingle,
            rdoc::decodeSingle)
        .def_static("encodeSingle", &Base64SigEncoding::encodeSingle,
            rdoc::encodeSingle)
        .def_static("isValid", &Base64SigEncoding::isValid, rdoc::isValid)
        .def_static("encodeInt", &Base64SigEncoding::encodeInt<long>,
            rdoc::encodeInt)
        .def_static("decodeInt", &Base64SigEncoding::decodeInt<long>,
            rdoc::decodeInt)
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 0>&>(
                &Base64SigEncoding::encodeTrits<0>),
            rdoc::encodeTrits)
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 1>&>(
                &Base64SigEncoding::encodeTrits<1>),
            rdoc::encodeTrits)
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 2>&>(
                &Base64SigEncoding::encodeTrits<2>),
            rdoc::encodeTrits)
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 3>&>(
                &Base64SigEncoding::encodeTrits<3>),
            rdoc::encodeTrits)
        // overload_cast cannot handle template vs non-template overloads.
        .def_static("decodeTrits", static_cast<std::array<uint8_t, 3>(&)(char)>(
                Base64SigEncoding::decodeTrits),
            rdoc::decodeTrits)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_SWITCH(Base64SigEncoder)

    auto e = pybind11::class_<Base64SigEncoder>(m, "Base64SigEncoder",
            rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def("str",
            static_cast<const std::string&(Base64SigEncoder::*)() const &>(
                &Base64SigEncoder::str),
            rdoc::str)
        .def("encodeSingle", &Base64SigEncoder::encodeSingle<long>,
            rdoc::encodeSingle)
        .def("encodeSize", &Base64SigEncoder::encodeSize<long>,
            rdoc::encodeSize)
        .def("encodeInt", &Base64SigEncoder::encodeInt<long>, rdoc::encodeInt)
        .def("encodeInts",
            [](Base64SigEncoder& enc, const std::vector<long>& v, int nChars) {
                enc.encodeInts(v.begin(), v.end(), nChars);
            }, rdoc::encodeInts)
        .def("encodeTrits",
            [](Base64SigEncoder& enc, const std::vector<uint8_t>& v) {
                enc.encodeTrits(v.begin(), v.end());
            }, rdoc::encodeTrits)
    ;
    regina::python::add_eq_operators(e);

    RDOC_SCOPE_SWITCH(Base64SigDecoder)

    auto d = pybind11::class_<Base64SigDecoder>(m, "Base64SigDecoder",
            rdoc_scope)
        .def(pybind11::init<const std::string&, bool>(),
            pybind11::arg(), pybind11::arg("skipInitialWhitespace") = true,
            rdoc::__init)
        .def("skipWhitespace", &Base64SigDecoder::skipWhitespace,
            rdoc::skipWhitespace)
        .def("done", &Base64SigDecoder::done,
            pybind11::arg("ignoreWhitespace") = true, rdoc::done)
        .def("decodeSingle", &Base64SigDecoder::decodeSingle<long>,
            rdoc::decodeSingle)
        .def("decodeSize", &Base64SigDecoder::decodeSize<long>,
            rdoc::decodeSize)
        .def("decodeInt", &Base64SigDecoder::decodeInt<long>, rdoc::decodeInt)
        // overload_cast cannot handle template vs non-template overloads.
        .def("decodeTrits",
            static_cast<std::array<uint8_t, 3>(Base64SigDecoder::*)()>(
                &Base64SigDecoder::decodeTrits),
            rdoc::decodeTrits)
        .def_static("isValid", &Base64SigDecoder::isValid, rdoc::isValid)
    ;
    regina::python::add_eq_operators(d);

    RDOC_SCOPE_END
}

