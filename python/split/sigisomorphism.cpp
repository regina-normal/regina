
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
#include "split/sigisomorphism.h"
#include "../helpers.h"
#include "../docstrings/split/sigisomorphism.h"

using regina::SigPartialIsomorphism;

void addSigIsomorphism(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SigPartialIsomorphism)

    auto c = pybind11::class_<SigPartialIsomorphism>(m, "SigPartialIsomorphism",
            rdoc_scope)
        .def(pybind11::init<int>(), rdoc::__init)
        .def(pybind11::init<const SigPartialIsomorphism&>(), rdoc::__copy)
        .def("swap", &SigPartialIsomorphism::swap, rdoc::swap)
        .def("makeCanonical", &SigPartialIsomorphism::makeCanonical,
            pybind11::arg(), pybind11::arg("fromCycleGroup") = 0,
            rdoc::makeCanonical)
        .def("compareWith", &SigPartialIsomorphism::compareWith,
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("fromCycleGroup") = 0,
            rdoc::compareWith)
        .def("compareWithIdentity", &SigPartialIsomorphism::compareWithIdentity,
            pybind11::arg(), pybind11::arg("fromCycleGroup") = 0,
            rdoc::compareWithIdentity)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<SigPartialIsomorphism>(m,
        rdoc::global_swap);

    RDOC_SCOPE_END
}

