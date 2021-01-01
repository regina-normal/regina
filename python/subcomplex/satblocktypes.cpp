
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "../pybind11/pybind11.h"
#include "subcomplex/layeredsolidtorus.h"
#include "subcomplex/satblocktypes.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::SatAnnulus;
using regina::SatBlock;
using regina::SatCube;
using regina::SatLayering;
using regina::SatLST;
using regina::SatMobius;
using regina::SatReflectorStrip;
using regina::SatTriPrism;

void addSatBlockTypes(pybind11::module_& m) {
    pybind11::class_<SatMobius, regina::SatBlock>(m, "SatMobius")
        .def(pybind11::init<const SatMobius&>())
        .def("position", &SatMobius::position)
        .def_static("isBlockMobius", [](const SatAnnulus& a) {
            SatBlock::TetList avoidTets;
            return SatMobius::isBlockMobius(a, avoidTets);
        })
    ;

    m.attr("NSatMobius") = m.attr("SatMobius");


    pybind11::class_<SatLST, regina::SatBlock>(m, "SatLST")
        .def(pybind11::init<const SatLST&>())
        .def("lst", &SatLST::lst,
            pybind11::return_value_policy::reference_internal)
        .def("roles", &SatLST::roles)
        .def_static("isBlockLST", [](const SatAnnulus& a) {
            SatBlock::TetList avoidTets;
            return SatLST::isBlockLST(a, avoidTets);
        })
    ;

    m.attr("NSatLST") = m.attr("SatLST");


    pybind11::class_<SatTriPrism, regina::SatBlock>(m, "SatTriPrism")
        .def(pybind11::init<const SatTriPrism&>())
        .def("isMajor", &SatTriPrism::isMajor)
        .def_static("isBlockTriPrism", [](const SatAnnulus& a) {
            SatBlock::TetList avoidTets;
            return SatTriPrism::isBlockTriPrism(a, avoidTets);
        })
        .def_static("insertBlock", &SatTriPrism::insertBlock)
    ;

    m.attr("NSatTriPrism") = m.attr("SatTriPrism");


    pybind11::class_<SatCube, regina::SatBlock>(m, "SatCube")
        .def(pybind11::init<const SatCube&>())
        .def_static("isBlockCube", [](const SatAnnulus& a) {
            SatBlock::TetList avoidTets;
            return SatCube::isBlockCube(a, avoidTets);
        })
        .def_static("insertBlock", &SatCube::insertBlock)
    ;

    m.attr("NSatCube") = m.attr("SatCube");


    pybind11::class_<SatReflectorStrip, regina::SatBlock>(
            m, "SatReflectorStrip")
        .def(pybind11::init<const SatReflectorStrip&>())
        .def_static("isBlockReflectorStrip", [](const SatAnnulus& a) {
            SatBlock::TetList avoidTets;
            return SatReflectorStrip::isBlockReflectorStrip(a, avoidTets);
        })
        .def_static("insertBlock", &SatReflectorStrip::insertBlock)
    ;

    m.attr("NSatReflectorStrip") = m.attr("SatReflectorStrip");


    pybind11::class_<SatLayering, regina::SatBlock>(m, "SatLayering")
        .def(pybind11::init<const SatLayering&>())
        .def("overHorizontal", &SatLayering::overHorizontal)
        .def_static("isBlockLayering", [](const SatAnnulus& a) {
            SatBlock::TetList avoidTets;
            return SatLayering::isBlockLayering(a, avoidTets);
        })
    ;

    m.attr("NSatLayering") = m.attr("SatLayering");
}

