
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
#include <pybind11/stl.h>
#include "algebra/abeliangroup.h"
#include "manifold/manifold.h"
#include "subcomplex/standardsubcomplex.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/standardsubcomplex.h"

using pybind11::overload_cast;

template <int dim> requires (dim == 3 || dim == 4)
void addStandardSubcomplexDim(pybind11::module_& m, const char* name) {
    using Subcomplex = regina::StandardSubcomplex<dim>;

    RDOC_SCOPE_BEGIN(StandardSubcomplex)

    auto c = pybind11::class_<Subcomplex>(m, name, rdoc::__class)
        .def("name", &Subcomplex::name, rdoc::name)
        .def("texName", &Subcomplex::texName, rdoc::texName)
        .def("manifold", &Subcomplex::manifold, rdoc::manifold)
        .def("homology", &Subcomplex::homology, rdoc::homology)
        .def_static("recognise", overload_cast<regina::Component<dim>*>(
            &Subcomplex::recognise), rdoc::recognise)
        .def_static("recognise",
            overload_cast<const regina::Triangulation<dim>&>(
            &Subcomplex::recognise), rdoc::recognise_2)
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    // Also leave the equality operators for subclasses to wrap, since each
    // subclass of StandardSubcomplex provides its own custom ==, != operators.
    regina::python::no_eq_abstract(c);

    RDOC_SCOPE_END
}

void addStandardSubcomplex(pybind11::module_& m) {
    addStandardSubcomplexDim<3>(m, "StandardSubcomplex3");
    addStandardSubcomplexDim<4>(m, "StandardSubcomplex4");

    // Deprecated type aliases:
    m.attr("StandardTriangulation") = m.attr("StandardSubcomplex3");
}

