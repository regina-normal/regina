
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
#include "subcomplex/snappedball.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/snappedball.h"

using regina::SnappedBall;

void addSnappedBall(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SnappedBall)

    auto c = pybind11::class_<SnappedBall, regina::StandardTriangulation>
            (m, "SnappedBall", rdoc_scope)
        .def(pybind11::init<const SnappedBall&>(), rdoc::__copy)
        .def("swap", &SnappedBall::swap, rdoc::swap)
        .def("tetrahedron", &SnappedBall::tetrahedron,
            pybind11::return_value_policy::reference, rdoc::tetrahedron)
        .def("boundaryFace", &SnappedBall::boundaryFace, rdoc::boundaryFace)
        .def("internalFace", &SnappedBall::internalFace, rdoc::internalFace)
        .def("equatorEdge", &SnappedBall::equatorEdge, rdoc::equatorEdge)
        .def("internalEdge", &SnappedBall::internalEdge, rdoc::internalEdge)
        .def_static("recognise", &SnappedBall::recognise, rdoc::recognise)
    ;
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_output(c);

    regina::python::add_global_swap<SnappedBall>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

