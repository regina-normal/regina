
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include "subcomplex/ntxicore.h"
#include <boost/python.hpp>

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
    class_<NTxICore, bases<regina::ShareableObject>,
            std::auto_ptr<NTxICore>, boost::noncopyable>("NTxICore", no_init)
        .def("core", &NTxICore::core,
            return_internal_reference<>())
        .def("bdryTet", &NTxICore::bdryTet)
        .def("bdryRoles", &NTxICore::bdryRoles)
        .def("bdryReln", &NTxICore::bdryReln,
            return_internal_reference<>())
        .def("parallelReln", &NTxICore::parallelReln,
            return_internal_reference<>())
        .def("writeName", writeName_stdio)
        .def("writeTeXName", writeTeXName_stdio)
    ;

    class_<NTxIDiagonalCore, bases<regina::NTxICore>,
            std::auto_ptr<NTxIDiagonalCore>, boost::noncopyable>
            ("NTxIDiagonalCore", init<unsigned long, unsigned long>())
        .def("size", &NTxIDiagonalCore::size)
        .def("k", &NTxIDiagonalCore::k)
    ;

    class_<NTxIParallelCore, bases<regina::NTxICore>,
            std::auto_ptr<NTxIParallelCore>, boost::noncopyable>
            ("NTxIParallelCore")
    ;

    implicitly_convertible<std::auto_ptr<NTxIDiagonalCore>,
        std::auto_ptr<regina::NTxICore> >();
    implicitly_convertible<std::auto_ptr<NTxIParallelCore>,
        std::auto_ptr<regina::NTxICore> >();
}

