
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

// We need to see Python.h first to avoid a "portability fix" in pyport.h
// that breaks boost.python on MacOSX.
#include "Python.h"
#include <boost/python.hpp>
#include "maths/perm.h"
#include "../globalarray.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Perm;
using regina::python::GlobalArray;

namespace {
    GlobalArray<Perm<2>> Perm2_S2_arr(Perm<2>::S2, 2);
    GlobalArray<unsigned> Perm2_invS2_arr(Perm<2>::invS2, 2);
    GlobalArray<Perm<2>> Perm2_S1_arr(Perm<2>::S1, 1);

    template <int k>
    struct Perm2_contract : boost::python::def_visitor<Perm2_contract<k>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &Perm<2>::contract<k>);
            c.def(Perm2_contract<k+1>());
        }
    };

    template <>
    struct Perm2_contract<16> :
            boost::python::def_visitor<Perm2_contract<16>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &Perm<2>::contract<16>);
        }
    };
}

void addPerm2() {
    {
        scope s = class_<Perm<2>>("Perm2")
            .def(init<int, int>())
            .def(init<const Perm<2>&>())
            .def("permCode", &Perm<2>::permCode)
            .def("setPermCode", &Perm<2>::setPermCode)
            .def("fromPermCode", &Perm<2>::fromPermCode)
            .def("isPermCode", &Perm<2>::isPermCode)
            .def(self * self)
            .def("inverse", &Perm<2>::inverse)
            .def("reverse", &Perm<2>::reverse)
            .def("sign", &Perm<2>::sign)
            .def("__getitem__", &Perm<2>::operator[])
            .def("preImageOf", &Perm<2>::preImageOf)
            .def("compareWith", &Perm<2>::compareWith)
            .def("isIdentity", &Perm<2>::isIdentity)
            .def("atIndex", &Perm<2>::atIndex)
            .def("index", &Perm<2>::index)
            .def("rand", &Perm<2>::rand)
            .def("trunc", &Perm<2>::trunc)
            .def("clear", &Perm<2>::clear)
            .def("S2Index", &Perm<2>::S2Index)
            .def("orderedS2Index", &Perm<2>::orderedS2Index)
            .def("orderedSnIndex", &Perm<2>::orderedS2Index)
            .def(Perm2_contract<3>())
            .def("__repr__", &Perm<2>::str)
            .def(regina::python::add_output_basic())
            .def(regina::python::add_eq_operators())
            .staticmethod("fromPermCode")
            .staticmethod("isPermCode")
            .staticmethod("atIndex")
            .staticmethod("rand")
            .staticmethod("contract")
        ;

        s.attr("nPerms") = Perm<2>::nPerms;
        s.attr("nPerms_1") = Perm<2>::nPerms_1;

        s.attr("S2") = &Perm2_S2_arr;
        s.attr("Sn") = &Perm2_S2_arr;
        s.attr("orderedS2") = &Perm2_S2_arr;
        s.attr("orderedSn") = &Perm2_S2_arr;
        s.attr("invS2") = &Perm2_invS2_arr;
        s.attr("invSn") = &Perm2_invS2_arr;
        s.attr("S1") = &Perm2_S1_arr;
        s.attr("Sn_1") = &Perm2_S1_arr;
    }

    scope().attr("NPerm2") = scope().attr("Perm2");
}

