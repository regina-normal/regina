
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

using namespace boost::python;
using regina::Component;

namespace {
    boost::python::list triangles_list(Component<2>& t) {
        boost::python::list ans;
        for (auto s : t.triangles())
            ans.append(boost::python::ptr(s));
        return ans;
    }

    boost::python::list bc_list(Component<2>& t) {
        boost::python::list ans;
        for (auto s : t.boundaryComponents())
            ans.append(boost::python::ptr(s));
        return ans;
    }
}

void addComponent2() {
    class_<Component<2>, std::auto_ptr<Component<2>>, boost::noncopyable>
            ("Component2", no_init)
        .def("index", &Component<2>::index)
        .def("size", &Component<2>::size)
        .def("countTriangles", &Component<2>::countTriangles)
        .def("countFaces", &regina::python::countFaces<Component<2>, 2>)
        .def("countEdges", &Component<2>::countEdges)
        .def("countVertices", &Component<2>::countVertices)
        .def("countBoundaryComponents", &Component<2>::countBoundaryComponents)
        .def("simplices", triangles_list)
        .def("triangles", triangles_list)
        .def("faces", &regina::python::faces<Component<2>, 2>)
        .def("edges", regina::python::faces_list<Component<2>, 2, 1>)
        .def("vertices", regina::python::faces_list<Component<2>, 2, 0>)
        .def("boundaryComponents", bc_list)
        .def("triangle", &Component<2>::triangle,
            return_value_policy<reference_existing_object>())
        .def("simplex", &Component<2>::simplex,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<Component<2>, 2, size_t>)
        .def("edge", &Component<2>::edge,
            return_value_policy<reference_existing_object>())
        .def("vertex", &Component<2>::vertex,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Component<2>::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isValid", &Component<2>::isValid)
        .def("isOrientable", &Component<2>::isOrientable)
        .def("isClosed", &Component<2>::isClosed)
        .def("hasBoundaryFacets", &Component<2>::hasBoundaryFacets)
        .def("hasBoundaryEdges", &Component<2>::hasBoundaryEdges)
        .def("countBoundaryFacets", &Component<2>::countBoundaryFacets)
        .def("countBoundaryEdges", &Component<2>::countBoundaryEdges)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("Dim2Component") = scope().attr("Component2");
}

