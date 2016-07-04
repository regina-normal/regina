
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
#include "maths/ninteger.h"
#include "maths/nlaurent.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NLaurent;
using regina::NInteger;

namespace {
    const regina::NInteger& getItem(const NLaurent<NInteger>& p,
            long exp) {
        return p[exp];
    }
    void setItem(NLaurent<NInteger>& p, long exp,
            const regina::NInteger& value) {
        p.set(exp, value);
    }

    NInteger* seqFromList(boost::python::list l) {
        long len = boost::python::len(l);
        NInteger* coeffs = new NInteger[len];
        for (long i = 0; i < len; ++i) {
            // Accept any type that we know how to convert to a rational.
            extract<regina::NInteger&> x_rat(l[i]);
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

    NLaurent<NInteger>* create_seq(long minExp, boost::python::list l) {
        NInteger* coeffs = seqFromList(l);
        if (coeffs) {
            NLaurent<NInteger>* ans = new NLaurent<NInteger>(
                minExp, coeffs, coeffs + boost::python::len(l));
            delete[] coeffs;
            return ans;
        }
        return 0;
    }

    void init_seq(NLaurent<NInteger>& p, long minExp, boost::python::list l) {
        NInteger* coeffs = seqFromList(l);
        if (coeffs) {
            p.init(minExp, coeffs, coeffs + boost::python::len(l));
            delete[] coeffs;
        }
    }

    void (NLaurent<NInteger>::*init_void)() =
        &NLaurent<NInteger>::init;
    void (NLaurent<NInteger>::*init_degree)(long) =
        &NLaurent<NInteger>::init;
}

void addNLaurent() {
    scope s = class_<NLaurent<NInteger>,
            std::auto_ptr<NLaurent<NInteger> >,
            boost::noncopyable>("NLaurent")
        .def(init<long>())
        .def(init<const NLaurent<NInteger>&>())
        .def("__init__", make_constructor(create_seq))
        .def("init", init_void)
        .def("init", init_degree)
        .def("init", init_seq)
        .def("minExp", &NLaurent<NInteger>::minExp)
        .def("maxExp", &NLaurent<NInteger>::maxExp)
        .def("isZero", &NLaurent<NInteger>::isZero)
        .def("__getitem__", getItem, return_internal_reference<>())
        .def("__setitem__", setItem)
        .def("set", &NLaurent<NInteger>::set)
        .def("swap", &NLaurent<NInteger>::swap)
        .def("shift", &NLaurent<NInteger>::shift)
        .def("negate", &NLaurent<NInteger>::negate)
        .def(self *= NInteger())
        .def(self /= NInteger())
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self_ns::str(self))
        .def(self_ns::repr(self))
        .def(regina::python::add_eq_operators())
    ;
}

