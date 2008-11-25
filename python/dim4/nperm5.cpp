
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

#include "dim4/nperm5.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPerm5;

namespace {
    int perm5_getItem(const NPerm5& p, int index) {
        return p[index];
    }
}

void addNPerm5() {
    class_<NPerm5>("NPerm5")
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
        .def("toString", &NPerm5::toString)
        .def("__str__", &NPerm5::toString)
    ;
}

