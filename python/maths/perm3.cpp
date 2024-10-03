
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "../docstrings/maths/perm3.h"

using pybind11::overload_cast;
using regina::Perm;

void addPerm3(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Perm)

    auto c = pybind11::class_<Perm<3>>(m, "Perm3", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<int, int>(), rdoc::__init)
        .def(pybind11::init<int, int, int>(), rdoc::__init_2)
        .def(pybind11::init<int, int, int, int, int, int>(), rdoc::__init_4)
        .def(pybind11::init<const Perm<3>&>(), rdoc::__copy)
        // The std::array constructor must come last.  Otherwise an attempt to
        // use the copy constructor throws an exception: Perm has no len()
        .def(pybind11::init<const std::array<int, 3>&>(), rdoc::__init_3)
        .def_static("precompute", &Perm<3>::precompute, rdoc::precompute)
        .def("permCode", &Perm<3>::permCode, rdoc::permCode)
        .def("setPermCode", &Perm<3>::setPermCode, rdoc::setPermCode)
        .def_static("fromPermCode", &Perm<3>::fromPermCode, rdoc::fromPermCode)
        .def_static("isPermCode", &Perm<3>::isPermCode, rdoc::isPermCode)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def("cachedComp", overload_cast<const Perm<3>&>(
            &Perm<3>::cachedComp, pybind11::const_), rdoc::cachedComp)
        .def("cachedComp", [](Perm<3> p, Perm<3> q, Perm<3> r) { // deprecated
            return p.cachedComp(q).cachedComp(r);
        }, rdoc::cachedComp_2)
        .def("inverse", &Perm<3>::inverse, rdoc::inverse)
        .def("cachedInverse", &Perm<3>::cachedInverse, rdoc::cachedInverse)
        .def("conjugate", &Perm<3>::conjugate, rdoc::conjugate)
        .def("cachedConjugate", &Perm<3>::cachedConjugate,
            rdoc::cachedConjugate)
        .def("pow", &Perm<3>::pow, rdoc::pow)
        .def("cachedPow", &Perm<3>::cachedPow, rdoc::cachedPow)
        .def("order", &Perm<3>::order, rdoc::order)
        .def("cachedOrder", &Perm<3>::cachedOrder, rdoc::cachedOrder)
        .def("reverse", &Perm<3>::reverse, rdoc::reverse)
        .def("sign", &Perm<3>::sign, rdoc::sign)
        .def("__getitem__", &Perm<3>::operator[], rdoc::__array)
        .def("pre", &Perm<3>::pre, rdoc::pre)
        .def("compareWith", &Perm<3>::compareWith, rdoc::compareWith)
        .def("isIdentity", &Perm<3>::isIdentity, rdoc::isIdentity)
        .def("inc", [](Perm<3>& p) {
            return p++;
        }, rdoc::__inc)
        .def_static("rot", &Perm<3>::rot, rdoc::rot)
        .def_static("rand", static_cast<Perm<3>(&)(bool)>(Perm<3>::rand),
            pybind11::arg("even") = false, rdoc::rand)
        .def("trunc", &Perm<3>::trunc, rdoc::trunc)
        .def("trunc2", &Perm<3>::trunc2, rdoc::trunc2)
        .def("clear", &Perm<3>::clear, rdoc::clear)
        .def("S3Index", &Perm<3>::S3Index, rdoc::S3Index)
        .def("SnIndex", &Perm<3>::SnIndex, rdoc::SnIndex)
        .def("orderedS3Index", &Perm<3>::orderedS3Index, rdoc::orderedS3Index)
        .def("orderedSnIndex", &Perm<3>::orderedSnIndex, rdoc::orderedSnIndex)
        .def("isConjugacyMinimal", &Perm<3>::isConjugacyMinimal,
            rdoc::isConjugacyMinimal)
        .def_static("extend", &Perm<3>::extend<2>, rdoc::extend)
        .def_readonly_static("degree", &Perm<3>::degree)
        .def_readonly_static("codeType", &Perm<3>::codeType)
        .def_readonly_static("nPerms", &Perm<3>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<3>::nPerms_1)
        .def_readonly_static("S3", &Perm<3>::S3)
        .def_readonly_static("Sn", &Perm<3>::Sn)
        .def_readonly_static("orderedS3", &Perm<3>::orderedS3)
        .def_readonly_static("orderedSn", &Perm<3>::orderedSn)
        .def_readonly_static("S2", &Perm<3>::S2)
        .def_readonly_static("Sn_1", &Perm<3>::Sn_1)
        .def_readonly_static("code012", &Perm<3>::code012)
        .def_readonly_static("code021", &Perm<3>::code021)
        .def_readonly_static("code102", &Perm<3>::code102)
        .def_readonly_static("code120", &Perm<3>::code120)
        .def_readonly_static("code201", &Perm<3>::code201)
        .def_readonly_static("code210", &Perm<3>::code210)
    ;
    regina::for_constexpr<4, 17>([&c](auto i) {
        c.def_static("contract", &Perm<3>::template contract<i>,
            rdoc::contract);
    });
    regina::python::add_output_basic(c, rdoc::str);
    regina::python::add_tight_encoding(c, rdoc::tightEncoding,
        rdoc::tightDecoding, rdoc::hash);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    regina::python::add_lightweight_array<decltype(Perm<3>::S3)>(c,
        "_S3", rdoc::S3Lookup);
    regina::python::add_lightweight_array<decltype(Perm<3>::orderedS3)>(c,
        "_orderedS3", rdoc::OrderedS3Lookup);
    regina::python::add_lightweight_array<decltype(Perm<3>::S2)>(c,
        "_S2", rdoc::S2Lookup);

    RDOC_SCOPE_END
}

