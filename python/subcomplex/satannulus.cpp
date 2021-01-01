
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
#include "maths/matrix2.h"
#include "subcomplex/satannulus.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::Perm;
using regina::SatAnnulus;
using regina::Tetrahedron;

void addSatAnnulus(pybind11::module_& m) {
    auto c = pybind11::class_<SatAnnulus>(m, "SatAnnulus")
        .def(pybind11::init<>())
        .def(pybind11::init<const SatAnnulus&>())
        .def(pybind11::init<Tetrahedron<3>*, Perm<4>,
            Tetrahedron<3>*, Perm<4>>())
        .def("tet", [](SatAnnulus& a, int which) {
            return a.tet[which];
        }, pybind11::return_value_policy::reference_internal)
        .def("roles", [](SatAnnulus& a, int which) {
            return a.roles[which];
        })
        .def("setTet", [](SatAnnulus& a, int which, Tetrahedron<3>* value) {
            a.tet[which] = value;
        })
        .def("setRoles", [](SatAnnulus& a, int which, Perm<4> value) {
            a.roles[which] = value;
        })
        .def("meetsBoundary", &SatAnnulus::meetsBoundary)
        .def("switchSides", &SatAnnulus::switchSides)
        .def("otherSide", &SatAnnulus::otherSide)
        .def("reflectVertical", &SatAnnulus::reflectVertical)
        .def("verticalReflection", &SatAnnulus::verticalReflection)
        .def("reflectHorizontal", &SatAnnulus::reflectHorizontal)
        .def("horizontalReflection", &SatAnnulus::horizontalReflection)
        .def("rotateHalfTurn", &SatAnnulus::rotateHalfTurn)
        .def("halfTurnRotation", &SatAnnulus::halfTurnRotation)
        .def("isAdjacent", [](const SatAnnulus& a, const SatAnnulus& other) {
            bool refVert = false;
            bool refHoriz = false;
            bool ans = a.isAdjacent(other, &refVert, &refHoriz);
            return pybind11::make_tuple(ans, refVert, refHoriz);
        })
        .def("isJoined", &SatAnnulus::isJoined)
        .def("isTwoSidedTorus", &SatAnnulus::isTwoSidedTorus)
        .def("transform", &SatAnnulus::transform)
        .def("image", &SatAnnulus::image)
        .def("attachLST", &SatAnnulus::attachLST)
    ;
    regina::python::add_eq_operators(c);

    m.attr("NSatAnnulus") = m.attr("SatAnnulus");
}

