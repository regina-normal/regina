
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

    auto c = pybind11::class_<Laurent<Integer>>(m, "Laurent", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Laurent<Integer>&>(), rdoc::__copy)
        .def(pybind11::init([](long exp) { // deprecated
            Laurent<Integer> ans;
            ans.initExp(exp);
            return ans;
        }), rdoc::__init)
        .def(pybind11::init([](long minExp, const std::vector<Integer>& c) {
            return new Laurent<Integer>(minExp, c.begin(), c.end());
        }), pybind11::arg("minExp"), pybind11::arg("coefficients"),
            rdoc::__init_2)
        // overload_cast has trouble with templated vs non-templated overloads.
        // Just cast directly.
        .def("init", static_cast<void (Laurent<Integer>::*)()>(
            &Laurent<Integer>::init), rdoc::init)
        .def("initExp", &Laurent<Integer>::initExp, rdoc::initExp)
        .def("init", &Laurent<Integer>::initExp, rdoc::init_2) // deprecated
        .def("init", [](Laurent<Integer>& p, long minExp,
                const std::vector<Integer>& c) {
            p.init(minExp, c.begin(), c.end());
        }, pybind11::arg("minExp"), pybind11::arg("coefficients"), rdoc::init_3)
        .def("minExp", &Laurent<Integer>::minExp, rdoc::minExp)
        .def("maxExp", &Laurent<Integer>::maxExp, rdoc::maxExp)
        .def("isZero", &Laurent<Integer>::isZero, rdoc::isZero)
        .def("__getitem__", [](const Laurent<Integer>& p, long exp) {
            return p[exp];
        }, pybind11::return_value_policy::copy, // to enforce constness
            rdoc::__array)
        .def("__setitem__", [](Laurent<Integer>& p, long exp,
                const Integer& value) {
            p.set(exp, value);
        }, rdoc::__array)
        .def("set", &Laurent<Integer>::set, rdoc::set)
        .def("swap", &Laurent<Integer>::swap, rdoc::swap)
        .def("shift", &Laurent<Integer>::shift, rdoc::shift)
        .def("scaleUp", &Laurent<Integer>::scaleUp, rdoc::scaleUp)
        .def("scaleDown", &Laurent<Integer>::scaleDown, rdoc::scaleDown)
        .def("negate", &Laurent<Integer>::negate, rdoc::negate)
        .def("invertX", &Laurent<Integer>::invertX, rdoc::invertX)
        .def("str", overload_cast<const char*>(
            &Laurent<Integer>::str, pybind11::const_), rdoc::str)
        .def("utf8", overload_cast<const char*>(
            &Laurent<Integer>::utf8, pybind11::const_), rdoc::utf8)
        .def(pybind11::self *= Integer(), rdoc::__imul)
        .def(pybind11::self /= Integer(), rdoc::__idiv)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= pybind11::self, rdoc::__imul_2)
        .def(pybind11::self * Integer(), rdoc_global::__mul)
        .def(Integer() * pybind11::self, rdoc_global::__mul_2)
        .def(pybind11::self / Integer(), rdoc_global::__div)
        .def(pybind11::self + pybind11::self, rdoc_global::__add)
        .def(pybind11::self - pybind11::self, rdoc_global::__sub_2)
        .def(pybind11::self * pybind11::self, rdoc_global::__mul_3)
        .def(- pybind11::self, rdoc_global::__sub)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    regina::python::add_global_swap<Laurent<Integer>>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

