
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

#include "triangulation/nfacepair.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NFacePair;

namespace {
    void facepair_inc_operator(NFacePair& p) {
        p++;
    }

    void facepair_dec_operator(NFacePair& p) {
        p--;
    }
}

void addNFacePair() {
    class_<NFacePair>("NFacePair")
        .def(init<int, int>())
        .def(init<const NFacePair&>())
        .def("lower", &NFacePair::lower)
        .def("upper", &NFacePair::upper)
        .def("isBeforeStart", &NFacePair::isBeforeStart)
        .def("isPastEnd", &NFacePair::isPastEnd)
        .def("complement", &NFacePair::complement)
        .def(self == self)
        .def(self < self)
        .def(self > self)
        .def(self <= self)
        .def(self >= self)
        .def("inc", facepair_inc_operator)
        .def("dec", facepair_dec_operator)
    ;
}

