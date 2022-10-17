
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
#include "../docstrings/maths/perm7.h"

using pybind11::overload_cast;
using regina::Perm;

void addPerm7(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Perm)

    auto c = pybind11::class_<Perm<7>>(m, "Perm7", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<int, int>(), rdoc::__init)
        .def(pybind11::init<int, int, int, int, int, int, int>(),
            rdoc::__init_2)
        .def(pybind11::init<const std::array<int, 7>&>(), rdoc::__init_3)
        .def(pybind11::init<int, int, int, int, int, int, int,
                            int, int, int, int, int, int, int>(),
            rdoc::__init_4)
        .def(pybind11::init<const Perm<7>&>(), rdoc::__copy)
        .def_static("precompute", &Perm<7>::precompute, rdoc::precompute)
        .def("permCode1", &Perm<7>::permCode1, rdoc::permCode1)
        .def("permCode2", &Perm<7>::permCode2, rdoc::permCode2)
        .def("setPermCode1", &Perm<7>::setPermCode1, rdoc::setPermCode1)
        .def("setPermCode2", &Perm<7>::setPermCode2, rdoc::setPermCode2)
        .def_static("fromPermCode1", &Perm<7>::fromPermCode1,
            rdoc::fromPermCode1)
        .def_static("fromPermCode2", &Perm<7>::fromPermCode2,
            rdoc::fromPermCode2)
        .def_static("isPermCode1", &Perm<7>::isPermCode1, rdoc::isPermCode1)
        .def_static("isPermCode2", &Perm<7>::isPermCode2, rdoc::isPermCode2)
        .def("imagePack", &Perm<7>::imagePack, rdoc::imagePack)
        .def_static("fromImagePack", &Perm<7>::fromImagePack,
            rdoc::fromImagePack)
        .def_static("isImagePack", &Perm<7>::isImagePack, rdoc::isImagePack)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def("cachedComp", overload_cast<const Perm<7>&>(
            &Perm<7>::cachedComp, pybind11::const_), rdoc::cachedComp)
        .def("cachedComp", overload_cast<const Perm<7>&, const Perm<7>&>(
            &Perm<7>::cachedComp, pybind11::const_), rdoc::cachedComp_2)
        .def("inverse", &Perm<7>::inverse, rdoc::inverse)
        .def("pow", &Perm<7>::pow, rdoc::pow)
        .def("cachedPow", &Perm<7>::cachedPow, rdoc::cachedPow)
        .def("order", &Perm<7>::order, rdoc::order)
        .def("cachedOrder", &Perm<7>::cachedOrder, rdoc::cachedOrder)
        .def("reverse", &Perm<7>::reverse, rdoc::reverse)
        .def("sign", &Perm<7>::sign, rdoc::sign)
        .def("__getitem__", &Perm<7>::operator[], rdoc::__array)
        .def("pre", &Perm<7>::pre, rdoc::pre)
        .def("compareWith", &Perm<7>::compareWith, rdoc::compareWith)
        .def("isIdentity", &Perm<7>::isIdentity, rdoc::isIdentity)
        .def("inc", [](Perm<7>& p) {
            return p++;
        }, rdoc::__inc)
        .def(pybind11::self < pybind11::self, rdoc::__lt)
        .def_static("rot", &Perm<7>::rot, rdoc::rot)
        .def_static("rand", static_cast<Perm<7>(&)(bool)>(Perm<7>::rand),
            pybind11::arg("even") = false, rdoc::rand)
        .def("trunc", &Perm<7>::trunc, rdoc::trunc)
        .def("clear", &Perm<7>::clear, rdoc::clear)
        .def("S7Index", static_cast<int (Perm<7>::*)() const>(
            &Perm<7>::S7Index), rdoc::S7Index)
        .def("SnIndex", &Perm<7>::SnIndex, rdoc::SnIndex)
        .def("orderedS7Index", &Perm<7>::orderedS7Index, rdoc::orderedS7Index)
        .def("orderedSnIndex", &Perm<7>::orderedSnIndex, rdoc::orderedSnIndex)
        .def("isConjugacyMinimal", &Perm<7>::isConjugacyMinimal,
            rdoc::isConjugacyMinimal)
        .def_static("extend", &Perm<7>::extend<2>, rdoc::extend)
        .def_static("extend", &Perm<7>::extend<3>, rdoc::extend)
        .def_static("extend", &Perm<7>::extend<4>, rdoc::extend)
        .def_static("extend", &Perm<7>::extend<5>, rdoc::extend)
        .def_static("extend", &Perm<7>::extend<6>, rdoc::extend)
        .def_readonly_static("codeType", &Perm<7>::codeType)
        .def_readonly_static("imageBits", &Perm<7>::imageBits)
        .def_readonly_static("imageMask", &Perm<7>::imageMask)
        .def_readonly_static("nPerms", &Perm<7>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<7>::nPerms_1)
        .def_readonly_static("S7", &Perm<7>::S7)
        .def_readonly_static("Sn", &Perm<7>::Sn)
        .def_readonly_static("orderedS7", &Perm<7>::orderedS7)
        .def_readonly_static("orderedSn", &Perm<7>::orderedSn)
    ;
    regina::for_constexpr<8, 17>([&c](auto i) {
        c.def_static("contract", &Perm<7>::template contract<i.value>,
            rdoc::contract);
    });
    regina::python::add_output_basic(c, rdoc::str);
    regina::python::add_tight_encoding(c, rdoc::tightEncoding,
        rdoc::tightDecoding);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_lightweight_array<decltype(Perm<7>::S7)>(c,
        "_S7", rdoc::S7Lookup);
    regina::python::add_lightweight_array<decltype(Perm<7>::orderedS7)>(c,
        "_orderedS7", rdoc::OrderedS7Lookup);

    RDOC_SCOPE_END
}

