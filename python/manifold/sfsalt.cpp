
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
#include "manifold/sfsalt.h"
#include "../helpers.h"
#include "../docstrings/manifold/sfsalt.h"

using pybind11::overload_cast;
using regina::SFSAlt;
using regina::SFSpace;

void addSFSAlt(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SFSAlt)

    auto s = pybind11::class_<SFSAlt>(m, "SFSAlt", rdoc_scope)
        .def(pybind11::init<const SFSpace&>(), rdoc::__init)
        .def(pybind11::init<const SFSAlt&, bool, bool>(),
            pybind11::arg(), pybind11::arg(), pybind11::arg("negate") = false,
            rdoc::__init_2)
        .def(pybind11::init<const SFSAlt&>(), rdoc::__copy)
        .def("swap", &SFSAlt::swap, rdoc::swap)
        .def_static("altSet", &SFSAlt::altSet, rdoc::altSet)
        .def_static("canNegate", &SFSAlt::canNegate, rdoc::canNegate)
        .def("alt", static_cast<const SFSpace& (SFSAlt::*)() const&>(
            &SFSAlt::alt), rdoc::alt)
        .def("conversion", &SFSAlt::conversion, rdoc::conversion)
        .def("reflected", &SFSAlt::reflected, rdoc::reflected)
    ;
    regina::python::add_output(s);
    regina::python::add_eq_operators(s, rdoc::__eq);

    regina::python::add_global_swap<SFSAlt>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

