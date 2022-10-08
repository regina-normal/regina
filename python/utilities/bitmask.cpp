
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
#include "utilities/bitmask.h"
#include "utilities/exception.h"
#include "utilities/intutils.h"
#include "../helpers.h"

using regina::Bitmask;
using regina::Bitmask1;
using regina::Bitmask2;

template <class B>
void addBitmaskOpt(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<B>(m, name)
        .def(pybind11::init<>())
        .def(pybind11::init<size_t>())
        .def(pybind11::init<const B&>())
        .def("reset", pybind11::overload_cast<>(&B::reset))
        .def("reset", pybind11::overload_cast<size_t>(&B::reset))
        .def("truncate", &B::truncate)
        .def("get", &B::get)
        .def("set", (void (B::*)(size_t, bool))(&B::set))
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
        })
        .def(pybind11::self &= pybind11::self)
        .def(pybind11::self |= pybind11::self)
        .def(pybind11::self ^= pybind11::self)
        .def(pybind11::self -= pybind11::self)
        .def("flip", &B::flip)
        .def("lessThan", &B::lessThan)
        .def(pybind11::self <= pybind11::self)
        .def("inUnion", &B::inUnion)
        .def("containsIntn", &B::containsIntn)
        .def("bits", &B::bits)
        .def("firstBit", &B::firstBit)
        .def("lastBit", &B::lastBit)
        .def("atMostOneBit", &B::atMostOneBit)
        .def_readonly_static("fixedSize", &B::fixedSize)
    ;
    regina::python::add_output_ostream(c);
    regina::python::add_eq_operators(c);
}

void addBitmaskGeneric(pybind11::module_& m) {
    auto c = pybind11::class_<Bitmask>(m, "Bitmask")
        .def(pybind11::init<>())
        .def(pybind11::init<size_t>())
        .def(pybind11::init<const Bitmask&>())
        .def("get", &Bitmask::get)
        .def("set", (void (Bitmask::*)(size_t, bool))(&Bitmask::set))
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
        })
        .def("reset", pybind11::overload_cast<>(&Bitmask::reset))
        .def("reset", pybind11::overload_cast<size_t>(&Bitmask::reset))
        .def("swap", &Bitmask::swap)
        .def("truncate", &Bitmask::truncate)
        .def(pybind11::self &= pybind11::self)
        .def(pybind11::self |= pybind11::self)
        .def(pybind11::self ^= pybind11::self)
        .def(pybind11::self -= pybind11::self)
        .def("flip", &Bitmask::flip)
        .def("lessThan", &Bitmask::lessThan)
        .def(pybind11::self <= pybind11::self)
        .def("inUnion", &Bitmask::inUnion)
        .def("containsIntn", &Bitmask::containsIntn)
        .def("bits", &Bitmask::bits)
        .def("firstBit", &Bitmask::firstBit)
        .def("lastBit", &Bitmask::lastBit)
        .def("atMostOneBit", &Bitmask::atMostOneBit)
        .def_readonly_static("fixedSize", &Bitmask::fixedSize)
    ;
    regina::python::add_output_ostream(c);
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(Bitmask&, Bitmask&))(regina::swap));
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

