
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
#include <pybind11/stl.h>
#include "manifold/graphloop.h"
#include "manifold/sfs.h"
#include "../helpers.h"
#include "../docstrings/manifold/graphloop.h"

using regina::GraphLoop;
using regina::Matrix2;
using regina::SFSpace;

void addGraphLoop(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(GraphLoop)

    auto c = pybind11::class_<GraphLoop, regina::Manifold>(m, "GraphLoop",
            rdoc_scope)
        .def(pybind11::init<const SFSpace&, long, long, long, long>(),
            rdoc::__init)
        .def(pybind11::init<const SFSpace&, const Matrix2&>(), rdoc::__init_3)
        .def(pybind11::init<const GraphLoop&>(), rdoc::__copy)
        .def("sfs", &GraphLoop::sfs,
            pybind11::return_value_policy::reference_internal, rdoc::sfs)
        .def("matchingReln", &GraphLoop::matchingReln,
            pybind11::return_value_policy::reference_internal,
            rdoc::matchingReln)
        .def("swap", &GraphLoop::swap, rdoc::swap)
        // Do not bind <, since this is already inherited from Manifold
        // and we do not want to hide that more general version.
    ;
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);
    regina::python::add_output(c);

    regina::python::add_global_swap<GraphLoop>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

