
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
#include "subcomplex/trivialtri.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::TrivialTri;

void addTrivialTri(pybind11::module_& m) {
    pybind11::class_<TrivialTri, regina::StandardTriangulation>(m, "TrivialTri")
        .def("clone", &TrivialTri::clone)
        .def("type", &TrivialTri::type)
        .def_static("isTrivialTriangulation",
            &TrivialTri::isTrivialTriangulation)
        .def_readonly_static("SPHERE_4_VERTEX", &TrivialTri::SPHERE_4_VERTEX)
        .def_readonly_static("BALL_3_VERTEX", &TrivialTri::BALL_3_VERTEX)
        .def_readonly_static("BALL_4_VERTEX", &TrivialTri::BALL_4_VERTEX)
        .def_readonly_static("N2", &TrivialTri::N2)
        .def_readonly_static("N3_1", &TrivialTri::N3_1)
        .def_readonly_static("N3_2", &TrivialTri::N3_2)
    ;

    m.attr("NTrivialTri") = m.attr("TrivialTri");
}

