
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "dim4/nperm5.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPerm5;
using regina::python::GlobalArray;

namespace {
    GlobalArray<NPerm5> NPerm5_S5_arr(NPerm5::S5, 120);
    GlobalArray<NPerm5> NPerm5_orderedS5_arr(NPerm5::orderedS5, 120);
    GlobalArray<unsigned> NPerm5_invS5_arr(NPerm5::invS5, 120);

    int perm5_getItem(const NPerm5& p, int index) {
        return p[index];
    }
}

void addNPerm5() {
    scope s = class_<NPerm5>("NPerm5")
        .def(init<unsigned>())
        .def(init<int, int>())
        .def(init<int, int, int, int, int>())
        .def(init<int, int, int, int, int, int, int, int, int, int>())
        .def(init<const NPerm5&>())
        .def("getPermCode", &NPerm5::getPermCode)
        .def("setPermCode", &NPerm5::setPermCode)
        .def("isPermCode", &NPerm5::isPermCode)
        .def(self * self)
        .def("inverse", &NPerm5::inverse)
        .def("sign", &NPerm5::sign)
        .def("__getitem__", perm5_getItem)
        .def("preImageOf", &NPerm5::preImageOf)
        .def(self == self)
        .def(self != self)
        .def("compareWith", &NPerm5::compareWith)
        .def("isIdentity", &NPerm5::isIdentity)
        .def("asPerm4", &NPerm5::asPerm4)
        .def("fromPerm4", &NPerm5::fromPerm4)
        .def("toString", &NPerm5::toString)
        .def("trunc2", &NPerm5::trunc2)
        .def("trunc3", &NPerm5::trunc3)
        .def("trunc4", &NPerm5::trunc4)
        .def("S5Index", &NPerm5::S5Index)
        .def("orderedS5Index", &NPerm5::orderedS5Index)
        .def("__str__", &NPerm5::toString)
        .staticmethod("fromPerm4");
    ;

    s.attr("S5") = &NPerm5_S5_arr;
    s.attr("orderedS5") = &NPerm5_orderedS5_arr;
    s.attr("invS5") = &NPerm5_invS5_arr;
}

