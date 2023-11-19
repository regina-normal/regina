
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

#include "../pybind11/pybind11.h"
#include "../pybind11/operators.h"
#include "../pybind11/stl.h"
#include "maths/3d.h"
#include "../helpers.h"
#include "../docstrings/maths/3d.h"

using regina::Rotation3D;
using regina::Vector3D;

void add3D(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Vector3D)

    auto v = pybind11::class_<Vector3D<double>>(m, "Vector3D", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Vector3D<double>&>(), rdoc::__copy)
        .def(pybind11::init<double, double, double>(), rdoc::__init)
        .def(pybind11::init<std::array<double, 3>>(), rdoc::__init_2)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def(pybind11::self - pybind11::self, rdoc::__sub)
        .def(pybind11::self * double(), rdoc::__mul)
        .def(pybind11::self / double(), rdoc::__div)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= double(), rdoc::__imul)
        .def(pybind11::self /= double(), rdoc::__idiv)
        .def("length", &Vector3D<double>::length, rdoc::length)
        .def("distance", &Vector3D<double>::distance, rdoc::distance)
        .def("midpoint", &Vector3D<double>::midpoint, rdoc::midpoint)
        .def_readwrite("x", &Vector3D<double>::x, rdoc::x)
        .def_readwrite("y", &Vector3D<double>::y, rdoc::y)
        .def_readwrite("z", &Vector3D<double>::z, rdoc::z)
    ;
    regina::python::add_output_ostream(v);
    regina::python::add_eq_operators(v, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_SWITCH(Rotation3D)

    auto r = pybind11::class_<Rotation3D<double>>(m, "Rotation3D", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Rotation3D<double>&>(), rdoc::__copy)
        .def(pybind11::init<double, double, double, double>(), rdoc::__init)
        .def("__getitem__", [](const Rotation3D<double>& r, int i) -> double {
            return r[i];
        }, rdoc::__array)
    ;
    regina::python::add_output_ostream(r);
    regina::python::add_eq_operators(r, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_END
}
