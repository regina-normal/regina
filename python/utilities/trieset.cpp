
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
#include "utilities/intutils.h"
#include "utilities/trieset.h"
#include "../helpers.h"
#include "../docstrings/utilities/trieset.h"

using regina::Bitmask;
using regina::Bitmask1;
using regina::Bitmask2;
using regina::TrieSet;

#ifdef INT128_AVAILABLE
using Bitmask128 = Bitmask1<regina::IntOfSize<16>::utype>;
using Bitmask256 = Bitmask2<regina::IntOfSize<16>::utype, regina::IntOfSize<16>::utype>;
#else
using Bitmask128 = Bitmask2<uint64_t, uint64_t>;
#endif

void addTrieSet(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(TrieSet)

    auto c = pybind11::class_<TrieSet>(m, "TrieSet", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const TrieSet&>(), rdoc::__copy)
        .def("swap", &TrieSet::swap, rdoc::swap)

        .def("insert", &TrieSet::insert<Bitmask>, rdoc::insert)
        .def("hasSubset", &TrieSet::hasSubset<Bitmask>, rdoc::hasSubset)
        .def("hasExtraSuperset", &TrieSet::hasExtraSuperset<Bitmask>,
            rdoc::hasExtraSuperset)

        // The following set of bitmask types matches the types that we
        // bind in python; see bitmask.cpp for the full list.

        .def("insert", &TrieSet::insert<Bitmask1<uint8_t>>, rdoc::insert)
        .def("hasSubset", &TrieSet::hasSubset<Bitmask1<uint8_t>>,
            rdoc::hasSubset)
        .def("hasExtraSuperset", &TrieSet::hasExtraSuperset<Bitmask1<uint8_t>>,
            rdoc::hasExtraSuperset)

        .def("insert", &TrieSet::insert<Bitmask1<uint16_t>>, rdoc::insert)
        .def("hasSubset", &TrieSet::hasSubset<Bitmask1<uint16_t>>,
            rdoc::hasSubset)
        .def("hasExtraSuperset", &TrieSet::hasExtraSuperset<Bitmask1<uint16_t>>,
            rdoc::hasExtraSuperset)

        .def("insert", &TrieSet::insert<Bitmask1<uint32_t>>, rdoc::insert)
        .def("hasSubset", &TrieSet::hasSubset<Bitmask1<uint32_t>>,
            rdoc::hasSubset)
        .def("hasExtraSuperset", &TrieSet::hasExtraSuperset<Bitmask1<uint32_t>>,
            rdoc::hasExtraSuperset)

        .def("insert", &TrieSet::insert<Bitmask1<uint64_t>>, rdoc::insert)
        .def("hasSubset", &TrieSet::hasSubset<Bitmask1<uint64_t>>,
            rdoc::hasSubset)
        .def("hasExtraSuperset", &TrieSet::hasExtraSuperset<Bitmask1<uint64_t>>,
            rdoc::hasExtraSuperset)

        .def("insert", &TrieSet::insert<Bitmask128>, rdoc::insert)
        .def("hasSubset", &TrieSet::hasSubset<Bitmask128>, rdoc::hasSubset)
        .def("hasExtraSuperset", &TrieSet::hasExtraSuperset<Bitmask128>,
            rdoc::hasExtraSuperset)

        #ifdef INT128_AVAILABLE
        .def("insert", &TrieSet::insert<Bitmask256>, rdoc::insert)
        .def("hasSubset", &TrieSet::hasSubset<Bitmask256>, rdoc::hasSubset)
        .def("hasExtraSuperset", &TrieSet::hasExtraSuperset<Bitmask256>,
            rdoc::hasExtraSuperset)
        #endif
    ;
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_output(c);

    regina::python::add_global_swap<TrieSet>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

