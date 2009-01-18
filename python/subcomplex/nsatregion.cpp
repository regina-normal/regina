
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

#include "manifold/nsfs.h"
#include "subcomplex/nsatregion.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NSatBlock;
using regina::NSatBlockSpec;
using regina::NSatRegion;

namespace {
    boost::python::tuple boundaryAnnulus_tuple(const NSatRegion& r,
            unsigned which) {
        NSatBlock* block;
        unsigned annulus;
        bool blockRefVert, blockRefHoriz;

        r.boundaryAnnulus(which, block, annulus, blockRefVert, blockRefHoriz);

        return make_tuple(ptr(block), annulus, blockRefVert, blockRefHoriz);
    }

    bool expand_nolist(NSatRegion& r, bool stopIfIncomplete = false) {
        NSatBlock::TetList avoidTets;
        return r.expand(avoidTets, stopIfIncomplete);
    }

    void writeBlockAbbrs_stdio(const NSatRegion& r, bool tex = false) {
        r.writeBlockAbbrs(std::cout, tex);
    }

    void writeDetail_stdio(const NSatRegion& r, const std::string& title) {
        r.writeDetail(std::cout, title);
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_expand, expand_nolist, 1, 2);

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeBlockAbbrs,
        writeBlockAbbrs_stdio, 1, 2);
}

void addNSatRegion() {
    class_<NSatBlockSpec>("NSatBlockSpec")
        .def(init<NSatBlock*, bool, bool>())
        .def_readonly("block", &NSatBlockSpec::block)
        .def_readonly("refVert", &NSatBlockSpec::refVert)
        .def_readonly("refHoriz", &NSatBlockSpec::refHoriz)
    ;

    class_<NSatRegion, boost::noncopyable, bases<regina::ShareableObject>,
            std::auto_ptr<NSatRegion> >("NSatRegion", init<NSatBlock*>())
        .def("numberOfBlocks", &NSatRegion::numberOfBlocks)
        .def("block", &NSatRegion::block, return_internal_reference<>())
        .def("blockIndex", &NSatRegion::blockIndex)
        .def("numberOfBoundaryAnnuli", &NSatRegion::numberOfBoundaryAnnuli)
        .def("boundaryAnnulus", boundaryAnnulus_tuple)
        .def("createSFS", &NSatRegion::createSFS,
            return_value_policy<manage_new_object>())
        .def("expand", expand_nolist, OL_expand())
        .def("writeBlockAbbrs", writeBlockAbbrs_stdio, OL_writeBlockAbbrs())
        .def("writeDetail", writeDetail_stdio)
    ;
}

