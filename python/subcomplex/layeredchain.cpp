
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "subcomplex/layeredchain.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/layeredchain.h"

using regina::LayeredChain;

void addLayeredChain(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(LayeredChain)

    auto c = pybind11::class_<LayeredChain, regina::StandardTriangulation>
            (m, "LayeredChain", rdoc_scope)
        .def(pybind11::init<regina::Tetrahedron<3>*, regina::Perm<4>>(),
            rdoc::__init)
        .def(pybind11::init<const LayeredChain&>(), rdoc::__copy)
        .def("swap", &LayeredChain::swap, rdoc::swap)
        .def("bottom", &LayeredChain::bottom,
            pybind11::return_value_policy::reference, rdoc::bottom)
        .def("top", &LayeredChain::top,
            pybind11::return_value_policy::reference, rdoc::top)
        .def("index", &LayeredChain::index, rdoc::index)
        .def("bottomVertexRoles", &LayeredChain::bottomVertexRoles,
            rdoc::bottomVertexRoles)
        .def("topVertexRoles", &LayeredChain::topVertexRoles,
            rdoc::topVertexRoles)
        .def("extendAbove", &LayeredChain::extendAbove, rdoc::extendAbove)
        .def("extendBelow", &LayeredChain::extendBelow, rdoc::extendBelow)
        .def("extendMaximal", &LayeredChain::extendMaximal, rdoc::extendMaximal)
        .def("reverse", &LayeredChain::reverse, rdoc::reverse)
        .def("invert", &LayeredChain::invert, rdoc::invert)
    ;
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_output(c);

    regina::python::add_global_swap<LayeredChain>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

