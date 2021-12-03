
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
#include "../pybind11/operators.h"
#include "utilities/qitmask.h"
#include "utilities/intutils.h"
#include "../helpers.h"

using regina::Qitmask1;
using regina::Qitmask2;

template <class Q>
void addQitmaskOpt(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<Q>(m, name)
        .def(pybind11::init<>())
        .def(pybind11::init<const Q&>())
        .def("reset", &Q::reset)
        .def("get", &Q::get)
        .def("set", &Q::set)
        .def("empty", &Q::empty)
        .def("nonEmpty", &Q::nonEmpty)
        .def("has3", &Q::has3)
        .def(pybind11::self += pybind11::self)
        .def(pybind11::self -= pybind11::self)
        .def("hasNonZeroMatch", &Q::hasNonZeroMatch)
    ;
    regina::python::add_output_ostream(c, name);
    regina::python::add_eq_operators(c);
}

void addQitmask(pybind11::module_& m) {
    addQitmaskOpt<Qitmask1<uint8_t>>(m, "Qitmask8");
    addQitmaskOpt<Qitmask1<uint16_t>>(m, "Qitmask16");
    addQitmaskOpt<Qitmask1<uint32_t>>(m, "Qitmask32");
    addQitmaskOpt<Qitmask1<uint64_t>>(m, "Qitmask64");
    #ifdef INT128_AVAILABLE
    addQitmaskOpt<Qitmask1<regina::IntOfSize<16>::utype>>(m, "Qitmask128");
    addQitmaskOpt<Qitmask2<regina::IntOfSize<16>::utype,
        regina::IntOfSize<16>::utype>>(m, "Qitmask256");
    #else
    addQitmaskOpt<Qitmask2<uint64_t, uint64_t>>(m, "Qitmask128");
    #endif
}

