
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
#include <pybind11/stl.h>
#include "maths/integer.h"
#include "maths/laurent2.h"
#include "../helpers.h"
#include "../docstrings/maths/laurent2.h"

using pybind11::overload_cast;
using regina::Integer;
using regina::Laurent2;

void addLaurent2(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Laurent2)

    auto c = pybind11::class_<Laurent2<Integer>>(m, "Laurent2", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Laurent2<Integer>&>(), rdoc::__copy)
        .def(pybind11::init([](long xExp, long yExp) { // deprecated
            Laurent2<Integer> ans;
            ans.initExp(xExp, yExp);
            return ans;
        }) , rdoc::__init)
        .def(pybind11::init<const Laurent2<Integer>&, long, long>(),
            rdoc::__init_2)
        .def(pybind11::init([](const std::vector<
                std::tuple<long, long, Integer>>& coeffs) {
            return new Laurent2<Integer>(coeffs.begin(), coeffs.end());
        }), pybind11::arg("coefficients"), rdoc::__init_3)
        .def("init", overload_cast<>(&Laurent2<Integer>::init), rdoc::init)
        .def("initExp", &Laurent2<Integer>::initExp, rdoc::initExp)
        .def("init", &Laurent2<Integer>::initExp, rdoc::init_2) // deprecated
        .def("isZero", &Laurent2<Integer>::isZero, rdoc::isZero)
        .def("set", &Laurent2<Integer>::set, rdoc::set)
        .def("swap", &Laurent2<Integer>::swap, rdoc::swap)
        .def("negate", &Laurent2<Integer>::negate, rdoc::negate)
        .def("invertX", &Laurent2<Integer>::invertX, rdoc::invertX)
        .def("invertY", &Laurent2<Integer>::invertY, rdoc::invertY)
        .def("str", overload_cast<const char*, const char*>(
            &Laurent2<Integer>::str, pybind11::const_),
            pybind11::arg(), pybind11::arg("varY") = nullptr, rdoc::str)
        .def("utf8", overload_cast<const char*, const char*>(
            &Laurent2<Integer>::utf8, pybind11::const_),
            pybind11::arg(), pybind11::arg("varY") = nullptr, rdoc::utf8)
        .def("__getitem__", [](const Laurent2<Integer>& p,
                std::pair<long, long> exponents) {
            return p(exponents.first, exponents.second);
        }, pybind11::return_value_policy::copy, // to enforce constness
            rdoc::__call)
        .def("__setitem__", [](Laurent2<Integer>& p,
                std::pair<long, long> exponents, const Integer& value) {
            p.set(exponents.first, exponents.second, value);
        }, rdoc::__call)
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

    regina::python::add_global_swap<Laurent2<Integer>>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

