
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
#include "subcomplex/txicore.h"
#include "../helpers.h"

using regina::TxICore;
using regina::TxIDiagonalCore;
using regina::TxIParallelCore;

void addTxICore(pybind11::module_& m) {
    auto c = pybind11::class_<TxICore>(m, "TxICore")
        .def("core", [](const TxICore& c) {
            // Return a fresh clone.  This is because we are using a custom
            // holder type, and so Python will ignore any return value policy
            // and always claim ownership over the returned triangulation.
            return new regina::Triangulation<3>(c.core());
        })
        .def("bdryTet", &TxICore::bdryTet)
        .def("bdryRoles", &TxICore::bdryRoles)
        .def("bdryReln", &TxICore::bdryReln,
            pybind11::return_value_policy::reference_internal)
        .def("parallelReln", &TxICore::parallelReln,
            pybind11::return_value_policy::reference_internal)
        .def("name", &TxICore::name)
        .def("TeXName", &TxICore::TeXName)
        .def("writeName", [](const TxICore& c) {
            c.writeName(std::cout);
        })
        .def("writeTeXName", [](const TxICore& c) {
            c.writeTeXName(std::cout);
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("NTxICore") = m.attr("TxICore");

    pybind11::class_<TxIDiagonalCore, regina::TxICore>(m, "TxIDiagonalCore")
        .def(pybind11::init<unsigned long, unsigned long>())
        .def("size", &TxIDiagonalCore::size)
        .def("k", &TxIDiagonalCore::k)
    ;

    m.attr("NTxIDiagonalCore") = m.attr("TxIDiagonalCore");

    pybind11::class_<TxIParallelCore, regina::TxICore>(m, "TxIParallelCore")
        .def(pybind11::init<>())
    ;

    m.attr("NTxIParallelCore") = m.attr("TxIParallelCore");
}

