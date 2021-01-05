
/**************************************************************************
 *                                                                        *
 *  Regina - A Hyper Surface Theory Calculator                           *
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
#include "hypersurface/hyperflags.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::HyperAlg;
using regina::HyperAlgFlags;
using regina::HyperList;
using regina::HyperListFlags;

void addHyperFlags(pybind11::module_& m) {
    pybind11::enum_<HyperListFlags>(m, "HyperListFlags")
        .value("HS_EMBEDDED_ONLY", regina::HS_EMBEDDED_ONLY)
        .value("HS_IMMERSED_SINGULAR", regina::HS_IMMERSED_SINGULAR)
        .value("HS_VERTEX", regina::HS_VERTEX)
        .value("HS_FUNDAMENTAL", regina::HS_FUNDAMENTAL)
        .value("HS_LEGACY", regina::HS_LEGACY)
        .value("HS_CUSTOM", regina::HS_CUSTOM)
        .export_values()
        // This __or__ promotes the argument from a flags enum to a
        // "combination of flags" object.  It must come *after* export_values,
        // since it returns a pybind11::class and not a pybind11::enum,
        // which means a subsequent export_values() would fail.
        .def("__or__", [](const HyperListFlags& lhs, const HyperListFlags& rhs){
                return HyperList(lhs) | rhs;})
        ;

    auto l = pybind11::class_<HyperList>(m, "HyperList")
        .def(pybind11::init<>())
        .def(pybind11::init<HyperListFlags>())
        .def(pybind11::init<const HyperList&>())
        .def("has", overload_cast<const HyperList&>(
            &HyperList::has, pybind11::const_))
        .def("intValue", &HyperList::intValue)
        .def_static("fromInt", &HyperList::fromInt)
        .def(pybind11::self |= pybind11::self)
        .def(pybind11::self &= pybind11::self)
        .def(pybind11::self ^= pybind11::self)
        .def(pybind11::self | pybind11::self)
        .def(pybind11::self & pybind11::self)
        .def(pybind11::self ^ pybind11::self)
        .def("clear", overload_cast<const HyperList&>(&HyperList::clear))
        .def("ensureOne",
            overload_cast<HyperListFlags, HyperListFlags>(
            &HyperList::ensureOne))
        .def("ensureOne",
            overload_cast<HyperListFlags, HyperListFlags, HyperListFlags>(
            &HyperList::ensureOne))
        .def("ensureOne",
            overload_cast<HyperListFlags, HyperListFlags, HyperListFlags,
                HyperListFlags>(
            &HyperList::ensureOne))
        ;
    regina::python::add_eq_operators(l);

    pybind11::implicitly_convertible<HyperListFlags, HyperList>();

    pybind11::enum_<HyperAlgFlags>(m, "HyperAlgFlags")
        .value("HS_ALG_DEFAULT", regina::HS_ALG_DEFAULT)
        .value("HS_VERTEX_DD", regina::HS_VERTEX_DD)
        .value("HS_HILBERT_PRIMAL", regina::HS_HILBERT_PRIMAL)
        .value("HS_HILBERT_DUAL", regina::HS_HILBERT_DUAL)
        .value("HS_ALG_LEGACY", regina::HS_ALG_LEGACY)
        .value("HS_ALG_CUSTOM", regina::HS_ALG_CUSTOM)
        .export_values()
        // This __or__ promotes the argument from a flags enum to a
        // "combination of flags" object.  It must come *after* export_values,
        // since it returns a pybind11::class and not a pybind11::enum,
        // which means a subsequent export_values() would fail.
        .def("__or__", [](const HyperAlgFlags& lhs, const HyperAlgFlags& rhs){
                return HyperAlg(lhs) | rhs;})
        ;

    auto a = pybind11::class_<HyperAlg>(m, "HyperAlg")
        .def(pybind11::init<>())
        .def(pybind11::init<HyperAlgFlags>())
        .def(pybind11::init<const HyperAlg&>())
        .def("has", overload_cast<const HyperAlg&>(
            &HyperAlg::has, pybind11::const_))
        .def("intValue", &HyperAlg::intValue)
        .def_static("fromInt", &HyperAlg::fromInt)
        .def(pybind11::self |= pybind11::self)
        .def(pybind11::self &= pybind11::self)
        .def(pybind11::self ^= pybind11::self)
        .def(pybind11::self | pybind11::self)
        .def(pybind11::self & pybind11::self)
        .def(pybind11::self ^ pybind11::self)
        .def("clear", overload_cast<const HyperAlg&>(&HyperAlg::clear))
        .def("ensureOne",
            overload_cast<HyperAlgFlags, HyperAlgFlags>(
            &HyperAlg::ensureOne))
        .def("ensureOne",
            overload_cast<HyperAlgFlags, HyperAlgFlags, HyperAlgFlags>(
            &HyperAlg::ensureOne))
        .def("ensureOne",
            overload_cast<HyperAlgFlags, HyperAlgFlags, HyperAlgFlags,
                HyperAlgFlags>(
            &HyperAlg::ensureOne))
        ;
    regina::python::add_eq_operators(a);

    pybind11::implicitly_convertible<HyperAlgFlags, HyperAlg>();
}
