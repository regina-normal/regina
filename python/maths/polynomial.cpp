
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
#include "maths/polynomial.h"
#include "maths/rational.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Polynomial;
using regina::Rational;

void addPolynomial(pybind11::module_& m) {
    auto c = pybind11::class_<Polynomial<Rational>>(m, "Polynomial")
        .def(pybind11::init<>())
        .def(pybind11::init<size_t>())
        .def(pybind11::init<const Polynomial<Rational>&>())
        .def(pybind11::init([](const std::vector<Rational>& coeffs) {
            return new Polynomial<Rational>(coeffs.begin(), coeffs.end());
        }))
        // overload_cast has trouble with templated vs non-templated overloads.
        // Just cast directly.
        .def("init", (void (Polynomial<Rational>::*)())
            &Polynomial<Rational>::init)
        .def("init", (void (Polynomial<Rational>::*)(size_t))
            &Polynomial<Rational>::init)
        .def("init", [](Polynomial<Rational>& p,
                const std::vector<Rational>& c) {
            p.init(c.begin(), c.end());
        })
        .def("degree", &Polynomial<Rational>::degree)
        .def("isZero", &Polynomial<Rational>::isZero)
        .def("isMonic", &Polynomial<Rational>::isMonic)
        .def("leading", &Polynomial<Rational>::leading,
            pybind11::return_value_policy::copy) // to enforce constness
        .def("__getitem__", [](const Polynomial<Rational>& p, size_t exp) {
            return p[exp];
        }, pybind11::return_value_policy::copy) // to enforce constness
        .def("__setitem__", [](Polynomial<Rational>& p, size_t exp,
                const regina::Rational& value) {
            p.set(exp, value);
        })
        .def("set", &Polynomial<Rational>::set)
        .def("swap", &Polynomial<Rational>::swap)
        .def("negate", &Polynomial<Rational>::negate)
        .def("str", overload_cast<const char*>(
            &Polynomial<Rational>::str, pybind11::const_))
        .def("utf8", overload_cast<const char*>(
            &Polynomial<Rational>::utf8, pybind11::const_))
        .def(pybind11::self *= Rational())
        .def(pybind11::self /= Rational())
        .def(pybind11::self += pybind11::self)
        .def(pybind11::self -= pybind11::self)
        .def(pybind11::self *= pybind11::self)
        .def(pybind11::self /= pybind11::self)
        .def(pybind11::self * regina::Rational())
        .def(regina::Rational() * pybind11::self)
        .def(pybind11::self / regina::Rational())
        .def(pybind11::self + pybind11::self)
        .def(pybind11::self - pybind11::self)
        .def(pybind11::self * pybind11::self)
        .def(pybind11::self / pybind11::self)
        .def(- pybind11::self)
        .def("divisionAlg", overload_cast<const Polynomial<Rational>&>(
            &Polynomial<Rational>::divisionAlg, pybind11::const_))
        .def("gcdWithCoeffs", &Polynomial<Rational>::gcdWithCoeffs<Rational>)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    regina::python::add_global_swap<Polynomial<Rational>>(m);
}

