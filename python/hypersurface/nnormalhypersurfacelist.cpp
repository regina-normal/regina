
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include "dim4/dim4triangulation.h"
#include "hypersurface/nnormalhypersurfacelist.h"
#include "maths/nmatrixint.h"
#include "progress/nprogressmanager.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NNormalHypersurfaceList;

namespace {
    // Write manual overload wrappers since this is a static member function.
    NNormalHypersurfaceList* enumerate_2(regina::Dim4Triangulation* owner,
            int flavour) {
        return NNormalHypersurfaceList::enumerate(owner, flavour);
    }
    NNormalHypersurfaceList* enumerate_3(regina::Dim4Triangulation* owner,
            int flavour, bool embedded) {
        return NNormalHypersurfaceList::enumerate(owner, flavour, embedded);
    }
    NNormalHypersurfaceList* enumerate_4(regina::Dim4Triangulation* owner,
            int flavour, bool embedded, regina::NProgressManager* manager) {
        return NNormalHypersurfaceList::enumerate(owner, flavour, embedded,
            manager);
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
        .def("recreateMatchingEquations",
            &NNormalHypersurfaceList::recreateMatchingEquations,
            return_value_policy<manage_new_object>())
        .def("getFlavour", &NNormalHypersurfaceList::getFlavour)
        .def("isEmbeddedOnly", &NNormalHypersurfaceList::isEmbeddedOnly)
        .def("getTriangulation", &NNormalHypersurfaceList::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("getNumberOfHypersurfaces",
            &NNormalHypersurfaceList::getNumberOfHypersurfaces)
        .def("getHypersurface", &NNormalHypersurfaceList::getHypersurface,
            return_internal_reference<>())
        .staticmethod("enumerate")
    ;

    s.attr("packetType") = NNormalHypersurfaceList::packetType;
    s.attr("STANDARD") = NNormalHypersurfaceList::STANDARD;
    s.attr("EDGE_WEIGHT") = NNormalHypersurfaceList::EDGE_WEIGHT;

    implicitly_convertible<std::auto_ptr<NNormalHypersurfaceList>,
        std::auto_ptr<regina::NPacket> >();
}

