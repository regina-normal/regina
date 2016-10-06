
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
using regina::NPerm<2>;
using regina::python::GlobalArray;

namespace {
    GlobalArray<NPerm<2>> NPerm2_S2_arr(NPerm<2>::S2, 2);
    GlobalArray<unsigned> NPerm2_invS2_arr(NPerm<2>::invS2, 2);
    GlobalArray<NPerm<2>> NPerm2_S1_arr(NPerm<2>::S1, 1);

    template <int k>
    struct NPerm2_contract : boost::python::def_visitor<NPerm2_contract<k>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &NPerm<2>::contract<k>);
            c.def(NPerm2_contract<k+1>());
        }
    };

    template <>
    struct NPerm2_contract<16> :
            boost::python::def_visitor<NPerm2_contract<16>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &NPerm<2>::contract<16>);
        }
    };
}

void addNPerm2() {
    scope s = class_<NPerm<2>>("NPerm2")
        .def(init<int, int>())
        .def(init<const NPerm<2>&>())
        .def("permCode", &NPerm<2>::permCode)
        .def("setPermCode", &NPerm<2>::setPermCode)
        .def("fromPermCode", &NPerm<2>::fromPermCode)
        .def("isPermCode", &NPerm<2>::isPermCode)
        .def(self * self)
        .def("inverse", &NPerm<2>::inverse)
        .def("reverse", &NPerm<2>::reverse)
        .def("sign", &NPerm<2>::sign)
        .def("__getitem__", &NPerm<2>::operator[])
        .def("preImageOf", &NPerm<2>::preImageOf)
        .def("compareWith", &NPerm<2>::compareWith)
        .def("isIdentity", &NPerm<2>::isIdentity)
        .def("atIndex", &NPerm<2>::atIndex)
        .def("index", &NPerm<2>::index)
        .def("rand", &NPerm<2>::rand)
        .def("trunc", &NPerm<2>::trunc)
        .def("S2Index", &NPerm<2>::S2Index)
        .def("orderedS2Index", &NPerm<2>::orderedS2Index)
        .def("orderedSnIndex", &NPerm<2>::orderedS2Index)
        .def(NPerm2_contract<3>())
        .def("__repr__", &NPerm<2>::str)
        .def(regina::python::add_output_basic())
        .def(regina::python::add_eq_operators())
        .staticmethod("fromPermCode")
        .staticmethod("isPermCode")
        .staticmethod("atIndex")
        .staticmethod("rand")
        .staticmethod("contract")
    ;

    s.attr("nPerms") = NPerm<2>::nPerms;
    s.attr("nPerms_1") = NPerm<2>::nPerms_1;

    s.attr("S2") = &NPerm2_S2_arr;
    s.attr("Sn") = &NPerm2_S2_arr;
    s.attr("orderedS2") = &NPerm2_S2_arr;
    s.attr("orderedSn") = &NPerm2_S2_arr;
    s.attr("invS2") = &NPerm2_invS2_arr;
    s.attr("invSn") = &NPerm2_invS2_arr;
    s.attr("S1") = &NPerm2_S1_arr;
    s.attr("Sn_1") = &NPerm2_S1_arr;
}

