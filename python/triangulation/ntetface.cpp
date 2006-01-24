
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include "triangulation/ntetface.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NTetFace;

namespace {
    NTetFace tetface_inc_operator(NTetFace& p) {
        return p++;
    }

    NTetFace tetface_dec_operator(NTetFace& p) {
        return p--;
    }
}

void addNTetFace() {
    class_<NTetFace>("NTetFace")
        .def(init<int, int>())
        .def(init<const NTetFace&>())
        .def_readwrite("tet", &NTetFace::tet)
        .def_readwrite("face", &NTetFace::face)
        .def("isBoundary", &NTetFace::isBoundary)
        .def("isBeforeStart", &NTetFace::isBeforeStart)
        .def("isPastEnd", &NTetFace::isPastEnd)
        .def("setFirst", &NTetFace::setFirst)
        .def("setBoundary", &NTetFace::setBoundary)
        .def("setBeforeStart", &NTetFace::setBeforeStart)
        .def("setPastEnd", &NTetFace::setPastEnd)
        .def("inc", tetface_inc_operator)
        .def("dec", tetface_dec_operator)
        .def(self == self)
        .def(self < self)
        .def(self <= self)
    ;
}

