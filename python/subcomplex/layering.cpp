
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "subcomplex/layering.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/layering.h"

using regina::Layering;
using regina::Perm;
using regina::Tetrahedron;

void addLayering(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Layering)

    auto c = pybind11::class_<Layering>(m, "Layering", rdoc_scope)
        .def(pybind11::init<Tetrahedron<3>*, Perm<4>,
            Tetrahedron<3>*, Perm<4>>(), rdoc::__init)
        .def(pybind11::init<const Layering&>(), rdoc::__copy)
        .def("size", &Layering::size, rdoc::size)
        .def("oldBoundaryTet", &Layering::oldBoundaryTet,
            pybind11::return_value_policy::reference, rdoc::oldBoundaryTet)
        .def("oldBoundaryRoles", &Layering::oldBoundaryRoles,
            rdoc::oldBoundaryRoles)
        .def("newBoundaryTet", &Layering::newBoundaryTet,
            pybind11::return_value_policy::reference, rdoc::newBoundaryTet)
        .def("newBoundaryRoles", &Layering::newBoundaryRoles,
            rdoc::newBoundaryRoles)
        .def("boundaryReln", &Layering::boundaryReln,
            pybind11::return_value_policy::reference_internal,
            rdoc::boundaryReln)
        .def("extendOne", &Layering::extendOne, rdoc::extendOne)
        .def("extend", &Layering::extend, rdoc::extend)
        .def("matchesTop", &Layering::matchesTop, rdoc::matchesTop)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_END
}

