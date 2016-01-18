
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include <boost/python.hpp>
#include "dim4/dim4triangulation.h"
#include "hypersurface/nnormalhypersurfacelist.h"
#include "maths/nmatrixint.h"
#include "progress/nprogresstracker.h"

using namespace boost::python;
using regina::HyperCoords;
using regina::NNormalHypersurfaceList;

namespace {
    // Write manual overload wrappers since these are static member functions.
    NNormalHypersurfaceList* enumerate_2(regina::Dim4Triangulation* owner,
            HyperCoords coords) {
        return NNormalHypersurfaceList::enumerate(owner, coords);
    }
    NNormalHypersurfaceList* enumerate_3(regina::Dim4Triangulation* owner,
            HyperCoords coords, bool embedded) {
        return NNormalHypersurfaceList::enumerate(owner, coords, embedded);
    }
    NNormalHypersurfaceList* enumerate_4(regina::Dim4Triangulation* owner,
            HyperCoords coords, bool embedded,
            regina::NProgressTracker* tracker) {
        return NNormalHypersurfaceList::enumerate(owner, coords, embedded,
            tracker);
    }

    NNormalHypersurfaceList* enumerateFundPrimal_2(
            regina::Dim4Triangulation* owner, HyperCoords coords) {
        return NNormalHypersurfaceList::enumerateFundPrimal(owner, coords);
    }
    NNormalHypersurfaceList* enumerateFundPrimal_3(
            regina::Dim4Triangulation* owner, HyperCoords coords,
            bool embedded) {
        return NNormalHypersurfaceList::enumerateFundPrimal(owner, coords,
            embedded);
    }
    NNormalHypersurfaceList* enumerateFundPrimal_4(
            regina::Dim4Triangulation* owner, HyperCoords coords,
            bool embedded, regina::NNormalHypersurfaceList* vtxSurfaces) {
        return NNormalHypersurfaceList::enumerateFundPrimal(owner, coords,
            embedded, vtxSurfaces);
    }
    NNormalHypersurfaceList* enumerateFundPrimal_5(
            regina::Dim4Triangulation* owner, HyperCoords coords,
            bool embedded, regina::NNormalHypersurfaceList* vtxSurfaces,
            regina::NProgressTracker* tracker) {
        return NNormalHypersurfaceList::enumerateFundPrimal(owner, coords,
            embedded, vtxSurfaces, tracker);
    }

    NNormalHypersurfaceList* enumerateFundDual_2(
            regina::Dim4Triangulation* owner, HyperCoords coords) {
        return NNormalHypersurfaceList::enumerateFundDual(owner, coords);
    }
    NNormalHypersurfaceList* enumerateFundDual_3(
            regina::Dim4Triangulation* owner, HyperCoords coords,
            bool embedded) {
        return NNormalHypersurfaceList::enumerateFundDual(owner, coords,
            embedded);
    }
    NNormalHypersurfaceList* enumerateFundDual_4(
            regina::Dim4Triangulation* owner, HyperCoords coords,
            bool embedded, regina::NProgressTracker* tracker) {
        return NNormalHypersurfaceList::enumerateFundDual(owner, coords,
            embedded, tracker);
    }
}

void addNNormalHypersurfaceList() {
    def("makeMatchingEquations",
        regina::makeMatchingEquations,
        return_value_policy<manage_new_object>());

    scope s = class_<NNormalHypersurfaceList, bases<regina::NPacket>,
            std::auto_ptr<NNormalHypersurfaceList>, boost::noncopyable>
            ("NNormalHypersurfaceList", no_init)
        .def("enumerate", enumerate_2,
            return_value_policy<reference_existing_object>())
        .def("enumerate", enumerate_3,
            return_value_policy<reference_existing_object>())
        .def("enumerate", enumerate_4,
            return_value_policy<reference_existing_object>())
        .def("enumerateFundPrimal", enumerateFundPrimal_2,
            return_value_policy<reference_existing_object>())
        .def("enumerateFundPrimal", enumerateFundPrimal_3,
            return_value_policy<reference_existing_object>())
        .def("enumerateFundPrimal", enumerateFundPrimal_4,
            return_value_policy<reference_existing_object>())
        .def("enumerateFundPrimal", enumerateFundPrimal_5,
            return_value_policy<reference_existing_object>())
        .def("enumerateFundDual", enumerateFundDual_2,
            return_value_policy<reference_existing_object>())
        .def("enumerateFundDual", enumerateFundDual_3,
            return_value_policy<reference_existing_object>())
        .def("enumerateFundDual", enumerateFundDual_4,
            return_value_policy<reference_existing_object>())
        .def("recreateMatchingEquations",
            &NNormalHypersurfaceList::recreateMatchingEquations,
            return_value_policy<manage_new_object>())
        .def("coords", &NNormalHypersurfaceList::coords)
        .def("isEmbeddedOnly", &NNormalHypersurfaceList::isEmbeddedOnly)
        .def("triangulation", &NNormalHypersurfaceList::triangulation,
            return_value_policy<reference_existing_object>())
        .def("size", &NNormalHypersurfaceList::size)
        .def("hypersurface", &NNormalHypersurfaceList::hypersurface,
            return_internal_reference<>())
        .staticmethod("enumerate")
        .staticmethod("enumerateFundPrimal")
        .staticmethod("enumerateFundDual")
    ;

    s.attr("packetType") =
        regina::PacketType(NNormalHypersurfaceList::packetType);

    implicitly_convertible<std::auto_ptr<NNormalHypersurfaceList>,
        std::auto_ptr<regina::NPacket> >();
}

