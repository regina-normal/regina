
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
#include "maths/polynomial.h"
#include "maths/rational.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Polynomial;
using regina::Rational;

namespace {
    // If this function returns, it guarantees to return non-null.
    Rational* seqFromList(pybind11::list l) {
        size_t len = l.size();
        Rational* coeffs = new Rational[len];
        if (! coeffs)
            throw std::bad_alloc();
        for (size_t i = 0; i < len; ++i) {
            // Accept any type that we know how to convert to a rational.
            // This includes (at least) regina::Rational, regina's large
            // integer classes, and python integers (both int and long).
            try {
                coeffs[i] = l[i].cast<regina::Rational>();
                continue;
            } catch (pybind11::cast_error const &) {
                delete[] coeffs;
                throw std::invalid_argument(
                    "List element not convertible to Rational");
            }
        }
        return coeffs;
    }
}

void addPolynomial(pybind11::module_& m) {
    auto c = pybind11::class_<Polynomial<Rational>>(m, "Polynomial")
        .def(pybind11::init<>())
        .def(pybind11::init<size_t>())
        .def(pybind11::init<const Polynomial<Rational>&>())
        .def(pybind11::init([](pybind11::list l) {
            Rational* coeffs = seqFromList(l);
            Polynomial<Rational>* ans = new Polynomial<Rational>(
                coeffs, coeffs + l.size());
            delete[] coeffs;
            return ans;
        }))
        // overload_cast has trouble with templated vs non-templated overloads.
        // Just cast directly.
        .def("init", (void (Polynomial<Rational>::*)())
            &Polynomial<Rational>::init)
        .def("init", (void (Polynomial<Rational>::*)(size_t))
            &Polynomial<Rational>::init)
        .def("init", [](Polynomial<Rational>& p, pybind11::list l) {
            Rational* coeffs = seqFromList(l);
            p.init(coeffs, coeffs + l.size());
            delete[] coeffs;
        })
        .def("degree", &Polynomial<Rational>::degree)
        .def("isZero", &Polynomial<Rational>::isZero)
        .def("isMonic", &Polynomial<Rational>::isMonic)
        .def("leading", &Polynomial<Rational>::leading,
            pybind11::return_value_policy::reference_internal)
        .def("__getitem__", [](const Polynomial<Rational>& p, size_t exp) {
            return p[exp];
        }, pybind11::return_value_policy::reference_internal)
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
        .def("divisionAlg", [](const Polynomial<Rational>& p,
                const Polynomial<Rational>& divisor) {
            std::unique_ptr<Polynomial<Rational>> q(new Polynomial<Rational>);
            std::unique_ptr<Polynomial<Rational>> r(new Polynomial<Rational>);

            p.divisionAlg(divisor, *q, *r);
            return std::make_pair(std::move(q), std::move(r));
        })
        .def("gcdWithCoeffs", &Polynomial<Rational>::gcdWithCoeffs<Rational>)
    ;
    regina::python::add_output(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);

    m.attr("NPolynomial") = m.attr("Polynomial");
}

