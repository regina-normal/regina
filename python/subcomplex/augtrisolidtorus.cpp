
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
#include "manifold/sfs.h"
#include "subcomplex/augtrisolidtorus.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::AugTriSolidTorus;

void addAugTriSolidTorus(pybind11::module_& m) {
    pybind11::class_<AugTriSolidTorus, regina::StandardTriangulation>
            (m, "AugTriSolidTorus")
        .def("clone", &AugTriSolidTorus::clone)
        .def("core", &AugTriSolidTorus::core,
            pybind11::return_value_policy::reference_internal)
        .def("augTorus", &AugTriSolidTorus::augTorus,
            pybind11::return_value_policy::reference_internal)
        .def("edgeGroupRoles", &AugTriSolidTorus::edgeGroupRoles)
        .def("chainLength", &AugTriSolidTorus::chainLength)
        .def("chainType", &AugTriSolidTorus::chainType)
        .def("torusAnnulus", &AugTriSolidTorus::torusAnnulus)
        .def("hasLayeredChain", &AugTriSolidTorus::hasLayeredChain)
        .def_static("isAugTriSolidTorus", &AugTriSolidTorus::isAugTriSolidTorus)
        .def_readonly_static("CHAIN_NONE", &AugTriSolidTorus::CHAIN_NONE)
        .def_readonly_static("CHAIN_MAJOR", &AugTriSolidTorus::CHAIN_MAJOR)
        .def_readonly_static("CHAIN_AXIS", &AugTriSolidTorus::CHAIN_AXIS)
    ;

    m.attr("NAugTriSolidTorus") = m.attr("AugTriSolidTorus");
}

