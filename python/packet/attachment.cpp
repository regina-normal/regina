
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
#include "packet/attachment.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Attachment;

void addAttachment(pybind11::module_& m) {
    pybind11::class_<Attachment, regina::Packet, std::shared_ptr<Attachment>>(
            m, "Attachment")
        .def(pybind11::init<>())
        .def(pybind11::init<const char*>())
        .def(pybind11::init<const Attachment&>())
        .def(pybind11::init([](pybind11::bytes data, std::string filename) {
            char* in = nullptr;
            Py_ssize_t inlen = 0;

            // The macro PYBIND11_BYTES_AS_STRING_AND_SIZE calls either
            // PyBytes_AsStringAndSize or PyString_AsStringAndSize,
            // according to whether are using Python 3 or 2 respectively.

            if (PYBIND11_BYTES_AS_STRING_AND_SIZE(data.ptr(), &in, &inlen)) {
                // pybind11_fail throws a std::runtime_error.
                // We use this for consistency with other parts of pybind11
                // that work directly with the Python C API.
                pybind11::pybind11_fail("Unable to extract Python "
                    "bytes contents in Attachment constructor");
            }

            return new Attachment(in, inlen, Attachment::DEEP_COPY, filename);
        }))
        .def("swap", &Attachment::swap)
        .def("isNull", &Attachment::isNull)
        .def("data", [](const Attachment& a) -> pybind11::object {
            if (a.isNull())
                return pybind11::none();
            else
                return pybind11::bytes(a.data(), a.size());
        })
        .def("size", &Attachment::size)
        .def("filename", &Attachment::filename)
        .def("extension", &Attachment::extension)
        .def("reset", overload_cast<>(&Attachment::reset))
        .def("reset", [](Attachment& a, pybind11::bytes data,
                std::string filename) {
            char* in = nullptr;
            Py_ssize_t inlen = 0;

            // See the constructor above for an explanation of this code.
            if (PYBIND11_BYTES_AS_STRING_AND_SIZE(data.ptr(), &in, &inlen)) {
                pybind11::pybind11_fail("Unable to extract Python "
                    "bytes contents in Attachment constructor");
            }

            a.reset(in, inlen, Attachment::DEEP_COPY, filename);
        })
        .def("save", &Attachment::save)
        .def("savePDF", &Attachment::save) // deprecated
        .def_readonly_static("typeID", &Attachment::typeID)
    ;

    m.def("swap", (void(*)(Attachment&, Attachment&))(regina::swap));

    m.attr("PDF") = m.attr("Attachment"); // deprecated typedef
}

