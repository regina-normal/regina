
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include "triangulation/nperm.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPerm;
using regina::python::GlobalArray;

namespace {
    GlobalArray<NPerm> allPermsS4_arr(regina::allPermsS4, 24);
    GlobalArray<unsigned> allPermsS4Inv_arr(regina::allPermsS4Inv, 24);
    GlobalArray<NPerm> orderedPermsS4_arr(regina::orderedPermsS4, 24);
    GlobalArray<NPerm> allPermsS3_arr(regina::allPermsS3, 6);
    GlobalArray<unsigned> allPermsS3Inv_arr(regina::allPermsS3Inv, 6);
    GlobalArray<NPerm> orderedPermsS3_arr(regina::orderedPermsS3, 6);
    GlobalArray<NPerm> allPermsS2_arr(regina::allPermsS2, 2);
    GlobalArray<unsigned> allPermsS2Inv_arr(regina::allPermsS2Inv, 2);

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
    class_<NPerm>("NPerm")
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
        .def("__str__", &NPerm::toString)
    ;

    // Global arrays:
    scope().attr("allPermsS4") = &allPermsS4_arr;
    scope().attr("allPermsS4Inv") = &allPermsS4Inv_arr;
    scope().attr("orderedPermsS4") = &orderedPermsS4_arr;
    scope().attr("allPermsS3") = &allPermsS3_arr;
    scope().attr("allPermsS3Inv") = &allPermsS3Inv_arr;
    scope().attr("orderedPermsS3") = &orderedPermsS3_arr;
    scope().attr("allPermsS2") = &allPermsS2_arr;
    scope().attr("allPermsS2Inv") = &allPermsS2Inv_arr;

    // Global functions:
    def("faceOrdering", regina::faceOrdering);
    def("edgeOrdering", regina::edgeOrdering);
    def("faceDescription", faceDescription_int);
    def("faceDescription", faceDescription_perm);
    def("edgeDescription", edgeDescription_int);
    def("edgeDescription", edgeDescription_perm);
}

