
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

template <int dim> requires (dim == 3)
void addManifoldDim(pybind11::module_& m, const char* name) {
    using Manifold = regina::Manifold<dim>;
    using Options = regina::ManifoldOptions<dim>;

    RDOC_SCOPE_BEGIN(Manifold)

    auto c = pybind11::class_<Manifold, Options>(m, name, rdoc::__class)
        .def("name", &Manifold::name, rdoc::name)
        .def("texName", &Manifold::texName, rdoc::texName)
        .def("structure", &Manifold::structure, rdoc::structure)
        .def("construct", &Manifold::construct, rdoc::construct)
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    // Also leave the equality operators for subclasses to wrap, since each
    // subclass of Manifold provides its own custom == and != operators.
    regina::python::no_eq_abstract(c);

    if constexpr (dim == 3) {
        // We cannot bind the comparison operators in the normal way:
        // see https://github.com/pybind/pybind11/issues/1487 for details.
        //
        // Also: I believe these operators need to be placed in the Manifold3
        // class for Python to find them (as opposed to C++ where they are
        // global).
        c
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
            }, rdoc::__cmp);
    }

    RDOC_SCOPE_END
}

void addManifold(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(ManifoldOptions3)

    // 3-D gets a specialised implementation.
    using Options = regina::ManifoldOptions<3>;
    auto c = pybind11::class_<Options>(m, "ManifoldOptions3", rdoc::__class)
        .def("homology", &Options::homology, rdoc::homology)
        .def("isHyperbolic", &Options::isHyperbolic, rdoc::isHyperbolic)
    ;
    regina::python::no_eq_abstract(c);

    RDOC_SCOPE_SWITCH(ManifoldOptions)

    regina::python::add_doc_only_class<rdoc>(m, "ManifoldOptions");

    RDOC_SCOPE_END

    addManifoldDim<3>(m, "Manifold3");

    // Deprecated type aliases:
    m.attr("Manifold") = m.attr("Manifold3");
}

