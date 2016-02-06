
/**************************************************************************
 *                                                                        *
 *  Regina - A Hyper Surface Theory Calculator                           *
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
#include "hypersurface/hyperflags.h"
#include "../helpers.h"

using namespace boost::python;
using regina::HyperAlg;
using regina::HyperList;

namespace {
    bool (HyperList::*has_list)(const HyperList&) const = &HyperList::has;
    bool (HyperAlg::*has_alg)(const HyperAlg&) const = &HyperAlg::has;
    void (HyperList::*clear_list)(const HyperList&) = &HyperList::clear;
    void (HyperAlg::*clear_alg)(const HyperAlg&) = &HyperAlg::clear;
}

void addHyperFlags() {
    scope global;

    class_<HyperList>("HyperList")
        .def(init<const HyperList&>())
        .def("has", has_list)
        .def("intValue", &HyperList::intValue)
        .def("fromInt", &HyperList::fromInt)
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

    global.attr("HS_EMBEDDED_ONLY") = HyperList(regina::HS_EMBEDDED_ONLY);
    global.attr("HS_IMMERSED_SINGULAR") =
        HyperList(regina::HS_IMMERSED_SINGULAR);
    global.attr("HS_VERTEX") = HyperList(regina::HS_VERTEX);
    global.attr("HS_FUNDAMENTAL") = HyperList(regina::HS_FUNDAMENTAL);
    global.attr("HS_LEGACY") = HyperList(regina::HS_LEGACY);
    global.attr("HS_CUSTOM") = HyperList(regina::HS_CUSTOM);

    class_<HyperAlg>("HyperAlg")
        .def(init<const HyperAlg&>())
        .def("has", has_alg)
        .def("intValue", &HyperAlg::intValue)
        .def("fromInt", &HyperAlg::fromInt)
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

    global.attr("HS_ALG_DEFAULT") = HyperAlg(regina::HS_ALG_DEFAULT);
    global.attr("HS_VERTEX_DD") = HyperAlg(regina::HS_VERTEX_DD);
    global.attr("HS_HILBERT_PRIMAL") = HyperAlg(regina::HS_HILBERT_PRIMAL);
    global.attr("HS_HILBERT_DUAL") = HyperAlg(regina::HS_HILBERT_DUAL);
    global.attr("HS_ALG_LEGACY") = HyperAlg(regina::HS_ALG_LEGACY);
    global.attr("HS_ALG_CUSTOM") = HyperAlg(regina::HS_ALG_CUSTOM);

}
