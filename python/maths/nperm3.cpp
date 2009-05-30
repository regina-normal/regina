
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

#include "maths/nperm3.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPerm3;
using regina::python::GlobalArray;

namespace {
    GlobalArray<NPerm3> NPerm3_S3_arr(NPerm3::S3, 6);
    GlobalArray<NPerm3> NPerm3_orderedS3_arr(NPerm3::orderedS3, 6);
    GlobalArray<int> NPerm3_invS3_arr(NPerm3::invS3, 6);

    int perm3_getItem(const NPerm3& p, int index) {
        return p[index];
    }
}

void addNPerm3() {
    scope s = class_<NPerm3>("NPerm3")
        .def(init<int>())
        .def(init<int, int, int>())
        .def(init<const NPerm3&>())
        .def("getPermCode", &NPerm3::getPermCode)
        .def("setPermCode", &NPerm3::setPermCode)
        .def("isPermCode", &NPerm3::isPermCode)
        .def(self * self)
        .def("inverse", &NPerm3::inverse)
        .def("sign", &NPerm3::sign)
        .def("__getitem__", perm3_getItem)
        .def("preImageOf", &NPerm3::preImageOf)
        .def(self == self)
        .def(self != self)
        .def("isIdentity", &NPerm3::isIdentity)
        .def("toString", &NPerm3::toString)
        .def("trunc2", &NPerm3::trunc2)
        .def("S3Index", &NPerm3::S3Index)
        .def("orderedS3Index", &NPerm3::orderedS3Index)
        .def("__str__", &NPerm3::toString)
        .staticmethod("isPermCode")
    ;

    s.attr("S3") = &NPerm3_S3_arr;
    s.attr("orderedS3") = &NPerm3_orderedS3_arr;
    s.attr("invS3") = &NPerm3_invS3_arr;
}

