
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
#include "../pybind11/operators.h"
#include "utilities/boolset.h"
#include "../helpers.h"
#include "../docstrings/utilities/boolset.h"

using regina::BoolSet;

void addBoolSet(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(BoolSet)

    auto c = pybind11::class_<BoolSet>(m, "BoolSet", rdoc_scope)
        .def(pybind11::init<>(), rdoc::BoolSet)
        .def(pybind11::init<bool>(), rdoc::BoolSet_2)
        .def(pybind11::init<const BoolSet&>(), rdoc::BoolSet_3)
        .def(pybind11::init<bool, bool>(), rdoc::BoolSet_4)
        .def("hasTrue", &BoolSet::hasTrue, rdoc::hasTrue)
        .def("hasFalse", &BoolSet::hasFalse, rdoc::hasFalse)
        .def("contains", &BoolSet::contains, rdoc::contains)
        .def("insertTrue", &BoolSet::insertTrue, rdoc::insertTrue)
        .def("insertFalse", &BoolSet::insertFalse, rdoc::insertFalse)
        .def("removeTrue", &BoolSet::removeTrue, rdoc::removeTrue)
        .def("removeFalse", &BoolSet::removeFalse, rdoc::removeFalse)
        .def("empty", &BoolSet::empty, rdoc::empty)
        .def("fill", &BoolSet::fill, rdoc::fill)
        .def(pybind11::self < pybind11::self, rdoc::__lt)
        .def(pybind11::self > pybind11::self, rdoc::__gt)
        .def(pybind11::self <= pybind11::self, rdoc::__le)
        .def(pybind11::self >= pybind11::self, rdoc::__ge)
        .def(pybind11::self |= pybind11::self, rdoc::__ior)
        .def(pybind11::self &= pybind11::self, rdoc::__iand)
        .def(pybind11::self ^= pybind11::self, rdoc::__ixor)
        .def(pybind11::self | pybind11::self, rdoc::__bor)
        .def(pybind11::self & pybind11::self, rdoc::__band)
        .def(pybind11::self ^ pybind11::self, rdoc::__bxor)
        .def(~ pybind11::self, rdoc::__bnot)
        .def("byteCode", &BoolSet::byteCode, rdoc::byteCode)
        .def("setByteCode", &BoolSet::setByteCode, rdoc::setByteCode)
        .def_static("fromByteCode", &BoolSet::fromByteCode, rdoc::fromByteCode)
        .def("stringCode", &BoolSet::stringCode, rdoc::stringCode)
        .def("setStringCode", &BoolSet::setStringCode, rdoc::setStringCode)
    ;
    regina::python::add_output_ostream(c);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_END

    pybind11::implicitly_convertible<bool, BoolSet>();
}

