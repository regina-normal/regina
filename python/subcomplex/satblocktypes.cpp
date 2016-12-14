
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
#include "subcomplex/layeredsolidtorus.h"
#include "subcomplex/satblocktypes.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using namespace boost::python;
using regina::SatAnnulus;
using regina::SatBlock;
using regina::SatCube;
using regina::SatLayering;
using regina::SatLST;
using regina::SatMobius;
using regina::SatReflectorStrip;
using regina::SatTriPrism;

namespace {
    SatMobius* isBlockMobius_nolist(const SatAnnulus& a) {
        SatBlock::TetList avoidTets;
        return SatMobius::isBlockMobius(a, avoidTets);
    }

    SatLST* isBlockLST_nolist(const SatAnnulus& a) {
        SatBlock::TetList avoidTets;
        return SatLST::isBlockLST(a, avoidTets);
    }

    SatTriPrism* isBlockTriPrism_nolist(const SatAnnulus& a) {
        SatBlock::TetList avoidTets;
        return SatTriPrism::isBlockTriPrism(a, avoidTets);
    }

    SatCube* isBlockCube_nolist(const SatAnnulus& a) {
        SatBlock::TetList avoidTets;
        return SatCube::isBlockCube(a, avoidTets);
    }

    SatReflectorStrip* isBlockReflectorStrip_nolist(const SatAnnulus& a) {
        SatBlock::TetList avoidTets;
        return SatReflectorStrip::isBlockReflectorStrip(a, avoidTets);
    }

    SatLayering* isBlockLayering_nolist(const SatAnnulus& a) {
        SatBlock::TetList avoidTets;
        return SatLayering::isBlockLayering(a, avoidTets);
    }
}

void addSatBlockTypes() {
    class_<SatMobius, bases<regina::SatBlock>,
            std::auto_ptr<SatMobius>, boost::noncopyable>
            ("SatMobius", init<const SatMobius&>())
        .def("position", &SatMobius::position)
        .def("isBlockMobius", isBlockMobius_nolist,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
        .staticmethod("isBlockMobius")
    ;

    implicitly_convertible<std::auto_ptr<SatMobius>,
        std::auto_ptr<regina::SatBlock> >();

    scope().attr("NSatMobius") = scope().attr("SatMobius");


    class_<SatLST, bases<regina::SatBlock>,
            std::auto_ptr<SatLST>, boost::noncopyable>
            ("SatLST", init<const SatLST&>())
        .def("lst", &SatLST::lst,
            return_internal_reference<>())
        .def("roles", &SatLST::roles)
        .def("isBlockLST", isBlockLST_nolist,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
        .staticmethod("isBlockLST")
    ;

    implicitly_convertible<std::auto_ptr<SatLST>,
        std::auto_ptr<regina::SatBlock> >();

    scope().attr("NSatLST") = scope().attr("SatLST");


    class_<SatTriPrism, bases<regina::SatBlock>,
            std::auto_ptr<SatTriPrism>, boost::noncopyable>
            ("SatTriPrism", init<const SatTriPrism&>())
        .def("isMajor", &SatTriPrism::isMajor)
        .def("isBlockTriPrism", isBlockTriPrism_nolist,
            return_value_policy<manage_new_object>())
        .def("insertBlock", &SatTriPrism::insertBlock,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
        .staticmethod("isBlockTriPrism")
        .staticmethod("insertBlock")
    ;

    implicitly_convertible<std::auto_ptr<SatTriPrism>,
        std::auto_ptr<regina::SatBlock> >();

    scope().attr("NSatTriPrism") = scope().attr("SatTriPrism");


    class_<SatCube, bases<regina::SatBlock>,
            std::auto_ptr<SatCube>, boost::noncopyable>
            ("SatCube", init<const SatCube&>())
        .def("isBlockCube", isBlockCube_nolist,
            return_value_policy<manage_new_object>())
        .def("insertBlock", &SatCube::insertBlock,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
        .staticmethod("isBlockCube")
        .staticmethod("insertBlock")
    ;

    implicitly_convertible<std::auto_ptr<SatCube>,
        std::auto_ptr<regina::SatBlock> >();

    scope().attr("NSatCube") = scope().attr("SatCube");


    class_<SatReflectorStrip, bases<regina::SatBlock>,
            std::auto_ptr<SatReflectorStrip>, boost::noncopyable>
            ("SatReflectorStrip", init<const SatReflectorStrip&>())
        .def("isBlockReflectorStrip", isBlockReflectorStrip_nolist,
            return_value_policy<manage_new_object>())
        .def("insertBlock", &SatReflectorStrip::insertBlock,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
        .staticmethod("isBlockReflectorStrip")
        .staticmethod("insertBlock")
    ;

    implicitly_convertible<std::auto_ptr<SatReflectorStrip>,
        std::auto_ptr<regina::SatBlock> >();

    scope().attr("NSatReflectorStrip") = scope().attr("SatReflectorStrip");


    class_<SatLayering, bases<regina::SatBlock>,
            std::auto_ptr<SatLayering>, boost::noncopyable>
            ("SatLayering", init<const SatLayering&>())
        .def("overHorizontal", &SatLayering::overHorizontal)
        .def("isBlockLayering", isBlockLayering_nolist,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
        .staticmethod("isBlockLayering")
    ;

    implicitly_convertible<std::auto_ptr<SatLayering>,
        std::auto_ptr<regina::SatBlock> >();

    scope().attr("NSatLayering") = scope().attr("SatLayering");
}

