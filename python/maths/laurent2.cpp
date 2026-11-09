
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
#include "maths/laurent.h"
#include "../helpers.h"
#include "../docstrings/maths/laurent.h"

using namespace pybind11::literals;

using pybind11::overload_cast;
using regina::Integer;
using regina::Laurent;
using regina::python::doc::common::neq_value;

void addLaurent2(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(LaurentCoefficient)

    using LC = regina::LaurentCoefficient<Integer, 2>;
    auto c = pybind11::class_<LC>(m, "LaurentCoefficient2", rdoc::__class)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const LC&>(), rdoc::__copy)
        .def("__bool__", &LC::operator bool, rdoc::__as_bool)
        .def("inc", [](LC& c) { return c++; }, rdoc::__inc)
        .def("dec", [](LC& c) { return c--; }, rdoc::__dec)
        .def("coeff", &LC::operator *,
            pybind11::return_value_policy::copy, // to enforce constness
            rdoc::__mul)
        .def("exp", static_cast<long (LC::*)(int) const>(&LC::exp), rdoc::exp)
        .def("constantTerm", &LC::constantTerm, rdoc::constantTerm)
        .def("__next__", [](LC& it) {
            if (it) {
                auto ans = std::pair<std::pair<long, long>, Integer>(
                    { it.exp<0>(), it.exp<1>() }, *it);
                ++it;
                return ans;
            } else
                throw pybind11::stop_iteration();
        }, rdoc::__next__)
    ;
    regina::python::add_output_basic(c, rdoc::str);
    regina::python::add_eq_operators(c, rdoc::__eq);

    RDOC_SCOPE_SWITCH(Laurent)

    // overload_cast has trouble with templated vs non-templated overloads
    // and also overloads in the presence of perfect forwarding, and so there
    // are several places below where we just static_cast directly.
    using LL = Laurent<Integer, 2>;
    auto c = pybind11::class_<LL>(m, "Laurent2", rdoc::__class)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const LL&>(), rdoc::__copy)
        .def(pybind11::init<const Integer&>(), rdoc::__init_2)
        .def(pybind11::init<long>(), rdoc::__init_3)
        .def(pybind11::init<const Laurent<Integer>&, const Laurent<Integer>&>(),
            rdoc::__init_4)
        .def("init", &LL::init, rdoc::init)
        .def("initExp", &LL::initExp<long>, rdoc::initExp)
        .def_static("monomial", &LL::monomial<long>, rdoc::monomial)
        .def_static("embed",
            static_cast<LL (*)(const Laurent<Integer>&, int)>(&LL::embed),
            rdoc::embed)
        .def_static("diagonalise",
            overload_cast<const Laurent<Integer>&>(&LL::diagonalise),
            rdoc::diagonalise)
        .def("isZero", &LL::isZero, rdoc::isZero)
        .def("allocation", &LL::allocation, rdoc::allocation)
        .def("reserveRange", &LL::reserveRange, rdoc::reserveRange)
        .def("__getitem__", [](const LL& p, std::pair<long, long> exp) {
            return p(exp.first, exp.second);
        }, pybind11::return_value_policy::copy, // to enforce constness
            rdoc::__call)
        .def("__setitem__", [](LL& p, std::pair<long, long> exp,
                const Integer& value) {
            p.set(exp.first, exp.second, value);
        }, rdoc::__call)
        .def("__iter__", &LL::first,
            pybind11::keep_alive<0, 1>(), // iterator keeps polynomial alive
            rdoc::__iter__)
        .def("first", &LL::first,
            pybind11::keep_alive<0, 1>(), // iterator keeps polynomial alive
            rdoc::first)
        .def("last", &LL::last,
            pybind11::keep_alive<0, 1>(), // iterator keeps polynomial alive
            rdoc::last)
        .def("set", &LL::set, rdoc::set_2)
        .def("setTo", &LL::setTo<long>, rdoc::setTo)
        .def("swap", &LL::swap, rdoc::swap)
        .def("shift", &LL::shift<long>, rdoc::shift)
        .def("shifted",
            static_cast<LL (LL::*)(long&&, long&&) const&>(
                &LL::shifted<long, long>),
            rdoc::shifted)
        .def("negate", &LL::negate, rdoc::negate)
        .def("invertX", &LL::invertX, rdoc::invertX)
        .def("addProduct",
            overload_cast<const LL&, const LL&>(&LL::addProduct),
            rdoc::addProduct)
        .def("addProduct",
            overload_cast<const LL&, const Integer&>(&LL::addProduct),
            rdoc::addProduct_2)
        .def("str", &LL::str<const char*, const char*>, rdoc::str)
        .def("utf8", &LL::utf8<const char*, const char*>, rdoc::utf8)
        .def("validate", &LL::validate, rdoc::validate)
        .def(pybind11::self *= Integer(), rdoc::__imul)
        .def(pybind11::self *= long(), rdoc::__imul_2)
        .def(pybind11::self /= Integer(), rdoc::__idiv)
        .def(pybind11::self /= long(), rdoc::__idiv_2)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= pybind11::self, rdoc::__imul_3)
        .def(pybind11::self * Integer(), rdoc::__mul)
        .def(pybind11::self * long(), rdoc::__mul_2)
        .def(Integer() * pybind11::self, rdoc::__mul_3)
        .def(long() * pybind11::self, rdoc::__mul_4)
        .def(pybind11::self / Integer(), rdoc::__div)
        .def(pybind11::self / long(), rdoc::__div_2)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def(pybind11::self - pybind11::self, rdoc::__sub_2)
        .def(pybind11::self * pybind11::self, rdoc::__mul_5)
        .def(- pybind11::self, rdoc::__sub)
        .def(pybind11::self == Integer(), rdoc::__eq_2)
        .def(pybind11::self == long(), rdoc::__eq_3)
        .def(pybind11::self != Integer(), neq_value)
        .def(pybind11::self != long(), neq_value)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);
    regina::python::add_global_swap<LL, rdoc>(m);

    RDOC_SCOPE_END
}

