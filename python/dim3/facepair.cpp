
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
#include "triangulation/facepair.h"
#include "../helpers.h"
#include "../python/docstrings/triangulation/facepair.h"

using regina::FacePair;

void addFacePair(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FacePair)

    auto c = pybind11::class_<FacePair>(m, "FacePair", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<int, int>(), rdoc::__init)
        .def(pybind11::init<const FacePair&>(), rdoc::__copy)
        .def("lower", &FacePair::lower, rdoc::lower)
        .def("upper", &FacePair::upper, rdoc::upper)
        .def("isBeforeStart", &FacePair::isBeforeStart, rdoc::isBeforeStart)
        .def("isPastEnd", &FacePair::isPastEnd, rdoc::isPastEnd)
        .def("complement", &FacePair::complement, rdoc::complement)
        .def("inc", [](FacePair& p) {
            return p++;
        }, rdoc::__inc)
        .def("dec", [](FacePair& p) {
            return p--;
        }, rdoc::__dec)
        .def("commonEdge", &FacePair::commonEdge, rdoc::commonEdge)
        .def("oppositeEdge", &FacePair::oppositeEdge, rdoc::oppositeEdge)
    ;
    regina::python::add_output_ostream(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    RDOC_SCOPE_END
}

