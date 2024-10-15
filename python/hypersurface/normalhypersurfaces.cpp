
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
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "hypersurface/normalhypersurfaces.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../docstrings/hypersurface/normalhypersurfaces.h"

using namespace regina::python;
using regina::HyperCoords;
using regina::NormalHypersurface;
using regina::NormalHypersurfaces;
using regina::ProgressTracker;
using regina::Triangulation;

void addNormalHypersurfaces(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    m.def("makeMatchingEquations", regina::makeMatchingEquations);
    m.def("makeEmbeddedConstraints", regina::makeEmbeddedConstraints);

    RDOC_SCOPE_SWITCH(NormalHypersurfaces)

    auto l = pybind11::class_<NormalHypersurfaces,
            std::shared_ptr<NormalHypersurfaces>>(m, "NormalHypersurfaces",
            rdoc_scope)
        .def(pybind11::init<const Triangulation<4>&, HyperCoords,
                regina::Flags<regina::HyperList>,
                regina::Flags<regina::HyperAlg>, ProgressTracker*>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("which") = regina::HyperList::Default,
            pybind11::arg("algHints") = regina::HyperAlg::Default,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<GILScopedRelease>(),
            rdoc::__init)
        .def(pybind11::init<const NormalHypersurfaces&>(), rdoc::__copy)
        .def("swap", &NormalHypersurfaces::swap, rdoc::swap)
        .def("sort", &NormalHypersurfaces::sort<const std::function<
            bool(const NormalHypersurface&, const NormalHypersurface&)>&>,
            rdoc::sort)
        .def("recreateMatchingEquations",
            &NormalHypersurfaces::recreateMatchingEquations,
            rdoc::recreateMatchingEquations)
        .def("coords", &NormalHypersurfaces::coords, rdoc::coords)
        .def("which", &NormalHypersurfaces::which, rdoc::which)
        .def("algorithm", &NormalHypersurfaces::algorithm, rdoc::algorithm)
        .def("allowsNonCompact", &NormalHypersurfaces::allowsNonCompact,
            rdoc::allowsNonCompact)
        .def("isEmbeddedOnly", &NormalHypersurfaces::isEmbeddedOnly,
            rdoc::isEmbeddedOnly)
        .def("triangulation", &NormalHypersurfaces::triangulation,
            pybind11::return_value_policy::reference_internal,
            rdoc::triangulation)
        .def("size", &NormalHypersurfaces::size, rdoc::size)
        .def("__len__", &NormalHypersurfaces::size, rdoc::size)
        .def("hypersurface", &NormalHypersurfaces::hypersurface,
            pybind11::return_value_policy::reference_internal,
            rdoc::hypersurface)
        .def("__getitem__", &NormalHypersurfaces::operator[],
            pybind11::return_value_policy::reference_internal, rdoc::__array)
        .def("__iter__", [](const NormalHypersurfaces& list) {
            return pybind11::make_iterator(list);
        }, pybind11::keep_alive<0, 1>(), // iterator keeps list alive
            rdoc::__iter__)
        .def("vectors", [](const NormalHypersurfaces& list) {
            return pybind11::make_iterator(
                list.beginVectors(), list.endVectors());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps list alive
            rdoc::vectors)
    ;
    regina::python::add_output(l);
    regina::python::packet_eq_operators(l, rdoc::__eq);
    regina::python::add_packet_data(l);

    auto wrap = regina::python::add_packet_wrapper<NormalHypersurfaces>(
        m, "PacketOfNormalHypersurfaces");
    regina::python::add_packet_constructor<const Triangulation<4>&, HyperCoords,
            regina::Flags<regina::HyperList>, regina::Flags<regina::HyperAlg>,
            ProgressTracker*>(wrap,
        pybind11::arg(), pybind11::arg(),
        pybind11::arg("which") = regina::HyperList::Default,
        pybind11::arg("algHints") = regina::HyperAlg::Default,
        pybind11::arg("tracker") = nullptr,
        pybind11::call_guard<GILScopedRelease>(),
        rdoc::__init);

    regina::python::add_global_swap<NormalHypersurfaces>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

