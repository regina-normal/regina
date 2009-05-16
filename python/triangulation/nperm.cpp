
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

#include "triangulation/nperm.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPerm;
using regina::python::GlobalArray;

namespace {
    GlobalArray<NPerm> NPerm_S4_arr(NPerm::S4, 24);
    GlobalArray<unsigned> NPerm_invS4_arr(NPerm::invS4, 24);
    GlobalArray<NPerm> NPerm_orderedS4_arr(NPerm::orderedS4, 24);
    GlobalArray<NPerm> NPerm_S3_arr(NPerm::S3, 6);
    GlobalArray<unsigned> NPerm_invS3_arr(NPerm::invS3, 6);
    GlobalArray<NPerm> NPerm_orderedS3_arr(NPerm::orderedS3, 6);
    GlobalArray<NPerm> NPerm_S2_arr(NPerm::S2, 2);
    GlobalArray<unsigned> NPerm_invS2_arr(NPerm::invS2, 2);

    void (NPerm::*setPerm_pair)(int, int) = &NPerm::setPerm;
    void (NPerm::*setPerm_quartet)(int, int, int, int) = &NPerm::setPerm;
    std::string (*faceDescription_int)(int) = &regina::faceDescription;
    std::string (*faceDescription_perm)(const NPerm&) =
        &regina::faceDescription;
    std::string (*edgeDescription_int)(int) = &regina::edgeDescription;
    std::string (*edgeDescription_perm)(const NPerm&) =
        &regina::edgeDescription;

    int perm_getItem(const NPerm& p, int index) {
        return p[index];
    }
}

void addNPerm() {
    // Global arrays:
    scope().attr("allPermsS4") = &NPerm_S4_arr;
    scope().attr("allPermsS4Inv") = &NPerm_invS4_arr;
    scope().attr("orderedPermsS4") = &NPerm_orderedS4_arr;
    scope().attr("allPermsS3") = &NPerm_S3_arr;
    scope().attr("allPermsS3Inv") = &NPerm_invS3_arr;
    scope().attr("orderedPermsS3") = &NPerm_orderedS3_arr;
    scope().attr("allPermsS2") = &NPerm_S2_arr;
    scope().attr("allPermsS2Inv") = &NPerm_invS2_arr;

    // Global functions:
    def("faceOrdering", regina::faceOrdering);
    def("edgeOrdering", regina::edgeOrdering);
    def("faceDescription", faceDescription_int);
    def("faceDescription", faceDescription_perm);
    def("edgeDescription", edgeDescription_int);
    def("edgeDescription", edgeDescription_perm);

    // Classes:
    scope s = class_<NPerm>("NPerm")
        .def(init<unsigned char>())
        .def(init<int, int>())
        .def(init<int, int, int, int>())
        .def(init<int, int, int, int, int, int, int, int>())
        .def(init<const NPerm&>())
        .def("getPermCode", &NPerm::getPermCode)
        .def("setPermCode", &NPerm::setPermCode)
        .def("isPermCode", &NPerm::isPermCode)
        .def("setPerm", setPerm_pair)
        .def("setPerm", setPerm_quartet)
        .def(self * self)
        .def("inverse", &NPerm::inverse)
        .def("sign", &NPerm::sign)
        .def("__getitem__", perm_getItem)
        .def("preImageOf", &NPerm::preImageOf)
        .def(self == self)
        .def(self != self)
        .def("compareWith", &NPerm::compareWith)
        .def("isIdentity", &NPerm::isIdentity)
        .def("toString", &NPerm::toString)
        .def("trunc2", &NPerm::toString)
        .def("trunc3", &NPerm::toString)
        .def("S4Index", &NPerm::S4Index)
        .def("orderedS4Index", &NPerm::orderedS4Index)
        .def("__str__", &NPerm::toString)
    ;

    s.attr("S4") = &NPerm_S4_arr;
    s.attr("invS4") = &NPerm_invS4_arr;
    s.attr("orderedS4") = &NPerm_orderedS4_arr;
    s.attr("S3") = &NPerm_S3_arr;
    s.attr("invS3") = &NPerm_invS3_arr;
    s.attr("orderedS3") = &NPerm_orderedS3_arr;
    s.attr("S2") = &NPerm_S2_arr;
    s.attr("invS2") = &NPerm_invS2_arr;
}

