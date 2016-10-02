
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

#include "maths/nmatrixint.h"
#include "progress/progresstracker.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::NNormalSurfaceList;

namespace {
    void writeAllSurfaces_stdio(const NNormalSurfaceList& s) {
        s.writeAllSurfaces(std::cout);
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_saveCSVStandard,
        NNormalSurfaceList::saveCSVStandard, 1, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_saveCSVEdgeWeight,
        NNormalSurfaceList::saveCSVEdgeWeight, 1, 2);

    // Write manual overload wrappers since these are static member functions.
    NNormalSurfaceList* unified_2(regina::NTriangulation* owner,
            regina::NormalCoords coords) {
        return NNormalSurfaceList::enumerate(owner, coords);
    }
    NNormalSurfaceList* unified_3(regina::NTriangulation* owner,
            regina::NormalCoords coords, regina::NormalList which) {
        return NNormalSurfaceList::enumerate(owner, coords, which);
    }
    NNormalSurfaceList* unified_4(regina::NTriangulation* owner,
            regina::NormalCoords coords, regina::NormalList which,
            regina::NormalAlg algHints) {
        return NNormalSurfaceList::enumerate(owner, coords, which, algHints);
    }
    NNormalSurfaceList* unified_5(regina::NTriangulation* owner,
            regina::NormalCoords coords, regina::NormalList which,
            regina::NormalAlg algHints, regina::ProgressTracker* tracker) {
        return NNormalSurfaceList::enumerate(owner, coords, which, algHints,
            tracker);
    }
}

void addNNormalSurfaceList() {
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

    scope s = class_<NNormalSurfaceList,
            bases<regina::NPacket>,
            SafeHeldType<NNormalSurfaceList>, boost::noncopyable>
            ("NNormalSurfaceList", no_init)
        .def("coords", &NNormalSurfaceList::coords)
        .def("which", &NNormalSurfaceList::which)
        .def("algorithm", &NNormalSurfaceList::algorithm)
        .def("allowsAlmostNormal", &NNormalSurfaceList::allowsAlmostNormal)
        .def("allowsSpun", &NNormalSurfaceList::allowsSpun)
        .def("allowsOriented", &NNormalSurfaceList::allowsOriented)
        .def("isEmbeddedOnly", &NNormalSurfaceList::isEmbeddedOnly)
        .def("triangulation", &NNormalSurfaceList::triangulation,
            return_value_policy<to_held_type<> >())
        .def("size", &NNormalSurfaceList::size)
        .def("surface", &NNormalSurfaceList::surface,
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
        .def("quadToStandard", &NNormalSurfaceList::quadToStandard,
            return_value_policy<to_held_type<> >())
        .def("quadOctToStandardAN", &NNormalSurfaceList::quadOctToStandardAN,
            return_value_policy<to_held_type<> >())
        .def("standardToQuad", &NNormalSurfaceList::standardToQuad,
            return_value_policy<to_held_type<> >())
        .def("standardANToQuadOct", &NNormalSurfaceList::standardANToQuadOct,
            return_value_policy<to_held_type<> >())
        .def("filterForLocallyCompatiblePairs",
            &NNormalSurfaceList::filterForLocallyCompatiblePairs,
            return_value_policy<to_held_type<> >())
        .def("filterForDisjointPairs",
            &NNormalSurfaceList::filterForDisjointPairs,
            return_value_policy<to_held_type<> >())
        .def("filterForPotentiallyIncompressible",
            &NNormalSurfaceList::filterForPotentiallyIncompressible,
            return_value_policy<to_held_type<> >())
        .def("recreateMatchingEquations",
            &NNormalSurfaceList::recreateMatchingEquations,
            return_value_policy<manage_new_object>())
        .def("saveCSVStandard", &NNormalSurfaceList::saveCSVStandard,
            OL_saveCSVStandard())
        .def("saveCSVEdgeWeight", &NNormalSurfaceList::saveCSVEdgeWeight,
            OL_saveCSVEdgeWeight())
        .staticmethod("enumerate")
    ;

    s.attr("typeID") = regina::PACKET_NORMALSURFACELIST;

    implicitly_convertible<SafeHeldType<NNormalSurfaceList>,
        SafeHeldType<regina::NPacket> >();

    FIX_REGINA_BOOST_CONVERTERS(NNormalSurfaceList);
}

