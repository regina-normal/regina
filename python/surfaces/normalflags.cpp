
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
#include "surfaces/normalflags.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::NormalAlg;
using regina::NormalAlgFlags;
using regina::NormalList;
using regina::NormalListFlags;

void addNormalFlags(pybind11::module_& m) {
    pybind11::enum_<NormalListFlags>(m, "NormalListFlags")
        .value("NS_EMBEDDED_ONLY", regina::NS_EMBEDDED_ONLY)
        .value("NS_IMMERSED_SINGULAR", regina::NS_IMMERSED_SINGULAR)
        .value("NS_VERTEX", regina::NS_VERTEX)
        .value("NS_FUNDAMENTAL", regina::NS_FUNDAMENTAL)
        .value("NS_LEGACY", regina::NS_LEGACY)
        .value("NS_CUSTOM", regina::NS_CUSTOM)
        .export_values()
        // This __or__ promotes the argument from a flags enum to a
        // "combination of flags" object.  It must come *after* export_values,
        // since it returns a pybind11::class and not a pybind11::enum,
        // which means a subsequent export_values() would fail.
        .def("__or__", [](const NormalListFlags& lhs, const NormalListFlags& rhs){
                return NormalList(lhs) | rhs;});

    auto l = pybind11::class_<NormalList>(m, "NormalList")
        .def(pybind11::init<>())
        .def(pybind11::init<NormalListFlags>())
        .def(pybind11::init<const NormalList&>())
        .def("has", overload_cast<const NormalList&>(
            &NormalList::has, pybind11::const_))
        .def("intValue", &NormalList::intValue)
        .def_static("fromInt", &NormalList::fromInt)
        .def(pybind11::self |= pybind11::self)
        .def(pybind11::self &= pybind11::self)
        .def(pybind11::self ^= pybind11::self)
        .def(pybind11::self | pybind11::self)
        .def(pybind11::self & pybind11::self)
        .def(pybind11::self ^ pybind11::self)
        .def("clear", overload_cast<const NormalList&>(&NormalList::clear))
        .def("ensureOne",
            overload_cast<NormalListFlags, NormalListFlags>(
            &NormalList::ensureOne))
        .def("ensureOne",
            overload_cast<NormalListFlags, NormalListFlags, NormalListFlags>(
            &NormalList::ensureOne))
        .def("ensureOne",
            overload_cast<NormalListFlags, NormalListFlags, NormalListFlags,
                NormalListFlags>(
            &NormalList::ensureOne))
        ;
    regina::python::add_eq_operators(l);

    pybind11::implicitly_convertible<NormalListFlags, NormalList>();

    pybind11::enum_<NormalAlgFlags>(m, "NormalAlgFlags")
        .value("NS_ALG_DEFAULT", regina::NS_ALG_DEFAULT)
        .value("NS_VERTEX_VIA_REDUCED", regina::NS_VERTEX_VIA_REDUCED)
        .value("NS_VERTEX_STD_DIRECT", regina::NS_VERTEX_STD_DIRECT)
        .value("NS_VERTEX_TREE", regina::NS_VERTEX_TREE)
        .value("NS_VERTEX_DD", regina::NS_VERTEX_DD)
        .value("NS_HILBERT_PRIMAL", regina::NS_HILBERT_PRIMAL)
        .value("NS_HILBERT_DUAL", regina::NS_HILBERT_DUAL)
        .value("NS_HILBERT_CD", regina::NS_HILBERT_CD)
        .value("NS_HILBERT_FULLCONE", regina::NS_HILBERT_FULLCONE)
        .value("NS_ALG_LEGACY", regina::NS_ALG_LEGACY)
        .value("NS_ALG_CUSTOM", regina::NS_ALG_CUSTOM)
        .export_values()
        // This __or__ promotes the argument from a flags enum to a
        // "combination of flags" object.  It must come *after* export_values,
        // since it returns a pybind11::class and not a pybind11::enum,
        // which means a subsequent export_values() would fail.
        .def("__or__", [](const NormalAlgFlags& lhs, const NormalAlgFlags& rhs){
                return NormalAlg(lhs) | rhs;});

    auto a = pybind11::class_<NormalAlg>(m, "NormalAlg")
        .def(pybind11::init<>())
        .def(pybind11::init<NormalAlgFlags>())
        .def(pybind11::init<const NormalAlg&>())
        .def("has", overload_cast<const NormalAlg&>(
            &NormalAlg::has, pybind11::const_))
        .def("intValue", &NormalAlg::intValue)
        .def_static("fromInt", &NormalAlg::fromInt)
        .def(pybind11::self |= pybind11::self)
        .def(pybind11::self &= pybind11::self)
        .def(pybind11::self ^= pybind11::self)
        .def(pybind11::self | pybind11::self)
        .def(pybind11::self & pybind11::self)
        .def(pybind11::self ^ pybind11::self)
        .def("clear", overload_cast<const NormalAlg&>(&NormalAlg::clear))
        .def("ensureOne",
            overload_cast<NormalAlgFlags, NormalAlgFlags>(
            &NormalAlg::ensureOne))
        .def("ensureOne",
            overload_cast<NormalAlgFlags, NormalAlgFlags, NormalAlgFlags>(
            &NormalAlg::ensureOne))
        .def("ensureOne",
            overload_cast<NormalAlgFlags, NormalAlgFlags, NormalAlgFlags,
                NormalAlgFlags>(
            &NormalAlg::ensureOne))
        ;
    regina::python::add_eq_operators(a);

    pybind11::implicitly_convertible<NormalAlgFlags, NormalAlg>();
}
