
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

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "maths/perm.h"
#include "utilities/typeutils.h"
#include "../helpers.h"
#include "../helpers/arraylike.h"
#include "../docstrings/maths/perm.h"

using pybind11::overload_cast;
using regina::Perm;
using regina::PermClass;
using regina::PermCodeType;
using regina::PermOrder;

template <int n>
void addPerm(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(Perm)

    auto c = pybind11::class_<Perm<n>>(m, name, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<int, int>(), rdoc::__init)
        .def(pybind11::init<const Perm<n>&>(), rdoc::__copy)
        // The std::array constructor must come last.  Otherwise an attempt to
        // use the copy constructor throws an exception: Perm has no len()
        .def(pybind11::init<const std::array<int, n>&>(), rdoc::__init_2)
        .def_static("precompute", &Perm<n>::precompute, rdoc::precompute)
        .def("permCode", &Perm<n>::permCode, rdoc::permCode)
        .def("setPermCode", &Perm<n>::setPermCode, rdoc::setPermCode)
        .def_static("fromPermCode", &Perm<n>::fromPermCode, rdoc::fromPermCode)
        .def_static("isPermCode", &Perm<n>::isPermCode, rdoc::isPermCode)
        .def("imagePack", &Perm<n>::imagePack, rdoc::imagePack)
        .def_static("fromImagePack", &Perm<n>::fromImagePack,
            rdoc::fromImagePack)
        .def_static("isImagePack", &Perm<n>::isImagePack, rdoc::isImagePack)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def("cachedComp", overload_cast<const Perm<n>&>(
            &Perm<n>::cachedComp, pybind11::const_), rdoc::cachedComp)
        .def("cachedComp", [](Perm<n> p, Perm<n> q, Perm<n> r) { // deprecated
            return p.cachedComp(q).cachedComp(r);
        }, rdoc::cachedComp_2)
        .def("inverse", &Perm<n>::inverse, rdoc::inverse)
        .def("cachedInverse", &Perm<n>::cachedInverse, rdoc::cachedInverse)
        .def("conjugate", &Perm<n>::conjugate, rdoc::conjugate)
        .def("cachedConjugate", &Perm<n>::cachedConjugate,
            rdoc::cachedConjugate)
        .def("pow", &Perm<n>::pow, rdoc::pow)
        .def("cachedPow", &Perm<n>::cachedPow, rdoc::cachedPow)
        .def("order", &Perm<n>::order, rdoc::order)
        .def("cachedOrder", &Perm<n>::cachedOrder, rdoc::cachedOrder)
        .def("reverse", &Perm<n>::reverse, rdoc::reverse)
        .def("sign", &Perm<n>::sign, rdoc::sign)
        .def("__getitem__", &Perm<n>::operator[], rdoc::__array)
        .def("pre", &Perm<n>::pre, rdoc::pre)
        .def("compareWith", &Perm<n>::compareWith, rdoc::compareWith)
        .def("isIdentity", &Perm<n>::isIdentity, rdoc::isIdentity)
        .def("inc", [](Perm<n>& p) {
            return p++;
        }, rdoc::__inc)
        .def_static("rot", &Perm<n>::rot, rdoc::rot)
        .def_static("rand", static_cast<Perm<n>(&)(bool)>(Perm<n>::rand),
            pybind11::arg("even") = false, rdoc::rand)
        .def("trunc", &Perm<n>::trunc, rdoc::trunc)
        .def("clear", &Perm<n>::clear, rdoc::clear)
        .def("SnIndex", &Perm<n>::SnIndex, rdoc::SnIndex)
        .def("orderedSnIndex", &Perm<n>::orderedSnIndex,
            rdoc::orderedSnIndex)
        .def("isConjugacyMinimal", &Perm<n>::isConjugacyMinimal,
            rdoc::isConjugacyMinimal)
        .def_readonly_static("degree", &Perm<n>::degree)
        .def_readonly_static("codeType", &Perm<n>::codeType)
        .def_readonly_static("imageBits", &Perm<n>::imageBits)
        .def_readonly_static("imageMask", &Perm<n>::imageMask)
        .def_readonly_static("nPerms", &Perm<n>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<n>::nPerms_1)
        .def_readonly_static("Sn", &Perm<n>::Sn)
        .def_readonly_static("orderedSn", &Perm<n>::orderedSn)
    ;
    regina::for_constexpr<2, n>([&c](auto i) {
        c.def_static("extend", &Perm<n>::template extend<i>, rdoc::extend);
    });
    regina::for_constexpr<n+1, 17>([&c](auto i) {
        c.def_static("contract", &Perm<n>::template contract<i>,
            rdoc::contract);
    });
    regina::python::add_output_basic(c, rdoc::str);
    regina::python::add_tight_encoding(c, rdoc::tightEncoding,
        rdoc::tightDecoding, rdoc::hash);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    RDOC_SCOPE_END
}

template <int n>
void addPermClass(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(PermClass)

    auto c = pybind11::class_<PermClass<n>>(m, name, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const PermClass<n>&>(), rdoc::__copy)
        .def("isIdentity", &PermClass<n>::isIdentity, rdoc::isIdentity)
        .def("cycle", &PermClass<n>::cycle, rdoc::cycle)
        .def("countCycles", &PermClass<n>::countCycles, rdoc::countCycles)
        .def("rep", &PermClass<n>::rep, rdoc::rep)
        .def("inc", [](PermClass<n>& p) {
            return p++;
        }, rdoc::__inc)
        .def("__bool__", &PermClass<n>::operator bool, rdoc::__as_bool)
        .def_readonly_static("count", &PermClass<n>::count)
    ;
    regina::python::add_output_basic(c, rdoc::str);
    regina::python::add_eq_operators(c, rdoc::__eq);

    RDOC_SCOPE_END
}

void addPerm(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    m.def("digit", regina::digit, rdoc::digit);
    m.def("factorial", regina::factorial, rdoc::factorial);

    RDOC_SCOPE_SWITCH(PermCodeType)

    pybind11::enum_<regina::PermCodeType>(m, "PermCodeType", rdoc_scope)
        .value("Images", PermCodeType::Images, rdoc::Images)
        .value("Index", PermCodeType::Index, rdoc::Index)
        ;

    // Deprecated constants:
    m.attr("PERM_CODE_IMAGES") = PermCodeType::Images;
    m.attr("PERM_CODE_INDEX") = PermCodeType::Index;

    RDOC_SCOPE_SWITCH(PermOrder)

    pybind11::enum_<regina::PermOrder>(m, "PermOrder", rdoc_scope)
        .value("Sign", PermOrder::Sign, rdoc::Sign)
        .value("Lex", PermOrder::Lex, rdoc::Lex)
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

    addPermClass<2>(m, "PermClass2");
    addPermClass<3>(m, "PermClass3");
    addPermClass<4>(m, "PermClass4");
    addPermClass<5>(m, "PermClass5");
    addPermClass<6>(m, "PermClass6");
    addPermClass<7>(m, "PermClass7");
    addPermClass<8>(m, "PermClass8");
    addPermClass<9>(m, "PermClass9");
    addPermClass<10>(m, "PermClass10");
    addPermClass<11>(m, "PermClass11");
    addPermClass<12>(m, "PermClass12");
    addPermClass<13>(m, "PermClass13");
    addPermClass<14>(m, "PermClass14");
    addPermClass<15>(m, "PermClass15");
    addPermClass<16>(m, "PermClass16");
}

