
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
#include "census/gluingpermsearcher3.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::ClosedPrimeMinSearcher;
using regina::CompactSearcher;
using regina::EulerSearcher;
using regina::FacetPairing;
using regina::GluingPermSearcher;
using regina::HyperbolicMinSearcher;

void addGluingPermSearcher3(pybind11::module_& m) {
    using Action = const std::function<void(const regina::GluingPerms<3>&)>&;

    auto g = pybind11::class_<GluingPermSearcher<3>>(m, "GluingPermSearcher3")
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList,
            bool, bool, int, Action>())
        .def("runSearch", &GluingPermSearcher<3>::runSearch,
            pybind11::arg("maxDepth") = -1)
        .def("completePermSet", &GluingPermSearcher<3>::completePermSet)
        .def("taggedData", &GluingPermSearcher<3>::taggedData)
        .def("data", &GluingPermSearcher<3>::data)
        .def_static("findAllPerms",
            &GluingPermSearcher<3>::findAllPerms<Action>)
        .def_static("bestSearcher",
            &GluingPermSearcher<3>::bestSearcher<Action>)
        .def_static("readTaggedData",
            pybind11::overload_cast<const std::string&, Action>(
                &GluingPermSearcher<3>::readTaggedData<Action>))
        .def_readonly_static("dataTag_", &GluingPermSearcher<3>::dataTag_)
        ;
    regina::python::add_eq_operators(g);

    pybind11::enum_<regina::GluingPermSearcher<3>::PurgeFlags>(g, "PurgeFlags")
        .value("PURGE_NONE",
            regina::GluingPermSearcher<3>::PURGE_NONE)
        .value("PURGE_NON_MINIMAL",
            regina::GluingPermSearcher<3>::PURGE_NON_MINIMAL)
        .value("PURGE_NON_PRIME",
            regina::GluingPermSearcher<3>::PURGE_NON_PRIME)
        .value("PURGE_NON_MINIMAL_PRIME",
            regina::GluingPermSearcher<3>::PURGE_NON_MINIMAL_PRIME)
        .value("PURGE_NON_MINIMAL_HYP",
            regina::GluingPermSearcher<3>::PURGE_NON_MINIMAL_HYP)
        .value("PURGE_P2_REDUCIBLE",
            regina::GluingPermSearcher<3>::PURGE_P2_REDUCIBLE)
        .export_values();

    // For backward compatibility with the old boost.python bindings:
    m.attr("PURGE_NONE") =
        GluingPermSearcher<3>::PURGE_NONE;
    m.attr("PURGE_NON_MINIMAL") =
        GluingPermSearcher<3>::PURGE_NON_MINIMAL;
    m.attr("PURGE_NON_PRIME") =
        GluingPermSearcher<3>::PURGE_NON_PRIME;
    m.attr("PURGE_NON_MINIMAL_PRIME") =
        GluingPermSearcher<3>::PURGE_NON_MINIMAL_PRIME;
    m.attr("PURGE_NON_MINIMAL_HYP") =
        GluingPermSearcher<3>::PURGE_NON_MINIMAL_HYP;
    m.attr("PURGE_P2_REDUCIBLE") =
        GluingPermSearcher<3>::PURGE_P2_REDUCIBLE;

    auto e = pybind11::class_<EulerSearcher, GluingPermSearcher<3>>(
            m, "EulerSearcher")
        .def(pybind11::init<int, FacetPairing<3>, FacetPairing<3>::IsoList,
            bool, int, Action>())
        .def_readonly_static("dataTag_", &EulerSearcher::dataTag_)
        ;
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<CompactSearcher, GluingPermSearcher<3>>(
            m, "CompactSearcher")
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList,
            bool, int, Action>())
        .def_readonly_static("dataTag_", &CompactSearcher::dataTag_)
        ;
    regina::python::add_eq_operators(c);

    auto p = pybind11::class_<ClosedPrimeMinSearcher, GluingPermSearcher<3>>(
            m, "ClosedPrimeMinSearcher")
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList,
            bool, Action>())
        .def_readonly_static("dataTag_", &ClosedPrimeMinSearcher::dataTag_)
        ;
    regina::python::add_eq_operators(p);

    auto h = pybind11::class_<HyperbolicMinSearcher, GluingPermSearcher<3>>(
            m, "HyperbolicMinSearcher")
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList,
            bool, Action>())
        .def_readonly_static("dataTag_", &HyperbolicMinSearcher::dataTag_)
        ;
    regina::python::add_eq_operators(h);
}

