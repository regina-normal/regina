
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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
#include "../pybind11/stl.h"
#include "link/tangle.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Crossing;
using regina::StrandRef;
using regina::Tangle;

void addTangle(pybind11::module& m) {
    auto c = pybind11::class_<Tangle>(m, "Tangle")
        .def(pybind11::init<int>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<const regina::Link&>())
        .def(pybind11::init<const Tangle&>())
        .def("type", &Tangle::type)
        .def("size", &Tangle::size)
        .def("crossing", &Tangle::crossing,
            pybind11::return_value_policy::reference_internal)
        .def("begin", &Tangle::begin)
        .def("end", &Tangle::end)
        .def("translate", &Tangle::translate)
        .def("swapContents", &Tangle::swapContents)
        .def("twist", &Tangle::twist,
            pybind11::arg("sign") = 1)
        .def("turn", &Tangle::turn,
            pybind11::arg("direction") = 1)
        .def("changeAll", &Tangle::changeAll)
        .def("add", &Tangle::add)
        .def("box", &Tangle::box)
        .def("negate", &Tangle::negate)
        .def("numClosure", &Tangle::numClosure)
        .def("denClosure", &Tangle::denClosure)
        .def("r1", &Tangle::r1,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("r2", overload_cast<StrandRef, bool, bool>(&Tangle::r2),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("r2", overload_cast<Crossing*, bool, bool>(&Tangle::r2),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("simplifyToLocalMinimum", &Tangle::simplifyToLocalMinimum,
            pybind11::arg("perform") = true)
        .def("orientedGauss",
            overload_cast<>(&Tangle::orientedGauss, pybind11::const_))
        .def_static("fromOrientedGauss", [](const std::vector<std::string>& v) {
            return Tangle::fromOrientedGauss(v.begin(), v.end());
        })
        .def_static("fromOrientedGauss", [](const std::string& s) {
            // overload_cast confused with templated vs non-templated versions.
            return Tangle::fromOrientedGauss(s);
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);
}
