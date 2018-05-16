
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

#include "link/modellinkgraph.h"
#include "../safeheldtype.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>
#include "../helpers.h"

using namespace boost::python;
using namespace regina::python;
using regina::ModelLinkGraphNode;
using regina::ModelLinkGraphArc;
using regina::ModelLinkGraph;
using regina::ModelLinkGraphCells;

namespace {
    ModelLinkGraph* (ModelLinkGraph::*flype1)(const ModelLinkGraphArc&) const =
        &ModelLinkGraph::flype;
    ModelLinkGraph* (ModelLinkGraph::*flype3)(const ModelLinkGraphArc&,
        const ModelLinkGraphArc&, const ModelLinkGraphArc&) const =
        &ModelLinkGraph::flype;

    void arc_inc_operator(ModelLinkGraphArc& a) {
       ++a;
    }

    void arc_dec_operator(ModelLinkGraphArc& a) {
       --a;
    }

    boost::python::tuple findFlype_tuple(const ModelLinkGraph& g,
            const ModelLinkGraphArc& a) {
        auto ans = g.findFlype(a);
        return boost::python::make_tuple(ans.first, ans.second);
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_canonicalPlantri,
        ModelLinkGraph::canonicalPlantri, 0, 2);
}

void addModelLinkGraph() {
    class_<ModelLinkGraphArc>("ModelLinkGraphArc", init<>())
        .def(init<ModelLinkGraphNode*, int>())
        .def(init<const ModelLinkGraphArc&>())
        .def("node", &ModelLinkGraphArc::node,
            return_value_policy<reference_existing_object>())
        .def("arc", &ModelLinkGraphArc::arc)
        .def("opposite", &ModelLinkGraphArc::opposite)
        .def("traverse", &ModelLinkGraphArc::traverse)
        .def("next", &ModelLinkGraphArc::next)
        .def("prev", &ModelLinkGraphArc::prev)
        .def("inc", arc_inc_operator)
        .def("dec", arc_dec_operator)
        .def(self_ns::str(self))
        .def(self_ns::repr(self))
        .def(regina::python::add_eq_operators())
    ;

    class_<ModelLinkGraphNode, std::auto_ptr<ModelLinkGraphNode>,
            boost::noncopyable>("ModelLinkGraphNode", no_init)
        .def("index", &ModelLinkGraphNode::index)
        .def("arc", &ModelLinkGraphNode::arc)
        .def("adj", &ModelLinkGraphNode::adj,
            return_value_policy<return_by_value>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<ModelLinkGraph, std::auto_ptr<ModelLinkGraph>,
            boost::noncopyable>("ModelLinkGraph", init<>())
        .def(init<const ModelLinkGraph&>())
        .def("size", &ModelLinkGraph::size)
        .def("node", &ModelLinkGraph::node, return_internal_reference<>())
        .def("swapContents", &ModelLinkGraph::swapContents)
        .def("reflect", &ModelLinkGraph::reflect)
        .def("findFlype", findFlype_tuple)
        .def("flype", flype3, return_value_policy<manage_new_object>())
        .def("flype", flype1, return_value_policy<manage_new_object>())
        .def("plantri", &ModelLinkGraph::plantri)
        .def("canonicalPlantri", &ModelLinkGraph::canonicalPlantri,
            OL_canonicalPlantri())
        .def("fromPlantri", &ModelLinkGraph::fromPlantri,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("fromPlantri")
    ;

    class_<ModelLinkGraphCells, std::auto_ptr<ModelLinkGraphCells>,
            boost::noncopyable>("ModelLinkGraphCells", no_init)
        .def("isValid", &ModelLinkGraphCells::isValid)
        .def("countCells", &ModelLinkGraphCells::countCells)
        .def("size", &ModelLinkGraphCells::size)
        .def("arc", &ModelLinkGraphNode::arc,
            return_value_policy<return_by_value>())
        .def("cell", &ModelLinkGraphCells::cell)
        .def("cellPos", &ModelLinkGraphCells::cellPos)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}
