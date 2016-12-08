
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
#include "subcomplex/snappedball.h"
#include "subcomplex/nsnappedtwosphere.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NSnappedTwoSphere;

namespace {
    NSnappedTwoSphere* (*formsStructure_tets)
        (regina::Tetrahedron<3>*, regina::Tetrahedron<3>*) =
        &NSnappedTwoSphere::formsSnappedTwoSphere;
    NSnappedTwoSphere* (*formsStructure_balls)
        (regina::SnappedBall*, regina::SnappedBall*) =
        &NSnappedTwoSphere::formsSnappedTwoSphere;
}

void addNSnappedTwoSphere() {
    class_<NSnappedTwoSphere, std::auto_ptr<NSnappedTwoSphere>,
            boost::noncopyable> ("NSnappedTwoSphere", no_init)
        .def("clone", &NSnappedTwoSphere::clone,
            return_value_policy<manage_new_object>())
        .def("snappedBall", &NSnappedTwoSphere::snappedBall,
            return_internal_reference<>())
        .def("formsSnappedTwoSphere", formsStructure_tets,
            return_value_policy<manage_new_object>())
        .def("formsSnappedTwoSphere", formsStructure_balls,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("formsSnappedTwoSphere")
    ;
}

