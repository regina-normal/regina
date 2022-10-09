
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
#include "../pybind11/stl.h"
#include "maths/perm.h"
#include "utilities/typeutils.h"
#include "../constarray.h"
#include "../helpers.h"

using regina::Perm;
using regina::python::ConstArray;

namespace {
    template <int n>
    ConstArray<decltype(Perm<n>::Sn), typename Perm<n>::Index> Perm_Sn_arr(
        Perm<n>::Sn, Perm<n>::nPerms);

    template <int n>
    ConstArray<decltype(Perm<n>::orderedSn), typename Perm<n>::Index>
        Perm_orderedSn_arr(Perm<n>::orderedSn, Perm<n>::nPerms);
}

template <int n>
void addPerm(pybind11::module_& m, const char* name) {
    decltype(Perm_Sn_arr<n>)::wrapClass(m,
        (std::string("ConstArray_") + name + "_Sn").c_str());
    decltype(Perm_orderedSn_arr<n>)::wrapClass(m,
        (std::string("ConstArray_") + name + "_orderedSn").c_str());;

    auto c = pybind11::class_<Perm<n>>(m, name)
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<const Perm<n>&>())
        .def(pybind11::init<const std::array<int, n>&>())
        .def("permCode", &Perm<n>::permCode)
        .def("setPermCode", &Perm<n>::setPermCode)
        .def_static("fromPermCode", &Perm<n>::fromPermCode)
        .def_static("isPermCode", &Perm<n>::isPermCode)
        .def("imagePack", &Perm<n>::imagePack)
        .def_static("fromImagePack", &Perm<n>::fromImagePack)
        .def_static("isImagePack", &Perm<n>::isImagePack)
        .def(pybind11::self * pybind11::self)
        .def("inverse", &Perm<n>::inverse)
        .def("reverse", &Perm<n>::reverse)
        .def("sign", &Perm<n>::sign)
        .def("__getitem__", &Perm<n>::operator[])
        .def("pre", &Perm<n>::pre)
        .def("compareWith", &Perm<n>::compareWith)
        .def("isIdentity", &Perm<n>::isIdentity)
        .def_static("rot", &Perm<n>::rot)
        .def_static("rand", static_cast<Perm<n>(&)(bool)>(Perm<n>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<n>::trunc)
        .def("clear", &Perm<n>::clear)
        .def("SnIndex", &Perm<n>::SnIndex)
        .def("orderedSnIndex", &Perm<n>::orderedSnIndex)
        .def_readonly_static("codeType", &Perm<n>::codeType)
        .def_readonly_static("imageBits", &Perm<n>::imageBits)
        .def_readonly_static("imageMask", &Perm<n>::imageMask)
        .def_readonly_static("nPerms", &Perm<n>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<n>::nPerms_1)
        .def_readonly_static("Sn", &Perm_Sn_arr<n>)
        .def_readonly_static("orderedSn", &Perm_orderedSn_arr<n>)
    ;
    regina::for_constexpr<2, n>([&c](auto i) {
        c.def_static("extend", &Perm<n>::template extend<i.value>);
    });
    regina::for_constexpr<n+1, 17>([&c](auto i) {
        c.def_static("contract", &Perm<n>::template contract<i.value>);
    });
    regina::python::add_output_basic(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c);
}

void addPerm(pybind11::module_& m) {
    m.def("digit", regina::digit);
    m.def("factorial", regina::factorial);

    pybind11::enum_<regina::PermCodeType>(m, "PermCodeType")
        .value("PERM_CODE_IMAGES", regina::PERM_CODE_IMAGES)
        .value("PERM_CODE_INDEX", regina::PERM_CODE_INDEX)
        .export_values()
        ;

    addPerm<8>(m, "Perm8");
    addPerm<9>(m, "Perm9");
    addPerm<10>(m, "Perm10");
    addPerm<11>(m, "Perm11");
    addPerm<12>(m, "Perm12");
    addPerm<13>(m, "Perm13");
    addPerm<14>(m, "Perm14");
    addPerm<15>(m, "Perm15");
    addPerm<16>(m, "Perm16");
}

