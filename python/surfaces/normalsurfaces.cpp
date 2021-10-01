
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
#include "../pybind11/stl.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "surfaces/normalsurfaces.h"
#include "surfaces/surfacefilter.h"
#include "triangulation/dim3.h"
#include "utilities/safeptr.h"
#include "../helpers.h"

using namespace regina::python;
using regina::NormalSurfaces;
using regina::ProgressTracker;
using regina::SurfaceFilter;
using regina::Triangulation;

void addNormalSurfaces(pybind11::module_& m) {
    pybind11::enum_<regina::SurfaceExportFields>(m, "SurfaceExportFields")
        .value("surfaceExportName", regina::surfaceExportName)
        .value("surfaceExportEuler", regina::surfaceExportEuler)
        .value("surfaceExportOrient", regina::surfaceExportOrient)
        .value("surfaceExportSides", regina::surfaceExportSides)
        .value("surfaceExportBdry", regina::surfaceExportBdry)
        .value("surfaceExportLink", regina::surfaceExportLink)
        .value("surfaceExportType", regina::surfaceExportType)
        .value("surfaceExportNone", regina::surfaceExportNone)
        .value("surfaceExportAllButName", regina::surfaceExportAllButName)
        .value("surfaceExportAll", regina::surfaceExportAll)
        .export_values()
    ;

    m.def("makeMatchingEquations", regina::makeMatchingEquations);

    SafeIterator<NormalSurfaces>::addBindings(m, "NormalSurfaceIterator");

    auto l = pybind11::class_<NormalSurfaces>(m, "NormalSurfaces")
        .def(pybind11::init<const Triangulation<3>&, regina::NormalCoords,
                regina::NormalList, regina::NormalAlg, ProgressTracker*>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("which") = regina::NS_LIST_DEFAULT,
            pybind11::arg("algHints") = regina::NS_ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr)
        .def(pybind11::init<const NormalSurfaces&, regina::NormalTransform>())
        .def(pybind11::init<const NormalSurfaces&>())
        .def("swap", &NormalSurfaces::swap)
        .def("coords", &NormalSurfaces::coords)
        .def("which", &NormalSurfaces::which)
        .def("algorithm", &NormalSurfaces::algorithm)
        .def("allowsAlmostNormal", &NormalSurfaces::allowsAlmostNormal)
        .def("allowsNonCompact", &NormalSurfaces::allowsNonCompact)
        .def("allowsSpun", &NormalSurfaces::allowsNonCompact) // deprecated
        .def("isEmbeddedOnly", &NormalSurfaces::isEmbeddedOnly)
        .def("triangulation", &NormalSurfaces::triangulation,
            pybind11::return_value_policy::reference_internal)
        .def("size", &NormalSurfaces::size)
        .def("__iter__", [](const NormalSurfaces& list) {
            return SafeIterator(list);
        })
        .def("surface", &NormalSurfaces::surface,
            pybind11::return_value_policy::reference_internal)
        .def("writeAllSurfaces", [](const NormalSurfaces& s) {
            s.writeAllSurfaces(std::cout);
        })
        .def_static("enumerate", [](Triangulation<3>& owner,
                regina::NormalCoords coords, regina::NormalList which,
                regina::NormalAlg algHints) -> NormalSurfaces* {
            // This is deprecated, so we reimplement it here ourselves.
            // This means we can't use the progress tracker variant, which
            // requires threading code internal to the NormalSurfaces class.
            try {
                if (regina::Packet* p = owner.packet()) {
                    auto ans = new regina::PacketOf<NormalSurfaces>(
                        std::in_place, owner, coords, which, algHints);
                    p->insertChildLast(ans);
                    return ans;
                } else
                    return new NormalSurfaces(owner, coords, which, algHints);
            } catch (const regina::NoMatchingEquations&) {
                return nullptr;
            }
        }, pybind11::arg(), pybind11::arg(),
            pybind11::arg("which") = regina::NS_LIST_DEFAULT,
            pybind11::arg("algHints") = regina::NS_ALG_DEFAULT)
        .def("quadToStandard", [](const NormalSurfaces& src)
                -> NormalSurfaces* {
            // This is deprecated, so we reimplement it here ourselves.
            const regina::Packet* p = src.packet();
            regina::Packet* parent = (p ? p->parent() : nullptr);
            try {
                if (parent) {
                    auto ans = new regina::PacketOf<NormalSurfaces>(
                        std::in_place, src, regina::NS_CONV_REDUCED_TO_STD);
                    parent->insertChildLast(ans);
                    return ans;
                } else
                    return new NormalSurfaces(src,
                        regina::NS_CONV_REDUCED_TO_STD);
            } catch (const regina::FailedPrecondition&) {
                return nullptr;
            }
        })
        .def("quadOctToStandardAN", [](const NormalSurfaces& src)
                -> NormalSurfaces* {
            // This is deprecated, so we reimplement it here ourselves.
            const regina::Packet* p = src.packet();
            regina::Packet* parent = (p ? p->parent() : nullptr);
            try {
                if (parent) {
                    auto ans = new regina::PacketOf<NormalSurfaces>(
                        std::in_place, src, regina::NS_CONV_REDUCED_TO_STD);
                    parent->insertChildLast(ans);
                    return ans;
                } else
                    return new NormalSurfaces(src,
                        regina::NS_CONV_REDUCED_TO_STD);
            } catch (const regina::FailedPrecondition&) {
                return nullptr;
            }
        })
        .def("standardToQuad", [](const NormalSurfaces& src)
                -> NormalSurfaces* {
            // This is deprecated, so we reimplement it here ourselves.
            const regina::Packet* p = src.packet();
            regina::Packet* parent = (p ? p->parent() : nullptr);
            try {
                if (parent) {
                    auto ans = new regina::PacketOf<NormalSurfaces>(
                        std::in_place, src, regina::NS_CONV_STD_TO_REDUCED);
                    parent->insertChildLast(ans);
                    return ans;
                } else
                    return new NormalSurfaces(src,
                        regina::NS_CONV_STD_TO_REDUCED);
            } catch (const regina::FailedPrecondition&) {
                return nullptr;
            }
        })
        .def("standardANToQuadOct", [](const NormalSurfaces& src)
                -> NormalSurfaces* {
            // This is deprecated, so we reimplement it here ourselves.
            const regina::Packet* p = src.packet();
            regina::Packet* parent = (p ? p->parent() : nullptr);
            try {
                if (parent) {
                    auto ans = new regina::PacketOf<NormalSurfaces>(
                        std::in_place, src, regina::NS_CONV_STD_TO_REDUCED);
                    parent->insertChildLast(ans);
                    return ans;
                } else
                    return new NormalSurfaces(src,
                        regina::NS_CONV_STD_TO_REDUCED);
            } catch (const regina::FailedPrecondition&) {
                return nullptr;
            }
        })
        .def("filter", [](const NormalSurfaces& src, const SurfaceFilter& f)
                -> NormalSurfaces* {
            // This is deprecated, so we reimplement it here ourselves.
            const regina::Packet* p = src.packet();
            regina::Packet* parent = (p ? p->parent() : nullptr);
            if (parent) {
                auto ans = new regina::PacketOf<NormalSurfaces>(
                    std::in_place, src, f);
                parent->insertChildLast(ans);
                return ans;
            } else
                return new NormalSurfaces(src, f);
        })
        .def("filterForLocallyCompatiblePairs", [](const NormalSurfaces& src)
                -> NormalSurfaces* {
            // This is deprecated, so we reimplement it here ourselves.
            const regina::Packet* p = src.packet();
            regina::Packet* parent = (p ? p->parent() : nullptr);
            try {
                if (parent) {
                    auto ans = new regina::PacketOf<NormalSurfaces>(
                        std::in_place, src, regina::NS_FILTER_COMPATIBLE);
                    parent->insertChildLast(ans);
                    return ans;
                } else
                    return new NormalSurfaces(src,
                        regina::NS_FILTER_COMPATIBLE);
            } catch (const regina::FailedPrecondition&) {
                return nullptr;
            }
        })
        .def("filterForDisjointPairs", [](const NormalSurfaces& src)
                -> NormalSurfaces* {
            // This is deprecated, so we reimplement it here ourselves.
            const regina::Packet* p = src.packet();
            regina::Packet* parent = (p ? p->parent() : nullptr);
            try {
                if (parent) {
                    auto ans = new regina::PacketOf<NormalSurfaces>(
                        std::in_place, src, regina::NS_FILTER_DISJOINT);
                    parent->insertChildLast(ans);
                    return ans;
                } else
                    return new NormalSurfaces(src, regina::NS_FILTER_DISJOINT);
            } catch (const regina::FailedPrecondition&) {
                return nullptr;
            }
        })
        .def("filterForPotentiallyIncompressible", [](const NormalSurfaces& src)
                -> NormalSurfaces* {
            // This is deprecated, so we reimplement it here ourselves.
            const regina::Packet* p = src.packet();
            regina::Packet* parent = (p ? p->parent() : nullptr);
            try {
                if (parent) {
                    auto ans = new regina::PacketOf<NormalSurfaces>(
                        std::in_place, src, regina::NS_FILTER_INCOMPRESSIBLE);
                    parent->insertChildLast(ans);
                    return ans;
                } else
                    return new NormalSurfaces(src,
                        regina::NS_FILTER_INCOMPRESSIBLE);
            } catch (const regina::FailedPrecondition&) {
                return nullptr;
            }
        })
        .def("recreateMatchingEquations",
            &NormalSurfaces::recreateMatchingEquations)
        .def("saveCSVStandard", &NormalSurfaces::saveCSVStandard,
            pybind11::arg(),
            pybind11::arg("additionalFields") = regina::surfaceExportAll)
        .def("saveCSVEdgeWeight", &NormalSurfaces::saveCSVEdgeWeight,
            pybind11::arg(),
            pybind11::arg("additionalFields") = regina::surfaceExportAll)
    ;
    regina::python::add_output(l);
    regina::python::add_eq_operators(l);

    auto wrap = regina::python::add_packet_wrapper<NormalSurfaces>(
        m, "PacketOfNormalSurfaces");
    regina::python::add_packet_constructor<const Triangulation<3>&,
            regina::NormalCoords, regina::NormalList, regina::NormalAlg,
            ProgressTracker*>(wrap,
        pybind11::arg(), pybind11::arg(),
        pybind11::arg("which") = regina::NS_LIST_DEFAULT,
        pybind11::arg("algHints") = regina::NS_ALG_DEFAULT,
        pybind11::arg("tracker") = nullptr);
    regina::python::add_packet_constructor<const NormalSurfaces&,
        regina::NormalTransform>(wrap);

    m.def("swap", (void(*)(NormalSurfaces&, NormalSurfaces&))(regina::swap));
}

