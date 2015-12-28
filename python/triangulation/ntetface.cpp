
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
#include "triangulation/ntetface.h"
#include "../helpers.h"

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
        .def_readwrite("tet", &NTetFace::simp)
        .def_readwrite("simp", &NTetFace::simp)
        .def_readwrite("face", &NTetFace::facet)
        .def_readwrite("facet", &NTetFace::facet)
        .def("isBoundary", &NTetFace::isBoundary)
        .def("isBeforeStart", &NTetFace::isBeforeStart)
        .def("isPastEnd", &NTetFace::isPastEnd)
        .def("setFirst", &NTetFace::setFirst)
        .def("setBoundary", &NTetFace::setBoundary)
        .def("setBeforeStart", &NTetFace::setBeforeStart)
        .def("setPastEnd", &NTetFace::setPastEnd)
        .def("inc", tetface_inc_operator)
        .def("dec", tetface_dec_operator)
        .def(self < self)
        .def(self <= self)
        .def(regina::python::add_eq_operators())
    ;
}

