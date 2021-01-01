
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
#include "triangulation/example3.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::Example;

void addExample3(pybind11::module_& m) {
    auto c = pybind11::class_<Example<3>>(m, "Example3")
        .def_static("sphere", &Example<3>::sphere)
        .def_static("simplicialSphere", &Example<3>::simplicialSphere)
        .def_static("sphereBundle", &Example<3>::sphereBundle)
        .def_static("twistedSphereBundle", &Example<3>::twistedSphereBundle)
        .def_static("ball", &Example<3>::ball)
        .def_static("ballBundle", &Example<3>::ballBundle)
        .def_static("twistedBallBundle", &Example<3>::twistedBallBundle)
        .def_static("doubleCone", &Example<3>::doubleCone)
        .def_static("singleCone", &Example<3>::singleCone)
        .def_static("threeSphere", &Example<3>::threeSphere)
        .def_static("bingsHouse", &Example<3>::bingsHouse)
        .def_static("s2xs1", &Example<3>::s2xs1)
        .def_static("rp2xs1", &Example<3>::rp2xs1)
        .def_static("rp3rp3", &Example<3>::rp3rp3)
        .def_static("lens", &Example<3>::lens)
        .def_static("poincareHomologySphere",
            &Example<3>::poincareHomologySphere)
        .def_static("weeks", &Example<3>::weeks)
        .def_static("weberSeifert", &Example<3>::weberSeifert)
        .def_static("smallClosedOrblHyperbolic",
            &Example<3>::smallClosedOrblHyperbolic)
        .def_static("smallClosedNonOrblHyperbolic",
            &Example<3>::smallClosedNonOrblHyperbolic)
        .def_static("sphere600", &Example<3>::sphere600)
        .def_static("lst", &Example<3>::lst)
        .def_static("solidKleinBottle", &Example<3>::solidKleinBottle)
        .def_static("figureEight", &Example<3>::figureEight)
        .def_static("trefoil", &Example<3>::trefoil)
        .def_static("whiteheadLink", &Example<3>::whiteheadLink)
        .def_static("gieseking", &Example<3>::gieseking)
        .def_static("cuspedGenusTwoTorus",
            &Example<3>::cuspedGenusTwoTorus)
    ;
    regina::python::no_eq_operators(c);

    m.attr("NExampleTriangulation") = m.attr("Example3");
}

