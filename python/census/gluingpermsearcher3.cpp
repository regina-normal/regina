
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
#include "../pybind11/functional.h" // for Action
#include "../pybind11/stl.h" // for IsoList
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
            bool, bool, regina::CensusPurge>())
        .def("runSearch", &GluingPermSearcher<3>::runSearch<Action>)
        .def("partialSearch", &GluingPermSearcher<3>::partialSearch<Action>)
        .def("isComplete", &GluingPermSearcher<3>::isComplete)
        .def("completePermSet", // deprecated
            &GluingPermSearcher<3>::isComplete)
        .def("taggedData", &GluingPermSearcher<3>::taggedData)
        .def("data", &GluingPermSearcher<3>::data)
        .def_static("findAllPerms",
            &GluingPermSearcher<3>::findAllPerms<Action>)
        .def_static("bestSearcher", &GluingPermSearcher<3>::bestSearcher)
        .def_static("fromTaggedData",
            pybind11::overload_cast<const std::string&>(
                &GluingPermSearcher<3>::fromTaggedData))
        .def_readonly_static("dataTag", &GluingPermSearcher<3>::dataTag)
        ;
    regina::python::add_eq_operators(g);

    // For backward compatibility; these inner class constants are deprecated.
    g.attr("PURGE_NONE") = regina::PURGE_NONE;
    g.attr("PURGE_NON_MINIMAL") = regina::PURGE_NON_MINIMAL;
    g.attr("PURGE_NON_PRIME") = regina::PURGE_NON_PRIME;
    g.attr("PURGE_NON_MINIMAL_PRIME") = regina::PURGE_NON_MINIMAL_PRIME;
    g.attr("PURGE_NON_MINIMAL_HYP") = regina::PURGE_NON_MINIMAL_HYP;
    g.attr("PURGE_P2_REDUCIBLE") = regina::PURGE_P2_REDUCIBLE;

    auto e = pybind11::class_<EulerSearcher, GluingPermSearcher<3>>(
            m, "EulerSearcher")
        .def(pybind11::init<int, FacetPairing<3>, FacetPairing<3>::IsoList,
            bool, regina::CensusPurge>())
        .def_readonly_static("dataTag", &EulerSearcher::dataTag)
        ;
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<CompactSearcher, GluingPermSearcher<3>>(
            m, "CompactSearcher")
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList,
            bool, regina::CensusPurge>())
        .def_readonly_static("dataTag", &CompactSearcher::dataTag)
        ;
    regina::python::add_eq_operators(c);

    auto p = pybind11::class_<ClosedPrimeMinSearcher, GluingPermSearcher<3>>(
            m, "ClosedPrimeMinSearcher")
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList, bool>())
        .def_readonly_static("dataTag", &ClosedPrimeMinSearcher::dataTag)
        ;
    regina::python::add_eq_operators(p);

    auto h = pybind11::class_<HyperbolicMinSearcher, GluingPermSearcher<3>>(
            m, "HyperbolicMinSearcher")
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList, bool>())
        .def_readonly_static("dataTag", &HyperbolicMinSearcher::dataTag)
        ;
    regina::python::add_eq_operators(h);
}

