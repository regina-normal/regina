
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

#include "subcomplex/nlayeredsolidtorus.h"
#include "subcomplex/nsatblocktypes.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NSatAnnulus;
using regina::NSatBlock;
using regina::NSatCube;
using regina::NSatLayering;
using regina::NSatLST;
using regina::NSatMobius;
using regina::NSatReflectorStrip;
using regina::NSatTriPrism;

namespace {
    NSatMobius* isBlockMobius_nolist(const NSatAnnulus& a) {
        NSatBlock::TetList avoidTets;
        return NSatMobius::isBlockMobius(a, avoidTets);
    }

    NSatLST* isBlockLST_nolist(const NSatAnnulus& a) {
        NSatBlock::TetList avoidTets;
        return NSatLST::isBlockLST(a, avoidTets);
    }

    NSatTriPrism* isBlockTriPrism_nolist(const NSatAnnulus& a) {
        NSatBlock::TetList avoidTets;
        return NSatTriPrism::isBlockTriPrism(a, avoidTets);
    }

    NSatCube* isBlockCube_nolist(const NSatAnnulus& a) {
        NSatBlock::TetList avoidTets;
        return NSatCube::isBlockCube(a, avoidTets);
    }

    NSatReflectorStrip* isBlockReflectorStrip_nolist(const NSatAnnulus& a) {
        NSatBlock::TetList avoidTets;
        return NSatReflectorStrip::isBlockReflectorStrip(a, avoidTets);
    }

    NSatLayering* isBlockLayering_nolist(const NSatAnnulus& a) {
        NSatBlock::TetList avoidTets;
        return NSatLayering::isBlockLayering(a, avoidTets);
    }
}

void addNSatBlockTypes() {
    class_<NSatMobius, bases<regina::NSatBlock>,
            std::auto_ptr<NSatMobius>, boost::noncopyable>
            ("NSatMobius", init<const NSatMobius&>())
        .def("position", &NSatMobius::position)
        .def("isBlockMobius", isBlockMobius_nolist,
            return_value_policy<manage_new_object>())
        .staticmethod("isBlockMobius")
    ;

    implicitly_convertible<std::auto_ptr<NSatMobius>,
        std::auto_ptr<regina::NSatBlock> >();



    class_<NSatLST, bases<regina::NSatBlock>,
            std::auto_ptr<NSatLST>, boost::noncopyable>
            ("NSatLST", init<const NSatLST&>())
        .def("lst", &NSatLST::lst,
            return_internal_reference<>())
        .def("roles", &NSatLST::roles)
        .def("isBlockLST", isBlockLST_nolist,
            return_value_policy<manage_new_object>())
        .staticmethod("isBlockLST")
    ;

    implicitly_convertible<std::auto_ptr<NSatLST>,
        std::auto_ptr<regina::NSatBlock> >();



    class_<NSatTriPrism, bases<regina::NSatBlock>,
            std::auto_ptr<NSatTriPrism>, boost::noncopyable>
            ("NSatTriPrism", init<const NSatTriPrism&>())
        .def("isMajor", &NSatTriPrism::isMajor)
        .def("isBlockTriPrism", isBlockTriPrism_nolist,
            return_value_policy<manage_new_object>())
        .def("insertBlock", &NSatTriPrism::insertBlock,
            return_value_policy<manage_new_object>())
        .staticmethod("isBlockTriPrism")
        .staticmethod("insertBlock")
    ;

    implicitly_convertible<std::auto_ptr<NSatTriPrism>,
        std::auto_ptr<regina::NSatBlock> >();



    class_<NSatCube, bases<regina::NSatBlock>,
            std::auto_ptr<NSatCube>, boost::noncopyable>
            ("NSatCube", init<const NSatCube&>())
        .def("isBlockCube", isBlockCube_nolist,
            return_value_policy<manage_new_object>())
        .def("insertBlock", &NSatCube::insertBlock,
            return_value_policy<manage_new_object>())
        .staticmethod("isBlockCube")
        .staticmethod("insertBlock")
    ;

    implicitly_convertible<std::auto_ptr<NSatCube>,
        std::auto_ptr<regina::NSatBlock> >();



    class_<NSatReflectorStrip, bases<regina::NSatBlock>,
            std::auto_ptr<NSatReflectorStrip>, boost::noncopyable>
            ("NSatReflectorStrip", init<const NSatReflectorStrip&>())
        .def("isBlockReflectorStrip", isBlockReflectorStrip_nolist,
            return_value_policy<manage_new_object>())
        .def("insertBlock", &NSatReflectorStrip::insertBlock,
            return_value_policy<manage_new_object>())
        .staticmethod("isBlockReflectorStrip")
        .staticmethod("insertBlock")
    ;

    implicitly_convertible<std::auto_ptr<NSatReflectorStrip>,
        std::auto_ptr<regina::NSatBlock> >();



    class_<NSatLayering, bases<regina::NSatBlock>,
            std::auto_ptr<NSatLayering>, boost::noncopyable>
            ("NSatLayering", init<const NSatLayering&>())
        .def("overHorizontal", &NSatLayering::overHorizontal)
        .def("isBlockLayering", isBlockLayering_nolist,
            return_value_policy<manage_new_object>())
        .staticmethod("isBlockLayering")
    ;

    implicitly_convertible<std::auto_ptr<NSatLayering>,
        std::auto_ptr<regina::NSatBlock> >();
}

