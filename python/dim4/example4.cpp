
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
#include "triangulation/example4.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::Example;

void addExample4() {
    class_<Example<4>>("Example4", no_init)
        .def("sphere", &Example<4>::sphere,
            return_value_policy<to_held_type<> >())
        .def("simplicialSphere", &Example<4>::simplicialSphere,
            return_value_policy<to_held_type<> >())
        .def("sphereBundle", &Example<4>::sphereBundle,
            return_value_policy<to_held_type<> >())
        .def("twistedSphereBundle", &Example<4>::twistedSphereBundle,
            return_value_policy<to_held_type<> >())
        .def("ball", &Example<4>::ball,
            return_value_policy<to_held_type<> >())
        .def("ballBundle", &Example<4>::ballBundle,
            return_value_policy<to_held_type<> >())
        .def("twistedBallBundle", &Example<4>::twistedBallBundle,
            return_value_policy<to_held_type<> >())
        .def("doubleCone", &Example<4>::doubleCone,
            return_value_policy<to_held_type<>>())
        .def("singleCone", &Example<4>::singleCone,
            return_value_policy<to_held_type<>>())
        .def("fourSphere", &Example<4>::fourSphere,
            return_value_policy<to_held_type<>>())
        .def("simplicialFourSphere",
            &Example<4>::simplicialFourSphere,
            return_value_policy<to_held_type<>>())
        .def("rp4", &Example<4>::rp4,
            return_value_policy<to_held_type<>>())
        .def("s3xs1", &Example<4>::s3xs1,
            return_value_policy<to_held_type<>>())
        .def("s3xs1Twisted", &Example<4>::s3xs1Twisted,
            return_value_policy<to_held_type<>>())
        .def("cappellShaneson", &Example<4>::cappellShaneson,
            return_value_policy<to_held_type<>>())
        .def("iBundle", &Example<4>::iBundle,
            return_value_policy<to_held_type<>>())
        .def("s1Bundle", &Example<4>::s1Bundle,
            return_value_policy<to_held_type<>>())
        .def("bundleWithMonodromy",
            &Example<4>::bundleWithMonodromy,
            return_value_policy<to_held_type<>>())
        .def(regina::python::no_eq_operators())
        .staticmethod("sphere")
        .staticmethod("simplicialSphere")
        .staticmethod("sphereBundle")
        .staticmethod("twistedSphereBundle")
        .staticmethod("ball")
        .staticmethod("ballBundle")
        .staticmethod("twistedBallBundle")
        .staticmethod("doubleCone")
        .staticmethod("singleCone")
        .staticmethod("fourSphere")
        .staticmethod("simplicialFourSphere")
        .staticmethod("rp4")
        .staticmethod("s3xs1")
        .staticmethod("s3xs1Twisted")
        .staticmethod("cappellShaneson")
        .staticmethod("iBundle")
        .staticmethod("s1Bundle")
        .staticmethod("bundleWithMonodromy")
    ;

    scope().attr("Dim4ExampleTriangulation") = scope().attr("Example4");
}

