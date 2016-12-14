
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
#include "subcomplex/satregion.h"
#include <iostream>
#include "../helpers.h"

using namespace boost::python;
using regina::SatBlock;
using regina::SatBlockSpec;
using regina::SatRegion;

namespace {
    boost::python::tuple boundaryAnnulus_tuple(const SatRegion& r,
            unsigned which) {
        SatBlock* block;
        unsigned annulus;
        bool blockRefVert, blockRefHoriz;

        r.boundaryAnnulus(which, block, annulus, blockRefVert, blockRefHoriz);

        return boost::python::make_tuple(
            ptr(block), annulus, blockRefVert, blockRefHoriz);
    }

    bool expand_nolist(SatRegion& r, bool stopIfIncomplete = false) {
        SatBlock::TetList avoidTets;
        return r.expand(avoidTets, stopIfIncomplete);
    }

    void writeBlockAbbrs_stdio(const SatRegion& r, bool tex = false) {
        r.writeBlockAbbrs(std::cout, tex);
    }

    void writeDetail_stdio(const SatRegion& r, const std::string& title) {
        r.writeDetail(std::cout, title);
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_expand, expand_nolist, 1, 2);

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeBlockAbbrs,
        writeBlockAbbrs_stdio, 1, 2);
}

void addSatRegion() {
    class_<SatBlockSpec>("SatBlockSpec")
        .def(init<SatBlock*, bool, bool>())
        .def_readonly("block", &SatBlockSpec::block)
        .def_readonly("refVert", &SatBlockSpec::refVert)
        .def_readonly("refHoriz", &SatBlockSpec::refHoriz)
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NSatBlockSpec") = scope().attr("SatBlockSpec");

    class_<SatRegion, boost::noncopyable,
            std::auto_ptr<SatRegion> >("SatRegion", init<SatBlock*>())
        .def("numberOfBlocks", &SatRegion::numberOfBlocks)
        .def("block", &SatRegion::block, return_internal_reference<>())
        .def("blockIndex", &SatRegion::blockIndex)
        .def("numberOfBoundaryAnnuli", &SatRegion::numberOfBoundaryAnnuli)
        .def("boundaryAnnulus", boundaryAnnulus_tuple)
        .def("createSFS", &SatRegion::createSFS,
            return_value_policy<manage_new_object>())
        .def("expand", expand_nolist, OL_expand())
        .def("writeBlockAbbrs", writeBlockAbbrs_stdio, OL_writeBlockAbbrs())
        .def("writeDetail", writeDetail_stdio)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NSatRegion") = scope().attr("SatRegion");

}

