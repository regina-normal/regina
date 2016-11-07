
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
#include "triangulation/example.h"
#include "triangulation/generic.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::Example;

template <int dim>
void addExample(const char* name) {
    class_<Example<dim>>(name, no_init)
        .def("sphere", &Example<dim>::sphere,
            return_value_policy<to_held_type<> >())
        .def("simplicialSphere", &Example<dim>::simplicialSphere,
            return_value_policy<to_held_type<> >())
        .def("sphereBundle", &Example<dim>::sphereBundle,
            return_value_policy<to_held_type<> >())
        .def("twistedSphereBundle", &Example<dim>::twistedSphereBundle,
            return_value_policy<to_held_type<> >())
        .def("ball", &Example<dim>::ball,
            return_value_policy<to_held_type<> >())
        .def("ballBundle", &Example<dim>::ballBundle,
            return_value_policy<to_held_type<> >())
        .def("twistedBallBundle", &Example<dim>::twistedBallBundle,
            return_value_policy<to_held_type<> >())
        .def("doubleCone", &Example<dim>::doubleCone,
            return_value_policy<to_held_type<>>())
        .def("singleCone", &Example<dim>::singleCone,
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
    ;
}

