
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
#include "algebra/abeliangroup.h"
#include "manifold/manifold.h"
#include "subcomplex/standardtri.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using namespace boost::python;
using regina::StandardTriangulation;

namespace {
    void writeName_stdio(const StandardTriangulation& t) {
        t.writeName(std::cout);
    }
    void writeTeXName_stdio(const StandardTriangulation& t) {
        t.writeTeXName(std::cout);
    }
    StandardTriangulation* (*isStandardTri_comp)(regina::Component<3>*) =
        &StandardTriangulation::isStandardTriangulation;
    StandardTriangulation* (*isStandardTri_tri)(regina::Triangulation<3>*) =
        &StandardTriangulation::isStandardTriangulation;
}

void addStandardTriangulation() {
    class_<StandardTriangulation, boost::noncopyable,
            std::auto_ptr<StandardTriangulation> >
            ("StandardTriangulation", no_init)
        .def("name", &StandardTriangulation::name)
        .def("TeXName", &StandardTriangulation::TeXName)
        .def("manifold", &StandardTriangulation::manifold,
            return_value_policy<manage_new_object>())
        .def("homology", &StandardTriangulation::homology,
            return_value_policy<manage_new_object>())
        .def("homologyH1", &StandardTriangulation::homologyH1,
            return_value_policy<manage_new_object>())
        .def("writeName", writeName_stdio)
        .def("writeTeXName", writeTeXName_stdio)
        .def("isStandardTriangulation", isStandardTri_comp,
            return_value_policy<manage_new_object>())
        .def("isStandardTriangulation", isStandardTri_tri,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("isStandardTriangulation")
    ;

    scope().attr("NStandardTriangulation") = scope().attr("StandardTriangulation");
}

