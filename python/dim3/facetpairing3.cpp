
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

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "triangulation/facepair.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../python/docstrings/triangulation/detail/facetpairing.h"
#include "../python/docstrings/triangulation/facetpairing3.h"

using pybind11::overload_cast;
using regina::BoolSet;
using regina::FacetPairing;
using regina::FacetSpec;
using regina::Triangulation;

void addFacetPairing3(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FacetPairing)
    RDOC_SCOPE_BASE(detail::FacetPairingBase)

    auto c = pybind11::class_<FacetPairing<3>>(m, "FacetPairing3", rdoc_scope)
        .def(pybind11::init<const FacetPairing<3>&>(), rdoc::__copy)
        .def(pybind11::init<const Triangulation<3>&>(), rdoc::__init)
        .def("swap", &FacetPairing<3>::swap, rbase::swap)
        .def("size", &FacetPairing<3>::size, rbase::size)
        .def("__len__", &FacetPairing<3>::size, rbase::size)
        // We do not ask dest() or operator[] to return by reference,
        // since FacetSpec is lightweight and we want to enforce constness.
        // Use the default policy for (const T&) which is to return by copy.
        .def("dest", overload_cast<const FacetSpec<3>&>(
            &FacetPairing<3>::dest, pybind11::const_), rbase::dest)
        .def("dest", overload_cast<size_t, int>(
            &FacetPairing<3>::dest, pybind11::const_), rbase::dest_2)
        .def("__getitem__", overload_cast<const FacetSpec<3>&>(
            &FacetPairing<3>::operator[], pybind11::const_), rbase::__array)
        .def("isUnmatched", overload_cast<const FacetSpec<3>&>(
            &FacetPairing<3>::isUnmatched, pybind11::const_),
            rbase::isUnmatched)
        .def("isUnmatched", overload_cast<size_t, int>(
            &FacetPairing<3>::isUnmatched, pybind11::const_),
            rbase::isUnmatched_2)
        .def("isConnected", &FacetPairing<3>::isConnected, rbase::isConnected)
        .def("hasMultiEdge", (bool (FacetPairing<3>::*)(int) const)(
                &FacetPairing<3>::hasMultiEdge),
            rbase::hasMultiEdge)
        .def("isCanonical", &FacetPairing<3>::isCanonical, rbase::isCanonical)
        .def("canonical", &FacetPairing<3>::canonical, rbase::canonical)
        .def("canonicalAll", &FacetPairing<3>::canonicalAll,
            rbase::canonicalAll)
        .def("findAutomorphisms", &FacetPairing<3>::findAutomorphisms,
            rbase::findAutomorphisms)
        .def("textRep", &FacetPairing<3>::textRep, rbase::textRep)
        .def("toTextRep", &FacetPairing<3>::textRep, // deprecated
            rbase::toTextRep)
        .def_static("fromTextRep", &FacetPairing<3>::fromTextRep,
            rbase::fromTextRep)
        .def("dot", &FacetPairing<3>::dot,
            pybind11::arg("prefix") = nullptr,
            pybind11::arg("subgraph") = false,
            pybind11::arg("labels") = false,
            rbase::dot)
        .def_static("dotHeader", &FacetPairing<3>::dotHeader,
            pybind11::arg("graphName") = nullptr, rbase::dotHeader)
        .def("divideConnected", &FacetPairing<3>::divideConnected,
            rbase::divideConnected)
        .def("isClosed", &FacetPairing<3>::isClosed, rbase::isClosed)
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
            pybind11::arg("nSimplices"), pybind11::arg("boundary"),
            pybind11::arg("nBdryFacets"), pybind11::arg("action"),
            rbase::findAllPairings)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rbase::__eq);

    regina::python::add_global_swap<FacetPairing<3>>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

