
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
    GlobalArray<NPerm<3>> NPerm3_S3_arr(NPerm<3>::S3, 6);
    GlobalArray<NPerm<3>> NPerm3_orderedS3_arr(NPerm<3>::orderedS3, 6);
    GlobalArray<unsigned> NPerm3_invS3_arr(NPerm<3>::invS3, 6);
    GlobalArray<NPerm<3>> NPerm3_S2_arr(NPerm<3>::S2, 2);

    template <int k>
    struct NPerm3_contract : boost::python::def_visitor<NPerm3_contract<k>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &NPerm<3>::contract<k>);
            c.def(NPerm3_contract<k+1>());
        }
    };

    template <>
    struct NPerm3_contract<16> :
            boost::python::def_visitor<NPerm3_contract<16>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &NPerm<3>::contract<16>);
        }
    };
}

void addNPerm3() {
    {
        scope s = class_<NPerm<3>>("Perm3")
            .def(init<int, int>())
            .def(init<int, int, int>())
            .def(init<const NPerm<3>&>())
            .def("permCode", &NPerm<3>::permCode)
            .def("setPermCode", &NPerm<3>::setPermCode)
            .def("fromPermCode", &NPerm<3>::fromPermCode)
            .def("isPermCode", &NPerm<3>::isPermCode)
            .def(self * self)
            .def("inverse", &NPerm<3>::inverse)
            .def("reverse", &NPerm<3>::reverse)
            .def("sign", &NPerm<3>::sign)
            .def("__getitem__", &NPerm<3>::operator[])
            .def("preImageOf", &NPerm<3>::preImageOf)
            .def("compareWith", &NPerm<3>::compareWith)
            .def("isIdentity", &NPerm<3>::isIdentity)
            .def("atIndex", &NPerm<3>::atIndex)
            .def("index", &NPerm<3>::index)
            .def("rand", &NPerm<3>::rand)
            .def("trunc", &NPerm<3>::trunc)
            .def("trunc2", &NPerm<3>::trunc2)
            .def("S3Index", &NPerm<3>::S3Index)
            .def("orderedS3Index", &NPerm<3>::orderedS3Index)
            .def("orderedSnIndex", &NPerm<3>::orderedS3Index)
            .def("extend", &NPerm<3>::extend<2>)
            .def(NPerm3_contract<4>())
            .def("__repr__", &NPerm<3>::str)
            .def(regina::python::add_output_basic())
            .def(regina::python::add_eq_operators())
            .staticmethod("fromPermCode")
            .staticmethod("isPermCode")
            .staticmethod("atIndex")
            .staticmethod("rand")
            .staticmethod("extend")
            .staticmethod("contract")
        ;

        s.attr("nPerms") = NPerm<3>::nPerms;
        s.attr("nPerms_1") = NPerm<3>::nPerms_1;

        s.attr("S3") = &NPerm3_S3_arr;
        s.attr("Sn") = &NPerm3_S3_arr;
        s.attr("orderedS3") = &NPerm3_orderedS3_arr;
        s.attr("orderedSn") = &NPerm3_orderedS3_arr;
        s.attr("invS3") = &NPerm3_invS3_arr;
        s.attr("invSn") = &NPerm3_invS3_arr;
        s.attr("S2") = &NPerm3_S2_arr;
        s.attr("Sn_1") = &NPerm3_S2_arr;
    }

    scope().attr("NPerm3") = scope().attr("Perm3");
}

