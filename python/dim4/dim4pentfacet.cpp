
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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
#include "generic/nfacetspec.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Dim4PentFacet;

namespace {
    Dim4PentFacet pentfacet_inc_operator(Dim4PentFacet& p) {
        return p++;
    }

    Dim4PentFacet pentfacet_dec_operator(Dim4PentFacet& p) {
        return p--;
    }
}

void addDim4PentFacet() {
    class_<Dim4PentFacet>("Dim4PentFacet")
        .def(init<int, int>())
        .def(init<const Dim4PentFacet&>())
        .def_readwrite("simp", &Dim4PentFacet::simp)
        .def_readwrite("facet", &Dim4PentFacet::facet)
        .def("isBoundary", &Dim4PentFacet::isBoundary)
        .def("isBeforeStart", &Dim4PentFacet::isBeforeStart)
        .def("isPastEnd", &Dim4PentFacet::isPastEnd)
        .def("setFirst", &Dim4PentFacet::setFirst)
        .def("setBoundary", &Dim4PentFacet::setBoundary)
        .def("setBeforeStart", &Dim4PentFacet::setBeforeStart)
        .def("setPastEnd", &Dim4PentFacet::setPastEnd)
        .def("inc", pentfacet_inc_operator)
        .def("dec", pentfacet_dec_operator)
        .def(self < self)
        .def(self <= self)
        .def(regina::python::add_eq_operators())
    ;
}

