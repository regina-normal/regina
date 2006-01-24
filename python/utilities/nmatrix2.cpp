
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

#include "utilities/nmatrix2.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NMatrix2;

void addNMatrix2() {
    class_<NMatrix2>("NMatrix2")
        .def(init<const NMatrix2&>())
        .def(init<long, long, long, long>())
        .def(self * self)
        .def(self * long())
        .def(self + self)
        .def(self - self)
        .def(- self)
        .def("transpose", &NMatrix2::transpose)
        .def("inverse", &NMatrix2::inverse)
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self *= long())
        .def("negate", &NMatrix2::negate)
        .def("invert", &NMatrix2::invert)
        .def(self == self)
        .def(self != self)
        .def("determinant", &NMatrix2::determinant)
        .def("isIdentity", &NMatrix2::isIdentity)
        .def("isZero", &NMatrix2::isZero)
        .def(self_ns::str(self))
    ;
}

