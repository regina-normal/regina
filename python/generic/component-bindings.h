
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
#include "../pybind11/stl.h"
#include "triangulation/generic.h"
#include "../helpers.h"

using regina::Component;

template <int dim>
void addComponent(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<Component<dim>>(m, name)
        .def("index", &Component<dim>::index)
        .def("size", &Component<dim>::size)
        .def("countBoundaryComponents",
            &Component<dim>::countBoundaryComponents)
        .def("simplices", &Component<dim>::simplices,
            pybind11::return_value_policy::reference)
        .def("simplex", &Component<dim>::simplex,
            pybind11::return_value_policy::reference)
        .def("boundaryComponents", &Component<dim>::boundaryComponents,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Component<dim>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("isValid", &Component<dim>::isValid)
        .def("isOrientable", &Component<dim>::isOrientable)
        .def("hasBoundaryFacets", &Component<dim>::hasBoundaryFacets)
        .def("countBoundaryFacets", &Component<dim>::countBoundaryFacets)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);
}

