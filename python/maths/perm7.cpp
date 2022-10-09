
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
#include "../docstrings/maths/perm7.h"

using pybind11::overload_cast;
using regina::Perm;

void addPerm7(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Perm)

    auto c = pybind11::class_<Perm<7>>(m, "Perm7", rdoc_scope)
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<int, int, int, int, int, int, int>())
        .def(pybind11::init<int, int, int, int, int, int, int,
                            int, int, int, int, int, int, int>())
        .def(pybind11::init<const std::array<int, 7>&>())
        .def(pybind11::init<const Perm<7>&>())
        .def_static("precompute", &Perm<7>::precompute)
        .def("permCode1", &Perm<7>::permCode1)
        .def("permCode2", &Perm<7>::permCode2)
        .def("setPermCode1", &Perm<7>::setPermCode1)
        .def("setPermCode2", &Perm<7>::setPermCode2)
        .def_static("fromPermCode1", &Perm<7>::fromPermCode1)
        .def_static("fromPermCode2", &Perm<7>::fromPermCode2)
        .def_static("isPermCode1", &Perm<7>::isPermCode1)
        .def_static("isPermCode2", &Perm<7>::isPermCode2)
        .def("imagePack", &Perm<7>::imagePack)
        .def_static("fromImagePack", &Perm<7>::fromImagePack)
        .def_static("isImagePack", &Perm<7>::isImagePack)
        .def(pybind11::self * pybind11::self)
        .def("cachedComp", overload_cast<const Perm<7>&>(
            &Perm<7>::cachedComp, pybind11::const_))
        .def("cachedComp", overload_cast<const Perm<7>&, const Perm<7>&>(
            &Perm<7>::cachedComp, pybind11::const_))
        .def("inverse", &Perm<7>::inverse)
        .def("pow", &Perm<7>::pow)
        .def("cachedPow", &Perm<7>::cachedPow)
        .def("order", &Perm<7>::order)
        .def("reverse", &Perm<7>::reverse)
        .def("sign", &Perm<7>::sign)
        .def("__getitem__", &Perm<7>::operator[])
        .def("pre", &Perm<7>::pre)
        .def("compareWith", &Perm<7>::compareWith)
        .def("isIdentity", &Perm<7>::isIdentity)
        .def("inc", [](Perm<7>& p) {
            return p++;
        })
        .def(pybind11::self < pybind11::self)
        .def_static("rot", &Perm<7>::rot)
        .def_static("rand", static_cast<Perm<7>(&)(bool)>(Perm<7>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<7>::trunc)
        .def("clear", &Perm<7>::clear)
        .def("S7Index", (int (Perm<7>::*)() const) &Perm<7>::S7Index)
        .def("SnIndex", &Perm<7>::SnIndex)
        .def("orderedS7Index", &Perm<7>::orderedS7Index)
        .def("orderedSnIndex", &Perm<7>::orderedS7Index)
        .def("isConjugacyMinimal", &Perm<7>::isConjugacyMinimal)
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

