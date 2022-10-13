
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
#include "../pybind11/stl.h"
#include "enumerate/validityconstraints.h"
#include "utilities/bitmask.h"
#include "../helpers.h"
#include "../docstrings/enumerate/validityconstraints.h"

using regina::ValidityConstraints;

void addValidityConstraints(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(ValidityConstraints)

    auto c = pybind11::class_<ValidityConstraints>(m, "ValidityConstraints")
        .def(pybind11::init<int, size_t, size_t, size_t>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("reserveLocal") = 0,
            pybind11::arg("reserveGlobal") = 0)
        .def(pybind11::init<const ValidityConstraints&>())
        .def("addLocal", [](ValidityConstraints& v,
                const std::vector<int>& pos) {
            v.addLocal(pos.begin(), pos.end());
        })
        .def("addGlobal", [](ValidityConstraints& v,
                const std::vector<int>& pos) {
            v.addGlobal(pos.begin(), pos.end());
        })
        .def("swap", &ValidityConstraints::swap)
        .def("bitmasks", pybind11::overload_cast<size_t>(
            &ValidityConstraints::bitmasks<regina::Bitmask>, pybind11::const_))
        .def("bitmasks", pybind11::overload_cast<>(
            &ValidityConstraints::bitmasks<regina::Bitmask>, pybind11::const_))
        .def_readonly_static("none", &ValidityConstraints::none)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    regina::python::add_global_swap<ValidityConstraints>(m);

    RDOC_SCOPE_END
}

