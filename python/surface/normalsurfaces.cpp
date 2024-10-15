
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
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "surface/normalsurfaces.h"
#include "surface/surfacefilter.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../helpers/flags.h"
#include "../docstrings/surface/normalsurfaces.h"

using namespace regina::python;
using regina::NormalSurfaces;
using regina::ProgressTracker;
using regina::SurfaceFilter;
using regina::Triangulation;

void addNormalSurfaces(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SurfaceExport)

    regina::python::add_flags<regina::SurfaceExport>(m, "SurfaceExport", {
            { "Name", regina::SurfaceExport::Name, rdoc::Name },
            { "Euler", regina::SurfaceExport::Euler, rdoc::Euler },
            { "Orient", regina::SurfaceExport::Orient, rdoc::Orient },
            { "Sides", regina::SurfaceExport::Sides, rdoc::Sides },
            { "Bdry", regina::SurfaceExport::Bdry, rdoc::Bdry },
            { "Link", regina::SurfaceExport::Link, rdoc::Link },
            { "Type", regina::SurfaceExport::Type, rdoc::Type },
            // Note: we cannot use "None", since this is reserved in Python.
            { "Nil", regina::SurfaceExport::None, rdoc::None },
            { "AllButName", regina::SurfaceExport::AllButName,
                rdoc::AllButName },
            { "All", regina::SurfaceExport::All, rdoc::All }
        }, rdoc_scope, rdoc_global::__bor);

    RDOC_SCOPE_SWITCH_MAIN

    // Deprecated type alias and constants:
    m.attr("SurfaceExportFields") = m.attr("SurfaceExport");
    m.attr("surfaceExportName") = regina::SurfaceExport::Name;
    m.attr("surfaceExportEuler") = regina::SurfaceExport::Euler;
    m.attr("surfaceExportOrient") = regina::SurfaceExport::Orient;
    m.attr("surfaceExportSides") = regina::SurfaceExport::Sides;
    m.attr("surfaceExportBdry") = regina::SurfaceExport::Bdry;
    m.attr("surfaceExportLink") = regina::SurfaceExport::Link;
    m.attr("surfaceExportType") = regina::SurfaceExport::Type;
    m.attr("surfaceExportNone") = regina::SurfaceExport::None;
    m.attr("surfaceExportAllButName") = regina::SurfaceExport::AllButName;
    m.attr("surfaceExportAll") = regina::SurfaceExport::All;

    m.def("makeMatchingEquations", regina::makeMatchingEquations,
        rdoc::makeMatchingEquations);
    m.def("makeEmbeddedConstraints", regina::makeEmbeddedConstraints,
        rdoc::makeEmbeddedConstraints);

    RDOC_SCOPE_SWITCH(NormalSurfaces)

    auto l = pybind11::class_<NormalSurfaces,
            std::shared_ptr<NormalSurfaces>>(m, "NormalSurfaces", rdoc_scope)
        .def(pybind11::init<const Triangulation<3>&, regina::NormalCoords,
                regina::Flags<regina::NormalList>,
                regina::Flags<regina::NormalAlg>, ProgressTracker*>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("which") = regina::NormalList::Default,
            pybind11::arg("algHints") = regina::NormalAlg::Default,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<GILScopedRelease>(),
            rdoc::__init)
        .def(pybind11::init<const NormalSurfaces&, regina::NormalTransform>(),
            rdoc::__init_2)
        .def(pybind11::init<const NormalSurfaces&, const SurfaceFilter&>(),
            rdoc::__init_3)
        .def(pybind11::init<const NormalSurfaces&>(), rdoc::__copy)
        .def("swap", &NormalSurfaces::swap, rdoc::swap)
        .def("coords", &NormalSurfaces::coords, rdoc::coords)
        .def("which", &NormalSurfaces::which, rdoc::which)
        .def("algorithm", &NormalSurfaces::algorithm, rdoc::algorithm)
        .def("allowsAlmostNormal", &NormalSurfaces::allowsAlmostNormal,
            rdoc::allowsAlmostNormal)
        .def("allowsNonCompact", &NormalSurfaces::allowsNonCompact,
            rdoc::allowsNonCompact)
        .def("isEmbeddedOnly", &NormalSurfaces::isEmbeddedOnly,
            rdoc::isEmbeddedOnly)
        .def("triangulation", &NormalSurfaces::triangulation,
            pybind11::return_value_policy::reference_internal,
            rdoc::triangulation)
        .def("size", &NormalSurfaces::size, rdoc::size)
        .def("__len__", &NormalSurfaces::size, rdoc::size)
        .def("__iter__", [](const NormalSurfaces& list) {
            return pybind11::make_iterator(list);
        }, pybind11::keep_alive<0, 1>(), // iterator keeps list alive
            rdoc::__iter__)
        .def("surface", &NormalSurfaces::surface,
            pybind11::return_value_policy::reference_internal, rdoc::surface)
        .def("__getitem__", &NormalSurfaces::operator[],
            pybind11::return_value_policy::reference_internal, rdoc::__array)
        .def("sort", &NormalSurfaces::sort<const std::function<
            bool(const regina::NormalSurface&, const regina::NormalSurface&)>&>,
            rdoc::sort)
        .def("recreateMatchingEquations",
            &NormalSurfaces::recreateMatchingEquations,
            rdoc::recreateMatchingEquations)
        .def("saveCSVStandard", &NormalSurfaces::saveCSVStandard,
            pybind11::arg(),
            pybind11::arg("additionalFields") = regina::SurfaceExport::All,
            rdoc::saveCSVStandard)
        .def("saveCSVEdgeWeight", &NormalSurfaces::saveCSVEdgeWeight,
            pybind11::arg(),
            pybind11::arg("additionalFields") = regina::SurfaceExport::All,
            rdoc::saveCSVEdgeWeight)
        .def("vectors", [](const NormalSurfaces& list) {
            return pybind11::make_iterator(
                list.beginVectors(), list.endVectors());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps list alive
            rdoc::vectors)
    ;
    regina::python::add_output(l);
    regina::python::packet_eq_operators(l, rdoc::__eq);
    regina::python::add_packet_data(l);

    auto wrap = regina::python::add_packet_wrapper<NormalSurfaces>(
        m, "PacketOfNormalSurfaces");
    regina::python::add_packet_constructor<const Triangulation<3>&,
            regina::NormalCoords, regina::Flags<regina::NormalList>,
            regina::Flags<regina::NormalAlg>, ProgressTracker*>(wrap,
        pybind11::arg(), pybind11::arg(),
        pybind11::arg("which") = regina::NormalList::Default,
        pybind11::arg("algHints") = regina::NormalAlg::Default,
        pybind11::arg("tracker") = nullptr,
        pybind11::call_guard<GILScopedRelease>(),
        rdoc::__init);
    regina::python::add_packet_constructor<const NormalSurfaces&,
        regina::NormalTransform>(wrap, rdoc::__init_2);
    regina::python::add_packet_constructor<const NormalSurfaces&,
        const SurfaceFilter&>(wrap, rdoc::__init_3);

    regina::python::add_global_swap<NormalSurfaces>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

