
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
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "surface/normalsurfaces.h"
#include "surface/surfacefilter.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../flags.h"

using namespace regina::python;
using regina::NormalSurfaces;
using regina::ProgressTracker;
using regina::SurfaceFilter;
using regina::Triangulation;

void addNormalSurfaces(pybind11::module_& m) {
    regina::python::add_flags<regina::SurfaceExportFields>(
        m, "SurfaceExportFields", "SurfaceExport", {
            { "surfaceExportName", regina::surfaceExportName },
            { "surfaceExportEuler", regina::surfaceExportEuler },
            { "surfaceExportOrient", regina::surfaceExportOrient },
            { "surfaceExportSides", regina::surfaceExportSides },
            { "surfaceExportBdry", regina::surfaceExportBdry },
            { "surfaceExportLink", regina::surfaceExportLink },
            { "surfaceExportType", regina::surfaceExportType },
            { "surfaceExportNone", regina::surfaceExportNone },
            { "surfaceExportAllButName", regina::surfaceExportAllButName },
            { "surfaceExportAll", regina::surfaceExportAll }
        });

    m.def("makeMatchingEquations", regina::makeMatchingEquations);
    m.def("makeEmbeddedConstraints", regina::makeEmbeddedConstraints);

    auto l = pybind11::class_<NormalSurfaces,
            std::shared_ptr<NormalSurfaces>>(m, "NormalSurfaces")
        .def(pybind11::init<const Triangulation<3>&, regina::NormalCoords,
                regina::NormalList, regina::NormalAlg, ProgressTracker*>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("which") = regina::NS_LIST_DEFAULT,
            pybind11::arg("algHints") = regina::NS_ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<GILScopedRelease>())
        .def(pybind11::init<const NormalSurfaces&, regina::NormalTransform>())
        .def(pybind11::init<const NormalSurfaces&, const SurfaceFilter&>())
        .def(pybind11::init<const NormalSurfaces&>())
        .def("swap", &NormalSurfaces::swap)
        .def("coords", &NormalSurfaces::coords)
        .def("which", &NormalSurfaces::which)
        .def("algorithm", &NormalSurfaces::algorithm)
        .def("allowsAlmostNormal", &NormalSurfaces::allowsAlmostNormal)
        .def("allowsNonCompact", &NormalSurfaces::allowsNonCompact)
        .def("isEmbeddedOnly", &NormalSurfaces::isEmbeddedOnly)
        .def("triangulation", &NormalSurfaces::triangulation,
            pybind11::return_value_policy::reference_internal)
        .def("size", &NormalSurfaces::size)
        .def("__iter__", [](const NormalSurfaces& list) {
            return pybind11::make_iterator(list);
        }, pybind11::keep_alive<0, 1>()) // iterator keeps list alive
        .def("surface", &NormalSurfaces::surface,
            pybind11::return_value_policy::reference_internal)
        .def("sort", &NormalSurfaces::sort<const std::function<
            bool(const regina::NormalSurface&, const regina::NormalSurface&)>&>)
        .def("recreateMatchingEquations",
            &NormalSurfaces::recreateMatchingEquations)
        .def("saveCSVStandard", &NormalSurfaces::saveCSVStandard,
            pybind11::arg(),
            pybind11::arg("additionalFields") = regina::surfaceExportAll)
        .def("saveCSVEdgeWeight", &NormalSurfaces::saveCSVEdgeWeight,
            pybind11::arg(),
            pybind11::arg("additionalFields") = regina::surfaceExportAll)
        .def("vectors", [](const NormalSurfaces& list) {
            return pybind11::make_iterator(
                list.beginVectors(), list.endVectors());
        }, pybind11::keep_alive<0, 1>()) // iterator keeps list alive
    ;
    regina::python::add_output(l);
    regina::python::packet_eq_operators(l);
    regina::python::add_packet_data(l);

    auto wrap = regina::python::add_packet_wrapper<NormalSurfaces>(
        m, "PacketOfNormalSurfaces");
    regina::python::add_packet_constructor<const Triangulation<3>&,
            regina::NormalCoords, regina::NormalList, regina::NormalAlg,
            ProgressTracker*>(wrap,
        pybind11::arg(), pybind11::arg(),
        pybind11::arg("which") = regina::NS_LIST_DEFAULT,
        pybind11::arg("algHints") = regina::NS_ALG_DEFAULT,
        pybind11::arg("tracker") = nullptr,
        pybind11::call_guard<GILScopedRelease>());
    regina::python::add_packet_constructor<const NormalSurfaces&,
        regina::NormalTransform>(wrap);

    m.def("swap", (void(*)(NormalSurfaces&, NormalSurfaces&))(regina::swap));
}

