
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
#include "../pybind11/stl.h"
#include "maths/vector.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::VectorInt;

namespace {
    // The static variables zero, one, minusOne in the class VectorInt
    // are deprecated in C++, and so generate compiler warnings if we attempt
    // to use them here.  For now, make our own replacements instead.
    regina::Integer zero(0), one(1), minusOne(-1);
}

void addVectorInt(pybind11::module_& m) {
    auto c = pybind11::class_<VectorInt>(m, "VectorInt")
        .def(pybind11::init<size_t>())
        .def(pybind11::init<size_t, const regina::Integer&>())
        .def(pybind11::init<const VectorInt&>())
        .def(pybind11::init([](const std::vector<regina::Integer> v) {
            return new VectorInt(v.begin(), v.end());
        }))
        .def("size", &VectorInt::size)
        .def("__getitem__", [](VectorInt& v, size_t index) ->
                regina::Integer& {
            return v[index];
        }, pybind11::return_value_policy::reference_internal)
        .def("setElement", [](VectorInt& v, size_t index, // deprecated
                const regina::Integer& value) {
            v[index] = value;
        })
        .def("__setitem__", [](VectorInt& v, size_t index,
                const regina::Integer& value) {
            v[index] = value;
        })
        .def("__iter__", [](const VectorInt& list) {
            return pybind11::make_iterator(list);
        }, pybind11::keep_alive<0, 1>()) // iterator keeps vector alive
        .def(pybind11::self += pybind11::self)
        .def(pybind11::self -= pybind11::self)
        .def(pybind11::self *= regina::Integer())
        .def(pybind11::self + pybind11::self)
        .def(pybind11::self - pybind11::self)
        .def(pybind11::self * regina::Integer())
        .def(pybind11::self * pybind11::self)
        .def("negate", &VectorInt::negate)
        .def("norm", &VectorInt::norm)
        .def("elementSum", &VectorInt::elementSum)
        .def("addCopies", &VectorInt::addCopies)
        .def("subtractCopies", &VectorInt::subtractCopies)
        .def("isZero", &VectorInt::isZero)
        // The C-style casts below are to avoid a compile error under gcc7
        // (but not gcc8), where the compiler cannot determine the type of a
        // template member function.
        .def("scaleDown",
            (void (VectorInt::*)())
            &VectorInt::scaleDown)
        .def_static("unit", &VectorInt::unit)
        .def_readonly_static("zero", &zero)
        .def_readonly_static("one", &one)
        .def_readonly_static("minusOne", &minusOne)
    ;
    regina::python::add_output(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(VectorInt&, VectorInt&))(regina::swap));
}

