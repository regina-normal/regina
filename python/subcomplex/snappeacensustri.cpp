
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
#include "subcomplex/snappeacensustri.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using namespace boost::python;
using regina::SnapPeaCensusTri;

void addSnapPeaCensusTri() {
    {
        scope s = class_<SnapPeaCensusTri, bases<regina::StandardTriangulation>,
                std::auto_ptr<SnapPeaCensusTri>, boost::noncopyable>
                ("SnapPeaCensusTri", no_init)
            .def("clone", &SnapPeaCensusTri::clone,
                return_value_policy<manage_new_object>())
            .def("section", &SnapPeaCensusTri::section)
            .def("index", &SnapPeaCensusTri::index)
            .def("isSmallSnapPeaCensusTri",
                &SnapPeaCensusTri::isSmallSnapPeaCensusTri,
                return_value_policy<manage_new_object>())
            .def(regina::python::add_eq_operators())
            .staticmethod("isSmallSnapPeaCensusTri")
        ;

        s.attr("SEC_5") = SnapPeaCensusTri::SEC_5;
        s.attr("SEC_6_OR") = SnapPeaCensusTri::SEC_6_OR;
        s.attr("SEC_6_NOR") = SnapPeaCensusTri::SEC_6_NOR;
        s.attr("SEC_7_OR") = SnapPeaCensusTri::SEC_7_OR;
        s.attr("SEC_7_NOR") = SnapPeaCensusTri::SEC_7_NOR;

        implicitly_convertible<std::auto_ptr<SnapPeaCensusTri>,
            std::auto_ptr<regina::StandardTriangulation> >();
    }

    scope().attr("NSnapPeaCensusTri") = scope().attr("SnapPeaCensusTri");
}

