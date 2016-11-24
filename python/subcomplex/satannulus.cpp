
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

#include <boost/python.hpp>
#include "maths/matrix2.h"
#include "subcomplex/satannulus.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Perm;
using regina::SatAnnulus;
using regina::Tetrahedron;

namespace {
    Tetrahedron<3>* tet_read(SatAnnulus& a, int which) {
        return a.tet[which];
    }

    void tet_write(SatAnnulus& a, int which, Tetrahedron<3>* value) {
        a.tet[which] = value;
    }

    Perm<4> roles_read(SatAnnulus& a, int which) {
        return a.roles[which];
    }

    void roles_write(SatAnnulus& a, int which, Perm<4> value) {
        a.roles[which] = value;
    }

    boost::python::tuple isAdjacent_tuple(const SatAnnulus& a,
            const SatAnnulus& other) {
        bool refVert = false;
        bool refHoriz = false;
        bool ans = a.isAdjacent(other, &refVert, &refHoriz);

        return boost::python::make_tuple(ans, refVert, refHoriz);
    }
}

void addSatAnnulus() {
    class_<SatAnnulus>("SatAnnulus")
        .def(init<const SatAnnulus&>())
        .def(init<Tetrahedron<3>*, Perm<4>, Tetrahedron<3>*, Perm<4>>())
        .def("tet", tet_read, return_internal_reference<>())
        .def("roles", roles_read)
        .def("setTet", tet_write)
        .def("setRoles", roles_write)
        .def("meetsBoundary", &SatAnnulus::meetsBoundary)
        .def("switchSides", &SatAnnulus::switchSides)
        .def("otherSide", &SatAnnulus::otherSide)
        .def("reflectVertical", &SatAnnulus::reflectVertical)
        .def("verticalReflection", &SatAnnulus::verticalReflection)
        .def("reflectHorizontal", &SatAnnulus::reflectHorizontal)
        .def("horizontalReflection", &SatAnnulus::horizontalReflection)
        .def("rotateHalfTurn", &SatAnnulus::rotateHalfTurn)
        .def("halfTurnRotation", &SatAnnulus::halfTurnRotation)
        .def("isAdjacent", isAdjacent_tuple)
        .def("isJoined", &SatAnnulus::isJoined)
        .def("isTwoSidedTorus", &SatAnnulus::isTwoSidedTorus)
        .def("transform", &SatAnnulus::transform)
        .def("image", &SatAnnulus::image)
        .def("attachLST", &SatAnnulus::attachLST)
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NSatAnnulus") = scope().attr("SatAnnulus");
}

