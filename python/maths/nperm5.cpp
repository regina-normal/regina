
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
#include "maths/nperm5.h"
#include "../globalarray.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NPerm5;
using regina::python::GlobalArray;

namespace {
    GlobalArray<NPerm5> NPerm5_S5_arr(NPerm5::S5, 120);
    GlobalArray<NPerm5> NPerm5_orderedS5_arr(NPerm5::orderedS5, 120);
    GlobalArray<unsigned> NPerm5_invS5_arr(NPerm5::invS5, 120);
    GlobalArray<NPerm5> NPerm5_S4_arr(NPerm5::S4, 24);
    GlobalArray<NPerm5> NPerm5_orderedS4_arr(NPerm5::orderedS4, 24);
    GlobalArray<NPerm5> NPerm5_S3_arr(NPerm5::S3, 6);
    GlobalArray<NPerm5> NPerm5_orderedS3_arr(NPerm5::orderedS3, 6);
    GlobalArray<NPerm5> NPerm5_S2_arr(NPerm5::S2, 2);

    template <int k>
    struct NPerm5_contract : boost::python::def_visitor<NPerm5_contract<k>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &NPerm5::contract<k>);
            c.def(NPerm5_contract<k+1>());
        }
    };

    template <>
    struct NPerm5_contract<16> :
            boost::python::def_visitor<NPerm5_contract<16>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &NPerm5::contract<16>);
        }
    };
}

void addNPerm5() {
    scope s = class_<NPerm5>("NPerm5")
        .def(init<int, int>())
        .def(init<int, int, int, int, int>())
        .def(init<int, int, int, int, int, int, int, int, int, int>())
        .def(init<const NPerm5&>())
        .def("permCode", &NPerm5::permCode)
        .def("setPermCode", &NPerm5::setPermCode)
        .def("fromPermCode", &NPerm5::fromPermCode)
        .def("isPermCode", &NPerm5::isPermCode)
        .def(self * self)
        .def("inverse", &NPerm5::inverse)
        .def("reverse", &NPerm5::reverse)
        .def("sign", &NPerm5::sign)
        .def("__getitem__", &NPerm5::operator[])
        .def("preImageOf", &NPerm5::preImageOf)
        .def("compareWith", &NPerm5::compareWith)
        .def("isIdentity", &NPerm5::isIdentity)
        .def("atIndex", &NPerm5::atIndex)
        .def("index", &NPerm5::index)
        .def("rand", &NPerm5::rand)
        .def("trunc", &NPerm5::trunc)
        .def("trunc2", &NPerm5::trunc2)
        .def("trunc3", &NPerm5::trunc3)
        .def("trunc4", &NPerm5::trunc4)
        .def("S5Index", &NPerm5::S5Index)
        .def("orderedS5Index", &NPerm5::orderedS5Index)
        .def("orderedSnIndex", &NPerm5::orderedS5Index)
        .def("extend", &NPerm5::extend<2>)
        .def("extend", &NPerm5::extend<3>)
        .def("extend", &NPerm5::extend<4>)
        .def(NPerm5_contract<6>())
        .def("__repr__", &NPerm5::str)
        .def(regina::python::add_output_basic())
        .def(regina::python::add_eq_operators())
        .staticmethod("fromPermCode")
        .staticmethod("isPermCode")
        .staticmethod("atIndex")
        .staticmethod("rand")
        .staticmethod("extend")
        .staticmethod("contract")
    ;

    s.attr("imageBits") = NPerm5::imageBits;
    s.attr("nPerms") = NPerm5::nPerms;
    s.attr("nPerms_1") = NPerm5::nPerms_1;

    s.attr("S5") = &NPerm5_S5_arr;
    s.attr("Sn") = &NPerm5_S5_arr;
    s.attr("orderedS5") = &NPerm5_orderedS5_arr;
    s.attr("orderedSn") = &NPerm5_orderedS5_arr;
    s.attr("invS5") = &NPerm5_invS5_arr;
    s.attr("invSn") = &NPerm5_invS5_arr;
    s.attr("S4") = &NPerm5_S4_arr;
    s.attr("Sn_1") = &NPerm5_S4_arr;
    s.attr("orderedS4") = &NPerm5_orderedS4_arr;
    s.attr("S3") = &NPerm5_S3_arr;
    s.attr("orderedS3") = &NPerm5_orderedS3_arr;
    s.attr("S2") = &NPerm5_S2_arr;
}

