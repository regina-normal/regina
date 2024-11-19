
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
using regina::PermOrder;
using regina::PermSn;

template <int n, regina::PermOrder order>
void addPermSn(pybind11::module_& m, const char* name) {
    using Class = PermSn<n, order>;

    RDOC_SCOPE_BEGIN(PermSn)

    auto c = pybind11::class_<Class>(m, name, rdoc_scope)
        .def("__getitem__", [](const Class& sn, typename Perm<n>::Index i) {
            // Give Python users range checking, for consistency with
            // Regina ≤ 7.3 (before the class PermSn<n> was introduced).
            if (i < 0 || i >= sn.size())
                throw pybind11::index_error("Array index out of range");
            return sn[i];
        })
        .def("size", &Class::size, rdoc::size)
        .def("__len__", &Class::size, rdoc::size)
        .def("__iter__", [](const Class& sn) {
            return sn.begin();
        }, pybind11::keep_alive<0, 1>(), // iterator keeps sn alive
            rdoc::__iter__)
    ;
    regina::python::add_output_custom(c, [](const Class& sn,
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

    using iterator = typename Class::iterator;
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
    addPermSn<2, PermOrder::Sign>(m, "PermSn2_Sign");
    addPermSn<3, PermOrder::Sign>(m, "PermSn3_Sign");
    addPermSn<4, PermOrder::Sign>(m, "PermSn4_Sign");
    addPermSn<5, PermOrder::Sign>(m, "PermSn5_Sign");
    addPermSn<6, PermOrder::Sign>(m, "PermSn6_Sign");
    addPermSn<7, PermOrder::Sign>(m, "PermSn7_Sign");
    addPermSn<8, PermOrder::Sign>(m, "PermSn8_Sign");
    addPermSn<9, PermOrder::Sign>(m, "PermSn9_Sign");
    addPermSn<10, PermOrder::Sign>(m, "PermSn10_Sign");
    addPermSn<11, PermOrder::Sign>(m, "PermSn11_Sign");
    addPermSn<12, PermOrder::Sign>(m, "PermSn12_Sign");
    addPermSn<13, PermOrder::Sign>(m, "PermSn13_Sign");
    addPermSn<14, PermOrder::Sign>(m, "PermSn14_Sign");
    addPermSn<15, PermOrder::Sign>(m, "PermSn15_Sign");
    addPermSn<16, PermOrder::Sign>(m, "PermSn16_Sign");

    addPermSn<2, PermOrder::Lex>(m, "PermSn2_Lex");
    addPermSn<3, PermOrder::Lex>(m, "PermSn3_Lex");
    addPermSn<4, PermOrder::Lex>(m, "PermSn4_Lex");
    addPermSn<5, PermOrder::Lex>(m, "PermSn5_Lex");
    addPermSn<6, PermOrder::Lex>(m, "PermSn6_Lex");
    addPermSn<7, PermOrder::Lex>(m, "PermSn7_Lex");
    addPermSn<8, PermOrder::Lex>(m, "PermSn8_Lex");
    addPermSn<9, PermOrder::Lex>(m, "PermSn9_Lex");
    addPermSn<10, PermOrder::Lex>(m, "PermSn10_Lex");
    addPermSn<11, PermOrder::Lex>(m, "PermSn11_Lex");
    addPermSn<12, PermOrder::Lex>(m, "PermSn12_Lex");
    addPermSn<13, PermOrder::Lex>(m, "PermSn13_Lex");
    addPermSn<14, PermOrder::Lex>(m, "PermSn14_Lex");
    addPermSn<15, PermOrder::Lex>(m, "PermSn15_Lex");
    addPermSn<16, PermOrder::Lex>(m, "PermSn16_Lex");
}

