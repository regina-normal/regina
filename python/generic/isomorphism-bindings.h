
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
#include "../pybind11/operators.h"
#include "triangulation/generic.h"
#include "triangulation/facetpairing.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Isomorphism;

template <int dim>
void addIsomorphism(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<Isomorphism<dim>>(m, name)
        .def(pybind11::init<const Isomorphism<dim>&>())
        .def(pybind11::init<size_t>())
        .def("swap", &Isomorphism<dim>::swap)
        .def("size", &Isomorphism<dim>::size)
        .def("simpImage", overload_cast<size_t>(
            &Isomorphism<dim>::simpImage, pybind11::const_))
        .def("setSimpImage", [](Isomorphism<dim>& iso, size_t s, ssize_t image) {
            iso.simpImage(s) = image;
        })
        .def("facetPerm", overload_cast<size_t>(
            &Isomorphism<dim>::facetPerm, pybind11::const_))
        .def("setFacetPerm", [](Isomorphism<dim>& iso, size_t s,
                regina::Perm<dim+1> p) {
            iso.facetPerm(s) = p;
        })
        .def("__getitem__", &Isomorphism<dim>::operator[])
        .def("isIdentity", &Isomorphism<dim>::isIdentity)
        .def("__call__", overload_cast<const regina::Triangulation<dim>&>(
            &Isomorphism<dim>::operator(), pybind11::const_))
        .def("__call__", overload_cast<const regina::FacetSpec<dim>&>(
            &Isomorphism<dim>::operator(), pybind11::const_))
        .def("__call__", overload_cast<const regina::FacetPairing<dim>&>(
            &Isomorphism<dim>::operator(), pybind11::const_))
        .def("apply", overload_cast<const regina::Triangulation<dim>&>(
            &Isomorphism<dim>::operator(), pybind11::const_)) // deprecated
        .def("applyInPlace", // deprecated
                [](const Isomorphism<dim>& iso, regina::Triangulation<dim>& t) {
            t = iso(t);
        })
        .def(pybind11::self * pybind11::self)
        .def("inverse", &Isomorphism<dim>::inverse)
        .def_static("random", &Isomorphism<dim>::random,
            pybind11::arg(), pybind11::arg("even") = false)
        .def_static("identity", &Isomorphism<dim>::identity)
    ;
    if constexpr (dim <= 6) {
        c.def("inc", [](Isomorphism<dim>& iso) {
            ++iso;
        });
    }
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c);

    m.def("swap",
        (void(*)(Isomorphism<dim>&, Isomorphism<dim>&))(regina::swap));
}

