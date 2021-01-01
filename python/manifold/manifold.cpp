
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
#include "../pybind11/operators.h"
#include "algebra/abeliangroup.h"
#include "manifold/manifold.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::Manifold;

void addManifold(pybind11::module_& m) {
    auto c = pybind11::class_<Manifold>(m, "Manifold")
        .def("name", &Manifold::name)
        .def("TeXName", &Manifold::TeXName)
        .def("structure", &Manifold::structure)
        .def("construct", &Manifold::construct)
        .def("homology", &Manifold::homology)
        .def("homologyH1", &Manifold::homologyH1)
        .def("isHyperbolic", &Manifold::isHyperbolic)
        .def("writeName", [](const Manifold& m) {
            m.writeName(std::cout);
        })
        .def("writeTeXName", [](const Manifold& m) {
            m.writeTeXName(std::cout);
        })
        .def("writeStructure", [](const Manifold& m) {
            m.writeStructure(std::cout);
        })
        // We cannot bind the < operator in the normal way:
        // see https://github.com/pybind/pybind11/issues/1487 for details.
        .def("__lt__", [](const Manifold& lhs, const Manifold& rhs) {
            return lhs < rhs;
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("NManifold") = m.attr("Manifold");
}

