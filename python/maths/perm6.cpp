
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

using pybind11::overload_cast;
using regina::Perm;
using regina::python::ConstArray;

namespace {
    ConstArray<decltype(Perm<6>::S6), int>
        Perm6_S6_arr(Perm<6>::S6, 720);
    ConstArray<decltype(Perm<6>::orderedS6), int>
        Perm6_orderedS6_arr(Perm<6>::orderedS6, 720);

    template <int k>
    struct Perm6_contract {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<6>::contract<k>);
            Perm6_contract<k+1>::add_bindings(c);
        }
    };

    template <>
    struct Perm6_contract<16> {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<6>::contract<16>);
        }
    };
}

void addPerm6(pybind11::module_& m) {
    decltype(Perm6_S6_arr)::wrapClass(m, "ConstArray_Perm6_S6");
    decltype(Perm6_orderedS6_arr)::wrapClass(m, "ConstArray_Perm6_orderedS6");

    auto c = pybind11::class_<Perm<6>>(m, "Perm6")
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<int, int, int, int, int, int>())
        .def(pybind11::init<int, int, int, int, int, int,
                            int, int, int, int, int, int>())
        .def(pybind11::init<const std::array<int, 6>&>())
        .def(pybind11::init<const Perm<6>&>())
        .def_static("precompute", &Perm<6>::precompute)
        .def("permCode1", &Perm<6>::permCode1)
        .def("permCode2", &Perm<6>::permCode2)
        .def("setPermCode1", &Perm<6>::setPermCode1)
        .def("setPermCode2", &Perm<6>::setPermCode2)
        .def_static("fromPermCode1", &Perm<6>::fromPermCode1)
        .def_static("fromPermCode2", &Perm<6>::fromPermCode2)
        .def_static("isPermCode1", &Perm<6>::isPermCode1)
        .def_static("isPermCode2", &Perm<6>::isPermCode2)
        .def("imagePack", &Perm<6>::imagePack)
        .def_static("fromImagePack", &Perm<6>::fromImagePack)
        .def_static("isImagePack", &Perm<6>::isImagePack)
        .def(pybind11::self * pybind11::self)
        .def("cachedComp", overload_cast<const Perm<6>&>(
            &Perm<6>::cachedComp, pybind11::const_))
        .def("cachedComp", overload_cast<const Perm<6>&, const Perm<6>&>(
            &Perm<6>::cachedComp, pybind11::const_))
        .def("inverse", &Perm<6>::inverse)
        .def("pow", &Perm<6>::pow)
        .def("cachedPow", &Perm<6>::cachedPow)
        .def("order", &Perm<6>::order)
        .def("reverse", &Perm<6>::reverse)
        .def("sign", &Perm<6>::sign)
        .def("__getitem__", &Perm<6>::operator[])
        .def("pre", &Perm<6>::pre)
        .def("compareWith", &Perm<6>::compareWith)
        .def("isIdentity", &Perm<6>::isIdentity)
        .def("inc", [](Perm<6>& p) {
            return p++;
        })
        .def(pybind11::self < pybind11::self)
        .def_static("rot", &Perm<6>::rot)
        .def_static("rand", (Perm<6> (*)(bool))(&Perm<6>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<6>::trunc)
        .def("clear", &Perm<6>::clear)
        .def("S6Index", (int (Perm<6>::*)() const) &Perm<6>::S6Index)
        .def("SnIndex", &Perm<6>::SnIndex)
        .def("orderedS6Index", &Perm<6>::orderedS6Index)
        .def("orderedSnIndex", &Perm<6>::orderedS6Index)
        .def("isConjugacyMinimal", &Perm<6>::isConjugacyMinimal)
        .def_static("extend", &Perm<6>::extend<2>)
        .def_static("extend", &Perm<6>::extend<3>)
        .def_static("extend", &Perm<6>::extend<4>)
        .def_static("extend", &Perm<6>::extend<5>)
        .def_readonly_static("codeType", &Perm<6>::codeType)
        .def_readonly_static("imageBits", &Perm<6>::imageBits)
        .def_readonly_static("imageMask", &Perm<6>::imageMask)
        .def_readonly_static("nPerms", &Perm<6>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<6>::nPerms_1)
        .def_readonly_static("S6", &Perm6_S6_arr)
        .def_readonly_static("Sn", &Perm6_S6_arr)
        .def_readonly_static("orderedS6", &Perm6_orderedS6_arr)
        .def_readonly_static("orderedSn", &Perm6_orderedS6_arr)
    ;
    Perm6_contract<7>::add_bindings(c);
    regina::python::add_output_basic(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c);
}

