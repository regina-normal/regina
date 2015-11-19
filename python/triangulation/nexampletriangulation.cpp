
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include <boost/python.hpp>
#include "triangulation/nexampletriangulation.h"
#include "triangulation/ntriangulation.h"
#include "../equality.h"

using namespace boost::python;
using regina::NExampleTriangulation;
using regina::NTriangulation;

void addNExampleTriangulation() {
    class_<NExampleTriangulation>("NExampleTriangulation", no_init)
        .def("threeSphere", &NExampleTriangulation::threeSphere,
            return_value_policy<manage_new_object>())
        .def("bingsHouse", &NExampleTriangulation::bingsHouse,
            return_value_policy<manage_new_object>())
        .def("s2xs1", &NExampleTriangulation::s2xs1,
            return_value_policy<manage_new_object>())
        .def("rp2xs1", &NExampleTriangulation::rp2xs1,
            return_value_policy<manage_new_object>())
        .def("rp3rp3", &NExampleTriangulation::rp3rp3,
            return_value_policy<manage_new_object>())
        .def("lens8_3", &NExampleTriangulation::lens8_3,
            return_value_policy<manage_new_object>())
        .def("poincareHomologySphere",
            &NExampleTriangulation::poincareHomologySphere,
            return_value_policy<manage_new_object>())
        .def("weeks", &NExampleTriangulation::weeks,
            return_value_policy<manage_new_object>())
        .def("seifertWeber",
            &NExampleTriangulation::seifertWeber,
            return_value_policy<manage_new_object>())
        .def("weberSeifert",
            &NExampleTriangulation::weberSeifert,
            return_value_policy<manage_new_object>())
        .def("smallClosedOrblHyperbolic",
            &NExampleTriangulation::smallClosedOrblHyperbolic,
            return_value_policy<manage_new_object>())
        .def("smallClosedNonOrblHyperbolic",
            &NExampleTriangulation::smallClosedNonOrblHyperbolic,
            return_value_policy<manage_new_object>())
        .def("sphere600", &NExampleTriangulation::sphere600,
            return_value_policy<manage_new_object>())
        .def("lst3_4_7", &NExampleTriangulation::lst3_4_7,
            return_value_policy<manage_new_object>())
        .def("solidKleinBottle", &NExampleTriangulation::solidKleinBottle,
            return_value_policy<manage_new_object>())
        .def("figureEight", &NExampleTriangulation::figureEight,
            return_value_policy<manage_new_object>())
        .def("figureEightKnotComplement",
            &NExampleTriangulation::figureEightKnotComplement,
            return_value_policy<manage_new_object>())
        .def("trefoil", &NExampleTriangulation::trefoil,
            return_value_policy<manage_new_object>())
        .def("trefoilKnotComplement",
            &NExampleTriangulation::trefoilKnotComplement,
            return_value_policy<manage_new_object>())
        .def("whiteheadLink", &NExampleTriangulation::whiteheadLink,
            return_value_policy<manage_new_object>())
        .def("whiteheadLinkComplement",
            &NExampleTriangulation::whiteheadLinkComplement,
            return_value_policy<manage_new_object>())
        .def("gieseking", &NExampleTriangulation::gieseking,
            return_value_policy<manage_new_object>())
        .def("cuspedGenusTwoTorus",
            &NExampleTriangulation::cuspedGenusTwoTorus,
            return_value_policy<manage_new_object>())
        .def(regina::python::no_eq_operators())
        .staticmethod("threeSphere")
        .staticmethod("bingsHouse")
        .staticmethod("s2xs1")
        .staticmethod("rp2xs1")
        .staticmethod("rp3rp3")
        .staticmethod("lens8_3")
        .staticmethod("poincareHomologySphere")
        .staticmethod("weeks")
        .staticmethod("seifertWeber")
        .staticmethod("weberSeifert")
        .staticmethod("smallClosedOrblHyperbolic")
        .staticmethod("smallClosedNonOrblHyperbolic")
        .staticmethod("sphere600")
        .staticmethod("lst3_4_7")
        .staticmethod("solidKleinBottle")
        .staticmethod("figureEight")
        .staticmethod("figureEightKnotComplement")
        .staticmethod("trefoil")
        .staticmethod("trefoilKnotComplement")
        .staticmethod("whiteheadLink")
        .staticmethod("whiteheadLinkComplement")
        .staticmethod("gieseking")
        .staticmethod("cuspedGenusTwoTorus")
    ;
}

