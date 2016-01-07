
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
#include "generic/simplex.h"
#include "generic/triangulation.h"
#include "maths/nperm5.h" // Specialisation needed for 4-D case.
#include "../helpers.h"

using namespace boost::python;
using regina::Simplex;

template <int dim>
void addSimplex(const char* name) {
    class_<regina::Simplex<dim>, std::auto_ptr<regina::Simplex<dim>>,
            boost::noncopyable>(name, no_init)
        .def("getDescription", &Simplex<dim>::getDescription,
            return_value_policy<return_by_value>())
        .def("setDescription", &Simplex<dim>::setDescription)
        .def("index", &Simplex<dim>::index)
        .def("adjacentSimplex", &Simplex<dim>::adjacentSimplex,
            return_value_policy<reference_existing_object>())
        .def("adjacentGluing", &Simplex<dim>::adjacentGluing)
        .def("adjacentFacet", &Simplex<dim>::adjacentFacet)
        .def("hasBoundary", &Simplex<dim>::hasBoundary)
        .def("joinTo", &Simplex<dim>::joinTo)
        .def("join", &Simplex<dim>::join)
        .def("unjoin", &Simplex<dim>::unjoin,
            return_value_policy<reference_existing_object>())
        .def("isolate", &Simplex<dim>::isolate)
        .def("triangulation", &Simplex<dim>::triangulation,
            return_value_policy<reference_existing_object>())
        .def("getTriangulation", &Simplex<dim>::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("str", &Simplex<dim>::str)
        .def("toString", &Simplex<dim>::toString)
        .def("detail", &Simplex<dim>::detail)
        .def("toStringLong", &Simplex<dim>::toStringLong)
        .def("__str__", &Simplex<dim>::str)
        .def(regina::python::add_eq_operators())
    ;
}

void addSimplex() {
    addSimplex<4>("Simplex4");
    addSimplex<5>("Simplex5");
    addSimplex<6>("Simplex6");
    addSimplex<7>("Simplex7");
    addSimplex<8>("Simplex8");
    addSimplex<9>("Simplex9");
    addSimplex<10>("Simplex10");
    addSimplex<11>("Simplex11");
    addSimplex<12>("Simplex12");
    addSimplex<13>("Simplex13");
    addSimplex<14>("Simplex14");
    addSimplex<15>("Simplex15");
}

