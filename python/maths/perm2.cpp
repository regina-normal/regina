
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
#include "../docstrings/maths/perm2.h"

using pybind11::overload_cast;
using regina::Perm;

void addPerm2(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Perm)

    auto c = pybind11::class_<Perm<2>>(m, "Perm2", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<int, int>(), rdoc::__init)
        .def(pybind11::init<const Perm<2>&>(), rdoc::__copy)
        // The std::array constructor must come last.  Otherwise an attempt to
        // use the copy constructor throws an exception: Perm has no len()
        .def(pybind11::init<const std::array<int, 2>&>(), rdoc::__init_2)
        .def_static("precompute", &Perm<2>::precompute, rdoc::precompute)
        .def("permCode", &Perm<2>::permCode, rdoc::permCode)
        .def("setPermCode", &Perm<2>::setPermCode, rdoc::setPermCode)
        .def_static("fromPermCode", &Perm<2>::fromPermCode, rdoc::fromPermCode)
        .def_static("isPermCode", &Perm<2>::isPermCode, rdoc::isPermCode)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def("cachedComp", overload_cast<const Perm<2>&>(
            &Perm<2>::cachedComp, pybind11::const_), rdoc::cachedComp)
        .def("cachedComp", [](Perm<2> p, Perm<2> q, Perm<2> r) { // deprecated
            return p.cachedComp(q).cachedComp(r);
        }, rdoc::cachedComp_2)
        .def("inverse", &Perm<2>::inverse, rdoc::inverse)
        .def("cachedInverse", &Perm<2>::cachedInverse, rdoc::cachedInverse)
        .def("conjugate", &Perm<2>::conjugate, rdoc::conjugate)
        .def("cachedConjugate", &Perm<2>::cachedConjugate,
            rdoc::cachedConjugate)
        .def("pow", &Perm<2>::pow, rdoc::pow)
        .def("cachedPow", &Perm<2>::cachedPow, rdoc::cachedPow)
        .def("order", &Perm<2>::order, rdoc::order)
        .def("cachedOrder", &Perm<2>::cachedOrder, rdoc::cachedOrder)
        .def("reverse", &Perm<2>::reverse, rdoc::reverse)
        .def("sign", &Perm<2>::sign, rdoc::sign)
        .def("__getitem__", &Perm<2>::operator[], rdoc::__array)
        .def("pre", &Perm<2>::pre, rdoc::pre)
        .def("compareWith", &Perm<2>::compareWith, rdoc::compareWith)
        .def("isIdentity", &Perm<2>::isIdentity, rdoc::isIdentity)
        .def("inc", [](Perm<2>& p) {
            return p++;
        }, rdoc::__inc)
        .def(pybind11::self < pybind11::self, rdoc::__lt)
        .def_static("rot", &Perm<2>::rot, rdoc::rot)
        .def_static("rand", static_cast<Perm<2>(&)(bool)>(Perm<2>::rand),
            pybind11::arg("even") = false, rdoc::rand)
        .def("trunc", &Perm<2>::trunc, rdoc::trunc)
        .def("clear", &Perm<2>::clear, rdoc::clear)
        .def("S2Index", &Perm<2>::S2Index, rdoc::S2Index)
        .def("SnIndex", &Perm<2>::SnIndex, rdoc::SnIndex)
        .def("orderedS2Index", &Perm<2>::orderedS2Index, rdoc::orderedS2Index)
        .def("orderedSnIndex", &Perm<2>::orderedSnIndex, rdoc::orderedSnIndex)
        .def("isConjugacyMinimal", &Perm<2>::isConjugacyMinimal,
            rdoc::isConjugacyMinimal)
        .def_readonly_static("codeType", &Perm<2>::codeType)
        .def_readonly_static("nPerms", &Perm<2>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<2>::nPerms_1)
        .def_readonly_static("S2", &Perm<2>::S2)
        .def_readonly_static("Sn", &Perm<2>::Sn)
        .def_readonly_static("orderedS2", &Perm<2>::orderedS2)
        .def_readonly_static("orderedSn", &Perm<2>::orderedSn)
        .def_readonly_static("S1", &Perm<2>::S1)
        .def_readonly_static("Sn_1", &Perm<2>::Sn_1)
    ;
    regina::for_constexpr<3, 17>([&c](auto i) {
        c.def_static("contract", &Perm<2>::template contract<i.value>,
            rdoc::contract);
    });
    regina::python::add_output_basic(c, rdoc::str);
    regina::python::add_tight_encoding(c, rdoc::tightEncoding,
        rdoc::tightDecoding, rdoc::hash);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_lightweight_array<decltype(Perm<2>::S2)>(c,
        "_S2", rdoc::S2Lookup);
    regina::python::add_lightweight_array<decltype(Perm<2>::S1)>(c,
        "_S1", rdoc::S1Lookup);

    RDOC_SCOPE_END
}

