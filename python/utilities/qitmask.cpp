
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "utilities/qitmask.h"
#include "utilities/intutils.h"
#include "../helpers.h"
#include "../docstrings/utilities/qitmask.h"

using regina::Qitmask1;
using regina::Qitmask2;

template <class Q>
void addQitmaskOpt(pybind11::module_& m, const char* name) {
    // Q could be an instance of either Qitmask1 or Qitmask2, but since the
    // Python docs are essentially the same we will just use Qitmask1 here.
    RDOC_SCOPE_BEGIN(Qitmask1)

    auto c = pybind11::class_<Q>(m, name, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Q&>(), rdoc::__copy)
        .def("reset", &Q::reset, rdoc::reset)
        .def("get", &Q::get, rdoc::get)
        .def("set", &Q::set, rdoc::set)
        .def("empty", &Q::empty, rdoc::empty)
        .def("nonEmpty", &Q::nonEmpty, rdoc::nonEmpty)
        .def("has3", &Q::has3, rdoc::has3)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def("hasNonZeroMatch", &Q::hasNonZeroMatch, rdoc::hasNonZeroMatch)
    ;
    regina::python::add_output_ostream(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    RDOC_SCOPE_END
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

