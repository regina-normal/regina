
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
#include "../docstrings/maths/perm6.h"

using pybind11::overload_cast;
using regina::Perm;

void addPerm6(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Perm)

    auto c = pybind11::class_<Perm<6>>(m, "Perm6", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<int, int>(), rdoc::__init)
        .def(pybind11::init<int, int, int, int, int, int>(), rdoc::__init_2)
        .def(pybind11::init<const std::array<int, 6>&>(), rdoc::__init_3)
        .def(pybind11::init<int, int, int, int, int, int,
                            int, int, int, int, int, int>(), rdoc::__init_4)
        .def(pybind11::init<const Perm<6>&>(), rdoc::__copy)
        .def_static("precompute", &Perm<6>::precompute, rdoc::precompute)
        .def("permCode1", &Perm<6>::permCode1, rdoc::permCode1)
        .def("permCode2", &Perm<6>::permCode2, rdoc::permCode2)
        .def("setPermCode1", &Perm<6>::setPermCode1, rdoc::setPermCode1)
        .def("setPermCode2", &Perm<6>::setPermCode2, rdoc::setPermCode2)
        .def_static("fromPermCode1", &Perm<6>::fromPermCode1,
            rdoc::fromPermCode1)
        .def_static("fromPermCode2", &Perm<6>::fromPermCode2,
            rdoc::fromPermCode2)
        .def_static("isPermCode1", &Perm<6>::isPermCode1, rdoc::isPermCode1)
        .def_static("isPermCode2", &Perm<6>::isPermCode2, rdoc::isPermCode2)
        .def("imagePack", &Perm<6>::imagePack, rdoc::imagePack)
        .def_static("fromImagePack", &Perm<6>::fromImagePack,
            rdoc::fromImagePack)
        .def_static("isImagePack", &Perm<6>::isImagePack, rdoc::isImagePack)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def("cachedComp", overload_cast<const Perm<6>&>(
            &Perm<6>::cachedComp, pybind11::const_), rdoc::cachedComp)
        .def("cachedComp", overload_cast<const Perm<6>&, const Perm<6>&>(
            &Perm<6>::cachedComp, pybind11::const_), rdoc::cachedComp_2)
        .def("inverse", &Perm<6>::inverse, rdoc::inverse)
        .def("cachedInverse", &Perm<6>::cachedInverse, RDOC_TODO)
        .def("pow", &Perm<6>::pow, rdoc::pow)
        .def("cachedPow", &Perm<6>::cachedPow, rdoc::cachedPow)
        .def("order", &Perm<6>::order, rdoc::order)
        .def("reverse", &Perm<6>::reverse, rdoc::reverse)
        .def("sign", &Perm<6>::sign, rdoc::sign)
        .def("__getitem__", &Perm<6>::operator[], rdoc::__array)
        .def("pre", &Perm<6>::pre, rdoc::pre)
        .def("compareWith", &Perm<6>::compareWith, rdoc::compareWith)
        .def("isIdentity", &Perm<6>::isIdentity, rdoc::isIdentity)
        .def("inc", [](Perm<6>& p) {
            return p++;
        }, rdoc::__inc)
        .def(pybind11::self < pybind11::self, rdoc::__lt)
        .def_static("rot", &Perm<6>::rot, rdoc::rot)
        .def_static("rand", static_cast<Perm<6>(&)(bool)>(Perm<6>::rand),
            pybind11::arg("even") = false, rdoc::rand)
        .def("trunc", &Perm<6>::trunc, rdoc::trunc)
        .def("clear", &Perm<6>::clear, rdoc::clear)
        .def("S6Index", static_cast<int (Perm<6>::*)() const>(
            &Perm<6>::S6Index), rdoc::S6Index)
        .def("SnIndex", &Perm<6>::SnIndex, rdoc::SnIndex)
        .def("orderedS6Index", &Perm<6>::orderedS6Index, rdoc::orderedS6Index)
        .def("orderedSnIndex", &Perm<6>::orderedSnIndex, rdoc::orderedSnIndex)
        .def("isConjugacyMinimal", &Perm<6>::isConjugacyMinimal,
            rdoc::isConjugacyMinimal)
        .def_static("extend", &Perm<6>::extend<2>, rdoc::extend)
        .def_static("extend", &Perm<6>::extend<3>, rdoc::extend)
        .def_static("extend", &Perm<6>::extend<4>, rdoc::extend)
        .def_static("extend", &Perm<6>::extend<5>, rdoc::extend)
        .def_readonly_static("codeType", &Perm<6>::codeType)
        .def_readonly_static("imageBits", &Perm<6>::imageBits)
        .def_readonly_static("imageMask", &Perm<6>::imageMask)
        .def_readonly_static("nPerms", &Perm<6>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<6>::nPerms_1)
        .def_readonly_static("S6", &Perm<6>::S6)
        .def_readonly_static("Sn", &Perm<6>::Sn)
        .def_readonly_static("orderedS6", &Perm<6>::orderedS6)
        .def_readonly_static("orderedSn", &Perm<6>::orderedSn)
    ;
    regina::for_constexpr<7, 17>([&c](auto i) {
        c.def_static("contract", &Perm<6>::template contract<i.value>,
            rdoc::contract);
    });
    regina::python::add_output_basic(c, rdoc::str);
    regina::python::add_tight_encoding(c, rdoc::tightEncoding,
        rdoc::tightDecoding);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_lightweight_array<decltype(Perm<6>::S6)>(c,
        "_S6", rdoc::S6Lookup);
    regina::python::add_lightweight_array<decltype(Perm<6>::orderedS6)>(c,
        "_orderedS6", rdoc::OrderedS6Lookup);

    RDOC_SCOPE_END
}

