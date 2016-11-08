
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../generic/facehelper.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::BoundaryComponent;

void addBoundaryComponent2() {
    {
        scope s = class_<BoundaryComponent<2>,
                std::auto_ptr<BoundaryComponent<2>>,
                boost::noncopyable> ("BoundaryComponent2", no_init)
            .def("index", &BoundaryComponent<2>::index)
            .def("size", &BoundaryComponent<2>::size)
            .def("countFaces",
                &regina::python::countFaces<BoundaryComponent<2>, 2>)
            .def("countEdges", &BoundaryComponent<2>::countEdges)
            .def("countVertices", &BoundaryComponent<2>::countVertices)
            .def("facets",
                regina::python::faces_list<BoundaryComponent<2>, 2, 1>)
            .def("faces", &regina::python::faces<BoundaryComponent<2>, 2>)
            .def("edges",
                regina::python::faces_list<BoundaryComponent<2>, 2, 1>)
            .def("vertices",
                regina::python::faces_list<BoundaryComponent<2>, 2, 0>)
            .def("facet", &BoundaryComponent<2>::facet,
                return_value_policy<reference_existing_object>())
            .def("face", &regina::python::face<BoundaryComponent<2>, 2, size_t>)
            .def("edge", &BoundaryComponent<2>::edge,
                return_value_policy<reference_existing_object>())
            .def("vertex", &BoundaryComponent<2>::vertex,
                return_value_policy<reference_existing_object>())
            .def("component", &BoundaryComponent<2>::component,
                return_value_policy<reference_existing_object>())
            .def("triangulation", &BoundaryComponent<2>::triangulation,
                return_value_policy<to_held_type<>>())
            .def("isOrientable", &BoundaryComponent<2>::isOrientable)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
        ;
        /*
         * If these bindings are enabled, we must use bool(...) on the RHS
         * to ensure that the values are not treated as references (since
         * these static class members are really just compile-time constants,
         * and are not defined in a way that gives them linkage).
        s.attr("allFaces") = bool(BoundaryComponent<2>::allFaces);
        s.attr("allowVertex") = bool(BoundaryComponent<2>::allowVertex);
        s.attr("canBuild") = bool(BoundaryComponent<2>::canBuild);
        */
    }

    scope().attr("Dim2BoundaryComponent") = scope().attr("BoundaryComponent2");
}

