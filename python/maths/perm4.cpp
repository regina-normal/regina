
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
    ConstArray<decltype(Perm<4>::S4)>
        Perm4_S4_arr(Perm<4>::S4, 24);
    ConstArray<decltype(Perm<4>::orderedS4)>
        Perm4_orderedS4_arr(Perm<4>::orderedS4, 24);
    ConstArray<decltype(Perm<4>::S3)> Perm4_S3_arr(Perm<4>::S3, 6);
    ConstArray<decltype(Perm<4>::orderedS3)>
        Perm4_orderedS3_arr(Perm<4>::orderedS3, 6);
    ConstArray<decltype(Perm<4>::S2)> Perm4_S2_arr(Perm<4>::S2, 2);

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
    decltype(Perm4_S4_arr)::wrapClass(m, "ConstArray_Perm4_S4");
    decltype(Perm4_orderedS4_arr)::wrapClass(m, "ConstArray_Perm4_orderedS4");
    decltype(Perm4_S3_arr)::wrapClass(m, "ConstArray_Perm4_S3");
    decltype(Perm4_orderedS3_arr)::wrapClass(m, "ConstArray_Perm4_orderedS3");
    decltype(Perm4_S2_arr)::wrapClass(m, "ConstArray_Perm4_S2");

    auto c = pybind11::class_<Perm<4>>(m, "Perm4")
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<int, int, int, int>())
        .def(pybind11::init<int, int, int, int, int, int, int, int>())
        .def(pybind11::init<const std::array<int, 4>&>())
        .def(pybind11::init<const Perm<4>&>())
        .def("permCode1", &Perm<4>::permCode1)
        .def("permCode2", &Perm<4>::permCode2)
        .def("setPermCode1", &Perm<4>::setPermCode1)
        .def("setPermCode2", &Perm<4>::setPermCode2)
        .def_static("fromPermCode1", &Perm<4>::fromPermCode1)
        .def_static("fromPermCode2", &Perm<4>::fromPermCode2)
        .def_static("isPermCode1", &Perm<4>::isPermCode1)
        .def_static("isPermCode2", &Perm<4>::isPermCode2)
        .def("imagePack", &Perm<4>::imagePack)
        .def_static("fromImagePack", &Perm<4>::fromImagePack)
        .def_static("isImagePack", &Perm<4>::isImagePack)
        .def(pybind11::self * pybind11::self)
        .def("inverse", &Perm<4>::inverse)
        .def("pow", &Perm<4>::pow)
        .def("order", &Perm<4>::order)
        .def("reverse", &Perm<4>::reverse)
        .def("sign", &Perm<4>::sign)
        .def("__getitem__", &Perm<4>::operator[])
        .def("pre", &Perm<4>::pre)
        .def("compareWith", &Perm<4>::compareWith)
        .def("isIdentity", &Perm<4>::isIdentity)
        .def("inc", [](Perm<4>& p) {
            return p++;
        })
        .def(pybind11::self < pybind11::self)
        .def_static("rot", &Perm<4>::rot)
        .def_static("rand", (Perm<4> (*)(bool))(&Perm<4>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<4>::trunc)
        .def("trunc2", &Perm<4>::trunc2)
        .def("trunc3", &Perm<4>::trunc3)
        .def("clear", &Perm<4>::clear)
        .def("S4Index", (int (Perm<4>::*)() const) &Perm<4>::S4Index)
        .def("SnIndex", &Perm<4>::SnIndex)
        .def("orderedS4Index", &Perm<4>::orderedS4Index)
        .def("orderedSnIndex", &Perm<4>::orderedS4Index)
        .def("isConjugacyMinimal", &Perm<4>::isConjugacyMinimal)
        .def("pairs", &Perm<4>::pairs)
        .def_static("extend", &Perm<4>::extend<2>)
        .def_static("extend", &Perm<4>::extend<3>)
        .def_readonly_static("codeType", &Perm<4>::codeType)
        .def_readonly_static("imageBits", &Perm<4>::imageBits)
        .def_readonly_static("imageMask", &Perm<4>::imageMask)
        .def_readonly_static("nPerms", &Perm<4>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<4>::nPerms_1)
        .def_readonly_static("S4", &Perm4_S4_arr)
        .def_readonly_static("Sn", &Perm4_S4_arr)
        .def_readonly_static("orderedS4", &Perm4_orderedS4_arr)
        .def_readonly_static("orderedSn", &Perm4_orderedS4_arr)
        .def_readonly_static("S3", &Perm4_S3_arr)
        .def_readonly_static("Sn_1", &Perm4_S3_arr)
        .def_readonly_static("orderedS3", &Perm4_orderedS3_arr)
        .def_readonly_static("S2", &Perm4_S2_arr)
    ;
    Perm4_contract<5>::add_bindings(c);
    regina::python::add_output_basic(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c);
}

