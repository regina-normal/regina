
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::NormalSurfaces;

namespace {
    void writeAllSurfaces_stdio(const NormalSurfaces& s) {
        s.writeAllSurfaces(std::cout);
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_saveCSVStandard,
        NormalSurfaces::saveCSVStandard, 1, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_saveCSVEdgeWeight,
        NormalSurfaces::saveCSVEdgeWeight, 1, 2);

    // Write manual overload wrappers since these are static member functions.
    NormalSurfaces* unified_2(regina::Triangulation<3>* owner,
            regina::NormalCoords coords) {
        return NormalSurfaces::enumerate(owner, coords);
    }
    NormalSurfaces* unified_3(regina::Triangulation<3>* owner,
            regina::NormalCoords coords, regina::NormalList which) {
        return NormalSurfaces::enumerate(owner, coords, which);
    }
    NormalSurfaces* unified_4(regina::Triangulation<3>* owner,
            regina::NormalCoords coords, regina::NormalList which,
            regina::NormalAlg algHints) {
        return NormalSurfaces::enumerate(owner, coords, which, algHints);
    }
    NormalSurfaces* unified_5(regina::Triangulation<3>* owner,
            regina::NormalCoords coords, regina::NormalList which,
            regina::NormalAlg algHints, regina::ProgressTracker* tracker) {
        return NormalSurfaces::enumerate(owner, coords, which, algHints,
            tracker);
    }
}

void addNormalSurfaces() {
    enum_<regina::SurfaceExportFields>("SurfaceExportFields")
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
    ;

    def("makeMatchingEquations",
        regina::makeMatchingEquations,
        return_value_policy<manage_new_object>());

    class_<NormalSurfaces, bases<regina::Packet>,
            SafeHeldType<NormalSurfaces>, boost::noncopyable>
            ("NormalSurfaces", no_init)
        .def("coords", &NormalSurfaces::coords)
        .def("which", &NormalSurfaces::which)
        .def("algorithm", &NormalSurfaces::algorithm)
        .def("allowsAlmostNormal", &NormalSurfaces::allowsAlmostNormal)
        .def("allowsSpun", &NormalSurfaces::allowsSpun)
        .def("allowsOriented", &NormalSurfaces::allowsOriented)
        .def("isEmbeddedOnly", &NormalSurfaces::isEmbeddedOnly)
        .def("triangulation", &NormalSurfaces::triangulation,
            return_value_policy<to_held_type<> >())
        .def("size", &NormalSurfaces::size)
        .def("surface", &NormalSurfaces::surface,
            return_internal_reference<>())
        .def("writeAllSurfaces", writeAllSurfaces_stdio)
        .def("enumerate", unified_2,
            return_value_policy<to_held_type<> >())
        .def("enumerate", unified_3,
            return_value_policy<to_held_type<> >())
        .def("enumerate", unified_4,
            return_value_policy<to_held_type<> >())
        .def("enumerate", unified_5,
            return_value_policy<to_held_type<> >())
        .def("quadToStandard", &NormalSurfaces::quadToStandard,
            return_value_policy<to_held_type<> >())
        .def("quadOctToStandardAN", &NormalSurfaces::quadOctToStandardAN,
            return_value_policy<to_held_type<> >())
        .def("standardToQuad", &NormalSurfaces::standardToQuad,
            return_value_policy<to_held_type<> >())
        .def("standardANToQuadOct", &NormalSurfaces::standardANToQuadOct,
            return_value_policy<to_held_type<> >())
        .def("filterForLocallyCompatiblePairs",
            &NormalSurfaces::filterForLocallyCompatiblePairs,
            return_value_policy<to_held_type<> >())
        .def("filterForDisjointPairs",
            &NormalSurfaces::filterForDisjointPairs,
            return_value_policy<to_held_type<> >())
        .def("filterForPotentiallyIncompressible",
            &NormalSurfaces::filterForPotentiallyIncompressible,
            return_value_policy<to_held_type<> >())
        .def("recreateMatchingEquations",
            &NormalSurfaces::recreateMatchingEquations,
            return_value_policy<manage_new_object>())
        .def("saveCSVStandard", &NormalSurfaces::saveCSVStandard,
            OL_saveCSVStandard())
        .def("saveCSVEdgeWeight", &NormalSurfaces::saveCSVEdgeWeight,
            OL_saveCSVEdgeWeight())
        .staticmethod("enumerate")
        .attr("typeID") = regina::PACKET_NORMALSURFACES;
    ;

    implicitly_convertible<SafeHeldType<NormalSurfaces>,
        SafeHeldType<regina::Packet> >();

    FIX_REGINA_BOOST_CONVERTERS(NormalSurfaces);

    scope().attr("NNormalSurfaceList") = scope().attr("NormalSurfaces");
}

