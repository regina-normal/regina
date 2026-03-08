
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
#include <pybind11/operators.h>
#include "triangulation/generic.h"
#include "triangulation/facetpairing.h"
#include "../helpers.h"
#include "../docstrings/triangulation/generic/isomorphism.h"

using pybind11::overload_cast;
using regina::Isomorphism;

template <int dim> requires (regina::supportedDim(dim))
void addIsomorphism(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(Isomorphism)

    auto c = pybind11::class_<Isomorphism<dim>>(m, name, rdoc_scope)
        .def(pybind11::init<const Isomorphism<dim>&>(), rdoc::__copy)
        .def(pybind11::init<size_t>(), rdoc::__init)
        .def("swap", &Isomorphism<dim>::swap, rdoc::swap)
        .def("size", &Isomorphism<dim>::size, rdoc::size)
        .def("__len__", &Isomorphism<dim>::size, rdoc::size)
        .def("simpImage", overload_cast<size_t>(
            &Isomorphism<dim>::simpImage, pybind11::const_), rdoc::simpImage)
        .def("setSimpImage", [](Isomorphism<dim>& iso, size_t s, ssize_t image) {
            iso.simpImage(s) = image;
        }, rdoc::setSimpImage)
        .def("facetPerm", overload_cast<size_t>(
            &Isomorphism<dim>::facetPerm, pybind11::const_), rdoc::facetPerm)
        .def("setFacetPerm", [](Isomorphism<dim>& iso, size_t s,
                regina::Perm<dim+1> p) {
            iso.facetPerm(s) = p;
        }, rdoc::setFacetPerm)
        .def("__getitem__", &Isomorphism<dim>::operator[], rdoc::__array)
        .def("isIdentity", &Isomorphism<dim>::isIdentity, rdoc::isIdentity)
        .def("__call__", overload_cast<const regina::Triangulation<dim>&>(
            &Isomorphism<dim>::operator(), pybind11::const_), rdoc::__call)
        .def("__call__", overload_cast<const regina::FacetSpec<dim>&>(
            &Isomorphism<dim>::operator(), pybind11::const_), rdoc::__call_2)
        .def("__call__", overload_cast<const regina::FacetPairing<dim>&>(
            &Isomorphism<dim>::operator(), pybind11::const_), rdoc::__call_3)
        .def("apply", overload_cast<const regina::Triangulation<dim>&>(
            &Isomorphism<dim>::operator(), pybind11::const_), // deprecated
            rdoc::apply)
        .def("applyInPlace", // deprecated
                [](const Isomorphism<dim>& iso, regina::Triangulation<dim>& t) {
            t = iso(t);
        }, rdoc::applyInPlace)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def("inverse", &Isomorphism<dim>::inverse, rdoc::inverse)
        .def("inc", [](Isomorphism<dim>& iso) { ++iso; }, rdoc::__inc)
        .def_static("random", &Isomorphism<dim>::random,
            pybind11::arg(), pybind11::arg("even") = false,
            rdoc::random)
        .def_static("identity", &Isomorphism<dim>::identity, rdoc::identity)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    regina::python::add_global_swap<Isomorphism<dim>>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

