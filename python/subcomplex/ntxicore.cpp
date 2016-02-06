
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
#include "subcomplex/ntxicore.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NTxICore;
using regina::NTxIDiagonalCore;
using regina::NTxIParallelCore;

namespace {
    void writeName_stdio(const NTxICore& c) {
        c.writeName(std::cout);
    }
    void writeTeXName_stdio(const NTxICore& c) {
        c.writeTeXName(std::cout);
    }
}

void addNTxICore() {
    class_<NTxICore, std::auto_ptr<NTxICore>, boost::noncopyable>
            ("NTxICore", no_init)
        .def("core", &NTxICore::core,
            return_internal_reference<>())
        .def("bdryTet", &NTxICore::bdryTet)
        .def("bdryRoles", &NTxICore::bdryRoles)
        .def("bdryReln", &NTxICore::bdryReln,
            return_internal_reference<>())
        .def("parallelReln", &NTxICore::parallelReln,
            return_internal_reference<>())
        .def("name", &NTxICore::name)
        .def("getName", &NTxICore::name)
        .def("TeXName", &NTxICore::TeXName)
        .def("getTeXName", &NTxICore::TeXName)
        .def("writeName", writeName_stdio)
        .def("writeTeXName", writeTeXName_stdio)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<NTxIDiagonalCore, bases<regina::NTxICore>,
            std::auto_ptr<NTxIDiagonalCore>, boost::noncopyable>
            ("NTxIDiagonalCore", init<unsigned long, unsigned long>())
        .def("size", &NTxIDiagonalCore::size)
        .def("k", &NTxIDiagonalCore::k)
        .def(regina::python::add_eq_operators())
    ;

    class_<NTxIParallelCore, bases<regina::NTxICore>,
            std::auto_ptr<NTxIParallelCore>, boost::noncopyable>
            ("NTxIParallelCore", init<>())
        .def(regina::python::add_eq_operators())
    ;

    implicitly_convertible<std::auto_ptr<NTxIDiagonalCore>,
        std::auto_ptr<regina::NTxICore> >();
    implicitly_convertible<std::auto_ptr<NTxIParallelCore>,
        std::auto_ptr<regina::NTxICore> >();
}

