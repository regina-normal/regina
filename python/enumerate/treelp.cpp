
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "enumerate/treelp.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Integer;
using regina::LPMatrix;
using regina::LPSystem;

void addTreeLP(pybind11::module_& m) {
    auto c = pybind11::class_<LPMatrix<Integer>>(m, "LPMatrix")
        .def(pybind11::init<>())
        .def(pybind11::init<unsigned, unsigned>())
        .def("swap", &LPMatrix<Integer>::swap)
        .def("reserve", &LPMatrix<Integer>::reserve)
        .def("initClone", &LPMatrix<Integer>::initClone)
        .def("initIdentity", &LPMatrix<Integer>::initIdentity)
        .def("entry",
            overload_cast<unsigned, unsigned>(&LPMatrix<Integer>::entry),
            pybind11::return_value_policy::reference_internal)
        .def("set", [](LPMatrix<Integer>& m, unsigned row, unsigned col,
                const regina::Integer& value){
            m.entry(row, col) = value;
        })
        .def("rows", &LPMatrix<Integer>::rows)
        .def("columns", &LPMatrix<Integer>::columns)
        .def("swapRows", &LPMatrix<Integer>::swapRows)
        .def("combRow", &LPMatrix<Integer>::combRow)
        .def("combRowAndNorm", &LPMatrix<Integer>::combRowAndNorm)
        .def("negateRow", &LPMatrix<Integer>::negateRow)
        .def("__str__", [](const LPMatrix<Integer>& m) {
            std::ostringstream out;
            m.dump(out);
            return out.str();
        })
        ;
    regina::python::add_eq_operators(c);

    m.def("swap",
        (void(*)(LPMatrix<Integer>&, LPMatrix<Integer>&))(regina::swap));

    auto s = pybind11::class_<LPSystem>(m, "LPSystem")
        .def(pybind11::init<regina::NormalEncoding>())
        .def(pybind11::init<const LPSystem&>())
        .def("normal", &LPSystem::normal)
        .def("angle", &LPSystem::angle)
        .def("standard", &LPSystem::standard)
        .def("quad", &LPSystem::quad)
        .def("coords", &LPSystem::coords)
        ;
    regina::python::add_eq_operators(s);
}

