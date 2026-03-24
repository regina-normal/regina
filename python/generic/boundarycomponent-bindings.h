
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
#include "triangulation/generic.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/generic/boundarycomponent.h"
#include "../docstrings/triangulation/detail/boundarycomponent.h"

using regina::BoundaryComponent;

template <int dim> requires (regina::supportedDim(dim))
void addBoundaryComponent(pybind11::module_& m, pybind11::module_& internal,
        const char* name) {
    // In higher dimensions:
    // - we do not store lower-dimensional faces;
    // - we do not recognise ideal or invalid vertices;
    // - we can still triangulate a real boundary component.

    // We use the global scope here because all of BoundaryComponent's members
    // are inherited, and so BoundaryComponent's own docstring namespace
    // does not exist.
    RDOC_SCOPE_BEGIN_MAIN
    RDOC_SCOPE_BASE(detail::BoundaryComponentBase)

    auto c = pybind11::class_<BoundaryComponent<dim>>(m, name,
            rdoc::BoundaryComponent)
        .def("index", &BoundaryComponent<dim>::index, rbase::index)
        .def("size", &BoundaryComponent<dim>::size, rbase::size)
        .def("countRidges", &BoundaryComponent<dim>::countRidges,
            rbase::countRidges)
        .def("countFaces",
            (regina::python::countFacesFunc<BoundaryComponent<dim>>)(
                &BoundaryComponent<dim>::countFaces),
            rbase::countFaces)
        .def("facets", &BoundaryComponent<dim>::facets, rbase::facets)
        .def("faces", (regina::python::facesFunc<BoundaryComponent<dim>>)(
                &BoundaryComponent<dim>::faces),
            pybind11::arg("subdim"), rbase::faces)
        .def("facet", &BoundaryComponent<dim>::facet,
            pybind11::return_value_policy::reference, rbase::facet)
        .def("face", (regina::python::faceFunc<BoundaryComponent<dim>>)(
                &BoundaryComponent<dim>::face),
            pybind11::return_value_policy::reference,
            pybind11::arg("subdim"), pybind11::arg("index"), rbase::face)
        .def("component", &BoundaryComponent<dim>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("triangulation", &BoundaryComponent<dim>::triangulation,
            rbase::triangulation)
        .def("build", [](const BoundaryComponent<dim>& b) {
            // Return a clone of the resulting triangulation.
            // This is because Python cannot enforce the constness of
            // the reference that would normally be returned.
            return new regina::Triangulation<dim-1>(b.build());
        }, rbase::build)
        .def("isReal", &BoundaryComponent<dim>::isReal, rbase::isReal)
        .def("isIdeal", &BoundaryComponent<dim>::isIdeal, rbase::isIdeal)
        .def("isInvalidVertex", &BoundaryComponent<dim>::isInvalidVertex,
            rbase::isInvalidVertex)
        .def("isOrientable", &BoundaryComponent<dim>::isOrientable,
            rbase::isOrientable)
        .def_readonly_static("dimension", &BoundaryComponent<dim>::dimension)
        .def_readonly_static("allFaces", &BoundaryComponent<dim>::allFaces)
        .def_readonly_static("allowVertex",
            &BoundaryComponent<dim>::allowVertex)
        .def_readonly_static("canBuild", &BoundaryComponent<dim>::canBuild)
    ;
    if constexpr (dim == 5) {
        c.def("countPentachora", &BoundaryComponent<dim>::countPentachora,
            rbase::countPentachora);
        c.def("pentachoron", &BoundaryComponent<dim>::pentachoron,
            pybind11::return_value_policy::reference, rbase::pentachoron);
        c.def("pentachora", &BoundaryComponent<dim>::pentachora,
            rbase::pentachora);
    }
    if constexpr (dim == 6) {
        c.def("countPentachora", &BoundaryComponent<dim>::countPentachora,
            rbase::countPentachora);
    }
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END

    regina::python::addListView<
        decltype(std::declval<BoundaryComponent<dim>>().facets())>(internal,
        (std::string(name) + "_facets").c_str());
}

