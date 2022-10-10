
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
#include "../pybind11/stl.h"
#include "maths/vector.h"
#include "../helpers.h"
#include "../docstrings/maths/vector.h"

using pybind11::overload_cast;
using regina::VectorInt;

void addVectorInt(pybind11::module_& m) {
    namespace global = regina::python::doc;

    RDOC_SCOPE_BEGIN(Vector)

    auto c = pybind11::class_<VectorInt>(m, "VectorInt", rdoc_scope)
        .def(pybind11::init<size_t>(), rdoc::Vector)
        .def(pybind11::init<size_t, const regina::Integer&>(), rdoc::Vector_2)
        .def(pybind11::init([](const std::vector<regina::Integer> v) {
            return new VectorInt(v.begin(), v.end());
        }), rdoc::Vector_3)
        .def(pybind11::init<const VectorInt&>(), rdoc::Vector_4)
        .def("size", &VectorInt::size, rdoc::size)
        .def("__getitem__", [](VectorInt& v, size_t index) ->
                regina::Integer& {
            return v[index];
        }, pybind11::return_value_policy::reference_internal, rdoc::__array)
        .def("__setitem__", [](VectorInt& v, size_t index,
                const regina::Integer& value) {
            v[index] = value;
        }, rdoc::__array_2)
        .def("__iter__", [](const VectorInt& list) {
            return pybind11::make_iterator(list);
        }, pybind11::keep_alive<0, 1>(), // iterator keeps vector alive
            global::common::iter)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= regina::Integer(), rdoc::__imul)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def(pybind11::self - pybind11::self, rdoc::__sub)
        .def(pybind11::self * regina::Integer(), rdoc::__mul)
        .def(pybind11::self * pybind11::self, rdoc::__mul_2)
        .def("swap", &VectorInt::swap, rdoc::swap)
        .def("negate", &VectorInt::negate, rdoc::negate)
        .def("norm", &VectorInt::norm, rdoc::norm)
        .def("elementSum", &VectorInt::elementSum, rdoc::elementSum)
        .def("addCopies", &VectorInt::addCopies, rdoc::addCopies)
        .def("subtractCopies", &VectorInt::subtractCopies, rdoc::subtractCopies)
        .def("isZero", &VectorInt::isZero, rdoc::isZero)
        // The C-style casts below are to avoid a compile error under gcc7
        // (but not gcc8), where the compiler cannot determine the type of a
        // template member function.
        .def("scaleDown",
            (regina::Integer (VectorInt::*)())(&VectorInt::scaleDown),
            rdoc::scaleDown)
        .def_static("unit", &VectorInt::unit, rdoc::unit)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<VectorInt>(m, global::swap);

    RDOC_SCOPE_END

    pybind11::implicitly_convertible<std::vector<int>, VectorInt>();
    pybind11::implicitly_convertible<std::vector<regina::Integer>, VectorInt>();
}

