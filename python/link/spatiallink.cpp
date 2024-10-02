
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
#include "link/spatiallink.h"
#include "../helpers.h"
#include "../docstrings/link/spatiallink.h"

using pybind11::overload_cast;
using regina::SpatialLink;

void addSpatialLink(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SpatialLink)

    auto l = pybind11::class_<SpatialLink, std::shared_ptr<SpatialLink>>(
            m, "SpatialLink", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const SpatialLink&>(), rdoc::__copy)
        .def(pybind11::init([](
                const std::vector<std::vector<std::array<double, 3>>>& nodes) {
            return new SpatialLink(nodes.begin(), nodes.end());
        }), pybind11::arg("components"), rdoc::__init)
        .def(pybind11::init([](
                const std::vector<std::vector<SpatialLink::Node>>& nodes) {
            return new SpatialLink(nodes.begin(), nodes.end());
        }), pybind11::arg("components"), rdoc::__init)
        .def("size", &SpatialLink::size, rdoc::size)
        .def("isEmpty", &SpatialLink::isEmpty, rdoc::isEmpty)
        .def("countComponents", &SpatialLink::countComponents,
            rdoc::countComponents)
        .def("component", &SpatialLink::component,
            pybind11::return_value_policy::reference_internal, rdoc::component)
        .def("components", &SpatialLink::components,
            pybind11::keep_alive<0, 1>(), rdoc::components)
        .def("componentSize", &SpatialLink::componentSize, rdoc::componentSize)
        .def("node", &SpatialLink::node,
            pybind11::return_value_policy::reference_internal, rdoc::node)
        .def("range", &SpatialLink::range, rdoc::range)
        .def("radius", &SpatialLink::radius, rdoc::radius)
        .def("setRadius", &SpatialLink::setRadius, rdoc::setRadius)
        .def("clearRadius", &SpatialLink::clearRadius, rdoc::clearRadius)
        .def("hasRadius", &SpatialLink::hasRadius, rdoc::hasRadius)
        .def("defaultRadius", &SpatialLink::defaultRadius, rdoc::defaultRadius)
        .def("swap", &SpatialLink::swap, rdoc::swap)
        .def("scale", &SpatialLink::scale, rdoc::scale)
        .def("translate", &SpatialLink::translate, rdoc::translate)
        .def("reflect", &SpatialLink::reflect,
            pybind11::arg("axis") = 2, rdoc::reflect)
        .def("refine", overload_cast<>(&SpatialLink::refine),
            rdoc::refine)
        .def("refine", overload_cast<int>(&SpatialLink::refine),
            rdoc::refine_2)
        .def_static("fromKnotPlot", &SpatialLink::fromKnotPlot,
            rdoc::fromKnotPlot)
    ;
    regina::python::add_output(l);
    regina::python::packet_eq_operators(l, rdoc::__eq);
    regina::python::add_packet_data(l);

    regina::python::addListView<decltype(SpatialLink().components())>(m);

    auto wrap = regina::python::add_packet_wrapper<SpatialLink>(m,
        "PacketOfSpatialLink");
    regina::python::add_packet_constructor<>(wrap, rdoc::__default);

    regina::python::add_global_swap<SpatialLink>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}
