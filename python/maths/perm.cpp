
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
#include "../helpers.h"
#include "../helpers/arraylike.h"
#include "../docstrings/maths/perm.h"

using regina::Perm;

template <int n>
void addPerm(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(Perm)

    auto c = pybind11::class_<Perm<n>>(m, name, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__init)
        .def(pybind11::init<int, int>(), rdoc::__init_2)
        .def(pybind11::init<const std::array<int, n>&>(), rdoc::__init_3)
        .def(pybind11::init<const Perm<n>&>(), rdoc::__copy)
        .def("permCode", &Perm<n>::permCode, rdoc::permCode)
        .def("setPermCode", &Perm<n>::setPermCode, rdoc::setPermCode)
        .def_static("fromPermCode", &Perm<n>::fromPermCode, rdoc::fromPermCode)
        .def_static("isPermCode", &Perm<n>::isPermCode, rdoc::isPermCode)
        .def("imagePack", &Perm<n>::imagePack, rdoc::imagePack)
        .def_static("fromImagePack", &Perm<n>::fromImagePack,
            rdoc::fromImagePack)
        .def_static("isImagePack", &Perm<n>::isImagePack, rdoc::isImagePack)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def("inverse", &Perm<n>::inverse, rdoc::inverse)
        .def("reverse", &Perm<n>::reverse, rdoc::reverse)
        .def("sign", &Perm<n>::sign, rdoc::sign)
        .def("__getitem__", &Perm<n>::operator[], rdoc::__array)
        .def("pre", &Perm<n>::pre, rdoc::pre)
        .def("compareWith", &Perm<n>::compareWith, rdoc::compareWith)
        .def("isIdentity", &Perm<n>::isIdentity, rdoc::isIdentity)
        .def_static("rot", &Perm<n>::rot, rdoc::rot)
        .def_static("rand", static_cast<Perm<n>(&)(bool)>(Perm<n>::rand),
            pybind11::arg("even") = false, rdoc::rand)
        .def("trunc", &Perm<n>::trunc, rdoc::trunc)
        .def("clear", &Perm<n>::clear, rdoc::clear)
        .def("SnIndex", &Perm<n>::SnIndex, rdoc::SnIndex)
        .def("orderedSnIndex", &Perm<n>::orderedSnIndex,
            rdoc::orderedSnIndex)
        .def_readonly_static("codeType", &Perm<n>::codeType)
        .def_readonly_static("imageBits", &Perm<n>::imageBits)
        .def_readonly_static("imageMask", &Perm<n>::imageMask)
        .def_readonly_static("nPerms", &Perm<n>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<n>::nPerms_1)
        .def_readonly_static("Sn", &Perm<n>::Sn)
        .def_readonly_static("orderedSn", &Perm<n>::orderedSn)
    ;
    regina::for_constexpr<2, n>([&c](auto i) {
        c.def_static("extend", &Perm<n>::template extend<i.value>,
            rdoc::extend);
    });
    regina::for_constexpr<n+1, 17>([&c](auto i) {
        c.def_static("contract", &Perm<n>::template contract<i.value>,
            rdoc::contract);
    });
    regina::python::add_output_basic(c, rdoc::str);
    regina::python::add_tight_encoding(c, rdoc::tightEncoding,
        rdoc::tightDecoding);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_lightweight_array<decltype(Perm<n>::Sn)>(c,
        "_Sn", rdoc::SnLookup);
    regina::python::add_lightweight_array<decltype(Perm<n>::orderedSn)>(c,
        "_OrderedSn", rdoc::OrderedSnLookup);

    RDOC_SCOPE_END
}

void addPerm(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    m.def("digit", regina::digit, rdoc::digit);
    m.def("factorial", regina::factorial, rdoc::factorial);

    RDOC_SCOPE_SWITCH(PermCodeType)

    pybind11::enum_<regina::PermCodeType>(m, "PermCodeType", rdoc_scope)
        .value("PERM_CODE_IMAGES", regina::PERM_CODE_IMAGES,
            rdoc::PERM_CODE_IMAGES)
        .value("PERM_CODE_INDEX", regina::PERM_CODE_INDEX,
            rdoc::PERM_CODE_INDEX)
        .export_values()
        ;

    RDOC_SCOPE_END

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

