
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "subcomplex/txicore.h"
#include "../helpers.h"

using namespace boost::python;
using regina::TxICore;
using regina::TxIDiagonalCore;
using regina::TxIParallelCore;

namespace {
    void writeName_stdio(const TxICore& c) {
        c.writeName(std::cout);
    }
    void writeTeXName_stdio(const TxICore& c) {
        c.writeTeXName(std::cout);
    }
}

void addTxICore() {
    class_<TxICore, std::auto_ptr<TxICore>, boost::noncopyable>
            ("TxICore", no_init)
        .def("core", &TxICore::core,
            return_internal_reference<>())
        .def("bdryTet", &TxICore::bdryTet)
        .def("bdryRoles", &TxICore::bdryRoles)
        .def("bdryReln", &TxICore::bdryReln,
            return_internal_reference<>())
        .def("parallelReln", &TxICore::parallelReln,
            return_internal_reference<>())
        .def("name", &TxICore::name)
        .def("TeXName", &TxICore::TeXName)
        .def("writeName", writeName_stdio)
        .def("writeTeXName", writeTeXName_stdio)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NTxICore") = scope().attr("TxICore");

    class_<TxIDiagonalCore, bases<regina::TxICore>,
            std::auto_ptr<TxIDiagonalCore>, boost::noncopyable>
            ("TxIDiagonalCore", init<unsigned long, unsigned long>())
        .def("size", &TxIDiagonalCore::size)
        .def("k", &TxIDiagonalCore::k)
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NTxIDiagonalCore") = scope().attr("TxIDiagonalCore");

    class_<TxIParallelCore, bases<regina::TxICore>,
            std::auto_ptr<TxIParallelCore>, boost::noncopyable>
            ("TxIParallelCore", init<>())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NTxIParallelCore") = scope().attr("TxIParallelCore");

    implicitly_convertible<std::auto_ptr<TxIDiagonalCore>,
        std::auto_ptr<regina::TxICore> >();
    implicitly_convertible<std::auto_ptr<TxIParallelCore>,
        std::auto_ptr<regina::TxICore> >();
}

