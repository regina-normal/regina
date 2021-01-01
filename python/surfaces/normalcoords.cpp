
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
#include "surfaces/normalcoords.h"

void addNormalCoords(pybind11::module_& m) {
    pybind11::enum_<regina::NormalCoords>(m, "NormalCoords")
        .value("NS_STANDARD", regina::NS_STANDARD)
        .value("NS_AN_STANDARD", regina::NS_AN_STANDARD)
        .value("NS_QUAD", regina::NS_QUAD)
        .value("NS_QUAD_CLOSED", regina::NS_QUAD_CLOSED)
        .value("NS_AN_QUAD_OCT", regina::NS_AN_QUAD_OCT)
        .value("NS_AN_QUAD_OCT_CLOSED", regina::NS_AN_QUAD_OCT_CLOSED)
        .value("NS_EDGE_WEIGHT", regina::NS_EDGE_WEIGHT)
        .value("NS_TRIANGLE_ARCS", regina::NS_TRIANGLE_ARCS)
        .value("NS_AN_LEGACY", regina::NS_AN_LEGACY)
        .value("NS_ORIENTED", regina::NS_ORIENTED)
        .value("NS_ORIENTED_QUAD", regina::NS_ORIENTED_QUAD)
        .value("NS_ANGLE", regina::NS_ANGLE)
        .export_values()
        ;
}

