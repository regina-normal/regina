
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
#include "triangulation/example3.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/triangulation/example3.h"
#include "../docstrings/triangulation/detail/example.h"

using regina::Example;

void addExample3(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Example)
    RDOC_SCOPE_BASE_2(detail::ExampleBase, detail::ExampleFromLowDim)

    auto c = pybind11::class_<Example<3>>(m, "Example3", rdoc_scope)
        .def_static("sphere", &Example<3>::sphere, rbase::sphere)
        .def_static("simplicialSphere", &Example<3>::simplicialSphere,
            rbase::simplicialSphere)
        .def_static("sphereBundle", &Example<3>::sphereBundle,
            rbase::sphereBundle)
        .def_static("twistedSphereBundle", &Example<3>::twistedSphereBundle,
            rbase::twistedSphereBundle)
        .def_static("ball", &Example<3>::ball, rbase::ball)
        .def_static("ballBundle", &Example<3>::ballBundle, rbase::ballBundle)
        .def_static("twistedBallBundle", &Example<3>::twistedBallBundle,
            rbase::twistedBallBundle)
        .def_static("doubleCone", &Example<3>::doubleCone, rbase2::doubleCone)
        .def_static("singleCone", &Example<3>::singleCone, rbase2::singleCone)
        .def_static("threeSphere", &Example<3>::threeSphere, rdoc::threeSphere)
        .def_static("bingsHouse", &Example<3>::bingsHouse, rdoc::bingsHouse)
        .def_static("s2xs1", &Example<3>::s2xs1, rdoc::s2xs1)
        .def_static("rp2xs1", &Example<3>::rp2xs1, rdoc::rp2xs1)
        .def_static("rp3rp3", &Example<3>::rp3rp3, rdoc::rp3rp3)
        .def_static("lens", &Example<3>::lens, rdoc::lens)
        .def_static("layeredLoop", &Example<3>::layeredLoop,
            rdoc::layeredLoop)
        .def_static("poincare", &Example<3>::poincare, rdoc::poincare)
        .def_static("augTriSolidTorus", &Example<3>::augTriSolidTorus,
            rdoc::augTriSolidTorus)
        .def_static("sfsOverSphere", &Example<3>::sfsOverSphere,
            pybind11::arg("a1") = 1, pybind11::arg("b1") = 0,
            pybind11::arg("a2") = 1, pybind11::arg("b2") = 0,
            pybind11::arg("a3") = 1, pybind11::arg("b3") = 0,
            rdoc::sfsOverSphere)
        .def_static("weeks", &Example<3>::weeks, rdoc::weeks)
        .def_static("weberSeifert", &Example<3>::weberSeifert,
            rdoc::weberSeifert)
        .def_static("smallClosedOrblHyperbolic",
            &Example<3>::smallClosedOrblHyperbolic,
            rdoc::smallClosedOrblHyperbolic)
        .def_static("smallClosedNonOrblHyperbolic",
            &Example<3>::smallClosedNonOrblHyperbolic,
            rdoc::smallClosedNonOrblHyperbolic)
        .def_static("sphere600", &Example<3>::sphere600, rdoc::sphere600)
        .def_static("lst", &Example<3>::lst, rdoc::lst)
        .def_static("handlebody", &Example<3>::handlebody, rdoc::handlebody)
        .def_static("solidKleinBottle", &Example<3>::solidKleinBottle,
            rdoc::solidKleinBottle)
        .def_static("figureEight", &Example<3>::figureEight, rdoc::figureEight)
        .def_static("trefoil", &Example<3>::trefoil, rdoc::trefoil)
        .def_static("whitehead", &Example<3>::whitehead, rdoc::whitehead)
        .def_static("whiteheadLink", &Example<3>::whitehead, // deprecated
            rdoc::whiteheadLink)
        .def_static("gieseking", &Example<3>::gieseking, rdoc::gieseking)
        .def_static("cuspedGenusTwoTorus",
            &Example<3>::cuspedGenusTwoTorus, rdoc::cuspedGenusTwoTorus)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END
}

