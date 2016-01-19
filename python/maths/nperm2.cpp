
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

// We need to see Python.h first to avoid a "portability fix" in pyport.h
// that breaks boost.python on MacOSX.
#include "Python.h"
#include <boost/python.hpp>
#include "maths/nperm2.h"
#include "../globalarray.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NPerm2;
using regina::python::GlobalArray;

namespace {
    GlobalArray<NPerm2> NPerm2_S2_arr(NPerm2::S2, 2);
    GlobalArray<unsigned> NPerm2_invS2_arr(NPerm2::invS2, 2);
    GlobalArray<NPerm2> NPerm2_S1_arr(NPerm2::S1, 1);

    int perm2_getItem(const NPerm2& p, int index) {
        return p[index];
    }
}

void addNPerm2() {
    scope s = class_<NPerm2>("NPerm2")
        .def(init<int, int>())
        .def(init<const NPerm2&>())
        .def("permCode", &NPerm2::permCode)
        .def("getPermCode", &NPerm2::getPermCode)
        .def("setPermCode", &NPerm2::setPermCode)
        .def("fromPermCode", &NPerm2::fromPermCode)
        .def("isPermCode", &NPerm2::isPermCode)
        .def(self * self)
        .def("inverse", &NPerm2::inverse)
        .def("reverse", &NPerm2::reverse)
        .def("sign", &NPerm2::sign)
        .def("__getitem__", perm2_getItem)
        .def("preImageOf", &NPerm2::preImageOf)
        .def("compareWith", &NPerm2::compareWith)
        .def("isIdentity", &NPerm2::isIdentity)
        .def("atIndex", &NPerm2::atIndex)
        .def("index", &NPerm2::index)
        .def("rand", &NPerm2::rand)
        .def("str", &NPerm2::str)
        .def("trunc", &NPerm2::trunc)
        .def("S2Index", &NPerm2::S2Index)
        .def("orderedS2Index", &NPerm2::orderedS2Index)
        .def("orderedSnIndex", &NPerm2::orderedS2Index)
        .def("__str__", &NPerm2::str)
        .def("__repr__", &NPerm2::str)
        .def(regina::python::add_eq_operators())
        .staticmethod("fromPermCode")
        .staticmethod("isPermCode")
        .staticmethod("atIndex")
        .staticmethod("rand")
    ;

    s.attr("nPerms") = NPerm2::nPerms;
    s.attr("nPerms_1") = NPerm2::nPerms_1;

    s.attr("S2") = &NPerm2_S2_arr;
    s.attr("Sn") = &NPerm2_S2_arr;
    s.attr("orderedS2") = &NPerm2_S2_arr;
    s.attr("orderedSn") = &NPerm2_S2_arr;
    s.attr("invS2") = &NPerm2_invS2_arr;
    s.attr("invSn") = &NPerm2_invS2_arr;
    s.attr("S1") = &NPerm2_S1_arr;
    s.attr("Sn_1") = &NPerm2_S1_arr;
}

