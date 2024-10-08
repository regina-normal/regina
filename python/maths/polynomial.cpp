
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
#include "maths/polynomial.h"
#include "maths/rational.h"
#include "../helpers.h"
#include "../docstrings/maths/polynomial.h"

using pybind11::overload_cast;
using regina::Polynomial;
using regina::Rational;

void addPolynomial(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Polynomial)

    auto c = pybind11::class_<Polynomial<Rational>>(m, "Polynomial", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<size_t>(), rdoc::__init)
        .def(pybind11::init<const Polynomial<Rational>&>(), rdoc::__copy)
        .def(pybind11::init([](const std::vector<Rational>& coeffs) {
            return new Polynomial<Rational>(coeffs.begin(), coeffs.end());
        }), pybind11::arg("coefficients"), rdoc::__init_2)
        // overload_cast has trouble with templated vs non-templated overloads.
        // Just cast directly.
        .def("init", static_cast<void (Polynomial<Rational>::*)()>(
            &Polynomial<Rational>::init), rdoc::init)
        .def("init", static_cast<void (Polynomial<Rational>::*)(size_t)>(
            &Polynomial<Rational>::init), rdoc::init_2)
        .def("init", [](Polynomial<Rational>& p,
                const std::vector<Rational>& c) {
            p.init(c.begin(), c.end());
        }, pybind11::arg("coefficients"), rdoc::init_3)
        .def("degree", &Polynomial<Rational>::degree, rdoc::degree)
        .def("isZero", &Polynomial<Rational>::isZero, rdoc::isZero)
        .def("isMonic", &Polynomial<Rational>::isMonic, rdoc::isMonic)
        .def("leading", &Polynomial<Rational>::leading,
            pybind11::return_value_policy::copy, // to enforce constness
            rdoc::leading)
        .def("__getitem__", [](const Polynomial<Rational>& p, size_t exp) {
            return p[exp];
        }, pybind11::return_value_policy::copy, // to enforce constness
            rdoc::__array)
        .def("__setitem__", [](Polynomial<Rational>& p, size_t exp,
                const regina::Rational& value) {
            p.set(exp, value);
        }, rdoc::__array)
        .def("set", &Polynomial<Rational>::set, rdoc::set)
        .def("swap", &Polynomial<Rational>::swap, rdoc::swap)
        .def("negate", &Polynomial<Rational>::negate, rdoc::negate)
        .def("str", overload_cast<const char*>(
            &Polynomial<Rational>::str, pybind11::const_), rdoc::str)
        .def("utf8", overload_cast<const char*>(
            &Polynomial<Rational>::utf8, pybind11::const_), rdoc::utf8)
        .def(pybind11::self *= Rational(), rdoc::__imul)
        .def(pybind11::self /= Rational(), rdoc::__idiv)
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
        .def("divisionAlg", overload_cast<const Polynomial<Rational>&>(
            &Polynomial<Rational>::divisionAlg, pybind11::const_),
            rdoc::divisionAlg)
        .def("gcdWithCoeffs", &Polynomial<Rational>::gcdWithCoeffs<Rational>,
            rdoc::gcdWithCoeffs)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    regina::python::add_global_swap<Polynomial<Rational>>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

