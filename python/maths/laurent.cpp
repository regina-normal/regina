
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "maths/integer.h"
#include "maths/laurent.h"
#include "../helpers.h"
#include "../docstrings/maths/laurent.h"

using pybind11::overload_cast;
using regina::Integer;
using regina::Laurent;

void addLaurent(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Laurent)

    auto c = pybind11::class_<Laurent<regina::Integer>>(m, "Laurent",
            rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<long>(), rdoc::__init)
        .def(pybind11::init<const Laurent<Integer>&>(), rdoc::__copy)
        .def(pybind11::init([](long minExp, const std::vector<Integer>& c) {
            return new Laurent<Integer>(minExp, c.begin(), c.end());
        }), pybind11::arg("minExp"), pybind11::arg("coefficients"),
            rdoc::__init_2)
        // overload_cast has trouble with templated vs non-templated overloads.
        // Just cast directly.
        .def("init", static_cast<void (Laurent<regina::Integer>::*)()>(
            &Laurent<regina::Integer>::init), rdoc::init)
        .def("init", static_cast<void (Laurent<regina::Integer>::*)(long)>(
            &Laurent<regina::Integer>::init), rdoc::init_2)
        .def("init", [](Laurent<regina::Integer>& p, long minExp,
                const std::vector<Integer>& c) {
            p.init(minExp, c.begin(), c.end());
        }, pybind11::arg("minExp"), pybind11::arg("coefficients"), rdoc::init_3)
        .def("minExp", &Laurent<regina::Integer>::minExp, rdoc::minExp)
        .def("maxExp", &Laurent<regina::Integer>::maxExp, rdoc::maxExp)
        .def("isZero", &Laurent<regina::Integer>::isZero, rdoc::isZero)
        .def("__getitem__", [](const Laurent<regina::Integer>& p, long exp) {
            return p[exp];
        }, pybind11::return_value_policy::copy, // to enforce constness
            rdoc::__array)
        .def("__setitem__", [](Laurent<regina::Integer>& p, long exp,
                const regina::Integer& value) {
            p.set(exp, value);
        }, rdoc::__array)
        .def("set", &Laurent<regina::Integer>::set, rdoc::set)
        .def("swap", &Laurent<regina::Integer>::swap, rdoc::swap)
        .def("shift", &Laurent<regina::Integer>::shift, rdoc::shift)
        .def("scaleUp", &Laurent<regina::Integer>::scaleUp, rdoc::scaleUp)
        .def("scaleDown", &Laurent<regina::Integer>::scaleDown, rdoc::scaleDown)
        .def("negate", &Laurent<regina::Integer>::negate, rdoc::negate)
        .def("invertX", &Laurent<regina::Integer>::invertX, rdoc::invertX)
        .def("str", overload_cast<const char*>(
            &Laurent<regina::Integer>::str, pybind11::const_), rdoc::str)
        .def("utf8", overload_cast<const char*>(
            &Laurent<regina::Integer>::utf8, pybind11::const_), rdoc::utf8)
        .def(pybind11::self *= regina::Integer(), rdoc::__imul)
        .def(pybind11::self /= regina::Integer(), rdoc::__idiv)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= pybind11::self, rdoc::__imul_2)
        .def(pybind11::self * regina::Integer(), rdoc_global::__mul)
        .def(regina::Integer() * pybind11::self, rdoc_global::__mul_2)
        .def(pybind11::self / regina::Integer(), rdoc_global::__div)
        .def(pybind11::self + pybind11::self, rdoc_global::__add)
        .def(pybind11::self - pybind11::self, rdoc_global::__sub_2)
        .def(pybind11::self * pybind11::self, rdoc_global::__mul_3)
        .def(- pybind11::self, rdoc_global::__sub)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    regina::python::add_global_swap<Laurent<regina::Integer>>(m,
        rdoc::global_swap);

    RDOC_SCOPE_END
}

