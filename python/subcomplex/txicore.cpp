
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
#include "subcomplex/txicore.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/txicore.h"

using regina::TxICore;
using regina::TxIDiagonalCore;
using regina::TxIParallelCore;

void addTxICore(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(TxICore)

    auto c = pybind11::class_<TxICore>(m, "TxICore", rdoc_scope)
        .def("core", &TxICore::core,
            pybind11::return_value_policy::reference_internal, rdoc::core)
        .def("bdryTet", &TxICore::bdryTet, rdoc::bdryTet)
        .def("bdryRoles", &TxICore::bdryRoles, rdoc::bdryRoles)
        .def("bdryReln", &TxICore::bdryReln,
            pybind11::return_value_policy::reference_internal, rdoc::bdryReln)
        .def("parallelReln", &TxICore::parallelReln,
            pybind11::return_value_policy::reference_internal,
            rdoc::parallelReln)
        .def("name", &TxICore::name, rdoc::name)
        .def("texName", &TxICore::texName, rdoc::texName)
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    regina::python::add_eq_operators(c, rdoc::__eq);

    RDOC_SCOPE_SWITCH(TxIDiagonalCore)

    auto d = pybind11::class_<TxIDiagonalCore, regina::TxICore>(
            m, "TxIDiagonalCore", rdoc_scope)
        .def(pybind11::init<size_t, size_t>(), rdoc::__init)
        .def(pybind11::init<const TxIDiagonalCore&>(), rdoc::__copy)
        .def("swap", &TxIDiagonalCore::swap, rdoc::swap)
        .def("size", &TxIDiagonalCore::size, rdoc::size)
        .def("k", &TxIDiagonalCore::k, rdoc::k)
    ;
    regina::python::add_output(d);

    regina::python::add_global_swap<TxIDiagonalCore>(m, rdoc::global_swap);

    RDOC_SCOPE_SWITCH(TxIParallelCore)

    auto p = pybind11::class_<TxIParallelCore, regina::TxICore>(
            m, "TxIParallelCore", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const TxIParallelCore&>(), rdoc::__copy)
        .def("swap", &TxIParallelCore::swap, rdoc::swap)
    ;
    regina::python::add_output(p);

    regina::python::add_global_swap<TxIParallelCore>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

