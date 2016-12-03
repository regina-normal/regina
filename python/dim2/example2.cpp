
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

#include "triangulation/example2.h"
#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../safeheldtype.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Example;

void addExample2() {
    class_<Example<2>>("Example2", no_init)
        .def("sphere", &Example<2>::sphere,
            return_value_policy<to_held_type<> >())
        .def("simplicialSphere", &Example<2>::simplicialSphere,
            return_value_policy<to_held_type<> >())
        .def("sphereBundle", &Example<2>::sphereBundle,
            return_value_policy<to_held_type<> >())
        .def("twistedSphereBundle", &Example<2>::twistedSphereBundle,
            return_value_policy<to_held_type<> >())
        .def("ball", &Example<2>::ball,
            return_value_policy<to_held_type<> >())
        .def("ballBundle", &Example<2>::ballBundle,
            return_value_policy<to_held_type<> >())
        .def("twistedBallBundle", &Example<2>::twistedBallBundle,
            return_value_policy<to_held_type<> >())
        .def("orientable", &Example<2>::orientable,
            return_value_policy<to_held_type<> >())
        .def("nonOrientable", &Example<2>::nonOrientable,
            return_value_policy<to_held_type<> >())
        .def("sphereTetrahedron", &Example<2>::sphereTetrahedron,
            return_value_policy<to_held_type<> >())
        .def("sphereOctahedron", &Example<2>::sphereOctahedron,
            return_value_policy<to_held_type<> >())
        .def("disc", &Example<2>::disc,
            return_value_policy<to_held_type<> >())
        .def("annulus", &Example<2>::annulus,
            return_value_policy<to_held_type<> >())
        .def("mobius", &Example<2>::mobius,
            return_value_policy<to_held_type<> >())
        .def("torus", &Example<2>::torus,
            return_value_policy<to_held_type<> >())
        .def("rp2", &Example<2>::rp2,
            return_value_policy<to_held_type<> >())
        .def("kb", &Example<2>::kb,
            return_value_policy<to_held_type<> >())
        .def(regina::python::no_eq_operators())
        .staticmethod("sphere")
        .staticmethod("simplicialSphere")
        .staticmethod("sphereBundle")
        .staticmethod("twistedSphereBundle")
        .staticmethod("ball")
        .staticmethod("ballBundle")
        .staticmethod("twistedBallBundle")
        .staticmethod("orientable")
        .staticmethod("nonOrientable")
        .staticmethod("sphereTetrahedron")
        .staticmethod("sphereOctahedron")
        .staticmethod("disc")
        .staticmethod("annulus")
        .staticmethod("mobius")
        .staticmethod("torus")
        .staticmethod("rp2")
        .staticmethod("kb")
    ;

    scope().attr("Dim2ExampleTriangulation") = scope().attr("Example2");
}

