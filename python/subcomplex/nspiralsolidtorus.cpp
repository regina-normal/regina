
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "subcomplex/nspiralsolidtorus.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NSpiralSolidTorus;

void addNSpiralSolidTorus() {
    class_<NSpiralSolidTorus, bases<regina::ShareableObject>,
            std::auto_ptr<NSpiralSolidTorus>, boost::noncopyable>
            ("NSpiralSolidTorus", no_init)
        .def("clone", &NSpiralSolidTorus::clone,
            return_value_policy<manage_new_object>())
        .def("getNumberOfTetrahedra",
            &NSpiralSolidTorus::getNumberOfTetrahedra)
        .def("getTetrahedron", &NSpiralSolidTorus::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getVertexRoles", &NSpiralSolidTorus::getVertexRoles)
        .def("reverse", &NSpiralSolidTorus::reverse)
        .def("cycle", &NSpiralSolidTorus::cycle)
        .def("makeCanonical", &NSpiralSolidTorus::makeCanonical)
        .def("isCanonical", &NSpiralSolidTorus::isCanonical)
        .def("isSpiralSolidTorus", &NSpiralSolidTorus::isSpiralSolidTorus,
            return_value_policy<manage_new_object>())
        .staticmethod("isSpiralSolidTorus")
    ;
}

