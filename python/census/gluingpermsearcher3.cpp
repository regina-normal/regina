
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

#include <pybind11/pybind11.h>
#include <pybind11/functional.h> // for Action
#include <pybind11/stl.h> // for IsoList
#include "census/gluingpermsearcher3.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/census/gluingpermsearcher3.h"

using regina::ClosedPrimeMinSearcher;
using regina::CompactSearcher;
using regina::EulerSearcher;
using regina::FacetPairing;
using regina::GluingPermSearcher;
using regina::HyperbolicMinSearcher;

void addGluingPermSearcher3(pybind11::module_& m) {
    using Action = const std::function<void(const regina::GluingPerms<3>&)>&;

    RDOC_SCOPE_BEGIN(GluingPermSearcher)

    auto g = pybind11::class_<GluingPermSearcher<3>>(m, "GluingPermSearcher3",
            rdoc_scope)
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList,
            bool, bool, regina::CensusPurge>(), rdoc::__init)
        .def("runSearch", &GluingPermSearcher<3>::runSearch<Action>,
            pybind11::arg("action"), rdoc::runSearch)
        .def("partialSearch", &GluingPermSearcher<3>::partialSearch<Action>,
            pybind11::arg("maxDepth"), pybind11::arg("action"),
            rdoc::partialSearch)
        .def("isComplete", &GluingPermSearcher<3>::isComplete, rdoc::isComplete)
        .def("taggedData", &GluingPermSearcher<3>::taggedData, rdoc::taggedData)
        .def("data", &GluingPermSearcher<3>::data, rdoc::data)
        .def_static("findAllPerms",
            &GluingPermSearcher<3>::findAllPerms<Action>,
            pybind11::arg("pairing"), pybind11::arg("autos"),
            pybind11::arg("orientableOnly"), pybind11::arg("finiteOnly"),
            pybind11::arg("whichPurge"), pybind11::arg("action"),
            rdoc::findAllPerms)
        .def_static("bestSearcher", &GluingPermSearcher<3>::bestSearcher,
            rdoc::bestSearcher)
        .def_static("fromTaggedData",
            pybind11::overload_cast<std::string>(
                &GluingPermSearcher<3>::fromTaggedData), rdoc::fromTaggedData)
        .def_readonly_static("dataTag", &GluingPermSearcher<3>::dataTag)
        ;
    regina::python::add_output(g);
    regina::python::add_eq_operators(g);

    RDOC_SCOPE_SWITCH(EulerSearcher)

    auto e = pybind11::class_<EulerSearcher, GluingPermSearcher<3>>(
            m, "EulerSearcher", rdoc_scope)
        .def(pybind11::init<int, FacetPairing<3>, FacetPairing<3>::IsoList,
            bool, regina::CensusPurge>(), rdoc::__init)
        .def_readonly_static("dataTag", &EulerSearcher::dataTag)
        ;
    // It would be nice for EulerSearcher's output to include the Euler
    // characteristic that we are searching for.  However, GluingPermSearcher<3>
    // is *not* an abstract base class, and so attempting to re-derive from
    // Output<EulerSearcher> would cause headaches with multiple base classes
    // that provide str().  The work-to-benefit ratio is not worth it here;
    // we will just use the more basic inherited output routines from
    // GluingPermSearcher<3> instead.
    regina::python::add_output(e);
    regina::python::add_eq_operators(e);

    RDOC_SCOPE_SWITCH(CompactSearcher)

    auto c = pybind11::class_<CompactSearcher, GluingPermSearcher<3>>(
            m, "CompactSearcher", rdoc_scope)
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList,
            bool, regina::CensusPurge>(), rdoc::__init)
        .def_readonly_static("dataTag", &CompactSearcher::dataTag)
        ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_SWITCH(ClosedPrimeMinSearcher)

    auto p = pybind11::class_<ClosedPrimeMinSearcher, GluingPermSearcher<3>>(
            m, "ClosedPrimeMinSearcher", rdoc_scope)
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList, bool>(),
            rdoc::__init)
        .def_readonly_static("dataTag", &ClosedPrimeMinSearcher::dataTag)
        ;
    regina::python::add_output(p);
    regina::python::add_eq_operators(p);

    RDOC_SCOPE_SWITCH(HyperbolicMinSearcher)

    auto h = pybind11::class_<HyperbolicMinSearcher, GluingPermSearcher<3>>(
            m, "HyperbolicMinSearcher", rdoc_scope)
        .def(pybind11::init<FacetPairing<3>, FacetPairing<3>::IsoList, bool>(),
            rdoc::__init)
        .def_readonly_static("dataTag", &HyperbolicMinSearcher::dataTag)
        ;
    regina::python::add_output(h);
    regina::python::add_eq_operators(h);

    RDOC_SCOPE_END
}

