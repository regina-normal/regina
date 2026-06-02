
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
#include "triangulation/triangulation.h"
#include "../../helpers.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/simplex.h"

using namespace pybind11::literals;

using pybind11::overload_cast;
using regina::Simplex;

template <int dim> requires (regina::supportedDim(dim))
void addSimplex(pybind11::module_& m, const char* name) {
    static_assert(! regina::standardDim(dim));

    // We use the global scope here because all of Simplex's members are
    // inherited, and so Simplex's own docstring namespace does not exist.
    RDOC_SCOPE_BEGIN(Simplex)

    auto c = pybind11::class_<regina::Simplex<dim>>(m, name,
            rdoc::__class)
        .def("description", &Simplex<dim>::description, rdoc::description)
        .def("setDescription", &Simplex<dim>::setDescription,
            rdoc::setDescription)
        .def("index", &Simplex<dim>::index, rdoc::index)
        .def("adjacentSimplex", &Simplex<dim>::adjacentSimplex,
            pybind11::return_value_policy::reference, rdoc::adjacentSimplex)
        .def("adjacentGluing", &Simplex<dim>::adjacentGluing,
            rdoc::adjacentGluing)
        .def("adjacentFacet", &Simplex<dim>::adjacentFacet,
            rdoc::adjacentFacet)
        .def("hasBoundary", &Simplex<dim>::hasBoundary, rdoc::hasBoundary)
        .def("join", &Simplex<dim>::join, rdoc::join)
        .def("unjoin", &Simplex<dim>::unjoin,
            pybind11::return_value_policy::reference, rdoc::unjoin)
        .def("isolate", &Simplex<dim>::isolate, rdoc::isolate)
        .def("lock", &Simplex<dim>::lock, rdoc::lock)
        .def("lockFacet", &Simplex<dim>::lockFacet, rdoc::lockFacet)
        .def("unlock", &Simplex<dim>::unlock, rdoc::unlock)
        .def("unlockFacet", &Simplex<dim>::unlockFacet, rdoc::unlockFacet)
        .def("unlockAll", &Simplex<dim>::unlockAll, rdoc::unlockAll)
        .def("isLocked", &Simplex<dim>::isLocked, rdoc::isLocked)
        .def("isFacetLocked", &Simplex<dim>::isFacetLocked,
            rdoc::isFacetLocked)
        .def("lockMask", &Simplex<dim>::lockMask, rdoc::lockMask)
        .def("triangulation", &Simplex<dim>::triangulation,
            rdoc::triangulation)
        .def("component", &Simplex<dim>::component,
            pybind11::return_value_policy::reference, rdoc::component)
        .def("face", &regina::python::face<Simplex<dim>>,
            "subdim"_a, "face"_a, rdoc::face)
        .def("vertex", &Simplex<dim>::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("edge", overload_cast<int>(&Simplex<dim>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rdoc::edge)
        .def("edge",
            overload_cast<int, int>(&Simplex<dim>::edge, pybind11::const_),
            pybind11::return_value_policy::reference, rdoc::edge_2)
        .def("triangle", &Simplex<dim>::triangle,
            pybind11::return_value_policy::reference, rdoc::triangle)
        .def("tetrahedron", &Simplex<dim>::tetrahedron,
            pybind11::return_value_policy::reference, rdoc::tetrahedron)
        .def("pentachoron", &Simplex<dim>::pentachoron,
            pybind11::return_value_policy::reference, rdoc::pentachoron)
        .def("faceMapping", &regina::python::faceMapping<Simplex<dim>>,
            "subdim"_a, "face"_a, rdoc::faceMapping)
        .def("vertexMapping", &Simplex<dim>::vertexMapping,
            rdoc::vertexMapping)
        .def("edgeMapping", &Simplex<dim>::edgeMapping, rdoc::edgeMapping)
        .def("triangleMapping", &Simplex<dim>::triangleMapping,
            rdoc::triangleMapping)
        .def("tetrahedronMapping", &Simplex<dim>::tetrahedronMapping,
            rdoc::tetrahedronMapping)
        .def("pentachoronMapping", &Simplex<dim>::pentachoronMapping,
            rdoc::pentachoronMapping)
        .def("orientation", &Simplex<dim>::orientation, rdoc::orientation)
        .def("facetInMaximalForest", &Simplex<dim>::facetInMaximalForest,
            rdoc::facetInMaximalForest)
        .def_readonly_static("dimension", &Simplex<dim>::dimension)
        .def_readonly_static("subdimension", &Simplex<dim>::subdimension)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END
}

