
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "census/dim4census.h"
#include "dim4/dim4triangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4Census;

namespace {
    unsigned long formCensus(regina::NPacket* p, unsigned n,
            regina::NBoolSet fin, regina::NBoolSet orb, regina::NBoolSet bdr,
            int bf) {
        return Dim4Census::formCensus(p, n, fin, orb, bdr, bf);
    }
    unsigned long formPartialCensus(const regina::Dim4FacetPairing* fp,
            regina::NPacket* p, regina::NBoolSet fin, regina::NBoolSet orb) {
        return Dim4Census::formPartialCensus(fp, p, fin, orb);
    }
}

void addDim4Census() {
    scope s = class_<Dim4Census, std::auto_ptr<Dim4Census>,
            boost::noncopyable>("Dim4Census", no_init)
        .def("formCensus", formCensus)
        .def("formPartialCensus", formPartialCensus)
        .staticmethod("formCensus")
        .staticmethod("formPartialCensus")
    ;
}

