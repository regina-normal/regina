
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

#include <boost/python.hpp>
#include "triangulation/nfacepair.h"
#include "../equality.h"

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
        .def(self < self)
        .def(self > self)
        .def(self <= self)
        .def(self >= self)
        .def("inc", facepair_inc_operator)
        .def("dec", facepair_dec_operator)
        .def(regina::python::add_eq_operators())
    ;
}

