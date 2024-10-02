
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
#include "../pybind11/iostream.h"
#include "../pybind11/operators.h"
#include "angle/anglestructure.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/angle/anglestructure.h"

using namespace regina::python;
using regina::AngleStructure;

void addAngleStructure(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(AngleStructure)

    auto c = pybind11::class_<AngleStructure>(m, "AngleStructure", rdoc_scope)
        .def(pybind11::init<const AngleStructure&>(), rdoc::__copy)
        .def(pybind11::init<const AngleStructure&,
            const regina::Triangulation<3>&>(), rdoc::__init)
        .def(pybind11::init<const regina::Triangulation<3>&,
            const regina::Vector<regina::Integer>&>(), rdoc::__init_2)
        .def(pybind11::init<const regina::Triangulation<3>&,
            const regina::Vector<regina::LargeInteger>&>(), rdoc::__init_2)
        .def(pybind11::init([](const regina::Triangulation<3>& t,
                pybind11::list values) {
            regina::Vector<regina::Integer> v(3 * t.size() + 1);
            if (values.size() != v.size())
                throw pybind11::index_error(
                    "Incorrect number of angle coordinates");
            try {
                for (size_t i = 0; i < v.size(); ++i)
                    v[i] = values[i].cast<regina::Integer>();
            } catch (pybind11::cast_error const &) {
                throw regina::InvalidArgument(
                    "List element not convertible to Integer");
            }
            return new AngleStructure(t, std::move(v));
        }), rdoc::__init_2)
        .def("swap", &AngleStructure::swap, rdoc::swap)
        .def("angle", &AngleStructure::angle, rdoc::angle)
        .def("triangulation", &AngleStructure::triangulation,
            pybind11::return_value_policy::reference_internal,
            rdoc::triangulation)
        .def("isStrict", &AngleStructure::isStrict, rdoc::isStrict)
        .def("isTaut", &AngleStructure::isTaut, rdoc::isTaut)
        .def("isVeering", &AngleStructure::isVeering, rdoc::isVeering)
        .def("vector", &AngleStructure::vector,
            pybind11::return_value_policy::reference_internal, rdoc::vector)
        .def("writeXMLData", [](const AngleStructure& s,
                pybind11::object file) {
            pybind11::scoped_ostream_redirect stream(std::cout, file);
            s.writeXMLData(std::cout);
        }, rdoc::writeXMLData)
        .def(pybind11::self < pybind11::self, rdoc::__lt)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    regina::python::add_global_swap<AngleStructure>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

