
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "algebra/nabeliangroup.h"
#include "manifold/nmanifold.h"
#include "subcomplex/nstandardtri.h"
#include "triangulation/ncomponent.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

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
            bases<regina::ShareableObject>,
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
        .staticmethod("isStandardTriangulation")
    ;
}

