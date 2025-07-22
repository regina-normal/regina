
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
#include "subcomplex/layeredsolidtorus.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/layeredsolidtorus.h"

using regina::LayeredSolidTorus;

void addLayeredSolidTorus(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(LayeredSolidTorus)

    auto c = pybind11::class_<LayeredSolidTorus, regina::StandardTriangulation>
            (m, "LayeredSolidTorus", rdoc_scope)
        .def(pybind11::init<const LayeredSolidTorus&>(), rdoc::__copy)
        .def("swap", &LayeredSolidTorus::swap, rdoc::swap)
        .def("size", &LayeredSolidTorus::size, rdoc::size)
        .def("base", &LayeredSolidTorus::base,
            pybind11::return_value_policy::reference, rdoc::base)
        .def("baseEdge", &LayeredSolidTorus::baseEdge, rdoc::baseEdge)
        .def("baseEdgeGroup", &LayeredSolidTorus::baseEdgeGroup,
            rdoc::baseEdgeGroup)
        .def("baseFace", &LayeredSolidTorus::baseFace, rdoc::baseFace)
        .def("topLevel", &LayeredSolidTorus::topLevel,
            pybind11::return_value_policy::reference, rdoc::topLevel)
        .def("meridinalCuts", &LayeredSolidTorus::meridinalCuts,
            rdoc::meridinalCuts)
        .def("topEdge", &LayeredSolidTorus::topEdge, rdoc::topEdge)
        .def("topEdgeGroup", &LayeredSolidTorus::topEdgeGroup,
            rdoc::topEdgeGroup)
        .def("topFace", &LayeredSolidTorus::topFace, rdoc::topFace)
        .def("flatten", &LayeredSolidTorus::flatten, rdoc::flatten)
        .def("transform", &LayeredSolidTorus::transform, rdoc::transform)
        .def_static("recogniseFromBase", &LayeredSolidTorus::recogniseFromBase,
            rdoc::recogniseFromBase)
        .def_static("recogniseFromTop", &LayeredSolidTorus::recogniseFromTop,
            rdoc::recogniseFromTop)
        .def_static("recognise", &LayeredSolidTorus::recognise, rdoc::recognise)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    regina::python::add_global_swap<LayeredSolidTorus>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

