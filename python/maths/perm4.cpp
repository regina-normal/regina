
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
    GlobalArray<Perm<4>> Perm4_S4_arr(Perm<4>::S4, 24);
    GlobalArray<unsigned> Perm4_invS4_arr(Perm<4>::invS4, 24);
    GlobalArray<Perm<4>> Perm4_orderedS4_arr(Perm<4>::orderedS4, 24);
    GlobalArray<Perm<4>> Perm4_S3_arr(Perm<4>::S3, 6);
    GlobalArray<Perm<4>> Perm4_orderedS3_arr(Perm<4>::orderedS3, 6);
    GlobalArray<Perm<4>> Perm4_S2_arr(Perm<4>::S2, 2);

    template <int k>
    struct Perm4_contract {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<4>::contract<k>);
            Perm4_contract<k+1>::add_bindings(c);
        }
    };

    template <>
    struct Perm4_contract<16> {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<4>::contract<16>);
        }
    };
}

void addPerm4(pybind11::module_& m) {
    auto c = pybind11::class_<Perm<4>>(m, "Perm4")
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<int, int, int, int>())
        .def(pybind11::init<int, int, int, int, int, int, int, int>())
        .def(pybind11::init<const Perm<4>&>())
        .def("permCode", &Perm<4>::permCode)
        .def("permCode2", &Perm<4>::permCode2)
        .def("setPermCode", &Perm<4>::setPermCode)
        .def("setPermCode2", &Perm<4>::setPermCode2)
        .def_static("fromPermCode", &Perm<4>::fromPermCode)
        .def_static("fromPermCode2", &Perm<4>::fromPermCode2)
        .def_static("isPermCode", &Perm<4>::isPermCode)
        .def_static("isPermCode2", &Perm<4>::isPermCode2)
        .def(pybind11::self * pybind11::self)
        .def("inverse", &Perm<4>::inverse)
        .def("reverse", &Perm<4>::reverse)
        .def("sign", &Perm<4>::sign)
        .def("__getitem__", &Perm<4>::operator[])
        .def("preImageOf", &Perm<4>::preImageOf)
        .def("compareWith", &Perm<4>::compareWith)
        .def("isIdentity", &Perm<4>::isIdentity)
        .def_static("atIndex", &Perm<4>::atIndex)
        .def("index", &Perm<4>::index)
        .def_static("rand", (Perm<4> (*)(bool))(&Perm<4>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<4>::trunc)
        .def("trunc2", &Perm<4>::trunc2)
        .def("trunc3", &Perm<4>::trunc3)
        .def("clear", &Perm<4>::clear)
        .def("S4Index", (int (Perm<4>::*)() const) &Perm<4>::S4Index)
        .def("orderedS4Index", &Perm<4>::orderedS4Index)
        .def("orderedSnIndex", &Perm<4>::orderedS4Index)
        .def_static("extend", &Perm<4>::extend<2>)
        .def_static("extend", &Perm<4>::extend<3>)
        .def_readonly_static("nPerms", &Perm<4>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<4>::nPerms_1)
        .def_readonly_static("S4", &Perm4_S4_arr)
        .def_readonly_static("Sn", &Perm4_S4_arr)
        .def_readonly_static("invS4", &Perm4_invS4_arr)
        .def_readonly_static("invSn", &Perm4_invS4_arr)
        .def_readonly_static("orderedS4", &Perm4_orderedS4_arr)
        .def_readonly_static("orderedSn", &Perm4_orderedS4_arr)
        .def_readonly_static("S3", &Perm4_S3_arr)
        .def_readonly_static("Sn_1", &Perm4_S3_arr)
        .def_readonly_static("orderedS3", &Perm4_orderedS3_arr)
        .def_readonly_static("S2", &Perm4_S2_arr)
    ;
    Perm4_contract<5>::add_bindings(c);
    regina::python::add_output_basic(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);

    m.attr("NPerm4") = m.attr("Perm4");
}

