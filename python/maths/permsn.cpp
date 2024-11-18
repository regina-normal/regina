
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "maths/perm.h"
#include "../helpers.h"
#include "../docstrings/maths/permsn.h"

using regina::Perm;
using regina::PermSn;

template <int n>
void addPermSn(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(PermSn)

    auto c = pybind11::class_<PermSn<n>>(m, name, rdoc_scope)
        .def("__getitem__", [](const PermSn<n>& sn, typename Perm<n>::Index i) {
            // Give Python users range checking, for consistency with
            // Regina ≤ 7.3 (before the class PermSn<n> was introduced).
            if (i < 0 || i >= sn.size())
                throw pybind11::index_error("Array index out of range");
            return sn[i];
        })
        .def("size", &PermSn<n>::size, rdoc::size)
        .def("__len__", &PermSn<n>::size, rdoc::size)
        .def("__iter__", [](const PermSn<n>& sn) {
            return sn.begin();
        }, pybind11::keep_alive<0, 1>(), // iterator keeps Sn alive
            rdoc::__iter__)
    ;
    regina::python::add_output_custom(c, [](const PermSn<n>& sn,
            std::ostream& out) {
        out << "[ ";
        // For small n, output the entire array.
        // For large n, do not output everything.
        if constexpr (n <= 4) {
            for (auto p : sn)
                out << p << ' ';
        } else {
            for (typename Perm<n>::Index i = 0; i < 4; ++i)
                out << sn[i] << ' ';
            out << "... " << sn[sn.size() - 1] << ' ';
        }
        out << "]";
    });
    regina::python::disable_eq_operators(c);

    RDOC_SCOPE_INNER_BEGIN(iterator)

    using iterator = typename PermSn<n>::iterator;
    auto it = pybind11::class_<iterator>(c, "iterator", rdoc_inner_scope)
        .def(pybind11::init<>(), rdoc_inner::__default)
        .def(pybind11::init<bool>(), rdoc_inner::__init)
        .def("__next__", [](iterator& it) {
            if (it)
                return *it++;
            else
                throw pybind11::stop_iteration();
        }, rdoc_inner::__next__);
    regina::python::add_eq_operators(it, rdoc_inner::__eq);

    RDOC_SCOPE_INNER_END

    c.attr("const_iterator") = c.attr("iterator");

    RDOC_SCOPE_END
}

void addPermSn(pybind11::module_& m) {
    addPermSn<2>(m, "PermSn2");
    addPermSn<3>(m, "PermSn3");
    addPermSn<4>(m, "PermSn4");
    addPermSn<5>(m, "PermSn5");
    addPermSn<6>(m, "PermSn6");
    addPermSn<7>(m, "PermSn7");
    addPermSn<8>(m, "PermSn8");
    addPermSn<9>(m, "PermSn9");
    addPermSn<10>(m, "PermSn10");
    addPermSn<11>(m, "PermSn11");
    addPermSn<12>(m, "PermSn12");
    addPermSn<13>(m, "PermSn13");
    addPermSn<14>(m, "PermSn14");
    addPermSn<15>(m, "PermSn15");
    addPermSn<16>(m, "PermSn16");
}

