
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

using regina::BoundaryComponent;

template <int dim>
void addBoundaryComponent(pybind11::module_& m, const char* name) {
    // In higher dimensions:
    // - we do not store lower-dimensional faces;
    // - we do not recognise ideal or invalid vertices;
    // - we can still triangulate a real boundary component.

    auto c = pybind11::class_<BoundaryComponent<dim>>(m, name)
        .def("index", &BoundaryComponent<dim>::index)
        .def("size", &BoundaryComponent<dim>::size)
        .def("facets", [](const BoundaryComponent<dim>& b) {
            pybind11::list ans;
            for (auto f : b.facets())
                ans.append(f); // Uses reference return value policy
            return ans;
        })
        .def("facet", &BoundaryComponent<dim>::facet,
            pybind11::return_value_policy::reference)
        .def("component", &BoundaryComponent<dim>::component,
            pybind11::return_value_policy::reference)
        .def("triangulation", &BoundaryComponent<dim>::triangulation)
        .def("build", [](const BoundaryComponent<dim>* b) {
            // Return a clone of the resulting triangulation.  This is because
            // triangulations have a custom holder type, and so pybind11 ignores
            // any attempt to pass return_value_policy::reference_internal.
            return new regina::Triangulation<dim-1>(*(b->build()));
        })
        .def("isOrientable", &BoundaryComponent<dim>::isOrientable)
        // We cannot take the addresses of the following header-only properties,
        // so we define getter functions instead.
        .def_property_readonly_static("allFaces", [](pybind11::object) {
            return BoundaryComponent<dim>::allFaces;
        })
        .def_property_readonly_static("allowVertex", [](pybind11::object) {
            return BoundaryComponent<dim>::allowVertex;
        })
        .def_property_readonly_static("canBuild", [](pybind11::object) {
            return BoundaryComponent<dim>::canBuild;
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);
}

