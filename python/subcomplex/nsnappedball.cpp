
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "subcomplex/nsnappedball.h"
#include "triangulation/ntetrahedron.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NSnappedBall;

void addNSnappedBall() {
    class_<NSnappedBall, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NSnappedBall>, boost::noncopyable>
            ("NSnappedBall", no_init)
        .def("clone", &NSnappedBall::clone,
            return_value_policy<manage_new_object>())
        .def("getTetrahedron", &NSnappedBall::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryFace", &NSnappedBall::getBoundaryFace)
        .def("getInternalFace", &NSnappedBall::getInternalFace)
        .def("getEquatorEdge", &NSnappedBall::getEquatorEdge)
        .def("getInternalEdge", &NSnappedBall::getInternalEdge)
        .def("formsSnappedBall", &NSnappedBall::formsSnappedBall,
            return_value_policy<manage_new_object>())
        .staticmethod("formsSnappedBall")
    ;

    implicitly_convertible<std::auto_ptr<NSnappedBall>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

