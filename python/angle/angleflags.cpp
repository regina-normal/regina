
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
#include "../pybind11/operators.h"
#include "angle/angleflags.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::AngleAlg;
using regina::AngleAlgFlags;

void addAngleFlags(pybind11::module_& m) {
    pybind11::enum_<AngleAlgFlags>(m, "AngleAlgFlags")
        .value("AS_ALG_DEFAULT", regina::AS_ALG_DEFAULT)
        .value("AS_ALG_TREE", regina::AS_ALG_TREE)
        .value("AS_ALG_DD", regina::AS_ALG_DD)
        .value("AS_ALG_LEGACY", regina::AS_ALG_LEGACY)
        .value("AS_ALG_CUSTOM", regina::AS_ALG_CUSTOM)
        .export_values()
        // This __or__ promotes the argument from a flags enum to a
        // "combination of flags" object.  It must come *after* export_values,
        // since it returns a pybind11::class and not a pybind11::enum,
        // which means a subsequent export_values() would fail.
        .def("__or__", [](const AngleAlgFlags& lhs, const AngleAlgFlags& rhs){
                return AngleAlg(lhs) | rhs;});

    auto a = pybind11::class_<AngleAlg>(m, "AngleAlg")
        .def(pybind11::init<>())
        .def(pybind11::init<AngleAlgFlags>())
        .def(pybind11::init<const AngleAlg&>())
        .def("has", overload_cast<const AngleAlg&>(
            &AngleAlg::has, pybind11::const_))
        .def("intValue", &AngleAlg::intValue)
        .def_static("fromInt", &AngleAlg::fromInt)
        .def(pybind11::self |= pybind11::self)
        .def(pybind11::self &= pybind11::self)
        .def(pybind11::self ^= pybind11::self)
        .def(pybind11::self | pybind11::self)
        .def(pybind11::self & pybind11::self)
        .def(pybind11::self ^ pybind11::self)
        .def("clear", overload_cast<const AngleAlg&>(&AngleAlg::clear))
        .def("ensureOne",
            overload_cast<AngleAlgFlags, AngleAlgFlags>(
            &AngleAlg::ensureOne))
        .def("ensureOne",
            overload_cast<AngleAlgFlags, AngleAlgFlags, AngleAlgFlags>(
            &AngleAlg::ensureOne))
        .def("ensureOne",
            overload_cast<AngleAlgFlags, AngleAlgFlags, AngleAlgFlags,
                AngleAlgFlags>(
            &AngleAlg::ensureOne))
        ;
    regina::python::add_eq_operators(a);

    pybind11::implicitly_convertible<AngleAlgFlags, AngleAlg>();
}
