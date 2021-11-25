
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

/*! \file python/flags.h
 *  \brief Assists with wrapping Regina's flag constants and related classes.
 */

#include <iomanip>
#include <sstream>
#include "utilities/flags.h"
#include "pybind11/operators.h"

namespace regina::python {

/**
 * Adds Python bindings for a flag enumeration type, as well as a
 * full-featured regina::Flags class type built upon it.
 *
 * The argument \c Enum should be a C++ enumeration type, and will be
 * given the Python enum name \a enumName.  The individual flag constants
 * should be passed in the list \a values: each pair is of the form
 * (\a python_name, \a real_value).
 *
 * The corresponding flags class is assumed to be regina::Flags<Enum>,
 * and will be given the Python class name \a flagsName.
 *
 * This routine will define __str__ and __repr__ functions for the flags class,
 * so that users can easily see what value a combination of flags holds.  The
 * flag will be written in hexadecimal, using a minimum of \a hexWidth digits.
 */
template <typename Enum, int hexWidth = 4>
void add_flags(pybind11::module_& m,
        const char* enumName, const char* flagsName,
        std::initializer_list<std::pair<const char*, Enum>> values) {
    using Flags = regina::Flags<Enum>;

    auto e = pybind11::enum_<Enum>(m, enumName);
    for (const auto& v : values)
        e.value(v.first, v.second);
    e.export_values();
    e.def("__or__", [](const Enum& lhs, const Enum& rhs) {
        return Flags(lhs) | rhs;
    });

    auto f = pybind11::class_<Flags>(m, flagsName)
        .def(pybind11::init<>())
        .def(pybind11::init<Enum>())
        .def(pybind11::init<const Flags&>())
        .def("has", pybind11::overload_cast<const Flags&>(
            &Flags::has, pybind11::const_))
        .def("intValue", &Flags::intValue)
        .def_static("fromInt", &Flags::fromInt)
        .def(pybind11::self |= pybind11::self)
        .def(pybind11::self &= pybind11::self)
        .def(pybind11::self ^= pybind11::self)
        .def(pybind11::self | pybind11::self)
        .def(pybind11::self & pybind11::self)
        .def(pybind11::self ^ pybind11::self)
        .def("clear", pybind11::overload_cast<const Flags&>(&Flags::clear))
        .def("ensureOne", pybind11::overload_cast<Enum, Enum>(
            &Flags::ensureOne))
        .def("ensureOne", pybind11::overload_cast<Enum, Enum, Enum>(
            &Flags::ensureOne))
        .def("ensureOne", pybind11::overload_cast<Enum, Enum, Enum, Enum>(
            &Flags::ensureOne))
        ;
    regina::python::add_eq_operators(f);

    auto repr = [](Flags f) {
        std::ostringstream out;
        out << "0x" << std::hex << std::setw(hexWidth) << std::setfill('0')
            << f.intValue();
        return out.str();
    };
    f.def("__str__", repr);
    f.def("__repr__", repr);

    pybind11::implicitly_convertible<Enum, Flags>();
}

} // namespace regina::python

