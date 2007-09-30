
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "census/ncensus.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NCensus;

namespace {
    unsigned long formCensus(regina::NPacket* p, unsigned n,
            regina::NBoolSet fin, regina::NBoolSet orb, regina::NBoolSet bdr,
            int bf, int wp) {
        return NCensus::formCensus(p, n, fin, orb, bdr, bf, wp);
    }
    unsigned long formPartialCensus(const regina::NFacePairing* fp,
            regina::NPacket* p, regina::NBoolSet fin, regina::NBoolSet orb,
            int wp) {
        return NCensus::formPartialCensus(fp, p, fin, orb, wp);
    }
    bool mightBeMinimal(regina::NTriangulation* t) {
        return NCensus::mightBeMinimal(t, 0);
    }
    unsigned long findAllCompletions(regina::NPacket* p,
            regina::NTriangulation* b, regina::NBoolSet fin,
            regina::NBoolSet orb) {
        return NCensus::findAllCompletions(p, b, fin, orb);
    }
}

void addNCensus() {
    scope s = class_<NCensus, std::auto_ptr<NCensus>,
            boost::noncopyable>("NCensus", no_init)
        .def("formCensus", formCensus)
        .def("formPartialCensus", formPartialCensus)
        .def("mightBeMinimal", mightBeMinimal)
        .def("findAllCompletions", findAllCompletions)
        .staticmethod("formCensus")
        .staticmethod("formPartialCensus")
        .staticmethod("mightBeMinimal")
        .staticmethod("findAllCompletions")
    ;

    s.attr("PURGE_NON_MINIMAL") = NCensus::PURGE_NON_MINIMAL;
    s.attr("PURGE_NON_PRIME") = NCensus::PURGE_NON_PRIME;
    s.attr("PURGE_NON_MINIMAL_PRIME") = NCensus::PURGE_NON_MINIMAL_PRIME;
    s.attr("PURGE_P2_REDUCIBLE") = NCensus::PURGE_P2_REDUCIBLE;
}

