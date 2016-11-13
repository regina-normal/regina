
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
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using regina::Component;

namespace {
    boost::python::list simplices_list(Component<3>& t) {
        boost::python::list ans;
        for (std::vector<regina::Simplex<3>*>::const_iterator it =
                t.simplices().begin(); it != t.simplices().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list bc_list(Component<3>& t) {
        boost::python::list ans;
        for (auto s : t.boundaryComponents())
            ans.append(boost::python::ptr(s));
        return ans;
    }
}

void addComponent3() {
    class_<Component<3>, std::auto_ptr<Component<3>>, boost::noncopyable>
            ("Component3", no_init)
        .def("index", &Component<3>::index)
        .def("size", &Component<3>::size)
        .def("countTetrahedra", &Component<3>::countTetrahedra)
        .def("countFaces", &regina::python::countFaces<Component<3>, 3>)
        .def("countTriangles", &Component<3>::countTriangles)
        .def("countEdges", &Component<3>::countEdges)
        .def("countVertices", &Component<3>::countVertices)
        .def("countBoundaryComponents", &Component<3>::countBoundaryComponents)
        .def("simplices", simplices_list)
        .def("tetrahedra", simplices_list)
        .def("simplex", &Component<3>::simplex,
            return_value_policy<reference_existing_object>())
        .def("tetrahedron", &Component<3>::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("faces", &regina::python::faces<Component<3>, 3>)
        .def("triangles", regina::python::faces_list<Component<3>, 3, 2>)
        .def("edges", regina::python::faces_list<Component<3>, 3, 1>)
        .def("vertices", regina::python::faces_list<Component<3>, 3, 0>)
        .def("boundaryComponents", bc_list)
        .def("face", &regina::python::face<Component<3>, 3, size_t>)
        .def("triangle", &Component<3>::triangle,
            return_value_policy<reference_existing_object>())
        .def("edge", &Component<3>::edge,
            return_value_policy<reference_existing_object>())
        .def("vertex", &Component<3>::vertex,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Component<3>::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isIdeal", &Component<3>::isIdeal)
        .def("isValid", &Component<3>::isValid)
        .def("isOrientable", &Component<3>::isOrientable)
        .def("isClosed", &Component<3>::isClosed)
        .def("hasBoundaryFacets", &Component<3>::hasBoundaryFacets)
        .def("hasBoundaryTriangles", &Component<3>::hasBoundaryTriangles)
        .def("countBoundaryFacets", &Component<3>::countBoundaryFacets)
        .def("countBoundaryTriangles", &Component<3>::countBoundaryTriangles)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NComponent") = scope().attr("Component3");
}

