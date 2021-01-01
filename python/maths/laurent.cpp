
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
#include "maths/integer.h"
#include "maths/laurent.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Laurent;

namespace {
    // If this function returns, it guarantees to return non-null.
    regina::Integer* seqFromList(pybind11::list l) {
        size_t len = l.size();
        regina::Integer* coeffs = new regina::Integer[len];
        if (! coeffs)
            throw std::bad_alloc();
        for (size_t i = 0; i < len; ++i) {
            // Accept any type that we know how to convert to regina::Integer.
            // This includes (at least) Regina's large integer classes,
            // python integers (both int and long), and strings.
            try {
                coeffs[i] = l[i].cast<regina::Integer>();
                continue;
            } catch (pybind11::cast_error const &) {
                delete[] coeffs;
                throw std::invalid_argument(
                    "List element not convertible to Integer");
            }
        }
        return coeffs;
    }
}

void addLaurent(pybind11::module_& m) {
    auto c = pybind11::class_<Laurent<regina::Integer>>(m, "Laurent")
        .def(pybind11::init<>())
        .def(pybind11::init<long>())
        .def(pybind11::init<const Laurent<regina::Integer>&>())
        .def(pybind11::init([](long minExp, pybind11::list l) {
            regina::Integer* coeffs = seqFromList(l);
            Laurent<regina::Integer>* ans = new Laurent<regina::Integer>(
                minExp, coeffs, coeffs + l.size());
            delete[] coeffs;
            return ans;
        }))
        // overload_cast has trouble with templated vs non-templated overloads.
        // Just cast directly.
        .def("init", (void (Laurent<regina::Integer>::*)())
            &Laurent<regina::Integer>::init)
        .def("init", (void (Laurent<regina::Integer>::*)(long))
            &Laurent<regina::Integer>::init)
        .def("init", [](Laurent<regina::Integer>& p, long minExp,
                pybind11::list l) {
            regina::Integer* coeffs = seqFromList(l);
            p.init(minExp, coeffs, coeffs + l.size());
            delete[] coeffs;
        })
        .def("minExp", &Laurent<regina::Integer>::minExp)
        .def("maxExp", &Laurent<regina::Integer>::maxExp)
        .def("isZero", &Laurent<regina::Integer>::isZero)
        .def("__getitem__", [](const Laurent<regina::Integer>& p, long exp) {
            return p[exp];
        }, pybind11::return_value_policy::reference_internal)
        .def("__setitem__", [](Laurent<regina::Integer>& p, long exp,
                const regina::Integer& value) {
            p.set(exp, value);
        })
        .def("set", &Laurent<regina::Integer>::set)
        .def("swap", &Laurent<regina::Integer>::swap)
        .def("shift", &Laurent<regina::Integer>::shift)
        .def("scaleUp", &Laurent<regina::Integer>::scaleUp)
        .def("scaleDown", &Laurent<regina::Integer>::scaleDown)
        .def("negate", &Laurent<regina::Integer>::negate)
        .def("str", overload_cast<const char*>(
            &Laurent<regina::Integer>::str, pybind11::const_))
        .def("utf8", overload_cast<const char*>(
            &Laurent<regina::Integer>::utf8, pybind11::const_))
        .def(pybind11::self *= regina::Integer())
        .def(pybind11::self /= regina::Integer())
        .def(pybind11::self += pybind11::self)
        .def(pybind11::self -= pybind11::self)
        .def(pybind11::self *= pybind11::self)
        .def(pybind11::self * regina::Integer())
        .def(regina::Integer() * pybind11::self)
        .def(pybind11::self / regina::Integer())
        .def(pybind11::self + pybind11::self)
        .def(pybind11::self - pybind11::self)
        .def(pybind11::self * pybind11::self)
        .def(- pybind11::self)
    ;
    regina::python::add_output(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);
}

