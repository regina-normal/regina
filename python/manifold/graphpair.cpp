
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "manifold/graphpair.h"
#include "manifold/sfs.h"
#include "../helpers.h"

using regina::GraphPair;
using regina::Matrix2;
using regina::SFSpace;

void addGraphPair(pybind11::module_& m) {
    auto c = pybind11::class_<GraphPair, regina::Manifold>(m, "GraphPair")
        .def(pybind11::init<const SFSpace&, const SFSpace&,
            long, long, long, long>())
        .def(pybind11::init<const SFSpace&, const SFSpace&, const Matrix2&>())
        .def(pybind11::init<const GraphPair&>())
        .def("sfs", &GraphPair::sfs,
            pybind11::return_value_policy::reference_internal)
        .def("matchingReln", &GraphPair::matchingReln,
            pybind11::return_value_policy::reference_internal)
        .def("swap", &GraphPair::swap)
    ;
    regina::python::add_eq_operators(c);
    regina::python::add_output(c);

    m.def("swap", (void(*)(GraphPair&, GraphPair&))(regina::swap));
}

