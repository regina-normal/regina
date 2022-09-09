
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "triangulation/dim2.h"
#include "triangulation/dim4.h"
#include "triangulation/facetpairing.h"
#include "triangulation/generic.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::BoolSet;
using regina::FacetPairing;
using regina::FacetSpec;
using regina::Triangulation;

template <int dim>
void addFacetPairing(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<regina::FacetPairing<dim>>(m, name)
        .def(pybind11::init<const FacetPairing<dim>&>())
        .def(pybind11::init<const Triangulation<dim>&>())
        .def("swap", &FacetPairing<dim>::swap)
        .def("size", &FacetPairing<dim>::size)
        // We do not ask dest() or operator[] to return by reference,
        // since FacetSpec is lightweight and we want to enforce constness.
        // Use the default policy for (const T&) which is to return by copy.
        .def("dest", overload_cast<const FacetSpec<dim>&>(
            &FacetPairing<dim>::dest, pybind11::const_))
        .def("dest", overload_cast<size_t, int>(
            &FacetPairing<dim>::dest, pybind11::const_))
        .def("__getitem__", overload_cast<const FacetSpec<dim>&>(
            &FacetPairing<dim>::operator[], pybind11::const_))
        .def("isUnmatched", overload_cast<const FacetSpec<dim>&>(
            &FacetPairing<dim>::isUnmatched, pybind11::const_))
        .def("isUnmatched", overload_cast<size_t, int>(
            &FacetPairing<dim>::isUnmatched, pybind11::const_))
        .def("isClosed", &FacetPairing<dim>::isClosed)
        .def("isConnected", &FacetPairing<dim>::isConnected)
        .def("isCanonical", &FacetPairing<dim>::isCanonical)
        .def("canonical", &FacetPairing<dim>::canonical)
        .def("canonicalAll", &FacetPairing<dim>::canonicalAll)
        .def("findAutomorphisms", &FacetPairing<dim>::findAutomorphisms)
        .def("textRep", &FacetPairing<dim>::textRep)
        .def("toTextRep", &FacetPairing<dim>::textRep) // deprecated
        .def_static("fromTextRep", &FacetPairing<dim>::fromTextRep)
        .def("dot", &FacetPairing<dim>::dot,
            pybind11::arg("prefix") = nullptr,
            pybind11::arg("subgraph") = false,
            pybind11::arg("labels") = false)
        .def_static("dotHeader", &FacetPairing<dim>::dotHeader,
            pybind11::arg("graphName") = nullptr)
        .def("divideConnected", &FacetPairing<dim>::divideConnected)
        .def_static("findAllPairings",
            &FacetPairing<dim>::template findAllPairings<
                const std::function<void(const FacetPairing<dim>&,
                    typename FacetPairing<dim>::IsoList)>&>)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c);

    m.def("swap",
        (void(*)(FacetPairing<dim>&, FacetPairing<dim>&))(regina::swap));
}

