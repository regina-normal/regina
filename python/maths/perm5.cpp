
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
#include "../globalarray.h"
#include "../helpers.h"

using regina::Perm;
using regina::python::GlobalArray;

namespace {
    GlobalArray<Perm<5>> Perm5_S5_arr(Perm<5>::S5, 120);
    GlobalArray<Perm<5>> Perm5_orderedS5_arr(Perm<5>::orderedS5, 120);
    GlobalArray<unsigned> Perm5_invS5_arr(Perm<5>::invS5, 120);
    GlobalArray<Perm<5>> Perm5_S4_arr(Perm<5>::S4, 24);
    GlobalArray<Perm<5>> Perm5_orderedS4_arr(Perm<5>::orderedS4, 24);
    GlobalArray<Perm<5>> Perm5_S3_arr(Perm<5>::S3, 6);
    GlobalArray<Perm<5>> Perm5_orderedS3_arr(Perm<5>::orderedS3, 6);
    GlobalArray<Perm<5>> Perm5_S2_arr(Perm<5>::S2, 2);

    template <int k>
    struct Perm5_contract {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<5>::contract<k>);
            Perm5_contract<k+1>::add_bindings(c);
        }
    };

    template <>
    struct Perm5_contract<16> {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<5>::contract<16>);
        }
    };
}

void addPerm5(pybind11::module_& m) {
    auto c = pybind11::class_<Perm<5>>(m, "Perm5")
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<int, int, int, int, int>())
        .def(pybind11::init<int, int, int, int, int, int, int, int, int, int>())
        .def(pybind11::init<const Perm<5>&>())
        .def("permCode", &Perm<5>::permCode)
        .def("setPermCode", &Perm<5>::setPermCode)
        .def_static("fromPermCode", &Perm<5>::fromPermCode)
        .def_static("isPermCode", &Perm<5>::isPermCode)
        .def(pybind11::self * pybind11::self)
        .def("inverse", &Perm<5>::inverse)
        .def("reverse", &Perm<5>::reverse)
        .def("sign", &Perm<5>::sign)
        .def("__getitem__", &Perm<5>::operator[])
        .def("preImageOf", &Perm<5>::preImageOf)
        .def("compareWith", &Perm<5>::compareWith)
        .def("isIdentity", &Perm<5>::isIdentity)
        .def_static("atIndex", &Perm<5>::atIndex)
        .def("index", &Perm<5>::index)
        .def_static("rand", (Perm<5> (*)(bool))(&Perm<5>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<5>::trunc)
        .def("trunc2", &Perm<5>::trunc2)
        .def("trunc3", &Perm<5>::trunc3)
        .def("trunc4", &Perm<5>::trunc4)
        .def("clear", &Perm<5>::clear)
        .def("S5Index", &Perm<5>::S5Index)
        .def("orderedS5Index", &Perm<5>::orderedS5Index)
        .def("orderedSnIndex", &Perm<5>::orderedS5Index)
        .def_static("extend", &Perm<5>::extend<2>)
        .def_static("extend", &Perm<5>::extend<3>)
        .def_static("extend", &Perm<5>::extend<4>)
        .def_readonly_static("imageBits", &Perm<5>::imageBits)
        .def_readonly_static("nPerms", &Perm<5>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<5>::nPerms_1)
        .def_readonly_static("S5", &Perm5_S5_arr)
        .def_readonly_static("Sn", &Perm5_S5_arr)
        .def_readonly_static("orderedS5", &Perm5_orderedS5_arr)
        .def_readonly_static("orderedSn", &Perm5_orderedS5_arr)
        .def_readonly_static("invS5", &Perm5_invS5_arr)
        .def_readonly_static("invSn", &Perm5_invS5_arr)
        .def_readonly_static("S4", &Perm5_S4_arr)
        .def_readonly_static("Sn_1", &Perm5_S4_arr)
        .def_readonly_static("orderedS4", &Perm5_orderedS4_arr)
        .def_readonly_static("S3", &Perm5_S3_arr)
        .def_readonly_static("orderedS3", &Perm5_orderedS3_arr)
        .def_readonly_static("S2", &Perm5_S2_arr)
    ;
    Perm5_contract<6>::add_bindings(c);
    regina::python::add_output_basic(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);

    m.attr("NPerm5") = m.attr("Perm5");
}

