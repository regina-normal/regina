
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
#include "../pybind11/stl.h"
#include "link/tangle.h"
#include "../helpers.h"
#include "../docstrings/link/tangle.h"

using pybind11::overload_cast;
using regina::Crossing;
using regina::StrandRef;
using regina::Tangle;

void addTangle(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Tangle)

    auto c = pybind11::class_<Tangle>(m, "Tangle", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<int>(), rdoc::__init)
        .def(pybind11::init<int, int>(), rdoc::__init_2)
        .def(pybind11::init<const regina::Link&>(), rdoc::__init_3)
        .def(pybind11::init<const Tangle&>(), rdoc::__copy)
        .def("type", &Tangle::type, rdoc::type)
        .def("size", &Tangle::size, rdoc::size)
        .def("crossing", &Tangle::crossing,
            pybind11::return_value_policy::reference_internal, rdoc::crossing)
        .def("crossings", &Tangle::crossings,
            pybind11::keep_alive<0, 1>(), rdoc::crossings)
        .def("begin", &Tangle::begin, rdoc::begin)
        .def("end", &Tangle::end, rdoc::end)
        .def("translate", &Tangle::translate, rdoc::translate)
        .def("swap", &Tangle::swap, rdoc::swap)
        .def("twist", &Tangle::twist,
            pybind11::arg("sign") = 1, rdoc::twist)
        .def("turn", &Tangle::turn,
            pybind11::arg("direction") = 1, rdoc::turn)
        .def("changeAll", &Tangle::changeAll, rdoc::changeAll)
        .def("add", &Tangle::add, rdoc::add)
        .def("box", &Tangle::box, rdoc::box)
        .def("negate", &Tangle::negate, rdoc::negate)
        .def("numClosure", &Tangle::numClosure, rdoc::numClosure)
        .def("denClosure", &Tangle::denClosure, rdoc::denClosure)
        .def("r1", &Tangle::r1,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::r1)
        .def("r2", overload_cast<StrandRef, bool, bool>(&Tangle::r2),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::r2)
        .def("r2", overload_cast<Crossing*, bool, bool>(&Tangle::r2),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::r2_2)
        .def("simplifyToLocalMinimum", &Tangle::simplifyToLocalMinimum,
            pybind11::arg("perform") = true, rdoc::simplifyToLocalMinimum)
        .def("brief", overload_cast<>(&Tangle::brief, pybind11::const_),
            rdoc::brief)
        .def("orientedGauss",
            overload_cast<>(&Tangle::orientedGauss, pybind11::const_),
            rdoc::orientedGauss)
        // In the following overloads, we define functions twice because
        // overload_cast gets confused between templated/non-templated variants.
        // Also: the versions that take a std::vector must come *last*,
        // since otherwise it treats func(x) as func([x]) never sees
        // the non-vector version.
        .def_static("fromOrientedGauss", [](const std::string& s) {
            return Tangle::fromOrientedGauss(s);
        }, rdoc::fromOrientedGauss)
        .def_static("fromOrientedGauss", [](const std::vector<std::string>& v) {
            return Tangle::fromOrientedGauss(v.begin(), v.end());
        }, pybind11::arg("tokens"), rdoc::fromOrientedGauss_2)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    // No need to register the ListView class for crossings(), since
    // this is the same class used by Link.

    regina::python::add_global_swap<Tangle>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}
