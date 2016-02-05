
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
#include "utilities/nbooleans.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NBoolSet;

void addNBoolSet() {
    scope s = class_<NBoolSet>("NBoolSet")
        .def(init<bool>())
        .def(init<const NBoolSet&>())
        .def(init<bool, bool>())
        .def("hasTrue", &NBoolSet::hasTrue)
        .def("hasFalse", &NBoolSet::hasFalse)
        .def("contains", &NBoolSet::contains)
        .def("insertTrue", &NBoolSet::insertTrue)
        .def("insertFalse", &NBoolSet::insertFalse)
        .def("removeTrue", &NBoolSet::removeTrue)
        .def("removeFalse", &NBoolSet::removeFalse)
        .def("empty", &NBoolSet::empty)
        .def("fill", &NBoolSet::fill)
        .def(self < self)
        .def(self > self)
        .def(self <= self)
        .def(self >= self)
        .def(self |= self)
        .def(self &= self)
        .def(self ^= self)
        .def(self | self)
        .def(self & self)
        .def(self ^ self)
        .def(~ self)
        .def("byteCode", &NBoolSet::byteCode)
        .def("getByteCode", &NBoolSet::byteCode)
        .def("setByteCode", &NBoolSet::setByteCode)
        .def("fromByteCode", &NBoolSet::fromByteCode)
        .staticmethod("fromByteCode")
        .def(self_ns::str(self))
        .def(regina::python::add_eq_operators())
    ;

    // Apparently there is no way in python to make a module attribute
    // read-only.
    s.attr("sNone") = NBoolSet::sNone;
    s.attr("sTrue") = NBoolSet::sTrue;
    s.attr("sFalse") = NBoolSet::sFalse;
    s.attr("sBoth") = NBoolSet::sBoth;
}

