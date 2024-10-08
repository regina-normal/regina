
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
#include "algebra/abeliangroup.h"
#include "manifold/torusbundle.h"
#include "../helpers.h"
#include "../docstrings/manifold/torusbundle.h"

using regina::Matrix2;
using regina::TorusBundle;

void addTorusBundle(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(TorusBundle)

    auto c = pybind11::class_<TorusBundle, regina::Manifold>(m, "TorusBundle",
            rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Matrix2&>(), rdoc::__init)
        .def(pybind11::init<long, long, long, long>(), rdoc::__init_2)
        .def(pybind11::init<const TorusBundle&>(), rdoc::__copy)
        .def("swap", &TorusBundle::swap, rdoc::swap)
        .def("monodromy", &TorusBundle::monodromy,
            pybind11::return_value_policy::reference_internal, rdoc::monodromy)
    ;
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_output(c);

    regina::python::add_global_swap<TorusBundle>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

