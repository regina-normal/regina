
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include "link/modellinkgraph.h"
#include "../helpers.h"
#include "../docstrings/link/modellinkgraph.h"

using pybind11::overload_cast;
using regina::ModelLinkGraphNode;
using regina::ModelLinkGraphArc;
using regina::ModelLinkGraph;
using regina::ModelLinkGraphCells;

void addModelLinkGraph(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(ModelLinkGraphArc)

    auto a = pybind11::class_<ModelLinkGraphArc>(m, "ModelLinkGraphArc",
            rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<ModelLinkGraphNode*, int>(), rdoc::__init)
        .def(pybind11::init<const ModelLinkGraphArc&>(), rdoc::__copy)
        .def("node", &ModelLinkGraphArc::node,
            pybind11::return_value_policy::reference, rdoc::node)
        .def("arc", &ModelLinkGraphArc::arc, rdoc::arc)
        .def("opposite", &ModelLinkGraphArc::opposite, rdoc::opposite)
        .def("traverse", &ModelLinkGraphArc::traverse, rdoc::traverse)
        .def("next", &ModelLinkGraphArc::next, rdoc::next)
        .def("prev", &ModelLinkGraphArc::prev, rdoc::prev)
        .def("inc", [](ModelLinkGraphArc& a) {
           return a++;
        }, rdoc::__inc)
        .def("dec", [](ModelLinkGraphArc& a) {
           return a--;
        }, rdoc::__dec)
        .def("__bool__", &ModelLinkGraphArc::operator bool, rdoc::__as_bool)
    ;
    regina::python::add_output_ostream(a);
    regina::python::add_eq_operators(a, rdoc::__eq);

    RDOC_SCOPE_SWITCH(ModelLinkGraphNode)

    auto n = pybind11::class_<ModelLinkGraphNode>(m, "ModelLinkGraphNode",
            rdoc_scope)
        .def("index", &ModelLinkGraphNode::index, rdoc::index)
        .def("arc", &ModelLinkGraphNode::arc, rdoc::arc)
        .def("adj", &ModelLinkGraphNode::adj, rdoc::adj)
        .def("loops", &ModelLinkGraphNode::loops, rdoc::loops)
        .def("bigons", &ModelLinkGraphNode::bigons, rdoc::bigons)
        .def("triangles", &ModelLinkGraphNode::triangles, rdoc::triangles)
    ;
    regina::python::add_output(n);
    regina::python::add_eq_operators(n);

    RDOC_SCOPE_SWITCH(ModelLinkGraph)

    auto g = pybind11::class_<ModelLinkGraph>(m, "ModelLinkGraph", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const regina::Link&>(), rdoc::__init)
        .def(pybind11::init<const ModelLinkGraph&>(), rdoc::__copy)
        .def("size", &ModelLinkGraph::size, rdoc::size)
        .def("node", &ModelLinkGraph::node,
            pybind11::return_value_policy::reference_internal, rdoc::node)
        .def("nodes", &ModelLinkGraph::nodes,
            pybind11::keep_alive<0, 1>(), rdoc::nodes)
        .def("swap", &ModelLinkGraph::swap, rdoc::swap)
        .def("reflect", &ModelLinkGraph::reflect, rdoc::reflect)
        .def("isConnected", &ModelLinkGraph::isConnected, rdoc::isConnected)
        .def("isSimple", &ModelLinkGraph::isSimple, rdoc::isSimple)
        .def("cells", &ModelLinkGraph::cells,
            pybind11::return_value_policy::reference_internal, rdoc::cells)
        .def("findFlype", &ModelLinkGraph::findFlype, rdoc::findFlype)
        .def("flype", overload_cast<const ModelLinkGraphArc&,
                const ModelLinkGraphArc&, const ModelLinkGraphArc&>(
            &ModelLinkGraph::flype, pybind11::const_), rdoc::flype)
        .def("flype", overload_cast<const ModelLinkGraphArc&>(
            &ModelLinkGraph::flype, pybind11::const_), rdoc::flype_2)
        .def("plantri", &ModelLinkGraph::plantri, rdoc::plantri)
        .def("canonicalPlantri", &ModelLinkGraph::canonicalPlantri,
            pybind11::arg("useReflection") = true,
            pybind11::arg("tight") = false,
            rdoc::canonicalPlantri)
        .def_static("fromPlantri", &ModelLinkGraph::fromPlantri,
            rdoc::fromPlantri)
        .def("generateAnyLink", &ModelLinkGraph::generateAnyLink,
            rdoc::generateAnyLink)
        .def("generateMinimalLinks", &ModelLinkGraph::generateMinimalLinks<
            const std::function<void(regina::Link&&)>&>,
            pybind11::arg("action"), rdoc::generateMinimalLinks)
    ;
    regina::python::add_output(g);
    regina::python::add_eq_operators(g, rdoc::__eq);

    regina::python::add_global_swap<ModelLinkGraph>(m, rdoc::global_swap);

    regina::python::addListView<decltype(ModelLinkGraph().nodes())>(m);

    RDOC_SCOPE_SWITCH(ModelLinkGraphCells)

    auto c = pybind11::class_<ModelLinkGraphCells>(m, "ModelLinkGraphCells",
            rdoc_scope)
        .def("isValid", &ModelLinkGraphCells::isValid, rdoc::isValid)
        .def("countCells", &ModelLinkGraphCells::countCells, rdoc::countCells)
        .def("size", &ModelLinkGraphCells::size, rdoc::size)
        .def("arc", &ModelLinkGraphCells::arc, rdoc::arc)
        .def("arcs", &ModelLinkGraphCells::arcs, rdoc::arcs)
        .def("cell", &ModelLinkGraphCells::cell, rdoc::cell)
        .def("cellPos", &ModelLinkGraphCells::cellPos, rdoc::cellPos)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    regina::python::addListView<decltype(ModelLinkGraph().cells().arcs(0))>(m);

    RDOC_SCOPE_END
}
