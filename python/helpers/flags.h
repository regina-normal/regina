
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

/*! \file python/helpers/flags.h
 *  \brief Assists with wrapping Regina's flag constants and related classes.
 *
 *  This header is \e not included automatically by python/helpers.h.
 *  If you need it, you will need to include it yourself.
 */

#include <iomanip>
#include <sstream>
#include "../helpers.h"
#include "../pybind11/operators.h"
#include "utilities/flags.h"
#include "../helpers/docstrings.h"
#include "../docstrings/utilities/flags.h"

namespace regina::python {

/**
 * Adds Python bindings for a flag enumeration type, as well as a
 * full-featured regina::Flags class type built upon it.
 *
 * The argument \c Enum should be a C++ enumeration type, and will be
 * given the Python enum name \a enumName and docstring \a enumDoc.
 * The individual flag constants should be passed in the list \a values:
 * each tuple is of the form (\a python_name, \a real_value, \a docstring).
 * The extra string \a borDoc is the docstring that will be used for the
 * bitwise OR of two individual flags.
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
        std::initializer_list<std::tuple<const char*, Enum, const char*>>
            values,
        const char* enumDoc, const char* borDoc) {
    using Flags = regina::Flags<Enum>;

    auto e = pybind11::enum_<Enum>(m, enumName, enumDoc);
    for (const auto& v : values) {
        // This should be a job for std::apply, except that e.value() is
        // a non-static member function.
        e.value(std::get<0>(v), std::get<1>(v), std::get<2>(v));
    }
    e.export_values();
    e.def("__or__", [](const Enum& lhs, const Enum& rhs) {
        return Flags(lhs) | rhs;
    });

    RDOC_SCOPE_BEGIN(Flags)

    auto f = pybind11::class_<Flags>(m, flagsName, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__init)
        .def(pybind11::init<Enum>(), rdoc::__init_2)
        .def(pybind11::init<const Flags&>(), rdoc::__copy)
        .def("has", pybind11::overload_cast<const Flags&>(
            &Flags::has, pybind11::const_), rdoc::has_2)
        .def("intValue", &Flags::intValue, rdoc::intValue)
        .def_static("fromInt", &Flags::fromInt, rdoc::fromInt)
        .def(pybind11::self |= pybind11::self, rdoc::__ior_2)
        .def(pybind11::self &= pybind11::self, rdoc::__iand_2)
        .def(pybind11::self ^= pybind11::self, rdoc::__ixor_2)
        .def(pybind11::self | pybind11::self, rdoc::__bor_2)
        .def(pybind11::self & pybind11::self, rdoc::__band_2)
        .def(pybind11::self ^ pybind11::self, rdoc::__bxor_2)
        .def("clear", pybind11::overload_cast<const Flags&>(&Flags::clear),
            rdoc::clear_2)
        .def("ensureOne", pybind11::overload_cast<Enum, Enum>(
            &Flags::ensureOne), rdoc::ensureOne)
        .def("ensureOne", pybind11::overload_cast<Enum, Enum, Enum>(
            &Flags::ensureOne), rdoc::ensureOne_2)
        .def("ensureOne", pybind11::overload_cast<Enum, Enum, Enum, Enum>(
            &Flags::ensureOne), rdoc::ensureOne_3)
        .def("__str__", [](Flags f) {
            std::ostringstream out;
            out << "0x" << std::hex << std::setw(hexWidth) << std::setfill('0')
                << f.intValue();
            return out.str();
        })
        .def("__repr__", [](Flags f) {
            std::ostringstream out;
            out << "<regina."
                << pybind11::str(pybind11::type::handle_of<Flags>().attr(
                    "__name__")).cast<std::string_view>()
                << ": 0x" << std::hex << std::setw(hexWidth)
                << std::setfill('0') << f.intValue() << '>';
            return out.str();
        })
        ;
    regina::python::add_eq_operators(f, rdoc::__eq_2, rdoc::__ne_2);

    RDOC_SCOPE_END

    pybind11::implicitly_convertible<Enum, Flags>();
}

} // namespace regina::python

