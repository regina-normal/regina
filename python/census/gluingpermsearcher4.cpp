
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
#include "census/gluingpermsearcher4.h"
#include "triangulation/dim4.h"
#include "../helpers.h"

using regina::FacetPairing;
using regina::GluingPermSearcher;

void addGluingPermSearcher4(pybind11::module_& m) {
    using Action = const std::function<void(const regina::GluingPerms<4>&)>&;

    auto c = pybind11::class_<GluingPermSearcher<4>>(m, "GluingPermSearcher4")
        .def(pybind11::init<FacetPairing<4>, FacetPairing<4>::IsoList,
            bool, bool, Action>())
        .def("runSearch", &GluingPermSearcher<4>::runSearch,
            pybind11::arg("maxDepth") = -1)
        .def("completePermSet", &GluingPermSearcher<4>::completePermSet)
        .def("taggedData", &GluingPermSearcher<4>::taggedData)
        .def("data", &GluingPermSearcher<4>::data)
        .def_static("findAllPerms",
            &GluingPermSearcher<4>::findAllPerms<Action>)
        .def_static("bestSearcher",
            &GluingPermSearcher<4>::bestSearcher<Action>)
        .def_static("readTaggedData",
            pybind11::overload_cast<const std::string&, Action>(
                &GluingPermSearcher<4>::readTaggedData<Action>))
        .def_readonly_static("dataTag_", &GluingPermSearcher<4>::dataTag_)
        ;
    regina::python::add_eq_operators(c);
}

