
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "census/dim2census.h"
#include "dim2/dim2triangulation.h"

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
        .staticmethod("formCensus")
        .staticmethod("formPartialCensus")
    ;
}

