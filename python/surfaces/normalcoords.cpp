
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
#include "surfaces/normalcoords.h"
#include "../helpers.h"

using namespace boost::python;

void addNormalCoords() {
    scope global;

    enum_<regina::NormalCoords>("NormalCoords")
        .value("NS_STANDARD", regina::NS_STANDARD)
        .value("NS_AN_STANDARD", regina::NS_AN_STANDARD)
        .value("NS_QUAD", regina::NS_QUAD)
        .value("NS_AN_QUAD_OCT", regina::NS_AN_QUAD_OCT)
        .value("NS_EDGE_WEIGHT", regina::NS_EDGE_WEIGHT)
        .value("NS_TRIANGLE_ARCS", regina::NS_TRIANGLE_ARCS)
        .value("NS_AN_LEGACY", regina::NS_AN_LEGACY)
        .value("NS_ORIENTED", regina::NS_ORIENTED)
        .value("NS_ORIENTED_QUAD", regina::NS_ORIENTED_QUAD)
        .value("NS_ANGLE", regina::NS_ANGLE)
        ;

    global.attr("NS_STANDARD") = regina::NS_STANDARD;
    global.attr("NS_AN_STANDARD") = regina::NS_AN_STANDARD;
    global.attr("NS_QUAD") = regina::NS_QUAD;
    global.attr("NS_AN_QUAD_OCT") = regina::NS_AN_QUAD_OCT;
    global.attr("NS_EDGE_WEIGHT") = regina::NS_EDGE_WEIGHT;
    global.attr("NS_TRIANGLE_ARCS") = regina::NS_TRIANGLE_ARCS;
    global.attr("NS_AN_LEGACY") = regina::NS_AN_LEGACY;
    global.attr("NS_ORIENTED") = regina::NS_ORIENTED;
    global.attr("NS_ORIENTED_QUAD") = regina::NS_ORIENTED_QUAD;
    global.attr("NS_ANGLE") = regina::NS_ANGLE;
}

