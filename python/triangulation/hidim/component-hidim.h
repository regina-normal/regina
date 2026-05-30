
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "triangulation/triangulation.h"
#include "../../helpers.h"
#include "../../docstrings/triangulation/detail/component.h"

using regina::Component;

template <int dim> requires (regina::supportedDim(dim))
void addComponent(pybind11::module_& m, pybind11::module_& internal,
        const char* name) {
    // We use the global scope here because all of Component's members are
    // inherited, and so Component's own docstring namespace does not exist.
    RDOC_SCOPE_BEGIN_MAIN

    auto c = pybind11::class_<Component<dim>>(m, name, rdoc::Component::__class)
        .def("index", &Component<dim>::index, rdoc::index)
        .def("triangulation", &Component<dim>::triangulation,
            rdoc::triangulation)
        .def("size", &Component<dim>::size, rdoc::size)
        .def("countFacets", &Component<dim>::countFacets, rdoc::countFacets)
        .def("countBoundaryComponents",
            &Component<dim>::countBoundaryComponents,
            rdoc::countBoundaryComponents)
        .def("simplices", &Component<dim>::simplices, rdoc::simplices)
        .def("simplex", &Component<dim>::simplex,
            pybind11::return_value_policy::reference, rdoc::simplex)
        .def("hasLocks", &Component<dim>::hasLocks, rdoc::hasLocks)
        .def("boundaryComponents", &Component<dim>::boundaryComponents,
            rdoc::boundaryComponents)
        .def("boundaryComponent", &Component<dim>::boundaryComponent,
            pybind11::return_value_policy::reference, rdoc::boundaryComponent)
        .def("isValid", &Component<dim>::isValid, rdoc::isValid)
        .def("isOrientable", &Component<dim>::isOrientable, rdoc::isOrientable)
        .def("hasBoundaryFacets", &Component<dim>::hasBoundaryFacets,
            rdoc::hasBoundaryFacets)
        .def("countBoundaryFacets", &Component<dim>::countBoundaryFacets,
            rdoc::countBoundaryFacets)
        .def_readonly_static("dimension", &Component<dim>::dimension)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    regina::python::addStdView<
        decltype(std::declval<Component<dim>>().simplices())>(internal,
        (std::string(name) + "_simplices").c_str());
    regina::python::addStdView<
        decltype(std::declval<Component<dim>>().boundaryComponents())>(internal,
        (std::string(name) + "_boundaryComponents").c_str());

    RDOC_SCOPE_END
}

