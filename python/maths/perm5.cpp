
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
    GlobalArray<Perm<5>> Perm5_S5_arr(Perm<5>::S5, 120);
    GlobalArray<Perm<5>> Perm5_orderedS5_arr(Perm<5>::orderedS5, 120);
    GlobalArray<unsigned> Perm5_invS5_arr(Perm<5>::invS5, 120);
    GlobalArray<Perm<5>> Perm5_S4_arr(Perm<5>::S4, 24);
    GlobalArray<Perm<5>> Perm5_orderedS4_arr(Perm<5>::orderedS4, 24);
    GlobalArray<Perm<5>> Perm5_S3_arr(Perm<5>::S3, 6);
    GlobalArray<Perm<5>> Perm5_orderedS3_arr(Perm<5>::orderedS3, 6);
    GlobalArray<Perm<5>> Perm5_S2_arr(Perm<5>::S2, 2);

    template <int k>
    struct Perm5_contract : boost::python::def_visitor<Perm5_contract<k>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &Perm<5>::contract<k>);
            c.def(Perm5_contract<k+1>());
        }
    };

    template <>
    struct Perm5_contract<16> :
            boost::python::def_visitor<Perm5_contract<16>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &Perm<5>::contract<16>);
        }
    };
}

void addPerm5() {
    {
        scope s = class_<Perm<5>>("Perm5")
            .def(init<int, int>())
            .def(init<int, int, int, int, int>())
            .def(init<int, int, int, int, int, int, int, int, int, int>())
            .def(init<const Perm<5>&>())
            .def("permCode", &Perm<5>::permCode)
            .def("setPermCode", &Perm<5>::setPermCode)
            .def("fromPermCode", &Perm<5>::fromPermCode)
            .def("isPermCode", &Perm<5>::isPermCode)
            .def(self * self)
            .def("inverse", &Perm<5>::inverse)
            .def("reverse", &Perm<5>::reverse)
            .def("sign", &Perm<5>::sign)
            .def("__getitem__", &Perm<5>::operator[])
            .def("preImageOf", &Perm<5>::preImageOf)
            .def("compareWith", &Perm<5>::compareWith)
            .def("isIdentity", &Perm<5>::isIdentity)
            .def("atIndex", &Perm<5>::atIndex)
            .def("index", &Perm<5>::index)
            .def("rand", &Perm<5>::rand)
            .def("trunc", &Perm<5>::trunc)
            .def("trunc2", &Perm<5>::trunc2)
            .def("trunc3", &Perm<5>::trunc3)
            .def("trunc4", &Perm<5>::trunc4)
            .def("clear", &Perm<5>::clear)
            .def("S5Index", &Perm<5>::S5Index)
            .def("orderedS5Index", &Perm<5>::orderedS5Index)
            .def("orderedSnIndex", &Perm<5>::orderedS5Index)
            .def("extend", &Perm<5>::extend<2>)
            .def("extend", &Perm<5>::extend<3>)
            .def("extend", &Perm<5>::extend<4>)
            .def(Perm5_contract<6>())
            .def("__repr__", &Perm<5>::str)
            .def(regina::python::add_output_basic())
            .def(regina::python::add_eq_operators())
            .staticmethod("fromPermCode")
            .staticmethod("isPermCode")
            .staticmethod("atIndex")
            .staticmethod("rand")
            .staticmethod("extend")
            .staticmethod("contract")
        ;

        s.attr("imageBits") = Perm<5>::imageBits;
        s.attr("nPerms") = Perm<5>::nPerms;
        s.attr("nPerms_1") = Perm<5>::nPerms_1;

        s.attr("S5") = &Perm5_S5_arr;
        s.attr("Sn") = &Perm5_S5_arr;
        s.attr("orderedS5") = &Perm5_orderedS5_arr;
        s.attr("orderedSn") = &Perm5_orderedS5_arr;
        s.attr("invS5") = &Perm5_invS5_arr;
        s.attr("invSn") = &Perm5_invS5_arr;
        s.attr("S4") = &Perm5_S4_arr;
        s.attr("Sn_1") = &Perm5_S4_arr;
        s.attr("orderedS4") = &Perm5_orderedS4_arr;
        s.attr("S3") = &Perm5_S3_arr;
        s.attr("orderedS3") = &Perm5_orderedS3_arr;
        s.attr("S2") = &Perm5_S2_arr;
    }

    scope().attr("NPerm5") = scope().attr("Perm5");
}

