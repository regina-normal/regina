
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
#include "algebra/nabeliangroup.h"
#include "manifold/nmanifold.h"
#include "subcomplex/nstandardtri.h"
#include "triangulation/ncomponent.h"
#include "triangulation/ntriangulation.h"

using namespace boost::python;
using regina::NStandardTriangulation;

namespace {
    void writeName_stdio(const NStandardTriangulation& t) {
        t.writeName(std::cout);
    }
    void writeTeXName_stdio(const NStandardTriangulation& t) {
        t.writeTeXName(std::cout);
    }
    NStandardTriangulation* (*isStandardTri_comp)(regina::NComponent*) =
        &NStandardTriangulation::isStandardTriangulation;
    NStandardTriangulation* (*isStandardTri_tri)(regina::NTriangulation*) =
        &NStandardTriangulation::isStandardTriangulation;
}

void addNStandardTriangulation() {
    class_<NStandardTriangulation, boost::noncopyable,
            std::auto_ptr<NStandardTriangulation> >
            ("NStandardTriangulation", no_init)
        .def("getName", &NStandardTriangulation::getName)
        .def("getTeXName", &NStandardTriangulation::getTeXName)
        .def("getManifold", &NStandardTriangulation::getManifold,
            return_value_policy<manage_new_object>())
        .def("getHomologyH1", &NStandardTriangulation::getHomologyH1,
            return_value_policy<manage_new_object>())
        .def("writeName", writeName_stdio)
        .def("writeTeXName", writeTeXName_stdio)
        .def("isStandardTriangulation", isStandardTri_comp,
            return_value_policy<manage_new_object>())
        .def("isStandardTriangulation", isStandardTri_tri,
            return_value_policy<manage_new_object>())
        .def("str", &NStandardTriangulation::str)
        .def("toString", &NStandardTriangulation::toString)
        .def("detail", &NStandardTriangulation::detail)
        .def("toStringLong", &NStandardTriangulation::toStringLong)
        .def("__str__", &NStandardTriangulation::str)
        .staticmethod("isStandardTriangulation")
    ;
}

