
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
#include "maths/arrow.h"
#include "../helpers.h"
#include "../docstrings/maths/arrow.h"

using pybind11::overload_cast;
using regina::Arrow;
using regina::Integer;
using regina::Laurent;

void addArrow(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Arrow)

    auto c = pybind11::class_<Arrow>(m, "Arrow", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Arrow&>(), rdoc::__copy)
        .def("init", overload_cast<>(&Arrow::init), rdoc::init)
        .def("initDiagram", [](Arrow& arrow, pybind11::args args) {
            size_t n = args.size();

            Arrow::DiagramSequence seq(n);
            auto it = args.begin();
            try {
                for (size_t i = 0; i < n; ++i)
                    seq[i] = pybind11::cast<size_t>(*it++);
            } catch (pybind11::cast_error const&) {
                throw regina::InvalidArgument("Element of the diagram "
                    "sequence given to initDiagram() not convertible to an "
                    "integer");
            }
            arrow.initDiagram(std::move(seq));
        }, rdoc::initDiagram)
        .def("isZero", &Arrow::isZero, rdoc::isZero)
        .def("set", [](Arrow& arrow, pybind11::args args) {
            size_t n = args.size();
            if (n == 0)
                throw regina::InvalidArgument("set() requires integer "
                    "arguments that represent a diagram sequence, "
                    "followed by a Laurent polynomial argument "
                    "representing the desired value");

            Arrow::DiagramSequence seq(n - 1);
            auto it = args.begin();
            try {
                for (size_t i = 0; i < n - 1; ++i)
                    seq[i] = pybind11::cast<size_t>(*it++);
            } catch (pybind11::cast_error const&) {
                throw regina::InvalidArgument("Element of the diagram "
                    "sequence given to set() not convertible to an "
                    "integer");
            }
            try {
                arrow.set(std::move(seq),
                    pybind11::cast<Laurent<Integer>>(*it));
            } catch (pybind11::cast_error const&) {
                throw regina::InvalidArgument("Value given to set() "
                    "not convertible to a Laurent polynomial");
            }
        }, rdoc::set)
        .def("swap", &Arrow::swap, rdoc::swap)
        .def("shift", &Arrow::shift, rdoc::shift)
        .def("scaleUp", &Arrow::scaleUp, rdoc::scaleUp)
        .def("scaleDown", &Arrow::scaleDown, rdoc::scaleDown)
        .def("negate", &Arrow::negate, rdoc::negate)
        .def("invertA", &Arrow::invertA, rdoc::invertA)
        .def("__getitem__", [](const Arrow& arrow, pybind11::list arg) {
            size_t n = arg.size();

            Arrow::DiagramSequence seq(n);
            auto it = arg.begin();
            try {
                for (size_t i = 0; i < n; ++i)
                    seq[i] = pybind11::cast<size_t>(*it++);
            } catch (pybind11::cast_error const&) {
                throw regina::InvalidArgument("Element of the diagram "
                    "sequence given to __getitem__ not convertible to an "
                    "integer");
            }
            return arrow[seq];
        }, pybind11::return_value_policy::copy, // to enforce constness
            rdoc::__array)
        .def("__setitem__", [](Arrow& arrow, pybind11::list arg,
                regina::Laurent<Integer> value) {
            size_t n = arg.size();

            Arrow::DiagramSequence seq(n);
            auto it = arg.begin();
            try {
                for (size_t i = 0; i < n; ++i)
                    seq[i] = pybind11::cast<size_t>(*it++);
            } catch (pybind11::cast_error const&) {
                throw regina::InvalidArgument("Element of the diagram "
                    "sequence given to __getitem__ not convertible to an "
                    "integer");
            }

            arrow.set(seq, std::move(value));
        }, pybind11::return_value_policy::copy, // to enforce constness
            rdoc::__array)
        .def(pybind11::self *= Integer(), rdoc::__imul)
        .def(pybind11::self *= Laurent<Integer>(), rdoc::__imul_2)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self * Integer(), rdoc_global::__mul)
        .def(Integer() * pybind11::self, rdoc_global::__mul_2)
        .def(pybind11::self * Laurent<Integer>(), rdoc_global::__mul_3)
        .def(Laurent<Integer>() * pybind11::self, rdoc_global::__mul_4)
        .def(pybind11::self + pybind11::self, rdoc_global::__add)
        .def(pybind11::self - pybind11::self, rdoc_global::__sub_2)
        .def(- pybind11::self, rdoc_global::__sub)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    regina::python::add_global_swap<Arrow>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

