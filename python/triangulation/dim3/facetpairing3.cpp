
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
#include "triangulation/facepair.h"
#include "triangulation/facetpairing.h"
#include "triangulation/dim3.h"
#include "../../helpers.h"
#include "../../docstrings/triangulation/facetpairing.h"

using namespace pybind11::literals;

using pybind11::overload_cast;
using regina::BoolSet;
using regina::FacetPairing;
using regina::FacetSpec;
using regina::Triangulation;

void addFacetPairing3(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FacetPairing)

    auto c = pybind11::class_<FacetPairing<3>>(m, "FacetPairing3",
            rdoc::__class)
        .def(pybind11::init<const FacetPairing<3>&>(), rdoc::__copy)
        .def(pybind11::init<const Triangulation<3>&>(), rdoc::__init)
        .def("swap", &FacetPairing<3>::swap, rdoc::swap)
        .def("size", &FacetPairing<3>::size, rdoc::size)
        .def("__len__", &FacetPairing<3>::size, rdoc::size)
        // We do not ask dest() or operator[] to return by reference,
        // since FacetSpec is lightweight and we want to enforce constness.
        // Use the default policy for (const T&) which is to return by copy.
        .def("dest", overload_cast<const FacetSpec<3>&>(
            &FacetPairing<3>::dest, pybind11::const_), rdoc::dest)
        .def("dest", overload_cast<size_t, int>(
            &FacetPairing<3>::dest, pybind11::const_), rdoc::dest_2)
        .def("__getitem__", overload_cast<const FacetSpec<3>&>(
            &FacetPairing<3>::operator[], pybind11::const_), rdoc::__array)
        .def("isUnmatched", overload_cast<const FacetSpec<3>&>(
            &FacetPairing<3>::isUnmatched, pybind11::const_),
            rdoc::isUnmatched)
        .def("isUnmatched", overload_cast<size_t, int>(
            &FacetPairing<3>::isUnmatched, pybind11::const_),
            rdoc::isUnmatched_2)
        .def("isConnected", &FacetPairing<3>::isConnected, rdoc::isConnected)
        .def("hasMultiEdge", (bool (FacetPairing<3>::*)(int) const)(
                &FacetPairing<3>::hasMultiEdge),
            rdoc::hasMultiEdge)
        .def("isCanonical", &FacetPairing<3>::isCanonical, rdoc::isCanonical)
        .def("canonical", &FacetPairing<3>::canonical, rdoc::canonical)
        .def("canonicalAll", &FacetPairing<3>::canonicalAll,
            rdoc::canonicalAll)
        .def("findAutomorphisms", &FacetPairing<3>::findAutomorphisms,
            rdoc::findAutomorphisms)
        .def("textRep", &FacetPairing<3>::textRep, rdoc::textRep)
        .def("toTextRep", &FacetPairing<3>::textRep, // deprecated
            rdoc::toTextRep)
        .def_static("fromTextRep", &FacetPairing<3>::fromTextRep,
            rdoc::fromTextRep)
        .def("dot", &FacetPairing<3>::dot,
            "prefix"_a = nullptr, "subgraph"_a = false, "labels"_a = false,
            rdoc::dot)
        .def_static("dotHeader", &FacetPairing<3>::dotHeader,
            "graphName"_a = nullptr, rdoc::dotHeader)
        .def("divideConnected", &FacetPairing<3>::divideConnected,
            rdoc::divideConnected)
        .def("isClosed", &FacetPairing<3>::isClosed, rdoc::isClosed)
        .def("hasDoubleEdge", &FacetPairing<3>::hasDoubleEdge,
            rdoc::hasDoubleEdge)
        .def("hasTripleEdge", &FacetPairing<3>::hasTripleEdge,
            rdoc::hasTripleEdge)
        .def("followChain", overload_cast<ssize_t&, regina::FacePair&>(
            &FacetPairing<3>::followChain, pybind11::const_),
            rdoc::followChain)
        .def("hasBrokenDoubleEndedChain", overload_cast<>(
            &FacetPairing<3>::hasBrokenDoubleEndedChain, pybind11::const_),
            rdoc::hasBrokenDoubleEndedChain)
        .def("hasOneEndedChainWithDoubleHandle", overload_cast<>(
            &FacetPairing<3>::hasOneEndedChainWithDoubleHandle,
            pybind11::const_), rdoc::hasOneEndedChainWithDoubleHandle)
        .def("hasWedgedDoubleEndedChain", overload_cast<>(
            &FacetPairing<3>::hasWedgedDoubleEndedChain, pybind11::const_),
            rdoc::hasWedgedDoubleEndedChain)
        .def("hasOneEndedChainWithStrayBigon", overload_cast<>(
            &FacetPairing<3>::hasOneEndedChainWithStrayBigon, pybind11::const_),
            rdoc::hasOneEndedChainWithStrayBigon)
        .def("hasTripleOneEndedChain", overload_cast<>(
            &FacetPairing<3>::hasTripleOneEndedChain, pybind11::const_),
            rdoc::hasTripleOneEndedChain)
        .def("hasSingleStar", &FacetPairing<3>::hasSingleStar,
            rdoc::hasSingleStar)
        .def("hasDoubleStar", &FacetPairing<3>::hasDoubleStar,
            rdoc::hasDoubleStar)
        .def("hasDoubleSquare", &FacetPairing<3>::hasDoubleSquare,
            rdoc::hasDoubleSquare)
        .def_static("findAllPairings", &FacetPairing<3>::findAllPairings<
            const std::function<void(const FacetPairing<3>&,
                FacetPairing<3>::IsoList)>&>,
            "nSimplices"_a, "boundary"_a, "nBdryFacets"_a, "action"_a,
            rdoc::findAllPairings)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    RDOC_SCOPE_SWITCH(FacetPairing)

    regina::python::add_global_swap<FacetPairing<3>, rdoc>(m);

    RDOC_SCOPE_END
}

