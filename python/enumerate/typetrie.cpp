
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

#include "../pybind11/pybind11.h"
#include "enumerate/typetrie.h"
#include "utilities/exception.h"
#include "../helpers.h"
#include "../docstrings/enumerate/typetrie.h"

using regina::TypeTrie;

template <int nTypes>
void addTypeTrieFor(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(TypeTrie)

    auto c = pybind11::class_<TypeTrie<nTypes>>(m, name, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const TypeTrie<nTypes>&>(), rdoc::__copy)
        .def("swap", &TypeTrie<nTypes>::swap, rdoc::swap)
        .def("clear", &TypeTrie<nTypes>::clear, rdoc::clear)
        .def("insert", [](TypeTrie<nTypes>& t, pybind11::list arg) {
            char* c = new char[arg.size() + 1];
            size_t len = 0;
            try {
                int type;
                for (auto item : arg) {
                    type = item.cast<int>();
                    if (type < 0 || type >= nTypes) {
                        delete[] c;
                        throw regina::InvalidArgument(
                            "Element of type vector is out of range");
                    }
                    c[len++] = static_cast<char>(type);
                }
            } catch (pybind11::cast_error const&) {
                delete[] c;
                throw regina::InvalidArgument(
                    "Element of type vector not convertible to integer");
            }
            c[len] = 0;
            t.insert(c, len);
            delete[] c;
        }, pybind11::arg("entry"), rdoc::insert)
        .def("dominates", [](const TypeTrie<nTypes>& t, pybind11::list arg) {
            char* c = new char[arg.size() + 1];
            size_t len = 0;
            try {
                int type;
                for (auto item : arg) {
                    type = item.cast<int>();
                    if (type < 0 || type >= nTypes) {
                        delete[] c;
                        throw regina::InvalidArgument(
                            "Element of type vector is out of range");
                    }
                    c[len++] = static_cast<char>(type);
                }
            } catch (pybind11::cast_error const&) {
                delete[] c;
                throw regina::InvalidArgument(
                    "Element of type vector not convertible to integer");
            }
            c[len] = 0;
            bool ans = t.dominates(c, len);
            delete[] c;
            return ans;
        }, pybind11::arg("vec"), rdoc::dominates)
    ;
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_output(c);

    regina::python::add_global_swap<TypeTrie<nTypes>>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

void addTypeTrie(pybind11::module_& m) {
    addTypeTrieFor<4>(m, "TypeTrie4");
    addTypeTrieFor<7>(m, "TypeTrie7");
}

