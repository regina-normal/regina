
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
    GlobalArray<Perm<3>> Perm3_S3_arr(Perm<3>::S3, 6);
    GlobalArray<Perm<3>> Perm3_orderedS3_arr(Perm<3>::orderedS3, 6);
    GlobalArray<unsigned> Perm3_invS3_arr(Perm<3>::invS3, 6);
    GlobalArray<Perm<3>> Perm3_S2_arr(Perm<3>::S2, 2);

    template <int k>
    struct Perm3_contract {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<3>::contract<k>);
            Perm3_contract<k+1>::add_bindings(c);
        }
    };

    template <>
    struct Perm3_contract<16> {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<3>::contract<16>);
        }
    };
}

void addPerm3(pybind11::module_& m) {
    auto c = pybind11::class_<Perm<3>>(m, "Perm3")
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<int, int, int>())
        .def(pybind11::init<const Perm<3>&>())
        .def("permCode", &Perm<3>::permCode)
        .def("setPermCode", &Perm<3>::setPermCode)
        .def_static("fromPermCode", &Perm<3>::fromPermCode)
        .def_static("isPermCode", &Perm<3>::isPermCode)
        .def(pybind11::self * pybind11::self)
        .def("inverse", &Perm<3>::inverse)
        .def("reverse", &Perm<3>::reverse)
        .def("sign", &Perm<3>::sign)
        .def("__getitem__", &Perm<3>::operator[])
        .def("preImageOf", &Perm<3>::preImageOf)
        .def("compareWith", &Perm<3>::compareWith)
        .def("isIdentity", &Perm<3>::isIdentity)
        .def_static("atIndex", &Perm<3>::atIndex)
        .def("index", &Perm<3>::index)
        .def_static("rand", (Perm<3> (*)(bool))(&Perm<3>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<3>::trunc)
        .def("trunc2", &Perm<3>::trunc2)
        .def("clear", &Perm<3>::clear)
        .def("S3Index", &Perm<3>::S3Index)
        .def("orderedS3Index", &Perm<3>::orderedS3Index)
        .def("orderedSnIndex", &Perm<3>::orderedS3Index)
        .def_static("extend", &Perm<3>::extend<2>)
        .def_readonly_static("nPerms", &Perm<3>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<3>::nPerms_1)
        .def_readonly_static("S3", &Perm3_S3_arr)
        .def_readonly_static("Sn", &Perm3_S3_arr)
        .def_readonly_static("orderedS3", &Perm3_orderedS3_arr)
        .def_readonly_static("orderedSn", &Perm3_orderedS3_arr)
        .def_readonly_static("invS3", &Perm3_invS3_arr)
        .def_readonly_static("invSn", &Perm3_invS3_arr)
        .def_readonly_static("S2", &Perm3_S2_arr)
        .def_readonly_static("Sn_1", &Perm3_S2_arr)
    ;
    Perm3_contract<4>::add_bindings(c);
    regina::python::add_output_basic(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);

    m.attr("NPerm3") = m.attr("Perm3");
}

