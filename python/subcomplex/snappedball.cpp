
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "../pybind11/pybind11.h"
#include "../pybind11/stl.h"
#include "subcomplex/snappedball.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::SnappedBall;

void addSnappedBall(pybind11::module_& m) {
    auto c = pybind11::class_<SnappedBall, regina::StandardTriangulation>
            (m, "SnappedBall")
        .def(pybind11::init<const SnappedBall&>())
        .def("clone", [](const SnappedBall& s) { // deprecated
            return SnappedBall(s);
        })
        .def("swap", &SnappedBall::swap)
        .def("tetrahedron", &SnappedBall::tetrahedron,
            pybind11::return_value_policy::reference)
        .def("boundaryFace", &SnappedBall::boundaryFace)
        .def("internalFace", &SnappedBall::internalFace)
        .def("equatorEdge", &SnappedBall::equatorEdge)
        .def("internalEdge", &SnappedBall::internalEdge)
        .def_static("recognise", &SnappedBall::recognise)
        .def_static("formsSnappedBall", &SnappedBall::recognise) // deprecated
    ;
    regina::python::add_output(c);

    m.def("swap", (void(*)(SnappedBall&, SnappedBall&))(regina::swap));
}

