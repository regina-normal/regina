
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
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "algebra/abeliangroup.h"
#include "manifold/manifold.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/manifold/manifold.h"

using regina::Manifold;

void addManifold(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Manifold)

    auto c = pybind11::class_<Manifold>(m, "Manifold", rdoc_scope)
        .def("name", &Manifold::name, rdoc::name)
        .def("texName", &Manifold::texName, rdoc::texName)
        .def("structure", &Manifold::structure, rdoc::structure)
        .def("construct", &Manifold::construct, rdoc::construct)
        .def("homology", &Manifold::homology, rdoc::homology)
        .def("isHyperbolic", &Manifold::isHyperbolic, rdoc::isHyperbolic)
        // We cannot bind the comparison operators in the normal way:
        // see https://github.com/pybind/pybind11/issues/1487 for details.
        .def("__lt__", [](const Manifold& lhs, const Manifold& rhs) {
            return lhs < rhs;
        }, rdoc::__cmp)
        .def("__le__", [](const Manifold& lhs, const Manifold& rhs) {
            return lhs <= rhs;
        }, rdoc::__cmp)
        .def("__gt__", [](const Manifold& lhs, const Manifold& rhs) {
            return lhs > rhs;
        }, rdoc::__cmp)
        .def("__ge__", [](const Manifold& lhs, const Manifold& rhs) {
            return lhs >= rhs;
        }, rdoc::__cmp)
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    // Also leave the equality operators for subclasses to wrap, since
    // each subclass of Manifold provides its own custom == and != operators.
    regina::python::no_eq_abstract(c);

    RDOC_SCOPE_END
}

