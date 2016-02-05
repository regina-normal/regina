
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
#include "subcomplex/nspiralsolidtorus.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NSpiralSolidTorus;

void addNSpiralSolidTorus() {
    class_<NSpiralSolidTorus, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NSpiralSolidTorus>, boost::noncopyable>
            ("NSpiralSolidTorus", no_init)
        .def("clone", &NSpiralSolidTorus::clone,
            return_value_policy<manage_new_object>())
        .def("size", &NSpiralSolidTorus::size)
        .def("getNumberOfTetrahedra", &NSpiralSolidTorus::size)
        .def("tetrahedron", &NSpiralSolidTorus::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &NSpiralSolidTorus::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("vertexRoles", &NSpiralSolidTorus::vertexRoles)
        .def("getVertexRoles", &NSpiralSolidTorus::vertexRoles)
        .def("reverse", &NSpiralSolidTorus::reverse)
        .def("cycle", &NSpiralSolidTorus::cycle)
        .def("makeCanonical", &NSpiralSolidTorus::makeCanonical)
        .def("isCanonical", &NSpiralSolidTorus::isCanonical)
        .def("formsSpiralSolidTorus", &NSpiralSolidTorus::formsSpiralSolidTorus,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
        .staticmethod("formsSpiralSolidTorus")
    ;

    implicitly_convertible<std::auto_ptr<NSpiralSolidTorus>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

