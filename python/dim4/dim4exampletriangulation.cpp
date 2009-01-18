
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

#include "dim4/dim4exampletriangulation.h"
#include "dim4/dim4triangulation.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4ExampleTriangulation;

void addDim4ExampleTriangulation() {
    class_<Dim4ExampleTriangulation>("Dim4ExampleTriangulation", no_init)
        .def("fourSphere", &Dim4ExampleTriangulation::fourSphere,
            return_value_policy<manage_new_object>())
        .def("rp4", &Dim4ExampleTriangulation::rp4,
            return_value_policy<manage_new_object>())
        .def("doubleCone", &Dim4ExampleTriangulation::doubleCone,
            return_value_policy<manage_new_object>())
        .def("singleCone", &Dim4ExampleTriangulation::singleCone,
            return_value_policy<manage_new_object>())
        .staticmethod("fourSphere")
        .staticmethod("rp4")
        .staticmethod("doubleCone")
        .staticmethod("singleCone")
    ;
}

