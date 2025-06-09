
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

namespace {
    // PythonType should be pybind11::list, pybind11::tuple, or pybind11::args.
    //
    // Only nElements will be extracted from arg.  Typically nElements would be
    // arg.size(); it may be smaller but it must not be larger.
    template <typename PythonType>
    Arrow::DiagramSequence sequenceArg(PythonType arg, size_t nElements) {
        Arrow::DiagramSequence seq(nElements);
        auto it = arg.begin();
        try {
            for (size_t i = 0; i < nElements; ++i)
                seq[i] = pybind11::cast<size_t>(*it++);
        } catch (pybind11::cast_error const&) {
            throw regina::InvalidArgument("Diagram sequence contains an "
                "element that is not convertible to a non-negative integer");
        }
        return seq;
    }
}

void addArrow(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Arrow)

    auto c = pybind11::class_<Arrow>(m, "Arrow", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Arrow&>(), rdoc::__copy)
        .def(pybind11::init([](pybind11::list arg) {
            std::vector<std::pair<Arrow::DiagramSequence, Laurent<Integer>>>
                data;
            for (auto a : arg) {
                try {
                    auto pair = pybind11::cast<std::pair<pybind11::list,
                        Laurent<Integer>>>(a);
                    data.emplace_back(
                        sequenceArg(pair.first, pair.first.size()),
                        std::move(pair.second));
                } catch (pybind11::cast_error const&) {
                    try {
                        auto pair = pybind11::cast<std::pair<pybind11::list,
                            std::pair<long, std::vector<Integer>>>>(a);
                        data.emplace_back(
                            sequenceArg(pair.first, pair.first.size()),
                            Laurent<Integer>(pair.second.first,
                                pair.second.second.begin(),
                                pair.second.second.end()));
                    } catch (pybind11::cast_error const&) {
                        throw regina::InvalidArgument("Each element of the "
                            "given list should be a pair describing a diagram "
                            "sequence and a Laurent polynomial");
                    }
                }
            }
            return Arrow(data.begin(), data.end());
        }), rdoc::__init)
        .def("init", overload_cast<>(&Arrow::init), rdoc::init)
        .def("initDiagram", [](Arrow& arrow, pybind11::args args) {
            arrow.initDiagram(sequenceArg(args, args.size()));
        }, rdoc::initDiagram)
        .def("isZero", &Arrow::isZero, rdoc::isZero)
        .def("set", [](Arrow& arrow, pybind11::args args) {
            size_t n = args.size();
            if (n == 0)
                throw regina::InvalidArgument("set() requires integer "
                    "arguments that represent a diagram sequence, "
                    "followed by a Laurent polynomial argument "
                    "representing the desired value");

            try {
                arrow.set(sequenceArg(args, n - 1),
                    pybind11::cast<Laurent<Integer>>(args[n - 1]));
            } catch (pybind11::cast_error const&) {
                throw regina::InvalidArgument("Final argument to set() "
                    "not convertible to a Laurent polynomial");
            }
        }, rdoc::set)
        .def("swap", &Arrow::swap, rdoc::swap)
        .def("shift", &Arrow::shift, rdoc::shift)
        .def("scaleUp", &Arrow::scaleUp, rdoc::scaleUp)
        .def("scaleDown", &Arrow::scaleDown, rdoc::scaleDown)
        .def("negate", &Arrow::negate, rdoc::negate)
        .def("invertA", &Arrow::invertA, rdoc::invertA)
        .def("multDiagram", &Arrow::multDiagram, rdoc::multDiagram)
        .def("__getitem__", [](const Arrow& arrow, pybind11::list arg) {
            return arrow[sequenceArg(arg, arg.size())];
        }, pybind11::return_value_policy::copy, // to enforce constness
            rdoc::__array)
        .def("__setitem__", [](Arrow& arrow, pybind11::list arg,
                regina::Laurent<Integer> value) {
            arrow.set(sequenceArg(arg, arg.size()), std::move(value));
        }, pybind11::return_value_policy::copy, // to enforce constness
            rdoc::__array)
        .def(pybind11::self == Laurent<Integer>(), rdoc::__eq_2)
        .def(pybind11::self *= Integer(), rdoc::__imul)
        .def(pybind11::self *= Laurent<Integer>(), rdoc::__imul_2)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= pybind11::self, rdoc::__imul_3)
        .def(pybind11::self * Integer(), rdoc_global::__mul)
        .def(Integer() * pybind11::self, rdoc_global::__mul_2)
        .def(pybind11::self * Laurent<Integer>(), rdoc_global::__mul_3)
        .def(Laurent<Integer>() * pybind11::self, rdoc_global::__mul_4)
        .def(pybind11::self + pybind11::self, rdoc_global::__add)
        .def(pybind11::self - pybind11::self, rdoc_global::__sub_2)
        .def(pybind11::self * pybind11::self, rdoc_global::__mul_5)
        .def(- pybind11::self, rdoc_global::__sub)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    regina::python::add_global_swap<Arrow>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

