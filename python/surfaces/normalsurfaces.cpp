
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
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "utilities/safeptr.h"
#include "../helpers.h"

using regina::NormalSurfaces;

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

    pybind11::class_<NormalSurfaces, regina::Packet,
            regina::SafePtr<NormalSurfaces>>(m, "NormalSurfaces")
        .def("coords", &NormalSurfaces::coords)
        .def("which", &NormalSurfaces::which)
        .def("algorithm", &NormalSurfaces::algorithm)
        .def("allowsAlmostNormal", &NormalSurfaces::allowsAlmostNormal)
        .def("allowsSpun", &NormalSurfaces::allowsSpun)
        .def("allowsOriented", &NormalSurfaces::allowsOriented)
        .def("isEmbeddedOnly", &NormalSurfaces::isEmbeddedOnly)
        .def("triangulation", &NormalSurfaces::triangulation)
        .def("size", &NormalSurfaces::size)
        .def("surface", &NormalSurfaces::surface,
            pybind11::return_value_policy::reference_internal)
        .def("writeAllSurfaces", [](const NormalSurfaces& s) {
            s.writeAllSurfaces(std::cout);
        })
        .def_static("enumerate", &NormalSurfaces::enumerate,
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("which") = regina::NS_LIST_DEFAULT,
            pybind11::arg("algHints") = regina::NS_ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr)
        .def("quadToStandard", &NormalSurfaces::quadToStandard)
        .def("quadOctToStandardAN", &NormalSurfaces::quadOctToStandardAN)
        .def("standardToQuad", &NormalSurfaces::standardToQuad)
        .def("standardANToQuadOct", &NormalSurfaces::standardANToQuadOct)
        .def("filterForLocallyCompatiblePairs",
            &NormalSurfaces::filterForLocallyCompatiblePairs)
        .def("filterForDisjointPairs",
            &NormalSurfaces::filterForDisjointPairs)
        .def("filterForPotentiallyIncompressible",
            &NormalSurfaces::filterForPotentiallyIncompressible)
        .def("recreateMatchingEquations",
            &NormalSurfaces::recreateMatchingEquations)
        .def("saveCSVStandard", &NormalSurfaces::saveCSVStandard,
            pybind11::arg(),
            pybind11::arg("additionalFields") = regina::surfaceExportAll)
        .def("saveCSVEdgeWeight", &NormalSurfaces::saveCSVEdgeWeight,
            pybind11::arg(),
            pybind11::arg("additionalFields") = regina::surfaceExportAll)
        .def_property_readonly_static("typeID", [](pybind11::object) {
            // We cannot take the address of typeID, so use a getter function.
            return NormalSurfaces::typeID;
        })
    ;

    m.attr("NNormalSurfaceList") = m.attr("NormalSurfaces");
}

