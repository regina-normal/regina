
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
#include "census/purgeflags.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::CensusPurge;
using regina::CensusPurgeFlags;

void addPurgeFlags(pybind11::module_& m) {
    pybind11::enum_<CensusPurgeFlags>(m, "CensusPurgeFlags")
        .value("PURGE_NONE", regina::PURGE_NONE)
        .value("PURGE_NON_MINIMAL", regina::PURGE_NON_MINIMAL)
        .value("PURGE_NON_PRIME", regina::PURGE_NON_PRIME)
        .value("PURGE_NON_MINIMAL_PRIME", regina::PURGE_NON_MINIMAL_PRIME)
        .value("PURGE_NON_MINIMAL_HYP", regina::PURGE_NON_MINIMAL_HYP)
        .value("PURGE_P2_REDUCIBLE", regina::PURGE_P2_REDUCIBLE)
        .export_values()
        // This __or__ promotes the argument from a flags enum to a
        // "combination of flags" object.  It must come *after* export_values,
        // since it returns a pybind11::class and not a pybind11::enum,
        // which means a subsequent export_values() would fail.
        .def("__or__", [](const CensusPurgeFlags& lhs, const CensusPurgeFlags& rhs){
                return CensusPurge(lhs) | rhs;});

    auto l = pybind11::class_<CensusPurge>(m, "CensusPurge")
        .def(pybind11::init<>())
        .def(pybind11::init<CensusPurgeFlags>())
        .def(pybind11::init<const CensusPurge&>())
        .def("has", overload_cast<const CensusPurge&>(
            &CensusPurge::has, pybind11::const_))
        .def("intValue", &CensusPurge::intValue)
        .def_static("fromInt", &CensusPurge::fromInt)
        .def(pybind11::self |= pybind11::self)
        .def(pybind11::self &= pybind11::self)
        .def(pybind11::self ^= pybind11::self)
        .def(pybind11::self | pybind11::self)
        .def(pybind11::self & pybind11::self)
        .def(pybind11::self ^ pybind11::self)
        .def("clear", overload_cast<const CensusPurge&>(&CensusPurge::clear))
        .def("ensureOne",
            overload_cast<CensusPurgeFlags, CensusPurgeFlags>(
            &CensusPurge::ensureOne))
        .def("ensureOne",
            overload_cast<CensusPurgeFlags, CensusPurgeFlags, CensusPurgeFlags>(
            &CensusPurge::ensureOne))
        .def("ensureOne",
            overload_cast<CensusPurgeFlags, CensusPurgeFlags, CensusPurgeFlags,
                CensusPurgeFlags>(
            &CensusPurge::ensureOne))
        ;
    regina::python::add_eq_operators(l);

    pybind11::implicitly_convertible<CensusPurgeFlags, CensusPurge>();
}
