
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
#include "maths/cyclotomic.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Cyclotomic;

void addCyclotomic(pybind11::module_& m) {
    auto c = pybind11::class_<Cyclotomic>(m, "Cyclotomic")
        .def(pybind11::init<>())
        .def(pybind11::init<size_t>())
        .def(pybind11::init<size_t, int>())
        .def(pybind11::init<size_t, const regina::Rational&>())
        .def(pybind11::init<const Cyclotomic&>())
        .def("init", &Cyclotomic::init)
        .def("field", &Cyclotomic::field)
        .def("degree", &Cyclotomic::degree)
        .def("__getitem__", [](const Cyclotomic& c, int exp) {
            return c[exp];
        }, pybind11::return_value_policy::reference_internal)
        .def("__setitem__", [](Cyclotomic& c, int exp,
                const regina::Rational& value) {
            c[exp] = value;
        })
        .def("polynomial", &Cyclotomic::polynomial)
        .def("evaluate", &Cyclotomic::evaluate,
            pybind11::arg("whichRoot") = 1)
        .def("negate", &Cyclotomic::invert)
        .def("invert", &Cyclotomic::invert)
        .def("inverse", &Cyclotomic::inverse)
        .def(pybind11::self *= regina::Rational())
        .def(pybind11::self /= regina::Rational())
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
        .def_static("cyclotomic", [](size_t n) {
            return new regina::Polynomial<regina::Rational>(
                Cyclotomic::cyclotomic(n));
        })
        .def("str", overload_cast<const char*>(
            &Cyclotomic::str, pybind11::const_))
        .def("utf8", overload_cast<const char*>(
            &Cyclotomic::utf8, pybind11::const_))
    ;
    regina::python::add_output(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);

    m.attr("NCyclotomic") = m.attr("Cyclotomic");
}

