
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
#include "triangulation/example3.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::Example;

void addExample3() {
    class_<Example<3>>("Example3", no_init)
        .def("sphere", &Example<3>::sphere,
            return_value_policy<to_held_type<> >())
        .def("simplicialSphere", &Example<3>::simplicialSphere,
            return_value_policy<to_held_type<> >())
        .def("sphereBundle", &Example<3>::sphereBundle,
            return_value_policy<to_held_type<> >())
        .def("twistedSphereBundle", &Example<3>::twistedSphereBundle,
            return_value_policy<to_held_type<> >())
        .def("ball", &Example<3>::ball,
            return_value_policy<to_held_type<> >())
        .def("ballBundle", &Example<3>::ballBundle,
            return_value_policy<to_held_type<> >())
        .def("twistedBallBundle", &Example<3>::twistedBallBundle,
            return_value_policy<to_held_type<> >())
        .def("doubleCone", &Example<3>::doubleCone,
            return_value_policy<to_held_type<>>())
        .def("singleCone", &Example<3>::singleCone,
            return_value_policy<to_held_type<>>())
        .def("threeSphere", &Example<3>::threeSphere,
            return_value_policy<to_held_type<> >())
        .def("bingsHouse", &Example<3>::bingsHouse,
            return_value_policy<to_held_type<> >())
        .def("s2xs1", &Example<3>::s2xs1,
            return_value_policy<to_held_type<> >())
        .def("rp2xs1", &Example<3>::rp2xs1,
            return_value_policy<to_held_type<> >())
        .def("rp3rp3", &Example<3>::rp3rp3,
            return_value_policy<to_held_type<> >())
        .def("lens", &Example<3>::lens,
            return_value_policy<to_held_type<> >())
        .def("poincareHomologySphere",
            &Example<3>::poincareHomologySphere,
            return_value_policy<to_held_type<> >())
        .def("weeks", &Example<3>::weeks,
            return_value_policy<to_held_type<> >())
        .def("weberSeifert", &Example<3>::weberSeifert,
            return_value_policy<to_held_type<> >())
        .def("smallClosedOrblHyperbolic",
            &Example<3>::smallClosedOrblHyperbolic,
            return_value_policy<to_held_type<> >())
        .def("smallClosedNonOrblHyperbolic",
            &Example<3>::smallClosedNonOrblHyperbolic,
            return_value_policy<to_held_type<> >())
        .def("sphere600", &Example<3>::sphere600,
            return_value_policy<to_held_type<> >())
        .def("lst", &Example<3>::lst,
            return_value_policy<to_held_type<> >())
        .def("solidKleinBottle", &Example<3>::solidKleinBottle,
            return_value_policy<to_held_type<> >())
        .def("figureEight", &Example<3>::figureEight,
            return_value_policy<to_held_type<> >())
        .def("trefoil", &Example<3>::trefoil,
            return_value_policy<to_held_type<> >())
        .def("whiteheadLink", &Example<3>::whiteheadLink,
            return_value_policy<to_held_type<> >())
        .def("gieseking", &Example<3>::gieseking,
            return_value_policy<to_held_type<> >())
        .def("cuspedGenusTwoTorus",
            &Example<3>::cuspedGenusTwoTorus,
            return_value_policy<to_held_type<> >())
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
        .staticmethod("threeSphere")
        .staticmethod("bingsHouse")
        .staticmethod("s2xs1")
        .staticmethod("rp2xs1")
        .staticmethod("rp3rp3")
        .staticmethod("lens")
        .staticmethod("poincareHomologySphere")
        .staticmethod("weeks")
        .staticmethod("weberSeifert")
        .staticmethod("smallClosedOrblHyperbolic")
        .staticmethod("smallClosedNonOrblHyperbolic")
        .staticmethod("sphere600")
        .staticmethod("lst")
        .staticmethod("solidKleinBottle")
        .staticmethod("figureEight")
        .staticmethod("trefoil")
        .staticmethod("whiteheadLink")
        .staticmethod("gieseking")
        .staticmethod("cuspedGenusTwoTorus")
    ;

    scope().attr("NExampleTriangulation") = scope().attr("Example3");
}

