
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/functional.h> // for Action
#include <pybind11/stl.h> // for IsoList
#include "census/gluingpermsearcher4.h"
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../docstrings/census/gluingpermsearcher4.h"

using regina::FacetPairing;
using regina::GluingPermSearcher;

void addGluingPermSearcher4(pybind11::module_& m) {
    using Action = const std::function<void(const regina::GluingPerms<4>&)>&;

    RDOC_SCOPE_BEGIN(GluingPermSearcher)

    auto c = pybind11::class_<GluingPermSearcher<4>>(m, "GluingPermSearcher4",
            rdoc_scope)
        .def(pybind11::init<FacetPairing<4>, FacetPairing<4>::IsoList,
            bool, bool>(), rdoc::__init)
        .def("runSearch", &GluingPermSearcher<4>::runSearch<Action>,
            pybind11::arg("action"), rdoc::runSearch)
        .def("partialSearch", &GluingPermSearcher<4>::partialSearch<Action>,
            pybind11::arg("maxDepth"), pybind11::arg("action"),
            rdoc::partialSearch)
        .def("isComplete", &GluingPermSearcher<4>::isComplete, rdoc::isComplete)
        .def("taggedData", &GluingPermSearcher<4>::taggedData, rdoc::taggedData)
        .def("data", &GluingPermSearcher<4>::data, rdoc::data)
        .def_static("findAllPerms",
            &GluingPermSearcher<4>::findAllPerms<Action>,
            pybind11::arg("pairing"), pybind11::arg("autos"),
            pybind11::arg("orientableOnly"), pybind11::arg("finiteOnly"),
            pybind11::arg("action"),
            rdoc::findAllPerms)
        .def_static("bestSearcher", &GluingPermSearcher<4>::bestSearcher,
            rdoc::bestSearcher)
        .def_static("fromTaggedData",
            pybind11::overload_cast<std::string>(
                &GluingPermSearcher<4>::fromTaggedData), rdoc::fromTaggedData)
        .def_readonly_static("dataTag", &GluingPermSearcher<4>::dataTag)
        ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END
}

