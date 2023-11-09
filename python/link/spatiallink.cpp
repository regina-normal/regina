
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
        .def("size", &SpatialLink::size, rdoc::size)
        .def("isEmpty", &SpatialLink::isEmpty, rdoc::isEmpty)
        .def("countComponents", &SpatialLink::countComponents,
            rdoc::countComponents)
        .def("component", overload_cast<size_t>(&SpatialLink::component),
            pybind11::return_value_policy::reference_internal, rdoc::component)
        .def("components", &SpatialLink::components,
            pybind11::keep_alive<0, 1>(), rdoc::components)
        .def("swap", &SpatialLink::swap, rdoc::swap)
        .def_static("fromKnotPlot", &SpatialLink::fromKnotPlot,
            rdoc::fromKnotPlot)
    ;
    regina::python::add_output(l);
    regina::python::packet_eq_operators(l, rdoc::__eq, rdoc::__ne);
    regina::python::add_packet_data(l);

    RDOC_SCOPE_INNER_BEGIN(Node)

    auto n = pybind11::class_<SpatialLink::Node>(l, "Node", rdoc_inner_scope)
        .def(pybind11::init<>(), rdoc_inner::__default)
        .def(pybind11::init<const SpatialLink::Node&>(), rdoc_inner::__copy)
        .def(pybind11::init<double, double, double>(), rdoc_inner::__init)
        .def_readwrite("x", &SpatialLink::Node::x, rdoc_inner::x)
        .def_readwrite("y", &SpatialLink::Node::y, rdoc_inner::y)
        .def_readwrite("z", &SpatialLink::Node::z, rdoc_inner::z)
    ;
    regina::python::add_output_ostream(n);
    regina::python::add_eq_operators(n, rdoc_inner::__eq, rdoc_inner::__ne);

    RDOC_SCOPE_INNER_END

    regina::python::addListView<decltype(SpatialLink().components())>(m);

    auto wrap = regina::python::add_packet_wrapper<SpatialLink>(m,
        "PacketOfSpatialLink");
    regina::python::add_packet_constructor<>(wrap, rdoc::__default);

    regina::python::add_global_swap<SpatialLink>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}