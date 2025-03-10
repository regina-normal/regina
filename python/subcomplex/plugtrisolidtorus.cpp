
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
#include <pybind11/stl.h>
#include "subcomplex/plugtrisolidtorus.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/plugtrisolidtorus.h"

using regina::PlugTriSolidTorus;

void addPlugTriSolidTorus(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(PlugTriSolidTorus)

    auto c = pybind11::class_<PlugTriSolidTorus, regina::StandardTriangulation>
            (m, "PlugTriSolidTorus", rdoc_scope)
        .def(pybind11::init<const PlugTriSolidTorus&>(), rdoc::__copy)
        .def("swap", &PlugTriSolidTorus::swap, rdoc::swap)
        .def("core", &PlugTriSolidTorus::core,
            pybind11::return_value_policy::reference_internal, rdoc::core)
        .def("chain", &PlugTriSolidTorus::chain,
            pybind11::return_value_policy::reference_internal, rdoc::chain)
        .def("chainType", &PlugTriSolidTorus::chainType, rdoc::chainType)
        .def("equatorType", &PlugTriSolidTorus::equatorType, rdoc::equatorType)
        .def_static("recognise", &PlugTriSolidTorus::recognise, rdoc::recognise)
        .def_readonly_static("CHAIN_NONE", &PlugTriSolidTorus::CHAIN_NONE)
        .def_readonly_static("CHAIN_MAJOR", &PlugTriSolidTorus::CHAIN_MAJOR)
        .def_readonly_static("CHAIN_MINOR", &PlugTriSolidTorus::CHAIN_MINOR)
        .def_readonly_static("EQUATOR_MAJOR", &PlugTriSolidTorus::EQUATOR_MAJOR)
        .def_readonly_static("EQUATOR_MINOR", &PlugTriSolidTorus::EQUATOR_MINOR)
    ;
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_output(c);

    regina::python::add_global_swap<PlugTriSolidTorus>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

