
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
#include "maths/perm.h"
#include "../helpers.h"

using regina::Perm;

namespace {
    template <int n, int k>
    struct Perm_extend {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("extend", &Perm<n>::template extend<k>);
            Perm_extend<n, k-1>::add_bindings(c);
        }
    };

    template <int n>
    struct Perm_extend<n, 2> {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("extend", &Perm<n>::template extend<2>);
        }
    };

    template <int n, int k>
    struct Perm_contract {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<n>::template contract<k>);
            Perm_contract<n, k+1>::add_bindings(c);
        }
    };

    template <int n>
    struct Perm_contract<n, 16> {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<n>::template contract<16>);
        }
    };

    template <int n>
    struct Perm_contract<n, 17> {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            // Only called for Perm16, which has no contract() methods at all.
        }
    };
}

template <int n>
void addPerm(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<Perm<n>>(m, name)
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<const Perm<n>&>())
        .def(pybind11::init([](pybind11::list l) {
            if (l.size() != n)
                throw pybind11::index_error(
                    "Initialisation list has the wrong length");
            int image[n];
            try {
                for (long i = 0; i < n; i++)
                    image[i] = l[i].cast<int>();
            } catch (pybind11::cast_error const &) {
                throw std::invalid_argument(
                    "List element not convertible to int");
            }
            return new Perm<n>(image);
        }))
        .def("permCode", &Perm<n>::permCode)
        .def("setPermCode", &Perm<n>::setPermCode)
        .def_static("fromPermCode", &Perm<n>::fromPermCode)
        .def_static("isPermCode", &Perm<n>::isPermCode)
        .def(pybind11::self * pybind11::self)
        .def("inverse", &Perm<n>::inverse)
        .def("reverse", &Perm<n>::reverse)
        .def("sign", &Perm<n>::sign)
        .def("__getitem__", &Perm<n>::operator[])
        .def("preImageOf", &Perm<n>::preImageOf)
        .def("compareWith", &Perm<n>::compareWith)
        .def("isIdentity", &Perm<n>::isIdentity)
        .def_static("atIndex", &Perm<n>::atIndex)
        .def("index", &Perm<n>::index)
        .def_static("rand", (Perm<n> (*)(bool))(&Perm<n>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<n>::trunc)
        .def("clear", &Perm<n>::clear)
        .def_readonly_static("nPerms", &Perm<n>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<n>::nPerms_1)
        .def_readonly_static("imageBits", &Perm<n>::imageBits)
    ;
    Perm_extend<n, n-1>::add_bindings(c);
    Perm_contract<n, n+1>::add_bindings(c);
    regina::python::add_output_basic(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);
}

void addPerm(pybind11::module_& m) {
    m.def("digit", regina::digit);
    m.def("factorial", regina::factorial);

    addPerm<6>(m, "Perm6");
    addPerm<7>(m, "Perm7");
    addPerm<8>(m, "Perm8");
    addPerm<9>(m, "Perm9");
    addPerm<10>(m, "Perm10");
    addPerm<11>(m, "Perm11");
    addPerm<12>(m, "Perm12");
    addPerm<13>(m, "Perm13");
    addPerm<14>(m, "Perm14");
    addPerm<15>(m, "Perm15");
    addPerm<16>(m, "Perm16");

    m.attr("NPerm6") = m.attr("Perm6");
    m.attr("NPerm7") = m.attr("Perm7");
    m.attr("NPerm8") = m.attr("Perm8");
    m.attr("NPerm9") = m.attr("Perm9");
    m.attr("NPerm10") = m.attr("Perm10");
    m.attr("NPerm11") = m.attr("Perm11");
    m.attr("NPerm12") = m.attr("Perm12");
    m.attr("NPerm13") = m.attr("Perm13");
    m.attr("NPerm14") = m.attr("Perm14");
    m.attr("NPerm15") = m.attr("Perm15");
    m.attr("NPerm16") = m.attr("Perm16");
}

