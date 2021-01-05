
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
#include "triangulation/example.h"
#include "triangulation/generic.h"
#include "../helpers.h"

using regina::Example;

template <int dim>
void addExample(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<Example<dim>>(m, name)
        .def_static("sphere", &Example<dim>::sphere)
        .def_static("simplicialSphere", &Example<dim>::simplicialSphere)
        .def_static("sphereBundle", &Example<dim>::sphereBundle)
        .def_static("twistedSphereBundle", &Example<dim>::twistedSphereBundle)
        .def_static("ball", &Example<dim>::ball)
        .def_static("ballBundle", &Example<dim>::ballBundle)
        .def_static("twistedBallBundle", &Example<dim>::twistedBallBundle)
        .def_static("doubleCone", &Example<dim>::doubleCone)
        .def_static("singleCone", &Example<dim>::singleCone)
    ;
    regina::python::no_eq_operators(c);
}

