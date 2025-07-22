
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
#include <pybind11/functional.h>
#include "angle/anglestructures.h"
#include "progress/progresstracker.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/angle/anglestructures.h"

using namespace regina::python;
using regina::AngleStructures;
using regina::ProgressTracker;
using regina::Triangulation;

void addAngleStructures(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    m.def("makeAngleEquations", regina::makeAngleEquations,
        rdoc::makeAngleEquations);

    RDOC_SCOPE_SWITCH(AngleStructures)

    auto l = pybind11::class_<AngleStructures,
            std::shared_ptr<AngleStructures>>(m, "AngleStructures", rdoc_scope)
        .def(pybind11::init<const Triangulation<3>&, bool,
                regina::Flags<regina::AngleAlg>, ProgressTracker*>(),
            pybind11::arg(),
            pybind11::arg("tautOnly") = false,
            pybind11::arg("algHints") = regina::AngleAlg::Default,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<GILScopedRelease>(),
            rdoc::__init)
        .def(pybind11::init<const AngleStructures&>(), rdoc::__copy)
        .def("swap", &AngleStructures::swap, rdoc::swap)
        .def("triangulation", &AngleStructures::triangulation,
            pybind11::return_value_policy::reference_internal,
            rdoc::triangulation)
        .def("isTautOnly", &AngleStructures::isTautOnly, rdoc::isTautOnly)
        .def("algorithm", &AngleStructures::algorithm, rdoc::algorithm)
        .def("size", &AngleStructures::size, rdoc::size)
        .def("__len__", &AngleStructures::size, rdoc::size)
        .def("structure", &AngleStructures::structure,
            pybind11::return_value_policy::reference_internal,
            rdoc::structure)
        .def("__getitem__", &AngleStructures::operator[],
            pybind11::return_value_policy::reference_internal, rdoc::__array)
        .def("__iter__", [](const AngleStructures& list) {
            return pybind11::make_iterator(list);
        }, pybind11::keep_alive<0, 1>(), // iterator keeps list alive
            rdoc::__iter__)
        .def("spansStrict", &AngleStructures::spansStrict, rdoc::spansStrict)
        .def("spansTaut", &AngleStructures::spansTaut, rdoc::spansTaut)
        .def("sort", &AngleStructures::sort<const std::function<
            bool(const regina::AngleStructure&,
                const regina::AngleStructure&)>&>,
            rdoc::sort)
    ;
    regina::python::add_output(l);
    regina::python::packet_eq_operators(l, rdoc::__eq);
    regina::python::add_packet_data(l);

    auto wrap = regina::python::add_packet_wrapper<AngleStructures>(
        m, "PacketOfAngleStructures");
    regina::python::add_packet_constructor<const Triangulation<3>&, bool,
            regina::Flags<regina::AngleAlg>, ProgressTracker*>(wrap,
        pybind11::arg(),
        pybind11::arg("tautOnly") = false,
        pybind11::arg("algHints") = regina::AngleAlg::Default,
        pybind11::arg("tracker") = nullptr,
        pybind11::call_guard<GILScopedRelease>(),
        rdoc::__init);

    regina::python::add_global_swap<AngleStructures>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

