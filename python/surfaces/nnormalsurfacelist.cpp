
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

#include "maths/nmatrixint.h"
#include "progress/nprogressmanager.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NNormalSurfaceList;

namespace {
    // Write manual overload wrappers since this is a static member function.
    NNormalSurfaceList* enumerate_2(regina::NTriangulation* owner,
            int flavour) {
        return NNormalSurfaceList::enumerate(owner, flavour);
    }
    NNormalSurfaceList* enumerate_3(regina::NTriangulation* owner,
            int flavour, bool embedded) {
        return NNormalSurfaceList::enumerate(owner, flavour, embedded);
    }
    NNormalSurfaceList* enumerate_4(regina::NTriangulation* owner,
            int flavour, bool embedded, regina::NProgressManager* manager) {
        return NNormalSurfaceList::enumerate(owner, flavour, embedded, manager);
    }
}

void addNNormalSurfaceList() {
    def("makeMatchingEquations",
        regina::makeMatchingEquations,
        return_value_policy<manage_new_object>());

    scope s = class_<NNormalSurfaceList,
            bases<regina::NPacket, regina::NSurfaceSet>,
            std::auto_ptr<NNormalSurfaceList>, boost::noncopyable>
            ("NNormalSurfaceList", no_init)
        .def("enumerate", enumerate_2,
            return_value_policy<reference_existing_object>())
        .def("enumerate", enumerate_3,
            return_value_policy<reference_existing_object>())
        .def("enumerate", enumerate_4,
            return_value_policy<reference_existing_object>())
        .def("enumerateStandardDirect",
            &NNormalSurfaceList::enumerateStandardDirect,
            return_value_policy<reference_existing_object>())
        .def("enumerateStandardANDirect",
            &NNormalSurfaceList::enumerateStandardANDirect,
            return_value_policy<reference_existing_object>())
        .def("quadToStandard", &NNormalSurfaceList::quadToStandard,
            return_value_policy<reference_existing_object>())
        .def("quadOctToStandardAN", &NNormalSurfaceList::quadOctToStandardAN,
            return_value_policy<reference_existing_object>())
        .def("standardToQuad", &NNormalSurfaceList::standardToQuad,
            return_value_policy<reference_existing_object>())
        .def("standardANToQuadOct", &NNormalSurfaceList::standardANToQuadOct,
            return_value_policy<reference_existing_object>())
        .def("recreateMatchingEquations",
            &NNormalSurfaceList::recreateMatchingEquations,
            return_value_policy<manage_new_object>())
        .staticmethod("enumerate")
        .staticmethod("enumerateStandardDirect")
        .staticmethod("enumerateStandardANDirect")
    ;

    s.attr("packetType") = NNormalSurfaceList::packetType;
    s.attr("STANDARD") = NNormalSurfaceList::STANDARD;
    s.attr("AN_STANDARD") = NNormalSurfaceList::AN_STANDARD;
    s.attr("QUAD") = NNormalSurfaceList::QUAD;
    s.attr("QUAD_OCT") = NNormalSurfaceList::QUAD_OCT;
    s.attr("EDGE_WEIGHT") = NNormalSurfaceList::EDGE_WEIGHT;
    s.attr("FACE_ARCS") = NNormalSurfaceList::FACE_ARCS;
    s.attr("AN_LEGACY") = NNormalSurfaceList::AN_LEGACY;

    implicitly_convertible<std::auto_ptr<NNormalSurfaceList>,
        std::auto_ptr<regina::NPacket> >();
    implicitly_convertible<std::auto_ptr<NNormalSurfaceList>,
        std::auto_ptr<regina::NSurfaceSet> >();
}

