
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
#include "triangulation/generic.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Isomorphism;

template <int dim>
void addIsomorphism(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<Isomorphism<dim>>(m, name)
        .def(pybind11::init<const Isomorphism<dim>&>())
        .def("size", &Isomorphism<dim>::size)
        .def("simpImage", overload_cast<unsigned>(
            &Isomorphism<dim>::simpImage, pybind11::const_))
        .def("facetPerm", overload_cast<unsigned>(
            &Isomorphism<dim>::facetPerm, pybind11::const_))
        .def("__getitem__", &Isomorphism<dim>::operator[])
        .def("isIdentity", &Isomorphism<dim>::isIdentity)
        .def("apply", &Isomorphism<dim>::apply)
        .def("applyInPlace", &Isomorphism<dim>::applyInPlace)
        .def_static("random", &Isomorphism<dim>::random,
            pybind11::arg(), pybind11::arg("even") = false)
        .def_static("identity", &Isomorphism<dim>::identity)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);
}

