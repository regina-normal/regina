
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

#include "subcomplex/ntrivialtri.h"
#include "triangulation/ncomponent.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NTrivialTri;

void addNTrivialTri() {
    scope s = class_<NTrivialTri, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NTrivialTri>, boost::noncopyable>
            ("NTrivialTri", no_init)
        .def("clone", &NTrivialTri::clone,
            return_value_policy<manage_new_object>())
        .def("getType", &NTrivialTri::getType)
        .def("isTrivialTriangulation", &NTrivialTri::isTrivialTriangulation,
            return_value_policy<manage_new_object>())
        .staticmethod("isTrivialTriangulation")
    ;

    s.attr("SPHERE_4_VERTEX") = NTrivialTri::SPHERE_4_VERTEX;
    s.attr("N2") = NTrivialTri::N2;
    s.attr("N3_1") = NTrivialTri::N3_1;
    s.attr("N3_2") = NTrivialTri::N3_2;
    s.attr("SNAPPEA_M000") = NTrivialTri::SNAPPEA_M000;
    s.attr("SNAPPEA_M001") = NTrivialTri::SNAPPEA_M001;
    s.attr("SNAPPEA_M002") = NTrivialTri::SNAPPEA_M002;
    s.attr("SNAPPEA_M003") = NTrivialTri::SNAPPEA_M003;
    s.attr("SNAPPEA_M004") = NTrivialTri::SNAPPEA_M004;

    implicitly_convertible<std::auto_ptr<NTrivialTri>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

