
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "census/nfacepairing.h"
#include "triangulation/nfacepair.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NFacePairing;
using regina::NTetFace;

namespace {
    const NTetFace& (NFacePairing::*dest_face)(const NTetFace&) const =
        &NFacePairing::dest;
    const NTetFace& (NFacePairing::*dest_unsigned)(unsigned, unsigned) const =
        &NFacePairing::dest;
    bool (NFacePairing::*isUnmatched_face)(const NTetFace&) const =
        &NFacePairing::isUnmatched;
    bool (NFacePairing::*isUnmatched_unsigned)(unsigned, unsigned) const =
        &NFacePairing::isUnmatched;

    bool (NFacePairing::*query_bdec)() const =
        &NFacePairing::hasBrokenDoubleEndedChain;
    bool (NFacePairing::*query_oecwdh)() const =
        &NFacePairing::hasOneEndedChainWithDoubleHandle;
    bool (NFacePairing::*query_wdec)() const =
        &NFacePairing::hasWedgedDoubleEndedChain;

    const NTetFace& getItem(const NFacePairing& p, const NTetFace& index) {
        return p[index];
    }
}

void addNFacePairing() {
    class_<NFacePairing, std::auto_ptr<NFacePairing>, boost::noncopyable>
            ("NFacePairing", init<const NFacePairing&>())
        .def("getNumberOfTetrahedra", &NFacePairing::getNumberOfTetrahedra)
        .def("dest", dest_face,
            return_value_policy<reference_existing_object>())
        .def("dest", dest_unsigned,
            return_value_policy<reference_existing_object>())
        .def("__getitem__", getItem,
            return_value_policy<reference_existing_object>())
        .def("isUnmatched", isUnmatched_face)
        .def("isUnmatched", isUnmatched_unsigned)
        .def("isCanonical", &NFacePairing::isCanonical)
        .def("toString", &NFacePairing::toString)
        .def("toTextRep", &NFacePairing::toTextRep)
        .def("fromTextRep", &NFacePairing::fromTextRep,
            return_value_policy<manage_new_object>())
        .def("isClosed", &NFacePairing::isClosed)
        .def("hasTripleEdge", &NFacePairing::hasTripleEdge)
        .def("followChain", &NFacePairing::followChain)

        // For reasons I do not understand, it seems I have to declare
        // these routines separately to avoid a compile error with
        // g++ 3.3.  *shrug*
        .def("hasBrokenDoubleEndedChain", query_bdec)
        .def("hasOneEndedChainWithDoubleHandle", query_oecwdh)
        .def("hasWedgedDoubleEndedChain", query_wdec)

        .def("__str__", &NFacePairing::toString)
        .staticmethod("fromTextRep")
    ;
}

