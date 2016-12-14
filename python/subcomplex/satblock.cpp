
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
#include "manifold/sfs.h"
#include "subcomplex/satblock.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using namespace boost::python;
using regina::SatAnnulus;
using regina::SatBlock;

namespace {
    boost::python::tuple nextBoundaryAnnulus_tuple(SatBlock& b, unsigned a,
            bool fromPrev) {
        SatBlock* nextBlock;
        unsigned nextAnnulus;
        bool refVert, refHoriz;

        b.nextBoundaryAnnulus(a, nextBlock, nextAnnulus, refVert, refHoriz,
            fromPrev);

        return boost::python::make_tuple(
            ptr(nextBlock), nextAnnulus, refVert, refHoriz);
    }
    void writeAbbr_stdio(const SatBlock& b, bool tex = false) {
        b.writeAbbr(std::cout, tex);
    }

    SatBlock* isBlock_nolist(const SatAnnulus& a) {
        SatBlock::TetList avoidTets;
        return SatBlock::isBlock(a, avoidTets);
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_abbr,
        SatBlock::abbr, 0, 1);

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeAbbr,
        writeAbbr_stdio, 1, 2);
}

void addSatBlock() {
    class_<SatBlock, boost::noncopyable,
            std::auto_ptr<SatBlock> >("SatBlock", no_init)
        .def("clone", &SatBlock::clone,
            return_value_policy<manage_new_object>())
        .def("nAnnuli", &SatBlock::nAnnuli)
        .def("annulus", &SatBlock::annulus,
            return_internal_reference<>())
        .def("twistedBoundary", &SatBlock::twistedBoundary)
        .def("hasAdjacentBlock", &SatBlock::hasAdjacentBlock)
        .def("adjacentBlock", &SatBlock::adjacentBlock,
            return_value_policy<reference_existing_object>())
        .def("adjacentAnnulus", &SatBlock::adjacentAnnulus)
        .def("adjacentReflected", &SatBlock::adjacentReflected)
        .def("adjacentBackwards", &SatBlock::adjacentBackwards)
        .def("setAdjacent", &SatBlock::setAdjacent)
        .def("adjustSFS", &SatBlock::adjustSFS)
        .def("transform", &SatBlock::transform)
        .def("nextBoundaryAnnulus", nextBoundaryAnnulus_tuple)
        .def("abbr", &SatBlock::abbr, OL_abbr())
        .def("writeAbbr", writeAbbr_stdio, OL_writeAbbr())
        .def(self < self)
        .def("isBlock", isBlock_nolist,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("isBlock")
    ;
    scope().attr("NSatBlock") = scope().attr("SatBlock");

}

