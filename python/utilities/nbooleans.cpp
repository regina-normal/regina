
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

#include "utilities/nbooleans.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NBoolSet;
using regina::NTriBool;

void addNTriBool() {
    scope s = class_<NTriBool>("NTriBool")
        .def(init<bool>())
        .def(init<const NTriBool&>())
        .def("isTrue", &NTriBool::isTrue)
        .def("isFalse", &NTriBool::isFalse)
        .def("isUnknown", &NTriBool::isUnknown)
        .def("isKnown", &NTriBool::isKnown)
        .def("setTrue", &NTriBool::setTrue)
        .def("setFalse", &NTriBool::setFalse)
        .def("setUnknown", &NTriBool::setUnknown)
        .def(self == self)
        // .def(self == bool())
        .def(self != self)
        // .def(self != bool())
        .def(self |= self)
        // .def(self |= bool())
        .def(self &= self)
        // .def(self &= bool())
        .def(self | self)
        // .def(self | bool())
        .def(self & self)
        // .def(self & bool())
        .def(~ self)
        .def(self_ns::str(self))
    ;

    // Apparently there is no way in python to make a module attribute
    // read-only.
    s.attr("True") = NTriBool::True;
    s.attr("False") = NTriBool::False;
    s.attr("Unknown") = NTriBool::Unknown;
}

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
        .def(self == self)
        .def(self != self)
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
        .def("getByteCode", &NBoolSet::getByteCode)
        .def("setByteCode", &NBoolSet::setByteCode)
        .def("fromByteCode", &NBoolSet::fromByteCode)
        .staticmethod("fromByteCode")
        .def(self_ns::str(self))
    ;

    // Apparently there is no way in python to make a module attribute
    // read-only.
    s.attr("sNone") = NBoolSet::sNone;
    s.attr("sTrue") = NBoolSet::sTrue;
    s.attr("sFalse") = NBoolSet::sFalse;
    s.attr("sBoth") = NBoolSet::sBoth;
}

