
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "triangulation/nexampletriangulation.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NExampleTriangulation;
using regina::NTriangulation;

void addNExampleTriangulation() {
    class_<NExampleTriangulation>("NExampleTriangulation", no_init)
        .def("threeSphere", &NExampleTriangulation::threeSphere,
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
        .def("smallClosedOrblHyperbolic",
            &NExampleTriangulation::smallClosedOrblHyperbolic,
            return_value_policy<manage_new_object>())
        .def("smallClosedNonOrblHyperbolic",
            &NExampleTriangulation::smallClosedNonOrblHyperbolic,
            return_value_policy<manage_new_object>())
        .def("lst3_4_7", &NExampleTriangulation::lst3_4_7,
            return_value_policy<manage_new_object>())
        .def("solidKleinBottle", &NExampleTriangulation::solidKleinBottle,
            return_value_policy<manage_new_object>())
        .def("figureEightKnotComplement",
            &NExampleTriangulation::figureEightKnotComplement,
            return_value_policy<manage_new_object>())
        .def("whiteheadLinkComplement",
            &NExampleTriangulation::whiteheadLinkComplement,
            return_value_policy<manage_new_object>())
        .def("gieseking", &NExampleTriangulation::gieseking,
            return_value_policy<manage_new_object>())
        .def("cuspedGenusTwoTorus",
            &NExampleTriangulation::cuspedGenusTwoTorus,
            return_value_policy<manage_new_object>())
        .staticmethod("threeSphere")
        .staticmethod("s2xs1")
        .staticmethod("rp2xs1")
        .staticmethod("rp3rp3")
        .staticmethod("lens8_3")
        .staticmethod("poincareHomologySphere")
        .staticmethod("smallClosedOrblHyperbolic")
        .staticmethod("smallClosedNonOrblHyperbolic")
        .staticmethod("lst3_4_7")
        .staticmethod("solidKleinBottle")
        .staticmethod("figureEightKnotComplement")
        .staticmethod("whiteheadLinkComplement")
        .staticmethod("gieseking")
        .staticmethod("cuspedGenusTwoTorus")
    ;
}

