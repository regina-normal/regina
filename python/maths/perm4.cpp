
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
#include "../docstrings/maths/perm4.h"

using pybind11::overload_cast;
using regina::Perm;

void addPerm4(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Perm)

    auto c = pybind11::class_<Perm<4>>(m, "Perm4", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<int, int>(), rdoc::__init)
        .def(pybind11::init<int, int, int, int>(), rdoc::__init_2)
        .def(pybind11::init<int, int, int, int,
                            int, int, int, int>(), rdoc::__init_4)
        .def(pybind11::init<const Perm<4>&>(), rdoc::__copy)
        // The std::array constructor must come last.  Otherwise an attempt to
        // use the copy constructor throws an exception: Perm has no len()
        .def(pybind11::init<const std::array<int, 4>&>(), rdoc::__init_3)
        .def_static("precompute", &Perm<4>::precompute, rdoc::precompute)
        .def("permCode1", &Perm<4>::permCode1, rdoc::permCode1)
        .def("permCode2", &Perm<4>::permCode2, rdoc::permCode2)
        .def("setPermCode1", &Perm<4>::setPermCode1, rdoc::setPermCode1)
        .def("setPermCode2", &Perm<4>::setPermCode2, rdoc::setPermCode2)
        .def_static("fromPermCode1", &Perm<4>::fromPermCode1,
            rdoc::fromPermCode1)
        .def_static("fromPermCode2", &Perm<4>::fromPermCode2,
            rdoc::fromPermCode2)
        .def_static("isPermCode1", &Perm<4>::isPermCode1, rdoc::isPermCode1)
        .def_static("isPermCode2", &Perm<4>::isPermCode2, rdoc::isPermCode2)
        .def("imagePack", &Perm<4>::imagePack, rdoc::imagePack)
        .def_static("fromImagePack", &Perm<4>::fromImagePack,
            rdoc::fromImagePack)
        .def_static("isImagePack", &Perm<4>::isImagePack, rdoc::isImagePack)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def("cachedComp", overload_cast<const Perm<4>&>(
            &Perm<4>::cachedComp, pybind11::const_), rdoc::cachedComp)
        .def("cachedComp", [](Perm<4> p, Perm<4> q, Perm<4> r) { // deprecated
            return p.cachedComp(q).cachedComp(r);
        }, rdoc::cachedComp_2)
        .def("inverse", &Perm<4>::inverse, rdoc::inverse)
        .def("cachedInverse", &Perm<4>::cachedInverse, rdoc::cachedInverse)
        .def("conjugate", &Perm<4>::conjugate, rdoc::conjugate)
        .def("cachedConjugate", &Perm<4>::cachedConjugate,
            rdoc::cachedConjugate)
        .def("pow", &Perm<4>::pow, rdoc::pow)
        .def("cachedPow", &Perm<4>::cachedPow, rdoc::cachedPow)
        .def("order", &Perm<4>::order, rdoc::order)
        .def("cachedOrder", &Perm<4>::cachedOrder, rdoc::cachedOrder)
        .def("reverse", &Perm<4>::reverse, rdoc::reverse)
        .def("sign", &Perm<4>::sign, rdoc::sign)
        .def("__getitem__", &Perm<4>::operator[], rdoc::__array)
        .def("pre", &Perm<4>::pre, rdoc::pre)
        .def("compareWith", &Perm<4>::compareWith, rdoc::compareWith)
        .def("isIdentity", &Perm<4>::isIdentity, rdoc::isIdentity)
        .def("inc", [](Perm<4>& p) {
            return p++;
        }, rdoc::__inc)
        .def_static("rot", &Perm<4>::rot, rdoc::rot)
        .def_static("rand", static_cast<Perm<4>(&)(bool)>(Perm<4>::rand),
            pybind11::arg("even") = false, rdoc::rand)
        .def("trunc", &Perm<4>::trunc, rdoc::trunc)
        .def("trunc2", &Perm<4>::trunc2, rdoc::trunc2)
        .def("trunc3", &Perm<4>::trunc3, rdoc::trunc3)
        .def("clear", &Perm<4>::clear, rdoc::clear)
        .def("S4Index", static_cast<int (Perm<4>::*)() const>(
            &Perm<4>::S4Index), rdoc::S4Index)
        .def("SnIndex", &Perm<4>::SnIndex, rdoc::SnIndex)
        .def("orderedS4Index", &Perm<4>::orderedS4Index, rdoc::orderedS4Index)
        .def("orderedSnIndex", &Perm<4>::orderedSnIndex, rdoc::orderedSnIndex)
        .def("isConjugacyMinimal", &Perm<4>::isConjugacyMinimal,
            rdoc::isConjugacyMinimal)
        .def("pairs", &Perm<4>::pairs, rdoc::pairs)
        .def_static("extend", &Perm<4>::extend<2>, rdoc::extend)
        .def_static("extend", &Perm<4>::extend<3>, rdoc::extend)
        .def_readonly_static("degree", &Perm<4>::degree)
        .def_readonly_static("codeType", &Perm<4>::codeType)
        .def_readonly_static("imageBits", &Perm<4>::imageBits)
        .def_readonly_static("imageMask", &Perm<4>::imageMask)
        .def_readonly_static("nPerms", &Perm<4>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<3>::nPerms) // deprecated
        .def_readonly_static("S4", &Perm<4>::S4)
        .def_readonly_static("Sn", &Perm<4>::Sn)
        .def_readonly_static("orderedS4", &Perm<4>::orderedS4)
        .def_readonly_static("orderedSn", &Perm<4>::orderedSn)
        .def_readonly_static("S3", &Perm<4>::S3)
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
        .def_readonly_static("Sn_1", &Perm<4>::Sn_1) // deprecated
        .def_readonly_static("S2", &Perm<4>::S2) // deprecated
        #if defined(__GNUC__)
        #pragma GCC diagnostic pop
        #endif
        .def_readonly_static("orderedS3", &Perm<4>::orderedS3)
    ;
    regina::for_constexpr<5, 17>([&c](auto i) {
        c.def_static("contract", &Perm<4>::template contract<i>,
            rdoc::contract);
    });
    regina::python::add_output_basic(c, rdoc::str);
    regina::python::add_tight_encoding(c, rdoc::tightEncoding,
        rdoc::tightDecoding, rdoc::hash);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    RDOC_SCOPE_END
}

