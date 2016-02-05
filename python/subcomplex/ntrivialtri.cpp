
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

#include <boost/python.hpp>
#include "subcomplex/ntrivialtri.h"
#include "triangulation/ncomponent.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NTrivialTri;

void addNTrivialTri() {
    scope s = class_<NTrivialTri, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NTrivialTri>, boost::noncopyable>
            ("NTrivialTri", no_init)
        .def("clone", &NTrivialTri::clone,
            return_value_policy<manage_new_object>())
        .def("type", &NTrivialTri::type)
        .def("getType", &NTrivialTri::type)
        .def("isTrivialTriangulation", &NTrivialTri::isTrivialTriangulation,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
        .staticmethod("isTrivialTriangulation")
    ;

    s.attr("SPHERE_4_VERTEX") = NTrivialTri::SPHERE_4_VERTEX;
    s.attr("BALL_3_VERTEX") = NTrivialTri::BALL_3_VERTEX;
    s.attr("BALL_4_VERTEX") = NTrivialTri::BALL_4_VERTEX;
    s.attr("N2") = NTrivialTri::N2;
    s.attr("N3_1") = NTrivialTri::N3_1;
    s.attr("N3_2") = NTrivialTri::N3_2;

    implicitly_convertible<std::auto_ptr<NTrivialTri>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

