
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
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "triangulation/dim2.h"
#include "triangulation/dim4.h"
#include "triangulation/facetpairing.h"
#include "triangulation/hidim.h"
#include "../helpers.h"
#include "../docstrings/triangulation/detail/facetpairing.h"

using namespace pybind11::literals;

using pybind11::overload_cast;
using regina::BoolSet;
using regina::FacetPairing;
using regina::FacetSpec;
using regina::Triangulation;

template <int dim> requires (regina::supportedDim(dim))
void addFacetPairing(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(FacetPairing)
    RDOC_SCOPE_BASE(detail::FacetPairingBase)

    auto c = pybind11::class_<regina::FacetPairing<dim>>(m, name, rdoc::__class)
        .def(pybind11::init<const FacetPairing<dim>&>(), rdoc::__copy)
        .def(pybind11::init<const Triangulation<dim>&>(), rdoc::__init)
        .def("swap", &FacetPairing<dim>::swap, rbase::swap)
        .def("size", &FacetPairing<dim>::size, rbase::size)
        .def("__len__", &FacetPairing<dim>::size, rbase::size)
        // We do not ask dest() or operator[] to return by reference,
        // since FacetSpec is lightweight and we want to enforce constness.
        // Use the default policy for (const T&) which is to return by copy.
        .def("dest", overload_cast<const FacetSpec<dim>&>(
            &FacetPairing<dim>::dest, pybind11::const_), rbase::dest)
        .def("dest", overload_cast<size_t, int>(
            &FacetPairing<dim>::dest, pybind11::const_), rbase::dest_2)
        .def("__getitem__", overload_cast<const FacetSpec<dim>&>(
            &FacetPairing<dim>::operator[], pybind11::const_),
            rbase::__array)
        .def("isUnmatched", overload_cast<const FacetSpec<dim>&>(
            &FacetPairing<dim>::isUnmatched, pybind11::const_),
            rbase::isUnmatched)
        .def("isUnmatched", overload_cast<size_t, int>(
            &FacetPairing<dim>::isUnmatched, pybind11::const_),
            rbase::isUnmatched_2)
        .def("isClosed", &FacetPairing<dim>::isClosed, rbase::isClosed)
        .def("isConnected", &FacetPairing<dim>::isConnected, rbase::isConnected)
        .def("hasMultiEdge", (bool (FacetPairing<dim>::*)(int) const)(
                &FacetPairing<dim>::hasMultiEdge),
            rbase::hasMultiEdge)
        .def("isCanonical", &FacetPairing<dim>::isCanonical, rbase::isCanonical)
        .def("canonical", &FacetPairing<dim>::canonical, rbase::canonical)
        .def("canonicalAll", &FacetPairing<dim>::canonicalAll,
            rbase::canonicalAll)
        .def("findAutomorphisms", &FacetPairing<dim>::findAutomorphisms,
            rbase::findAutomorphisms)
        .def("textRep", &FacetPairing<dim>::textRep, rbase::textRep)
        .def("toTextRep", &FacetPairing<dim>::textRep, // deprecated
            rbase::toTextRep)
        .def_static("fromTextRep", &FacetPairing<dim>::fromTextRep,
            rbase::fromTextRep)
        .def("dot", &FacetPairing<dim>::dot,
            "prefix"_a = nullptr, "subgraph"_a = false, "labels"_a = false,
            rbase::dot)
        .def_static("dotHeader", &FacetPairing<dim>::dotHeader,
            "graphName"_a = nullptr, rbase::dotHeader)
        .def("divideConnected", &FacetPairing<dim>::divideConnected,
            rbase::divideConnected)
        .def_static("findAllPairings",
            &FacetPairing<dim>::template findAllPairings<
                const std::function<void(const FacetPairing<dim>&,
                    typename FacetPairing<dim>::IsoList)>&>,
            "nSimplices"_a, "boundary"_a, "nBdryFacets"_a, "action"_a,
            rbase::findAllPairings)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rbase::__eq);
    regina::python::add_global_swap<FacetPairing<dim>, rdoc>(m);

    RDOC_SCOPE_END
}

