
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
#include "../pybind11/stl.h"
#include "maths/perm.h"
#include "../constarray.h"
#include "../helpers.h"

using regina::Perm;
using regina::python::ConstArray;

namespace {
    // Note that S2 and S1 are the same C++ type.
    ConstArray<decltype(Perm<2>::S2)> Perm2_S2_arr(Perm<2>::S2, 2);
    ConstArray<decltype(Perm<2>::S2)> Perm2_S1_arr(Perm<2>::S1, 1);

    template <int k>
    struct Perm2_contract {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<2>::contract<k>);
            Perm2_contract<k+1>::add_bindings(c);
        }
    };

    template <>
    struct Perm2_contract<16> {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<2>::contract<16>);
        }
    };
}

void addPerm2(pybind11::module_& m) {
    decltype(Perm2_S2_arr)::wrapClass(m, "ConstArray_Perm2_S2");

    auto c = pybind11::class_<Perm<2>>(m, "Perm2")
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<const std::array<int, 2>&>())
        .def(pybind11::init<const Perm<2>&>())
        .def("permCode", &Perm<2>::permCode)
        .def("setPermCode", &Perm<2>::setPermCode)
        .def_static("fromPermCode", &Perm<2>::fromPermCode)
        .def_static("isPermCode", &Perm<2>::isPermCode)
        .def(pybind11::self * pybind11::self)
        .def("inverse", &Perm<2>::inverse)
        .def("pow", &Perm<2>::pow)
        .def("order", &Perm<2>::order)
        .def("reverse", &Perm<2>::reverse)
        .def("sign", &Perm<2>::sign)
        .def("__getitem__", &Perm<2>::operator[])
        .def("pre", &Perm<2>::pre)
        .def("compareWith", &Perm<2>::compareWith)
        .def("isIdentity", &Perm<2>::isIdentity)
        .def("inc", [](Perm<2>& p) {
            return p++;
        })
        .def(pybind11::self < pybind11::self)
        .def_static("rot", &Perm<2>::rot)
        .def_static("rand", (Perm<2> (*)(bool))(&Perm<2>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<2>::trunc)
        .def("clear", &Perm<2>::clear)
        .def("S2Index", &Perm<2>::S2Index)
        .def("SnIndex", &Perm<2>::SnIndex)
        .def("orderedS2Index", &Perm<2>::orderedS2Index)
        .def("orderedSnIndex", &Perm<2>::orderedS2Index)
        .def("isConjugacyMinimal", &Perm<2>::isConjugacyMinimal)
        .def_readonly_static("codeType", &Perm<2>::codeType)
        .def_readonly_static("nPerms", &Perm<2>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<2>::nPerms_1)
        .def_readonly_static("S2", &Perm2_S2_arr)
        .def_readonly_static("Sn", &Perm2_S2_arr)
        .def_readonly_static("orderedS2", &Perm2_S2_arr)
        .def_readonly_static("orderedSn", &Perm2_S2_arr)
        .def_readonly_static("S1", &Perm2_S1_arr)
        .def_readonly_static("Sn_1", &Perm2_S1_arr)
    ;
    Perm2_contract<3>::add_bindings(c);
    regina::python::add_output_basic(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c);
}

