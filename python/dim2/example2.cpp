
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
#include "triangulation/example2.h"
#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../docstrings/triangulation/example2.h"
#include "../docstrings/triangulation/detail/example.h"

using regina::Example;

void addExample2(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Example)
    RDOC_SCOPE_BASE(detail::ExampleBase)

    auto c = pybind11::class_<Example<2>>(m, "Example2", rdoc_scope)
        .def_static("sphere", &Example<2>::sphere, rbase::sphere)
        .def_static("simplicialSphere", &Example<2>::simplicialSphere,
            rbase::simplicialSphere)
        .def_static("sphereBundle", &Example<2>::sphereBundle,
            rbase::sphereBundle)
        .def_static("twistedSphereBundle", &Example<2>::twistedSphereBundle,
            rbase::twistedSphereBundle)
        .def_static("ball", &Example<2>::ball, rbase::ball)
        .def_static("ballBundle", &Example<2>::ballBundle, rbase::ballBundle)
        .def_static("twistedBallBundle", &Example<2>::twistedBallBundle,
            rbase::twistedBallBundle)
        .def_static("orientable", &Example<2>::orientable, rdoc::orientable)
        .def_static("nonOrientable", &Example<2>::nonOrientable,
            rdoc::nonOrientable)
        .def_static("sphereTetrahedron", &Example<2>::sphereTetrahedron,
            rdoc::sphereTetrahedron)
        .def_static("sphereOctahedron", &Example<2>::sphereOctahedron,
            rdoc::sphereOctahedron)
        .def_static("disc", &Example<2>::disc, rdoc::disc)
        .def_static("annulus", &Example<2>::annulus, rdoc::annulus)
        .def_static("mobius", &Example<2>::mobius, rdoc::mobius)
        .def_static("torus", &Example<2>::torus, rdoc::torus)
        .def_static("rp2", &Example<2>::rp2, rdoc::rp2)
        .def_static("kb", &Example<2>::kb, rdoc::kb)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END
}

