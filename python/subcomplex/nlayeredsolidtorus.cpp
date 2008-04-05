
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "subcomplex/nlayeredsolidtorus.h"
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NLayeredSolidTorus;

void addNLayeredSolidTorus() {
    class_<NLayeredSolidTorus, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NLayeredSolidTorus>, boost::noncopyable>
            ("NLayeredSolidTorus", no_init)
        .def("clone", &NLayeredSolidTorus::clone,
            return_value_policy<manage_new_object>())
        .def("getNumberOfTetrahedra",
            &NLayeredSolidTorus::getNumberOfTetrahedra)
        .def("getBase", &NLayeredSolidTorus::getBase,
            return_value_policy<reference_existing_object>())
        .def("getBaseEdge", &NLayeredSolidTorus::getBaseEdge)
        .def("getBaseEdgeGroup", &NLayeredSolidTorus::getBaseEdgeGroup)
        .def("getBaseFace", &NLayeredSolidTorus::getBaseFace)
        .def("getTopLevel", &NLayeredSolidTorus::getTopLevel,
            return_value_policy<reference_existing_object>())
        .def("getMeridinalCuts", &NLayeredSolidTorus::getMeridinalCuts)
        .def("getTopEdge", &NLayeredSolidTorus::getTopEdge)
        .def("getTopEdgeGroup", &NLayeredSolidTorus::getTopEdgeGroup)
        .def("getTopFace", &NLayeredSolidTorus::getTopFace)
        .def("flatten", &NLayeredSolidTorus::flatten,
            return_value_policy<manage_new_object>())
        .def("transform", &NLayeredSolidTorus::transform)
        .def("formsLayeredSolidTorusBase",
            &NLayeredSolidTorus::formsLayeredSolidTorusBase,
            return_value_policy<manage_new_object>())
        .def("formsLayeredSolidTorusTop",
            &NLayeredSolidTorus::formsLayeredSolidTorusTop,
            return_value_policy<manage_new_object>())
        .def("isLayeredSolidTorus", &NLayeredSolidTorus::isLayeredSolidTorus,
            return_value_policy<manage_new_object>())
        .staticmethod("formsLayeredSolidTorusBase")
        .staticmethod("formsLayeredSolidTorusTop")
        .staticmethod("isLayeredSolidTorus")
    ;

    implicitly_convertible<std::auto_ptr<NLayeredSolidTorus>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

