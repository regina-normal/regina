
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
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
            std::shared_ptr<NormalHypersurfaces>>(m, "NormalHypersurfaces")
        .def(pybind11::init<const Triangulation<4>&, HyperCoords,
                regina::HyperList, regina::HyperAlg, ProgressTracker*>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("which") = regina::HS_LIST_DEFAULT,
            pybind11::arg("algHints") = regina::HS_ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<GILScopedRelease>(),
            rdoc::NormalHypersurfaces)
        .def(pybind11::init<const NormalHypersurfaces&>(),
            rdoc::NormalHypersurfaces_2)
        .def("swap", &NormalHypersurfaces::swap)
        .def("sort", &NormalHypersurfaces::sort<const std::function<
            bool(const NormalHypersurface&, const NormalHypersurface&)>&>)
        .def("recreateMatchingEquations",
            &NormalHypersurfaces::recreateMatchingEquations)
        .def("coords", &NormalHypersurfaces::coords)
        .def("which", &NormalHypersurfaces::which)
        .def("algorithm", &NormalHypersurfaces::algorithm)
        .def("isEmbeddedOnly", &NormalHypersurfaces::isEmbeddedOnly)
        .def("triangulation", &NormalHypersurfaces::triangulation,
            pybind11::return_value_policy::reference_internal)
        .def("size", &NormalHypersurfaces::size)
        .def("hypersurface", &NormalHypersurfaces::hypersurface,
            pybind11::return_value_policy::reference_internal)
        .def("__getitem__", &NormalHypersurfaces::operator[],
            pybind11::return_value_policy::reference_internal)
        .def("__iter__", [](const NormalHypersurfaces& list) {
            return pybind11::make_iterator(list);
        }, pybind11::keep_alive<0, 1>()) // iterator keeps list alive
        .def("vectors", [](const NormalHypersurfaces& list) {
            return pybind11::make_iterator(
                list.beginVectors(), list.endVectors());
        }, pybind11::keep_alive<0, 1>()) // iterator keeps list alive
    ;
    regina::python::add_output(l);
    regina::python::packet_eq_operators(l);
    regina::python::add_packet_data(l);

    auto wrap = regina::python::add_packet_wrapper<NormalHypersurfaces>(
        m, "PacketOfNormalHypersurfaces");
    regina::python::add_packet_constructor<const Triangulation<4>&, HyperCoords,
            regina::HyperList, regina::HyperAlg, ProgressTracker*>(wrap,
        nullptr /* docstring */,
        pybind11::arg(), pybind11::arg(),
        pybind11::arg("which") = regina::HS_LIST_DEFAULT,
        pybind11::arg("algHints") = regina::HS_ALG_DEFAULT,
        pybind11::arg("tracker") = nullptr,
        pybind11::call_guard<GILScopedRelease>(),
        rdoc::NormalHypersurfaces);

    regina::python::add_global_swap<NormalHypersurfaces>(m);

    RDOC_SCOPE_END
}

