
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "triangulation/generic.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/generic/simplex.h"
#include "../docstrings/triangulation/detail/simplex.h"

using pybind11::overload_cast;
using regina::Simplex;

template <int dim>
void addSimplex(pybind11::module_& m, const char* name) {
    static_assert(! regina::standardDim(dim));

    // We use the global scope here because all of Simplex's members are
    // inherited, and so Simplex's own docstring namespace does not exist.
    RDOC_SCOPE_BEGIN_MAIN
    RDOC_SCOPE_BASE(detail::SimplexBase)

    auto c = pybind11::class_<regina::Simplex<dim>>(m, name, rdoc::Face)
        .def("description", &Simplex<dim>::description, rbase::description)
        .def("setDescription", &Simplex<dim>::setDescription,
            rbase::setDescription)
        .def("index", &Simplex<dim>::index, rbase::index)
        .def("adjacentSimplex", &Simplex<dim>::adjacentSimplex,
            pybind11::return_value_policy::reference, rbase::adjacentSimplex)
        .def("adjacentGluing", &Simplex<dim>::adjacentGluing,
            rbase::adjacentGluing)
        .def("adjacentFacet", &Simplex<dim>::adjacentFacet,
            rbase::adjacentFacet)
        .def("hasBoundary", &Simplex<dim>::hasBoundary, rbase::hasBoundary)
        .def("join", &Simplex<dim>::join, rbase::join)
        .def("unjoin", &Simplex<dim>::unjoin,
            pybind11::return_value_policy::reference, rbase::unjoin)
        .def("isolate", &Simplex<dim>::isolate, rbase::isolate)
        .def("triangulation", &Simplex<dim>::triangulation,
            rbase::triangulation)
        .def("component", &Simplex<dim>::component,
            pybind11::return_value_policy::reference, rbase::component)
        .def("face", &regina::python::face<Simplex<dim>, dim, int>,
            pybind11::arg("subdim"), pybind11::arg("face"), rbase::face)
        .def("vertex", &Simplex<dim>::vertex,
            pybind11::return_value_policy::reference, rbase::vertex)
        .def("edge", overload_cast<int>(&Simplex<dim>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rbase::edge)
        .def("edge",
            overload_cast<int, int>(&Simplex<dim>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rbase::edge_2)
        .def("triangle", &Simplex<dim>::triangle,
            pybind11::return_value_policy::reference, rbase::triangle)
        .def("tetrahedron", &Simplex<dim>::tetrahedron,
            pybind11::return_value_policy::reference, rbase::tetrahedron)
        .def("pentachoron", &Simplex<dim>::pentachoron,
            pybind11::return_value_policy::reference, rbase::pentachoron)
        .def("faceMapping", &regina::python::faceMapping<Simplex<dim>, dim>,
            pybind11::arg("subdim"), pybind11::arg("face"), rbase::faceMapping)
        .def("vertexMapping", &Simplex<dim>::vertexMapping,
            rbase::vertexMapping)
        .def("edgeMapping", &Simplex<dim>::edgeMapping, rbase::edgeMapping)
        .def("triangleMapping", &Simplex<dim>::triangleMapping,
            rbase::triangleMapping)
        .def("tetrahedronMapping", &Simplex<dim>::tetrahedronMapping,
            rbase::tetrahedronMapping)
        .def("pentachoronMapping", &Simplex<dim>::pentachoronMapping,
            rbase::pentachoronMapping)
        .def("orientation", &Simplex<dim>::orientation, rbase::orientation)
        .def("facetInMaximalForest", &Simplex<dim>::facetInMaximalForest,
            rbase::facetInMaximalForest)
        .def_readonly_static("dimension", &Simplex<dim>::dimension)
        .def_readonly_static("subdimension", &Simplex<dim>::subdimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END
}

