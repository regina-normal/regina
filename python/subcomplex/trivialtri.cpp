
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
#include "subcomplex/trivialtri.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/trivialtri.h"

using regina::TrivialTri;

void addTrivialTri(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(TrivialTri)

    auto c = pybind11::class_<TrivialTri, regina::StandardTriangulation>(
            m, "TrivialTri", rdoc_scope)
        .def(pybind11::init<const TrivialTri&>(), rdoc::__copy)
        .def("swap", &TrivialTri::swap, rdoc::swap)
        .def("type", &TrivialTri::type, rdoc::type)
        .def_static("recognise", &TrivialTri::recognise, rdoc::recognise)
        .def_readonly_static("SPHERE_4_VERTEX", &TrivialTri::SPHERE_4_VERTEX)
        .def_readonly_static("BALL_3_VERTEX", &TrivialTri::BALL_3_VERTEX)
        .def_readonly_static("BALL_4_VERTEX", &TrivialTri::BALL_4_VERTEX)
        .def_readonly_static("L31_PILLOW", &TrivialTri::L31_PILLOW)
        .def_readonly_static("N2", &TrivialTri::N2)
        .def_readonly_static("N3_1", &TrivialTri::N3_1)
        .def_readonly_static("N3_2", &TrivialTri::N3_2)
    ;
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_output(c);

    regina::python::add_global_swap<TrivialTri>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

