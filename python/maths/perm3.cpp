
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
    ConstArray<decltype(Perm<3>::S3)>
        Perm3_S3_arr(Perm<3>::S3, 6);
    ConstArray<decltype(Perm<3>::orderedS3)>
        Perm3_orderedS3_arr(Perm<3>::orderedS3, 6);
    ConstArray<decltype(Perm<3>::S2)>
        Perm3_S2_arr(Perm<3>::S2, 2);

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
    decltype(Perm3_S3_arr)::wrapClass(m, "ConstArray_Perm3_S3");
    decltype(Perm3_orderedS3_arr)::wrapClass(m, "ConstArray_Perm3_orderedS3");
    decltype(Perm3_S2_arr)::wrapClass(m, "ConstArray_Perm3_S2");

    auto c = pybind11::class_<Perm<3>>(m, "Perm3")
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<int, int, int>())
        .def(pybind11::init<int, int, int, int, int, int>())
        .def(pybind11::init<const std::array<int, 3>&>())
        .def(pybind11::init<const Perm<3>&>())
        .def("permCode", &Perm<3>::permCode)
        .def("setPermCode", &Perm<3>::setPermCode)
        .def_static("fromPermCode", &Perm<3>::fromPermCode)
        .def_static("isPermCode", &Perm<3>::isPermCode)
        .def(pybind11::self * pybind11::self)
        .def("inverse", &Perm<3>::inverse)
        .def("pow", &Perm<3>::pow)
        .def("order", &Perm<3>::order)
        .def("reverse", &Perm<3>::reverse)
        .def("sign", &Perm<3>::sign)
        .def("__getitem__", &Perm<3>::operator[])
        .def("pre", &Perm<3>::pre)
        .def("preImageOf", &Perm<3>::pre) // deprecated
        .def("compareWith", &Perm<3>::compareWith)
        .def("isIdentity", &Perm<3>::isIdentity)
        .def("inc", [](Perm<3>& p) {
            return p++;
        })
        .def(pybind11::self < pybind11::self)
        .def_static("rot", &Perm<3>::rot)
        // index and atIndex are deprecated, so do not call them directly.
        .def_static("atIndex",
            [](Perm<3>::Index i) { return Perm<3>::orderedSn[i]; })
        .def("index", &Perm<3>::orderedSnIndex)
        .def_static("rand", (Perm<3> (*)(bool))(&Perm<3>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<3>::trunc)
        .def("trunc2", &Perm<3>::trunc2)
        .def("clear", &Perm<3>::clear)
        .def("S3Index", &Perm<3>::S3Index)
        .def("SnIndex", &Perm<3>::SnIndex)
        .def("orderedS3Index", &Perm<3>::orderedS3Index)
        .def("orderedSnIndex", &Perm<3>::orderedS3Index)
        .def("isConjugacyMinimal", &Perm<3>::isConjugacyMinimal)
        .def_static("extend", &Perm<3>::extend<2>)
        .def_property_readonly_static("codeType",
            [](pybind11::object /* self */) { return Perm<3>::codeType; })
        .def_property_readonly_static("nPerms",
            [](pybind11::object /* self */) { return Perm<3>::nPerms; })
        .def_property_readonly_static("nPerms_1",
            [](pybind11::object /* self */) { return Perm<3>::nPerms_1; })
        .def_readonly_static("S3", &Perm3_S3_arr)
        .def_readonly_static("Sn", &Perm3_S3_arr)
        .def_readonly_static("orderedS3", &Perm3_orderedS3_arr)
        .def_readonly_static("orderedSn", &Perm3_orderedS3_arr)
        .def_readonly_static("S2", &Perm3_S2_arr)
        .def_readonly_static("Sn_1", &Perm3_S2_arr)
        .def_readonly_static("code012", &Perm<3>::code012)
        .def_readonly_static("code021", &Perm<3>::code021)
        .def_readonly_static("code102", &Perm<3>::code102)
        .def_readonly_static("code120", &Perm<3>::code120)
        .def_readonly_static("code201", &Perm<3>::code201)
        .def_readonly_static("code210", &Perm<3>::code210)
    ;
    Perm3_contract<4>::add_bindings(c);
    regina::python::add_output_basic(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);
}

