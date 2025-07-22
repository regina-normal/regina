
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
#include <pybind11/stl.h>
#include "algebra/abeliangroup.h"
#include "manifold/manifold.h"
#include "subcomplex/standardtri.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/subcomplex/standardtri.h"

using pybind11::overload_cast;
using regina::StandardTriangulation;

void addStandardTriangulation(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(StandardTriangulation)

    auto c = pybind11::class_<StandardTriangulation>(m, "StandardTriangulation",
            rdoc_scope)
        .def("name", &StandardTriangulation::name, rdoc::name)
        .def("texName", &StandardTriangulation::texName, rdoc::texName)
        .def("manifold", &StandardTriangulation::manifold, rdoc::manifold)
        .def("homology", &StandardTriangulation::homology, rdoc::homology)
        .def_static("recognise",
            overload_cast<regina::Component<3>*>(
            &StandardTriangulation::recognise), rdoc::recognise)
        .def_static("recognise",
            overload_cast<const regina::Triangulation<3>&>(
            &StandardTriangulation::recognise), rdoc::recognise_2)
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    // Also leave the equality operators for subclasses to wrap, since
    // each subclass of StandardTriangulation provides its own custom
    // == and != operators.
    regina::python::no_eq_abstract(c);

    RDOC_SCOPE_END
}

