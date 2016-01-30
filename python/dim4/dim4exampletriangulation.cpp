
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include <boost/python.hpp>
#include "dim4/dim4exampletriangulation.h"
#include "dim4/dim4triangulation.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::Dim4ExampleTriangulation;

void addDim4ExampleTriangulation() {
    class_<Dim4ExampleTriangulation>("Dim4ExampleTriangulation", no_init)
        .def("fourSphere", &Dim4ExampleTriangulation::fourSphere,
            return_value_policy<to_held_type<>>())
        .def("simplicialFourSphere",
            &Dim4ExampleTriangulation::simplicialFourSphere,
            return_value_policy<to_held_type<>>())
        .def("rp4", &Dim4ExampleTriangulation::rp4,
            return_value_policy<to_held_type<>>())
        .def("s3xs1", &Dim4ExampleTriangulation::s3xs1,
            return_value_policy<to_held_type<>>())
        .def("s3xs1Twisted", &Dim4ExampleTriangulation::s3xs1Twisted,
            return_value_policy<to_held_type<>>())
        .def("cappellShaneson", &Dim4ExampleTriangulation::cappellShaneson,
            return_value_policy<to_held_type<>>())
        .def("doubleCone", &Dim4ExampleTriangulation::doubleCone,
            return_value_policy<to_held_type<>>())
        .def("singleCone", &Dim4ExampleTriangulation::singleCone,
            return_value_policy<to_held_type<>>())
        .def("iBundle", &Dim4ExampleTriangulation::iBundle,
            return_value_policy<to_held_type<>>())
        .def("s1Bundle", &Dim4ExampleTriangulation::s1Bundle,
            return_value_policy<to_held_type<>>())
        .def("bundleWithMonodromy",
            &Dim4ExampleTriangulation::bundleWithMonodromy,
            return_value_policy<to_held_type<>>())
        .def(regina::python::no_eq_operators())
        .staticmethod("fourSphere")
        .staticmethod("simplicialFourSphere")
        .staticmethod("rp4")
        .staticmethod("s3xs1")
        .staticmethod("s3xs1Twisted")
        .staticmethod("cappellShaneson")
        .staticmethod("doubleCone")
        .staticmethod("singleCone")
        .staticmethod("iBundle")
        .staticmethod("s1Bundle")
        .staticmethod("bundleWithMonodromy")
    ;
}

