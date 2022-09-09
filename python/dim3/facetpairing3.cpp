
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
#include "triangulation/facepair.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::BoolSet;
using regina::FacetPairing;
using regina::FacetSpec;
using regina::Triangulation;

void addFacetPairing3(pybind11::module_& m) {
    auto c = pybind11::class_<FacetPairing<3>>(m, "FacetPairing3")
        .def(pybind11::init<const FacetPairing<3>&>())
        .def(pybind11::init<const Triangulation<3>&>())
        .def("swap", &FacetPairing<3>::swap)
        .def("size", &FacetPairing<3>::size)
        // We do not ask dest() or operator[] to return by reference,
        // since FacetSpec is lightweight and we want to enforce constness.
        // Use the default policy for (const T&) which is to return by copy.
        .def("dest", overload_cast<const FacetSpec<3>&>(
            &FacetPairing<3>::dest, pybind11::const_))
        .def("dest", overload_cast<size_t, int>(
            &FacetPairing<3>::dest, pybind11::const_))
        .def("__getitem__", overload_cast<const FacetSpec<3>&>(
            &FacetPairing<3>::operator[], pybind11::const_))
        .def("isUnmatched", overload_cast<const FacetSpec<3>&>(
            &FacetPairing<3>::isUnmatched, pybind11::const_))
        .def("isUnmatched", overload_cast<size_t, int>(
            &FacetPairing<3>::isUnmatched, pybind11::const_))
        .def("isConnected", &FacetPairing<3>::isConnected)
        .def("isCanonical", &FacetPairing<3>::isCanonical)
        .def("canonical", &FacetPairing<3>::canonical)
        .def("canonicalAll", &FacetPairing<3>::canonicalAll)
        .def("findAutomorphisms", &FacetPairing<3>::findAutomorphisms)
        .def("textRep", &FacetPairing<3>::textRep)
        .def("toTextRep", &FacetPairing<3>::textRep) // deprecated
        .def_static("fromTextRep", &FacetPairing<3>::fromTextRep)
        .def("dot", &FacetPairing<3>::dot,
            pybind11::arg("prefix") = nullptr,
            pybind11::arg("subgraph") = false,
            pybind11::arg("labels") = false)
        .def_static("dotHeader", &FacetPairing<3>::dotHeader,
            pybind11::arg("graphName") = nullptr)
        .def("divideConnected", &FacetPairing<3>::divideConnected)
        .def("isClosed", &FacetPairing<3>::isClosed)
        .def("hasTripleEdge", &FacetPairing<3>::hasTripleEdge)
        .def("followChain", overload_cast<ssize_t&, regina::FacePair&>(
            &FacetPairing<3>::followChain, pybind11::const_))
        .def("hasBrokenDoubleEndedChain", overload_cast<>(
            &FacetPairing<3>::hasBrokenDoubleEndedChain, pybind11::const_))
        .def("hasOneEndedChainWithDoubleHandle", overload_cast<>(
            &FacetPairing<3>::hasOneEndedChainWithDoubleHandle,
            pybind11::const_))
        .def("hasWedgedDoubleEndedChain", overload_cast<>(
            &FacetPairing<3>::hasWedgedDoubleEndedChain, pybind11::const_))
        .def("hasOneEndedChainWithStrayBigon", overload_cast<>(
            &FacetPairing<3>::hasOneEndedChainWithStrayBigon, pybind11::const_))
        .def("hasTripleOneEndedChain", overload_cast<>(
            &FacetPairing<3>::hasTripleOneEndedChain, pybind11::const_))
        .def("hasSingleStar", &FacetPairing<3>::hasSingleStar)
        .def("hasDoubleStar", &FacetPairing<3>::hasDoubleStar)
        .def("hasDoubleSquare", &FacetPairing<3>::hasDoubleSquare)
        .def_static("findAllPairings", &FacetPairing<3>::findAllPairings<
            const std::function<void(const FacetPairing<3>&,
                FacetPairing<3>::IsoList)>&>)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c);

    m.def("swap",
        (void(*)(FacetPairing<3>&, FacetPairing<3>&))(regina::swap));
}

