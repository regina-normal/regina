
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "maths/polynomial.h"
#include "maths/rational.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Polynomial;
using regina::Rational;

namespace {
    const regina::Rational& getItem(const Polynomial<Rational>& p,
            size_t exp) {
        return p[exp];
    }
    void setItem(Polynomial<Rational>& p, size_t exp,
            const regina::Rational& value) {
        p.set(exp, value);
    }

    boost::python::tuple divisionAlg(const Polynomial<Rational>& p,
            const Polynomial<Rational>& divisor) {
        std::auto_ptr<Polynomial<Rational> > q(new Polynomial<Rational>);
        std::auto_ptr<Polynomial<Rational> > r(new Polynomial<Rational>);

        p.divisionAlg(divisor, *q, *r);
        return boost::python::make_tuple(q, r);
    }

    Rational* seqFromList(boost::python::list l) {
        long len = boost::python::len(l);
        Rational* coeffs = new Rational[len];
        for (long i = 0; i < len; ++i) {
            // Accept any type that we know how to convert to a rational.
            extract<regina::Rational&> x_rat(l[i]);
            if (x_rat.check()) {
                coeffs[i] = x_rat();
                continue;
            }
            extract<regina::LargeInteger&> x_large(l[i]);
            if (x_large.check()) {
                coeffs[i] = x_large();
                continue;
            }
            extract<long> x_long(l[i]);
            if (x_long.check()) {
                coeffs[i] = x_long();
                continue;
            }

            // Throw an exception.
            delete[] coeffs;
            x_rat();
        }
        return coeffs;
    }

    Polynomial<Rational>* create_seq(boost::python::list l) {
        Rational* coeffs = seqFromList(l);
        if (coeffs) {
            Polynomial<Rational>* ans = new Polynomial<Rational>(
                coeffs, coeffs + boost::python::len(l));
            delete[] coeffs;
            return ans;
        }
        return 0;
    }

    void init_seq(Polynomial<Rational>& p, boost::python::list l) {
        Rational* coeffs = seqFromList(l);
        if (coeffs) {
            p.init(coeffs, coeffs + boost::python::len(l));
            delete[] coeffs;
        }
    }

    void (Polynomial<Rational>::*init_void)() =
        &Polynomial<Rational>::init;
    void (Polynomial<Rational>::*init_degree)(size_t) =
        &Polynomial<Rational>::init;
    std::string (Polynomial<Rational>::*str_variable)(const char*) const =
        &Polynomial<Rational>::str;
    std::string (Polynomial<Rational>::*utf8_variable)(const char*) const =
        &Polynomial<Rational>::utf8;
}

void addPolynomial() {
    class_<Polynomial<Rational>, std::auto_ptr<Polynomial<Rational> >,
            boost::noncopyable>("Polynomial")
        .def(init<size_t>())
        .def(init<const Polynomial<Rational>&>())
        .def("__init__", make_constructor(create_seq))
        .def("init", init_void)
        .def("init", init_degree)
        .def("init", init_seq)
        .def("degree", &Polynomial<Rational>::degree)
        .def("isZero", &Polynomial<Rational>::isZero)
        .def("isMonic", &Polynomial<Rational>::isMonic)
        .def("leading", &Polynomial<Rational>::leading,
            return_internal_reference<>())
        .def("__getitem__", getItem, return_internal_reference<>())
        .def("__setitem__", setItem)
        .def("set", &Polynomial<Rational>::set)
        .def("swap", &Polynomial<Rational>::swap)
        .def("str", str_variable)
        .def("utf8", utf8_variable)
        .def(self *= Rational())
        .def(self /= Rational())
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self /= self)
        .def("divisionAlg", &divisionAlg)
        .def("gcdWithCoeffs", &Polynomial<Rational>::gcdWithCoeffs<Rational>)
        .def(regina::python::add_output())
        .def(self_ns::repr(self)) // add_output only gives __str__
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NPolynomial") = scope().attr("Polynomial");
}

