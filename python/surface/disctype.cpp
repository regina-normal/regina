
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
#include "surface/disctype.h"
#include "../helpers.h"
#include "../docstrings/surface/disctype.h"

using regina::DiscType;

void addDiscType(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(DiscType)

    auto c = pybind11::class_<DiscType>(m, "DiscType", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<size_t, int>(), rdoc::__init)
        .def(pybind11::init<const DiscType&>(), rdoc::__copy)
        .def("__bool__", &DiscType::operator bool, rdoc::__as_bool)
        .def_readwrite("tetIndex", &DiscType::tetIndex)
        .def_readwrite("type", &DiscType::type)
    ;
    regina::python::add_output_ostream(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    RDOC_SCOPE_END
}

