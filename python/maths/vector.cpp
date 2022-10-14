
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

template <typename T>
void addVectorOf(pybind11::module_& m, const char* className) {
    using Vec = regina::Vector<T>;

    namespace global = regina::python::doc;

    RDOC_SCOPE_BEGIN(Vector)

    auto c = pybind11::class_<Vec>(m, className, rdoc_scope)
        .def(pybind11::init<size_t>(), rdoc::__init)
        .def(pybind11::init<size_t, const T&>(), rdoc::__init_2)
        .def(pybind11::init([](const std::vector<T> v) {
            return new Vec(v.begin(), v.end());
        }), rdoc::__init_3)
        .def(pybind11::init<const Vec&>(), rdoc::__init_4)
        .def("size", &Vec::size, rdoc::size)
        .def("__getitem__", [](Vec& v, size_t index) -> T& {
            return v[index];
        }, pybind11::return_value_policy::reference_internal, rdoc::__array)
        .def("__setitem__", [](Vec& v, size_t index, const T& value) {
            v[index] = value;
        }, rdoc::__array_2)
        .def("__iter__", [](const Vec& list) {
            return pybind11::make_iterator(list);
        }, pybind11::keep_alive<0, 1>(), // iterator keeps vector alive
            rdoc::__iter__)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= T(), rdoc::__imul)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def(pybind11::self - pybind11::self, rdoc::__sub)
        .def(pybind11::self * T(), rdoc::__mul)
        .def(pybind11::self * pybind11::self, rdoc::__mul_2)
        .def("swap", &Vec::swap, rdoc::swap)
        .def("negate", &Vec::negate, rdoc::negate)
        .def("norm", &Vec::norm, rdoc::norm)
        .def("elementSum", &Vec::elementSum, rdoc::elementSum)
        .def("addCopies", &Vec::addCopies, rdoc::addCopies)
        .def("subtractCopies", &Vec::subtractCopies, rdoc::subtractCopies)
        .def("isZero", &Vec::isZero, rdoc::isZero)
        // The static casts below are to avoid a compile error under gcc7
        // (but not gcc8), where the compiler cannot determine the type of a
        // template member function.
        .def("scaleDown",
            static_cast<T (Vec::*)()>(&Vec::scaleDown), rdoc::scaleDown)
        .def_static("unit", &Vec::unit, rdoc::unit)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<Vec>(m, rdoc::global_swap);

    RDOC_SCOPE_END

    pybind11::implicitly_convertible<std::vector<int>, Vec>();
    pybind11::implicitly_convertible<std::vector<T>, Vec>();
}

void addVector(pybind11::module_& m) {
    addVectorOf<regina::Integer>(m, "VectorInt");
    addVectorOf<regina::LargeInteger>(m, "VectorLarge");
}

