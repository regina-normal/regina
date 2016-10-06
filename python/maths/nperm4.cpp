
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
using regina::NPerm;
using regina::python::GlobalArray;

namespace {
    GlobalArray<NPerm<4>> NPerm4_S4_arr(NPerm<4>::S4, 24);
    GlobalArray<unsigned> NPerm4_invS4_arr(NPerm<4>::invS4, 24);
    GlobalArray<NPerm<4>> NPerm4_orderedS4_arr(NPerm<4>::orderedS4, 24);
    GlobalArray<NPerm<4>> NPerm4_S3_arr(NPerm<4>::S3, 6);
    GlobalArray<NPerm<4>> NPerm4_orderedS3_arr(NPerm<4>::orderedS3, 6);
    GlobalArray<NPerm<4>> NPerm4_S2_arr(NPerm<4>::S2, 2);

    int (NPerm<4>::*S4Index_void)() const = &NPerm<4>::S4Index;

    template <int k>
    struct NPerm4_contract : boost::python::def_visitor<NPerm4_contract<k>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &NPerm<4>::contract<k>);
            c.def(NPerm4_contract<k+1>());
        }
    };

    template <>
    struct NPerm4_contract<16> :
            boost::python::def_visitor<NPerm4_contract<16>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &NPerm<4>::contract<16>);
        }
    };
}

void addNPerm4() {
    scope s = class_<NPerm<4>>("NPerm4")
        .def(init<int, int>())
        .def(init<int, int, int, int>())
        .def(init<int, int, int, int, int, int, int, int>())
        .def(init<const NPerm<4>&>())
        .def("permCode", &NPerm<4>::permCode)
        .def("permCode2", &NPerm<4>::permCode2)
        .def("setPermCode", &NPerm<4>::setPermCode)
        .def("setPermCode2", &NPerm<4>::setPermCode2)
        .def("fromPermCode", &NPerm<4>::fromPermCode)
        .def("fromPermCode2", &NPerm<4>::fromPermCode2)
        .def("isPermCode", &NPerm<4>::isPermCode)
        .def("isPermCode2", &NPerm<4>::isPermCode2)
        .def(self * self)
        .def("inverse", &NPerm<4>::inverse)
        .def("reverse", &NPerm<4>::reverse)
        .def("sign", &NPerm<4>::sign)
        .def("__getitem__", &NPerm<4>::operator[])
        .def("preImageOf", &NPerm<4>::preImageOf)
        .def("compareWith", &NPerm<4>::compareWith)
        .def("isIdentity", &NPerm<4>::isIdentity)
        .def("atIndex", &NPerm<4>::atIndex)
        .def("index", &NPerm<4>::index)
        .def("rand", &NPerm<4>::rand)
        .def("trunc", &NPerm<4>::trunc)
        .def("trunc2", &NPerm<4>::trunc2)
        .def("trunc3", &NPerm<4>::trunc3)
        .def("S4Index", S4Index_void)
        .def("orderedS4Index", &NPerm<4>::orderedS4Index)
        .def("orderedSnIndex", &NPerm<4>::orderedS4Index)
        .def("extend", &NPerm<4>::extend<2>)
        .def("extend", &NPerm<4>::extend<3>)
        .def(NPerm4_contract<5>())
        .def("__repr__", &NPerm<4>::str)
        .def(regina::python::add_output_basic())
        .def(regina::python::add_eq_operators())
        .staticmethod("fromPermCode")
        .staticmethod("fromPermCode2")
        .staticmethod("isPermCode")
        .staticmethod("isPermCode2")
        .staticmethod("atIndex")
        .staticmethod("rand")
        .staticmethod("extend")
        .staticmethod("contract")
    ;

    s.attr("nPerms") = NPerm<4>::nPerms;
    s.attr("nPerms_1") = NPerm<4>::nPerms_1;

    s.attr("S4") = &NPerm4_S4_arr;
    s.attr("Sn") = &NPerm4_S4_arr;
    s.attr("invS4") = &NPerm4_invS4_arr;
    s.attr("invSn") = &NPerm4_invS4_arr;
    s.attr("orderedS4") = &NPerm4_orderedS4_arr;
    s.attr("orderedSn") = &NPerm4_orderedS4_arr;
    s.attr("S3") = &NPerm4_S3_arr;
    s.attr("Sn_1") = &NPerm4_S3_arr;
    s.attr("orderedS3") = &NPerm4_orderedS3_arr;
    s.attr("S2") = &NPerm4_S2_arr;
}

