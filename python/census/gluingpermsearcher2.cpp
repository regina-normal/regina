
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
#include "census/gluingpermsearcher2.h"
#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../docstrings/census/gluingpermsearcher2.h"

using regina::FacetPairing;
using regina::GluingPermSearcher;

void addGluingPermSearcher2(pybind11::module_& m) {
    using Action = const std::function<void(const regina::GluingPerms<2>&)>&;

    RDOC_SCOPE_BEGIN(GluingPermSearcher)

    auto c = pybind11::class_<GluingPermSearcher<2>>(m, "GluingPermSearcher2",
            rdoc_scope)
        .def(pybind11::init<FacetPairing<2>, FacetPairing<2>::IsoList, bool>(),
            rdoc::__init)
        .def("runSearch", &GluingPermSearcher<2>::runSearch<Action>,
            pybind11::arg("action"), rdoc::runSearch)
        .def("partialSearch", &GluingPermSearcher<2>::partialSearch<Action>,
            pybind11::arg("maxDepth"), pybind11::arg("action"),
            rdoc::partialSearch)
        .def("isComplete", &GluingPermSearcher<2>::isComplete, rdoc::isComplete)
        .def("taggedData", &GluingPermSearcher<2>::taggedData, rdoc::taggedData)
        .def("data", &GluingPermSearcher<2>::data, rdoc::data)
        .def_static("findAllPerms",
            &GluingPermSearcher<2>::findAllPerms<Action>,
            pybind11::arg("pairing"), pybind11::arg("autos"),
            pybind11::arg("orientableOnly"), pybind11::arg("action"),
            rdoc::findAllPerms)
        .def_static("bestSearcher", &GluingPermSearcher<2>::bestSearcher,
            rdoc::bestSearcher)
        .def_static("fromTaggedData",
            pybind11::overload_cast<std::string>(
                &GluingPermSearcher<2>::fromTaggedData), rdoc::fromTaggedData)
        .def_readonly_static("dataTag", &GluingPermSearcher<2>::dataTag)
        ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END
}

