
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

#include "dim4/dim4triangulation.h"
#include "hypersurface/nnormalhypersurfacelist.h"
#include "maths/nmatrixint.h"
#include "progress/nprogresstracker.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::HyperCoords;
using regina::NNormalHypersurfaceList;

namespace {
    // Write manual overload wrappers since these are static member functions.
    NNormalHypersurfaceList* unified_2(regina::Dim4Triangulation* owner,
            regina::HyperCoords coords) {
        return NNormalHypersurfaceList::enumerate(owner, coords);
    }
    NNormalHypersurfaceList* unified_3(regina::Dim4Triangulation* owner,
            regina::HyperCoords coords, regina::HyperList which) {
        return NNormalHypersurfaceList::enumerate(owner, coords, which);
    }
    NNormalHypersurfaceList* unified_4(regina::Dim4Triangulation* owner,
            regina::HyperCoords coords, regina::HyperList which,
            regina::HyperAlg algHints) {
        return NNormalHypersurfaceList::enumerate(owner, coords, which,
            algHints);
    }
    NNormalHypersurfaceList* unified_5(regina::Dim4Triangulation* owner,
            regina::HyperCoords coords, regina::HyperList which,
            regina::HyperAlg algHints, regina::NProgressTracker* tracker) {
        return NNormalHypersurfaceList::enumerate(owner, coords, which,
            algHints, tracker);
    }
}

void addNNormalHypersurfaceList() {
    def("makeMatchingEquations",
        regina::makeMatchingEquations,
        return_value_policy<manage_new_object>());

    scope s = class_<NNormalHypersurfaceList, bases<regina::NPacket>,
            SafeHeldType<NNormalHypersurfaceList>, boost::noncopyable>
            ("NNormalHypersurfaceList", no_init)
        .def("enumerate", unified_2,
            return_value_policy<to_held_type<>>())
        .def("enumerate", unified_3,
            return_value_policy<to_held_type<>>())
        .def("enumerate", unified_4,
            return_value_policy<to_held_type<>>())
        .def("enumerate", unified_5,
            return_value_policy<to_held_type<>>())
        .def("recreateMatchingEquations",
            &NNormalHypersurfaceList::recreateMatchingEquations,
            return_value_policy<manage_new_object>())
        .def("coords", &NNormalHypersurfaceList::coords)
        .def("which", &NNormalHypersurfaceList::which)
        .def("algorithm", &NNormalHypersurfaceList::algorithm)
        .def("isEmbeddedOnly", &NNormalHypersurfaceList::isEmbeddedOnly)
        .def("triangulation", &NNormalHypersurfaceList::triangulation,
            return_value_policy<to_held_type<>>())
        .def("size", &NNormalHypersurfaceList::size)
        .def("hypersurface", &NNormalHypersurfaceList::hypersurface,
            return_internal_reference<>())
        .staticmethod("enumerate")
    ;

    s.attr("typeID") = regina::PACKET_NORMALHYPERSURFACELIST;
    s.attr("packetType") = regina::PACKET_NORMALHYPERSURFACELIST;

    implicitly_convertible<SafeHeldType<NNormalHypersurfaceList>,
        SafeHeldType<regina::NPacket> >();

    FIX_REGINA_BOOST_CONVERTERS(NNormalHypersurfaceList);
}

