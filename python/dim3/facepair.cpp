
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "triangulation/facepair.h"
#include "../helpers.h"

using namespace boost::python;
using regina::FacePair;

namespace {
    void facepair_inc_operator(FacePair& p) {
        p++;
    }

    void facepair_dec_operator(FacePair& p) {
        p--;
    }
}

void addFacePair() {
    class_<FacePair>("FacePair")
        .def(init<int, int>())
        .def(init<const FacePair&>())
        .def("lower", &FacePair::lower)
        .def("upper", &FacePair::upper)
        .def("isBeforeStart", &FacePair::isBeforeStart)
        .def("isPastEnd", &FacePair::isPastEnd)
        .def("complement", &FacePair::complement)
        .def(self < self)
        .def(self > self)
        .def(self <= self)
        .def(self >= self)
        .def("inc", facepair_inc_operator)
        .def("dec", facepair_dec_operator)
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NFacePair") = scope().attr("FacePair");
}

