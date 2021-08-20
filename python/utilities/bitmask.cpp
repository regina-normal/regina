
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
#include "utilities/bitmask.h"
#include "../helpers.h"

using regina::Bitmask;

void addBitmask(pybind11::module_& m) {
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
                    throw std::invalid_argument(
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
        // On some systems we cannot take addresses of inline class constants
        // (e.g., this fails with gcc10 on windows).  We therefore define
        // getter functions instead.
        .def_property_readonly_static("fixedSize", [](pybind11::object) {
            return Bitmask::fixedSize;
        })
    ;
    regina::python::add_output_ostream(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(Bitmask&, Bitmask&))(regina::swap));
}

