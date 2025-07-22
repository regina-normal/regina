
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "utilities/bitmask.h"
#include "utilities/exception.h"
#include "utilities/intutils.h"
#include "../helpers.h"
#include "../docstrings/utilities/bitmask.h"

using regina::Bitmask;
using regina::Bitmask1;
using regina::Bitmask2;

template <class B>
void addBitmaskOpt(pybind11::module_& m, const char* name) {
    // B could be an instance of either Bitmask1 or Bitmask2, but since the
    // Python docs are essentially the same we will just use Bitmask1 here.
    RDOC_SCOPE_BEGIN(Bitmask1)

    auto c = pybind11::class_<B>(m, name, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<size_t>(), rdoc::__init)
        .def(pybind11::init<const B&>(), rdoc::__copy)
        .def("reset", pybind11::overload_cast<>(&B::reset), rdoc::reset)
        .def("reset", pybind11::overload_cast<size_t>(&B::reset), rdoc::reset_2)
        .def("truncate", &B::truncate, rdoc::truncate)
        .def("get", &B::get, rdoc::get)
        .def("set", static_cast<void (B::*)(size_t, bool)>(&B::set), rdoc::set)
        .def("set", [](B& b, pybind11::list indices, bool value) {
            std::vector<size_t> arg;
            for (auto item : indices) {
                try {
                    arg.push_back(item.cast<size_t>());
                } catch (pybind11::cast_error const&) {
                    throw regina::InvalidArgument(
                        "Bitmask index not convertible to integer");
                }
            }
            b.set(arg.begin(), arg.end(), value);
        }, pybind11::arg("indices"), pybind11::arg("value"), rdoc::set_2)
        .def(pybind11::self &= pybind11::self, rdoc::__iand)
        .def(pybind11::self |= pybind11::self, rdoc::__ior)
        .def(pybind11::self ^= pybind11::self, rdoc::__ixor)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def("flip", &B::flip, rdoc::flip)
        .def("lessThan", &B::lessThan, rdoc::lessThan)
        .def("inUnion", &B::inUnion, rdoc::inUnion)
        .def("containsIntn", &B::containsIntn, rdoc::containsIntn)
        .def("bits", &B::bits, rdoc::bits)
        .def("firstBit", &B::firstBit, rdoc::firstBit)
        .def("lastBit", &B::lastBit, rdoc::lastBit)
        .def("atMostOneBit", &B::atMostOneBit, rdoc::atMostOneBit)
        .def_readonly_static("fixedSize", &B::fixedSize)
    ;
    regina::python::add_output_ostream(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    RDOC_SCOPE_END
}

void addBitmaskGeneric(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Bitmask)

    auto c = pybind11::class_<Bitmask>(m, "Bitmask", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<size_t>(), rdoc::__init)
        .def(pybind11::init<const Bitmask&>(), rdoc::__copy)
        .def("get", &Bitmask::get, rdoc::get)
        .def("set", static_cast<void (Bitmask::*)(size_t, bool)>(&Bitmask::set),
            rdoc::set)
        .def("set", [](Bitmask& b, pybind11::list indices, bool value) {
            std::vector<size_t> arg;
            for (auto item : indices) {
                try {
                    arg.push_back(item.cast<size_t>());
                } catch (pybind11::cast_error const&) {
                    throw regina::InvalidArgument(
                        "Bitmask index not convertible to integer");
                }
            }
            b.set(arg.begin(), arg.end(), value);
        }, pybind11::arg("indices"), pybind11::arg("value"), rdoc::set_2)
        .def("reset", pybind11::overload_cast<>(&Bitmask::reset), rdoc::reset)
        .def("reset", pybind11::overload_cast<size_t>(&Bitmask::reset),
            rdoc::reset_2)
        .def("swap", &Bitmask::swap, rdoc::swap)
        .def("truncate", &Bitmask::truncate, rdoc::truncate)
        .def(pybind11::self &= pybind11::self, rdoc::__iand)
        .def(pybind11::self |= pybind11::self, rdoc::__ior)
        .def(pybind11::self ^= pybind11::self, rdoc::__ixor)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def("flip", &Bitmask::flip, rdoc::flip)
        .def("lessThan", &Bitmask::lessThan, rdoc::lessThan)
        .def("inUnion", &Bitmask::inUnion, rdoc::inUnion)
        .def("containsIntn", &Bitmask::containsIntn, rdoc::containsIntn)
        .def("bits", &Bitmask::bits, rdoc::bits)
        .def("firstBit", &Bitmask::firstBit, rdoc::firstBit)
        .def("lastBit", &Bitmask::lastBit, rdoc::lastBit)
        .def("atMostOneBit", &Bitmask::atMostOneBit, rdoc::atMostOneBit)
        .def_readonly_static("fixedSize", &Bitmask::fixedSize)
    ;
    regina::python::add_output_ostream(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    regina::python::add_global_swap<Bitmask>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

void addBitmask(pybind11::module_& m) {
    addBitmaskGeneric(m);
    addBitmaskOpt<Bitmask1<uint8_t>>(m, "Bitmask8");
    addBitmaskOpt<Bitmask1<uint16_t>>(m, "Bitmask16");
    addBitmaskOpt<Bitmask1<uint32_t>>(m, "Bitmask32");
    addBitmaskOpt<Bitmask1<uint64_t>>(m, "Bitmask64");
    #ifdef INT128_AVAILABLE
    addBitmaskOpt<Bitmask1<regina::IntOfSize<16>::utype>>(m, "Bitmask128");
    addBitmaskOpt<Bitmask2<regina::IntOfSize<16>::utype,
        regina::IntOfSize<16>::utype>>(m, "Bitmask256");
    #else
    addBitmaskOpt<Bitmask2<uint64_t, uint64_t>>(m, "Bitmask128");
    #endif
}

