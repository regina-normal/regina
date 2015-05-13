
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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
#include "subcomplex/nsnappedball.h"
#include "subcomplex/nsnappedtwosphere.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include "../utilities/equality.h"

using namespace boost::python;
using regina::NSnappedTwoSphere;

namespace {
    NSnappedTwoSphere* (*formsStructure_tets)
        (regina::NTetrahedron*, regina::NTetrahedron*) =
        &NSnappedTwoSphere::formsSnappedTwoSphere;
    NSnappedTwoSphere* (*formsStructure_balls)
        (regina::NSnappedBall*, regina::NSnappedBall*) =
        &NSnappedTwoSphere::formsSnappedTwoSphere;
}

void addNSnappedTwoSphere() {
    class_<NSnappedTwoSphere, std::auto_ptr<NSnappedTwoSphere>,
            boost::noncopyable> ("NSnappedTwoSphere", no_init)
        .def("clone", &NSnappedTwoSphere::clone,
            return_value_policy<manage_new_object>())
        .def("getSnappedBall", &NSnappedTwoSphere::getSnappedBall,
            return_internal_reference<>())
        .def("formsSnappedTwoSphere", formsStructure_tets,
            return_value_policy<manage_new_object>())
        .def("formsSnappedTwoSphere", formsStructure_balls,
            return_value_policy<manage_new_object>())
        .def("str", &NSnappedTwoSphere::str)
        .def("toString", &NSnappedTwoSphere::toString)
        .def("detail", &NSnappedTwoSphere::detail)
        .def("toStringLong", &NSnappedTwoSphere::toStringLong)
        .def("__str__", &NSnappedTwoSphere::str)
        EQUAL_BY_PTR(NSnappedTwoSphere)
        .staticmethod("formsSnappedTwoSphere")
    ;
}

