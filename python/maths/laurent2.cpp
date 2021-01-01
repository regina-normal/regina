
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
#include "maths/laurent2.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Laurent2;

void addLaurent2(pybind11::module_& m) {
    auto c = pybind11::class_<Laurent2<regina::Integer>>(m, "Laurent2")
        .def(pybind11::init<>())
        .def(pybind11::init<long, long>())
        .def(pybind11::init<const Laurent2<regina::Integer>&>())
        .def(pybind11::init<const Laurent2<regina::Integer>&, long, long>())
        .def("init", overload_cast<>(
            &Laurent2<regina::Integer>::init))
        .def("init", overload_cast<long, long>(
            &Laurent2<regina::Integer>::init))
        .def("isZero", &Laurent2<regina::Integer>::isZero)
        .def("set", &Laurent2<regina::Integer>::set)
        .def("swap", &Laurent2<regina::Integer>::swap)
        .def("negate", &Laurent2<regina::Integer>::negate)
        .def("str", overload_cast<const char*, const char*>(
            &Laurent2<regina::Integer>::str, pybind11::const_),
            pybind11::arg(), pybind11::arg("varY") = nullptr)
        .def("utf8", overload_cast<const char*, const char*>(
            &Laurent2<regina::Integer>::utf8, pybind11::const_),
            pybind11::arg(), pybind11::arg("varY") = nullptr)
        .def("__getitem__", [](const Laurent2<regina::Integer>& p,
                std::pair<long, long> exponents) {
            return p(exponents.first, exponents.second);
        }, pybind11::return_value_policy::reference_internal)
        .def("__setitem__", [](Laurent2<regina::Integer>& p,
                std::pair<long, long> exponents, const regina::Integer& value) {
            p.set(exponents.first, exponents.second, value);
        })
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

