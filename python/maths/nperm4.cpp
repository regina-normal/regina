
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

#include "maths/nperm4.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPerm4;
using regina::python::GlobalArray;

namespace {
    GlobalArray<NPerm4> NPerm4_S4_arr(NPerm4::S4, 24);
    GlobalArray<unsigned> NPerm4_invS4_arr(NPerm4::invS4, 24);
    GlobalArray<NPerm4> NPerm4_orderedS4_arr(NPerm4::orderedS4, 24);
    GlobalArray<NPerm4> NPerm4_S3_arr(NPerm4::S3, 6);
    GlobalArray<unsigned> NPerm4_invS3_arr(NPerm4::invS3, 6);
    GlobalArray<NPerm4> NPerm4_orderedS3_arr(NPerm4::orderedS3, 6);
    GlobalArray<NPerm4> NPerm4_S2_arr(NPerm4::S2, 2);
    GlobalArray<unsigned> NPerm4_invS2_arr(NPerm4::invS2, 2);

    void (NPerm4::*setPerm_pair)(int, int) = &NPerm4::setPerm;
    void (NPerm4::*setPerm_quartet)(int, int, int, int) = &NPerm4::setPerm;
    int (NPerm4::*S4Index_void)() const = &NPerm4::S4Index;
    std::string (*faceDescription_int)(int) = &regina::faceDescription;
    std::string (*faceDescription_perm)(const NPerm4&) =
        &regina::faceDescription;
    std::string (*edgeDescription_int)(int) = &regina::edgeDescription;
    std::string (*edgeDescription_perm)(const NPerm4&) =
        &regina::edgeDescription;

    int perm_getItem(const NPerm4& p, int index) {
        return p[index];
    }
}

void addNPerm4() {
    // Global arrays:
    scope().attr("allPermsS4") = &NPerm4_S4_arr;
    scope().attr("allPermsS4Inv") = &NPerm4_invS4_arr;
    scope().attr("orderedPermsS4") = &NPerm4_orderedS4_arr;
    scope().attr("allPermsS3") = &NPerm4_S3_arr;
    scope().attr("allPermsS3Inv") = &NPerm4_invS3_arr;
    scope().attr("orderedPermsS3") = &NPerm4_orderedS3_arr;
    scope().attr("allPermsS2") = &NPerm4_S2_arr;
    scope().attr("allPermsS2Inv") = &NPerm4_invS2_arr;

    // Global functions:
    def("faceOrdering", regina::faceOrdering);
    def("edgeOrdering", regina::edgeOrdering);
    def("faceDescription", faceDescription_int);
    def("faceDescription", faceDescription_perm);
    def("edgeDescription", edgeDescription_int);
    def("edgeDescription", edgeDescription_perm);

    // Classes:
    {
        scope s = class_<NPerm4>("NPerm4")
            .def(init<int, int>())
            .def(init<int, int, int, int>())
            .def(init<int, int, int, int, int, int, int, int>())
            .def(init<const NPerm4&>())
            .def("getPermCode", &NPerm4::getPermCode)
            .def("getPermCode2", &NPerm4::getPermCode2)
            .def("setPermCode", &NPerm4::setPermCode)
            .def("setPermCode2", &NPerm4::setPermCode2)
            .def("fromPermCode", &NPerm4::fromPermCode)
            .def("fromPermCode2", &NPerm4::fromPermCode2)
            .def("isPermCode", &NPerm4::isPermCode)
            .def("isPermCode2", &NPerm4::isPermCode2)
            .def("setPerm", setPerm_pair)
            .def("setPerm", setPerm_quartet)
            .def(self * self)
            .def("inverse", &NPerm4::inverse)
            .def("sign", &NPerm4::sign)
            .def("__getitem__", perm_getItem)
            .def("preImageOf", &NPerm4::preImageOf)
            .def(self == self)
            .def(self != self)
            .def("compareWith", &NPerm4::compareWith)
            .def("isIdentity", &NPerm4::isIdentity)
            .def("toString", &NPerm4::toString)
            .def("trunc2", &NPerm4::toString)
            .def("trunc3", &NPerm4::toString)
            .def("S4Index", S4Index_void)
            .def("orderedS4Index", &NPerm4::orderedS4Index)
            .def("__str__", &NPerm4::toString)
            .staticmethod("fromPermCode")
            .staticmethod("fromPermCode2")
            .staticmethod("isPermCode")
            .staticmethod("isPermCode2")
        ;

        s.attr("S4") = &NPerm4_S4_arr;
        s.attr("invS4") = &NPerm4_invS4_arr;
        s.attr("orderedS4") = &NPerm4_orderedS4_arr;
        s.attr("S3") = &NPerm4_S3_arr;
        s.attr("invS3") = &NPerm4_invS3_arr;
        s.attr("orderedS3") = &NPerm4_orderedS3_arr;
        s.attr("S2") = &NPerm4_S2_arr;
        s.attr("invS2") = &NPerm4_invS2_arr;
    }

    // Support for deprecated typedef:
    scope().attr("NPerm") = scope().attr("NPerm4");
}

