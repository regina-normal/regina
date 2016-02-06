
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

#include <boost/python.hpp>
#include "surfaces/normalflags.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NormalAlg;
using regina::NormalList;

namespace {
    bool (NormalList::*has_list)(const NormalList&) const = &NormalList::has;
    bool (NormalAlg::*has_alg)(const NormalAlg&) const = &NormalAlg::has;
    void (NormalList::*clear_list)(const NormalList&) = &NormalList::clear;
    void (NormalAlg::*clear_alg)(const NormalAlg&) = &NormalAlg::clear;
}

void addNormalFlags() {
    scope global;

    class_<NormalList>("NormalList")
        .def(init<const NormalList&>())
        .def("has", has_list)
        .def("intValue", &NormalList::intValue)
        .def("fromInt", &NormalList::fromInt)
        .def(self |= self)
        .def(self &= self)
        .def(self ^= self)
        .def(self | self)
        .def(self & self)
        .def(self ^ self)
        .def("clear", clear_list)
        .def(regina::python::add_eq_operators())
        .staticmethod("fromInt")
        ;

    global.attr("NS_EMBEDDED_ONLY") = NormalList(regina::NS_EMBEDDED_ONLY);
    global.attr("NS_IMMERSED_SINGULAR") =
        NormalList(regina::NS_IMMERSED_SINGULAR);
    global.attr("NS_VERTEX") = NormalList(regina::NS_VERTEX);
    global.attr("NS_FUNDAMENTAL") = NormalList(regina::NS_FUNDAMENTAL);
    global.attr("NS_LEGACY") = NormalList(regina::NS_LEGACY);
    global.attr("NS_CUSTOM") = NormalList(regina::NS_CUSTOM);

    class_<NormalAlg>("NormalAlg")
        .def(init<const NormalAlg&>())
        .def("has", has_alg)
        .def("intValue", &NormalAlg::intValue)
        .def("fromInt", &NormalAlg::fromInt)
        .def(self |= self)
        .def(self &= self)
        .def(self ^= self)
        .def(self | self)
        .def(self & self)
        .def(self ^ self)
        .def("clear", clear_alg)
        .def(regina::python::add_eq_operators())
        .staticmethod("fromInt")
        ;

    global.attr("NS_ALG_DEFAULT") = NormalAlg(regina::NS_ALG_DEFAULT);
    global.attr("NS_VERTEX_VIA_REDUCED") =
        NormalAlg(regina::NS_VERTEX_VIA_REDUCED);
    global.attr("NS_VERTEX_STD_DIRECT") =
        NormalAlg(regina::NS_VERTEX_STD_DIRECT);
    global.attr("NS_VERTEX_TREE") = NormalAlg(regina::NS_VERTEX_TREE);
    global.attr("NS_VERTEX_DD") = NormalAlg(regina::NS_VERTEX_DD);
    global.attr("NS_HILBERT_PRIMAL") = NormalAlg(regina::NS_HILBERT_PRIMAL);
    global.attr("NS_HILBERT_DUAL") = NormalAlg(regina::NS_HILBERT_DUAL);
    global.attr("NS_HILBERT_CD") = NormalAlg(regina::NS_HILBERT_CD);
    global.attr("NS_HILBERT_FULLCONE") = NormalAlg(regina::NS_HILBERT_FULLCONE);
    global.attr("NS_ALG_LEGACY") = NormalAlg(regina::NS_ALG_LEGACY);
    global.attr("NS_ALG_CUSTOM") = NormalAlg(regina::NS_ALG_CUSTOM);

}
