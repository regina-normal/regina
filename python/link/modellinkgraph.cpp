
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
#include "link/modellinkgraph.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::ModelLinkGraphNode;
using regina::ModelLinkGraphArc;
using regina::ModelLinkGraph;
using regina::ModelLinkGraphCells;

void addModelLinkGraph(pybind11::module_& m) {
    auto a = pybind11::class_<ModelLinkGraphArc>(m, "ModelLinkGraphArc")
        .def(pybind11::init<>())
        .def(pybind11::init<ModelLinkGraphNode*, int>())
        .def(pybind11::init<const ModelLinkGraphArc&>())
        .def("node", &ModelLinkGraphArc::node,
            pybind11::return_value_policy::reference)
        .def("arc", &ModelLinkGraphArc::arc)
        .def("opposite", &ModelLinkGraphArc::opposite)
        .def("traverse", &ModelLinkGraphArc::traverse)
        .def("next", &ModelLinkGraphArc::next)
        .def("prev", &ModelLinkGraphArc::prev)
        .def("inc", [](ModelLinkGraphArc& a) {
           return a++;
        })
        .def("dec", [](ModelLinkGraphArc& a) {
           return a--;
        })
    ;
    regina::python::add_output_ostream(a, true /* __repr__ */);
    regina::python::add_eq_operators(a);

    auto n = pybind11::class_<ModelLinkGraphNode>(m, "ModelLinkGraphNode")
        .def("index", &ModelLinkGraphNode::index)
        .def("arc", &ModelLinkGraphNode::arc)
        .def("adj", &ModelLinkGraphNode::adj)
    ;
    regina::python::add_output(n);
    regina::python::add_eq_operators(n);

    auto g = pybind11::class_<ModelLinkGraph>(m, "ModelLinkGraph")
        .def(pybind11::init<>())
        .def(pybind11::init<const ModelLinkGraph&>())
        .def("size", &ModelLinkGraph::size)
        .def("node", &ModelLinkGraph::node,
            pybind11::return_value_policy::reference_internal)
        .def("swapContents", &ModelLinkGraph::swapContents)
        .def("reflect", &ModelLinkGraph::reflect)
        .def("findFlype", &ModelLinkGraph::findFlype)
        .def("flype", overload_cast<const ModelLinkGraphArc&,
                const ModelLinkGraphArc&, const ModelLinkGraphArc&>(
            &ModelLinkGraph::flype, pybind11::const_))
        .def("flype", overload_cast<const ModelLinkGraphArc&>(
            &ModelLinkGraph::flype, pybind11::const_))
        .def("plantri", &ModelLinkGraph::plantri)
        .def("canonicalPlantri", &ModelLinkGraph::canonicalPlantri,
            pybind11::arg("useReflection") = true,
            pybind11::arg("tight") = false)
        .def_static("fromPlantri", &ModelLinkGraph::fromPlantri)
    ;
    regina::python::add_output(g);
    regina::python::add_eq_operators(g);

    auto c = pybind11::class_<ModelLinkGraphCells>(m, "ModelLinkGraphCells")
        .def("isValid", &ModelLinkGraphCells::isValid)
        .def("countCells", &ModelLinkGraphCells::countCells)
        .def("size", &ModelLinkGraphCells::size)
        .def("arc", &ModelLinkGraphCells::arc)
        .def("cell", &ModelLinkGraphCells::cell)
        .def("cellPos", &ModelLinkGraphCells::cellPos)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);
}
