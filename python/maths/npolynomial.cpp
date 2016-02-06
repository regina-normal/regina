
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
#include "maths/npolynomial.h"
#include "maths/nrational.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NPolynomial;
using regina::NRational;

namespace {
    const regina::NRational& getItem(const NPolynomial<NRational>& p,
            size_t exp) {
        return p[exp];
    }
    void setItem(NPolynomial<NRational>& p, size_t exp,
            const regina::NRational& value) {
        p.set(exp, value);
    }

    boost::python::tuple divisionAlg(const NPolynomial<NRational>& p,
            const NPolynomial<NRational>& divisor) {
        std::auto_ptr<NPolynomial<NRational> > q(new NPolynomial<NRational>);
        std::auto_ptr<NPolynomial<NRational> > r(new NPolynomial<NRational>);

        p.divisionAlg(divisor, *q, *r);
        return boost::python::make_tuple(q, r);
    }

    NRational* seqFromList(boost::python::list l) {
        long len = boost::python::len(l);
        NRational* coeffs = new NRational[len];
        for (long i = 0; i < len; ++i) {
            // Accept any type that we know how to convert to a rational.
            extract<regina::NRational&> x_rat(l[i]);
            if (x_rat.check()) {
                coeffs[i] = x_rat();
                continue;
            }
            extract<regina::NLargeInteger&> x_large(l[i]);
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

    NPolynomial<NRational>* create_seq(boost::python::list l) {
        NRational* coeffs = seqFromList(l);
        if (coeffs) {
            NPolynomial<NRational>* ans = new NPolynomial<NRational>(
                coeffs, coeffs + boost::python::len(l));
            delete[] coeffs;
            return ans;
        }
        return 0;
    }

    void init_seq(NPolynomial<NRational>& p, boost::python::list l) {
        NRational* coeffs = seqFromList(l);
        if (coeffs) {
            p.init(coeffs, coeffs + boost::python::len(l));
            delete[] coeffs;
        }
    }

    void (NPolynomial<NRational>::*init_void)() =
        &NPolynomial<NRational>::init;
    void (NPolynomial<NRational>::*init_degree)(size_t) =
        &NPolynomial<NRational>::init;
}

void addNPolynomial() {
    scope s = class_<NPolynomial<NRational>,
            std::auto_ptr<NPolynomial<NRational> >,
            boost::noncopyable>("NPolynomial")
        .def(init<size_t>())
        .def(init<const NPolynomial<NRational>&>())
        .def("__init__", make_constructor(create_seq))
        .def("init", init_void)
        .def("init", init_degree)
        .def("init", init_seq)
        .def("degree", &NPolynomial<NRational>::degree)
        .def("isZero", &NPolynomial<NRational>::isZero)
        .def("isMonic", &NPolynomial<NRational>::isMonic)
        .def("leading", &NPolynomial<NRational>::leading,
            return_internal_reference<>())
        .def("__getitem__", getItem, return_internal_reference<>())
        .def("__setitem__", setItem)
        .def("set", &NPolynomial<NRational>::set)
        .def("swap", &NPolynomial<NRational>::swap)
        .def(self *= NRational())
        .def(self /= NRational())
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self /= self)
        .def("divisionAlg", &divisionAlg)
        .def("gcdWithCoeffs", &NPolynomial<NRational>::gcdWithCoeffs<NRational>)
        .def(self_ns::str(self))
        .def(self_ns::repr(self))
        .def(regina::python::add_eq_operators())
    ;
}

