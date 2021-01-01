
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
#include "triangulation/example4.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"

using namespace regina::python;
using regina::Example;

void addExample4(pybind11::module_& m) {
    auto c = pybind11::class_<Example<4>>(m, "Example4")
        .def_static("sphere", &Example<4>::sphere)
        .def_static("simplicialSphere", &Example<4>::simplicialSphere)
        .def_static("sphereBundle", &Example<4>::sphereBundle)
        .def_static("twistedSphereBundle", &Example<4>::twistedSphereBundle)
        .def_static("ball", &Example<4>::ball)
        .def_static("ballBundle", &Example<4>::ballBundle)
        .def_static("twistedBallBundle", &Example<4>::twistedBallBundle)
        .def_static("doubleCone", &Example<4>::doubleCone)
        .def_static("singleCone", &Example<4>::singleCone)
        .def_static("fourSphere", &Example<4>::fourSphere)
        .def_static("simplicialFourSphere", &Example<4>::simplicialFourSphere)
        .def_static("rp4", &Example<4>::rp4)
        .def_static("s3xs1", &Example<4>::s3xs1)
        .def_static("s3xs1Twisted", &Example<4>::s3xs1Twisted)
        .def_static("cappellShaneson", &Example<4>::cappellShaneson)
        .def_static("iBundle", &Example<4>::iBundle)
        .def_static("s1Bundle", &Example<4>::s1Bundle)
        .def_static("bundleWithMonodromy", &Example<4>::bundleWithMonodromy)
    ;
    regina::python::no_eq_operators(c);

    m.attr("Dim4ExampleTriangulation") = m.attr("Example4");
}

