
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "hypersurface/normalhypersurfaces.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "../helpers.h"

using namespace regina::python;
using regina::HyperCoords;
using regina::NormalHypersurface;
using regina::NormalHypersurfaces;
using regina::ProgressTracker;
using regina::Triangulation;

void addNormalHypersurfaces(pybind11::module_& m) {
    m.def("makeMatchingEquations", regina::makeMatchingEquations);
    m.def("makeEmbeddedConstraints", regina::makeEmbeddedConstraints);

    auto l = pybind11::class_<NormalHypersurfaces,
            std::shared_ptr<NormalHypersurfaces>>(m, "NormalHypersurfaces")
        .def(pybind11::init<const Triangulation<4>&, HyperCoords,
                regina::HyperList, regina::HyperAlg, ProgressTracker*>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("which") = regina::HS_LIST_DEFAULT,
            pybind11::arg("algHints") = regina::HS_ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr)
        .def(pybind11::init<const NormalHypersurfaces&>())
        .def("swap", &NormalHypersurfaces::swap)
        .def_static("enumerate", [](Triangulation<4>& owner,
                HyperCoords coords, regina::HyperList which,
                regina::HyperAlg algHints) {
            // This is deprecated, so we reimplement it here ourselves.
            // This means we can't use the progress tracker variant, which
            // requires threading code internal to NormalHypersurfaces.
            try {
                auto ans = regina::make_packet<NormalHypersurfaces>(
                    std::in_place, owner, coords, which, algHints);
                if (auto p = owner.packet())
                    p->insertChildLast(ans);
                return ans;
            } catch (const regina::ReginaException&) {
                return std::shared_ptr<regina::PacketOf<NormalHypersurfaces>>();
            }
        }, pybind11::arg(), pybind11::arg(),
            pybind11::arg("which") = regina::HS_LIST_DEFAULT,
            pybind11::arg("algHints") = regina::HS_ALG_DEFAULT)
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
        .def("__iter__", [](const NormalHypersurfaces& list) {
            return pybind11::make_iterator(list);
        }, pybind11::keep_alive<0, 1>()) // iterator keeps list alive
        .def("vectors", [](const NormalHypersurfaces& list) {
            return pybind11::make_iterator(
                list.beginVectors(), list.endVectors());
        }, pybind11::keep_alive<0, 1>()) // iterator keeps list alive
    ;
    regina::python::add_output(l);
    regina::python::add_eq_operators(l);

    auto wrap = regina::python::add_packet_wrapper<NormalHypersurfaces>(
        m, "PacketOfNormalHypersurfaces");
    regina::python::add_packet_constructor<const Triangulation<4>&, HyperCoords,
            regina::HyperList, regina::HyperAlg, ProgressTracker*>(wrap,
        pybind11::arg(), pybind11::arg(),
        pybind11::arg("which") = regina::HS_LIST_DEFAULT,
        pybind11::arg("algHints") = regina::HS_ALG_DEFAULT,
        pybind11::arg("tracker") = nullptr);

    m.def("swap",
        (void(*)(NormalHypersurfaces&, NormalHypersurfaces&))(regina::swap));
}

