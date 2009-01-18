
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

#include "subcomplex/nsatannulus.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include "utilities/nmatrix2.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPerm;
using regina::NSatAnnulus;
using regina::NTetrahedron;

namespace {
    NTetrahedron* tet_read(NSatAnnulus& a, int which) {
        return a.tet[which];
    }

    void tet_write(NSatAnnulus& a, int which, NTetrahedron* value) {
        a.tet[which] = value;
    }

    NPerm roles_read(NSatAnnulus& a, int which) {
        return a.roles[which];
    }

    void roles_write(NSatAnnulus& a, int which, NPerm value) {
        a.roles[which] = value;
    }

    boost::python::tuple isAdjacent_tuple(const NSatAnnulus& a,
            const NSatAnnulus& other) {
        bool refVert = false;
        bool refHoriz = false;
        bool ans = a.isAdjacent(other, &refVert, &refHoriz);

        return make_tuple(ans, refVert, refHoriz);
    }
}

void addNSatAnnulus() {
    class_<NSatAnnulus>("NSatAnnulus")
        .def(init<const NSatAnnulus&>())
        .def(init<NTetrahedron*, NPerm, NTetrahedron*, NPerm>())
        .def("tet", tet_read, return_internal_reference<>())
        .def("roles", roles_read)
        .def("setTet", tet_write)
        .def("setRoles", roles_write)
        .def(self == self)
        .def(self != self)
        .def("meetsBoundary", &NSatAnnulus::meetsBoundary)
        .def("switchSides", &NSatAnnulus::switchSides)
        .def("otherSide", &NSatAnnulus::otherSide)
        .def("reflectVertical", &NSatAnnulus::reflectVertical)
        .def("verticalReflection", &NSatAnnulus::verticalReflection)
        .def("reflectHorizontal", &NSatAnnulus::reflectHorizontal)
        .def("horizontalReflection", &NSatAnnulus::horizontalReflection)
        .def("rotateHalfTurn", &NSatAnnulus::rotateHalfTurn)
        .def("halfTurnRotation", &NSatAnnulus::halfTurnRotation)
        .def("isAdjacent", isAdjacent_tuple)
        .def("isJoined", &NSatAnnulus::isJoined)
        .def("isTwoSidedTorus", &NSatAnnulus::isTwoSidedTorus)
        .def("transform", &NSatAnnulus::transform)
        .def("image", &NSatAnnulus::image)
        .def("attachLST", &NSatAnnulus::attachLST)
    ;
}

