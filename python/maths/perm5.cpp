
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
#include "../docstrings/maths/perm5.h"

using pybind11::overload_cast;
using regina::Perm;

void addPerm5(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Perm)

    auto c = pybind11::class_<Perm<5>>(m, "Perm5", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<int, int>(), rdoc::__init)
        .def(pybind11::init<int, int, int, int, int>(), rdoc::__init_2)
        .def(pybind11::init<int, int, int, int, int,
                            int, int, int, int, int>(), rdoc::__init_4)
        .def(pybind11::init<const Perm<5>&>(), rdoc::__copy)
        // The std::array constructor must come last.  Otherwise an attempt to
        // use the copy constructor throws an exception: Perm has no len()
        .def(pybind11::init<const std::array<int, 5>&>(), rdoc::__init_3)
        .def_static("precompute", &Perm<5>::precompute, rdoc::precompute)
        .def("permCode1", &Perm<5>::permCode1, rdoc::permCode1)
        .def("permCode2", &Perm<5>::permCode2, rdoc::permCode2)
        .def("setPermCode1", &Perm<5>::setPermCode1, rdoc::setPermCode1)
        .def("setPermCode2", &Perm<5>::setPermCode2, rdoc::setPermCode2)
        .def_static("fromPermCode1", &Perm<5>::fromPermCode1,
            rdoc::fromPermCode1)
        .def_static("fromPermCode2", &Perm<5>::fromPermCode2,
            rdoc::fromPermCode2)
        .def_static("isPermCode1", &Perm<5>::isPermCode1, rdoc::isPermCode1)
        .def_static("isPermCode2", &Perm<5>::isPermCode2, rdoc::isPermCode2)
        .def("imagePack", &Perm<5>::imagePack, rdoc::imagePack)
        .def_static("fromImagePack", &Perm<5>::fromImagePack,
            rdoc::fromImagePack)
        .def_static("isImagePack", &Perm<5>::isImagePack, rdoc::isImagePack)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def("cachedComp", overload_cast<const Perm<5>&>(
            &Perm<5>::cachedComp, pybind11::const_), rdoc::cachedComp)
        .def("cachedComp", [](Perm<5> p, Perm<5> q, Perm<5> r) { // deprecated
            return p.cachedComp(q).cachedComp(r);
        }, rdoc::cachedComp_2)
        .def("inverse", &Perm<5>::inverse, rdoc::inverse)
        .def("cachedInverse", &Perm<5>::cachedInverse, rdoc::cachedInverse)
        .def("conjugate", &Perm<5>::conjugate, rdoc::conjugate)
        .def("cachedConjugate", &Perm<5>::cachedConjugate,
            rdoc::cachedConjugate)
        .def("pow", &Perm<5>::pow, rdoc::pow)
        .def("cachedPow", &Perm<5>::cachedPow, rdoc::cachedPow)
        .def("order", &Perm<5>::order, rdoc::order)
        .def("cachedOrder", &Perm<5>::cachedOrder, rdoc::cachedOrder)
        .def("reverse", &Perm<5>::reverse, rdoc::reverse)
        .def("sign", &Perm<5>::sign, rdoc::sign)
        .def("__getitem__", &Perm<5>::operator[], rdoc::__array)
        .def("pre", &Perm<5>::pre, rdoc::pre)
        .def("compareWith", &Perm<5>::compareWith, rdoc::compareWith)
        .def("isIdentity", &Perm<5>::isIdentity, rdoc::isIdentity)
        .def("inc", [](Perm<5>& p) {
            return p++;
        }, rdoc::__inc)
        .def_static("rot", &Perm<5>::rot, rdoc::rot)
        .def_static("rand", static_cast<Perm<5>(&)(bool)>(Perm<5>::rand),
            pybind11::arg("even") = false, rdoc::rand)
        .def("trunc", &Perm<5>::trunc, rdoc::trunc)
        .def("trunc2", &Perm<5>::trunc2, rdoc::trunc2)
        .def("trunc3", &Perm<5>::trunc3, rdoc::trunc3)
        .def("trunc4", &Perm<5>::trunc4, rdoc::trunc4)
        .def("clear", &Perm<5>::clear, rdoc::clear)
        .def("S5Index", static_cast<int (Perm<5>::*)() const>(
            &Perm<5>::S5Index), rdoc::S5Index)
        .def("SnIndex", &Perm<5>::SnIndex, rdoc::SnIndex)
        .def("orderedS5Index", &Perm<5>::orderedS5Index, rdoc::orderedS5Index)
        .def("orderedSnIndex", &Perm<5>::orderedSnIndex, rdoc::orderedSnIndex)
        .def("isConjugacyMinimal", &Perm<5>::isConjugacyMinimal,
            rdoc::isConjugacyMinimal)
        .def_static("extend", &Perm<5>::extend<2>, rdoc::extend)
        .def_static("extend", &Perm<5>::extend<3>, rdoc::extend)
        .def_static("extend", &Perm<5>::extend<4>, rdoc::extend)
        .def_readonly_static("degree", &Perm<5>::degree)
        .def_readonly_static("codeType", &Perm<5>::codeType)
        .def_readonly_static("imageBits", &Perm<5>::imageBits)
        .def_readonly_static("imageMask", &Perm<5>::imageMask)
        .def_readonly_static("nPerms", &Perm<5>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<4>::nPerms) // deprecated
        .def_readonly_static("S5", &Perm<5>::S5)
        .def_readonly_static("Sn", &Perm<5>::Sn)
        .def_readonly_static("orderedS5", &Perm<5>::orderedS5)
        .def_readonly_static("orderedSn", &Perm<5>::orderedSn)
    ;
    #if defined(__GNUC__)
    // The following members are deprecated, but we still need to bind
    // them.  Silence the inevitable deprecation warnings that will occur.
    #pragma GCC diagnostic push
    #if defined(__clang__)
    #pragma GCC diagnostic ignored "-Wdeprecated"
    #else
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #endif
    #endif
    c.def_readonly_static("Sn_1", &Perm<5>::S4) // deprecated
        .def_readonly_static("S4", &Perm<5>::S4) // deprecated
        .def_readonly_static("orderedS4", &Perm<5>::orderedS4) // deprecated
        .def_readonly_static("S3", &Perm<5>::S3) // deprecated
        .def_readonly_static("orderedS3", &Perm<5>::orderedS3) // deprecated
        .def_readonly_static("S2", &Perm<5>::S2) // deprecated
    ;
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
    regina::for_constexpr<6, 17>([&c](auto i) {
        c.def_static("contract", &Perm<5>::template contract<i>,
            rdoc::contract);
    });
    regina::python::add_output_basic(c, rdoc::str);
    regina::python::add_tight_encoding(c, rdoc::tightEncoding,
        rdoc::tightDecoding, rdoc::hash);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    RDOC_SCOPE_END
}

