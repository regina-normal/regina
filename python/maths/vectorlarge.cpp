
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
#include "maths/vector.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::VectorLarge;

void addVectorLarge(pybind11::module_& m) {
    auto c = pybind11::class_<VectorLarge>(m, "VectorLarge")
        .def(pybind11::init<size_t>())
        .def(pybind11::init<size_t, const regina::Integer&>())
        .def(pybind11::init<const VectorLarge&>())
        .def(pybind11::init([](const std::vector<regina::LargeInteger> v) {
            return new VectorLarge(v.begin(), v.end());
        }))
        .def("size", &VectorLarge::size)
        .def("__getitem__", [](VectorLarge& v, size_t index) ->
                regina::LargeInteger& {
            return v[index];
        }, pybind11::return_value_policy::reference_internal)
        .def("__setitem__", [](VectorLarge& v, size_t index,
                const regina::Integer& value) {
            v[index] = value;
        })
        .def("__iter__", [](const VectorLarge& list) {
            return pybind11::make_iterator(list);
        }, pybind11::keep_alive<0, 1>()) // iterator keeps vector alive
        .def(pybind11::self += pybind11::self)
        .def(pybind11::self -= pybind11::self)
        .def(pybind11::self *= regina::Integer())
        .def(pybind11::self + pybind11::self)
        .def(pybind11::self - pybind11::self)
        .def(pybind11::self * regina::Integer())
        .def(pybind11::self * pybind11::self)
        .def("negate", &VectorLarge::negate)
        .def("norm", &VectorLarge::norm)
        .def("elementSum", &VectorLarge::elementSum)
        .def("addCopies", &VectorLarge::addCopies)
        .def("subtractCopies", &VectorLarge::subtractCopies)
        .def("isZero", &VectorLarge::isZero)
        // The C-style casts below are to avoid a compile error under gcc7
        // (but not gcc8), where the compiler cannot determine the type of a
        // template member function.
        .def("scaleDown",
            (regina::LargeInteger (VectorLarge::*)())
            &VectorLarge::scaleDown)
        .def_static("unit", &VectorLarge::unit)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(VectorLarge&, VectorLarge&))(regina::swap));

    pybind11::implicitly_convertible<std::vector<int>, VectorLarge>();
    pybind11::implicitly_convertible<std::vector<regina::LargeInteger>,
        VectorLarge>();
}

