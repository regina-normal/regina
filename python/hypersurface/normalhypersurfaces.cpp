
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

#include "hypersurface/normalhypersurfaces.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::HyperCoords;
using regina::NormalHypersurfaces;

namespace {
    // Write manual overload wrappers since these are static member functions.
    NormalHypersurfaces* unified_2(regina::Triangulation<4>* owner,
            regina::HyperCoords coords) {
        return NormalHypersurfaces::enumerate(owner, coords);
    }
    NormalHypersurfaces* unified_3(regina::Triangulation<4>* owner,
            regina::HyperCoords coords, regina::HyperList which) {
        return NormalHypersurfaces::enumerate(owner, coords, which);
    }
    NormalHypersurfaces* unified_4(regina::Triangulation<4>* owner,
            regina::HyperCoords coords, regina::HyperList which,
            regina::HyperAlg algHints) {
        return NormalHypersurfaces::enumerate(owner, coords, which,
            algHints);
    }
    NormalHypersurfaces* unified_5(regina::Triangulation<4>* owner,
            regina::HyperCoords coords, regina::HyperList which,
            regina::HyperAlg algHints, regina::ProgressTracker* tracker) {
        return NormalHypersurfaces::enumerate(owner, coords, which,
            algHints, tracker);
    }
}

void addNormalHypersurfaces() {
    def("makeMatchingEquations",
        regina::makeMatchingEquations,
        return_value_policy<manage_new_object>());

    class_<NormalHypersurfaces, bases<regina::Packet>,
            SafeHeldType<NormalHypersurfaces>, boost::noncopyable>
            ("NormalHypersurfaces", no_init)
        .def("enumerate", unified_2,
            return_value_policy<to_held_type<>>())
        .def("enumerate", unified_3,
            return_value_policy<to_held_type<>>())
        .def("enumerate", unified_4,
            return_value_policy<to_held_type<>>())
        .def("enumerate", unified_5,
            return_value_policy<to_held_type<>>())
        .def("recreateMatchingEquations",
            &NormalHypersurfaces::recreateMatchingEquations,
            return_value_policy<manage_new_object>())
        .def("coords", &NormalHypersurfaces::coords)
        .def("which", &NormalHypersurfaces::which)
        .def("algorithm", &NormalHypersurfaces::algorithm)
        .def("isEmbeddedOnly", &NormalHypersurfaces::isEmbeddedOnly)
        .def("triangulation", &NormalHypersurfaces::triangulation,
            return_value_policy<to_held_type<>>())
        .def("size", &NormalHypersurfaces::size)
        .def("hypersurface", &NormalHypersurfaces::hypersurface,
            return_internal_reference<>())
        .staticmethod("enumerate")
        .attr("typeID") = regina::PACKET_NORMALHYPERSURFACES;
    ;

    implicitly_convertible<SafeHeldType<NormalHypersurfaces>,
        SafeHeldType<regina::Packet> >();

    FIX_REGINA_BOOST_CONVERTERS(NormalHypersurfaces);

    scope().attr("NNormalHypersurfaceList") =
        scope().attr("NormalHypersurfaces");
}

