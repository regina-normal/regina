
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
#include "census/dim2census.h"
#include "dim2/dim2triangulation.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Dim2Census;

namespace {
    unsigned long formCensus(regina::NPacket* p, unsigned n,
            regina::NBoolSet orb, regina::NBoolSet bdr, int bf) {
        return Dim2Census::formCensus(p, n, orb, bdr, bf);
    }
    unsigned long formPartialCensus(const regina::Dim2EdgePairing* ep,
            regina::NPacket* p, regina::NBoolSet orb) {
        return Dim2Census::formPartialCensus(ep, p, orb);
    }
}

void addDim2Census() {
    scope s = class_<Dim2Census, std::auto_ptr<Dim2Census>,
            boost::noncopyable>("Dim2Census", no_init)
        .def("formCensus", formCensus)
        .def("formPartialCensus", formPartialCensus)
        .def(regina::python::no_eq_operators())
        .staticmethod("formCensus")
        .staticmethod("formPartialCensus")
    ;
}

