
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

#include "../pybind11/pybind11.h"
#include "../pybind11/complex.h"
#include "../pybind11/operators.h"
#include "../pybind11/stl.h"
#include "maths/cyclotomic.h"
#include "../helpers.h"
#include "../docstrings/maths/cyclotomic.h"

using pybind11::overload_cast;
using regina::Cyclotomic;
using regina::Rational;

void addCyclotomic(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Cyclotomic)

    auto c = pybind11::class_<Cyclotomic>(m, "Cyclotomic", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<size_t>(), rdoc::__init)
        .def(pybind11::init<size_t, int>(), rdoc::__init_2)
        .def(pybind11::init<size_t, const regina::Rational&>(), rdoc::__init_3)
        .def(pybind11::init<const Cyclotomic&>(), rdoc::__copy)
        .def(pybind11::init([](size_t field, const std::vector<Rational>& c) {
            return new Cyclotomic(field, c.begin(), c.end());
        }), pybind11::arg("field"), pybind11::arg("coefficients"),
            rdoc::__init_4)
        .def("init", &Cyclotomic::init, rdoc::init)
        .def("field", &Cyclotomic::field, rdoc::field)
        .def("degree", &Cyclotomic::degree, rdoc::degree)
        .def("__len__", &Cyclotomic::degree, rdoc::degree)
        .def("__getitem__", [](Cyclotomic& c, int exp) -> regina::Rational& {
            return c[exp];
        }, pybind11::return_value_policy::reference_internal, rdoc::__array)
        .def("__setitem__", [](Cyclotomic& c, int exp,
                const regina::Rational& value) {
            c[exp] = value;
        }, rdoc::__array_2)
        .def("polynomial", &Cyclotomic::polynomial, rdoc::polynomial)
        .def("evaluate", &Cyclotomic::evaluate,
            pybind11::arg("whichRoot") = 1, rdoc::evaluate)
        .def("swap", &Cyclotomic::swap, rdoc::swap)
        .def("negate", &Cyclotomic::negate, rdoc::negate)
        .def("invert", &Cyclotomic::invert, rdoc::invert)
        .def("inverse", &Cyclotomic::inverse, rdoc::inverse)
        .def(pybind11::self *= regina::Rational(), rdoc::__imul)
        .def(pybind11::self /= regina::Rational(), rdoc::__idiv)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= pybind11::self, rdoc::__imul_2)
        .def(pybind11::self /= pybind11::self, rdoc::__idiv_2)
        .def(pybind11::self * regina::Rational(), rdoc_global::__mul)
        .def(regina::Rational() * pybind11::self, rdoc_global::__mul_2)
        .def(pybind11::self / regina::Rational(), rdoc_global::__div)
        .def(pybind11::self + pybind11::self, rdoc_global::__add)
        .def(pybind11::self - pybind11::self, rdoc_global::__sub_2)
        .def(pybind11::self * pybind11::self, rdoc_global::__mul_3)
        .def(pybind11::self / pybind11::self, rdoc_global::__div_2)
        .def(- pybind11::self, rdoc_global::__sub)
        .def_static("cyclotomic", [](size_t n) {
            return new regina::Polynomial<regina::Rational>(
                Cyclotomic::cyclotomic(n));
        }, rdoc::cyclotomic)
        .def("str", overload_cast<const char*>(
            &Cyclotomic::str, pybind11::const_), rdoc::str)
        .def("utf8", overload_cast<const char*>(
            &Cyclotomic::utf8, pybind11::const_), rdoc::utf8)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<Cyclotomic>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

